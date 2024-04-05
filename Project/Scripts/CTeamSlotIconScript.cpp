#include "pch.h"
#include "CTeamSlotIconScript.h"


CTeamSlotIconScript::CTeamSlotIconScript()
	: CScript(TEAMSLOTICONSCRIPT)
{
}

CTeamSlotIconScript::CTeamSlotIconScript(const CTeamSlotIconScript& _Origin)
	: CScript(TEAMSLOTICONSCRIPT)
{
}

CTeamSlotIconScript::~CTeamSlotIconScript()
{
}

void CTeamSlotIconScript::begin()
{
	if (L"AtkIcon" == GetOwner()->GetName())
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	else if (L"DefIcon" == GetOwner()->GetName())
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CTeamSlotIconScript::tick()
{
}
