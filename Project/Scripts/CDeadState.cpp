#include "pch.h"
#include "CDeadState.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>
#include <Engine\CFSM.h>

#include "CChampScript.h"


CDeadState::CDeadState()
{
}

CDeadState::~CDeadState()
{
}

void CDeadState::finaltick()
{

}

void CDeadState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	GETCHAMP(pSelf)->SetChampState(CHAMP_STATE::DEAD);
}

void CDeadState::Exit()
{
}