#pragma once
#include <Engine\singleton.h>

class CTGMgr :
    public CSingleton<CTGMgr>
{
    SINGLE(CTGMgr);

public:
	static int G_Year;
	static int G_Month;
	static int G_Week;

	static int G_Coin;

	static map<wstring, CGameObject*> G_Gamer;			// 선수 목록
	static map<wstring, CGameObject*> G_RecruitList;	// 영입 가능 선수 목록

	static map<wstring, CGameObject*> G_Champ;	// 챔피언 목록

	static list<CGameObject*> G_TeamBlue;
	static list<CGameObject*> G_TeamRed;

	static int G_Time;	// 경기 시간

	static int G_BlueKillScore;
	static int G_RedKillScore;

public:
	static void ResetInGameTime();
};

