#include "pch.h"
#include "CNinjaScript.h"

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

#include "CNinjaCloneScript.h"

CNinjaScript::CNinjaScript()
	: CChampScript(NINJASCRIPT)
	, m_DealDelay(0.f)
	, m_DealActive(false)
	, m_Skill1Delay(0.f)
	, m_Skill2Delay(0.f)
	, m_Skill1Active(false)
	, m_Skill2Active(false)
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

CNinjaScript::CNinjaScript(const CNinjaScript& _Origin)
	: CChampScript(NINJASCRIPT)
	, m_DealDelay(0.f)
	, m_DealActive(false)
	, m_Skill1Delay(0.f)
	, m_Skill2Delay(0.f)
	, m_Skill1Active(false)
	, m_Skill2Active(false)
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

CNinjaScript::~CNinjaScript()
{
}

void CNinjaScript::begin()
{
	CChampScript::begin();

	InitChampInfo();
	InitChampAnim();
	InitStateMachine();

	SpawnShadow();
}

void CNinjaScript::tick()
{
	CChampScript::tick();

	CheckStateMachine();

	m_InGameStatus.CoolTime_Attack += DT;
	m_InGameStatus.CoolTime_Skill += DT;
	m_DealDelay += DT;
	m_Skill1Delay += DT;
	m_Skill2Delay += DT;
	m_UltiDelay += DT;
	m_DeadDelay += DT;

	float delay = 1 / m_Info.ATKSpeed;
	if (m_InGameStatus.CoolTime_Attack > delay)
	{
		m_bAttack = false;
	}
}

void CNinjaScript::render()
{
	CChampScript::render();
}


void CNinjaScript::InitChampInfo()
{
	SetChampInfo(120, 25, 10, 0.95f, 40, 8, CHAMP_TYPE::ASSASSIN);	// 기본 정보 설정

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

void CNinjaScript::InitChampStatus(int _GamerATK, int _GamerDEF)
{
	m_InGameStatus.HP = m_Info.MaxHP;
	m_InGameStatus.ATK = m_Info.ATK + _GamerATK;
	m_InGameStatus.DEF = m_Info.DEF + _GamerDEF;

	m_InGameStatus.CoolTime_Attack = 0.f;
	m_InGameStatus.CoolTime_Skill = 0.f;
	m_InGameStatus.bSkillPlay = false;
	m_InGameStatus.UltimateUseTime = 45.f;
	m_InGameStatus.bUltimate = false;

	m_InGameStatus.RespawnTime = 0.f;
}

void CNinjaScript::InitChampAnim()
{
	Transform()->SetRelativeScale(Vec3(128.f, 128.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ChampMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	Collider2D()->SetOffsetPos(Vec2(0.f, -8.f));
	Collider2D()->SetOffsetScale(Vec2(21.f, 10.f));

	Animator2D()->LoadAnimation(L"animdata\\NinjaIdle.txt");
	Animator2D()->LoadAnimation(L"animdata\\NinjaTrace.txt");
	Animator2D()->LoadAnimation(L"animdata\\NinjaAttack.txt");
	Animator2D()->LoadAnimation(L"animdata\\NinjaSkill1.txt");
	Animator2D()->LoadAnimation(L"animdata\\NinjaSkill2.txt");
	Animator2D()->LoadAnimation(L"animdata\\NinjaUlti.txt");
	Animator2D()->LoadAnimation(L"animdata\\CommonDead.txt");
	Animator2D()->Play(L"NinjaIdle");
}

void CNinjaScript::InitStateMachine()
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

void CNinjaScript::CheckStateMachine()
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

void CNinjaScript::SetChampInfo(int _MaxHP, int _ATK, int _DEF, float _ATKSpeed, int _ATKRange, int _MoveSpeed, CHAMP_TYPE _Type)
{
	m_Info.MaxHP = _MaxHP;
	m_Info.ATK = _ATK;
	m_Info.DEF = _DEF;
	m_Info.ATKSpeed = _ATKSpeed;
	m_Info.ATKRange = _ATKRange;
	m_Info.MOV = _MoveSpeed;
	m_Info.Type = _Type;
}

void CNinjaScript::EnterIdleState()
{
	Animator2D()->Play(L"NinjaIdle");
}

void CNinjaScript::EnterTraceState()
{
	Animator2D()->Play(L"NinjaTrace");
}

void CNinjaScript::EnterAttackState()
{
	if (!m_bAttack)
	{
		Animator2D()->FindAnim(L"NinjaAttack")->Reset();
		Animator2D()->Play(L"NinjaAttack", false);
		m_bAttack = true;
		m_InGameStatus.CoolTime_Attack = 0.f;
		m_DealActive = true;
		m_DealDelay = 0.f;

		Vec3 vOffset = Transform()->GetRelativePos();

		if (0.f == GetOwner()->Transform()->GetRelativeRotation().y)
			vOffset.x += 10.f;
		else
			vOffset.x -= 10.f;
		vOffset.y += 30.f;
		SpawnEffect(vOffset, Transform()->GetRelativeScale()
					, Transform()->GetRelativeRotation(), L"NinjaAttackEffect", 0.5f);
	}

	if (m_bAttack && m_DealActive && m_DealDelay > 0.5f)
	{
		GamePlayStatic::Play2DSound(L"sound\\Dagger8.wav", 1, 2.f, true);
		Damaged(GetOwner(), m_Target);
		m_DealActive = false;
	}
}

void CNinjaScript::EnterSkillState()
{
	if (!m_Skill1Active)
	{
		Animator2D()->FindAnim(L"NinjaSkill1")->Reset();
		Animator2D()->Play(L"NinjaSkill1", false);
		m_Skill1Delay = 0.f;
		m_Skill1Active = true;
		m_InGameStatus.bSkillPlay = true;

		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
			, Transform()->GetRelativeRotation(), L"NinjaSkill1Effect", 0.8f, false, Vec3(0.f, 30.f, 0.f));

		GamePlayStatic::Play2DSound(L"sound\\Dirtfootstep.wav", 1, 2.f, true);
	}
	else
	{
		if (m_Skill1Delay > 0.8f && !m_Skill2Active)
		{
			vector<CGameObject*> pObjs = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(3)->GetParentObjects();
			TEAM team = GETCHAMP(GetOwner())->GetTeamColor();
			CGameObject* pTarget = nullptr;
			for (size_t i = 0; i < pObjs.size(); i++)
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

						if (prevdist.Length() < dist.Length())
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

			Vec3 TargetPos = m_Target->Transform()->GetRelativePos();
			Vec2 TargetColSize = m_Target->Collider2D()->GetOffsetScale();

			if (0.f == m_Target->Transform()->GetRelativeRotation().y)
				TargetPos.x = TargetPos.x - TargetColSize.x;
			else
				TargetPos.x = TargetPos.x + TargetColSize.x;

			Transform()->SetRelativePos(TargetPos);
			Animator2D()->FindAnim(L"NinjaSkill2")->Reset();
			Animator2D()->Play(L"NinjaSkill2", false);
			m_Skill1Active = true;
			m_Skill1Delay = 0.f;
			m_Skill2Active = true;
			m_Skill2Delay = 0.f;

			SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
						, Transform()->GetRelativeRotation(), L"NinjaSkill2Effect", 0.5f, false ,Vec3(0.f, 30.f, 0.f));
		}
		else if (m_Skill2Active && m_Skill2Delay > 0.5f)
		{
			GamePlayStatic::Play2DSound(L"sound\\Dagger8.wav", 1, 2.f, true);
			Damaged(GetOwner(), m_Target, 30);
			m_Skill1Active = false;
			m_Skill2Active = false;
			m_InGameStatus.CoolTime_Skill = 0.f;
			m_InGameStatus.bSkillPlay = false;
		}
	}
}

void CNinjaScript::EnterUltimateState()
{
	if (!m_InGameStatus.bUltimate)
	{
		Animator2D()->FindAnim(L"NinjaUlti")->Reset();
		Animator2D()->Play(L"NinjaUlti", false);

		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
			, Transform()->GetRelativeRotation(), L"NinjaUltiEffect", 0.9f);

		m_InGameStatus.bUltimate = true;
		m_UltiDelay = 0.f;
		m_UltiActive = true;
	}
	else
	{
		if (m_UltiActive && m_UltiDelay > 0.9f)
		{
			CGameObject* clone = new CGameObject;
			clone->SetName(L"NinjaClone");
			clone->AddComponent(new CTransform);
			clone->AddComponent(new CMeshRender);
			clone->AddComponent(new CAnimator2D);
			clone->AddComponent(new CCollider2D);
			clone->AddComponent(new CStateMachine);
			clone->AddComponent(new CNinjaCloneScript);
			clone->GetScript<CNinjaCloneScript>()->SetTeam(GETCHAMP(GetOwner())->GetTeamColor());

			GamePlayStatic::SpawnGameObject(clone, 3);


			m_UltiActive = false;
			m_InGameStatus.bUltimateDone = true;
		}
	}
}

void CNinjaScript::EnterDeadState()
{
	if (!m_bRespawn)
	{
		Animator2D()->Play(L"CommonDead");
		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale()
			, Transform()->GetRelativeRotation(), L"NinjaDead", 0.9f);

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
