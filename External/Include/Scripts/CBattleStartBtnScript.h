#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CBattleStartBtnScript :
    public CScript
{
private:
    CGameObject*    m_Text;

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
    CLONE(CBattleStartBtnScript);
    CBattleStartBtnScript();
    CBattleStartBtnScript(const CBattleStartBtnScript& _Origin);
    ~CBattleStartBtnScript();
};

