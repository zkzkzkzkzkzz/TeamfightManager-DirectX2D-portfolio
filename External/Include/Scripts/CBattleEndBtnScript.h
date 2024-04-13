#pragma once
#include <Engine\CScript.h>

#include "CBanpickLevel.h"

class CBattleEndBtnScript :
    public CScript
{
private:
    CBanpickLevel* m_Level;

    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void CheckMousePos();

    void OnHovered();
    void OnUnHovered();
    void LBtnClicked();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CBattleEndBtnScript);
    CBattleEndBtnScript();
    CBattleEndBtnScript(const CBattleEndBtnScript& _Origin);
    ~CBattleEndBtnScript();
};

