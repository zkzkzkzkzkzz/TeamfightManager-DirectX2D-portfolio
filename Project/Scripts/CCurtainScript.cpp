#include "pch.h"
#include "CCurtainScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTransform.h>

CCurtainScript::CCurtainScript()
	: CScript(CURTAINSCRIPT)
	, m_idx(TEXT_INDEX::NONE)
	, m_bTime(false)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
	, m_Time(0.f)
{
}

CCurtainScript::CCurtainScript(const CCurtainScript& _Origin)
	: CScript(CURTAINSCRIPT)
	, m_idx(TEXT_INDEX::NONE)
	, m_bTime(false)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
	, m_Time(0.f)
{
}

CCurtainScript::~CCurtainScript()
{
}

void CCurtainScript::begin()
{
}

void CCurtainScript::tick()
{
	m_idx = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"TextBox")->GetScript<CTextBoxScript>()->GetTextIdx();

	m_Time += DT;

	if (!m_bUIPos && m_Time > 1.f)
	{
		if (L"LeftCurtain" == GetOwner()->GetName())
			OpenLeftCurtain();
		else
			OpenRightCurtain();
	}

	if (TEXT_INDEX::END == m_idx)
	{
		if (!m_bTime)
		{
			m_Time = 0.f;
			m_bTime = true;
		}

		if (m_bUIPos && m_Time > 1.f)
		{
			if (L"LeftCurtain" == GetOwner()->GetName())
				CloseLeftCurtain();
			else
				CloseRightCurtain();
		}
	}
}


void CCurtainScript::OpenLeftCurtain()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 2.f - m_UIPosTime;

	if (m_UIPosTime < 0.5f)
	{
		vPos.x -= 750.f * DT * BtwTime;
		if (vPos.x <= -1000.f)
			vPos.x = -1000.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.5f && m_UIPosTime < 2.f)
	{
		vPos.x -= 100.f * DT * BtwTime;
		if (vPos.x <= -1000.f)
			vPos.x = -1000.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.x = -1000.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}

void CCurtainScript::OpenRightCurtain()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 2.f - m_UIPosTime;

	if (m_UIPosTime < 0.5f)
	{
		vPos.x += 750.f * DT * BtwTime;
		if (vPos.x >= 1000.f)
			vPos.x = 1000.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.5f && m_UIPosTime < 2.f)
	{
		vPos.x += 100.f * DT * BtwTime;
		if (vPos.x >= 1000.f)
			vPos.x = 1000.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.x = 1000.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}

void CCurtainScript::CloseLeftCurtain()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 2.f - m_UIPosTime;

	if (m_UIPosTime < 0.5f)
	{
		vPos.x += 750.f * DT * BtwTime;
		if (vPos.x >= -320.f)
			vPos.x = -320.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.5f && m_UIPosTime < 2.f)
	{
		vPos.x += 100.f * DT * BtwTime;
		if (vPos.x >= -320.f)
			vPos.x = -320.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.x = -320.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}

void CCurtainScript::CloseRightCurtain()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 2.f - m_UIPosTime;

	if (m_UIPosTime < 0.5f)
	{
		vPos.x -= 750.f * DT * BtwTime;
		if (vPos.x <= 320.f)
			vPos.x = 320.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.5f && m_UIPosTime < 2.f)
	{
		vPos.x -= 100.f * DT * BtwTime;
		if (vPos.x <= 320.f)
			vPos.x = 320.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.x = 320.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}
