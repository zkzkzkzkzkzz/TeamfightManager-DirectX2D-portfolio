#include "pch.h"
#include "CBattleStartBtnScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include "CBanpickLevel.h"

CBattleStartBtnScript::CBattleStartBtnScript()
	: CScript(BATTLESTARTBTNSCRIPT)
	, m_Text(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CBattleStartBtnScript::CBattleStartBtnScript(const CBattleStartBtnScript& _Origin)
	: CScript(BATTLESTARTBTNSCRIPT)
	, m_Text(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CBattleStartBtnScript::~CBattleStartBtnScript()
{
}


void CBattleStartBtnScript::begin()
{
	Transform()->SetRelativePos(Vec3(0.f, -78.f, -10.f));
	Transform()->SetRelativeScale(Vec3(160.f, 60.f, 1.f));
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BtnMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	m_Text = new CGameObject;
	m_Text->SetName(L"StartBtnText");
	m_Text->AddComponent(new CTransform);
	m_Text->AddComponent(new CTextRender);
	m_Text->TextRender()->SetString(L"Start");
	m_Text->TextRender()->SetFont(L"Galmuri14");
	m_Text->TextRender()->SetFontSize(30.f);
	m_Text->TextRender()->SetFontColor(255, 255, 255, 255);
	m_Text->TextRender()->SetOffsetPos(Vec3(-42.f, -12.f, -10.f));
	GetOwner()->AddChild(m_Text);
	m_Text->SetActive(false);
}

void CBattleStartBtnScript::tick()
{
	if (GetOwner()->IsActive())
		m_Text->SetActive(true);

	CheckMousePos();
}

void CBattleStartBtnScript::CheckMousePos()
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
	}
}

void CBattleStartBtnScript::OnHovered()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CBattleStartBtnScript::OnUnHovered()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CBattleStartBtnScript::LBtnClicked()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	CBanpickLevel* pLevel = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();
	pLevel->SetBanPickState(BANPICK_STATE::BATTLE);

	m_Text->SetActive(false);
	GetOwner()->SetActive(false);
}