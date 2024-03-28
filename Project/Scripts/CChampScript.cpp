#include "pch.h"
#include "CChampScript.h"



CChampScript::CChampScript()
	: CScript(CHAMPSCRIPT)
	, m_Info{}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
	, m_bAttack(false)
{
}

CChampScript::CChampScript(UINT _ScriptType)
	: CScript(_ScriptType)
	, m_Info{}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
	, m_bAttack(false)
{
}

CChampScript::CChampScript(const CChampScript& _Origin)
	: CScript(CHAMPSCRIPT)
	, m_Info(_Origin.m_Info)
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
	, m_bAttack(false)
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
}

void CChampScript::render()
{
}


void CChampScript::Damaged(int HP, int ATK, int DEF)
{
	int Damage = (ATK * 100 + 99 + DEF) / (100 + DEF);

	HP -= Damage;

	m_Info.HP = HP;
}