#include "pch.h"
#include "CNewBtnScript.h"

#include <Engine\CEngine.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAnim.h>

CNewBtnScript::CNewBtnScript()
	: CScript(NEWBTNSCRIPT)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_CurImg(nullptr)
	, m_LeftSword(nullptr)
	, m_RightSword(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{

}

CNewBtnScript::CNewBtnScript(const CNewBtnScript& _Other)
	: CScript(NEWBTNSCRIPT)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_CurImg(nullptr)
	, m_LeftSword(nullptr)
	, m_RightSword(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{

}

CNewBtnScript::~CNewBtnScript()
{
	if (nullptr != m_LeftSword)
		m_LeftSword->Destroy();

	if (nullptr != m_RightSword)
		m_RightSword->Destroy();
}


void CNewBtnScript::begin()
{
	m_NormalImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_NewGame_Idle.png", L"texture\\Title\\Title_NewGame_Idle.png");
	m_HoverImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_NewGame_Over.png", L"texture\\Title\\Title_NewGame_Over.png");
	m_CurImg = m_NormalImg;

	m_LeftSword = new CGameObject;
	m_LeftSword->SetName(L"LeftSword");
	m_LeftSword->AddComponent(new CTransform);
	m_LeftSword->AddComponent(new CMeshRender);
	m_LeftSword->AddComponent(new CAnimator2D);
	m_LeftSword->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_LeftSword->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LeftSwordMtrl"));
	m_LeftSword->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
	m_LeftSword->Animator2D()->LoadAnimation(L"animdata\\LeftSword.txt");
	m_LeftSword->SetActive(false);
	GetOwner()->AddChild(m_LeftSword);

	m_RightSword = new CGameObject;
	m_RightSword->SetName(L"RightSword");
	m_RightSword->AddComponent(new CTransform);
	m_RightSword->AddComponent(new CMeshRender);
	m_RightSword->AddComponent(new CAnimator2D);
	m_RightSword->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_RightSword->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RightSwordMtrl"));
	m_RightSword->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
	m_RightSword->Animator2D()->LoadAnimation(L"animdata\\RightSword.txt");
	m_RightSword->SetActive(false);
	GetOwner()->AddChild(m_RightSword);
}

void CNewBtnScript::tick()
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

void CNewBtnScript::render()
{
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
	if (m_HoverImg == m_CurImg && nullptr != m_CurImg)
	{
		CursorAnim();
	}
}

void CNewBtnScript::OnHovered()
{
	m_CurImg = m_HoverImg;
	m_LeftSword->SetActive(true);
	m_RightSword->SetActive(true);
}

void CNewBtnScript::OnUnHovered()
{
	m_CurImg = m_NormalImg;
	m_LeftSword->SetActive(false);
	m_RightSword->SetActive(false);
}

void CNewBtnScript::LBtnDown()
{
	m_CurImg = m_NormalImg;
	m_LeftSword->SetActive(false);
	m_RightSword->SetActive(false);
}

void CNewBtnScript::LBtnUp()
{
	m_CurImg = m_NormalImg;
	m_LeftSword->SetActive(false);
	m_RightSword->SetActive(false);
}

void CNewBtnScript::LBtnReleased()
{
	m_CurImg = m_NormalImg;
	m_bMouseLBtnDown = false;
	m_LeftSword->SetActive(false);
	m_RightSword->SetActive(false);
}

void CNewBtnScript::LBtnClicked()
{
	m_CurImg = m_NormalImg;
	m_LeftSword->SetActive(false);
	m_RightSword->SetActive(false);

	// CallBack
	if (m_CallBackFunc) m_CallBackFunc();

	// Delegate
	if (m_Inst != nullptr && m_Delegate != nullptr)
		(m_Inst->*m_Delegate)();
}

void CNewBtnScript::CursorAnim()
{
	Vec3 vWorldPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vWorldScale = GetOwner()->Transform()->GetWorldScale();

	Vec3 vLeftSword = Vec3(vWorldPos.x - vWorldScale.x / 2.f - 30.f, 0.f, 0.f);
	Vec3 vRightSword = Vec3(vWorldPos.x + vWorldScale.x / 2.f + 30.f, 0.f, 0.f);

	m_LeftSword->Transform()->SetRelativePos(vLeftSword);
	m_LeftSword->Transform()->SetRelativeScale(Vec3(36.f, 14.f, 1.f));
	m_RightSword->Transform()->SetRelativePos(vRightSword);
	m_RightSword->Transform()->SetRelativeScale(Vec3(36.f, 14.f, 1.f));
	m_LeftSword->Animator2D()->Play(L"LeftSword");
	m_RightSword->Animator2D()->Play(L"RightSword");
}