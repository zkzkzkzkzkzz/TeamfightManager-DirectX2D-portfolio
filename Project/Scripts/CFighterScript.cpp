#include "pch.h"
#include "CFighterScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAnim.h>
#include <Engine\CAssetMgr.h>

#include "CBTMgr.h"


CFighterScript::CFighterScript()
	: CChampScript(FIGHTERSCRIPT)
	, m_Target(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_InGameStatus.HP);
	AddScriptParam(SCRIPT_PARAM::INT, "ATK", &m_InGameStatus.ATK);
	AddScriptParam(SCRIPT_PARAM::INT, "DEF", &m_InGameStatus.DEF);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Speed", &m_Info.ATKSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Range", &m_Info.ATKRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed", &m_Info.MOV);
	AddScriptParam(SCRIPT_PARAM::INT, "Champ Type", &m_Info.Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Team", &m_Team);

	m_bAttack = false;
}

CFighterScript::CFighterScript(const CFighterScript& _Origin)
	: CChampScript(FIGHTERSCRIPT)
	, m_Target(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_InGameStatus.HP);
	AddScriptParam(SCRIPT_PARAM::INT, "ATK", &m_InGameStatus.ATK);
	AddScriptParam(SCRIPT_PARAM::INT, "DEF", &m_InGameStatus.DEF);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Speed", &m_Info.ATKSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Range", &m_Info.ATKRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed", &m_Info.MOV);
	AddScriptParam(SCRIPT_PARAM::INT, "Champ Type", &m_Info.Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Team", &m_Team);

	m_bAttack = false;
}

CFighterScript::~CFighterScript()
{
}


void CFighterScript::begin()
{
	CChampScript::begin();

	InitChampInfo();
	InitChampAnim();
	InitStateMachine();
}

void CFighterScript::tick()
{
	CChampScript::tick();

	CheckStateMachine();

	m_InGameStatus.CoolTime_Attack += DT;

	float delay = 1 / m_Info.ATKSpeed;

	if (m_InGameStatus.CoolTime_Attack > delay)
	{
		m_bAttack = false;
		m_InGameStatus.CoolTime_Attack = 0.f;
	}
}


void CFighterScript::InitChampInfo()
{
	SetChampInfo(190, 10, 40, 1.18f, 35, 7, CHAMP_TYPE::WARRIOR);	// 기본 정보 설정
	InitChampStatus(0, 0);	// 인게임 정보 설정

	m_State = CHAMP_STATE::IDLE;
	m_bRespawn = false;

	if (StateMachine() && nullptr != StateMachine()->GetFSM())
	{
		StateMachine()->GetFSM()->SetState(L"Idle");
	}
}

void CFighterScript::InitChampStatus(int _GamerATK, int _GamerDEF)
{
	m_InGameStatus.HP = m_Info.MaxHP;
	m_InGameStatus.ATK = m_Info.ATK + _GamerATK;
	m_InGameStatus.DEF = m_Info.DEF + _GamerDEF;

	m_InGameStatus.CoolTime_Attack = 0.f;
	m_InGameStatus.CoolTime_Skill = 0.f;
	m_InGameStatus.UltimateUseTime = 60.f;
	m_InGameStatus.bUltimate = false;

	m_InGameStatus.RespawnTime = 0.f;

	m_InGameStatus.TotalDeal = 0;
	m_InGameStatus.TotalDamage = 0;
	m_InGameStatus.TotalHeal = 0;
	m_InGameStatus.KillPoint = 0;
	m_InGameStatus.DeathPoint = 0;
	m_InGameStatus.AssistPoint = 0;
}

void CFighterScript::InitChampAnim()
{
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ChampMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	Collider2D()->SetOffsetPos(Vec2(0.05f, 0.05f));
	Collider2D()->SetOffsetScale(Vec2(0.4f, 0.5f));

	Animator2D()->LoadAnimation(L"animdata\\FighterIdle.txt");
	Animator2D()->LoadAnimation(L"animdata\\FighterTrace.txt");
	Animator2D()->LoadAnimation(L"animdata\\FighterAttack.txt");
	Animator2D()->LoadAnimation(L"animdata\\FighterDead.txt");
	Animator2D()->Play(L"FighterIdle");
}

void CFighterScript::InitStateMachine()
{
	if (StateMachine())
	{
		StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"ChampFSM"));

		StateMachine()->AddBlackboardData(L"MoveSpeed", BB_DATA::FLOAT, &m_Info.MOV);
		StateMachine()->AddBlackboardData(L"AttackRange", BB_DATA::INT, &m_Info.ATKRange);
		StateMachine()->AddBlackboardData(L"AttackSpeed", BB_DATA::INT, &m_Info.ATKSpeed);
		StateMachine()->AddBlackboardData(L"Skill_Cooltime", BB_DATA::INT, &m_InGameStatus.CoolTime_Skill);

		vector<CGameObject*> pObjs = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();

		TEAM team = GETCHAMP(GetOwner())->GetTeamColor();
		CGameObject* pTarget = nullptr;
		for (size_t i = 0; i < pObjs.size(); ++i)
		{
			if (team != GETCHAMP(pObjs[i])->GetTeamColor()
				&& TEAM::NONE != GETCHAMP(pObjs[i])->GetTeamColor()
				&& TEAM::END != GETCHAMP(pObjs[i])->GetTeamColor())
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

		StateMachine()->AddBlackboardData(L"HP", BB_DATA::INT, &m_InGameStatus.HP);
		StateMachine()->AddBlackboardData(L"Target", BB_DATA::OBJECT, pTarget);


		if (nullptr != StateMachine()->GetFSM())
		{
			StateMachine()->GetFSM()->SetState(L"Idle");
		}
	}
}

void CFighterScript::CheckStateMachine()
{
	vector<CGameObject*> pObjs = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();
	if (StateMachine())
	{
		if (!pObjs.empty())
		{
			TEAM team = GETCHAMP(GetOwner())->GetTeamColor();
			CGameObject* pTarget = nullptr;
			for (size_t i = 0; i < pObjs.size(); ++i)
			{
				if (m_Target->IsActive()
					&& TEAM::NONE != team
					&& team != GETCHAMP(pObjs[i])->GetTeamColor()
					&& TEAM::NONE != GETCHAMP(pObjs[i])->GetTeamColor()
					&& TEAM::END != GETCHAMP(pObjs[i])->GetTeamColor())
				{
					Vec3 dist = Transform()->GetRelativePos() - pObjs[i]->Transform()->GetRelativePos();

					if (nullptr != pTarget && pTarget->IsActive())
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

void CFighterScript::SetChampInfo(int _MaxHP, int _ATK, int _DEF, float _ATKSpeed, int _ATKRange, int _MoveSpeed, CHAMP_TYPE _Type)
{
	m_Info.MaxHP = _MaxHP;
	m_Info.ATK = _ATK;
	m_Info.DEF = _DEF;
	m_Info.ATKSpeed = _ATKSpeed;
	m_Info.ATKRange = _ATKRange;
	m_Info.MOV = _MoveSpeed;
	m_Info.Type = _Type;
}

void CFighterScript::EnterIdleState()
{
	Animator2D()->Play(L"FighterIdle");
}

void CFighterScript::EnterTraceState()
{
	Animator2D()->Play(L"FighterTrace");
}

void CFighterScript::EnterAttackState()
{
	if (!m_bAttack && m_InGameStatus.CoolTime_Attack < m_Info.ATKSpeed)
	{
		Animator2D()->Play(L"FighterAttack");
		m_bAttack = true;
		m_InGameStatus.CoolTime_Attack = 0.f;
	}

	if (Animator2D()->GetCurAnim()->IsFinish())
	{
		Damaged(GetOwner(), m_Target);

		Animator2D()->FindAnim(L"FighterAttack")->Reset();

		if (m_bAttack)
		{
			m_State = CHAMP_STATE::IDLE;
		}
	}
}

void CFighterScript::EnterSkillState()
{

}

void CFighterScript::EnterUltimateState()
{
}

void CFighterScript::EnterDeadState()
{
	if (!m_bRespawn)
	{
		Animator2D()->Play(L"FighterDead", false);
		m_bRespawn = true;
	}

	if (Animator2D()->GetCurAnim() && Animator2D()->GetCurAnim()->IsFinish())
	{
		CBTMgr::GetInst()->RegistRespawnPool(GetOwner());
		Animator2D()->GetCurAnim()->Reset();
		m_InGameStatus.CoolTime_Attack = 0.f;
		GetOwner()->SetActive(false);
	}
}


//void CFighterScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
//{
//}
//
//void CFighterScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
//{
//}
//
//void CFighterScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
//{
//}
