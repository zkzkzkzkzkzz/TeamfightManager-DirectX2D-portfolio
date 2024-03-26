#include "pch.h"
#include "CScriptMgr.h"

#include "CCursorScript.h"
#include "CEndBtnScript.h"
#include "CGamerScript.h"
#include "CGosuScript.h"
#include "CJoongSooScript.h"
#include "CLobbyBtnScript.h"
#include "CLobbyHdScript.h"
#include "CMgrBtnScript.h"
#include "CNewBtnScript.h"
#include "CProceedBtnScript.h"
#include "CRecruitmentBtnScript.h"
#include "CRecruitmentScript.h"
#include "CRecruitSlotScript.h"
#include "CSquadBtnScript.h"
#include "CTrainningBtnScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCursorScript");
	_vec.push_back(L"CEndBtnScript");
	_vec.push_back(L"CGamerScript");
	_vec.push_back(L"CGosuScript");
	_vec.push_back(L"CJoongSooScript");
	_vec.push_back(L"CLobbyBtnScript");
	_vec.push_back(L"CLobbyHdScript");
	_vec.push_back(L"CMgrBtnScript");
	_vec.push_back(L"CNewBtnScript");
	_vec.push_back(L"CProceedBtnScript");
	_vec.push_back(L"CRecruitmentBtnScript");
	_vec.push_back(L"CRecruitmentScript");
	_vec.push_back(L"CRecruitSlotScript");
	_vec.push_back(L"CSquadBtnScript");
	_vec.push_back(L"CTrainningBtnScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCursorScript" == _strScriptName)
		return new CCursorScript;
	if (L"CEndBtnScript" == _strScriptName)
		return new CEndBtnScript;
	if (L"CGamerScript" == _strScriptName)
		return new CGamerScript;
	if (L"CGosuScript" == _strScriptName)
		return new CGosuScript;
	if (L"CJoongSooScript" == _strScriptName)
		return new CJoongSooScript;
	if (L"CLobbyBtnScript" == _strScriptName)
		return new CLobbyBtnScript;
	if (L"CLobbyHdScript" == _strScriptName)
		return new CLobbyHdScript;
	if (L"CMgrBtnScript" == _strScriptName)
		return new CMgrBtnScript;
	if (L"CNewBtnScript" == _strScriptName)
		return new CNewBtnScript;
	if (L"CProceedBtnScript" == _strScriptName)
		return new CProceedBtnScript;
	if (L"CRecruitmentBtnScript" == _strScriptName)
		return new CRecruitmentBtnScript;
	if (L"CRecruitmentScript" == _strScriptName)
		return new CRecruitmentScript;
	if (L"CRecruitSlotScript" == _strScriptName)
		return new CRecruitSlotScript;
	if (L"CSquadBtnScript" == _strScriptName)
		return new CSquadBtnScript;
	if (L"CTrainningBtnScript" == _strScriptName)
		return new CTrainningBtnScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CURSORSCRIPT:
		return new CCursorScript;
		break;
	case (UINT)SCRIPT_TYPE::ENDBTNSCRIPT:
		return new CEndBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMERSCRIPT:
		return new CGamerScript;
		break;
	case (UINT)SCRIPT_TYPE::GOSUSCRIPT:
		return new CGosuScript;
		break;
	case (UINT)SCRIPT_TYPE::JOONGSOOSCRIPT:
		return new CJoongSooScript;
		break;
	case (UINT)SCRIPT_TYPE::LOBBYBTNSCRIPT:
		return new CLobbyBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::LOBBYHDSCRIPT:
		return new CLobbyHdScript;
		break;
	case (UINT)SCRIPT_TYPE::MGRBTNSCRIPT:
		return new CMgrBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::NEWBTNSCRIPT:
		return new CNewBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::PROCEEDBTNSCRIPT:
		return new CProceedBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::RECRUITMENTBTNSCRIPT:
		return new CRecruitmentBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::RECRUITMENTSCRIPT:
		return new CRecruitmentScript;
		break;
	case (UINT)SCRIPT_TYPE::RECRUITSLOTSCRIPT:
		return new CRecruitSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::SQUADBTNSCRIPT:
		return new CSquadBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::TRAINNINGBTNSCRIPT:
		return new CTrainningBtnScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CURSORSCRIPT:
		return L"CCursorScript";
		break;

	case SCRIPT_TYPE::ENDBTNSCRIPT:
		return L"CEndBtnScript";
		break;

	case SCRIPT_TYPE::GAMERSCRIPT:
		return L"CGamerScript";
		break;

	case SCRIPT_TYPE::GOSUSCRIPT:
		return L"CGosuScript";
		break;

	case SCRIPT_TYPE::JOONGSOOSCRIPT:
		return L"CJoongSooScript";
		break;

	case SCRIPT_TYPE::LOBBYBTNSCRIPT:
		return L"CLobbyBtnScript";
		break;

	case SCRIPT_TYPE::LOBBYHDSCRIPT:
		return L"CLobbyHdScript";
		break;

	case SCRIPT_TYPE::MGRBTNSCRIPT:
		return L"CMgrBtnScript";
		break;

	case SCRIPT_TYPE::NEWBTNSCRIPT:
		return L"CNewBtnScript";
		break;

	case SCRIPT_TYPE::PROCEEDBTNSCRIPT:
		return L"CProceedBtnScript";
		break;

	case SCRIPT_TYPE::RECRUITMENTBTNSCRIPT:
		return L"CRecruitmentBtnScript";
		break;

	case SCRIPT_TYPE::RECRUITMENTSCRIPT:
		return L"CRecruitmentScript";
		break;

	case SCRIPT_TYPE::RECRUITSLOTSCRIPT:
		return L"CRecruitSlotScript";
		break;

	case SCRIPT_TYPE::SQUADBTNSCRIPT:
		return L"CSquadBtnScript";
		break;

	case SCRIPT_TYPE::TRAINNINGBTNSCRIPT:
		return L"CTrainningBtnScript";
		break;

	}
	return nullptr;
}