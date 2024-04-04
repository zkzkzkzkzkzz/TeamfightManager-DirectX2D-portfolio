#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CLineUpSlotScript :
    public CScript
{
private:
    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    CGameObject* m_Gamer;
    bool    m_SelectSlot;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetGamerToSlot(CGameObject* _Gamer) { m_Gamer = _Gamer; }
    CGameObject* GetGamerFromSlot() { return m_Gamer; }

    void SetSlotInfo();
    bool IsSelect() { return m_SelectSlot; }

    void CheckMousePos();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void OnHovered();
    void OnUnHovered();
    void LBtnClicked();

public:
    CLONE(CLineUpSlotScript);
    CLineUpSlotScript();
    CLineUpSlotScript(const CLineUpSlotScript& _Other);
    ~CLineUpSlotScript();
};

