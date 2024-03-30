#include "pch.h"
#include "CChampScript.h"

#include <Engine\CTimeMgr.h>	 

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
	Vec3 vSpawnPos = Vec3(0.f, 0.f, 300.f);

	if (TEAM::BLUE == m_Team)
		vSpawnPos = Vec3(-290.f, 0.f, 300.f);
	else
		vSpawnPos = Vec3(290.f, 0.f, 300.f);

	vSpawnPos.x += (float)(rand() % 50);
	vSpawnPos.y += (float)(rand() % 200);

	Transform()->SetRelativePos(vSpawnPos);

	Transform()->SetRelativeScale(Vec3(64.f, 64.f, 1.f));
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
	int ATK = GETCHAMP(Attacker)->GetInGameChampATK();
	int HP = GETCHAMP(Target)->GetInGameChampHP();
	int DEF = GETCHAMP(Target)->GetInGameChampDEF();

	int Damage = (ATK * 100 + 99 + DEF) / (100 + DEF);

	HP -= Damage;

	GETCHAMP(Target)->m_InGameStatus.HP = HP;

	if (GETCHAMP(Target)->m_InGameStatus.HP <= 0)
	{
		GETCHAMP(Attacker)->m_InGameStatus.KillPoint += 1;
		GETCHAMP(Target)->m_InGameStatus.DeathPoint -= 1;
		GETCHAMP(Target)->SetChampState(CHAMP_STATE::DEAD);
	}
}

void CChampScript::RespawnInfo()
{
	m_InGameStatus.HP = m_Info.MaxHP;       // 현재 체력
	m_InGameStatus.CoolTime_Attack = 0.f;	// 공격 쿨타임
	m_InGameStatus.CoolTime_Skill = 0.f;	// 스킬 쿨타임

	m_InGameStatus.RespawnTime = 0.f;
}