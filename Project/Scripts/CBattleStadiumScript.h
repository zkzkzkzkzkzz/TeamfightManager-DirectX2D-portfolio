#pragma once
#include <Engine\CScript.h>

#include "CBanpickLevel.h"

class CGameObject;

class CBattleStadiumScript :
    public CScript
{
private:
    CBanpickLevel* m_Level;
    CGameObject* m_BTMgr;

    float m_BattleStartTime;
    bool m_BlueSpawn;
    bool m_RedSpawn;

    bool m_bUIPos;
    float m_UIPosTime;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void OpenStadium();
    void CloseStadium();

    void CreateBlueTeamChamp();
    void CreateRedTeamChamp();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CBattleStadiumScript);
    CBattleStadiumScript();
    CBattleStadiumScript(const CBattleStadiumScript& _Origin);
    ~CBattleStadiumScript();
};

