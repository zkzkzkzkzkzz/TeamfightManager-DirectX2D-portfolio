#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CBlueTeamSlotScript :
    public CScript
{
private:
    Ptr<CTexture>   m_SlotTex;

    CGameObject*    m_SubSlot;
    Ptr<CTexture>   m_SubSlotTex;

    CGameObject*    m_KDAText;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:


public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CBlueTeamSlotScript);
    CBlueTeamSlotScript();
    CBlueTeamSlotScript(const CBlueTeamSlotScript& _Origin);
    ~CBlueTeamSlotScript();
};

