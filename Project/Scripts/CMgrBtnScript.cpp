#include "pch.h"
#include "CMgrBtnScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>
#include <Engine\CFontMgr.h>

CMgrBtnScript::CMgrBtnScript()
	: CScript(MGRBTNSCRIPT)
	, m_CurImg(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_PressedImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_isOpen(false)
	, m_BtnText(nullptr)
	, m_NormalIcon(nullptr)
	, m_HoverIcon(nullptr)
	, m_PressedIcon(nullptr)
	, m_CurIcon(nullptr)
	, m_Icon(nullptr)
{
}

CMgrBtnScript::~CMgrBtnScript()
{
}



bool CMgrBtnScript::CheckChildBtnPos(Vec2 _MousePos)
{
    return false;
}

void CMgrBtnScript::begin()
{
	m_NormalImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_0.png");
	m_HoverImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_1.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_1.png");
	m_PressedImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\menu\\main_menu_button_2.png", L"texture\\Lobby\\btn\\menu\\main_menu_button_2.png");
	m_CurImg = m_NormalImg;

	m_NormalIcon = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\icon\\main_button_icon_1.png", L"texture\\Lobby\\icon\\main_button_icon_1.png");
	m_PressedIcon = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\icon\\main_button_icon_1_1.png", L"texture\\Lobby\\icon\\main_button_icon_1_1.png");
	m_HoverIcon = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\icon\\main_button_icon_1_2.png", L"texture\\Lobby\\icon\\main_button_icon_1_2.png");
	m_CurIcon = m_NormalIcon;

	m_BtnText = new CGameObject;
	m_BtnText->AddComponent(new CTransform);
	m_BtnText->AddComponent(new CTextRender);
	m_BtnText->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	m_BtnText->TextRender()->SetString(L"Management");
	m_BtnText->TextRender()->TextInit(L"Galmuri11", 24.f, FONT_RGBA(255, 255, 255, 255));
	m_BtnText->TextRender()->SetOffsetPos(Vec3(-50.f, -8.f, 0.f));
	GetOwner()->AddChild(m_BtnText);

	m_Icon = new CGameObject;
	m_Icon->AddComponent(new CTransform);
	m_Icon->AddComponent(new CMeshRender);
	m_Icon->Transform()->SetRelativePos(Vec3(-65.f, 0.f, -10.f));
	m_Icon->Transform()->SetRelativeScale(Vec3(22.f, 22.f, 1.f));
	m_Icon->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Icon->MeshRender()->SetMaterial(GetOwner()->MeshRender()->GetDynamicMaterial());
	GetOwner()->AddChild(m_Icon);
}

void CMgrBtnScript::tick()
{
	m_bMouseOn_Prev = m_bMouseOn;

	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	vMousePos.x -= vResolution.x / 2.f;
	vMousePos.y = -(vMousePos.y - vResolution.y / 2.f);

	Vec3 vWorldPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vWorldScale = GetOwner()->Transform()->GetRelativeScale();

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
			if (!CheckChildBtnPos(vMousePos))
			{
				LBtnReleased();
			}
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

void CMgrBtnScript::render()
{
	if (nullptr != m_CurImg)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);

	if (nullptr != m_CurIcon)
		m_Icon->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurIcon);
}

void CMgrBtnScript::OnHovered()
{
	if (!m_isOpen)
	{
		m_CurImg = m_HoverImg;
		m_CurIcon = m_HoverIcon;
		m_BtnText->TextRender()->SetFontColor(103, 255, 103, 255);
	}
}

void CMgrBtnScript::OnUnHovered()
{
	if (!m_isOpen)
	{
		m_CurImg = m_NormalImg;
		m_CurIcon = m_NormalIcon;
		m_BtnText->TextRender()->SetFontColor(255, 255, 255, 255);
	}
}

void CMgrBtnScript::LBtnUp()
{
	if (!m_isOpen)
		m_CurImg = m_NormalImg;
}

void CMgrBtnScript::LBtnReleased()
{
	m_isOpen = false;
	m_CurImg = m_NormalImg;
	m_CurIcon = m_NormalIcon;

	m_BtnText->TextRender()->SetFontColor(255, 255, 255, 255);
}

void CMgrBtnScript::LBtnClicked()
{
	m_isOpen = true;
	m_CurImg = m_PressedImg;
	m_CurIcon = m_PressedIcon;

	m_BtnText->TextRender()->SetFontColor(0, 0, 0, 255);

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