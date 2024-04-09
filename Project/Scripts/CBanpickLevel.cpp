#include "pch.h"
#include "CBanpickLevel.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>

#include "CGamerScript.h"
#include "CTeamHeaderScript.h"
#include "CTeamSlotScript.h"
#include "CBansSlotScript.h"
#include "CChampSlotScript.h"
#include "CBattleStartBtnScript.h"
#include "CBattleStadiumScript.h"
#include "CCursorScript.h"
#include "CEffectScript.h"

#include "CIdleState.h"
#include "CTraceState.h"
#include "CAttackState.h"
#include "CSkillState.h"
#include "CUltimateState.h"
#include "CDeadState.h"

CBanpickLevel::CBanpickLevel()
	: m_BlueTeam{}
	, m_RedTeam{}
	, m_BanPickSlot{}
	, m_EnemyTime(0.f)
{
	for (const auto& pair : CTGMgr::GetInst()->G_Gamer)
	{
		pair.second->DisconnectWithLayer();
	}

	for (size_t i = 0; i < CTGMgr::GetInst()->G_ShortlistSlot.size(); ++i)
	{
		CTGMgr::GetInst()->G_ShortlistSlot[i]->DisconnectWithLayer();
	}

	for (size_t i = 0; i < CTGMgr::GetInst()->G_TeamGorilla.size(); ++i)
	{
		CTGMgr::GetInst()->G_TeamGorilla[i]->DisconnectWithLayer();
	}

	InitUI();
}

CBanpickLevel::CBanpickLevel(const CBanpickLevel& _Origin)
	: m_BlueTeam{}
	, m_RedTeam{}
	, m_BanPickSlot{}
	, m_EnemyTime(0.f)
{
	for (const auto& pair : CTGMgr::GetInst()->G_Gamer)
	{
		pair.second->DisconnectWithLayer();
	}

	for (size_t i = 0; i < CTGMgr::GetInst()->G_ShortlistSlot.size(); ++i)
	{
		CTGMgr::GetInst()->G_ShortlistSlot[i]->DisconnectWithLayer();
	}

	InitUI();
}

CBanpickLevel::~CBanpickLevel()
{
}


void CBanpickLevel::begin()
{
	// 배경
	CGameObject* Obj = new CGameObject;
	Obj->SetName(L"BanPickBG");
	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 8000.f));
	Obj->Transform()->SetRelativeScale(Vec3(1280.f, 800.f, 1.f));
	Obj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Obj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BanPickBG.mtrl"));
	Obj->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	AddObject(Obj, 2);

	Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\TeamHeader.prefab")->Instatiate();
	AddObject(Obj, 2);


	for (size_t i = 0; i < CTGMgr::GetInst()->G_ParticipatingPlayer.size(); ++i)
	{
		Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\BlueTeamCard_2.prefab")->Instatiate();
		Obj->GetScript<CTeamSlotScript>()->SetGamerToSlot(CTGMgr::GetInst()->G_ParticipatingPlayer[i]);
		GETGAMER(CTGMgr::GetInst()->G_ParticipatingPlayer[i])->SetGamerTeam(TEAM::BLUE);
		

		if (0 == i)
			Obj->Transform()->SetRelativePos(Vec3(-700.f, 210.f, 1000.f));
		else
			Obj->Transform()->SetRelativePos(Vec3(-700.f, 49.f, 1000.f));

		m_BanPickSlot.push_back(Obj);
		AddObject(Obj, 2);
	}

	for (size_t i = 0; i < CTGMgr::GetInst()->G_TeamGorilla.size(); ++i)
	{
		Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RedTeamCard_2.prefab")->Instatiate();
		Obj->GetScript<CTeamSlotScript>()->SetGamerToSlot(CTGMgr::GetInst()->G_TeamGorilla[i]);
		GETGAMER(CTGMgr::GetInst()->G_TeamGorilla[i])->SetGamerTeam(TEAM::RED);

		if (0 == i)
			Obj->Transform()->SetRelativePos(Vec3(700.f, 210.f, 1000.f));
		else
			Obj->Transform()->SetRelativePos(Vec3(700.f, 49.f, 1000.f));

		m_BanPickSlot.push_back(Obj);
		AddObject(Obj, 2);
	}

	Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\BanPickUI_5.prefab")->Instatiate();
	Obj->Transform()->SetRelativePos(Vec3(0.f, -800.f, 3000.f));
	Obj->Transform()->SetRelativeScale(Vec3(1280.f, 720.f, 1.f));
	AddObject(Obj, 2);

	Obj = new CGameObject;
	Obj->SetName(L"BattleStartBtn");
	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CBattleStartBtnScript);
	AddObject(Obj, 2);
	Obj->SetActive(false);

	Obj = new CGameObject;
	Obj->AddComponent(new CBattleStadiumScript);
	AddObject(Obj, 2);

	m_CurState = BANPICK_STATE::BLUEBAN;

	Ptr<CFSM> pFSM = new CFSM(false);

	pFSM->AddState(L"Idle", new CIdleState);
	pFSM->AddState(L"Trace", new CTraceState);
	pFSM->AddState(L"Attack", new CAttackState);
	pFSM->AddState(L"Skill", new CSkillState);
	pFSM->AddState(L"Ultimate", new CUltimateState);
	pFSM->AddState(L"Dead", new CDeadState);

	CAssetMgr::GetInst()->AddAsset<CFSM>(L"ChampFSM", pFSM.Get());

	pFSM = new CFSM(false);

	pFSM->AddState(L"Idle", new CIdleState);
	pFSM->AddState(L"Trace", new CTraceState);
	pFSM->AddState(L"Attack", new CAttackState);
	pFSM->AddState(L"Dead", new CDeadState);

	CAssetMgr::GetInst()->AddAsset<CFSM>(L"SummonFSM", pFSM.Get());

	CCollisionMgr::GetInst()->LayerCheck(3, 3);
	CCollisionMgr::GetInst()->LayerCheck(3, 5);

	CLevel::begin();
}

void CBanpickLevel::tick()
{
	CLevel::tick();

	m_EnemyTime += DT;

	if (BANPICK_STATE::REDBAN == m_CurState && m_EnemyTime > 3.f
		&& !CTGMgr::GetInst()->G_ChampSlot.empty())
	{
		SpawnEffect(CTGMgr::GetInst()->G_ChampSlot[1]->Transform()->GetRelativePos()
			, CTGMgr::GetInst()->G_ChampSlot[1]->Transform()->GetRelativeScale()
			, CTGMgr::GetInst()->G_ChampSlot[1]->Transform()->GetRelativeRotation()
			, L"BanAnim", 0.2, false, Vec3(0.f, -25.f, -10.f));

		CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"BanSlot2")->GetScript<CBansSlotScript>()->SetBanChampSlot(CTGMgr::GetInst()->G_ChampSlot[1]);
		CTGMgr::GetInst()->G_ChampSlot[1]->GetScript<CChampSlotScript>()->SetSlotState(SLOT_STATE::BAN);
		m_CurState = BANPICK_STATE::BLUEPICK1;
	}
	else if ((BANPICK_STATE::REDPICK1 == m_CurState || BANPICK_STATE::REDPICK2 == m_CurState) && m_EnemyTime > 2.f
		&& !CTGMgr::GetInst()->G_ChampSlot.empty())
	{
		if (BANPICK_STATE::REDPICK1 == m_CurState)
		{
			CTGMgr::GetInst()->G_ChampSlot[3]->GetScript<CChampSlotScript>()->SetSlotTeamColor(TEAM::RED);
			CTGMgr::GetInst()->G_ChampSlot[3]->GetScript<CChampSlotScript>()->SetSlotState(SLOT_STATE::PICK);
			GETGAMER(CTGMgr::GetInst()->G_TeamGorilla[0])->SetSelectedChamp(CTGMgr::GetInst()->G_ChampSlot[3]->GetScript<CChampSlotScript>()->GetChampList());
			m_CurState = BANPICK_STATE::BLUEPICK2;
		}
		else if (BANPICK_STATE::REDPICK2 == m_CurState)
		{
			CTGMgr::GetInst()->G_ChampSlot[4]->GetScript<CChampSlotScript>()->SetSlotTeamColor(TEAM::RED);
			CTGMgr::GetInst()->G_ChampSlot[4]->GetScript<CChampSlotScript>()->SetSlotState(SLOT_STATE::PICK);
			GETGAMER(CTGMgr::GetInst()->G_TeamGorilla[1])->SetSelectedChamp(CTGMgr::GetInst()->G_ChampSlot[4]->GetScript<CChampSlotScript>()->GetChampList());

			if (m_EnemyTime > 4.f)
			{
				m_CurState = BANPICK_STATE::READY;
			}
		}
	}
	else if (BANPICK_STATE::READY == m_CurState)
	{
		CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"BattleStartBtn")->SetActive(true);
	}
	else if (BANPICK_STATE::BATTLE == m_CurState)
	{
		if (CTGMgr::GetInst()->G_Time <= 0)
		{
			m_CurState = BANPICK_STATE::DONE;
		}
	}
}

void CBanpickLevel::finaltick()
{
	CLevel::finaltick();
}

void CBanpickLevel::InitUI()
{
	GetLayer(0)->SetName(L"Default");
	GetLayer(1)->SetName(L"Light");
	GetLayer(2)->SetName(L"Background");
	GetLayer(3)->SetName(L"Champ");
	GetLayer(4)->SetName(L"Cursor");
	GetLayer(5)->SetName(L"Projectile");
	GetLayer(6)->SetName(L"Effect");
	GetLayer(30)->SetName(L"GameInfo");
	GetLayer(31)->SetName(L"UI");

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
	pCamObj->Camera()->LayerCheck(5, false);
	pCamObj->Camera()->LayerCheck(7, false);
	pCamObj->Camera()->LayerCheck(30, false);
	AddObject(pCamObj, 0);

	// BG Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"BGCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(2, true);
	AddObject(pCamObj, 0);

	// Champ Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"ChampCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(2);
	pCamObj->Camera()->LayerCheck(3, true);
	AddObject(pCamObj, 0);

	// Projectile Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"ProjectileCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(3);
	pCamObj->Camera()->LayerCheck(5, true);
	AddObject(pCamObj, 0);

	// 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light2D");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);
	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(1.f, 1.f, 1.f));
	AddObject(pLight, 1);

	CGameObject* pCursor = new CGameObject;
	pCursor->SetName(L"Cursor");
	pCursor->AddComponent(new CTransform);
	pCursor->AddComponent(new CMeshRender);
	pCursor->AddComponent(new CCursorScript);
	AddObject(pCursor, 4);

	for (const auto& pair : CTGMgr::GetInst()->G_Gamer)
	{
		AddObject(pair.second, 30);
		pair.second->SetActive(false);
	}

	for (size_t i = 0; i < CTGMgr::GetInst()->G_ShortlistSlot.size(); ++i)
	{
		AddObject(CTGMgr::GetInst()->G_ShortlistSlot[i], 30);
	}

	for (size_t i = 0; i < CTGMgr::GetInst()->G_TeamGorilla.size(); ++i)
	{
		AddObject(CTGMgr::GetInst()->G_TeamGorilla[i], 30);
	}
}


void CBanpickLevel::SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat, Vec3 _offset)
{
	CGameObject* effect = new CGameObject;
	effect->AddComponent(new CTransform);
	effect->AddComponent(new CMeshRender);
	effect->AddComponent(new CAnimator2D);
	effect->AddComponent(new CEffectScript);

	_Pos.x += _offset.x;
	_Pos.y += _offset.y;
	_Pos.z += _offset.z;

	GETEFFECT(effect)->SetEffectInfo(_Pos, _Scale, _Rotation, _anim, _time, _repeat);
	GamePlayStatic::SpawnGameObject(effect, 6);
}
