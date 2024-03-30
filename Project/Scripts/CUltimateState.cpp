#include "pch.h"
#include "CUltimateState.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>
#include <Engine\CFSM.h>

#include "CChampScript.h"

CUltimateState::CUltimateState()
{
}

CUltimateState::~CUltimateState()
{
}

void CUltimateState::finaltick()
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

	if (GETCHAMP(pSelf)->IsUltimateDone())
	{
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
				if (CoolTime >= SKILLCOOLTIME)
					ChangeState(L"Skill");
				else
					ChangeState(L"Attack");
			}
		}
	}
}

void CUltimateState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	GETCHAMP(pSelf)->SetChampState(CHAMP_STATE::ULTIMATE);
}

void CUltimateState::Exit()
{
}

