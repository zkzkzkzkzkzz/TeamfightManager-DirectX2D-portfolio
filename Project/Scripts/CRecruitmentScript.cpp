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
{
}

CRecruitmentScript::~CRecruitmentScript()
{
}


void CRecruitmentScript::begin()
{
	m_PanelTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruitment_uifix.png",
													L"texture\\Lobby\\recruitment\\recruitment_uifix.png");

	GetOwner()->Transform()->SetRelativePos(Vec3(450.f, 0.f, 150.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(1200.f, 545.f, 1.f));

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RecruitmentMtrl"));
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
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

	render();
}

void CRecruitmentScript::render()
{
	if (nullptr != m_PanelTex)
		GetOwner()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_PanelTex);
}

void CRecruitmentScript::OnHovered()
{
}

void CRecruitmentScript::OnUnHovered()
{
}

void CRecruitmentScript::LBtnUp()
{
}

void CRecruitmentScript::LBtnReleased()
{
}

void CRecruitmentScript::LBtnClicked()
{
}
