#include "pch.h"
#include "CIdleState.h"

#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>
#include <Engine\CFSM.h>
#include "CChampScript.h"

CIdleState::CIdleState()
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::finaltick()
{
	int HP = *((int*)GetBlackboardData(L"HP"));
	if (HP <= 0)
	{
		ChangeState(L"Dead");
		return;
	}

	int AttRange = *((int*)GetBlackboardData(L"AttackRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"Target"));

	CFSM* fsm = GetFSM();
	CStateMachine* sm = GetFSM()->GetStateMachine();

	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	if (nullptr != pTarget)
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
		else if (vDir.Length() <= AttRange && !(pSelf->GetScript<CChampScript>()->IsAttack()))
		{
			ChangeState(L"Attack");
		}
	}
}

void CIdleState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	pSelf->GetScript<CChampScript>()->SetChampState(CHAMP_STATE::IDLE);
}

void CIdleState::Exit()
{
}
