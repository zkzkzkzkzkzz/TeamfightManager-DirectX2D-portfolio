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
#include <Engine\CFontMgr.h>

#include "CLevelSaveLoad.h"

#include <Scripts\CTGMgr.h>
#include <Scripts\CLobbyHdScript.h>
#include <Scripts\CLobbyBtnScript.h>
#include <Scripts\CTrainningBtnScript.h>
#include <Scripts\CRecruitmentBtnScript.h>
#include <Scripts\CSquadBtnScript.h>
#include <Scripts\CCursorScript.h>
#include <Scripts\CProceedBtnScript.h>
#include <Scripts\CMgrBtnScript.h>
#include <Scripts\CRecruitmentScript.h>
#include <Scripts\CRecruitSlotScript.h>
#include <Scripts\CGamerScript.h>
#include <Scripts\CJoongSooScript.h>
#include <Scripts\CGosuScript.h>


bool compare(CGameObject* a, CGameObject* b)
{
	return a->Transform()->GetRelativePos().z > b->Transform()->GetRelativePos().z;
}


void LobbyLevel::CreateTempLevel()
{

	vector<CGameObject*> vecUI = {};

	CLevel* pTempLevel = new CLevel;

	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Light");
	pTempLevel->GetLayer(2)->SetName(L"Background");
	pTempLevel->GetLayer(3)->SetName(L"Gamer");
	pTempLevel->GetLayer(4)->SetName(L"Cursor");
	pTempLevel->GetLayer(31)->SetName(L"UI");

	CGameObject* gamer = new CGameObject;
	gamer->AddComponent(new CTransform);
	gamer->AddComponent(new CMeshRender);
	gamer->AddComponent(new CGamerScript);
	gamer->SetName(L"Chobo");
	pTempLevel->AddObject(gamer, 2);
	CTGMgr::GetInst()->G_Gamer.insert(make_pair(gamer->GetName(), gamer));

	gamer = new CGameObject;
	gamer->AddComponent(new CTransform);
	gamer->AddComponent(new CMeshRender);
	gamer->AddComponent(new CJoongSooScript);
	gamer->SetName(L"JoongSoo");
	pTempLevel->AddObject(gamer, 2);
	CTGMgr::GetInst()->G_Gamer.insert(make_pair(gamer->GetName(), gamer));

	gamer = new CGameObject;
	gamer->AddComponent(new CTransform);
	gamer->AddComponent(new CMeshRender);
	gamer->AddComponent(new CGosuScript);
	gamer->SetName(L"GaeGosu");
	pTempLevel->AddObject(gamer, 2);
	CTGMgr::GetInst()->G_RecruitList.insert(make_pair(gamer->GetName(), gamer));


	CGameObject* pCursor = new CGameObject;
	pCursor->SetName(L"Cursor");
	pCursor->AddComponent(new CTransform);
	pCursor->AddComponent(new CMeshRender);
	pCursor->AddComponent(new CCursorScript);
	pTempLevel->AddObject(pCursor, 4);

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
	house->Transform()->SetRelativePos(Vec3(0.f, -32.f, 400.f));
	house->Transform()->SetRelativeScale(Vec3(167.f * 2.5f, 128.f * 2.5f, 1.f));
	house->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	house->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HouseMtrl"));
	house->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\house_bg_custom.png", L"texture\\Lobby\\house_bg_custom.png");
	house->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	vecUI.push_back(house);

	// 로비 배경(하늘)
	CGameObject* LobbySky = new CGameObject;
	LobbySky->SetName(L"LobbySky");
	LobbySky->AddComponent(new CTransform);
	LobbySky->AddComponent(new CMeshRender);
	LobbySky->Transform()->SetRelativePos(Vec3(0.f, 96.f, 500.f));
	LobbySky->Transform()->SetRelativeScale(Vec3(1280.f, 768.f, 1.f));
	LobbySky->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	LobbySky->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbySkyMtrl"));
	LobbySky->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\sky_day.png", L"texture\\Lobby\\sky_day.png");
	LobbySky->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	vecUI.push_back(LobbySky);
	
	// 로비 배경(땅)
	CGameObject* LobbyGround = new CGameObject;
	LobbyGround->SetName(L"LobbyGround");
	LobbyGround->AddComponent(new CTransform);
	LobbyGround->AddComponent(new CMeshRender);
	LobbyGround->Transform()->SetRelativePos(Vec3(0.f, -288.f, 350.f));
	LobbyGround->Transform()->SetRelativeScale(Vec3(480.f * 3.f, 64.f * 3.f, 1.f));
	LobbyGround->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	LobbyGround->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyGroundMtrl"));
	LobbyGround->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\ground.png", L"texture\\Lobby\\ground.png");
	LobbyGround->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	vecUI.push_back(LobbyGround);

	// 헤더
	CGameObject* Header = new CGameObject;
	Header->SetName(L"Header");
	Header->AddComponent(new CTransform);
	Header->AddComponent(new CMeshRender);
	Header->AddComponent(new CLobbyHdScript);
	Header->Transform()->SetRelativePos(Vec3(0.f, 330.f, 330.f));
	Header->Transform()->SetRelativeScale(Vec3(1280.f, 74.f, 1.f));
	Header->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Header->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderMtrl"));
	Header->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_bg.png", L"texture\\Lobby\\header\\header_bg.png");
	Header->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	// 헤더 슬롯
	CGameObject* HeaderSlot = new CGameObject;
	HeaderSlot->SetName(L"HeaderSlot");
	HeaderSlot->AddComponent(new CTransform);
	HeaderSlot->AddComponent(new CMeshRender);
	HeaderSlot->Transform()->SetRelativePos(Vec3(520.f, 0.f, -20.f));
	HeaderSlot->Transform()->SetRelativeScale(Vec3(226.f, 48.f, 1.f));
	HeaderSlot->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	HeaderSlot->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderSlotMtrl"));
	HeaderSlot->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_slot_bg.png", L"texture\\Lobby\\header\\header_slot_bg.png");
	HeaderSlot->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	Header->AddChild(HeaderSlot);

	// 코인 아이콘
	CGameObject* Coin = new CGameObject;
	Coin->SetName(L"Coin");
	Coin->AddComponent(new CTransform);
	Coin->AddComponent(new CMeshRender);
	Coin->Transform()->SetRelativePos(Vec3(-91.f, 0.f, -20.f));
	Coin->Transform()->SetRelativeScale(Vec3(28.f, 28.f, 1.f));
	Coin->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Coin->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CoinMtrl"));
	Coin->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_gold_icon.png", L"texture\\Lobby\\header\\header_gold_icon.png");
	Coin->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	HeaderSlot->AddChild(Coin);

	// 헤더 슬롯 2
	CGameObject* HeaderSlot2 = new CGameObject;
	HeaderSlot2->SetName(L"HeaderSlot2");
	HeaderSlot2->AddComponent(new CTransform);
	HeaderSlot2->AddComponent(new CMeshRender);
	HeaderSlot2->Transform()->SetRelativePos(Vec3(285.f, 0.f, -20.f));
	HeaderSlot2->Transform()->SetRelativeScale(Vec3(226.f, 48.f, 1.f));
	HeaderSlot2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	HeaderSlot2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderSlotMtrl"));
	HeaderSlot2->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Header->AddChild(HeaderSlot2);

	// 달력 아이콘
	CGameObject* Calender = new CGameObject;
	Calender->SetName(L"Calender");
	Calender->AddComponent(new CTransform);
	Calender->AddComponent(new CMeshRender);
	Calender->Transform()->SetRelativePos(Vec3(-91.f, 0.f, -20.f));
	Calender->Transform()->SetRelativeScale(Vec3(28.f, 28.f, 1.f));
	Calender->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Calender->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CalenderMtrl"));
	Calender->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_calendar_icon.png", L"texture\\Lobby\\header\\header_calendar_icon.png");
	Calender->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	HeaderSlot2->AddChild(Calender);
	vecUI.push_back(Header);

	CGameObject* CalenderTextYear = new CGameObject;
	CalenderTextYear->AddComponent(new CTransform);
	CalenderTextYear->AddComponent(new CTextRender);
	CalenderTextYear->Transform()->SetRelativePos(Vec3(0.f, 0.f, -20.f));

	string str = {};
	str	= std::to_string(CTGMgr::GetInst()->G_Year) + "/ ";
	str += std::to_string(CTGMgr::GetInst()->G_Month) + "/ ";
	str += std::to_string(CTGMgr::GetInst()->G_Week);

	CalenderTextYear->TextRender()->SetString(ToWString(str));
	CalenderTextYear->TextRender()->SetFont(L"Galmuri14");
	CalenderTextYear->TextRender()->SetFontSize(23.f);
	CalenderTextYear->TextRender()->SetFontColor(255, 255, 255, 255);
	CalenderTextYear->TextRender()->SetOffsetPos(Vec3(66.f, -11.f, 300.f));
	Calender->AddChild(CalenderTextYear);


	// 팀관리 버튼
	CGameObject* LobbyBtn = new CGameObject;
	LobbyBtn->SetName(L"Team");
	LobbyBtn->AddComponent(new CTransform);
	LobbyBtn->AddComponent(new CMeshRender);
	LobbyBtn->AddComponent(new CLobbyBtnScript);
	LobbyBtn->Transform()->SetRelativePos(Vec3(-523.f, -330.f, 250.f));
	LobbyBtn->Transform()->SetRelativeScale(Vec3(180.f, 57.f, 1.f));
	LobbyBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	LobbyBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyBtnMtrl"));
	LobbyBtn->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	CGameObject* ChildBtn = new CGameObject;
	ChildBtn->SetName(L"Recruitment");
	ChildBtn->AddComponent(new CTransform);
	ChildBtn->AddComponent(new CMeshRender);
	ChildBtn->AddComponent(new CRecruitmentBtnScript);
	ChildBtn->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	ChildBtn->Transform()->SetRelativeScale(Vec3(180.f, 57.f, 1.f));
	ChildBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	ChildBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyBtnMtrl"));
	ChildBtn->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	ChildBtn->SetActive(false);
	LobbyBtn->AddChild(ChildBtn);

	ChildBtn = new CGameObject;
	ChildBtn->SetName(L"Traning");
	ChildBtn->AddComponent(new CTransform);
	ChildBtn->AddComponent(new CMeshRender);
	ChildBtn->AddComponent(new CTrainningBtnScript);
	ChildBtn->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	ChildBtn->Transform()->SetRelativeScale(Vec3(180.f, 57.f, 1.f));
	ChildBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	ChildBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyBtnMtrl"));
	ChildBtn->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	ChildBtn->SetActive(false);
	LobbyBtn->AddChild(ChildBtn);

	ChildBtn = new CGameObject;
	ChildBtn->SetName(L"Squad");
	ChildBtn->AddComponent(new CTransform);
	ChildBtn->AddComponent(new CMeshRender);
	ChildBtn->AddComponent(new CSquadBtnScript);
	ChildBtn->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	ChildBtn->Transform()->SetRelativeScale(Vec3(180.f, 57.f, 1.f));
	ChildBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	ChildBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyBtnMtrl"));
	ChildBtn->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	ChildBtn->SetActive(false);
	LobbyBtn->AddChild(ChildBtn);

	vecUI.push_back(LobbyBtn);


	// 운영
	CGameObject* MgrBtn = new CGameObject;
	MgrBtn->SetName(L"Management");
	MgrBtn->AddComponent(new CTransform);
	MgrBtn->AddComponent(new CMeshRender);
	MgrBtn->AddComponent(new CMgrBtnScript);
	MgrBtn->Transform()->SetRelativePos(Vec3(-330.f, -330.f, 250.f));
	MgrBtn->Transform()->SetRelativeScale(Vec3(180.f, 57.f, 1.f));
	MgrBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MgrBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyBtnMtrl"));
	vecUI.push_back(MgrBtn);

	// Proceed Button
	CGameObject* proceedBtn = new CGameObject;
	proceedBtn->SetName(L"ProceedBtn");
	proceedBtn->AddComponent(new CTransform);
	proceedBtn->AddComponent(new CMeshRender);
	proceedBtn->AddComponent(new CProceedBtnScript);
	vecUI.push_back(proceedBtn);

	// Recruitment
	CGameObject* Recruit = new CGameObject;
	Recruit->SetName(L"RecruitmentUI");
	Recruit->AddComponent(new CTransform);
	Recruit->AddComponent(new CMeshRender);
	Recruit->AddComponent(new CRecruitmentScript);
	
	CGameObject* RecruitSlot = new CGameObject;
	RecruitSlot->SetName(L"RecruitSlot");
	RecruitSlot->AddComponent(new CTransform);
	RecruitSlot->AddComponent(new CMeshRender);
	RecruitSlot->AddComponent(new CRecruitSlotScript);
	Recruit->AddChild(RecruitSlot);
	RecruitSlot->SetActive(false);
	Recruit->SetActive(false);

	vecUI.push_back(Recruit);

	Ptr<CMaterial> pBtn = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BtnMtrl");
	pBtn->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pBtn->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\btn\\important_button_0.png",
																			L"texture\\Lobby\\recruitment\\btn\\important_button_0.png"));
	pBtn->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	pBtn->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\btn\\important_button_1.png",
																			L"texture\\Lobby\\recruitment\\btn\\important_button_1.png"));
	pBtn->SetScalarParam(SCALAR_PARAM::INT_0, 2);
	pBtn->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\btn\\default_button_4.png",
																			L"texture\\Lobby\\recruitment\\btn\\default_button_4.png"));

	Ptr<CMaterial> pRecruit = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RecruitmentMtrl");
	pRecruit->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pRecruit->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruitment_ui_bg.png",
																				L"texture\\Lobby\\recruitment\\recruitment_ui_bg.png"));
	pRecruit->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	pRecruit->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_slot_bg.png",
																				L"texture\\Lobby\\recruitment\\recruit_slot_bg.png"));
	pRecruit->SetScalarParam(SCALAR_PARAM::INT_0, 2);
	pRecruit->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_player_slot_bg.png",
																				L"texture\\Lobby\\recruitment\\recruit_player_slot_bg.png"));

	Ptr<CMaterial> pTextUI = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TextUIMtrl");
	pTextUI->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\notification_bg_header.png",
																				L"texture\\Lobby\\header\\notification_bg_header.png"));
	
	
	Ptr<CMaterial> pLogo = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LogoMtrl");
	pLogo->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pLogo->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\captains.png", 
																			L"texture\\Lobby\\header\\captains.png"));
	pLogo->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	pLogo->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\league_icon_custom.png",
																			L"texture\\Lobby\\league_icon_custom.png"));

	//Ptr<CMaterial> pGamer = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"GamerMtrl");
	//pGamer->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Avatar\\Gamer\\custom\\character_custom0.png",
	//																		L"texture\\Avatar\\Gamer\\custom\\character_custom0.png"));
	//pGamer->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Avatar\\Gamer\\custom\\character_custom1.png",
	//																		L"texture\\Avatar\\Gamer\\custom\\character_custom1.png"));	
	//pGamer->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Avatar\\Gamer\\custom\\character_custom2.png",
	//																		L"texture\\Avatar\\Gamer\\custom\\character_custom2.png"));

	sort(vecUI.begin(), vecUI.end(), compare);

	for (size_t i = 0; i < vecUI.size(); ++i)
	{
		pTempLevel->AddObject(vecUI[i], 2);
	}

	// 레벨 플레이
	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

}
