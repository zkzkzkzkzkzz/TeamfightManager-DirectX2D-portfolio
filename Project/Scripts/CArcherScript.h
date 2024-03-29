#pragma once
#include "CChampScript.h"

class CGameObject;

class CArcherScript :
    public CChampScript
{
private:
    CGameObject* m_Target;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void InitChampInfo();   // 챔프 정보 설정
    virtual void InitChampStatus(int _GamerATK, int _GamerDEF); // 챔프 인게임 정보 설정
    virtual void InitChampAnim();   // 챔프 애니메이션 설정
    virtual void InitStateMachine();
    virtual void CheckStateMachine();

    virtual void SetChampInfo(int _MaxHP, int _ATK, int _DEF, float _ATKSpeed, int _ATKRange, int _MoveSpeed, CHAMP_TYPE _Type) override;

public:
    virtual void EnterIdleState() override;
    virtual void EnterTraceState() override;
    virtual void EnterAttackState() override;
    virtual void EnterSkillState() override;
    virtual void EnterUltimateState() override;
    virtual void EnterDeadState() override;

public:
    virtual void begin() override;
    virtual void tick() override;

    //virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    //virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    //virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

public:
    CLONE(CArcherScript);
    CArcherScript();
    CArcherScript(const CArcherScript& _Origin);
    ~CArcherScript();

    friend class CArrowScript;
};

