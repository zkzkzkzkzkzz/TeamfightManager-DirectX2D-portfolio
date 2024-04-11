#include "pch.h"
#include "CAvatarScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CAvatarScript::CAvatarScript()
	: CScript(AVATARSCRIPT)
	, m_idx(TEXT_INDEX::NONE)
	, m_AvatarName{}
{
}

CAvatarScript::CAvatarScript(const CAvatarScript& _Origin)
	: CScript(AVATARSCRIPT)
	, m_idx(_Origin.m_idx)
	, m_AvatarName{}
{
}

CAvatarScript::~CAvatarScript()
{
}

void CAvatarScript::begin()
{
	m_AvatarName = GetOwner()->GetName();
}

void CAvatarScript::tick()
{
	m_idx = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"TextBox")->GetScript<CTextBoxScript>()->GetTextIdx();

	switch (m_idx)
	{
	case TEXT_INDEX::NONE:
		break;
	case TEXT_INDEX::TEXT0:
		break;
	case TEXT_INDEX::TEXT1:
		break;
	case TEXT_INDEX::TEXT2:
		PlayIdleAnim();
		break;
	case TEXT_INDEX::TEXT3:
		PlayJumpAnim();
		break;
	case TEXT_INDEX::TEXT4:
		PlayJumpAnim();
		break;
	case TEXT_INDEX::END:
		GamePlayStatic::DestroyGameObject(GetOwner());
		break;
	default:
		break;
	}
}


void CAvatarScript::PlayIdleAnim()
{
	if (L"Manager" == m_AvatarName)
	{
		Animator2D()->Play(L"ManagerIdle");
	}
	else if (L"Chobo" == m_AvatarName)
	{
		Animator2D()->Play(L"ChoboIdle");
	}
	else if (L"JoongSoo" == m_AvatarName)
	{
		Animator2D()->Play(L"JoongSooIdle");
	}
	else if (L"Gosu" == m_AvatarName)
	{
		Animator2D()->Play(L"GosuIdle");
	}
}

void CAvatarScript::PlayJumpAnim()
{
	if (L"Manager" == m_AvatarName)
	{
		Animator2D()->Play(L"ManagerJump");
	}
	else if (L"Chobo" == m_AvatarName)
	{
		Animator2D()->Play(L"ChoboJump");
	}
	else if (L"JoongSoo" == m_AvatarName)
	{
		Animator2D()->Play(L"JoongSooJump");
	}
	else if (L"Gosu" == m_AvatarName)
	{
		Animator2D()->Play(L"GosuJump");
	}
}