#include "pch.h"
#include "CGamerScript.h"

#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>

CGamerScript::CGamerScript()
	: CScript(GAMERSCRIPT)
	, m_GamerName{}
	, m_GamerATK(6)
	, m_GamerDEF(6)
	, m_GamerAge(19)
	, m_Tex(nullptr)
	, m_Text(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::INT, "GamerATK", &m_GamerATK);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerDEF", &m_GamerDEF);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerAge", &m_GamerAge);
}

CGamerScript::~CGamerScript()
{
}

void CGamerScript::begin()
{
	m_Tex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Avatar\\Gamer\\custom\\character_custom0.png",
												L"texture\\Avatar\\Gamer\\custom\\character_custom0.png");

	m_GamerName = GetOwner()->GetName();

	GetOwner()->Transform()->SetRelativePos(Vec3(-147.f, -158.f, 330.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(192.f, 192.f, 1.f));

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"GamerMtrl"));
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CGamerScript::tick()
{

	render();
}

void CGamerScript::render()
{
	if (nullptr != m_Tex)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Tex);
}

