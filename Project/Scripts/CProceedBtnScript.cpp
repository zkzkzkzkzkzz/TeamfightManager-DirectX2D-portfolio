#include "pch.h"
#include "CProceedBtnScript.h"

#include <Engine\CEngine.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CGameObject.h>
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
	, m_Arrow(nullptr)
	, m_AnimImg(nullptr)
	, m_Time(0.f)
	, m_Duration(0.9f)
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

	m_AnimImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\btn\\proceed\\proceed_button_2.png", L"texture\\Lobby\\btn\\proceed\\proceed_button_2.png");
	m_Arrow = new CGameObject;
	m_Arrow->AddComponent(new CTransform);
	m_Arrow->AddComponent(new CMeshRender);
	m_Arrow->Transform()->SetRelativePos(Vec3(65.f, 0.f, -1.f));
	m_Arrow->Transform()->SetRelativeScale(Vec3(126.f, 88.f, 1.f));
	m_Arrow->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Arrow->MeshRender()->SetMaterial(GetOwner()->MeshRender()->GetDynamicMaterial());
	m_Arrow->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_AnimImg);
	GetOwner()->AddChild(m_Arrow);
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

	if (nullptr != m_CurImg)
	{
		if (m_CurImg == m_HoverImg)
		{
			MoveArrow();
		}
		else
		{
			ResetArrow();
		}
	}
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

void CProceedBtnScript::MoveArrow()
{
	m_Time += DT;

	float BtwTime = m_Duration - m_Time;

	Vec3 vPos = m_Arrow->Transform()->GetRelativePos();

	if (0.7 > m_Time)
	{
		if (vPos.x >= 65.f)
			vPos.x = 30.f;

		vPos.x += 130.f * DT * BtwTime;

		if (vPos.x >= 64.f)
			vPos.x = 64.f;
	}
	else if (m_Time < m_Duration && m_Time >= 0.7f && vPos.x >= 65.f)
	{
		vPos.x = 65.f;
	}
	else if (m_Time >= m_Duration)
	{
		vPos.x = 65.f;
		m_Time = 0.f;
	}

	m_Arrow->Transform()->SetRelativePos(vPos);
}

void CProceedBtnScript::ResetArrow()
{
	m_Time = 0.f;
	m_Arrow->Transform()->SetRelativePos(Vec3(65.f, 0.f, -1.f));
}