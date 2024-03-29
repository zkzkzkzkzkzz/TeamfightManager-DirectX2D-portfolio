#include "pch.h"
#include "CSkillState.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>
#include <Engine\CFSM.h>

#include "CChampScript.h"


CSkillState::CSkillState()
{
}

CSkillState::~CSkillState()
{
}


void CSkillState::finaltick()
{
	int HP = *((int*)GetBlackboardData(L"HP"));
	if (HP <= 0)
	{
		ChangeState(L"Dead");
		return;
	}

	int AttRange = *((int*)GetBlackboardData(L"AttackRange"));
	float Speed = *((float*)GetBlackboardData(L"MoveSpeed"));
	float CoolTime = *((float*)GetBlackboardData(L"Skill_Cooltime"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"Target"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	if (nullptr == pTarget)
	{
		ChangeState(L"Idle");
	}
	else
	{
		Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();

		if (vDir.x > 0)
			pSelf->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		else
			pSelf->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));

		if (vDir.Length() > AttRange)
		{
			ChangeState(L"Trace");
		}
		else if (vDir.Length() <= AttRange)
		{
			if (vDir.Length() < AttRange - 20.f && CoolTime >= SKILLCOOLTIME)
				ChangeState(L"Skill");
			else
				ChangeState(L"Attack");
		}
	}
}

void CSkillState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	GETCHAMP(pSelf)->SetChampState(CHAMP_STATE::SKILL);
}

void CSkillState::Exit()
{
}
