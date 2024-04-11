#include "pch.h"
#include "CMCScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CMCScript::CMCScript()
	: CScript(MCSCRIPT)
	, m_idx(TEXT_INDEX::END)
{
}

CMCScript::CMCScript(const CMCScript& _Origin)
	: CScript(MCSCRIPT)
	, m_idx(TEXT_INDEX::END)
{
}

CMCScript::~CMCScript()
{
}


void CMCScript::begin()
{
	m_idx = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"TextBox")->GetScript<CTextBoxScript>()->GetTextIdx();
}

void CMCScript::tick()
{
	m_idx = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"TextBox")->GetScript<CTextBoxScript>()->GetTextIdx();

	switch (m_idx)
	{
	case TEXT_INDEX::NONE:
		Animator2D()->Play(L"MCIdle");
		break;
	case TEXT_INDEX::TEXT0:
		Animator2D()->Play(L"MCMic");
		break;
	case TEXT_INDEX::TEXT1:
		Animator2D()->Play(L"MCMic");
		break;
	case TEXT_INDEX::TEXT2:
		Animator2D()->Play(L"MCMic");
		break;
	case TEXT_INDEX::TEXT3:
		Animator2D()->Play(L"MCClap");
		break;
	case TEXT_INDEX::TEXT4:
		Animator2D()->Play(L"MCMic");
		break;
	case TEXT_INDEX::END:
		Animator2D()->Play(L"MCIdle");
		break;
	default:
		break;
	}
}
