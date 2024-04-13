#pragma once
#include <Engine\CGameObject.h>


class CChamp :
    public CGameObject
{
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void render();

public:
    CLONE(CChamp);
    CChamp();
    CChamp(const CChamp& _Origin);
    ~CChamp();
};

