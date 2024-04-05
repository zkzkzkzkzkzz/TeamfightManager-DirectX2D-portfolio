#pragma once
#include <Engine\CLevel.h>

enum class BATTLE_STATE
{
    NONE,
    BLUEBAN,
    REDBAN,
    BLUEPICK1,
    REDPICK1,
    BLUEPICK2,
    REDPICK2,
    BATTLE,
    DONE,
    END,
};

class CBanpickLevel :
    public CLevel
{
public:
    void InitUI();

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;

public:
    CLONE(CBanpickLevel);
    CBanpickLevel();
    CBanpickLevel(const CBanpickLevel& _Origin);
    ~CBanpickLevel();
};

