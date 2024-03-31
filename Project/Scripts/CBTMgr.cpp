#include "pch.h"
#include "CBTMgr.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include "CChampScript.h"
#include "CEffectScript.h"

list<CGameObject*> CBTMgr::G_Respawn;

CBTMgr::CBTMgr()
{
}

CBTMgr::~CBTMgr()
{
}

void CBTMgr::tick()
{
    CTGMgr::G_Time -= DT;

    list<CGameObject*>::iterator iter = G_Respawn.begin();

    while (iter != G_Respawn.end())
    {
        float* rtime = &(GETCHAMP((*iter))->m_InGameStatus.RespawnTime);
        *rtime += DT;

        if (*rtime >= RESPAWNTIME)
        {
            SpawnChamp((*iter));
            GETCHAMP((*iter))->InitChampInfo();
            iter = G_Respawn.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void CBTMgr::RegistRespawnPool(CGameObject* _Obj)
{
    TEAM team = GETCHAMP(_Obj)->GetTeamColor();

    Vec3 vSpawnPos = Vec3(0.f, 0.f, 300.f);

    if (TEAM::BLUE == team)
        vSpawnPos = Vec3(-290.f, 0.f, 300.f);
    else
        vSpawnPos = Vec3(290.f, 0.f, 300.f);

    vSpawnPos.x += (float)(rand() % 50);
    vSpawnPos.y += (float)(rand() % 40);

    _Obj->Transform()->SetRelativePos(vSpawnPos);

    GETCHAMP(_Obj)->m_InGameStatus.RespawnTime = 0.f;

    G_Respawn.push_back(_Obj);
}

void CBTMgr::SpawnChamp(CGameObject* _Obj)
{
    GETCHAMP(_Obj)->RespawnInfo();

    CGameObject* effect = new CGameObject;
    effect->AddComponent(new CTransform);
    effect->AddComponent(new CMeshRender);
    effect->AddComponent(new CAnimator2D);
    effect->AddComponent(new CEffectScript);
    GETEFFECT(effect)->SetEffectInfo(_Obj->Transform()->GetRelativePos(), Vec3(64.f, 64.f, 64.f)
                                    , _Obj->Transform()->GetRelativeRotation(), L"SpawnEffect", 0.55f);
    GamePlayStatic::SpawnGameObject(effect, 6);


    _Obj->SetActive(true);

    GETCHAMP(_Obj)->SetChampState(CHAMP_STATE::IDLE);
}
