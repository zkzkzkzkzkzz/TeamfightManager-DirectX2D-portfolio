#include "pch.h"
#include "CChampScript.h"

#include "CBTMgr.h"

CChampScript::CChampScript()
	: CScript(CHAMPSCRIPT)
	, m_Info{}
	, m_InGameStatus{}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
	, m_bAttack(false)
	, m_bRespawn(false)
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
{
}

CChampScript::~CChampScript()
{
}


void CChampScript::begin()
{
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
	}

	CBTMgr::tick();
}

void CChampScript::render()
{
}


void CChampScript::Damaged(CGameObject* Attacker, CGameObject* Target)
{
	int ATK = Attacker->GetScript<CChampScript>()->GetInGameChampATK();
	int HP = Target->GetScript<CChampScript>()->GetInGameChampHP();
	int DEF = Target->GetScript<CChampScript>()->GetInGameChampDEF();

	int Damage = (ATK * 100 + 99 + DEF) / (100 + DEF);

	HP -= Damage;

	Target->GetScript<CChampScript>()->m_InGameStatus.HP = HP;

	if (Target->GetScript<CChampScript>()->m_InGameStatus.HP <= 0)
	{
		Target->GetScript<CChampScript>()->SetChampState(CHAMP_STATE::DEAD);
	}
}

void CChampScript::RespawnInfo()
{
	m_InGameStatus.HP = m_Info.MaxHP;       // 현재 체력
	m_InGameStatus.CoolTime_Attack = 0.f;	// 공격 쿨타임
	m_InGameStatus.CoolTime_Skill = 0.f;	// 스킬 쿨타임

	m_InGameStatus.RespawnTime = 0.f;
}