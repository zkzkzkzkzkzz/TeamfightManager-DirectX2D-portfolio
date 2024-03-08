#pragma once
#include "CComputeShader.h"


class CParticleUpdate :
    public CComputeShader
{
private:
    CStructuredBuffer*  m_ParticleBuffer;
    CStructuredBuffer*  m_ParticleModuleBuffer;
    CStructuredBuffer*  m_SpawnCountBuffer;
   
    Vec3    m_vParticleWorldPos;

public:
    void SetParticleBuffer(CStructuredBuffer* _ParticleBuffer) { m_ParticleBuffer = _ParticleBuffer; }
    void SetParticleModuleBuffer(CStructuredBuffer* _ParticleBuffer) { m_ParticleModuleBuffer = _ParticleBuffer; }
    void SetParticleSpawnCount(CStructuredBuffer* _ParticleBuffer) { m_SpawnCountBuffer = _ParticleBuffer; }

    void SetParticleWorldPos(Vec3 _vWorldPos) { m_vParticleWorldPos = _vWorldPos; }

public:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    CParticleUpdate();
    ~CParticleUpdate();
};

