#pragma once
#include <Engine\CScript.h>


class CArrowScript :
    public CScript
{
private:
    Ptr<CTexture>   m_Tex;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void render();

public:
    CLONE(CArrowScript);
    CArrowScript();
    CArrowScript(const CArrowScript& _Origin);
    ~CArrowScript();
};

