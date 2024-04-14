#include "pch.h"
#include "CScriptMgr.h"

#include "CNewBtnScript.h"
#include "CEndBtnScript.h"
#include "CLobbyHdScript.h"
#include "CRecruitmentBtnScript.h"
#include "CTrainningBtnScript.h"
#include "CSquadBtnScript.h"
#include "CCursorScript.h"
#include "CProceedBtnScript.h"
#include "CLobbyBtnScript.h"
#include "CMgrBtnScript.h"
#include "CRecruitmentScript.h"
#include "CRecruitSlotScript.h"
#include "CGamerScript.h"
#include "CChampScript.h"
#include "CArcherScript.h"
#include "CArrowScript.h"
#include "CFighterScript.h"
#include "CEffectScript.h"
#include "CKnightScript.h"
#include "CMonkScript.h"
#include "CNinjaScript.h"
#include "CNinjaCloneScript.h"
#include "CPriestScript.h"
#include "CTeamHeaderScript.h"
#include "CBlueTeamSlotScript.h"
#include "CLineUpBGScript.h"
#include "CLineUpTeamScript.h"
#include "CDraftArrowScript.h"
#include "CLineUpSlotScript.h"
#include "CNextBtnScript.h"
#include "CTeamSlotScript.h"
#include "CBanPickUIScript.h"
#include "CTeamSlotIconScript.h"
#include "CBansSlotScript.h"
#include "CChampSlotScript.h"
#include "CMiddleBanScript.h"
#include "CBattleStartBtnScript.h"
#include "CBattleStadiumScript.h"
#include "CRedTeamSlotScript.h"
#include "CBattleResultScript.h"
#include "CBattleEndBtnScript.h"
#include "CCurtainScript.h"
#include "CMCScript.h"
#include "CTextBoxScript.h"
#include "CTextEffectScript.h"
#include "CAvatarScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CNewBtnScript");
	_vec.push_back(L"CEndBtnScript");
	_vec.push_back(L"CLobbyHdScript");
	_vec.push_back(L"CRecruitmentBtnScript");
	_vec.push_back(L"CTrainningBtnScript");
	_vec.push_back(L"CSquadBtnScript");
	_vec.push_back(L"CCursorScript");
	_vec.push_back(L"CProceedBtnScript");
	_vec.push_back(L"CLobbyBtnScript");
	_vec.push_back(L"CMgrBtnScript");
	_vec.push_back(L"CRecruitmentScript");
	_vec.push_back(L"CRecruitSlotScript");
	_vec.push_back(L"CGamerScript");
	_vec.push_back(L"CChampScript");
	_vec.push_back(L"CArcherScript");
	_vec.push_back(L"CArrowScript");
	_vec.push_back(L"CFighterScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CKnightScript");
	_vec.push_back(L"CMonkScript");
	_vec.push_back(L"CNinjaScript");
	_vec.push_back(L"CNinjaCloneScript");
	_vec.push_back(L"CPriestScript");
	_vec.push_back(L"CTeamHeaderScript");
	_vec.push_back(L"CBlueTeamSlotScript");
	_vec.push_back(L"CLineUpBGScript");
	_vec.push_back(L"CLineUpTeamScript");
	_vec.push_back(L"CDraftArrowScript");
	_vec.push_back(L"CLineUpSlotScript");
	_vec.push_back(L"CNextBtnScript");
	_vec.push_back(L"CTeamSlotScript");
	_vec.push_back(L"CBanPickUIScript");
	_vec.push_back(L"CTeamSlotIconScript");
	_vec.push_back(L"CBansSlotScript");
	_vec.push_back(L"CChampSlotScript");
	_vec.push_back(L"CMiddleBanScript");
	_vec.push_back(L"CBattleStartBtnScript");
	_vec.push_back(L"CBattleStadiumScript");
	_vec.push_back(L"CRedTeamSlotScript");
	_vec.push_back(L"CBattleResultScript");
	_vec.push_back(L"CBattleEndBtnScript");
	_vec.push_back(L"CCurtainScript");
	_vec.push_back(L"CMCScript");
	_vec.push_back(L"CTextBoxScript");
	_vec.push_back(L"CTextEffectScript");
	_vec.push_back(L"CAvatarScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CNewBtnScript" == _strScriptName)
		return new CNewBtnScript;
	if (L"CEndBtnScript" == _strScriptName)
		return new CEndBtnScript;
	if (L"CLobbyHdScript" == _strScriptName)
		return new CLobbyHdScript;
	if (L"CRecruitmentBtnScript" == _strScriptName)
		return new CRecruitmentBtnScript;
	if (L"CTrainningBtnScript" == _strScriptName)
		return new CTrainningBtnScript;
	if (L"CSquadBtnScript" == _strScriptName)
		return new CSquadBtnScript;
	if (L"CCursorScript" == _strScriptName)
		return new CCursorScript;
	if (L"CProceedBtnScript" == _strScriptName)
		return new CProceedBtnScript;
	if (L"CLobbyBtnScript" == _strScriptName)
		return new CLobbyBtnScript;
	if (L"CMgrBtnScript" == _strScriptName)
		return new CMgrBtnScript;
	if (L"CRecruitmentScript" == _strScriptName)
		return new CRecruitmentScript;
	if (L"CRecruitSlotScript" == _strScriptName)
		return new CRecruitSlotScript;
	if (L"CGamerScript" == _strScriptName)
		return new CGamerScript;
	if (L"CChampScript" == _strScriptName)
		return new CChampScript;
	if (L"CArcherScript" == _strScriptName)
		return new CArcherScript;
	if (L"CArrowScript" == _strScriptName)
		return new CArrowScript;
	if (L"CFighterScript" == _strScriptName)
		return new CFighterScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CKnightScript" == _strScriptName)
		return new CKnightScript;
	if (L"CMonkScript" == _strScriptName)
		return new CMonkScript;
	if (L"CNinjaScript" == _strScriptName)
		return new CNinjaScript;
	if (L"CNinjaCloneScript" == _strScriptName)
		return new CNinjaCloneScript;
	if (L"CPriestScript" == _strScriptName)
		return new CPriestScript;
	if (L"CTeamHeaderScript" == _strScriptName)
		return new CTeamHeaderScript;
	if (L"CBlueTeamSlotScript" == _strScriptName)
		return new CBlueTeamSlotScript;
	if (L"CLineUpBGScript" == _strScriptName)
		return new CLineUpBGScript;
	if (L"CLineUpTeamScript" == _strScriptName)
		return new CLineUpTeamScript;
	if (L"CDraftArrowScript" == _strScriptName)
		return new CDraftArrowScript;
	if (L"CLineUpSlotScript" == _strScriptName)
		return new CLineUpSlotScript;
	if (L"CNextBtnScript" == _strScriptName)
		return new CNextBtnScript;
	if (L"CTeamSlotScript" == _strScriptName)
		return new CTeamSlotScript;
	if (L"CBanPickUIScript" == _strScriptName)
		return new CBanPickUIScript;
	if (L"CTeamSlotIconScript" == _strScriptName)
		return new CTeamSlotIconScript;
	if (L"CBansSlotScript" == _strScriptName)
		return new CBansSlotScript;
	if (L"CChampSlotScript" == _strScriptName)
		return new CChampSlotScript;
	if (L"CMiddleBanScript" == _strScriptName)
		return new CMiddleBanScript;
	if (L"CBattleStartBtnScript" == _strScriptName)
		return new CBattleStartBtnScript;
	if (L"CBattleStadiumScript" == _strScriptName)
		return new CBattleStadiumScript;
	if (L"CRedTeamSlotScript" == _strScriptName)
		return new CRedTeamSlotScript;
	if (L"CBattleResultScript" == _strScriptName)
		return new CBattleResultScript;
	if (L"CBattleEndBtnScript" == _strScriptName)
		return new CBattleEndBtnScript;
	if (L"CCurtainScript" == _strScriptName)
		return new CCurtainScript;
	if (L"CMCScript" == _strScriptName)
		return new CMCScript;
	if (L"CTextBoxScript" == _strScriptName)
		return new CTextBoxScript;
	if (L"CTextEffectScript" == _strScriptName)
		return new CTextEffectScript;
	if (L"CAvatarScript" == _strScriptName)
		return new CAvatarScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::NEWBTNSCRIPT:
		return new CNewBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::ENDBTNSCRIPT:
		return new CEndBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::LOBBYHDSCRIPT:
		return new CLobbyHdScript;
		break;
	case (UINT)SCRIPT_TYPE::RECRUITMENTBTNSCRIPT:
		return new CRecruitmentBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::TRAINNINGBTNSCRIPT:
		return new CTrainningBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::SQUADBTNSCRIPT:
		return new CSquadBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::CURSORSCRIPT:
		return new CCursorScript;
		break;
	case (UINT)SCRIPT_TYPE::PROCEEDBTNSCRIPT:
		return new CProceedBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::LOBBYBTNSCRIPT:
		return new CLobbyBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::MGRBTNSCRIPT:
		return new CMgrBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::RECRUITMENTSCRIPT:
		return new CRecruitmentScript;
		break;
	case (UINT)SCRIPT_TYPE::RECRUITSLOTSCRIPT:
		return new CRecruitSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMERSCRIPT:
		return new CGamerScript;
		break;
	case (UINT)SCRIPT_TYPE::CHAMPSCRIPT:
		return new CChampScript;
		break;
	case (UINT)SCRIPT_TYPE::ARCHERSCRIPT:
		return new CArcherScript;
		break;
	case (UINT)SCRIPT_TYPE::ARROWSCRIPT:
		return new CArrowScript;
		break;
	case (UINT)SCRIPT_TYPE::FIGHTERSCRIPT:
		return new CFighterScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::KNIGHTSCRIPT:
		return new CKnightScript;
		break;
	case (UINT)SCRIPT_TYPE::MONKSCRIPT:
		return new CMonkScript;
		break;
	case (UINT)SCRIPT_TYPE::NINJASCRIPT:
		return new CNinjaScript;
		break;
	case (UINT)SCRIPT_TYPE::NINJACLONESCRIPT:
		return new CNinjaCloneScript;
		break;
	case (UINT)SCRIPT_TYPE::PRIESTSCRIPT:
		return new CPriestScript;
		break;
	case (UINT)SCRIPT_TYPE::TEAMHEADERSCRIPT:
		return new CTeamHeaderScript;
		break;
	case (UINT)SCRIPT_TYPE::BLUETEAMSLOTSCRIPT:
		return new CBlueTeamSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::LINEUPBGSCRIPT:
		return new CLineUpBGScript;
		break;
	case (UINT)SCRIPT_TYPE::LINEUPTEAMSCRIPT:
		return new CLineUpTeamScript;
		break;
	case (UINT)SCRIPT_TYPE::DRAFTARROWSCRIPT:
		return new CDraftArrowScript;
		break;
	case (UINT)SCRIPT_TYPE::LINEUPSLOTSCRIPT:
		return new CLineUpSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::NEXTBTNSCRIPT:
		return new CNextBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::TEAMSLOTSCRIPT:
		return new CTeamSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::BANPICKUISCRIPT:
		return new CBanPickUIScript;
		break;
	case (UINT)SCRIPT_TYPE::TEAMSLOTICONSCRIPT:
		return new CTeamSlotIconScript;
		break;
	case (UINT)SCRIPT_TYPE::BANSSLOTSCRIPT:
		return new CBansSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::CHAMPSLOTSCRIPT:
		return new CChampSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::MIDDLEBANSCRIPT:
		return new CMiddleBanScript;
		break;
	case (UINT)SCRIPT_TYPE::BATTLESTARTBTNSCRIPT:
		return new CBattleStartBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::BATTLESTADIUMSCRIPT:
		return new CBattleStadiumScript;
		break;
	case (UINT)SCRIPT_TYPE::REDTEAMSLOTSCRIPT:
		return new CRedTeamSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::BATTLERESULTSCRIPT:
		return new CBattleResultScript;
		break;
	case (UINT)SCRIPT_TYPE::BATTLEENDBTNSCRIPT:
		return new CBattleEndBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::CURTAINSCRIPT:
		return new CCurtainScript;
		break;
	case (UINT)SCRIPT_TYPE::MCSCRIPT:
		return new CMCScript;
		break;
	case (UINT)SCRIPT_TYPE::TEXTBOXSCRIPT:
		return new CTextBoxScript;
		break;
	case (UINT)SCRIPT_TYPE::TEXTEFFECTSCRIPT:
		return new CTextEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::AVATARSCRIPT:
		return new CAvatarScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::NEWBTNSCRIPT:
		return L"CNewBtnScript";
		break;

	case SCRIPT_TYPE::ENDBTNSCRIPT:
		return L"CEndBtnScript";
		break;

	case SCRIPT_TYPE::LOBBYHDSCRIPT:
		return L"CLobbyHdScript";
		break;

	case SCRIPT_TYPE::RECRUITMENTBTNSCRIPT:
		return L"CRecruitmentBtnScript";
		break;

	case SCRIPT_TYPE::TRAINNINGBTNSCRIPT:
		return L"CTrainningBtnScript";
		break;

	case SCRIPT_TYPE::SQUADBTNSCRIPT:
		return L"CSquadBtnScript";
		break;

	case SCRIPT_TYPE::CURSORSCRIPT:
		return L"CCursorScript";
		break;

	case SCRIPT_TYPE::PROCEEDBTNSCRIPT:
		return L"CProceedBtnScript";
		break;

	case SCRIPT_TYPE::LOBBYBTNSCRIPT:
		return L"CLobbyBtnScript";
		break;

	case SCRIPT_TYPE::MGRBTNSCRIPT:
		return L"CMgrBtnScript";
		break;

	case SCRIPT_TYPE::RECRUITMENTSCRIPT:
		return L"CRecruitmentScript";
		break;

	case SCRIPT_TYPE::RECRUITSLOTSCRIPT:
		return L"CRecruitSlotScript";
		break;

	case SCRIPT_TYPE::GAMERSCRIPT:
		return L"CGamerScript";
		break;

	case SCRIPT_TYPE::CHAMPSCRIPT:
		return L"CChampScript";
		break;

	case SCRIPT_TYPE::ARCHERSCRIPT:
		return L"CArcherScript";
		break;

	case SCRIPT_TYPE::ARROWSCRIPT:
		return L"CArrowScript";
		break;

	case SCRIPT_TYPE::FIGHTERSCRIPT:
		return L"CFighterScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::KNIGHTSCRIPT:
		return L"CKnightScript";
		break;

	case SCRIPT_TYPE::MONKSCRIPT:
		return L"CMonkScript";
		break;

	case SCRIPT_TYPE::NINJASCRIPT:
		return L"CNinjaScript";
		break;

	case SCRIPT_TYPE::NINJACLONESCRIPT:
		return L"CNinjaCloneScript";
		break;

	case SCRIPT_TYPE::PRIESTSCRIPT:
		return L"CPriestScript";
		break;

	case SCRIPT_TYPE::TEAMHEADERSCRIPT:
		return L"CTeamHeaderScript";
		break;

	case SCRIPT_TYPE::BLUETEAMSLOTSCRIPT:
		return L"CBlueTeamSlotScript";
		break;

	case SCRIPT_TYPE::LINEUPBGSCRIPT:
		return L"CLineUpBGScript";
		break;

	case SCRIPT_TYPE::LINEUPTEAMSCRIPT:
		return L"CLineUpTeamScript";
		break;

	case SCRIPT_TYPE::DRAFTARROWSCRIPT:
		return L"CDraftArrowScript";
		break;

	case SCRIPT_TYPE::LINEUPSLOTSCRIPT:
		return L"CLineUpSlotScript";
		break;

	case SCRIPT_TYPE::NEXTBTNSCRIPT:
		return L"CNextBtnScript";
		break;

	case SCRIPT_TYPE::TEAMSLOTSCRIPT:
		return L"CTeamSlotScript";
		break;

	case SCRIPT_TYPE::BANPICKUISCRIPT:
		return L"CBanPickUIScript";
		break;

	case SCRIPT_TYPE::TEAMSLOTICONSCRIPT:
		return L"CTeamSlotIconScript";
		break;

	case SCRIPT_TYPE::BANSSLOTSCRIPT:
		return L"CBansSlotScript";
		break;

	case SCRIPT_TYPE::CHAMPSLOTSCRIPT:
		return L"CChampSlotScript";
		break;

	case SCRIPT_TYPE::MIDDLEBANSCRIPT:
		return L"CMiddleBanScript";
		break;

	case SCRIPT_TYPE::BATTLESTARTBTNSCRIPT:
		return L"CBattleStartBtnScript";
		break;

	case SCRIPT_TYPE::BATTLESTADIUMSCRIPT:
		return L"CBattleStadiumScript";
		break;

	case SCRIPT_TYPE::REDTEAMSLOTSCRIPT:
		return L"CRedTeamSlotScript";
		break;

	case SCRIPT_TYPE::BATTLERESULTSCRIPT:
		return L"CBattleResultScript";
		break;

	case SCRIPT_TYPE::BATTLEENDBTNSCRIPT:
		return L"CBattleEndBtnScript";
		break;

	case SCRIPT_TYPE::CURTAINSCRIPT:
		return L"CCurtainScript";
		break;

	case SCRIPT_TYPE::MCSCRIPT:
		return L"CMCScript";
		break;

	case SCRIPT_TYPE::TEXTBOXSCRIPT:
		return L"CTextBoxScript";
		break;

	case SCRIPT_TYPE::TEXTEFFECTSCRIPT:
		return L"CTextEffectScript";
		break;

	case SCRIPT_TYPE::AVATARSCRIPT:
		return L"CAvatarScript";
		break;

	}
	return nullptr;
}