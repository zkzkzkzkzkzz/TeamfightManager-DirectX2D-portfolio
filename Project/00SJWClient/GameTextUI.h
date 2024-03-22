#pragma once
#include "ComponentUI.h"


class GameTextUI :
    public ComponentUI
{

public:
    virtual void render_update() override;


public:
    GameTextUI();
    ~GameTextUI();
};

