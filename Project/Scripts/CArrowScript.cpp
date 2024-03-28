#include "pch.h"
#include "CArrowScript.h"


CArrowScript::CArrowScript()
	: CScript(ARROWSCRIPT)
	, m_Tex(nullptr)
{
}

CArrowScript::CArrowScript(const CArrowScript& _Origin)
	: CScript(ARROWSCRIPT)
	, m_Tex(_Origin.m_Tex)
{
}

CArrowScript::~CArrowScript()
{
}

void CArrowScript::begin()
{
	m_Tex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\arrow.png",
												L"texture\\Champ\\arrow.png");
}

void CArrowScript::tick()
{
	render();
}

void CArrowScript::render()
{
	if (nullptr != m_Tex)
		MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Tex);
}

