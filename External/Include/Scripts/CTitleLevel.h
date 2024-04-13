#pragma once
#include <Engine\CLevel.h>


class CTitleLevel :
    public CLevel
{
private:
    bool m_bBGM;

public:
    void InitUI();

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;

public:
    CLONE(CTitleLevel);
    CTitleLevel();
    CTitleLevel(const CTitleLevel& _Origin);
    ~CTitleLevel();
};

