#pragma once
#include "AssetUI.h"


class MeshDataUI :
    public AssetUI
{
public:
    virtual void render_update() override;

public:
    MeshDataUI();
    ~MeshDataUI();
};

