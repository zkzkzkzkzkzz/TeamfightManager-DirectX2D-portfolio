#include "pch.h"
#include "CLineUpSlotScript.h"

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

void CLineUpSlotScript::OnHovered()
{
}

void CLineUpSlotScript::OnUnHovered()
{
}

void CLineUpSlotScript::LBtnDown()
{
}

void CLineUpSlotScript::LBtnUp()
{
}

void CLineUpSlotScript::LBtnReleased()
{
}

void CLineUpSlotScript::LBtnClicked()
{
}
