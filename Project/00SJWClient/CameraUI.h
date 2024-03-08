#pragma once
#include "ComponentUI.h"

#include <Engine\CGameObject.h>

enum class CAMERALAYER
{
    NONE,
    EVERYTHING,
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

class CameraUI :
    public ComponentUI
{
private:
    CAMERALAYER        m_LayerIdx;

public:
    virtual void render_update() override;

public:
    void SetLayerIdx(CAMERALAYER _idx) { m_LayerIdx = _idx; }
    CAMERALAYER GetLayerIdx() { return m_LayerIdx; }
    string LayerToString(CAMERALAYER _layer);


public:
    CameraUI();
    ~CameraUI();
};

