#pragma once
#include "ComponentUI.h"

#include <Engine\CGameObject.h>

enum class OBJECTLAYER
{
    LAYER0,
    LAYER1,
    LAYER2,
    LAYER3,
    LAYER4,
    LAYER5,
    LAYER6,
    LAYER7,
    LAYER8,
    LAYER9,
    LAYER10,
    LAYER11,
    LAYER12,
    LAYER13,
    LAYER14,
    LAYER15,
    LAYER16,
    LAYER17,
    LAYER18,
    LAYER19,
    LAYER20,
    LAYER21,
    LAYER22,
    LAYER23,
    LAYER24,
    LAYER25,
    LAYER26,
    LAYER27,
    LAYER28,
    LAYER29,
    LAYER30,
    LAYER31,
    END,
};


class TransformUI :
    public ComponentUI
{
private:
    OBJECTLAYER        m_LayerIdx;

public:
    virtual void render_update() override;

public:
    string LayerToString(OBJECTLAYER _layer);

public:
    TransformUI();
    ~TransformUI();
};

