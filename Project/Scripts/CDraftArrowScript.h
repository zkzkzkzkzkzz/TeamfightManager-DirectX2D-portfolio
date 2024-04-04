#pragma once
#include <Engine\CScript.h>


class CDraftArrowScript :
    public CScript
{
private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CDraftArrowScript);
    CDraftArrowScript();
    CDraftArrowScript(const CDraftArrowScript& _Other);
    ~CDraftArrowScript();
};

