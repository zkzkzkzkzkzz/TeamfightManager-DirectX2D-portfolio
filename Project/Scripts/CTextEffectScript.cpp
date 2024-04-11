#include "pch.h"
#include "CTextEffectScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTransform.h>

CTextEffectScript::CTextEffectScript()
	: CScript(TEXTEFFECTSCRIPT)
	, m_Idx(TEXT_INDEX::NONE)
	, m_PrevScaleX(0.f)
	, m_CurScaleX(0.f)
{
}

CTextEffectScript::CTextEffectScript(const CTextEffectScript& _Origin)
	: CScript(TEXTEFFECTSCRIPT)
	, m_Idx(_Origin.m_Idx)
	, m_PrevScaleX(0.f)
	, m_CurScaleX(0.f)
{
}

CTextEffectScript::~CTextEffectScript()
{
}

void CTextEffectScript::begin()
{
	m_Idx = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"TextBox")->GetScript<CTextBoxScript>()->GetTextIdx();

	m_PrevScaleX = m_CurScaleX = Transform()->GetRelativeScale().x;
}

void CTextEffectScript::tick()
{
	DecreaseScale();
}


void CTextEffectScript::DecreaseScale()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vScale = Transform()->GetRelativeScale();

	m_CurScaleX = Transform()->GetRelativeScale().x;

	if (m_CurScaleX <= 0.f)
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	m_CurScaleX -= 600.f * DT;

	if (m_PrevScaleX != m_CurScaleX)
	{
		float dist = m_PrevScaleX - m_CurScaleX;

		vPos.x += dist / 2.f;
	}

	m_PrevScaleX = m_CurScaleX;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeScale(Vec3(m_CurScaleX, vScale.y, vScale.z));
}
