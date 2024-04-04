#include "pch.h"
#include "CLineUpSlotScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>
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
	}
	else
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		m_SelectSlot = false;
	}
}
