#include "pch.h"
#include "CTraceState.h"


#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>
#include <Engine\CFSM.h>

#include "CChampScript.h"

CTraceState::CTraceState()
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::finaltick()
{
	int AttRange = *((int*)GetBlackboardData(L"AttackRange"));
	float DetectRange = *((float*)GetBlackboardData(L"DetectRange"));
	float Speed = *((float*)GetBlackboardData(L"MoveSpeed"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"Target"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	if (nullptr == pTarget)
	{
		ChangeState(L"Idle");
	}
	else
	{
		Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();

		if (vDir.Length() <= DetectRange && vDir.Length() > AttRange)
		{
			vDir.Normalize();

			Vec3 vPos = pSelf->Transform()->GetWorldPos() + vDir * DT * Speed * 30.f;
			pSelf->Transform()->SetRelativePos(vPos);
		}
		else if (vDir.Length() <= DetectRange && vDir.Length() <= AttRange)
		{
			ChangeState(L"Attack");
		}
	}
}

void CTraceState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	pSelf->GetScript<CChampScript>()->EnterTraceState();
}

void CTraceState::Exit()
{
}
