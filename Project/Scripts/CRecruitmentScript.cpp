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
	m_PanelTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruitment_ui_bg.png",
													L"texture\\Lobby\\recruitment\\recruitment_ui_bg.png");

	GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, 150.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(1087.f, 548.f, 1.f));

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RecruitmentMtrl"));
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

}

void CRecruitmentScript::tick()
{
}

void CRecruitmentScript::render()
{
	//if (nullptr != m_PanelTex)
	//	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_PanelTex);
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
