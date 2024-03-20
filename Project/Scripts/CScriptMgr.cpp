#include "pch.h"
#include "CScriptMgr.h"

#include "CEndBtnScript.h"
#include "CNewBtnScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CEndBtnScript");
	_vec.push_back(L"CNewBtnScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CEndBtnScript" == _strScriptName)
		return new CEndBtnScript;
	if (L"CNewBtnScript" == _strScriptName)
		return new CNewBtnScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ENDBTNSCRIPT:
		return new CEndBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::NEWBTNSCRIPT:
		return new CNewBtnScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ENDBTNSCRIPT:
		return L"CEndBtnScript";
		break;

	case SCRIPT_TYPE::NEWBTNSCRIPT:
		return L"CNewBtnScript";
		break;

	}
	return nullptr;
}