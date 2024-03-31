#include "pch.h"
#include "CKnightScript.h"

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

CKnightScript::CKnightScript()
	: CChampScript(KNIGHTSCRIPT)
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

CKnightScript::CKnightScript(const CKnightScript& _Origin)
	: CChampScript(KNIGHTSCRIPT)
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

CKnightScript::~CKnightScript()
{
}

void CKnightScript::begin()
{
	CChampScript::begin();

	InitChampInfo();
	InitChampAnim();
	InitStateMachine();
}

void CKnightScript::tick()
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

void CKnightScript::InitChampInfo()
{
	SetChampInfo(200, 21, 60, 0.67f, 37, 5, CHAMP_TYPE::WARRIOR);	// 기본 정보 설정
	InitChampStatus(0, 0);	// 인게임 정보 설정

	m_State = CHAMP_STATE::IDLE;
	m_bRespawn = false;

	if (StateMachine() && nullptr != StateMachine()->GetFSM())
	{
		StateMachine()->GetFSM()->SetState(L"Idle");
	}
}

void CKnightScript::InitChampStatus(int _GamerATK, int _GamerDEF)
{
	m_InGameStatus.HP = m_Info.MaxHP;
	m_InGameStatus.ATK = m_Info.ATK + _GamerATK;
	m_InGameStatus.DEF = m_Info.DEF + _GamerDEF;

	m_InGameStatus.CoolTime_Attack = 0.f;
	m_InGameStatus.CoolTime_Skill = 0.f;
	m_InGameStatus.bSkillPlay = false;
	m_InGameStatus.UltimateUseTime = 30.f;
	m_InGameStatus.bUltimate = false;

	m_InGameStatus.RespawnTime = 0.f;

	m_InGameStatus.TotalDeal = 0;
	m_InGameStatus.TotalDamage = 0;
	m_InGameStatus.TotalHeal = 0;
	m_InGameStatus.KillPoint = 0;
	m_InGameStatus.DeathPoint = 0;
	m_InGameStatus.AssistPoint = 0;
}

void CKnightScript::InitChampAnim()
{
	Transform()->SetRelativeScale(Vec3(96.f, 96.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ChampMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	Collider2D()->SetOffsetPos(Vec2(2.f, -7.f));
	Collider2D()->SetOffsetScale(Vec2(23.f, 10.f));

	Animator2D()->LoadAnimation(L"animdata\\KnightIdle.txt");
	Animator2D()->LoadAnimation(L"animdata\\KnightTrace.txt");
	Animator2D()->LoadAnimation(L"animdata\\KnightAttack.txt");
	Animator2D()->LoadAnimation(L"animdata\\KnightSkill.txt");
	Animator2D()->LoadAnimation(L"animdata\\KnightUlti.txt");
	Animator2D()->LoadAnimation(L"animdata\\CommonDead.txt");
	Animator2D()->Play(L"KnightIdle");
}

void CKnightScript::InitStateMachine()
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

void CKnightScript::CheckStateMachine()
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

void CKnightScript::SetChampInfo(int _MaxHP, int _ATK, int _DEF, float _ATKSpeed, int _ATKRange, int _MoveSpeed, CHAMP_TYPE _Type)
{
	m_Info.MaxHP = _MaxHP;
	m_Info.ATK = _ATK;
	m_Info.DEF = _DEF;
	m_Info.ATKSpeed = _ATKSpeed;
	m_Info.ATKRange = _ATKRange;
	m_Info.MOV = _MoveSpeed;
	m_Info.Type = _Type;
}

void CKnightScript::EnterIdleState()
{
	Animator2D()->Play(L"KnightIdle");
}

void CKnightScript::EnterTraceState()
{
	Animator2D()->Play(L"KnightTrace");
}

void CKnightScript::EnterAttackState()
{
	if (!m_bAttack)
	{
		Animator2D()->FindAnim(L"KnightAttack")->Reset();
		Animator2D()->Play(L"KnightAttack", false);
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

void CKnightScript::EnterSkillState()
{
	if (!m_SkillActive)
	{
		Animator2D()->FindAnim(L"KnightSkill")->Reset();
		Animator2D()->Play(L"KnightSkill", false);
		m_SkillDelay = 0.f;
		m_SkillActive = true;
		m_InGameStatus.bSkillPlay = true;

		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
					, Transform()->GetRelativeRotation(), L"KnightSkillEffect", 0.8f);

		m_InGameStatus.DEF += 50.f;
	}
	else
	{
		if (m_SkillDelay > 1.6f)
		{
			m_InGameStatus.DEF = m_Info.DEF;
			m_SkillActive = false;
			m_InGameStatus.CoolTime_Skill = 0.f;
			m_InGameStatus.bSkillPlay = false;
		}
	}
}

void CKnightScript::EnterUltimateState()
{
	if (!m_InGameStatus.bUltimate)
	{
		Animator2D()->FindAnim(L"KnightUlti")->Reset();
		Animator2D()->Play(L"KnightUlti", false);

		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
				, Transform()->GetRelativeRotation(), L"KnightUltiEffect", 0.8f);

		m_InGameStatus.bUltimate = true;
		m_UltiDelay = 0.f;
		m_UltiActive = true;

		vector<CGameObject*> pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();
		TEAM team = GETCHAMP(GetOwner())->GetTeamColor();

		for (size_t i = 0; i < pTarget.size(); ++i)
		{
			if (team == GETCHAMP(pTarget[i])->GetTeamColor())
			{
				int def = GETCHAMP(pTarget[i])->GetInGameChampDEF() + 150;
				GETCHAMP(pTarget[i])->SetChampDEF(def);
			}
		}
	}
	else
	{
		if (m_UltiActive && m_UltiDelay > 0.8f)
		{
			m_UltiActive = false;
			m_InGameStatus.bUltimateDone = true;
		}
	}
}

void CKnightScript::EnterDeadState()
{
	if (!m_bRespawn)
	{
		Animator2D()->Play(L"CommonDead");
		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
			, Transform()->GetRelativeRotation(), L"KnightDead", 1.f);

		m_DeadDelay = 0.f;
		m_bRespawn = true;
	}
	else
	{
		if (m_DeadDelay > 1.f)
		{
			CBTMgr::GetInst()->RegistRespawnPool(GetOwner());
			m_InGameStatus.CoolTime_Attack = 0.f;
			m_InGameStatus.CoolTime_Skill = 0.f;
			GetOwner()->SetActive(false);
		}
	}
}


