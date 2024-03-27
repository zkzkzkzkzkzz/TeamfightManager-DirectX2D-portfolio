#include "pch.h"
#include "CArcherScript.h"

#include <Engine\CAssetMgr.h>

CArcherScript::CArcherScript()
	: m_Info{}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
{
}

CArcherScript::CArcherScript(const CArcherScript& _Origin)
	: m_Info{}
	, m_State(CHAMP_STATE::END)
	, m_Team(TEAM::NONE)
{
}

CArcherScript::~CArcherScript()
{
}

void CArcherScript::InitChampInfo()
{
	m_Info.HP = 100;
	m_Info.MP = 0;
	m_Info.ATK = 42;
	m_Info.DEF = 5;
	m_Info.ATKSpeed = 0.67f;
	m_Info.ATKRange = 120;
	m_Info.MOV = 3.f;
	m_Info.Type = CHAMP_TYPE::ARCHERS;
}

void CArcherScript::InitChampAnim()
{
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ChampMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	//Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\archer_64pix.png", 
	//														L"texture\\Champ\\archer_64pix.png");
	//GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	Animator2D()->LoadAnimation(L"animdata\\ArcherTest.txt");
	Animator2D()->Play(L"ArcherTest");
}

void CArcherScript::Attack()
{

}

void CArcherScript::AttackComplete()
{
}

void CArcherScript::Heal()
{
}

void CArcherScript::HealComplete()
{
}

void CArcherScript::begin()
{
	if (TEAM::BLUE == GetTeamColor())
		Transform()->SetRelativePos(Vec3(-290.f, 0.f, 300.f));
	else
		Transform()->SetRelativePos(Vec3(290.f, 0.f, 300.f));

	Transform()->SetRelativeScale(Vec3(64.f, 64.f, 1.f));

	InitChampInfo();
	InitChampAnim();
}

void CArcherScript::tick()
{
	CChampScript::tick();
}

void CArcherScript::render()
{
}
