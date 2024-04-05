#include "pch.h"
#include "CTGMgr.h"

#include <Engine\CGameObject.h>

int CTGMgr::G_Year = 2021;
int CTGMgr::G_Month = 1;
int CTGMgr::G_Week = 1;

int CTGMgr::G_Coin = 200;

int CTGMgr::G_FirstSlot = -1;
int CTGMgr::G_SecondSlot = -1;

vector<CGameObject*> CTGMgr::G_ShortlistSlot;

vector<CGameObject*> CTGMgr::G_ParticipatingPlayer;
vector<CGameObject*> CTGMgr::G_PlayableGamer;

map<wstring, CGameObject*> CTGMgr::G_Gamer;			// 선수 목록
map<wstring, CGameObject*> CTGMgr::G_RecruitList;	// 영입 가능 선수 목록
map<wstring, CGameObject*> CTGMgr::G_Champ;	// 챔피언 목록

int CTGMgr::G_Time = 60;

int CTGMgr::G_BlueKillScore = 0;
int CTGMgr::G_RedKillScore = 0;
int CTGMgr::G_LevelChange = 0;

CTGMgr::CTGMgr()
{
}

CTGMgr::~CTGMgr()
{
	if (0 == G_LevelChange)
	{
		//Delete_Map(G_Gamer);

		//G_RecruitList.clear();
		//G_ParticipatingPlayer.clear();
		//G_PlayableGamer.clear();
	}
	else
	{
		Delete_Map(G_Gamer);
		Delete_Map(G_RecruitList);
		Delete_Vec(G_ParticipatingPlayer);
		Delete_Vec(G_PlayableGamer);
	}

	Delete_Vec(G_ShortlistSlot);
	Delete_Map(G_Champ);
}

void CTGMgr::CloneTGMgr()
{
	for (size_t i = 0; i < G_ShortlistSlot.size(); ++i)
	{
		G_ShortlistSlot[i] = G_ShortlistSlot[i]->Clone();
	}

	for (size_t i = 0; i < G_ParticipatingPlayer.size(); ++i)
	{
		G_ParticipatingPlayer[i] = G_ParticipatingPlayer[i]->Clone();
	}

	for (size_t i = 0; i < G_PlayableGamer.size(); ++i)
	{
		G_PlayableGamer[i] = G_PlayableGamer[i]->Clone();
	}

	for (const auto& pair : G_Gamer)
	{
		G_Gamer[pair.first] = new CGameObject(*pair.second);
	}

	for (const auto& pair : G_RecruitList)
	{
		G_RecruitList[pair.first] = new CGameObject(*pair.second);
	}

	for (const auto& pair : G_Champ)
	{
		G_Champ[pair.first] = new CGameObject(*pair.second);
	}

	++G_LevelChange;
}

void CTGMgr::ResetInGameTime()
{
	G_Time = 60;
}
