#include "pch.h"
#include "CGosuScript.h"


#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>


CGosuScript::CGosuScript()
	: CScript(GOSUSCRIPT)
	, m_GamerName{}
	, m_GamerATK(30)
	, m_GamerDEF(30)
	, m_GamerAge(23)
	, m_Tex(nullptr)
	, m_Text(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::INT, "GamerATK", &m_GamerATK);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerDEF", &m_GamerDEF);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerAge", &m_GamerAge);
}

CGosuScript::~CGosuScript()
{

}


void CGosuScript::begin()
{
	m_Tex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Avatar\\Gamer\\custom\\character_custom2.png",
												L"texture\\Avatar\\Gamer\\custom\\character_custom2.png");

	m_GamerName = GetOwner()->GetName();

	GetOwner()->Transform()->SetRelativePos(Vec3(117.f, -158.f, 330.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(192.f, 192.f, 1.f));

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"GamerMtrl"));
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CGosuScript::tick()
{
	render();
}

void CGosuScript::render()
{
	if (nullptr != m_Tex)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Tex);
}
