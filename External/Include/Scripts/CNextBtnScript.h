#pragma once
#include <Engine\CScript.h>


class CNextBtnScript :
    public CScript
{
private:
    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void OnHovered();
    void OnUnHovered();
    void LBtnClicked();

    void CheckMousePos();

public:
    CLONE(CNextBtnScript);
    CNextBtnScript();
    CNextBtnScript(const CNextBtnScript& _Other);
    ~CNextBtnScript();
};

