#include "pch.h"
#include "CDraftArrowScript.h"

#include <Engine\CAssetMgr.h>

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
}
