#pragma once
#include <Engine\CScript.h>

#include "CTextBoxScript.h"

class CTextEffectScript :
    public CScript
{
private:
    TEXT_INDEX m_Idx;

    float m_PrevScaleX;
    float m_CurScaleX;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void DecreaseScale();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CTextEffectScript);
    CTextEffectScript();
    CTextEffectScript(const CTextEffectScript& _Origin);
    ~CTextEffectScript();
};

