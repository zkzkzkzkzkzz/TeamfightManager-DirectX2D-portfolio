#include "pch.h"
#include "CBansSlotScript.h"

#include <Engine\CGameObject.h>

CBansSlotScript::CBansSlotScript()
	: CScript(BANSSLOTSCRIPT)
	, m_BanChamp(nullptr)
{
}

CBansSlotScript::CBansSlotScript(const CBansSlotScript& _Origin)
	: CScript(BANSSLOTSCRIPT)
	, m_BanChamp(nullptr)
{
}

CBansSlotScript::~CBansSlotScript()
{
}

void CBansSlotScript::begin()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);
}

void CBansSlotScript::tick()
{
}
