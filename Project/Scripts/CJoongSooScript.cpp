#include "pch.h"
#include "CJoongSooScript.h"

#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>

CJoongSooScript::CJoongSooScript()
	: CScript(JOONGSOOSCRIPT)
	, m_GamerName{}
	, m_GamerATK(15)
	, m_GamerDEF(15)
	, m_GamerAge(21)
	, m_Tex(nullptr)
	, m_Text(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::INT, "GamerATK", &m_GamerATK);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerDEF", &m_GamerDEF);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerAge", &m_GamerAge);
}

CJoongSooScript::~CJoongSooScript()
{
}

void CJoongSooScript::begin()
{
	m_Tex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Avatar\\Gamer\\custom\\character_custom1.png",
												L"texture\\Avatar\\Gamer\\custom\\character_custom1.png");

	m_GamerName = GetOwner()->GetName();

	GetOwner()->Transform()->SetRelativePos(Vec3(-54.f, -38.f, 330.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(192.f, 192.f, 1.f));
	GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 180.f, 0.f));

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"GamerMtrl"));
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CJoongSooScript::tick()
{

	render();
}

void CJoongSooScript::render()
{
	if (nullptr != m_Tex)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Tex);
}

