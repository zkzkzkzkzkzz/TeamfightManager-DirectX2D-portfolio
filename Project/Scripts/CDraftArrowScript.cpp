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
	for (size_t i = 0; i < CTGMgr::GetInst()->G_ShortlistSlot.size(); ++i)
	{
		if (CTGMgr::GetInst()->G_ShortlistSlot[i]->GetScript<CLineUpSlotScript>()->IsSelect())
		{
			return true;
		}
	}

	return false;
}
