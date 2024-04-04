#include "pch.h"
#include "CLineUpTeamScript.h"

#include <Engine\CAssetMgr.h>

CLineUpTeamScript::CLineUpTeamScript()
	: CScript(LINEUPTEAMSCRIPT)
{
}

CLineUpTeamScript::CLineUpTeamScript(const CLineUpTeamScript& _Other)
	: CScript(LINEUPTEAMSCRIPT)
{
}

CLineUpTeamScript::~CLineUpTeamScript()
{
}

void CLineUpTeamScript::begin()
{
	wstring name = GetOwner()->GetName();

	if (L"BlueLineUp" == name)
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	else if (L"RedLineUp" == name)
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	else if (L"SubTitle" == name)
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);
}

void CLineUpTeamScript::tick()
{
}