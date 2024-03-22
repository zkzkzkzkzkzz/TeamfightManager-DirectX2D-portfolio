#include "pch.h"
#include "CRecruitmentBtnScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>
#include <Engine\CFontMgr.h>

#include "CLobbyBtnScript.h"

CRecruitmentBtnScript::CRecruitmentBtnScript()
	: CScript(RECRUITMENTBTNSCRIPT)
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
	, m_BtnText(nullptr)
{
}

CRecruitmentBtnScript::CRecruitmentBtnScript(const CRecruitmentBtnScript& _Other)
	: CScript(RECRUITMENTBTNSCRIPT)
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
	, m_BtnText(nullptr)
{
}

CRecruitmentBtnScript::~CRecruitmentBtnScript()
{
}


void CRecruitmentBtnScript::begin()
{
	m_NormalImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png");
	m_HoverImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_1.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_1.png");
	m_CurImg = m_NormalImg;

	m_BtnText = new CGameObject;
	m_BtnText->AddComponent(new CTransform);
	m_BtnText->AddComponent(new CTextRender);
	m_BtnText->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	m_BtnText->TextRender()->SetString(L"선수 영입");
	m_BtnText->TextRender()->TextInit(L"Galmuri11", 24.f, FONT_RGBA(255, 255, 255, 255));
	m_BtnText->TextRender()->SetOffsetPos(Vec3(-40.f, -8.f, 1.f));
	GetOwner()->AddChild(m_BtnText);
	m_BtnText->SetLayerIdx(5);
	m_BtnText->SetActive(false);
}

void CRecruitmentBtnScript::tick()
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

void CRecruitmentBtnScript::render()
{
	if (nullptr != m_CurImg)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
}

void CRecruitmentBtnScript::OnHovered()
{
	m_CurImg = m_HoverImg;
}

void CRecruitmentBtnScript::OnUnHovered()
{
	m_CurImg = m_NormalImg;
}

void CRecruitmentBtnScript::LBtnUp()
{
	m_CurImg = m_NormalImg;
}

void CRecruitmentBtnScript::LBtnReleased()
{
	m_CurImg = m_NormalImg;
}

void CRecruitmentBtnScript::LBtnClicked()
{
	m_CurImg = m_HoverImg;
}


void CRecruitmentBtnScript::OpenBtn()
{
	m_Time += DT;

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	float BtwTime = m_Duration - m_Time;

	if (m_Time < 0.15f)
	{
		vPos.y += 900.f * DT * BtwTime;
		if (vPos.y >= 60.f)
			vPos.y = 60.f;

		GetOwner()->Transform()->SetRelativePos(vPos);

		if (!m_BtnText->IsActive())
			m_BtnText->SetActive(true);
	}
	else if (m_Time >= 0.15f && m_Time < m_Duration)
	{
		vPos.y -= 400.f * DT * BtwTime;
		if (vPos.y <= 58.f)
			vPos.y = 58.f;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = 58.f;
		GetOwner()->Transform()->SetRelativePos(vPos);

		m_isBtnOpen = true;
		m_Time = 0.f;
	}
}


void CRecruitmentBtnScript::CloseBtn()
{
	m_Time += DT;

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	float BtwTime = m_Duration - m_Time;

	if (m_Time < 0.2f)
	{
		vPos.y += 20.f * DT * BtwTime;
		GetOwner()->Transform()->SetRelativePos(vPos);
	}
	else if (m_Time >= 0.2f && m_Time < m_Duration && BtwTime >= 0.f)
	{
		vPos.y -= 1000.f * DT * BtwTime;
		if (vPos.y <= 0.f)
			vPos.y = 0.f;

		GetOwner()->Transform()->SetRelativePos(vPos);

		if (m_BtnText->IsActive())
			m_BtnText->SetActive(false);
	}
	else
	{
		vPos.y = 0.f;
		GetOwner()->Transform()->SetRelativePos(vPos);

		m_isBtnOpen = false;
		m_Time = 0.f;
	}
}
