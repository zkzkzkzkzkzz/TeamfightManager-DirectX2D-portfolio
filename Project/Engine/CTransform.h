#pragma once
#include "CComponent.h"


class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRotation;
    Vec3    m_arrLocalDir[3];       // 상대적인 오브젝트 좌표
    Vec3    m_arrWorldDir[3];       // 진짜 월드 좌표

    // 상태 행렬 (4*4 크기를 가지는 행렬)
    Matrix  m_matWorld;
    bool    m_bAbsolute;            // 자식 오브젝트를 부모 오브젝트와 독립적인 좌표로 사용할 것인가

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;

public:
    void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rotation) { m_vRelativeRotation = _Rotation; }

    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() { return m_vRelativeRotation; }

    void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }
    bool IsAbsolute() { return m_bAbsolute; }

    const Matrix& GetWorldMat() { return m_matWorld; }

    Vec3 GetLocalDir(DIR_TYPE _Type) { return m_arrLocalDir[(UINT)_Type]; }
    Vec3 GetWorldDir(DIR_TYPE _Type) { return m_arrWorldDir[(UINT)_Type]; }
    void SetWorldMat(const Matrix _matWorld) { m_matWorld = _matWorld; }

    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetWorldScale();
    //Vec3 GetWorldRot();


    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CTransform);

public:
    CTransform();
    ~CTransform();
};

