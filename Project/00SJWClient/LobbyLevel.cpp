#include "pch.h"
#include "LobbyLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>
#include <Engine\CFSM.h>

#include <Engine\CMesh.h>
#include <Engine\CGraphicsShader.h>
#include <Engine\CTexture.h>
#include <Engine\CSetColorShader.h>

#include "CLevelSaveLoad.h"

void LobbyLevel::Init()
{
}

void LobbyLevel::CreateTempLevel()
{
	CLevel* pTempLevel = new CLevel;

	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Light");
	pTempLevel->GetLayer(2)->SetName(L"Background");
	pTempLevel->GetLayer(3)->SetName(L"Gamer");

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

	// 로비 하우스
	CGameObject* house = new CGameObject;
	house->SetName(L"House");
	house->AddComponent(new CTransform);
	house->AddComponent(new CMeshRender);

	house->Transform()->SetRelativePos(Vec3(0.f, 0.f, 300.f));
	house->Transform()->SetRelativeScale(Vec3(167.f * 3.f, 128.f * 3.f, 1.f));

	house->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	house->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HouseMtrl"));
	house->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\house_bg_custom.png", L"texture\\Lobby\\house_bg_custom.png");
	house->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(house, 2);

	// 로비 배경(하늘)
	CGameObject* LobbySky = new CGameObject;
	LobbySky->SetName(L"LobbySky");
	LobbySky->AddComponent(new CTransform);
	LobbySky->AddComponent(new CMeshRender);

	LobbySky->Transform()->SetRelativePos(Vec3(0.f, 145.f, 500.f));
	LobbySky->Transform()->SetRelativeScale(Vec3(1280.f, 768.f, 1.f));

	LobbySky->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	LobbySky->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbySkyMtrl"));
	LobbySky->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\sky_day.png", L"texture\\Lobby\\sky_day.png");
	LobbySky->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(LobbySky, 2);
	
	// 로비 배경(땅)
	CGameObject* LobbyGround = new CGameObject;
	LobbyGround->SetName(L"LobbyGround");
	LobbyGround->AddComponent(new CTransform);
	LobbyGround->AddComponent(new CMeshRender);

	LobbyGround->Transform()->SetRelativePos(Vec3(0.f, -288.f, 300.f));
	LobbyGround->Transform()->SetRelativeScale(Vec3(480.f * 3.f, 64.f * 3.f, 1.f));

	LobbyGround->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	LobbyGround->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyGroundMtrl"));
	LobbyGround->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\ground.png", L"texture\\Lobby\\ground.png");
	LobbyGround->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(LobbyGround, 2);

	// 레벨 플레이
	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);
}
