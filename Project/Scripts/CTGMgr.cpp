#include "pch.h"
#include "CTGMgr.h"


int CTGMgr::G_Year = 2021;
int CTGMgr::G_Month = 1;
int CTGMgr::G_Week = 1;

int CTGMgr::G_Coin = 200;

map<wstring, CGameObject*> CTGMgr::G_Gamer;			// 선수 목록
map<wstring, CGameObject*> CTGMgr::G_RecruitList;	// 영입 가능 선수 목록

map<wstring, CGameObject*> CTGMgr::G_Champ;	// 챔피언 목록

float CTGMgr::G_Time = 60.f;



CTGMgr::CTGMgr()
{
}

CTGMgr::~CTGMgr()
{
}

void CTGMgr::ResetInGameTime()
{
	G_Time = 60.f;
}
