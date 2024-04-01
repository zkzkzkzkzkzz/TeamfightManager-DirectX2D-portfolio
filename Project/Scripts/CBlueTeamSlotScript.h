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
    CGameObject*    m_InfoText;

    CGameObject*    m_CurGamer; // 현재 슬롯에 배치된 선수

    int Kill;
    int Dead;
    int Deal;
    int Damaged;
    int Heal;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void CheckInGameInfo();

    void SetGamer(CGameObject* _Gamer);

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

