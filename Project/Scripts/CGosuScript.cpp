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
	, m_Team(TEAM::NONE)
	, m_SelectedChamp(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::INT, "GamerATK", &m_GamerATK);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerDEF", &m_GamerDEF);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerAge", &m_GamerAge);
}

CGosuScript::CGosuScript(const CGosuScript& _Origin)
	: CScript(GAMERSCRIPT)
	, m_GamerName{}
	, m_GamerATK(_Origin.m_GamerATK)
	, m_GamerDEF(_Origin.m_GamerDEF)
	, m_GamerAge(_Origin.m_GamerAge)
	, m_Tex(_Origin.m_Tex)
	, m_Text(_Origin.m_Text)
	, m_Team(TEAM::NONE)
	, m_SelectedChamp(nullptr)
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

	GetOwner()->SetActive(false);

	CTGMgr::GetInst()->G_RecruitList.insert(make_pair(m_GamerName, GetOwner()));
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
