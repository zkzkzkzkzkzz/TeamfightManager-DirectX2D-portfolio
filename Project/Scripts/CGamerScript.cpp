#include "pch.h"
#include "CGamerScript.h"

#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\components.h>


CGamerScript::CGamerScript()
	: CScript(GAMERSCRIPT)
	, m_GamerName{}
	, m_GamerLevel(GAMER_LEVEL::NONE)
	, m_GamerATK(0)
	, m_GamerDEF(0)
	, m_Tex(nullptr)
	, m_Team(TEAM::NONE)
	, m_SelectedChamp(CHAMP_LIST::END)
{
	AddScriptParam(SCRIPT_PARAM::INT, "GamerTeam", &m_Team);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerATK", &m_GamerATK);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerDEF", &m_GamerDEF);
}

CGamerScript::CGamerScript(UINT _ScriptType)
	: CScript(_ScriptType)
	, m_GamerName{}
	, m_GamerLevel(GAMER_LEVEL::NONE)
	, m_GamerATK(0)
	, m_GamerDEF(0)
	, m_Tex(nullptr)
	, m_Team(TEAM::NONE)
	, m_SelectedChamp(CHAMP_LIST::END)
{
	AddScriptParam(SCRIPT_PARAM::INT, "GamerTeam", &m_Team);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerATK", &m_GamerATK);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerDEF", &m_GamerDEF);
}

CGamerScript::CGamerScript(const CGamerScript& _Origin)
	: CScript(GAMERSCRIPT)
	, m_GamerName(_Origin.m_GamerName)
	, m_GamerLevel(_Origin.m_GamerLevel)
	, m_GamerATK(_Origin.m_GamerATK)
	, m_GamerDEF(_Origin.m_GamerDEF)
	, m_Tex(nullptr)
	, m_Team(_Origin.m_Team)
	, m_SelectedChamp(_Origin.m_SelectedChamp)
{
	AddScriptParam(SCRIPT_PARAM::INT, "GamerTeam", &m_Team);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerATK", &m_GamerATK);
	AddScriptParam(SCRIPT_PARAM::INT, "GamerDEF", &m_GamerDEF);
	
	if (nullptr != _Origin.m_Tex)
	{
		m_Tex = _Origin.m_Tex;
	}
}

CGamerScript::~CGamerScript()
{
}


void CGamerScript::begin()
{
	Transform()->SetRelativeScale(Vec3(192.f, 192.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"GamerMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	GAMER_LEVEL lv = GetGamerLevel();

	switch (lv)
	{
	case GAMER_LEVEL::NONE:
		break;
	case GAMER_LEVEL::CHOBO:
		SetGamerInfo(6, 6);
		break;
	case GAMER_LEVEL::JOONGSOO:
		SetGamerInfo(15, 15);
		break;
	case GAMER_LEVEL::GOSU:
		SetGamerInfo(30, 30);
		break;
	case GAMER_LEVEL::END:
		break;
	default:
		break;
	}
}

void CGamerScript::tick()
{
	IsPlayableGamer();
	SetGamerPos();

	render();
}

void CGamerScript::render()
{
	if (nullptr != m_Tex)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Tex);
}


void CGamerScript::SetGamerInfo(int _ATK, int _DEF, TEAM _team)
{
	m_GamerATK = _ATK;
	m_GamerDEF = _DEF;
	m_Team = _team;
}


void CGamerScript::SetTexture(const wstring& _str)
{
	m_Tex = CAssetMgr::GetInst()->Load<CTexture>(_str, _str);
}

void CGamerScript::SetPlayableGamer()
{
	CTGMgr::GetInst()->G_PlayableGamer.push_back(GetOwner());
}

void CGamerScript::InsertGamerList()
{
	CTGMgr::GetInst()->G_Gamer.insert(make_pair(GETGAMER(GetOwner())->GetGamerName(), GetOwner()));
}

void CGamerScript::InsertRecruitList()
{
	CTGMgr::GetInst()->G_RecruitList.insert(make_pair(GETGAMER(GetOwner())->GetGamerName(), GetOwner()));
}

void CGamerScript::IsPlayableGamer()
{
	map<wstring, CGameObject*>::iterator iter = CTGMgr::GetInst()->G_Gamer.find(GETGAMER(GetOwner())->GetGamerName());

	if (iter != CTGMgr::GetInst()->G_Gamer.end())
	{
		vector<CGameObject*>::iterator Iter = find(CTGMgr::GetInst()->G_PlayableGamer.begin()
													, CTGMgr::GetInst()->G_PlayableGamer.end(), iter->second);

		if (Iter != CTGMgr::GetInst()->G_PlayableGamer.end())
		{
			(iter->second)->SetActive(true);
		}
		else
		{
			(iter->second)->SetActive(false);
		}
	}
}

void CGamerScript::SetGamerPos()
{
	int x = 0, y = 1;

	vector<CGameObject*>::iterator iter = CTGMgr::GetInst()->G_PlayableGamer.begin();

	for (; iter != CTGMgr::GetInst()->G_PlayableGamer.end(); ++iter, ++x, y *= -1)
	{
		(*iter)->Transform()->SetRelativePos(Vec3(-150.f + (50.f * x), -98.f + (-60.f * y), 1000.f));
		(*iter)->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI * y * x, 0.f));
	}
}
