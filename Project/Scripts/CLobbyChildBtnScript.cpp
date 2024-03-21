#include "pch.h"
#include "CLobbyChildBtnScript.h"

#include <Engine\CTimeMgr.h>

CLobbyChildBtnScript::CLobbyChildBtnScript()
	: CScript(LOBBYCHILDBTNSCRIPT)
	, m_CurImg(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_isOpen(false)
	, m_Time(0.f)
	, m_Duration(0.7f)
{
}

CLobbyChildBtnScript::CLobbyChildBtnScript(const CLobbyChildBtnScript& _Other)
	: CScript(LOBBYCHILDBTNSCRIPT)
	, m_CurImg(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_isOpen(false)
	, m_Time(0.f)
	, m_Duration(0.7f)
{
}

CLobbyChildBtnScript::~CLobbyChildBtnScript()
{
}


void CLobbyChildBtnScript::begin()
{

}

void CLobbyChildBtnScript::tick()
{
	if (!m_isOpen)
		MoveBtn();
	
	m_Time += DT;

}

void CLobbyChildBtnScript::render()
{
}

void CLobbyChildBtnScript::OnHovered()
{
}

void CLobbyChildBtnScript::OnUnHovered()
{
}

void CLobbyChildBtnScript::LBtnUp()
{
}

void CLobbyChildBtnScript::LBtnReleased()
{
}

void CLobbyChildBtnScript::LBtnClicked()
{
}



void CLobbyChildBtnScript::MoveBtn()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	float BtwTime = m_Duration - m_Time;

	if (m_Time < 0.3f)
	{
		vPos.y += 380.f * DT * BtwTime;
		GetOwner()->Transform()->SetRelativePos(vPos);
	}
	else if (m_Time >= 0.3f && m_Time < m_Duration && BtwTime >= 0.f)
	{
		vPos.y -= 10.f * DT * BtwTime;
		GetOwner()->Transform()->SetRelativePos(vPos);
	}
	else
	{
		m_isOpen = true;
	}
}