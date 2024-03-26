#pragma once

class CGameObject;
class CGamer;

class TGMgr
{
public:
	static int G_Year;
	static int G_Month;
	static int G_Week;

	static int G_Coin;

	static map<wstring, CGameObject*> G_Gamer;			// 선수 목록
	static map<wstring, CGameObject*> G_RecruitList;	// 영입 가능 선수 목록
	
	static float G_Time;	// 경기 시간

public:
	static void GamerInit();

};

