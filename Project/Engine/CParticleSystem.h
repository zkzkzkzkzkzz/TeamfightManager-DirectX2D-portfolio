#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdate.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*      m_ParticleBuffer;
    UINT                    m_MaxParticleCount;

    Ptr<CParticleUpdate>    m_CSParticleUpdate;

    tParticleModule         m_Module;
    CStructuredBuffer*      m_ParticleModuleBuffer;
    CStructuredBuffer*      m_SpawnCountBuffer;
    
    float   m_Time;

    Ptr<CTexture>   m_ParticleTex;

public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CParticleSystem);

public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _OriginParticle);
    ~CParticleSystem();
};

