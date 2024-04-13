#pragma once
#include <Engine\CScript.h>

#include "CTextBoxScript.h"

class CMCScript :
    public CScript
{
private:
    TEXT_INDEX m_idx;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CMCScript);
    CMCScript();
    CMCScript(const CMCScript& _Origin);
    ~CMCScript();
};

