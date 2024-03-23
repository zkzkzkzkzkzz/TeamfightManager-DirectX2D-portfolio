#pragma once
#include "ComponentUI.h"

#include <Engine\CGameObject.h>

class MeshRenderUI :
    public ComponentUI
{
private:
    TEX_PARAM   m_SelectTexParam;

public:
    virtual void render_update() override;

public:
    void MeshSelect(DWORD_PTR _ptr);
    void MaterialSelect(DWORD_PTR _ptr);
    void ScalarParamSelect();

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

