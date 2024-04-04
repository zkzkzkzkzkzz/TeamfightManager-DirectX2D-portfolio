#include "pch.h"
#include "CLineUpBGScript.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>

#include "CLineUpSlotScript.h"

CLineUpBGScript::CLineUpBGScript()
	: CScript(LINEUPBGSCRIPT)
{
}

CLineUpBGScript::CLineUpBGScript(const CLineUpBGScript& _Other)
	: CScript(LINEUPBGSCRIPT)
{
}

CLineUpBGScript::~CLineUpBGScript()
{
}


void CLineUpBGScript::begin()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CLineUpBGScript::tick()
{
	if (CheckSlotSelect())
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	else
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}


bool CLineUpBGScript::CheckSlotSelect()
{
	for (size_t i = 0; i < CTGMgr::GetInst()->G_ParticipatingSlot.size(); ++i)
	{
		if (CTGMgr::GetInst()->G_ParticipatingSlot[i]->GetScript<CLineUpSlotScript>()->IsSelect())
		{
			return true;
		}
	}

	for (size_t i = 0; i < CTGMgr::GetInst()->G_SubstituesSlot.size(); ++i)
	{
		if (CTGMgr::GetInst()->G_SubstituesSlot[i]->GetScript<CLineUpSlotScript>()->IsSelect())
		{
			return true;
		}
	}

	return false;
}