#include "pch.h"
#include "CBattleResultScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

CBattleResultScript::CBattleResultScript()
	: CScript(BATTLERESULTSCRIPT)
	, m_bUIPos(false)
	, m_bOpen(false)
	, m_UIPosTime(0.f)
	, m_OpenTime(0.f)
	, m_Level(nullptr)
	, m_WinText(nullptr)
	, m_BlueScoreText(nullptr)
	, m_RedScoreText(nullptr)
	, m_MiddleText(nullptr)
	, m_BlueTeamText(nullptr)
	, m_RedTeamText(nullptr)
{
}

CBattleResultScript::CBattleResultScript(const CBattleResultScript& _Origin)
	: CScript(BATTLERESULTSCRIPT)
	, m_bUIPos(false)
	, m_bOpen(false)
	, m_UIPosTime(0.f)
	, m_OpenTime(0.f)
	, m_Level(nullptr)
	, m_WinText(nullptr)
	, m_BlueScoreText(nullptr)
	, m_RedScoreText(nullptr)
	, m_MiddleText(nullptr)
	, m_BlueTeamText(nullptr)
	, m_RedTeamText(nullptr)
{
}

CBattleResultScript::~CBattleResultScript()
{
}

void CBattleResultScript::begin()
{
	m_Level = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();

	Transform()->SetRelativePos(Vec3(0.f, 800.f, 2000.f));

	vector<CGameObject*> Logos = GetOwner()->GetChild();

	for (size_t i = 0; i < Logos.size(); ++i)
	{
		if (L"ResultTeamLogo1" == Logos[i]->GetName())
		{
			Logos[i]->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		}
		else if (L"ResultTeamLogo2" == Logos[i]->GetName())
		{
			Logos[i]->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);
		}
	}

	InitResultText();
}

void CBattleResultScript::tick()
{
	m_OpenTime += DT;

	if (BANPICK_STATE::DONE == m_Level->GetCurBanPickState() && !m_bOpen)
	{
		m_OpenTime = 0.f;
		m_bOpen = true;
	}

	if (BANPICK_STATE::DONE == m_Level->GetCurBanPickState() && !m_bUIPos && m_OpenTime > 1.f)
		OpenResult();
}

void CBattleResultScript::OpenResult()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.13f)
	{
		vPos.y -= 6800.f * DT * BtwTime;
		if (vPos.y <= 0.f)
			vPos.y = 0.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.13f && m_UIPosTime < 1.f)
	{
		vPos.y -= 680.f * DT * BtwTime;
		if (vPos.y <= 0.f)
			vPos.y = 0.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = 0.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}


void CBattleResultScript::InitResultText()
{
	m_WinText = new CGameObject;
	m_WinText->SetName(L"WinText");
	m_WinText->AddComponent(new CTransform);
	m_WinText->AddComponent(new CTextRender);
	m_WinText->TextRender()->SetString(L"WIN");
	m_WinText->TextRender()->SetFont(L"Galmuri14");
	m_WinText->TextRender()->SetFontSize(50.f);
	m_WinText->TextRender()->SetFontColor(0, 255, 0, 255);
	m_WinText->TextRender()->SetOffsetPos(Vec3(-37.f, -77.f, -10.f));
	GetOwner()->AddChild(m_WinText);

	m_BlueScoreText = new CGameObject;
	m_BlueScoreText->SetName(L"BlueScoreText");
	m_BlueScoreText->AddComponent(new CTransform);
	m_BlueScoreText->AddComponent(new CTextRender);
	m_BlueScoreText->TextRender()->SetString(L"1");
	m_BlueScoreText->TextRender()->SetFont(L"Galmuri14");
	m_BlueScoreText->TextRender()->SetFontSize(50.f);
	m_BlueScoreText->TextRender()->SetFontColor(0, 255, 0, 255);
	m_BlueScoreText->TextRender()->SetOffsetPos(Vec3(-49.f, 30.f, -10.f));
	GetOwner()->AddChild(m_BlueScoreText);
	
	m_RedScoreText = new CGameObject;
	m_RedScoreText->SetName(L"RedScoreText");
	m_RedScoreText->AddComponent(new CTransform);
	m_RedScoreText->AddComponent(new CTextRender);
	m_RedScoreText->TextRender()->SetString(L"0");
	m_RedScoreText->TextRender()->SetFont(L"Galmuri14");
	m_RedScoreText->TextRender()->SetFontSize(50.f);
	m_RedScoreText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_RedScoreText->TextRender()->SetOffsetPos(Vec3(34.f, 30.f, -10.f));
	GetOwner()->AddChild(m_RedScoreText);

	m_MiddleText = new CGameObject;
	m_MiddleText->SetName(L"MiddleText");
	m_MiddleText->AddComponent(new CTransform);
	m_MiddleText->AddComponent(new CTextRender);
	m_MiddleText->TextRender()->SetString(L":");
	m_MiddleText->TextRender()->SetFont(L"Galmuri14");
	m_MiddleText->TextRender()->SetFontSize(50.f);
	m_MiddleText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_MiddleText->TextRender()->SetOffsetPos(Vec3(-1.f, 23.f, -10.f));
	GetOwner()->AddChild(m_MiddleText);

	m_BlueTeamText = new CGameObject;
	m_BlueTeamText->SetName(L"BlueTeamName");
	m_BlueTeamText->AddComponent(new CTransform);
	m_BlueTeamText->AddComponent(new CTextRender);
	m_BlueTeamText->TextRender()->SetString(L"AR49");
	m_BlueTeamText->TextRender()->SetFont(L"Galmuri14");
	m_BlueTeamText->TextRender()->SetFontSize(15.f);
	m_BlueTeamText->TextRender()->SetFontColor(255, 220, 0, 255);
	m_BlueTeamText->TextRender()->SetOffsetPos(Vec3(-147.f, 99.f, -10.f));
	GetOwner()->AddChild(m_BlueTeamText);

	m_RedTeamText = new CGameObject;
	m_RedTeamText->SetName(L"RedTeamName");
	m_RedTeamText->AddComponent(new CTransform);
	m_RedTeamText->AddComponent(new CTextRender);
	m_RedTeamText->TextRender()->SetString(L"OOH EEH AH AH");
	m_RedTeamText->TextRender()->SetFont(L"Galmuri14");
	m_RedTeamText->TextRender()->SetFontSize(15.f);
	m_RedTeamText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_RedTeamText->TextRender()->SetOffsetPos(Vec3(71.f, 99.f, -10.f));
	GetOwner()->AddChild(m_RedTeamText);
}
