#include "pch.h"
#include "CChampScript.h"

#include <ctime>
#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CTexture.h>

#include "CBTMgr.h"
#include "CGamerScript.h"
#include "CBanpickLevel.h"


CChampScript::CChampScript()
	: CScript(CHAMPSCRIPT)
	, m_Info{}
	, m_InGameStatus{}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
	, m_bAttack(false)
	, m_bRespawn(false)
	, m_Target(nullptr)
	, m_Shadow(nullptr)
	, m_ShadowTex(nullptr)
	, m_PlayingGamer(nullptr)
{
}

CChampScript::CChampScript(UINT _ScriptType)
	: CScript(_ScriptType)
	, m_Info{}
	, m_InGameStatus{}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
	, m_bAttack(false)
	, m_bRespawn(false)
	, m_Target(nullptr)
	, m_Shadow(nullptr)
	, m_ShadowTex(nullptr)
	, m_PlayingGamer(nullptr)
{
}

CChampScript::CChampScript(const CChampScript& _Origin)
	: CScript(CHAMPSCRIPT)
	, m_Info(_Origin.m_Info)
	, m_InGameStatus{_Origin.m_InGameStatus}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
	, m_bAttack(false)
	, m_bRespawn(false)
	, m_Target(nullptr)
	, m_Shadow(nullptr)
	, m_ShadowTex(_Origin.m_ShadowTex)
	, m_PlayingGamer(nullptr)
{
}

CChampScript::~CChampScript()
{
}


void CChampScript::begin()
{
	Vec3 vSpawnPos = Vec3(0.f, 0.f, 300.f);

	if (TEAM::BLUE == m_Team)
		vSpawnPos = Vec3(-290.f, 0.f, 300.f);
	else
		vSpawnPos = Vec3(290.f, 0.f, 300.f);

	vSpawnPos.x += (float)(rand() % 50);
	vSpawnPos.y += (float)(rand() % 200) - 200.f;
	vSpawnPos.z += vSpawnPos.y;

	Transform()->SetRelativePos(vSpawnPos);

	CBanpickLevel* pLevel = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();

	if (BANPICK_STATE::BATTLE == pLevel->GetCurBanPickState())
	{
		CheckPlayingGamer();
	}
}

void CChampScript::tick()
{
	switch (m_State)
	{
	case CHAMP_STATE::IDLE:
		this->EnterIdleState();
		break;
	case CHAMP_STATE::TRACE:
		EnterTraceState();
		break;
	case CHAMP_STATE::ATTACK:
		EnterAttackState();
		break;
	case CHAMP_STATE::SKILL:
		EnterSkillState();
		break;
	case CHAMP_STATE::ULTIMATE:
		EnterUltimateState();
		break;
	case CHAMP_STATE::DEAD:
		EnterDeadState();
		break;
	case CHAMP_STATE::END:
		break;
	default:
		break;
	}

	Vec3 vPos = Transform()->GetRelativePos();
	vPos.z = 300.f + vPos.y;
	Transform()->SetRelativePos(vPos);

	if (L"BTMgr" == GetOwner()->GetName())
	{
		CBTMgr::tick();
	}

	CheckShadow();
	render();
}

void CChampScript::render()
{
	if (nullptr != m_ShadowTex)
		m_Shadow->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_ShadowTex);
}


void CChampScript::Damaged(CGameObject* Attacker, CGameObject* Target, int _ExtraDmg)
{
	int ATK = GETCHAMP(Attacker)->GetInGameChampATK() + _ExtraDmg;
	int HP = GETCHAMP(Target)->GetInGameChampHP();
	int DEF = GETCHAMP(Target)->GetInGameChampDEF();

	int Damage = (ATK * 100 + 99 + DEF) / (100 + DEF);

	GETCHAMP(Attacker)->m_InGameStatus.TotalDeal += Damage;
	GETCHAMP(Target)->m_InGameStatus.TotalDamage += Damage;

	HP -= Damage;

	GETCHAMP(Target)->m_InGameStatus.HP = HP;

	if (GETCHAMP(Target)->m_InGameStatus.HP <= 0 
		&& CHAMP_STATE::DEAD != GETCHAMP(Target)->GetChampState()
		&& L"NinjaClone" != Target->GetName())
	{
		GETCHAMP(Attacker)->m_InGameStatus.KillPoint += 1;
		GETCHAMP(Target)->m_InGameStatus.DeathPoint += 1;
		GETCHAMP(Target)->SetChampState(CHAMP_STATE::DEAD);

		TEAM team = GETCHAMP(Attacker)->GetTeamColor();

		if (TEAM::BLUE == team)
			++CTGMgr::G_BlueKillScore;
		else if (TEAM::RED == team)
			++CTGMgr::G_RedKillScore;

	}
}

void CChampScript::Healed(CGameObject* Attacker, CGameObject* Target, int _ExtraHeal)
{
	int ATK = GETCHAMP(Attacker)->GetInGameChampATK() + _ExtraHeal;

	int heal = GETCHAMP(Target)->m_InGameStatus.HP + ATK;

	if (heal >= GETCHAMP(Target)->GetChampMaxHP())
	{
		heal = GETCHAMP(Target)->GetChampMaxHP();
	}

	GETCHAMP(Target)->m_InGameStatus.TotalHeal += heal;
	GETCHAMP(Target)->m_InGameStatus.HP = heal;
}


void CChampScript::RespawnInfo()
{
	m_InGameStatus.HP = m_Info.MaxHP;       // 현재 체력
	m_InGameStatus.CoolTime_Attack = 0.f;	// 공격 쿨타임
	m_InGameStatus.CoolTime_Skill = 0.f;	// 스킬 쿨타임

	m_InGameStatus.RespawnTime = 0.f;
}


void CChampScript::SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat, Vec3 _offset)
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

void CChampScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CChampScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Vec3 vPos = _Collider->GetOwner()->Transform()->GetRelativePos();
	Vec2 vColSize = _Collider->GetOffsetScale();

	Vec3 vOtherPos = _OtherObj->Transform()->GetRelativePos();
	Vec2 vOhterColSize = _OtherCollider->GetOffsetScale();
	
	if (3 == _OtherObj->GetLayerIdx() && 3 == _Collider->GetOwner()->GetLayerIdx())
	{
		if (vPos.x < vOtherPos.x)
			vOtherPos.x += DT * 10.f;
		else if (vPos.x >= vOtherPos.x)
			vOtherPos.x -= DT * 10.f;

		if (vPos.y < vOtherPos.y)
			vOtherPos.y += DT * 10.f;
		else if (vPos.y >= vOtherPos.y)
			vOtherPos.y -= DT * 10.f;
	}	
	
	_OtherObj->Transform()->SetRelativePos(vOtherPos);
}


void CChampScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) 
{

}


void CChampScript::SpawnShadow()
{
	m_Shadow = new CGameObject;
	m_Shadow->SetName(L"Shadow");
	m_Shadow->AddComponent(new CTransform);
	m_Shadow->AddComponent(new CMeshRender);
	m_Shadow->Transform()->SetRelativePos(Vec3(1.f, -13.f, 10.f));
	m_Shadow->Transform()->SetRelativeScale(Vec3(30.f, 15.f, 1.f));
	m_Shadow->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Shadow->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShadowMtrl"));
	m_Shadow->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_ShadowTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\player_shadow.png", L"texture\\Champ\\player_shadow.png");
	GetOwner()->AddChild(m_Shadow);
	m_Shadow->SetActive(false);
}

void CChampScript::CheckShadow()
{
	if (nullptr != GETCHAMP(GetOwner())->m_Shadow && CHAMP_STATE::DEAD != GETCHAMP(GetOwner())->GetChampState())
		GETCHAMP(GetOwner())->m_Shadow->SetActive(true);
	else if (nullptr != GETCHAMP(GetOwner())->m_Shadow && CHAMP_STATE::DEAD == GETCHAMP(GetOwner())->GetChampState())
		GETCHAMP(GetOwner())->m_Shadow->SetActive(false);
}

void CChampScript::CheckPlayingGamer()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	
	for (size_t i = 0; i < CTGMgr::GetInst()->G_ParticipatingPlayer.size(); ++i)
	{
		CHAMP_LIST list = GETGAMER(CTGMgr::GetInst()->G_ParticipatingPlayer[i])->GetSelectedChamp();

		switch (list)
		{
		case CHAMP_LIST::ARCHER:
			if (nullptr != pLevel->FindObjectByName(L"Archer"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Archer"))->SetPlayingGamer(CTGMgr::GetInst()->G_ParticipatingPlayer[i]);
			}
			break;
		case CHAMP_LIST::FIGHTER:
			if (nullptr != pLevel->FindObjectByName(L"Fighter"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Fighter"))->SetPlayingGamer(CTGMgr::GetInst()->G_ParticipatingPlayer[i]);
			}
			break;
		case CHAMP_LIST::KNIGHT:
			if (nullptr != pLevel->FindObjectByName(L"Knight"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Knight"))->SetPlayingGamer(CTGMgr::GetInst()->G_ParticipatingPlayer[i]);
			}
			break;
		case CHAMP_LIST::MONK:
			if (nullptr != pLevel->FindObjectByName(L"Monk"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Monk"))->SetPlayingGamer(CTGMgr::GetInst()->G_ParticipatingPlayer[i]);
			}
			break;
		case CHAMP_LIST::NINJA:
			if (nullptr != pLevel->FindObjectByName(L"Ninja"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Ninja"))->SetPlayingGamer(CTGMgr::GetInst()->G_ParticipatingPlayer[i]);
			}
			break;
		case CHAMP_LIST::PRIEST:
			if (nullptr != pLevel->FindObjectByName(L"Priest"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Priest"))->SetPlayingGamer(CTGMgr::GetInst()->G_ParticipatingPlayer[i]);
			}
			break;
		case CHAMP_LIST::END:
			break;
		default:
			break;
		}
	}

	for (size_t i = 0; i < CTGMgr::GetInst()->G_TeamGorilla.size(); ++i)
	{
		CHAMP_LIST list = GETGAMER(CTGMgr::GetInst()->G_TeamGorilla[i])->GetSelectedChamp();

		switch (list)
		{
		case CHAMP_LIST::ARCHER:
			if (nullptr != pLevel->FindObjectByName(L"Archer"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Archer"))->SetPlayingGamer(CTGMgr::GetInst()->G_TeamGorilla[i]);
			}
			break;
		case CHAMP_LIST::FIGHTER:
			if (nullptr != pLevel->FindObjectByName(L"Fighter"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Fighter"))->SetPlayingGamer(CTGMgr::GetInst()->G_TeamGorilla[i]);
			}
			break;
		case CHAMP_LIST::KNIGHT:
			if (nullptr != pLevel->FindObjectByName(L"Knight"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Knight"))->SetPlayingGamer(CTGMgr::GetInst()->G_TeamGorilla[i]);
			}
			break;
		case CHAMP_LIST::MONK:
			if (nullptr != pLevel->FindObjectByName(L"Monk"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Monk"))->SetPlayingGamer(CTGMgr::GetInst()->G_TeamGorilla[i]);
			}
			break;
		case CHAMP_LIST::NINJA:
			if (nullptr != pLevel->FindObjectByName(L"Ninja"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Ninja"))->SetPlayingGamer(CTGMgr::GetInst()->G_TeamGorilla[i]);
			}
			break;
		case CHAMP_LIST::PRIEST:
			if (nullptr != pLevel->FindObjectByName(L"Priest"))
			{
				GETCHAMP(pLevel->FindObjectByName(L"Priest"))->SetPlayingGamer(CTGMgr::GetInst()->G_TeamGorilla[i]);
			}
			break;
		case CHAMP_LIST::END:
			break;
		default:
			break;
		}
	}
}
