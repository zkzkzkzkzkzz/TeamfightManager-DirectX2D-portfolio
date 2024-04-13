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

	static int G_FirstSlot;
	static int G_SecondSlot;

	static vector<CGameObject*> G_ShortlistSlot;
	static vector<CGameObject*> G_ParticipatingPlayer;
	static vector<CGameObject*>	G_PlayableGamer;
	static vector<CGameObject*> G_TeamGorilla;

	static vector<CGameObject*>	G_ChampSlot;

	static map<wstring, CGameObject*> G_Gamer;			// 선수 목록
	static map<wstring, CGameObject*> G_RecruitList;	// 영입 가능 선수 목록

	static int G_Time;		// 현재 경기 시간

	static int G_BlueKillScore;
	static int G_RedKillScore;

public:
	static void CloneTGMgr();
	static void ResetInGameTime();
};
