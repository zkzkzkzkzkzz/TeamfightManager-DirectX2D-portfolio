#include "pch.h"
#include "CScript.h"



CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}



void CScript::Instantiate(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _LayerIdx)
{
	if (nullptr == _Prefab)
		return;

	CGameObject* pNewObj = _Prefab->Instatiate();
	pNewObj->Transform()->SetRelativePos(_vWorldPos);
	GamePlayStatic::SpawnGameObject(pNewObj, _LayerIdx);
}

void CScript::SaveToFile(FILE* _File)
{
}

void CScript::LoadFromFile(FILE* _File)
{
}
