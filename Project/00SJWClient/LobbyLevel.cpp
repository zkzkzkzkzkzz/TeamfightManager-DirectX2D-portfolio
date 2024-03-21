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

	house->Transform()->SetRelativePos(Vec3(0.f, -32.f, 300.f));
	house->Transform()->SetRelativeScale(Vec3(167.f * 2.5f, 128.f * 2.5f, 1.f));

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

	LobbySky->Transform()->SetRelativePos(Vec3(0.f, 96.f, 500.f));
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

	// 헤더
	CGameObject* Header = new CGameObject;
	Header->SetName(L"Header");
	Header->AddComponent(new CTransform);
	Header->AddComponent(new CMeshRender);

	Header->Transform()->SetRelativePos(Vec3(0.f, 330.f, 300.f));
	Header->Transform()->SetRelativeScale(Vec3(1280.f, 74.f, 1.f));

	Header->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Header->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderMtrl"));
	Header->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_bg.png", L"texture\\Lobby\\header\\header_bg.png");
	Header->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	
	// 텍스트UI 슬롯
	CGameObject* TextUI = new CGameObject;
	TextUI->SetName(L"TextUI");
	TextUI->AddComponent(new CTransform);
	TextUI->AddComponent(new CMeshRender);

	TextUI->Transform()->SetRelativePos(Vec3(-506.f, -54.f, 10.f));
	TextUI->Transform()->SetRelativeScale(Vec3(272.f, 48.f, 1.f));

	TextUI->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	TextUI->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TextUIMtrl"));
	TextUI->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\notification_bg_header.png", L"texture\\Lobby\\header\\notification_bg_header.png");
	TextUI->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	Header->AddChild(TextUI);

	// 헤더 슬롯
	CGameObject* HeaderSlot = new CGameObject;
	HeaderSlot->SetName(L"HeaderSlot");
	HeaderSlot->AddComponent(new CTransform);
	HeaderSlot->AddComponent(new CMeshRender);

	HeaderSlot->Transform()->SetRelativePos(Vec3(520.f, 0.f, -10.f));
	HeaderSlot->Transform()->SetRelativeScale(Vec3(226.f, 48.f, 1.f));

	HeaderSlot->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	HeaderSlot->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderSlotMtrl"));
	HeaderSlot->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
	
	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_slot_bg.png", L"texture\\Lobby\\header\\header_slot_bg.png");
	HeaderSlot->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	Header->AddChild(HeaderSlot);

	// 달력 아이콘
	CGameObject* Coin = new CGameObject;
	Coin->SetName(L"Coin");
	Coin->AddComponent(new CTransform);
	Coin->AddComponent(new CMeshRender);

	Coin->Transform()->SetRelativePos(Vec3(-91.f, 0.f, -10.f));
	Coin->Transform()->SetRelativeScale(Vec3(28.f, 28.f, 1.f));
	
	Coin->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Coin->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CoinMtrl"));
	Coin->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_gold_icon.png", L"texture\\Lobby\\header\\header_gold_icon.png");
	Coin->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	HeaderSlot->AddChild(Coin);

	// 헤더 슬롯 2
	CGameObject* HeaderSlot2 = new CGameObject;
	HeaderSlot2->SetName(L"HeaderSlot2");
	HeaderSlot2->AddComponent(new CTransform);
	HeaderSlot2->AddComponent(new CMeshRender);

	HeaderSlot2->Transform()->SetRelativePos(Vec3(285.f, 0.f, -10.f));
	HeaderSlot2->Transform()->SetRelativeScale(Vec3(226.f, 48.f, 1.f));

	HeaderSlot2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	HeaderSlot2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderSlotMtrl"));
	HeaderSlot2->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
	Header->AddChild(HeaderSlot2);

	// 달력 아이콘
	CGameObject* Calender = new CGameObject;
	Calender->SetName(L"Calender");
	Calender->AddComponent(new CTransform);
	Calender->AddComponent(new CMeshRender);

	Calender->Transform()->SetRelativePos(Vec3(-91.f, 0.f, -10.f));
	Calender->Transform()->SetRelativeScale(Vec3(28.f, 28.f, 1.f));

	Calender->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Calender->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CalenderMtrl"));
	Calender->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_calendar_icon.png", L"texture\\Lobby\\header\\header_calendar_icon.png");
	Calender->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	HeaderSlot2->AddChild(Calender);

	pTempLevel->AddObject(Header, 2);


	// 로비 버튼
	CGameObject* LobbyBtn = new CGameObject;
	LobbyBtn->SetName(L"LobbyBtn");
	LobbyBtn->AddComponent(new CTransform);
	LobbyBtn->AddComponent(new CMeshRender);

	LobbyBtn->Transform()->SetRelativePos(Vec3(-523.f, -330.f, 250.f));
	LobbyBtn->Transform()->SetRelativeScale(Vec3(180.f, 57.f, 1.f));

	LobbyBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	LobbyBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyBtnMtrl"));
	LobbyBtn->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png");
	LobbyBtn->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	pTempLevel->AddObject(LobbyBtn, 2);

	// 레벨 플레이
	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);
}
