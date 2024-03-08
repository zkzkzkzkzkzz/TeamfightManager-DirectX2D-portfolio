#pragma once
#include "AssetUI.h"


class ComputeShaderUI :
    public AssetUI
{
public:
    virtual void render_update() override;

public:
    ComputeShaderUI();
    ~ComputeShaderUI();
};

