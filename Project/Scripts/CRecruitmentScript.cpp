#include "pch.h"
#include "CRecruitmentScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>
#include <Engine\CFontMgr.h>

CRecruitmentScript::CRecruitmentScript()
	: CScript(RECRUITMENTSCRIPT)
	, m_Panel(nullptr)
	, m_PanelTex(nullptr)
	, m_CloseBtn(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_CurImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CRecruitmentScript::~CRecruitmentScript()
{
}


void CRecruitmentScript::begin()
{
	m_PanelTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruitment_uifix.png",
													L"texture\\Lobby\\recruitment\\recruitment_uifix.png");

	m_NormalImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\btn\\default_button_0.png",
													L"texture\\Lobby\\recruitment\\btn\\default_button_0.png");
	m_HoverImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\btn\\default_button_1.png",
													L"texture\\Lobby\\recruitment\\btn\\default_button_1.png");
	m_CurImg = m_NormalImg;

	GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, 150.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(332.f, 543.f, 1.f));
	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RecruitmentMtrl"));
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	m_CloseBtn = new CGameObject;
	m_CloseBtn->AddComponent(new CTransform);
	m_CloseBtn->AddComponent(new CMeshRender);
	m_CloseBtn->Transform()->SetRelativePos(Vec3(81.f, -232.f, -10.f));
	m_CloseBtn->Transform()->SetRelativeScale(Vec3(138.f, 50.f, 1.f));
	m_CloseBtn->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_CloseBtn->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LobbyBtnMtrl"));
	m_CloseBtn->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	GetOwner()->AddChild(m_CloseBtn);
	m_CloseBtn->SetActive(false);

	CGameObject* pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitmentText.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);

	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitClose.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);
}

void CRecruitmentScript::tick()
{
	vector<CGameObject*> pchild = GetOwner()->GetChild();
	
	if (GetOwner()->IsActive())
	{
		for (size_t i = 0; i < pchild.size(); ++i)
		{
			pchild[i]->SetActive(true);
		}
	}
	else
	{
		for (size_t i = 0; i < pchild.size(); ++i)
		{
			pchild[i]->SetActive(false);
		}
	}

	CheckMousePos();

	render();
}

void CRecruitmentScript::render()
{
	if (nullptr != m_PanelTex)
		GetOwner()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_PanelTex);

	if (nullptr != m_CurImg)
		m_CloseBtn->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
}

void CRecruitmentScript::OnHovered()
{
	m_CurImg = m_HoverImg;
}

void CRecruitmentScript::OnUnHovered()
{
	m_CurImg = m_NormalImg;
}

void CRecruitmentScript::LBtnUp()
{
	m_CurImg = m_NormalImg;
}

void CRecruitmentScript::LBtnReleased()
{
	m_CurImg = m_NormalImg;
}

void CRecruitmentScript::LBtnClicked()
{
	m_CurImg = m_HoverImg;

	vector<CGameObject*> pChild = GetOwner()->GetChild();

	for (size_t i = 0; i < pChild.size(); ++i)
	{
		pChild[i]->SetActive(false);
	}

	GetOwner()->SetActive(false);
}

void CRecruitmentScript::CheckMousePos()
{
	m_bMouseOn_Prev = m_bMouseOn;

	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	vMousePos.x -= vResolution.x / 2.f;
	vMousePos.y = -(vMousePos.y - vResolution.y / 2.f);

	Vec3 vRelativePos = m_CloseBtn->Transform()->GetRelativePos();
	Vec3 vWorldPos = m_CloseBtn->Transform()->GetRelativePos();
	Vec3 vWorldScale = m_CloseBtn->Transform()->GetRelativeScale();

	if (m_CloseBtn->GetParent())
	{
		vWorldPos += m_CloseBtn->GetParent()->Transform()->GetRelativePos();
	}


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
}
