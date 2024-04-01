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

#include "CBTMgr.h"
#include "CEffectScript.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CArrowScript.h"

CArcherScript::CArcherScript()
	: CChampScript(ARCHERSCRIPT)
	, m_arrowDelay(0.f)
	, m_arrowspawn(false)
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

CArcherScript::CArcherScript(const CArcherScript& _Origin)
	: CChampScript(ARCHERSCRIPT)
	, m_arrowDelay(0.f)
	, m_arrowspawn(false)
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

CArcherScript::~CArcherScript()
{
}


void CArcherScript::begin()
{
	CChampScript::begin();

	InitChampInfo();
	InitChampAnim();
	InitStateMachine();

	SpawnShadow();
}

void CArcherScript::tick()
{
	CChampScript::tick();

	CheckStateMachine();

	m_InGameStatus.CoolTime_Attack += DT;
	m_InGameStatus.CoolTime_Skill += DT;
	m_arrowDelay += DT;
	m_SkillDelay += DT;
	m_UltiDelay += DT;
	m_DeadDelay += DT;

	float delay = 1 / m_Info.ATKSpeed;
	if (m_InGameStatus.CoolTime_Attack > delay)
	{
		m_bAttack = false;
	}
}

void CArcherScript::render()
{
	CChampScript::render();
}


void CArcherScript::InitChampInfo()
{
	SetChampInfo(100, 42, 5, 0.67f, 120, 3, CHAMP_TYPE::MARKSMAN);	// 기본 정보 설정
	InitChampStatus(0, 0);	// 인게임 정보 설정

	m_State = CHAMP_STATE::IDLE;
	m_bRespawn = false;

	if (StateMachine() && nullptr != StateMachine()->GetFSM())
	{
		StateMachine()->GetFSM()->SetState(L"Idle");
	}
}

void CArcherScript::InitChampStatus(int _GamerATK, int _GamerDEF)
{
	m_InGameStatus.HP = m_Info.MaxHP;
	m_InGameStatus.ATK = m_Info.ATK + _GamerATK;
	m_InGameStatus.DEF = m_Info.DEF + _GamerDEF;

	m_InGameStatus.CoolTime_Attack = 0.f;
	m_InGameStatus.CoolTime_Skill = 0.f;
	m_InGameStatus.UltimateUseTime = 10.f;
	m_InGameStatus.bSkillPlay = false;

	if (false == m_InGameStatus.bUltimate)
	{
		m_InGameStatus.bUltimate = false;
	}
	
	m_InGameStatus.RespawnTime = 0.f;
	
	m_InGameStatus.TotalDeal = 0;
	m_InGameStatus.TotalDamage = 0;
	m_InGameStatus.TotalHeal = 0;
	m_InGameStatus.KillPoint = 0;
	m_InGameStatus.DeathPoint = 0;
	m_InGameStatus.AssistPoint = 0;
}

void CArcherScript::InitChampAnim()
{
	Transform()->SetRelativeScale(Vec3(64.f, 64.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ChampMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	Collider2D()->SetOffsetPos(Vec2(1.f, -8.f));
	Collider2D()->SetOffsetScale(Vec2(21.f, 10.f));

	Animator2D()->LoadAnimation(L"animdata\\ArcherIdle.txt");
	Animator2D()->LoadAnimation(L"animdata\\ArcherTrace.txt");
	Animator2D()->LoadAnimation(L"animdata\\ArcherAttack.txt");
	Animator2D()->LoadAnimation(L"animdata\\ArcherDead.txt");
	Animator2D()->LoadAnimation(L"animdata\\ArcherSkill.txt");
	Animator2D()->LoadAnimation(L"animdata\\CommonDead.txt");
	Animator2D()->Play(L"ArcherIdle");
}

void CArcherScript::InitStateMachine()
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

void CArcherScript::CheckStateMachine()
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
				if ( TEAM::NONE != team
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

void CArcherScript::SetChampInfo(int _MaxHP, int _ATK, int _DEF, float _ATKSpeed, int _ATKRange, int _MoveSpeed, CHAMP_TYPE _Type)
{
	m_Info.MaxHP = _MaxHP;
	m_Info.ATK = _ATK;
	m_Info.DEF = _DEF;
	m_Info.ATKSpeed = _ATKSpeed;
	m_Info.ATKRange = _ATKRange;
	m_Info.MOV = _MoveSpeed;
	m_Info.Type = _Type;
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
	if (!m_bAttack)
	{
		Animator2D()->FindAnim(L"ArcherAttack")->Reset();
		Animator2D()->Play(L"ArcherAttack", false);
		m_bAttack = true;
		m_InGameStatus.CoolTime_Attack = 0.f;
		m_arrowDelay = 0.f;
		m_arrowspawn = false;
	}

	if (m_bAttack && !m_arrowspawn && m_arrowDelay > 0.4f && nullptr != m_Target)
	{
		SpawnArrow();
	}
}

void CArcherScript::EnterSkillState()
{
	if (!m_SkillActive)
	{
		Animator2D()->FindAnim(L"ArcherSkill")->Reset();
		Animator2D()->Play(L"ArcherSkill", false);
		m_SkillActive = true;
		m_arrowspawn = false;
		m_arrowDelay = 0.f;
		m_SkillDelay = 0.f;
		m_InGameStatus.bSkillPlay = true;
	}
	else
	{
		if (m_SkillDelay < 0.65f)
		{
			BackStepMoving();
		}
		else
		{
			if (!m_arrowspawn && m_arrowDelay > 0.3f)
			{
				SpawnArrow();
				m_SkillActive = false;
				m_InGameStatus.CoolTime_Skill = 0.f;
				m_InGameStatus.bSkillPlay = false;
			}
		}
	}
}

void CArcherScript::EnterUltimateState()
{
	if (!m_InGameStatus.bUltimate)
	{
		Animator2D()->FindAnim(L"ArcherAttack")->Reset();
		Animator2D()->Play(L"ArcherAttack", false);
		Vec3 vPos = Transform()->GetRelativePos();
		if (0.f == Transform()->GetRelativeRotation().y)
			vPos.x += 20.f;
		else
			vPos.x -= 20.f;
		vPos.y += 5.f;
		vPos.z -= 10.f;
		SpawnEffect(vPos, Transform()->GetRelativeScale()
					, Transform()->GetRelativeRotation(), L"ArcherUltiEnter", 0.3f);

		m_InGameStatus.bUltimate = true;
		m_UltiDelay = 0.f;
		m_arrowspawn = false;
		m_arrowDelay = 0.f;
	}
	else
	{
		if (!m_UltiActive && m_UltiDelay >= 0.3f && m_UltiDelay < 3.f)
		{
			Vec3 vPos = Transform()->GetRelativePos();
			if (0.f == Transform()->GetRelativeRotation().y)
				vPos.x += 20.f;
			else
				vPos.x -= 20.f;
			vPos.y += 5.f;
			vPos.z -= 10.f;
			SpawnEffect(vPos, Transform()->GetRelativeScale()
						, Transform()->GetRelativeRotation(), L"ArcherUltiPlay", 3.f, true);

			m_UltiActive = true;
		}
		else if (m_UltiActive && m_UltiDelay >= 3.3f)
		{
			Vec3 vPos = Transform()->GetRelativePos();
			if (0.f == Transform()->GetRelativeRotation().y)
				vPos.x += 20.f;
			else
				vPos.x -= 20.f;
			vPos.y += 5.f;
			vPos.z -= 10.f;
			SpawnEffect(vPos, Transform()->GetRelativeScale()
						, Transform()->GetRelativeRotation(), L"ArcherUltiExit", 0.4f);

			m_UltiActive = false;
			m_InGameStatus.bUltimateDone = true;
		}

		if (m_UltiActive && !m_arrowspawn && nullptr != m_Target && CHAMP_STATE::DEAD != GETCHAMP(m_Target)->GetChampState())
			SpawnArrow();
		else if (m_arrowspawn && m_arrowDelay > 0.1f)
		{
			m_arrowspawn = false;
			m_arrowDelay = 0.f;
		}
	}
}

void CArcherScript::EnterDeadState()
{
	if (!m_bRespawn)
	{
		Animator2D()->Play(L"CommonDead");
		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
					, Transform()->GetRelativeRotation(), L"ArcherDead", 1.f);

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

void CArcherScript::BackStepMoving()
{
	Vec3 vDir = m_Target->Transform()->GetRelativePos() - Transform()->GetRelativePos();
	vDir.Normalize();

	Vec3 vNewPos = Transform()->GetRelativePos() + ( -1 * vDir * DT * 30.f);
	Transform()->SetRelativePos(vNewPos);
}

void CArcherScript::SpawnArrow()
{
	CGameObject* arrow = new CGameObject;
	arrow->SetName(L"Arrow");
	arrow->AddComponent(new CTransform);
	arrow->AddComponent(new CMeshRender);
	arrow->AddComponent(new CCollider2D);
	arrow->AddComponent(new CArrowScript);
	arrow->GetScript<CArrowScript>()->SetShooter(GetOwner());
	arrow->GetScript<CArrowScript>()->SetTarget(m_Target);
	GamePlayStatic::SpawnGameObject(arrow, 5);
	m_arrowspawn = true;
}
