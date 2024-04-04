#include "pch.h"
#include "CLineUpBGScript.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>

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
	if (KEY_TAP(L))
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	}
}

