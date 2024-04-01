#include "pch.h"
#include "CMonkScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAnim.h>
#include <Engine\CAssetMgr.h>

#include "CBTMgr.h"
#include "CEffectScript.h"


CMonkScript::CMonkScript()
	: CChampScript(MONKSCRIPT)
	, m_DealDelay(0.f)
	, m_DealActive(false)
	, m_SkillDelay(0.f)
	, m_SkillActive(false)
	, m_UltiDelay(0.f)
	, m_UltiActive(false)
	, m_DeadDelay(0.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_InGameStatus.HP);
	AddScriptParam(SCRIPT_PARAM::INT, "ATK", &m_InGameStatus.ATK);
	AddScriptParam(SCRIPT_PARAM::INT, "DEF", &m_InGameStatus.DEF);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Speed", &m_Info.ATKSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Range", &m_Info.ATKRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed", &m_Info.MOV);
	AddScriptParam(SCRIPT_PARAM::INT, "Champ Type", &m_Info.Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Team", &m_Team);
	AddScriptParam(SCRIPT_PARAM::INT, "State", &m_State);
	AddScriptParam(SCRIPT_PARAM::INT, "ULTI", &m_InGameStatus.bUltimate);

	m_bAttack = false;
}

CMonkScript::CMonkScript(const CMonkScript& _Origin)
	: CChampScript(MONKSCRIPT)
	, m_DealDelay(0.f)
	, m_DealActive(false)
	, m_SkillDelay(0.f)
	, m_SkillActive(false)
	, m_UltiDelay(0.f)
	, m_UltiActive(false)
	, m_DeadDelay(0.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_InGameStatus.HP);
	AddScriptParam(SCRIPT_PARAM::INT, "ATK", &m_InGameStatus.ATK);
	AddScriptParam(SCRIPT_PARAM::INT, "DEF", &m_InGameStatus.DEF);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Speed", &m_Info.ATKSpeed);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Range", &m_Info.ATKRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed", &m_Info.MOV);
	AddScriptParam(SCRIPT_PARAM::INT, "Champ Type", &m_Info.Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Team", &m_Team);
	AddScriptParam(SCRIPT_PARAM::INT, "State", &m_State);
	AddScriptParam(SCRIPT_PARAM::INT, "ULTI", &m_InGameStatus.bUltimate);

	m_bAttack = false;
}

CMonkScript::~CMonkScript()
{
}


void CMonkScript::begin()
{
	CChampScript::begin();

	InitChampInfo();
	InitChampAnim();
	InitStateMachine();

	SpawnShadow();
}

void CMonkScript::tick()
{
	CChampScript::tick();

	CheckStateMachine();

	m_InGameStatus.CoolTime_Attack += DT;
	m_InGameStatus.CoolTime_Skill += DT;
	m_DealDelay += DT;
	m_SkillDelay += DT;
	m_UltiDelay += DT;
	m_DeadDelay += DT;

	float delay = 1 / m_Info.ATKSpeed;
	if (m_InGameStatus.CoolTime_Attack > delay)
	{
		m_bAttack = false;
	}
}

void CMonkScript::render()
{
	CChampScript::render();
}


void CMonkScript::InitChampInfo()
{
	SetChampInfo(220, 20, 40, 0.71f, 35, 5, CHAMP_TYPE::SUPPORT);	// 기본 정보 설정
	InitChampStatus(0, 0);	// 인게임 정보 설정

	m_State = CHAMP_STATE::IDLE;
	m_bRespawn = false;

	if (StateMachine() && nullptr != StateMachine()->GetFSM())
	{
		StateMachine()->GetFSM()->SetState(L"Idle");
	}
}

void CMonkScript::InitChampStatus(int _GamerATK, int _GamerDEF)
{
	m_InGameStatus.HP = m_Info.MaxHP;
	m_InGameStatus.ATK = m_Info.ATK + _GamerATK;
	m_InGameStatus.DEF = m_Info.DEF + _GamerDEF;

	m_InGameStatus.CoolTime_Attack = 0.f;
	m_InGameStatus.CoolTime_Skill = 0.f;
	m_InGameStatus.bSkillPlay = false;
	m_InGameStatus.UltimateUseTime = 25.f;
	m_InGameStatus.bUltimate = false;

	m_InGameStatus.RespawnTime = 0.f;

	m_InGameStatus.TotalDeal = 0;
	m_InGameStatus.TotalDamage = 0;
	m_InGameStatus.TotalHeal = 0;
	m_InGameStatus.KillPoint = 0;
	m_InGameStatus.DeathPoint = 0;
	m_InGameStatus.AssistPoint = 0;
}

void CMonkScript::InitChampAnim()
{
	Transform()->SetRelativeScale(Vec3(64.f, 64.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ChampMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	Collider2D()->SetOffsetPos(Vec2(0.f, -8.f));
	Collider2D()->SetOffsetScale(Vec2(21.f, 10.f));

	Animator2D()->LoadAnimation(L"animdata\\MonkIdle.txt");
	Animator2D()->LoadAnimation(L"animdata\\MonkTrace.txt");
	Animator2D()->LoadAnimation(L"animdata\\MonkAttack.txt");
	Animator2D()->LoadAnimation(L"animdata\\MonkSkill.txt");
	Animator2D()->LoadAnimation(L"animdata\\MonkUlti.txt");
	Animator2D()->LoadAnimation(L"animdata\\CommonDead.txt");
	Animator2D()->Play(L"MonkIdle");
}

void CMonkScript::InitStateMachine()
{
	if (StateMachine())
	{
		StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"ChampFSM"));

		StateMachine()->AddBlackboardData(L"MoveSpeed", BB_DATA::FLOAT, &m_Info.MOV);
		StateMachine()->AddBlackboardData(L"AttackRange", BB_DATA::INT, &m_Info.ATKRange);
		StateMachine()->AddBlackboardData(L"AttackSpeed", BB_DATA::INT, &m_Info.ATKSpeed);
		StateMachine()->AddBlackboardData(L"Skill_Cooltime", BB_DATA::INT, &m_InGameStatus.CoolTime_Skill);
		StateMachine()->AddBlackboardData(L"UltimateTime", BB_DATA::FLOAT, &m_InGameStatus.UltimateUseTime);

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

void CMonkScript::CheckStateMachine()
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
				if (TEAM::NONE != team
					&& team != GETCHAMP(pObjs[i])->GetTeamColor()
					&& TEAM::NONE != GETCHAMP(pObjs[i])->GetTeamColor()
					&& TEAM::END != GETCHAMP(pObjs[i])->GetTeamColor()
					&& CHAMP_STATE::DEAD != GETCHAMP(pObjs[i])->GetChampState())
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

				if (L"Knight" == pObjs[i]->GetName() && CHAMP_STATE::SKILL == GETCHAMP(pObjs[i])->GetChampState())
				{
					m_Target = pTarget = pObjs[i];
				}
			}


			StateMachine()->SetBlackboardData(L"Target", BB_DATA::OBJECT, pTarget);
		}
	}
}

void CMonkScript::SetChampInfo(int _MaxHP, int _ATK, int _DEF, float _ATKSpeed, int _ATKRange, int _MoveSpeed, CHAMP_TYPE _Type)
{
	m_Info.MaxHP = _MaxHP;
	m_Info.ATK = _ATK;
	m_Info.DEF = _DEF;
	m_Info.ATKSpeed = _ATKSpeed;
	m_Info.ATKRange = _ATKRange;
	m_Info.MOV = _MoveSpeed;
	m_Info.Type = _Type;
}

void CMonkScript::EnterIdleState()
{
	Animator2D()->Play(L"MonkIdle");
}

void CMonkScript::EnterTraceState()
{
	Animator2D()->Play(L"MonkTrace");
}

void CMonkScript::EnterAttackState()
{
	if (!m_bAttack)
	{
		Animator2D()->FindAnim(L"MonkAttack")->Reset();
		Animator2D()->Play(L"MonkAttack", false);
		m_bAttack = true;
		m_InGameStatus.CoolTime_Attack = 0.f;
		m_DealActive = true;
		m_DealDelay = 0.f;
	}

	if (m_bAttack && m_DealActive && m_DealDelay > 0.4f)
	{
		Damaged(GetOwner(), m_Target);
		m_DealActive = false;
	}
}

void CMonkScript::EnterSkillState()
{
	if (!m_SkillActive)
	{
		Animator2D()->FindAnim(L"MonkSkill")->Reset();
		Animator2D()->Play(L"MonkSkill", false);
		m_SkillDelay = 0.f;
		m_SkillActive = true;
		m_InGameStatus.bSkillPlay = true;

		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
					, Transform()->GetRelativeRotation(), L"MonkSkillEffect", 0.9f);

		vector<CGameObject*> pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();
		TEAM team = GETCHAMP(GetOwner())->GetTeamColor();

		for (size_t i = 0; i < pTarget.size(); ++i)
		{
			if (team == GETCHAMP(pTarget[i])->GetTeamColor())
			{
				int HP = GETCHAMP(pTarget[i])->GetInGameChampHP() + 25;
				if (GETCHAMP(pTarget[i])->GetChampMaxHP() <= HP)
				{
					HP = GETCHAMP(pTarget[i])->GetChampMaxHP();
				}
				
				GETCHAMP(pTarget[i])->SetChampHP(HP);
			}
		}
	}
	else
	{
		if (m_SkillDelay > 0.9f)
		{
			m_SkillActive = false;
			m_InGameStatus.CoolTime_Skill = 0.f;
			m_InGameStatus.bSkillPlay = false;
		}
	}
}

void CMonkScript::EnterUltimateState()
{
	if (!m_InGameStatus.bUltimate)
	{
		Animator2D()->FindAnim(L"MonkUlti")->Reset();
		Animator2D()->Play(L"MonkUlti", false);

		SpawnEffect(Transform()->GetRelativePos(), Vec3(388.f, 379.f, 1.f)
			, Transform()->GetRelativeRotation(), L"MonkUltiEffect", 1.2f);

		m_InGameStatus.bUltimate = true;
		m_UltiDelay = 0.f;
		m_UltiActive = true;

		vector<CGameObject*> pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();
		TEAM team = GETCHAMP(GetOwner())->GetTeamColor();

		for (size_t i = 0; i < pTarget.size(); ++i)
		{
			if (team == GETCHAMP(pTarget[i])->GetTeamColor())
			{
				int HP = GETCHAMP(pTarget[i])->GetInGameChampHP() + 150;
				GETCHAMP(pTarget[i])->SetChampHP(HP);
			}

			int Speed = GETCHAMP(pTarget[i])->GetChampMoveSpeed() + 0.3f;
			GETCHAMP(pTarget[i])->SetChampMoveSpeed(Speed);
		}
	}
	else
	{
		if (m_UltiActive && m_UltiDelay > 1.2f)
		{
			m_UltiActive = false;
			m_InGameStatus.bUltimateDone = true;
		}
	}
}

void CMonkScript::EnterDeadState()
{
	if (!m_bRespawn)
	{
		Animator2D()->Play(L"CommonDead");
		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
			, Transform()->GetRelativeRotation(), L"MonkDead", 1.5f);

		m_DeadDelay = 0.f;
		m_bRespawn = true;
	}
	else
	{
		if (m_DeadDelay > 1.5f)
		{
			CBTMgr::GetInst()->RegistRespawnPool(GetOwner());
			m_InGameStatus.CoolTime_Attack = 0.f;
			m_InGameStatus.CoolTime_Skill = 0.f;
			GetOwner()->SetActive(false);
		}
	}
}
