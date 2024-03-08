#pragma once
#include "CComponent.h"

enum class PROJ_TYPE
{
    ORTHOGRAPHIC,   // 직교투영
    PERSPECTIVE,    // 원근투영
};

class CCamera :
    public CComponent
{
private:
    PROJ_TYPE   m_ProjType;     // 투영 방식

    // 원근 투영(Perspective)
    float       m_FOV;          // 시야각

    // 직교 투영(Orthographic)
    float       m_Width;        // 직교투영 가로 길이
    float       m_Scale;        // 직교투영 배율

    // Both (둘 다 해당되는 멤버)
    float       m_AspectRatio;  // 종횡비, 투영 가로세로 비율
    float       m_Far;          // 투영 최대 거리
    
    // 변환 행렬
    Matrix      m_matView;
    Matrix      m_matProj;

    UINT        m_LayerCheck;

    int         m_CameraPriority;


    // 물체 분류
    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMasked;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecPostProcess;

public:
    PROJ_TYPE GetProjType() { return m_ProjType; }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }

    float GetScale() { return m_Scale; }
    void SetScale(float _Scale) { m_Scale = _Scale; }

    float GetFOV() { return m_FOV; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }

    float GetFar() { return m_Far; }
    void SetFar(float _far) { m_Far = _far; }

    UINT GetCheckLayer() { return m_LayerCheck; }
    void SetCheckLayer(UINT _Idx) { m_LayerCheck = _Idx; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    void SetCameraPriority(int _Priority);
    void LayerCheck(UINT _LayerIdx, bool _bCheck);
    void LayerCheck(const wstring& _strLayerName, bool _bCheck);
    void LayerCheckAll() { m_LayerCheck = 0xffffffff; }
    void AllLayerOff();

public:
    virtual void begin() override;
    virtual void finaltick() override;
    void render();
    void SortObject();

private:
    void render(vector<CGameObject*>& _vecObj);
    void render_postprocess();

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CCamera);

public:
    CCamera();
    ~CCamera();
};

