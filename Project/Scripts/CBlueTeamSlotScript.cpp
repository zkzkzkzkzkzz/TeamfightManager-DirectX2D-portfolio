#include "pch.h"
#include "CBlueTeamSlotScript.h"

#include <Engine\CEngine.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

CBlueTeamSlotScript::CBlueTeamSlotScript()
	: CScript(BLUETEAMSLOTSCRIPT)
	, m_SlotTex(nullptr)
	, m_SubSlot(nullptr)
	, m_SubSlotTex(nullptr)
	, m_KDAText(nullptr)
	, m_InfoText(nullptr)
	, m_CurGamer(nullptr)
	, Kill(0)
	, Dead(0)
	, Deal(0)
	, Damaged(0)
	, Heal(0)
{
}

CBlueTeamSlotScript::CBlueTeamSlotScript(const CBlueTeamSlotScript& _Origin)
	: CScript(BLUETEAMSLOTSCRIPT)
	, m_SlotTex(_Origin.m_SlotTex)
	, m_SubSlot(nullptr)
	, m_SubSlotTex(_Origin.m_SubSlotTex)
	, m_KDAText(nullptr)
	, m_InfoText(nullptr)
	, m_CurGamer(nullptr)
	, Kill(0)
	, Dead(0)
	, Deal(0)
	, Damaged(0)
	, Heal(0)
{
}

CBlueTeamSlotScript::~CBlueTeamSlotScript()
{
}

void CBlueTeamSlotScript::begin()
{
	Transform()->SetRelativeScale(Vec3(162.f, 156.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"InGameUIMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	
	m_SubSlot = new CGameObject;
	m_SubSlot->SetName(L"SubSlot");
	m_SubSlot->AddComponent(new CTransform);
	m_SubSlot->AddComponent(new CMeshRender);
	m_SubSlot->Transform()->SetRelativePos(Vec3(121.f, 0.f, 0.f));
	m_SubSlot->Transform()->SetRelativeScale(Vec3(82.f, 156.f, 0.f));
	m_SubSlot->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_SubSlot->MeshRender()->SetMaterial(GetOwner()->MeshRender()->GetDynamicMaterial());
	m_SubSlot->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	GetOwner()->AddChild(m_SubSlot);

	m_SlotTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\IngameUI\\player_card_bg_0.png"
													, L"texture\\Champ\\IngameUI\\player_card_bg_0.png");

	m_SubSlotTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\IngameUI\\data_ui_bg_0.png"
														, L"texture\\Champ\\IngameUI\\data_ui_bg_0.png");

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
	m_SubSlot->AddChild(m_InfoText);

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
	m_SubSlot->AddChild(m_KDAText);
}

void CBlueTeamSlotScript::tick()
{
	CheckInGameInfo();
	render();
}

void CBlueTeamSlotScript::render()
{
	if (nullptr != m_SlotTex)
		MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_SlotTex);

	if (nullptr != m_SubSlotTex)
		m_SubSlot->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_SubSlotTex);
}



void CBlueTeamSlotScript::SetGamer(CGameObject* _Gamer)
{
	m_CurGamer = _Gamer;
}

void CBlueTeamSlotScript::CheckInGameInfo()
{
	
}
