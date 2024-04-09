#include "pch.h"
#include "CPriestScript.h"

#include <ctime>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAnim.h>
#include <Engine\CAssetMgr.h>

#include "CBTMgr.h"
#include "CGamerScript.h"
#include "CEffectScript.h"

CPriestScript::CPriestScript()
	: CChampScript(PRIESTSCRIPT)
	, m_DealDelay(0.f)
	, m_DealActive(false)
	, m_healdelay(0.f)
	, m_healactive(false)
	, m_SkillDelay(0.f)
	, m_SkillActive(false)
	, m_UltiDelay(0.f)
	, m_UltiActive(false)
	, m_DeadDelay(0.f)
	, m_UltiPos{}
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

CPriestScript::CPriestScript(const CPriestScript& _Origin)
	: CChampScript(PRIESTSCRIPT)
	, m_DealDelay(0.f)
	, m_DealActive(false)
	, m_healdelay(0.f)
	, m_healactive(false)
	, m_SkillDelay(0.f)
	, m_SkillActive(false)
	, m_UltiDelay(0.f)
	, m_UltiActive(false)
	, m_DeadDelay(0.f)
	, m_UltiPos{}
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

CPriestScript::~CPriestScript()
{
}

void CPriestScript::begin()
{
	CChampScript::begin();

	InitChampInfo();
	InitChampAnim();
	InitStateMachine();

	SpawnShadow();
}

void CPriestScript::tick()
{
	CChampScript::tick();

	CheckStateMachine();

	m_InGameStatus.CoolTime_Attack += DT;
	m_InGameStatus.CoolTime_Skill += DT;
	m_DealDelay += DT;
	m_SkillDelay += DT;
	m_UltiDelay += DT;
	m_DeadDelay += DT;
	m_healdelay += DT;

	float delay = 1 / m_Info.ATKSpeed;

	if (m_InGameStatus.CoolTime_Attack > delay)
	{
		m_bAttack = false;
	}
}

void CPriestScript::render()
{
	CChampScript::render();
}


void CPriestScript::InitChampInfo()
{
	SetChampInfo(65, 30, 3, 0.69f, 130, 3, CHAMP_TYPE::SUPPORT);	// 기본 정보 설정

	int atk = GETGAMER(m_PlayingGamer)->GetATK();
	int def = GETGAMER(m_PlayingGamer)->GetDEF();
	InitChampStatus(atk, def);	// 인게임 정보 설정

	m_State = CHAMP_STATE::IDLE;
	m_bRespawn = false;

	if (StateMachine() && nullptr != StateMachine()->GetFSM())
	{
		StateMachine()->GetFSM()->SetState(L"Idle");
	}
}

void CPriestScript::InitChampStatus(int _GamerATK, int _GamerDEF)
{
	m_InGameStatus.HP = m_Info.MaxHP;
	m_InGameStatus.ATK = m_Info.ATK + _GamerATK;
	m_InGameStatus.DEF = m_Info.DEF + _GamerDEF;

	m_InGameStatus.CoolTime_Attack = 0.f;
	m_InGameStatus.CoolTime_Skill = 0.f;
	m_InGameStatus.bSkillPlay = false;
	m_InGameStatus.UltimateUseTime = 0.f;
	m_InGameStatus.bUltimate = false;

	m_InGameStatus.RespawnTime = 0.f;
}

void CPriestScript::InitChampAnim()
{
	Transform()->SetRelativeScale(Vec3(128.f, 128.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ChampMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	Collider2D()->SetOffsetPos(Vec2(0.f, -8.f));
	Collider2D()->SetOffsetScale(Vec2(21.f, 10.f));

	Animator2D()->LoadAnimation(L"animdata\\PriestIdle.txt");
	Animator2D()->LoadAnimation(L"animdata\\PriestTrace.txt");
	Animator2D()->LoadAnimation(L"animdata\\PriestHeal.txt");
	Animator2D()->LoadAnimation(L"animdata\\PriestSkill.txt");
	Animator2D()->LoadAnimation(L"animdata\\PriestUlti.txt");
	Animator2D()->LoadAnimation(L"animdata\\CommonDead.txt");
	Animator2D()->Play(L"PriestIdle");
}

void CPriestScript::InitStateMachine()
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
			if (team == GETCHAMP(pObjs[i])->GetTeamColor())
			{
				int HP = GETCHAMP(pObjs[i])->GetInGameChampHP();

				if (HP >= GETCHAMP(pObjs[i])->GetChampMaxHP())
					continue;

				if (nullptr != pTarget)
				{
					int prevHP = GETCHAMP(pTarget)->GetInGameChampHP();

					if (HP < prevHP)
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

void CPriestScript::CheckStateMachine()
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
				if (team == GETCHAMP(pObjs[i])->GetTeamColor()
					&& CHAMP_STATE::DEAD != GETCHAMP(pObjs[i])->GetChampState())
				{
					int HP = GETCHAMP(pObjs[i])->GetInGameChampHP();

					if (HP >= GETCHAMP(pObjs[i])->GetChampMaxHP())
						continue;

					if (nullptr != pTarget)
					{
						int prevHP = GETCHAMP(pTarget)->GetInGameChampHP();

						if (HP < prevHP)
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

void CPriestScript::SetChampInfo(int _MaxHP, int _ATK, int _DEF, float _ATKSpeed, int _ATKRange, int _MoveSpeed, CHAMP_TYPE _Type)
{
	m_Info.MaxHP = _MaxHP;
	m_Info.ATK = _ATK;
	m_Info.DEF = _DEF;
	m_Info.ATKSpeed = _ATKSpeed;
	m_Info.ATKRange = _ATKRange;
	m_Info.MOV = _MoveSpeed;
	m_Info.Type = _Type;
}


void CPriestScript::EnterIdleState()
{
	Animator2D()->Play(L"PriestIdle");
}

void CPriestScript::EnterTraceState()
{
	Animator2D()->Play(L"PriestTrace");
}

void CPriestScript::EnterAttackState()
{
	if (!m_bAttack)
	{
		Animator2D()->FindAnim(L"PriestHeal")->Reset();
		Animator2D()->Play(L"PriestHeal", false);
		m_bAttack = true;
		m_DealActive = true;
		m_InGameStatus.CoolTime_Attack = 0.f;

		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
			, Transform()->GetRelativeRotation(), L"PriestAttackEffect1", 1.f);

		m_DealDelay = 0.f;
	}

	if (m_bAttack && !m_healactive)
	{
		SpawnEffect(m_Target->Transform()->GetRelativePos(), Vec3(38.f, 39.f, 1.f)
			, Transform()->GetRelativeRotation(), L"PriestHealEffectDown", 0.6f);

		SpawnEffect(m_Target->Transform()->GetRelativePos(), Vec3(38.f, 39.f, 1.f)
			, Transform()->GetRelativeRotation(), L"PriestHealEffectUp", 0.6f, false, Vec3(0.f, 0.f, -10.f));

		m_healactive = true;
		m_healdelay = 0.f;
	}


	if (m_healactive && m_healdelay > 0.6f)
	{
		if (m_bAttack && m_DealActive && m_DealDelay > 1.f)
		{
			Healed(GetOwner(), m_Target);
			m_DealActive = false;
			m_healactive = false;
		}
	}
}

void CPriestScript::EnterSkillState()
{
	if (!m_SkillActive)
	{
		Animator2D()->FindAnim(L"PriestSkill")->Reset();
		Animator2D()->Play(L"PriestSkill", false);
		m_SkillDelay = 0.f;
		m_SkillActive = true;
		m_InGameStatus.bSkillPlay = true;

		SpawnEffect(Transform()->GetRelativePos(), Vec3(29.f, 29.f, 1.f)
			, Transform()->GetRelativeRotation(), L"PriestSkillEffect", 0.9f);

		vector<CGameObject*> pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();
		TEAM team = GETCHAMP(GetOwner())->GetTeamColor();

		for (size_t i = 0; i < pTarget.size(); ++i)
		{
			if (team == GETCHAMP(pTarget[i])->GetTeamColor())
			{
				float ATKSpeed = GETCHAMP(pTarget[i])->GetChampATKSpeed() + 0.5f;

				GETCHAMP(pTarget[i])->SetChampATKSpeed(ATKSpeed);
			}
		}
	}
	else
	{
		if (m_SkillDelay > 0.9f)
		{
			vector<CGameObject*> pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();
			TEAM team = GETCHAMP(GetOwner())->GetTeamColor();

			for (size_t i = 0; i < pTarget.size(); ++i)
			{
				if (team == GETCHAMP(pTarget[i])->GetTeamColor())
				{
					float ATKSpeed = GETCHAMP(pTarget[i])->GetChampATKSpeed() - 0.5f;

					GETCHAMP(pTarget[i])->SetChampATKSpeed(ATKSpeed);
				}
			}

			m_SkillActive = false;
			m_InGameStatus.CoolTime_Skill = 0.f;
			m_InGameStatus.bSkillPlay = false;
		}
	}
}

void CPriestScript::EnterUltimateState()
{
	if (!m_InGameStatus.bUltimate)
	{
		Animator2D()->FindAnim(L"PriestUlti")->Reset();
		Animator2D()->Play(L"PriestUlti", false);
		m_UltiPos = Transform()->GetRelativePos();
		SpawnEffect(m_UltiPos, Vec3(81.f, 56.f, 0.f), Transform()->GetRelativeRotation()
					, L"PriestUltiEffect", 1.f);

		m_InGameStatus.bUltimate = true;
		m_UltiDelay = 0.f;
		m_UltiActive = true;
	}
	else
	{
		if (m_UltiActive && m_UltiDelay > 1.f && m_UltiDelay < 2.f)
		{
			Healed(GetOwner(), m_Target);
		}
		else if (m_UltiActive && m_UltiDelay >= 2.f)
		{
			m_UltiActive = false;
			m_InGameStatus.bUltimateDone = true;
		}
	}
}

void CPriestScript::EnterDeadState()
{
	if (!m_bRespawn)
	{
		Animator2D()->Play(L"CommonDead");
		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
			, Transform()->GetRelativeRotation(), L"PriestDead", 0.9f);

		m_DeadDelay = 0.f;
		m_bRespawn = true;
	}
	else
	{
		if (m_DeadDelay > 0.9f)
		{
			CBTMgr::GetInst()->RegistRespawnPool(GetOwner());
			m_InGameStatus.CoolTime_Attack = 0.f;
			m_InGameStatus.CoolTime_Skill = 0.f;
			GetOwner()->SetActive(false);
		}
	}
}
