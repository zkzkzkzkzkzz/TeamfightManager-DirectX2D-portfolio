#include "pch.h"
#include "CUIScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>

CUIScript::CUIScript()
	: CScript(UISCRIPT)
	, m_NormalImg(nullptr)
	, m_PressedImg(nullptr)
	, m_Clicked(false)
{
}

CUIScript::CUIScript(const CUIScript& _Other)
	: CScript(UISCRIPT)
{
}

CUIScript::~CUIScript()
{
}


void CUIScript::begin()
{
	m_NormalImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_NewGame_Idle.png", L"texture\\Title\\Title_NewGame_Idle.png");
	m_PressedImg = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\Title_NewGame_Over.png", L"texture\\Title\\Title_NewGame_Over.png");
}

void CUIScript::tick()
{
	if (KEY_TAP(LBTN))
	{
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		Vec2 vResolution = CEngine::GetInst()->GetResolution();

		vMousePos.x -= vResolution.x / 2.f;
		vMousePos.y = -(vMousePos.y - vResolution.y / 2.f);

		Vec3 vWorldPos = GetOwner()->Transform()->GetWorldPos();
		Vec3 vWorldScale = GetOwner()->Transform()->GetWorldScale();

		Vec2 vLT = Vec2(vWorldPos.x - vWorldScale.x / 2, vWorldPos.y - vWorldScale.y / 2);
		Vec2 vRB = Vec2(vWorldPos.x + vWorldScale.x / 2, vWorldPos.y + vWorldScale.y / 2);

		if (vLT.x < vMousePos.x && vMousePos.x < vRB.x
			&& vLT.y < vMousePos.y && vMousePos.y < vRB.y)
		{
			LBtnClicked();
		}
	}

	if (m_Clicked && KEY_RELEASED(LBTN))
		LBtnReleased();

}

void CUIScript::LBtnReleased()
{
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_NormalImg);
	m_Clicked = false;
}

void CUIScript::LBtnClicked()
{
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_PressedImg);
	m_Clicked = true;

	// CallBack
	if (m_CallBackFunc) m_CallBackFunc();

	// Delegate
	if (m_Inst != nullptr && m_Delegate != nullptr)
		(m_Inst->*m_Delegate)();

}