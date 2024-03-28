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
	int HP = *((int*)GetBlackboardData(L"HP"));
	if (HP <= 0)
	{
		ChangeState(L"Dead");
		return;
	}

	int AttRange = *((int*)GetBlackboardData(L"AttackRange"));
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

		if (vDir.x > 0)
			pSelf->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		else
			pSelf->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));

		if (vDir.Length() > AttRange)
		{
			vDir.Normalize();

			Vec3 vPos = pSelf->Transform()->GetWorldPos() + vDir * DT * Speed * 25.f;
			pSelf->Transform()->SetRelativePos(vPos);
		}
		else if (vDir.Length() <= AttRange)
		{
			ChangeState(L"Attack");
		}
	}
}

void CTraceState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	pSelf->GetScript<CChampScript>()->SetChampState(CHAMP_STATE::TRACE);
}

void CTraceState::Exit()
{
}
