#include "pch.h"
#include "CNextBtnScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>

CNextBtnScript::CNextBtnScript()
	: CScript(NEXTBTNSCRIPT)
{
}

CNextBtnScript::CNextBtnScript(const CNextBtnScript& _Other)
	: CScript(NEXTBTNSCRIPT)
{
}

CNextBtnScript::~CNextBtnScript()
{
}


void CNextBtnScript::begin()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CNextBtnScript::tick()
{
	CheckMousePos();
}


void CNextBtnScript::CheckMousePos()
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


void CNextBtnScript::OnHovered()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CNextBtnScript::OnUnHovered()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CNextBtnScript::LBtnClicked()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}