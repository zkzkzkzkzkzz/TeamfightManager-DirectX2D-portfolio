#include "pch.h"
#include "CProceedBtnScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CMeshRender.h>

CProceedBtnScript::CProceedBtnScript()
    : CScript(PROCEEDBTNSCRIPT)
    , m_NormalImg(nullptr)
    , m_HoverImg(nullptr)
    , m_CurImg(nullptr)
    , m_bMouseOn(false)
    , m_bMouseOn_Prev(false)
    , m_bMouseLBtnDown(false)
{
}

CProceedBtnScript::~CProceedBtnScript()
{
}

void CProceedBtnScript::begin()
{
	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProceedBtnMtrl"));
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

    m_NormalImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\proceed\\proceed_button_0.png", L"texture\\Lobby\\btn\\proceed\\proceed_button_0.png");
    m_HoverImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\proceed\\proceed_button_1.png", L"texture\\Lobby\\btn\\proceed\\proceed_button_1.png");
    m_CurImg = m_NormalImg;

	GetOwner()->Transform()->SetRelativePos(Vec3(491.f, -295.f, 200.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(258.f, 88.f, 1.f));
}

void CProceedBtnScript::tick()
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

void CProceedBtnScript::render()
{
    if (nullptr != m_CurImg)
        GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
}


void CProceedBtnScript::OnHovered()
{
    m_CurImg = m_HoverImg;
}

void CProceedBtnScript::OnUnHovered()
{
    m_CurImg = m_NormalImg;
}

void CProceedBtnScript::LBtnUp()
{
    m_CurImg = m_NormalImg;
}

void CProceedBtnScript::LBtnClicked()
{
    m_CurImg = m_HoverImg;
}
