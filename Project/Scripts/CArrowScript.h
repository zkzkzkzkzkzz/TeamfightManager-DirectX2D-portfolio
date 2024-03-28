#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CArrowScript :
    public CScript
{
private:
    CGameObject*    m_Target;
    Ptr<CTexture>   m_Tex;

    Vec3    m_Pos;
    Vec3    m_Rotation;
    float   m_Speed;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    Vec3 Rotate(Vec3 _vDir, float _angle);

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void render();

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

public:
    CLONE(CArrowScript);
    CArrowScript();
    CArrowScript(const CArrowScript& _Origin);
    ~CArrowScript();
};

