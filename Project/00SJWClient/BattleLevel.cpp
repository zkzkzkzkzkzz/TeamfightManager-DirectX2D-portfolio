#include "pch.h"
#include "BattleLevel.h"

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
#include <Engine\CFontMgr.h>

#include "CLevelSaveLoad.h"


#include <Scripts\CTGMgr.h>
#include <Scripts\CChampScript.h>
#include <Scripts\CArcherScript.h>


void BattleLevel::CreateTempLevel()
{
	CLevel* pTempLevel = new CLevel;

	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Light");
	pTempLevel->GetLayer(2)->SetName(L"Background");
	pTempLevel->GetLayer(3)->SetName(L"Champ");
	pTempLevel->GetLayer(4)->SetName(L"Cursor");
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
	pCamObj->Camera()->LayerCheck(2, false);
	pCamObj->Camera()->LayerCheck(3, false);
	pTempLevel->AddObject(pCamObj, 0);

	// BG Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"BGCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(2, true);
	pTempLevel->AddObject(pCamObj, 0);

	// Champ Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"ChampCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(2);
	pCamObj->Camera()->LayerCheck(3, true);
	pTempLevel->AddObject(pCamObj, 0);

	// 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light2D");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(1.f, 1.f, 1.f));

	pTempLevel->AddObject(pLight, 1);


	CGameObject* Stadium = new CGameObject;
	Stadium->SetName(L"Stadium");
	Stadium->AddComponent(new CTransform);
	Stadium->AddComponent(new CMeshRender);
	Stadium->Transform()->SetRelativePos(Vec3(0.f, 0.f, 400.f));
	Stadium->Transform()->SetRelativeScale(Vec3(1427.f, 1029.f, 1.f));
	Stadium->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Stadium->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StadiumMtrl"));
	Stadium->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\stadium.png",
															L"texture\\Champ\\stadium.png");
 	Stadium->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(Stadium, 2);

	CGameObject* Archer = new CGameObject;
	Archer->SetName(L"Archer");
	Archer->AddComponent(new CTransform);
	Archer->AddComponent(new CMeshRender);
	Archer->AddComponent(new CCollider2D);
	Archer->AddComponent(new CAnimator2D);
	Archer->AddComponent(new CArcherScript);
	Archer->GetScript<CArcherScript>()->SetTeam(TEAM::BLUE);
	pTempLevel->AddObject(Archer, 3);

	Archer = Archer->Clone();
	Archer->SetName(L"Enemy");
	Archer->Transform()->SetRelativePos(Vec3(290.f, 0.f, 300.f));
	Archer->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
	Archer->GetScript<CArcherScript>()->SetTeam(TEAM::RED);
	pTempLevel->AddObject(Archer, 3);

	// 레벨 플레이
	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);
}
