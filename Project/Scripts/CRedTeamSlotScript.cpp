#include "pch.h"
#include "CRedTeamSlotScript.h"

#include <Engine\CEngine.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include "CGamerScript.h"
#include "CChampScript.h"

CRedTeamSlotScript::CRedTeamSlotScript()
	: CScript(REDTEAMSLOTSCRIPT)
	, m_SlotTex(nullptr)
	, m_KDAText(nullptr)
	, m_InfoText(nullptr)
	, m_CurGamer(nullptr)
	, m_CurChamp(nullptr)
	, m_CurList(CHAMP_LIST::END)
	, Kill(0)
	, Dead(0)
	, Deal(0)
	, Damaged(0)
	, Heal(0)
	, m_Level(nullptr)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
	, m_KillText(nullptr)
	, m_DeadText(nullptr)
	, m_DealText(nullptr)
	, m_DamagedText(nullptr)
	, m_HealText(nullptr)
	, m_AssistText(nullptr)
{
}

CRedTeamSlotScript::CRedTeamSlotScript(const CRedTeamSlotScript& _Origin)
	: CScript(REDTEAMSLOTSCRIPT)
	, m_SlotTex(_Origin.m_SlotTex)
	, m_KDAText(nullptr)
	, m_InfoText(nullptr)
	, m_CurGamer(nullptr)
	, m_CurChamp(nullptr)
	, m_CurList(CHAMP_LIST::END)
	, Kill(0)
	, Dead(0)
	, Deal(0)
	, Damaged(0)
	, Heal(0)
	, m_Level(nullptr)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
	, m_KillText(nullptr)
	, m_DeadText(nullptr)
	, m_DealText(nullptr)
	, m_DamagedText(nullptr)
	, m_HealText(nullptr)
	, m_AssistText(nullptr)
{
}

CRedTeamSlotScript::~CRedTeamSlotScript()
{
}

void CRedTeamSlotScript::begin()
{
	m_Level = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();

	GetOwner()->SetName(L"SubSlot");
	GetOwner()->AddComponent(new CTransform);
	GetOwner()->AddComponent(new CMeshRender);
	Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	Transform()->SetRelativeScale(Vec3(82.f, 156.f, 0.f));
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"InGameUIMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	m_SlotTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\IngameUI\\data_ui_bg_1.png"
													, L"texture\\Champ\\IngameUI\\data_ui_bg_1.png");

	m_InfoText = new CGameObject;
	m_InfoText->SetName(L"Info");
	m_InfoText->AddComponent(new CTransform);
	m_InfoText->AddComponent(new CTextRender);
	m_InfoText->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	m_InfoText->TextRender()->SetString(L"Info");
	m_InfoText->TextRender()->SetFont(L"Galmuri14");
	m_InfoText->TextRender()->SetFontSize(15.f);
	m_InfoText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_InfoText->TextRender()->SetOffsetPos(Vec3(-14.f, -69.f, -10.f));
	GetOwner()->AddChild(m_InfoText);
	m_InfoText->SetActive(false);

	m_KDAText = new CGameObject;
	m_KDAText->SetName(L"KDA");
	m_KDAText->AddComponent(new CTransform);
	m_KDAText->AddComponent(new CTextRender);
	m_KDAText->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	m_KDAText->TextRender()->SetString(L"K    D    A");
	m_KDAText->TextRender()->SetFont(L"Galmuri14");
	m_KDAText->TextRender()->SetFontSize(10.f);
	m_KDAText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_KDAText->TextRender()->SetOffsetPos(Vec3(-28.f, -41.f, -10.f));
	GetOwner()->AddChild(m_KDAText);
	m_KDAText->SetActive(false);

	InitInGameInfo();
}

void CRedTeamSlotScript::tick()
{
	m_CurList = GETGAMER(m_CurGamer)->GetSelectedChamp();
	if (nullptr == m_CurChamp)
	{
		FindChampFromList();
	}

	if (BANPICK_STATE::BATTLE == m_Level->GetCurBanPickState() && !m_bUIPos)
	{
		OpenSlot();
		m_InfoText->SetActive(true);
		m_KDAText->SetActive(true);
	}

	if (nullptr != m_CurChamp)
	{
		CheckInGameInfo();
		RenderInGameInfo();
	}

	render();
}

void CRedTeamSlotScript::render()
{
	if (nullptr != m_SlotTex)
		MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_SlotTex);
}


void CRedTeamSlotScript::OpenSlot()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.15f)
	{
		vPos.x -= 750.f * DT * BtwTime;
		if (vPos.x <= -121.f)
			vPos.x = -121.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.15f && m_UIPosTime < 1.f)
	{
		vPos.x -= 100.f * DT * BtwTime;
		if (vPos.x <= -121.f)
			vPos.x = -121.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.x = -121.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}


void CRedTeamSlotScript::CheckInGameInfo()
{
	Kill = GETCHAMP(m_CurChamp)->m_InGameStatus.KillPoint;
	Dead = GETCHAMP(m_CurChamp)->m_InGameStatus.DeathPoint;
	Deal = GETCHAMP(m_CurChamp)->m_InGameStatus.TotalDeal;
	Damaged = GETCHAMP(m_CurChamp)->m_InGameStatus.TotalDamage;
	Heal = GETCHAMP(m_CurChamp)->m_InGameStatus.TotalHeal;
}

void CRedTeamSlotScript::RenderInGameInfo()
{
	m_KillText->TextRender()->SetString(ToWString(std::to_string(Kill)));
	m_DeadText->TextRender()->SetString(ToWString(std::to_string(Dead)));
	m_DealText->TextRender()->SetString(ToWString(std::to_string(Deal)));
	m_DamagedText->TextRender()->SetString(ToWString(std::to_string(Damaged)));
	m_HealText->TextRender()->SetString(ToWString(std::to_string(Heal)));
}

void CRedTeamSlotScript::FindChampFromList()
{
	switch (m_CurList)
	{
	case CHAMP_LIST::ARCHER:
		m_CurChamp = m_Level->FindObjectByName(L"Archer");
		break;
	case CHAMP_LIST::FIGHTER:
		m_CurChamp = m_Level->FindObjectByName(L"Fighter");
		break;
	case CHAMP_LIST::KNIGHT:
		m_CurChamp = m_Level->FindObjectByName(L"Knight");
		break;
	case CHAMP_LIST::MONK:
		m_CurChamp = m_Level->FindObjectByName(L"Monk");
		break;
	case CHAMP_LIST::NINJA:
		m_CurChamp = m_Level->FindObjectByName(L"Ninja");
		break;
	case CHAMP_LIST::PRIEST:
		m_CurChamp = m_Level->FindObjectByName(L"Priest");
		break;
	case CHAMP_LIST::END:
		break;
	default:
		break;
	}

	m_KillText->SetActive(true);
	m_DeadText->SetActive(true);
	m_AssistText->SetActive(true);
	m_DealText->SetActive(true);
	m_DamagedText->SetActive(true);
	m_HealText->SetActive(true);
}

void CRedTeamSlotScript::InitInGameInfo()
{
	m_KillText = new CGameObject;
	m_KillText->SetName(L"Kill");
	m_KillText->AddComponent(new CTransform);
	m_KillText->AddComponent(new CTextRender);
	m_KillText->TextRender()->SetString(ToWString(std::to_string(Kill)));
	m_KillText->TextRender()->SetFont(L"Galmuri14");
	m_KillText->TextRender()->SetFontSize(14.f);
	m_KillText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_KillText->TextRender()->SetOffsetPos(Vec3(-29.f, -22.f, -10.f));
	GetOwner()->AddChild(m_KillText);

	m_DeadText = new CGameObject;
	m_DeadText->SetName(L"Dead");
	m_DeadText->AddComponent(new CTransform);
	m_DeadText->AddComponent(new CTextRender);
	m_DeadText->TextRender()->SetString(ToWString(std::to_string(Dead)));
	m_DeadText->TextRender()->SetFont(L"Galmuri14");
	m_DeadText->TextRender()->SetFontSize(14.f);
	m_DeadText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_DeadText->TextRender()->SetOffsetPos(Vec3(-5.f, -22.f, -10.f));
	GetOwner()->AddChild(m_DeadText);

	m_AssistText = new CGameObject;
	m_AssistText->SetName(L"Assist");
	m_AssistText->AddComponent(new CTransform);
	m_AssistText->AddComponent(new CTextRender);
	m_AssistText->TextRender()->SetString(L"0");
	m_AssistText->TextRender()->SetFont(L"Galmuri14");
	m_AssistText->TextRender()->SetFontSize(14.f);
	m_AssistText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_AssistText->TextRender()->SetOffsetPos(Vec3(19.f, -22.f, -10.f));
	GetOwner()->AddChild(m_AssistText);

	m_DealText = new CGameObject;
	m_DealText->SetName(L"Deal");
	m_DealText->AddComponent(new CTransform);
	m_DealText->AddComponent(new CTextRender);
	m_DealText->TextRender()->SetString(ToWString(std::to_string(Deal)));
	m_DealText->TextRender()->SetFont(L"Galmuri14");
	m_DealText->TextRender()->SetFontSize(14.f);
	m_DealText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_DealText->TextRender()->SetOffsetPos(Vec3(3.f, 6.f, -10.f));
	GetOwner()->AddChild(m_DealText);

	m_DamagedText = new CGameObject;
	m_DamagedText->SetName(L"Damaged");
	m_DamagedText->AddComponent(new CTransform);
	m_DamagedText->AddComponent(new CTextRender);
	m_DamagedText->TextRender()->SetString(ToWString(std::to_string(Damaged)));
	m_DamagedText->TextRender()->SetFont(L"Galmuri14");
	m_DamagedText->TextRender()->SetFontSize(14.f);
	m_DamagedText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_DamagedText->TextRender()->SetOffsetPos(Vec3(3.f, 31.f, -10.f));
	GetOwner()->AddChild(m_DamagedText);

	m_HealText = new CGameObject;
	m_HealText->SetName(L"Heal");
	m_HealText->AddComponent(new CTransform);
	m_HealText->AddComponent(new CTextRender);
	m_HealText->TextRender()->SetString(ToWString(std::to_string(Heal)));
	m_HealText->TextRender()->SetFont(L"Galmuri14");
	m_HealText->TextRender()->SetFontSize(14.f);
	m_HealText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_HealText->TextRender()->SetOffsetPos(Vec3(3.f, 55.f, -10.f));
	GetOwner()->AddChild(m_HealText);

	m_KillText->SetActive(false);
	m_DeadText->SetActive(false);
	m_AssistText->SetActive(false);
	m_DealText->SetActive(false);
	m_DamagedText->SetActive(false);
	m_HealText->SetActive(false);
}
