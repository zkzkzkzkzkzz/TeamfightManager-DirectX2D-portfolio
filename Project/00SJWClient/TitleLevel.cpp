#include "pch.h"
#include "TitleLevel.h"

#include <Engine\CCollisionMgr.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

#include <Engine\CMesh.h>
#include <Engine\CGraphicsShader.h>
#include <Engine\CTexture.h>
#include <Engine\CSetColorShader.h>

#include <Scripts\CNewBtnScript.h>
#include <Scripts\CEndBtnScript.h>

#include "CLevelSaveLoad.h"

void TitleLevel::Init()
{
}

void TitleLevel::CreateTempLevel()
{
	CLevel* pTempLevel = new CLevel;

	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Light");
	pTempLevel->GetLayer(2)->SetName(L"Background");

	pTempLevel->GetLayer(31)->SetName(L"UI");

	// Main Camera 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(31, false);

	pTempLevel->AddObject(pCamObj, 0);

	// UI Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(31, true);

	pTempLevel->AddObject(pCamObj, 0);

	// 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light2D");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(0.8f, 0.8f, 0.8f));

	pTempLevel->AddObject(pLight, 1);

	// Sky 추가
	CGameObject* pBG = new CGameObject;
	pBG->SetName(L"TitleSky");
	pBG->AddComponent(new CTransform);
	pBG->AddComponent(new CMeshRender);

	pBG->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pBG->Transform()->SetRelativeScale(Vec3(1088.f, 752.f, 1.f));

	pBG->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBG->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyMtrl"));
	pBG->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\sky.png", L"texture\\Title\\sky.png");
	pBG->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(pBG, 2);

	// Stadium 추가
	pBG = new CGameObject;
	pBG->SetName(L"TitleStadium");
	pBG->AddComponent(new CTransform);
	pBG->AddComponent(new CMeshRender);

	pBG->Transform()->SetRelativePos(Vec3(0.f, 0.f, 400.f));
	pBG->Transform()->SetRelativeScale(Vec3(1427.f, 1029.f, 1.f));

	pBG->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBG->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StadiumMtrl"));
	pBG->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\stadium.png", L"texture\\Title\\stadium.png");
	pBG->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(pBG, 2);

	// Background 추가
	pBG = new CGameObject;
	pBG->SetName(L"TitleBG");
	pBG->AddComponent(new CTransform);
	pBG->AddComponent(new CMeshRender);

	pBG->Transform()->SetRelativePos(Vec3(0.f, 0.f, 300.f));
	pBG->Transform()->SetRelativeScale(Vec3(1280.f, 768.f, 1.f));

	pBG->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBG->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl"));
	pBG->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\titleBG.png", L"texture\\Title\\titleBG.png");
	pBG->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(pBG, 2);

	// Title 추가
	pBG = new CGameObject;
	pBG->SetName(L"Title");
	pBG->AddComponent(new CTransform);
	pBG->AddComponent(new CMeshRender);

	pBG->Transform()->SetRelativePos(Vec3(0.f, 203.f, 300.f));
	pBG->Transform()->SetRelativeScale(Vec3(550.f, 205.f, 1.f));

	pBG->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBG->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TitleMtrl"));
	pBG->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\title.png", L"texture\\Title\\title.png");
	pBG->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(pBG, 2);

	// 새 게임 버튼
	CGameObject* NewGameBtn = new CGameObject;
	NewGameBtn->SetName(L"NewGameBtn");
	NewGameBtn->AddComponent(new CTransform);
	NewGameBtn->AddComponent(new CMeshRender);
	NewGameBtn->AddComponent(new CNewBtnScript);

	NewGameBtn->Transform()->SetRelativePos(Vec3(0.f, 32.f, 100.f));
	NewGameBtn->Transform()->SetRelativeScale(Vec3(66.f, 23.f, 1.f));

	NewGameBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	NewGameBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NewGameMtrl"));
	NewGameBtn->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_NewGame_Idle.png", L"texture\\Title\\Title_NewGame_Idle.png");
	NewGameBtn->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(NewGameBtn, 2);

	// 종료하기 버튼
	CGameObject* EndGameBtn = new CGameObject;
	EndGameBtn->SetName(L"EndGameBtn");
	EndGameBtn->AddComponent(new CTransform);
	EndGameBtn->AddComponent(new CMeshRender);
	EndGameBtn->AddComponent(new CEndBtnScript);

	EndGameBtn->Transform()->SetRelativePos(Vec3(0.f, -16.f, 100.f));
	EndGameBtn->Transform()->SetRelativeScale(Vec3(78.f, 23.f, 1.f));

	EndGameBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	EndGameBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EndGameMtrl"));
	EndGameBtn->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_Exit_Idle.png", L"texture\\Title\\Title_Exit_Idle.png");
	EndGameBtn->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(EndGameBtn, 2);

	// 레벨 플레이
	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);
}
