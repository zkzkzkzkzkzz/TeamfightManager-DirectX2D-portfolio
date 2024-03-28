#include "pch.h"
#include "CArcherScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAnim.h>
#include <Engine\CAssetMgr.h>

#include "CIdleState.h"
#include "CTraceState.h"
#include "CArrowScript.h"

CArcherScript::CArcherScript()
	: CChampScript(ARCHERSCRIPT)
	, m_Target(nullptr)
	, m_AccTime(0.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_Info.HP);
	AddScriptParam(SCRIPT_PARAM::INT, "MP", &m_Info.MP);
	AddScriptParam(SCRIPT_PARAM::INT, "ATK", &m_Info.ATK);
	AddScriptParam(SCRIPT_PARAM::INT, "DEF", &m_Info.DEF);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Speed", &m_Info.ATKSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Range", &m_Info.ATKRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed", &m_Info.MOV);
	AddScriptParam(SCRIPT_PARAM::INT, "Champ Type", &m_Info.Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Team", &m_Team);

	m_bAttack = false;
}

CArcherScript::CArcherScript(const CArcherScript& _Origin)
	: CChampScript(ARCHERSCRIPT)
	, m_Target(nullptr)
	, m_AccTime(0.f)
{

	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_Info.HP);
	AddScriptParam(SCRIPT_PARAM::INT, "MP", &m_Info.MP);
	AddScriptParam(SCRIPT_PARAM::INT, "ATK", &m_Info.ATK);
	AddScriptParam(SCRIPT_PARAM::INT, "DEF", &m_Info.DEF);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Speed", &m_Info.ATKSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Range", &m_Info.ATKRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed", &m_Info.MOV);
	AddScriptParam(SCRIPT_PARAM::INT, "Champ Type", &m_Info.Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Team", &m_Team);

	m_bAttack = false;
}

CArcherScript::~CArcherScript()
{
}


void CArcherScript::begin()
{
	CChampScript::begin();

	if (TEAM::BLUE == GetTeamColor())
		Transform()->SetRelativePos(Vec3(-190.f, 100.f, 300.f));
	else
		Transform()->SetRelativePos(Vec3(190.f, -100.f, 300.f));

	Transform()->SetRelativeScale(Vec3(64.f, 64.f, 1.f));

	InitChampInfo();
	InitChampAnim();
	InitStateMachine();
}

void CArcherScript::tick()
{
	CChampScript::tick();

	CheckStateMachine();

	m_AccTime += DT;

	float delay = 1 / m_Info.ATKSpeed;

	if (m_AccTime > delay)
	{
		m_bAttack = false;
		m_AccTime = 0.f;
	}
}

void CArcherScript::render()
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

	Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	Collider2D()->SetOffsetScale(Vec2(0.5f, 0.5f));

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

		StateMachine()->AddBlackboardData(L"MoveSpeed", BB_DATA::FLOAT, &m_Info.MOV);
		StateMachine()->AddBlackboardData(L"AttackRange", BB_DATA::INT, &m_Info.ATKRange);
		StateMachine()->AddBlackboardData(L"AttackSpeed", BB_DATA::INT, &m_Info.ATKSpeed);
		StateMachine()->AddBlackboardData(L"ChampMP", BB_DATA::INT, &m_Info.MP);

		vector<CGameObject*> pObjs = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();

		TEAM team = GetOwner()->GetScript<CChampScript>()->GetTeamColor();
		CGameObject* pTarget = nullptr;
		for (size_t i = 0; i < pObjs.size(); ++i)
		{
			if (team != pObjs[i]->GetScript<CChampScript>()->GetTeamColor() 
				&& TEAM::NONE != pObjs[i]->GetScript<CChampScript>()->GetTeamColor()
				&& TEAM::END != pObjs[i]->GetScript<CChampScript>()->GetTeamColor())
			{
				Vec3 dist = Transform()->GetRelativePos() - pObjs[i]->Transform()->GetRelativePos();

				if (nullptr != pTarget)
				{
					Vec3 prevdist = Transform()->GetRelativePos() - pTarget->Transform()->GetRelativePos();

					if (prevdist.Length() > dist.Length())
						m_Target = pTarget = pObjs[i];
				}
				else
				{
					m_Target = pTarget = pObjs[i];
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

void CArcherScript::CheckStateMachine()
{
	vector<CGameObject*> pObjs = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();
	if (StateMachine())
	{
		if (!pObjs.empty())
		{
			TEAM team = GetOwner()->GetScript<CChampScript>()->GetTeamColor();
			CGameObject* pTarget = nullptr;
			for (size_t i = 0; i < pObjs.size(); ++i)
			{
				if (TEAM::NONE != team
					&& team != pObjs[i]->GetScript<CChampScript>()->GetTeamColor()
					&& TEAM::NONE != pObjs[i]->GetScript<CChampScript>()->GetTeamColor()
					&& TEAM::END != pObjs[i]->GetScript<CChampScript>()->GetTeamColor())
				{
					Vec3 dist = Transform()->GetRelativePos() - pObjs[i]->Transform()->GetRelativePos();

					if (nullptr != pTarget)
					{
						Vec3 prevdist = Transform()->GetRelativePos() - pTarget->Transform()->GetRelativePos();

						if (prevdist.Length() > dist.Length())
							m_Target = pTarget = pObjs[i];
					}
					else
					{
						m_Target = pTarget = pObjs[i];
					}
				}
			}

			StateMachine()->SetBlackboardData(L"Target", BB_DATA::OBJECT, pTarget);
		}
	}
}


void CArcherScript::EnterIdleState()
{
	Animator2D()->Play(L"ArcherIdle");
}

void CArcherScript::EnterTraceState()
{
	Animator2D()->Play(L"ArcherTrace");
}

void CArcherScript::EnterAttackState()
{
	if (!m_bAttack && m_AccTime < m_Info.ATKSpeed)
	{
		Animator2D()->Play(L"ArcherAttack");
		m_bAttack = true;
		m_AccTime = 0.f;
	}

	if (Animator2D()->FindAnim(L"ArcherAttack")->IsFinish())
	{
		//CGameObject* arrow = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Arrow3.prefab")->Instatiate();
		CGameObject* arrow = new CGameObject;
		arrow->AddComponent(new CTransform);
		arrow->AddComponent(new CMeshRender);
		arrow->AddComponent(new CCollider2D);
		arrow->AddComponent(new CArrowScript);
		GetOwner()->AddChild(arrow);
		GamePlayStatic::SpawnGameObject(arrow, 5);
		arrow->SetLayerIdx(5);

		Animator2D()->FindAnim(L"ArcherAttack")->Reset();

		if (m_bAttack)
		{
			m_State = CHAMP_STATE::IDLE;
		}
	}
}

void CArcherScript::EnterSkillState()
{
}

void CArcherScript::EnterUltimateState()
{
}

void CArcherScript::EnterDeadState()
{
	Animator2D()->Play(L"ArcherDead");
}


void CArcherScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	int a = 0;
}

void CArcherScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CArcherScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}
