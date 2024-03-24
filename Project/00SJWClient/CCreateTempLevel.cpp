#include "pch.h"
#include "CCreateTempLevel.h"

#include <Engine\CCollisionMgr.h>

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

#include <Engine\CScript.h>
#include <Scripts\CNewBtnScript.h>
#include <Scripts\CEndBtnScript.h>

#include "TitleLevel.h"
#include "LobbyLevel.h"


void TestFunc();
INT_PTR CALLBACK CreateTempLevel();

void CCreateTempLevel::Init()
{	

}

void CCreateTempLevel::CreateTempLevel()
{
	//Ptr<CMaterial> pSkyMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyMtrl"); 
	//Ptr<CMaterial> pStadiumMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StadiumMtrl");
	//Ptr<CMaterial> pBackgroudMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl");
	//Ptr<CMaterial> pTitleMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TitleMtrl");
	//Ptr<CMaterial> pNGBtnMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NewGameMtrl");
	//Ptr<CMaterial> pEGBtnMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EndGameMtrl");

	//pSkyMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\sky.png", L"texture\\Title\\sky.png"));
	//pStadiumMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\stadium.png", L"texture\\Title\\stadium.png"));
	//pBackgroudMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\titleBG.png", L"texture\\Title\\titleBG.png"));
	//pTitleMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\title.png", L"texture\\Title\\title.png"));
	//pNGBtnMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_NewGame_Idle.png", L"texture\\Title\\Title_NewGame_Idle.png"));
	//pEGBtnMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_Exit_Idle.png", L"texture\\Title\\Title_Exit_Idle.png"));

	//CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\TitleLV1.lv");
	//pLevel->FindObjectByName(L"NewGameBtn")->GetScript<CNewBtnScript>()->SetCallBack(TestFunc);

	//CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);

	//TitleLevel::CreateTempLevel();
	LobbyLevel::CreateTempLevel();
}

void TestFunc()
{
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HouseMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0,
					CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\house_bg_custom.png", L"texture\\Lobby\\house_bg_custom.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbySkyMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0,
					CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\sky_day.png", L"texture\\Lobby\\sky_day.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyGroundMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0,
					CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\ground.png", L"texture\\Lobby\\ground.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, 
					CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_bg.png", L"texture\\Lobby\\header\\header_bg.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderSlotMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, 
					CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_slot_bg.png", L"texture\\Lobby\\header\\header_slot_bg.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CalenderMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0,
					CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_calendar_icon.png", L"texture\\Lobby\\header\\header_calendar_icon.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CoinMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, 
					CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_gold_icon.png", L"texture\\Lobby\\header\\header_gold_icon.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LogoMtrl");
	pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\captains.png", L"texture\\Lobby\\header\\captains.png"));
	pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\league_icon_custom.png", L"texture\\Lobby\\league_icon_custom.png"));

	//Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"IconMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\house_bg_custom.png", L"texture\\Lobby\\house_bg_custom.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TextUIMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\notification_bg_header.png", L"texture\\Lobby\\header\\notification_bg_header.png"));

	pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyBtnMtrl");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png"));

	//Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProceedBtnMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\house_bg_custom.png", L"texture\\Lobby\\house_bg_custom.png"));

	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\LobbyLV2.lv");
	CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);
}


//{
//	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\temp.lv");
//	CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);
//	return;
//
//	 초기 레벨 구성
//	CLevel* pTempLevel = new CLevel;
//
//	pTempLevel->GetLayer(0)->SetName(L"Default");
//	pTempLevel->GetLayer(1)->SetName(L"Background");
//	pTempLevel->GetLayer(2)->SetName(L"Tile");
//	pTempLevel->GetLayer(3)->SetName(L"Player");
//	pTempLevel->GetLayer(4)->SetName(L"Monster");
//	pTempLevel->GetLayer(5)->SetName(L"Light");
//
//	pTempLevel->GetLayer(31)->SetName(L"UI");
//
//	 충돌 설정
//	CCollisionMgr::GetInst()->LayerCheck(3, 4);
//	CCollisionMgr::GetInst()->LayerCheck(4, 4);
//
//	 Main Camera 생성
//	CGameObject* pCamObj = new CGameObject;
//	pCamObj->SetName(L"MainCamera");
//	pCamObj->AddComponent(new CTransform);
//	pCamObj->AddComponent(new CCamera);
//
//	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
//	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
//
//	pCamObj->Camera()->SetCameraPriority(0);
//	pCamObj->Camera()->LayerCheckAll();
//	 '메인 카메라에 UI는 그리지 마라'라는 뜻
//	 즉, 메인카메라가 움직여도 UI에 해당하는 부분은 영향받지 않는다
//	pCamObj->Camera()->LayerCheck(31, false);
//
//	pTempLevel->AddObject(pCamObj, 0);
//
//	 UI Camera 생성
//	pCamObj = new CGameObject;
//	pCamObj->SetName(L"UICamera");
//	pCamObj->AddComponent(new CTransform);
//	pCamObj->AddComponent(new CCamera);
//
//	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
//	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
//
//	pCamObj->Camera()->SetCameraPriority(1);
//	pCamObj->Camera()->LayerCheck(31, true);
//
//	pTempLevel->AddObject(pCamObj, 0);
//
//	 광원 추가
//	CGameObject* pLight = new CGameObject;
//	pLight->SetName(L"Light2D");
//	pLight->AddComponent(new CTransform);
//	pLight->AddComponent(new CLight2D);
//
//	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
//	pLight->Light2D()->SetAmbient(Vec3(0.8f, 0.8f, 0.8f));
//
//	pTempLevel->AddObject(pLight, 5);
//
//	 Player 생성
//	CGameObject* pObj = nullptr;
//	pObj = new CGameObject;
//	pObj->SetName(L"PlayerObj");
//
//	pObj->AddComponent(new CTransform);
//	pObj->AddComponent(new CMeshRender);
//	pObj->AddComponent(new CCollider2D);
//	pObj->AddComponent(new CAnimator2D);
//
//	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 300.f));
//	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
//
//	pObj->Collider2D()->SetAbsolute(true);
//	pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
//	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, -20.f));
//
//	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
//	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
//	pObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
//
//	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\link.png", L"texture\\link.png");
//	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
//
//	 Particle  생성
//	CGameObject* pParticleObj = new CGameObject;
//	pParticleObj->SetName(L"Particle");
//	pParticleObj->AddComponent(new CTransform);
//	pParticleObj->AddComponent(new CParticleSystem);
//	pParticleObj->Transform()->SetRelativePos(Vec3(100.f, 0.f, 200.f));
//	pObj->AddChild(pParticleObj);
//
//	pTempLevel->AddObject(pObj, 3, false);
//
//	pObj = pObj->Clone();
//	pObj->Transform()->SetRelativePos(Vec3(-500.f, 0.f, 500.f));
//	pTempLevel->AddObject(pObj, 3, false);
//
//	 background  생성
//	CGameObject* pBObj = new CGameObject;
//	pBObj->SetName(L"Background");
//
//	pBObj->AddComponent(new CTransform);
//	pBObj->AddComponent(new CMeshRender);
//
//	pBObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
//	pBObj->Transform()->SetRelativeScale(Vec3(1800.f, 800.f, 1.f));
//
//	pBObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
//	pBObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl"));
//	pBObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
//
//	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Background.jpg", L"texture\\Background.jpg");
//	pBObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
//
//	pTempLevel->AddObject(pBObj, 1, false);
//
//	 몬스터 생성
//	pObj = new CGameObject;
//	pObj->SetName(L"MonsterObj");
//
//	pObj->AddComponent(new CTransform);
//	pObj->AddComponent(new CMeshRender);
//	pObj->AddComponent(new CCollider2D);
//	pObj->AddComponent(new CStateMachine);
//
//	pObj->Transform()->SetRelativePos(Vec3(500.f, 0.f, 500.f));
//	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
//
//	pObj->Collider2D()->SetAbsolute(true);
//	pObj->Collider2D()->SetOffsetScale(Vec2(120.f, 120.f));
//	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
//
//	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
//	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
//	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Fighter.bmp", L"texture\\Fighter.bmp"));
//
//	pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"NormalMonsterFSM"));
//
//	pTempLevel->AddObject(pObj, L"Monster", false);
//
//	 레벨 플레이
//	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);
//
//	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");
//}
