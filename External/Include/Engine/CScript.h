#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CGameObject.h"
#include "components.h"

class CScript :
    public CComponent
{
private:
    const UINT m_iScriptType;

public:
    UINT GetScriptType() { return m_iScriptType; }

protected:
    void Instantiate(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _LayerIdx);

public:
    // final을 붙여 스크립트를 상속받는 클래스에서는 finaltick을 사용하지 않겠다고 명시
    virtual void finaltick() final {}

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CScript* Clone() = 0;

public:
    CScript(UINT _ScriptType);
    ~CScript();
};

