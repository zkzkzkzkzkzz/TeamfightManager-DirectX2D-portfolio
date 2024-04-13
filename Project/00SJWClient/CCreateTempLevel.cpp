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
#include <Engine\CPrefab.h>
#include <Scripts\CNewBtnScript.h>
#include <Scripts\CEndBtnScript.h>

#include <Scripts\CTGMgr.h>

#include "TitleLevel.h"
#include "LobbyLevel.h"
#include "BattleLevel.h"

void TestFunc();
INT_PTR CALLBACK CreateTempLevel();

void CCreateTempLevel::Init()
{	

}

void CCreateTempLevel::CreateTempLevel()
{
	Ptr<CMaterial> pSkyMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyMtrl"); 
	Ptr<CMaterial> pStadiumMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StadiumMtrl");
	Ptr<CMaterial> pBackgroudMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl");
	Ptr<CMaterial> pTitleMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TitleMtrl");
	Ptr<CMaterial> pNGBtnMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NewGameMtrl");
	Ptr<CMaterial> pEGBtnMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EndGameMtrl");

	pSkyMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\sky.png", L"texture\\Title\\sky.png"));
	pStadiumMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\stadium.png", L"texture\\Title\\stadium.png"));
	pBackgroudMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\titleBG.png", L"texture\\Title\\titleBG.png"));
	pTitleMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\title.png", L"texture\\Title\\title.png"));
	pNGBtnMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_NewGame_Idle.png", L"texture\\Title\\Title_NewGame_Idle.png"));
	pEGBtnMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_Exit_Idle.png", L"texture\\Title\\Title_Exit_Idle.png"));

	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\TitleLV3.lv");
	pLevel->FindObjectByName(L"NewGameBtn")->GetScript<CNewBtnScript>()->SetCallBack(TestFunc);

	GamePlayStatic::Play2DBGM(L"sound\\Title.wav", 1.f);
	CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);

	//TitleLevel::CreateTempLevel();
	//LobbyLevel::CreateTempLevel();
	//BattleLevel::CreateTempLevel();

	//CLevelMgr::GetInst()->ChangeLevel(new CBanpickLevel, LEVEL_STATE::STOP);
	//CLevelMgr::GetInst()->ChangeLevel(new CAwardsLevel, LEVEL_STATE::STOP);
}

void TestFunc()
{
	//Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HouseMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0,
	//				CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\house_bg_custom.png", L"texture\\Lobby\\house_bg_custom.png"));

	//pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbySkyMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0,
	//				CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\sky_day.png", L"texture\\Lobby\\sky_day.png"));

	//pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyGroundMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0,
	//				CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\ground.png", L"texture\\Lobby\\ground.png"));

	//pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0, 
	//				CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_bg.png", L"texture\\Lobby\\header\\header_bg.png"));

	//pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"HeaderSlotMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0, 
	//				CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_slot_bg.png", L"texture\\Lobby\\header\\header_slot_bg.png"));

	//pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CalenderMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0,
	//				CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_calendar_icon.png", L"texture\\Lobby\\header\\header_calendar_icon.png"));

	//pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CoinMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0, 
	//				CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\header_gold_icon.png", L"texture\\Lobby\\header\\header_gold_icon.png"));

	//pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LogoMtrl");
	//pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\captains.png", L"texture\\Lobby\\header\\captains.png"));
	//pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	//pMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\league_icon_custom.png", L"texture\\Lobby\\league_icon_custom.png"));

	//pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TextUIMtrl");
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\notification_bg_header.png", L"texture\\Lobby\\header\\notification_bg_header.png"));

	//CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\LobbyLV9.lv");
	//CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);

	LobbyLevel::CreateTempLevel();
}