#include "pch.h"
#include "CSquadBtnScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>

#include "CLobbyBtnScript.h"

CSquadBtnScript::CSquadBtnScript()
	: CScript(SQUADBTNSCRIPT)
	, m_CurImg(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_isOpen(false)
	, m_isBtnOpen(false)
	, m_Time(0.f)
	, m_Duration(0.7f)
{
}

CSquadBtnScript::CSquadBtnScript(const CSquadBtnScript& _Other)
	: CScript(SQUADBTNSCRIPT)
	, m_CurImg(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_isOpen(false)
	, m_isBtnOpen(false)
	, m_Time(0.f)
	, m_Duration(0.7f)
{
}

CSquadBtnScript::~CSquadBtnScript()
{
}


void CSquadBtnScript::begin()
{
	m_NormalImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png");
	m_HoverImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_1.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_1.png");
	m_CurImg = m_NormalImg;
}

void CSquadBtnScript::tick()
{
	if (GetOwner()->GetParent()->GetScript<CLobbyBtnScript>()->m_isOpen && !m_isBtnOpen)
		OpenBtn();
	else if (GetOwner()->GetParent()->GetScript<CLobbyBtnScript>()->m_isOpen == false && m_isBtnOpen)
		CloseBtn();


	if (m_isBtnOpen)
	{
		m_bMouseOn_Prev = m_bMouseOn;

		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		Vec2 vResolution = CEngine::GetInst()->GetResolution();

		vMousePos.x -= vResolution.x / 2.f;
		vMousePos.y = -(vMousePos.y - vResolution.y / 2.f);

		Vec3 vRelativePos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vWorldPos = GetOwner()->GetParent()->Transform()->GetWorldPos();
		Vec3 vWorldScale = GetOwner()->GetParent()->Transform()->GetWorldScale();


		Vec2 vLT = Vec2(vWorldPos.x - vWorldScale.x / 2, (vWorldPos.y + vRelativePos.y) - vWorldScale.y / 2);
		Vec2 vRB = Vec2(vWorldPos.x + vWorldScale.x / 2, (vWorldPos.y + vRelativePos.y) + vWorldScale.y / 2);

		if (vLT.x < vMousePos.x && vMousePos.x < vRB.x
			&& vLT.y < vMousePos.y && vMousePos.y < vRB.y)
		{
			m_bMouseOn = true;
		}
		else
		{
			m_bMouseOn = false;
		}

		bool bLBtnTap = KEY_TAP(LBTN);
		bool bLbtnReleased = KEY_RELEASED(LBTN);

		if (bLBtnTap)
		{
			if (vLT.x < vMousePos.x && vMousePos.x < vRB.x
				&& vLT.y < vMousePos.y && vMousePos.y < vRB.y)
			{
				m_isOpen = true;
			}
			else
			{
				LBtnReleased();
			}
		}
		if (m_bMouseOn)
		{
			if (m_bMouseOn_Prev != m_bMouseOn)
				OnHovered();

			if (bLbtnReleased)
			{
				LBtnUp();

				if (m_bMouseLBtnDown)
				{
					LBtnClicked();
				}
			}
			else if (bLBtnTap)
			{
				m_bMouseLBtnDown = true;
			}

			if (bLbtnReleased)
				m_bMouseLBtnDown = false;
		}
		else
		{
			if (m_bMouseOn_Prev != m_bMouseOn)
				OnUnHovered();
		}
	}
	else
	{
		OnUnHovered();
	}


	render();
}

void CSquadBtnScript::render()
{
	if (nullptr != m_CurImg)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
}

void CSquadBtnScript::OnHovered()
{
	m_CurImg = m_HoverImg;
}

void CSquadBtnScript::OnUnHovered()
{
	m_CurImg = m_NormalImg;
}

void CSquadBtnScript::LBtnUp()
{
	m_CurImg = m_NormalImg;
}

void CSquadBtnScript::LBtnReleased()
{
	m_CurImg = m_NormalImg;
}

void CSquadBtnScript::LBtnClicked()
{
	m_CurImg = m_HoverImg;
}


void CSquadBtnScript::OpenBtn()
{
	m_Time += DT;

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	float BtwTime = m_Duration - m_Time;

	if (m_Time < 0.2f)
	{
		vPos.y += 1500.f * DT * BtwTime;
		if (vPos.y >= 187.f)
			vPos.y = 187.f;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}
	else if (m_Time >= 0.2f && m_Time < m_Duration && BtwTime >= 0.f)
	{
		vPos.y -= 60.f * DT * BtwTime;
		if (vPos.y <= 186.f)
			vPos.y = 186.f;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = 186.f;
		GetOwner()->Transform()->SetRelativePos(vPos);

		m_isBtnOpen = true;
		m_Time = 0.f;
	}
}


void CSquadBtnScript::CloseBtn()
{
	m_Time += DT;

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	float BtwTime = m_Duration - m_Time;

	if (m_Time < 0.2f)
	{
		vPos.y += 15.f * DT * BtwTime;
		GetOwner()->Transform()->SetRelativePos(vPos);
	}
	else if (m_Time >= 0.2f && m_Time < m_Duration && BtwTime >= 0.f)
	{
		vPos.y -= 1500.f * DT * BtwTime;
		if (vPos.y <= 0.f)
			vPos.y = 0.f;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = 0.f;
		GetOwner()->Transform()->SetRelativePos(vPos);

		m_isBtnOpen = false;
		m_Time = 0.f;
	}
}