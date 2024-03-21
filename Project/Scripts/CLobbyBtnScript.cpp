#include "pch.h"
#include "CLobbyBtnScript.h"


#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>

CLobbyBtnScript::CLobbyBtnScript()
	: CScript(LOBBYBTNSCRIPT)
	, m_CurImg(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_PressedImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_isOpen(false)
{
}

CLobbyBtnScript::CLobbyBtnScript(const CLobbyBtnScript& _Other)
	: CScript(LOBBYBTNSCRIPT)
	, m_CurImg(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_PressedImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_isOpen(false)
{
}

CLobbyBtnScript::~CLobbyBtnScript()
{
}

void CLobbyBtnScript::begin()
{
	m_NormalImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png");
	m_HoverImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_1.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_1.png");
	m_PressedImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_2.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_2.png");
	m_CurImg = m_NormalImg;
}

void CLobbyBtnScript::tick()
{
	m_bMouseOn_Prev = m_bMouseOn;

	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	vMousePos.x -= vResolution.x / 2.f;
	vMousePos.y = -(vMousePos.y - vResolution.y / 2.f);

	Vec3 vWorldPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vWorldScale = GetOwner()->Transform()->GetWorldScale();

	Vec2 vLT = Vec2(vWorldPos.x - vWorldScale.x / 2, vWorldPos.y - vWorldScale.y / 2);
	Vec2 vRB = Vec2(vWorldPos.x + vWorldScale.x / 2, vWorldPos.y + vWorldScale.y / 2);

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

	render();
}

void CLobbyBtnScript::render()
{
	if (nullptr != m_CurImg)
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
}

void CLobbyBtnScript::OnHovered()
{
	if (!m_isOpen)
		m_CurImg = m_HoverImg;
}

void CLobbyBtnScript::OnUnHovered()
{
	if (!m_isOpen)
		m_CurImg = m_NormalImg;
}

void CLobbyBtnScript::LBtnUp()
{
	if (!m_isOpen)
		m_CurImg = m_NormalImg;
}

void CLobbyBtnScript::LBtnReleased()
{
	m_isOpen = false;
	m_CurImg = m_NormalImg;
}

void CLobbyBtnScript::LBtnClicked()
{
	m_isOpen = true;
	m_CurImg = m_PressedImg;

	vector<CGameObject*> vChild = GetOwner()->GetChild();

	for (size_t i = 0; i < vChild.size(); ++i)
	{
		vChild[i]->SetActive(true);
	}

	// CallBack
	if (m_CallBackFunc) m_CallBackFunc();

	// Delegate
	if (m_Inst != nullptr && m_Delegate != nullptr)
		(m_Inst->*m_Delegate)();
}