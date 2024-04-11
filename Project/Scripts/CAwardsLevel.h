#pragma once
#include <Engine\CLevel.h>

#include "CTextBoxScript.h"

class CGameObject;

class CAwardsLevel :
    public CLevel
{
private:
    TEXT_INDEX m_idx;

    float m_LVChangeTime;

    bool m_bBGM;
    float m_BGMTime;

    int m_ParticleCount;
    bool m_ParticleSpawn;
    float m_ParticleTime;
    vector<CGameObject*> m_Particle;

public:
    void InitUI();
    void SpawnParticle();

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

