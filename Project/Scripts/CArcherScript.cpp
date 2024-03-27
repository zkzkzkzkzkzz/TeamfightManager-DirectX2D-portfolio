#include "pch.h"
#include "CArcherScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>

#include "CIdleState.h"
#include "CTraceState.h"

CArcherScript::CArcherScript()
	: m_DetectRange(10000.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_Info.HP);
	AddScriptParam(SCRIPT_PARAM::INT, "MP", &m_Info.MP);
	AddScriptParam(SCRIPT_PARAM::INT, "ATK", &m_Info.ATK);
	AddScriptParam(SCRIPT_PARAM::INT, "DEF", &m_Info.DEF);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Speed", &m_Info.ATKSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Range", &m_Info.ATKRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed", &m_Info.MOV);
	AddScriptParam(SCRIPT_PARAM::INT, "Champ Type", &m_Info.Type);
}

CArcherScript::CArcherScript(const CArcherScript& _Origin)
	: m_DetectRange(10000.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_Info.HP);
	AddScriptParam(SCRIPT_PARAM::INT, "MP", &m_Info.MP);
	AddScriptParam(SCRIPT_PARAM::INT, "ATK", &m_Info.ATK);
	AddScriptParam(SCRIPT_PARAM::INT, "DEF", &m_Info.DEF);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Speed", &m_Info.ATKSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Range", &m_Info.ATKRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed", &m_Info.MOV);
	AddScriptParam(SCRIPT_PARAM::INT, "Champ Type", &m_Info.Type);
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
	m_Info.Type = CHAMP_TYPE::MARKSMAN;

	m_State = CHAMP_STATE::IDLE;
}

void CArcherScript::InitChampAnim()
{
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ChampMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	Animator2D()->LoadAnimation(L"animdata\\ArcherIdle.txt");
	Animator2D()->LoadAnimation(L"animdata\\ArcherTrace.txt");
	Animator2D()->LoadAnimation(L"animdata\\ArcherAttack.txt");
	Animator2D()->LoadAnimation(L"animdata\\ArcherDead.txt");
	Animator2D()->Play(L"ArcherIdle");
}

void CArcherScript::InitStateMachine()
{
	if (StateMachine())
	{
		StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"ArcherFSM"));

		StateMachine()->AddBlackboardData(L"DetectRange", BB_DATA::FLOAT, &m_DetectRange);
		StateMachine()->AddBlackboardData(L"MoveSpeed", BB_DATA::FLOAT, &m_Info.MOV);
		StateMachine()->AddBlackboardData(L"AttackRange", BB_DATA::INT, &m_Info.ATKRange);
		StateMachine()->AddBlackboardData(L"AttackSpeed", BB_DATA::INT, &m_Info.ATKSpeed);
		StateMachine()->AddBlackboardData(L"ChampMP", BB_DATA::INT, &m_Info.MP);

		vector<CGameObject*> pObjs = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetLayerObjects();

		TEAM team = GetOwner()->GetScript<CChampScript>()->GetTeamColor();
		CGameObject* pTarget = nullptr;
		for (size_t i = 0; i < pObjs.size(); ++i)
		{
			if (team != pObjs[i]->GetScript<CChampScript>()->GetTeamColor())
			{
				Vec3 dist = Transform()->GetRelativePos() - pObjs[i]->Transform()->GetRelativePos();

				if (nullptr != pTarget)
				{
					Vec3 prevdist = Transform()->GetRelativePos() - pTarget->Transform()->GetRelativePos();

					if (prevdist.Length() > dist.Length())
						pTarget = pObjs[i];
				}
				else
				{
					pTarget = pObjs[i];
				}
			}
		}

		StateMachine()->AddBlackboardData(L"Target", BB_DATA::OBJECT, pTarget);


		if (nullptr != StateMachine()->GetFSM())
		{
			StateMachine()->GetFSM()->SetState(L"Idle");
		}
	}
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

void CArcherScript::EnterIdleState()
{
	Animator2D()->Play(L"ArcherIdle");
}

void CArcherScript::EnterTraceState()
{
	Animator2D()->Play(L"ArcherTrace");
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
	InitStateMachine();
}

void CArcherScript::tick()
{
	CChampScript::tick();
}

void CArcherScript::render()
{
}
