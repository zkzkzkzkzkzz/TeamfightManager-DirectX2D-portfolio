#pragma once
#include "AssetUI.h"
class GraphicsShaderUI :
    public AssetUI
{
public:
    virtual void render_update() override;

public:
    GraphicsShaderUI();
    ~GraphicsShaderUI();
};

