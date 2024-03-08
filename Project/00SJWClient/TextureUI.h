#pragma once
#include "AssetUI.h"


class TextureUI :
    public AssetUI
{

public:
    virtual void render_update() override;

public:
    TextureUI();
    ~TextureUI();
};

