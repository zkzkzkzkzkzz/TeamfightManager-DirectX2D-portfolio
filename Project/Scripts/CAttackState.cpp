#include "pch.h"
#include "CAttackState.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>
#include <Engine\CFSM.h>

#include "CChampScript.h"


CAttackState::CAttackState()
{
}

CAttackState::~CAttackState()
{
}


void CAttackState::finaltick()
{
}

void CAttackState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	pSelf->GetScript<CChampScript>()->EnterAttackState();
}

void CAttackState::Exit()
{
}