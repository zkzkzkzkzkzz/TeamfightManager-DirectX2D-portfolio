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
	int HP = *((int*)GetBlackboardData(L"HP"));
	if (HP <= 0)
	{
		ChangeState(L"Dead");
		return;
	}

}

void CSkillState::Enter()
{
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();
	pSelf->GetScript<CChampScript>()->SetChampState(CHAMP_STATE::SKILL);
}

void CSkillState::Exit()
{
}
