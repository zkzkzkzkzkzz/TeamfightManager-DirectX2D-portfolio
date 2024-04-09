#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CTeamSlotScript :
    public CScript
{
private:
    CGameObject*    m_Gamer;
    TEAM            m_Team;

    bool m_bUIPos;
    float m_UIPosTime;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetGamerToSlot(CGameObject* _Gamer) { m_Gamer = _Gamer; }
    CGameObject* GetGamerFromSlot() { return m_Gamer; }

    void SetBlueSlotPos();
    void SetRedSlotPos();
    void SetSlotInfo();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CTeamSlotScript);
    CTeamSlotScript();
    CTeamSlotScript(const CTeamSlotScript& _Origin);
    ~CTeamSlotScript();
};

