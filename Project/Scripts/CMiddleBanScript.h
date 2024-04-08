#pragma once
#include <Engine\CScript.h>
#include "CBanpickLevel.h"

class CGameObject;

class CMiddleBanScript :
    public CScript
{
private:
    CGameObject* m_Arrow;
    CGameObject* m_Text;

    CBanpickLevel* m_Level;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void InitArrow();
    void InitText();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CMiddleBanScript);
    CMiddleBanScript();
    CMiddleBanScript(const CMiddleBanScript& _Origin);
    ~CMiddleBanScript();
};

