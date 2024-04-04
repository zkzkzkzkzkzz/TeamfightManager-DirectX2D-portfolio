#pragma once
#include <Engine\CScript.h>


class CLineUpBGScript :
    public CScript
{
private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CLineUpBGScript);
    CLineUpBGScript();
    CLineUpBGScript(const CLineUpBGScript& _Other);
    ~CLineUpBGScript();
};

