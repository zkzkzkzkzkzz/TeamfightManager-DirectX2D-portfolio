#include "pch.h"
#include "TGMgr.h"

#include "CGamer.h"
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CMeshRender.h>

int TGMgr::G_Year = 2021;
int TGMgr::G_Month = 1;
int TGMgr::G_Week = 1;

int TGMgr::G_Coin = 200;

map<wstring, CGameObject*> TGMgr::G_Gamer;			// 선수 목록
map<wstring, CGameObject*> TGMgr::G_RecruitList;	// 영입 가능 선수 목록

float TGMgr::G_Time = 60.f;

void TGMgr::GamerInit()
{

}
