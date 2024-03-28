#include "pch.h"
#include "CSkillState.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CStateMachine.h>
#include <Engine\CTransform.h>
#include <Engine\CFSM.h>

#include "CChampScript.h"


CSkillState::CSkillState()
{
}

CSkillState::~CSkillState()
{
}


void CSkillState::finaltick()
{
}

void CSkillState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	pSelf->GetScript<CChampScript>()->EnterSkillState();
}

void CSkillState::Exit()
{
}
