#pragma once
#include <Engine\CLevel.h>

#include "CTextBoxScript.h"

class CAwardsLevel :
    public CLevel
{
private:
    TEXT_INDEX m_idx;

public:
    void InitUI();

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;

    void SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat = false, Vec3 _offset = Vec3(0.f, 0.f, 0.f));

public:
    CLONE(CAwardsLevel);
    CAwardsLevel();
    CAwardsLevel(const CAwardsLevel& _Origin);
    ~CAwardsLevel();
};

