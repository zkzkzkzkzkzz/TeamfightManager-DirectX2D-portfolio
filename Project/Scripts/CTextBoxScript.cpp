#include "pch.h"
#include "CTextBoxScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

CTextBoxScript::CTextBoxScript()
	: CScript(TEXTBOXSCRIPT)
	, m_idx(TEXT_INDEX::NONE)
	, m_Text(nullptr)
	, m_SecondText(nullptr)
	, m_SubText(nullptr)
	, m_Gold(nullptr)
	, m_Mouse(nullptr)
	, m_TextEffect(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CTextBoxScript::CTextBoxScript(const CTextBoxScript& _Origin)
	: CScript(TEXTBOXSCRIPT)
	, m_idx(TEXT_INDEX::NONE)
	, m_Text(nullptr)
	, m_SecondText(nullptr)
	, m_SubText(nullptr)
	, m_Gold(nullptr)
	, m_Mouse(nullptr)
	, m_TextEffect(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CTextBoxScript::~CTextBoxScript()
{
}


void CTextBoxScript::begin()
{
	m_Text = new CGameObject;
	m_Text->AddComponent(new CTransform);
	m_Text->AddComponent(new CTextRender);
	m_Text->TextRender()->SetString(L"Now, let's begin the awards ceremony of Amateur League season 2021!");
	m_Text->TextRender()->SetFont(L"Galmuri14");
	m_Text->TextRender()->SetFontColor(255, 255, 255, 255);
	m_Text->TextRender()->SetFontSize(23.f);
	m_Text->TextRender()->SetOffsetPos(Vec3(-583.f, -44.f, -10.f));
	GetOwner()->AddChild(m_Text);
	m_Text->SetActive(false);

	m_SecondText = new CGameObject;
	m_SecondText->AddComponent(new CTransform);
	m_SecondText->AddComponent(new CTextRender);
	m_SecondText->TextRender()->SetString(L"Prize money worth         and                              will be awarded to the winning team.");
	m_SecondText->TextRender()->SetFont(L"Galmuri14");
	m_SecondText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_SecondText->TextRender()->SetFontSize(23.f);
	m_SecondText->TextRender()->SetOffsetPos(Vec3(-583.f, 0.f, -10.f));
	GetOwner()->AddChild(m_SecondText);
	m_SecondText->SetActive(false);

	m_SubText = new CGameObject;
	m_SubText->AddComponent(new CTransform);
	m_SubText->AddComponent(new CTextRender);
	m_SubText->TextRender()->SetString(L"AR49");
	m_SubText->TextRender()->SetFont(L"Galmuri14");
	m_SubText->TextRender()->SetFontColor(255, 255, 0, 255);
	m_SubText->TextRender()->SetFontSize(23.f);
	m_SubText->TextRender()->SetOffsetPos(Vec3(-61.f, -44.f, -10.f));
	GetOwner()->AddChild(m_SubText);
	m_SubText->SetActive(false);

	m_Gold = new CGameObject;
	m_Gold->AddComponent(new CTransform);
	m_Gold->AddComponent(new CTextRender);
	m_Gold->TextRender()->SetString(L"400G");
	m_Gold->TextRender()->SetFont(L"Galmuri14");
	m_Gold->TextRender()->SetFontColor(255, 255, 0, 255);
	m_Gold->TextRender()->SetFontSize(23.f);
	m_Gold->TextRender()->SetOffsetPos(Vec3(-338.f, 0.f, -10.f));
	GetOwner()->AddChild(m_Gold);
	m_Gold->SetActive(false);

	m_Mouse = new CGameObject;
	m_Mouse->AddComponent(new CTransform);
	m_Mouse->AddComponent(new CTextRender);
	m_Mouse->TextRender()->SetString(L"Amateur Golden Mouse");
	m_Mouse->TextRender()->SetFont(L"Galmuri14");
	m_Mouse->TextRender()->SetFontColor(255, 255, 0, 255);
	m_Mouse->TextRender()->SetFontSize(23.f);
	m_Mouse->TextRender()->SetOffsetPos(Vec3(-207.f, 0.f, -10.f));
	GetOwner()->AddChild(m_Mouse);
	m_Mouse->SetActive(false);
}

void CTextBoxScript::tick()
{
	CheckMousePos();
}


void CTextBoxScript::SetTextByIndex()
{
	switch (m_idx)
	{
	case TEXT_INDEX::NONE:
		break;
	case TEXT_INDEX::TEXT0:
		m_TextEffect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_TextEffect.prefab")->Instatiate();
		GamePlayStatic::SpawnGameObject(m_TextEffect, 2);
		m_Text->SetActive(true);
		break;
	case TEXT_INDEX::TEXT1:
		m_TextEffect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_TextEffect.prefab")->Instatiate();
		GamePlayStatic::SpawnGameObject(m_TextEffect, 2);
		m_Text->TextRender()->SetString(L"We have the Champion Team Award.");
		break;
	case TEXT_INDEX::TEXT2:
		m_TextEffect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_TextEffect.prefab")->Instatiate();
		GamePlayStatic::SpawnGameObject(m_TextEffect, 2);
		m_Text->TextRender()->SetString(L"The Amateur League champion team is...");
		break;
	case TEXT_INDEX::TEXT3:
		m_TextEffect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_TextEffect.prefab")->Instatiate();
		m_TextEffect->Transform()->SetRelativePos(Vec3(211.f, -262.f, 1000.f));
		m_TextEffect->Transform()->SetRelativeScale(Vec3(500.f, 44.f, 1.f));
		GamePlayStatic::SpawnGameObject(m_TextEffect, 2);
		m_Text->TextRender()->SetString(L"The Amateur League champion team is...        ! Congratulations!");
		m_SubText->SetActive(true);
		break;
	case TEXT_INDEX::TEXT4:
		m_TextEffect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_TextEffect.prefab")->Instatiate();
		m_TextEffect->Transform()->SetRelativePos(Vec3(0.f, -293.f, 1000.f));
		m_TextEffect->Transform()->SetRelativeScale(Vec3(1200.f, 44.f, 1.f));
		GamePlayStatic::SpawnGameObject(m_TextEffect, 2);


		m_SecondText->SetActive(true);
		m_Gold->SetActive(true);
		m_Mouse->SetActive(true);
		break;
	case TEXT_INDEX::END:
		m_Text->SetActive(false);
		m_SecondText->SetActive(false);
		m_SubText->SetActive(false);
		m_Gold->SetActive(false);
		m_Mouse->SetActive(false);
		break;
	default:
		break;
	}
}

void CTextBoxScript::CheckMousePos()
{
	bool bLBtnTap = KEY_TAP(LBTN);
	bool bLbtnReleased = KEY_RELEASED(LBTN);

	if (bLbtnReleased)
	{
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


void CTextBoxScript::LBtnClicked()
{
	if (TEXT_INDEX::NONE == m_idx)
		m_idx = TEXT_INDEX::TEXT0;
	else if (TEXT_INDEX::TEXT0 == m_idx)
		m_idx = TEXT_INDEX::TEXT1;
	else if (TEXT_INDEX::TEXT1 == m_idx)
		m_idx = TEXT_INDEX::TEXT2;
	else if (TEXT_INDEX::TEXT2 == m_idx)
		m_idx = TEXT_INDEX::TEXT3;
	else if (TEXT_INDEX::TEXT3 == m_idx)
		m_idx = TEXT_INDEX::TEXT4;
	else if (TEXT_INDEX::TEXT4 == m_idx)
		m_idx = TEXT_INDEX::END;

	SetTextByIndex();
}
