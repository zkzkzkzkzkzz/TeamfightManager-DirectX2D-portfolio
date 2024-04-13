#pragma once
#include <Engine\CScript.h>


class CLineUpTeamScript :
    public CScript
{
private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CLineUpTeamScript);
    CLineUpTeamScript();
    CLineUpTeamScript(const CLineUpTeamScript& _Other);
    ~CLineUpTeamScript();
};

