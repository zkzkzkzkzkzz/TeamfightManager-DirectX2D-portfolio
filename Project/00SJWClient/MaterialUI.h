#pragma once
#include "AssetUI.h"


class MaterialUI :
    public AssetUI
{
private:
    TEX_PARAM   m_SelectTexParam;

public:
    void SelectTexture(DWORD_PTR _dwData);

public:
    virtual void render_update() override;

public:
    MaterialUI();
    ~MaterialUI();
};

