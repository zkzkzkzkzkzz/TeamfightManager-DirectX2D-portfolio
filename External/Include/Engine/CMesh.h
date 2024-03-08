#pragma once
#include "CAsset.h"


class CMesh :
    public CAsset
{
private:
    ComPtr<ID3D11Buffer>    m_VB;   // 버텍스 버퍼
    ComPtr<ID3D11Buffer>    m_IB;   // 인덱스 버퍼

    D3D11_BUFFER_DESC       m_VBDesc;
    D3D11_BUFFER_DESC       m_IBDesc;

    UINT                    m_VtxCount;
    UINT                    m_IdxCount;

    void*                   m_VtxSysMem;
    void*                   m_IdxSysMem;

private:
    void UpdateData();

public:
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);
    void render();
    void render_asparticle(UINT _ParticleCount);

    CLONE_DISABLE(CMesh);

public:
    CMesh(bool _Engine = false);
    ~CMesh();
};

