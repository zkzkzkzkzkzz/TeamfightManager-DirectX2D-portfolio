#include "pch.h"
#include "CTraceState.h"


#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>


CTraceState::CTraceState()
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::finaltick()
{
	float Speed = *((float*)GetBlackboardData(L"MoveSpeed"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"Target"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	vDir.Normalize();

	Vec3 vPos = pSelf->Transform()->GetWorldPos() + vDir * DT * Speed;
	pSelf->Transform()->SetRelativePos(vPos);
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}
