#include "pch.h"
#include "CDraftArrowScript.h"

#include <Engine\CAssetMgr.h>

#include "CLineUpSlotScript.h"

CDraftArrowScript::CDraftArrowScript()
	: CScript(DRAFTARROWSCRIPT)
{
}

CDraftArrowScript::CDraftArrowScript(const CDraftArrowScript& _Other)
	: CScript(DRAFTARROWSCRIPT)
{
}

CDraftArrowScript::~CDraftArrowScript()
{
}

void CDraftArrowScript::begin()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

void CDraftArrowScript::tick()
{
	if (CheckSlotSelect())
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	else
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}

bool CDraftArrowScript::CheckSlotSelect()
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
