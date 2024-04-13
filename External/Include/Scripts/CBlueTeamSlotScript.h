#pragma once
#include <Engine\CScript.h>

#include "CBanpickLevel.h"

class CGameObject;

class CBlueTeamSlotScript :
    public CScript
{
private:
    Ptr<CTexture>   m_SlotTex;

    CGameObject*    m_KDAText;
    CGameObject*    m_InfoText;

    CGameObject*    m_CurGamer; // 현재 슬롯에 배치된 선수

    CHAMP_LIST      m_CurList;
    CGameObject*    m_CurChamp;

    CBanpickLevel* m_Level;

    CGameObject*    m_KillText;
    CGameObject*    m_DeadText;
    CGameObject*    m_DealText;
    CGameObject*    m_DamagedText;
    CGameObject*    m_HealText;
    CGameObject*    m_AssistText;
    int Kill;
    int Dead;
    int Deal;
    int Damaged;
    int Heal;

    bool m_bUIPos;
    float m_UIPosTime;


    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void OpenSlot();
    void CheckInGameInfo();
    void InitInGameInfo();
    void RenderInGameInfo();
    void FindChampFromList();

    void SetGamer(CGameObject* _Gamer) { m_CurGamer = _Gamer; }

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

