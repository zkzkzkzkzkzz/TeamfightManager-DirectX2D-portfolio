#pragma once
#include "ComponentUI.h"


class Light2DUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

public:
    Vec3 AngleToVector(float _angle);

public:
    Light2DUI();
    ~Light2DUI();
};

