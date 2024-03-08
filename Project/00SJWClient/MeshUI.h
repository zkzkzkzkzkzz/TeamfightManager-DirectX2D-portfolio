#pragma once
#include "AssetUI.h"


class MeshUI :
    public AssetUI
{
public:
    virtual void render_update() override;

public:
    MeshUI();
    ~MeshUI();
};

