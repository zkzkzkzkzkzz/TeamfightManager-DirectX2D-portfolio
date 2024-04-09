#include "pch.h"
#include "CTeamSlotScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>

#include "CGamerScript.h"
#include "CChampSlotScript.h"
#include "CBlueTeamSlotScript.h"
#include "CRedTeamSlotScript.h"

CTeamSlotScript::CTeamSlotScript()
	: CScript(TEAMSLOTSCRIPT)
	, m_Gamer(nullptr)
	, m_Team(TEAM::NONE)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
	, m_SubSlot(nullptr)
	, m_Level(nullptr)
{
}

CTeamSlotScript::CTeamSlotScript(const CTeamSlotScript& _Origin)
	: CScript(TEAMSLOTSCRIPT)
	, m_Gamer(nullptr)
	, m_Team(TEAM::NONE)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
	, m_SubSlot(nullptr)
	, m_Level(nullptr)
{
}

CTeamSlotScript::~CTeamSlotScript()
{
}

void CTeamSlotScript::begin()
{
	m_Level = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();

	SetSlotInfo();

	m_Team = GETGAMER(m_Gamer)->GetGamerTeam();

	if (TEAM::BLUE == m_Team)
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

		m_SubSlot = new CGameObject;
		m_SubSlot->AddComponent(new CBlueTeamSlotScript);
		m_SubSlot->GetScript<CBlueTeamSlotScript>()->SetGamer(m_Gamer);
		GetOwner()->AddChild(m_SubSlot);
		m_SubSlot->SetActive(false);
	}
	else
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);

		m_SubSlot = new CGameObject;
		m_SubSlot->AddComponent(new CRedTeamSlotScript);
		m_SubSlot->GetScript<CRedTeamSlotScript>()->SetGamer(m_Gamer);
		GetOwner()->AddChild(m_SubSlot);
		m_SubSlot->SetActive(false);
	}
}

void CTeamSlotScript::tick()
{
	if (!m_bUIPos)
	{
		if (TEAM::BLUE == m_Team)
			SetBlueSlotPos();
		else
			SetRedSlotPos();
	}

	if (m_bUIPos && BANPICK_STATE::BATTLE == m_Level->GetCurBanPickState())
	{
		m_SubSlot->SetActive(true);
	}
}


void CTeamSlotScript::SetBlueSlotPos()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.15f)
	{
		vPos.x += 750.f * DT * BtwTime;
		if (vPos.x >= -562.f)
			vPos.x = -562.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.15f && m_UIPosTime < 1.f)
	{
		vPos.x += 100.f * DT * BtwTime;
		if (vPos.x >= -562.f)
			vPos.x = -562.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.x = -562.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}

void CTeamSlotScript::SetRedSlotPos()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.15f)
	{
		vPos.x -= 750.f * DT * BtwTime;
		if (vPos.x <= 562.f)
			vPos.x = 562.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.15f && m_UIPosTime < 1.f)
	{
		vPos.x -= 100.f * DT * BtwTime;
		if (vPos.x <= 562.f)
			vPos.x = 562.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.x = 562.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}

void CTeamSlotScript::SetSlotInfo()
{
	CGameObject* icon = new CGameObject;
	icon->SetName(L"GamerIcon");
	icon->AddComponent(new CTransform);
	icon->AddComponent(new CMeshRender);
	icon->Transform()->SetRelativePos(Vec3(-51.f, 15.f, -5.f));
	icon->Transform()->SetRelativeScale(Vec3(128.f, 128.f, 1.f));
	icon->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	icon->MeshRender()->SetMaterial(MeshRender()->GetDynamicMaterial());
	icon->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Ptr<CTexture> pTex = GETGAMER(m_Gamer)->GetGamerTexture();
	icon->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	GetOwner()->AddChild(icon);

	wstring GamerName = GETGAMER(m_Gamer)->GetGamerName();
	wstring GamerATK = ToWString(std::to_string(GETGAMER(m_Gamer)->GetATK()));
	wstring GamerDEF = ToWString(std::to_string(GETGAMER(m_Gamer)->GetDEF()));

	// 선수 이름
	CGameObject* Text = new CGameObject;
	Text->SetName(L"GamerName");
	Text->AddComponent(new CTransform);
	Text->AddComponent(new CTextRender);
	Text->TextRender()->SetString(GamerName);
	Text->TextRender()->SetFont(L"Galmuri14");
	Text->TextRender()->SetFontSize(16.f);
	Text->TextRender()->SetFontColor(255, 255, 255, 255);
	Text->TextRender()->SetOffsetPos(Vec3(-65.f, -70.f, -1.f));
	GetOwner()->AddChild(Text);

	// 선수 공격력
	Text = new CGameObject;
	Text->SetName(L"GamerATK");
	Text->AddComponent(new CTransform);
	Text->AddComponent(new CTextRender);
	Text->TextRender()->SetString(GamerATK);
	Text->TextRender()->SetFont(L"Galmuri14");
	Text->TextRender()->SetFontSize(16.f);
	Text->TextRender()->SetFontColor(255, 255, 255, 255);
	Text->TextRender()->SetOffsetPos(Vec3(-36.f, 15.f, -1.f));
	GetOwner()->AddChild(Text);

	// 선수 방어력
	Text = new CGameObject;
	Text->SetName(L"GamerDEF");
	Text->AddComponent(new CTransform);
	Text->AddComponent(new CTextRender);
	Text->TextRender()->SetString(GamerDEF);
	Text->TextRender()->SetFont(L"Galmuri14");
	Text->TextRender()->SetFontSize(16.f);
	Text->TextRender()->SetFontColor(255, 255, 255, 255);
	Text->TextRender()->SetOffsetPos(Vec3(36.f, 15.f, -1.f));
	GetOwner()->AddChild(Text);
}
