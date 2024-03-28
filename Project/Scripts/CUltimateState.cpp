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
}

void CUltimateState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	pSelf->GetScript<CChampScript>()->EnterDeadState();
}

void CUltimateState::Exit()
{
}

