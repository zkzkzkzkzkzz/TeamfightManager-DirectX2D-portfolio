#pragma once
#include <Engine\singleton.h>

class CGameObject;

class CBTMgr :
    public CSingleton<CBTMgr>
{
    SINGLE(CBTMgr);

private:
    static list<CGameObject*> G_Respawn;
    static float m_CurTime;

public:
    static void tick();

public:
    static void RegistRespawnPool(CGameObject* _Obj);
    static void SpawnChamp(CGameObject* _Obj);
};

