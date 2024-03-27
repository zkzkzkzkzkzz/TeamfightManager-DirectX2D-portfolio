#include "pch.h"
#include "CIdleState.h"

#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>

CIdleState::CIdleState()
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::finaltick()
{
	float DetectRange = *((float*)GetBlackboardData(L"DetectRange"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"Target"));

	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDist = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();

	GamePlayStatic::DrawDebugCircle(pSelf->Transform()->GetWorldPos(), DetectRange, Vec3(0.1f, 1.f, 0.1f), 0.f);

	if (vDist.Length() <= DetectRange)
	{
		// 추적상태로 변경한다.
		ChangeState(L"Trace");
	}
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}
