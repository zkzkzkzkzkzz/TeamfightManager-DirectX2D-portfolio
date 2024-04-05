#include "pch.h"
#include "CLineUpSlotScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>


#include "CLineUpBGScript.h"
#include "CDraftArrowScript.h"
#include "CGamerScript.h"

CLineUpSlotScript::CLineUpSlotScript()
	: CScript(LINEUPSLOTSCRIPT)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_SelectSlot(false)
	, m_Gamer(nullptr)
{
}

CLineUpSlotScript::CLineUpSlotScript(const CLineUpSlotScript& _Other)
	: CScript(LINEUPSLOTSCRIPT)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_SelectSlot(false)
	, m_Gamer(nullptr)
{
}

CLineUpSlotScript::~CLineUpSlotScript()
{
}


void CLineUpSlotScript::begin()
{
	if (nullptr != m_Gamer)
	{
		TEAM team = GETGAMER(m_Gamer)->GetGamerTeam();

		if (TEAM::BLUE == team)
			MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		else if (TEAM::RED == team)
			MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 3);
	}

	SetSlotInfo();
}

void CLineUpSlotScript::tick()
{
	CheckMousePos();

	SetSlotPos();
}

void CLineUpSlotScript::SetSlotInfo()
{
	CGameObject* icon = new CGameObject;
	icon->SetName(L"GamerIcon");
	icon->AddComponent(new CTransform);
	icon->AddComponent(new CMeshRender);
	icon->Transform()->SetRelativePos(Vec3(-50.f, 20.f, -5.f));
	icon->Transform()->SetRelativeScale(Vec3(96.f, 96.f, 1.f));
	icon->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	icon->MeshRender()->SetMaterial(MeshRender()->GetDynamicMaterial());
	icon->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Ptr<CTexture> pTex = GETGAMER(m_Gamer)->GetGamerTexture();
	icon->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	GetOwner()->AddChild(icon);

	wstring GamerName = GETGAMER(m_Gamer)->GetGamerName();
	wstring GamerATK = ToWString(std::to_string(GETGAMER(m_Gamer)->GetATK()));
	wstring GamerDEF = ToWString(std::to_string(GETGAMER(m_Gamer)->GetDEF()));

	// 선수 이름
	CGameObject* Text = new CGameObject;
	Text->SetName(L"GamerName");
	Text->AddComponent(new CTransform);
	Text->AddComponent(new CTextRender);
	Text->TextRender()->SetString(GamerName);
	Text->TextRender()->SetFont(L"Galmuri14");
	Text->TextRender()->SetFontSize(16.f);
	Text->TextRender()->SetFontColor(255, 255, 255, 255);
	Text->TextRender()->SetOffsetPos(Vec3(-65.f, -70.f, -1.f));
	GetOwner()->AddChild(Text);

	// 선수 공격력
	Text = new CGameObject;
	Text->SetName(L"GamerATK");
	Text->AddComponent(new CTransform);
	Text->AddComponent(new CTextRender);
	Text->TextRender()->SetString(GamerATK);
	Text->TextRender()->SetFont(L"Galmuri14");
	Text->TextRender()->SetFontSize(16.f);
	Text->TextRender()->SetFontColor(255, 255, 255, 255);
	Text->TextRender()->SetOffsetPos(Vec3(-36.f, 15.f, -1.f));
	GetOwner()->AddChild(Text);

	// 선수 방어력
	Text = new CGameObject;
	Text->SetName(L"GamerDEF");
	Text->AddComponent(new CTransform);
	Text->AddComponent(new CTextRender);
	Text->TextRender()->SetString(GamerDEF);
	Text->TextRender()->SetFont(L"Galmuri14");
	Text->TextRender()->SetFontSize(16.f);
	Text->TextRender()->SetFontColor(255, 255, 255, 255);
	Text->TextRender()->SetOffsetPos(Vec3(36.f, 15.f, -1.f));
	GetOwner()->AddChild(Text);
}

void CLineUpSlotScript::CheckMousePos()
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

	if (m_bMouseOn)
	{
		if (m_bMouseOn_Prev != m_bMouseOn)
			OnHovered();

		if (bLbtnReleased)
		{
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

		if (bLbtnReleased)
		{
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
}

void CLineUpSlotScript::OnHovered()
{
	if (!m_SelectSlot)
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CLineUpSlotScript::OnUnHovered()
{
	if (!m_SelectSlot)
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CLineUpSlotScript::LBtnClicked()
{
	if (m_bMouseOn)
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);
		m_SelectSlot = true;

		GetSlotIndex();

		if (-1 != CTGMgr::GetInst()->G_FirstSlot && -1 != CTGMgr::GetInst()->G_SecondSlot)
		{
			SwapSlot();
			CTGMgr::GetInst()->G_FirstSlot = -1;
			CTGMgr::GetInst()->G_SecondSlot = -1;
			MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
			m_SelectSlot = false;
		}
	}
	else
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		m_SelectSlot = false;
	}
}

void CLineUpSlotScript::SetSlotPos()
{
	for (size_t i = 0; i < CTGMgr::GetInst()->G_ShortlistSlot.size(); ++i)
	{
		if (0 == i)
			CTGMgr::GetInst()->G_ShortlistSlot[i]->Transform()->SetRelativePos(Vec3(-436.f, 26.f, 50.f));
		else if (1 == i)
			CTGMgr::GetInst()->G_ShortlistSlot[i]->Transform()->SetRelativePos(Vec3(-264.f, 26.f, 50.f));
		else if (2 == i)
			CTGMgr::GetInst()->G_ShortlistSlot[i]->Transform()->SetRelativePos(Vec3(44.f, 26.f, 50.f));

	}
}

void CLineUpSlotScript::GetSlotIndex()
{
	for (size_t i = 0; i < CTGMgr::GetInst()->G_ShortlistSlot.size(); ++i)
	{
		if (-1 == CTGMgr::GetInst()->G_FirstSlot 
			&& CTGMgr::GetInst()->G_ShortlistSlot[i]->GetScript<CLineUpSlotScript>()->IsSelect())
		{
			CTGMgr::GetInst()->G_FirstSlot = i;
		}
		else if (-1 != CTGMgr::GetInst()->G_FirstSlot
			&& CTGMgr::GetInst()->G_FirstSlot != i
			&& CTGMgr::GetInst()->G_ShortlistSlot[i]->GetScript<CLineUpSlotScript>()->IsSelect())
		{
			CTGMgr::GetInst()->G_SecondSlot = i;
		}
	}
}

void CLineUpSlotScript::SwapSlot()
{
	swap(CTGMgr::GetInst()->G_ShortlistSlot[CTGMgr::GetInst()->G_FirstSlot]
		, CTGMgr::GetInst()->G_ShortlistSlot[CTGMgr::GetInst()->G_SecondSlot]);
}
