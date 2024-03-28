#include "pch.h"
#include "CChampScript.h"



CChampScript::CChampScript()
	: CScript(CHAMPSCRIPT)
	, m_Info{}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
{
}

CChampScript::CChampScript(const CChampScript& _Origin)
	: CScript(CHAMPSCRIPT)
	, m_Info(_Origin.m_Info)
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
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
}

void CChampScript::render()
{
}