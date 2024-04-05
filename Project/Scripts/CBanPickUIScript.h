#pragma once
#include <Engine\CScript.h>


class CBanPickUIScript :
    public CScript
{
private:
    bool m_bUIPos;
    float m_UIPosTime;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetSlotPos();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CBanPickUIScript);
    CBanPickUIScript();
    CBanPickUIScript(const CBanPickUIScript& _Origin);
    ~CBanPickUIScript();
};

