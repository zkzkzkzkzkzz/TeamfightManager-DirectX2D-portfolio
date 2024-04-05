#include "pch.h"
#include "CTeamHeaderScript.h"

#include <Engine\CEngine.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAnim.h>

CTeamHeaderScript::CTeamHeaderScript()
	: CScript(TEAMHEADERSCRIPT)
	, m_HeaderTex(nullptr)
	, m_BlueTeam(nullptr)
	, m_RedTeam(nullptr)
	, m_BlueTeamLogo(nullptr)
	, m_RedTeamLogo(nullptr)
	, m_Time(nullptr)
	, m_strTime{} 
	, m_BlueScore(nullptr)
	, m_BScore{}
	, m_RedScore(nullptr)
	, m_RScore{}
	, m_BlueTeamName(nullptr)
	, m_RedTeamName(nullptr)
	, m_UIPosTime(0.f)
{
}

CTeamHeaderScript::CTeamHeaderScript(const CTeamHeaderScript& _Origin)
	: CScript(TEAMHEADERSCRIPT)
	, m_HeaderTex(_Origin.m_HeaderTex)
	, m_BlueTeam(nullptr)
	, m_RedTeam(nullptr)
	, m_BlueTeamLogo(_Origin.m_BlueTeamLogo)
	, m_RedTeamLogo(_Origin.m_RedTeamLogo)
	, m_Time(nullptr)
	, m_strTime{}
	, m_BlueScore(nullptr)
	, m_BScore{}
	, m_RedScore(nullptr)
	, m_RScore{}
	, m_BlueTeamName(nullptr)
	, m_RedTeamName(nullptr)
	, m_UIPosTime(0.f)
{
}

CTeamHeaderScript::~CTeamHeaderScript()
{
}

void CTeamHeaderScript::begin()
{
	Transform()->SetRelativePos(Vec3(0.f, 450.f, 2000.f));
	Transform()->SetRelativeScale(Vec3(1280.f, 82.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"InGameUIMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	m_HeaderTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\IngameUI\\ingameheader_bg.png"
														, L"texture\\Champ\\IngameUI\\ingameheader_bg.png");


	InitBlueTeam();
	InitRedTeam();
	InitTimer();
	InitTeamName();
}

void CTeamHeaderScript::tick()
{
	int CurTime = CheckTime();

	m_UIPosTime += DT;

	SetHeaderPos();

	SetTimer(CurTime);
	CheckScore();
	render();
}

void CTeamHeaderScript::render()
{
	if (nullptr != m_HeaderTex)
		MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HeaderTex);

	if (nullptr != m_BlueTeamLogo)
		m_BlueTeam->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_1, m_BlueTeamLogo);

	if (nullptr != m_RedTeamLogo)
		m_RedTeam->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_2, m_RedTeamLogo);
}


void CTeamHeaderScript::CheckScore()
{
	int BlueScore = CTGMgr::G_BlueKillScore;
	m_BScore = ToWString(std::to_string(BlueScore));
	m_BlueScore->TextRender()->SetString(m_BScore);

	int RedScore = CTGMgr::G_RedKillScore;
	m_RScore = ToWString(std::to_string(RedScore));
	m_RedScore->TextRender()->SetString(m_RScore);
}

int CTeamHeaderScript::CheckTime()
{
	return CTGMgr::G_Time;
}

void CTeamHeaderScript::SetTimer(int _CurTime)
{
	m_strTime = L": ";

	m_strTime += ToWString(std::to_string(_CurTime));

	m_Time->TextRender()->SetString(m_strTime);
}


void CTeamHeaderScript::InitBlueTeam()
{
	m_BlueTeam = new CGameObject;
	m_BlueTeam->SetName(L"BlueTeamLogo");
	m_BlueTeam->AddComponent(new CTransform);
	m_BlueTeam->AddComponent(new CMeshRender);
	m_BlueTeam->Transform()->SetRelativePos(Vec3(-599.f, 7.f, -10.f));
	m_BlueTeam->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 1.f));
	m_BlueTeam->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_BlueTeam->MeshRender()->SetMaterial(MeshRender()->GetDynamicMaterial());
	m_BlueTeam->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	GetOwner()->AddChild(m_BlueTeam);
	m_BlueTeamLogo = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\IngameUI\\captains.png"
														, L"texture\\Champ\\IngameUI\\captains.png");


	m_BScore = L"0";
	m_BlueScore = new CGameObject;
	m_BlueScore->SetName(L"BlueScore");
	m_BlueScore->AddComponent(new CTransform);
	m_BlueScore->AddComponent(new CTextRender);
	m_BlueScore->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	m_BlueScore->TextRender()->SetString(m_BScore);
	m_BlueScore->TextRender()->SetFont(L"Galmuri14");
	m_BlueScore->TextRender()->SetFontSize(35.f);
	m_BlueScore->TextRender()->SetFontColor(255, 255, 255, 255);
	m_BlueScore->TextRender()->SetOffsetPos(Vec3(-101.f, -29.f, 0.f));
	GetOwner()->AddChild(m_BlueScore);
}

void CTeamHeaderScript::InitRedTeam()
{
	m_RedTeam = new CGameObject;
	m_RedTeam->SetName(L"RedTeamLogo");
	m_RedTeam->AddComponent(new CTransform);
	m_RedTeam->AddComponent(new CMeshRender);
	m_RedTeam->Transform()->SetRelativePos(Vec3(599.f, 7.f, -10.f));
	m_RedTeam->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 1.f));
	m_RedTeam->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_RedTeam->MeshRender()->SetMaterial(MeshRender()->GetDynamicMaterial());
	m_RedTeam->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);
	GetOwner()->AddChild(m_RedTeam);
	m_RedTeamLogo = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\IngameUI\\gorillas.png"
														, L"texture\\Champ\\IngameUI\\gorillas.png");

	m_RScore = L"0";
	m_RedScore = new CGameObject;
	m_RedScore->SetName(L"RedScore");
	m_RedScore->AddComponent(new CTransform);
	m_RedScore->AddComponent(new CTextRender);
	m_RedScore->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	m_RedScore->TextRender()->SetString(m_RScore);
	m_RedScore->TextRender()->SetFont(L"Galmuri14");
	m_RedScore->TextRender()->SetFontSize(30.f);
	m_RedScore->TextRender()->SetFontColor(255, 255, 255, 255);
	m_RedScore->TextRender()->SetOffsetPos(Vec3(80.f, -29.f, 0.f));
	GetOwner()->AddChild(m_RedScore);
}

void CTeamHeaderScript::InitTimer()
{
	m_strTime = L": ";
	m_Time = new CGameObject;
	m_Time->SetName(L"Time");
	m_Time->AddComponent(new CTransform);
	m_Time->AddComponent(new CTextRender);
	m_Time->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	m_Time->TextRender()->SetString(m_strTime);
	m_Time->TextRender()->SetFont(L"Galmuri14");
	m_Time->TextRender()->SetFontSize(15.f);
	m_Time->TextRender()->SetFontColor(255, 255, 255, 255);
	m_Time->TextRender()->SetOffsetPos(Vec3(-13.f, 22.f, 0.f));
	GetOwner()->AddChild(m_Time);
}

void CTeamHeaderScript::InitTeamName()
{
	m_BlueTeamName = new CGameObject;
	m_BlueTeamName->SetName(L"BlueTeamName");
	m_BlueTeamName->AddComponent(new CTransform);
	m_BlueTeamName->AddComponent(new CTextRender);
	m_BlueTeamName->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	m_BlueTeamName->TextRender()->SetString(L"AR49");
	m_BlueTeamName->TextRender()->SetFont(L"Galmuri14");
	m_BlueTeamName->TextRender()->SetFontSize(45.f);
	m_BlueTeamName->TextRender()->SetFontColor(255, 255, 255, 255);
	m_BlueTeamName->TextRender()->SetOffsetPos(Vec3(-449.f, -26.f, 0.f));
	GetOwner()->AddChild(m_BlueTeamName);

	m_RedTeamName = new CGameObject;
	m_RedTeamName->SetName(L"RedTeamName");
	m_RedTeamName->AddComponent(new CTransform);
	m_RedTeamName->AddComponent(new CTextRender);
	m_RedTeamName->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	m_RedTeamName->TextRender()->SetString(L"OOH EEH AH AH");
	m_RedTeamName->TextRender()->SetFont(L"Galmuri14");
	m_RedTeamName->TextRender()->SetFontSize(45.f);
	m_RedTeamName->TextRender()->SetFontColor(255, 255, 255, 255);
	m_RedTeamName->TextRender()->SetOffsetPos(Vec3(184.f, -26.f, 0.f));
	GetOwner()->AddChild(m_RedTeamName);
}


void CTeamHeaderScript::SetHeaderPos()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.15f)
	{
		vPos.y -= 1300.f * DT * BtwTime;
		if (vPos.y <= 324.f)
			vPos.y = 324.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.15f && m_UIPosTime < 1.f)
	{
		vPos.y -= 200.f * DT * BtwTime;
		if (vPos.y <= 324.f)
			vPos.y = 324.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = 324.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
	}
}