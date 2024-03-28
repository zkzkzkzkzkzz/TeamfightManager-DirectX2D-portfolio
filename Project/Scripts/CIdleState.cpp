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
	int AttRange = *((int*)GetBlackboardData(L"AttackRange"));
	float DetectRange = *((float*)GetBlackboardData(L"DetectRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"Target"));

	CFSM* fsm = GetFSM();
	CStateMachine* sm = GetFSM()->GetStateMachine();

	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDist = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();

	GamePlayStatic::DrawDebugCircle(pSelf->Transform()->GetWorldPos(), DetectRange, Vec3(0.1f, 1.f, 0.1f), 0.f);

	if (vDist.Length() <= DetectRange && vDist.Length() > AttRange)
	{
		ChangeState(L"Trace");
	}
	else if (vDist.Length() <= DetectRange && vDist.Length() <= AttRange)
	{
		ChangeState(L"Attack");
	}
}

void CIdleState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	pSelf->GetScript<CChampScript>()->EnterIdleState();
}

void CIdleState::Exit()
{
}
