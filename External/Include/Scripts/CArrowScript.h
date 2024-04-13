#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CArrowScript :
    public CScript
{
private:
    CGameObject*    m_Shooter;
    CGameObject*    m_Target;
    Ptr<CTexture>   m_Tex;


    Vec3    m_prevPos;
    Vec3    m_Pos;
    Vec3    m_Rotation;
    float   m_Speed;
    float   m_lifeTime;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetShooter(CGameObject* _Obj) { m_Shooter = _Obj; }
    void SetTarget(CGameObject* _Obj) { m_Target = _Obj; }

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

