#pragma once
#include "CChampScript.h"

class CGameObject;

class CArcherScript :
    public CChampScript
{
private:
    CGameObject* m_Target;
    float   m_AccTime;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetTeam(TEAM _team) { m_Team = _team; }
    TEAM GetTeamColor() { return m_Team; }


public:
    virtual void InitChampInfo();   // 챔프 정보 설정
    virtual void InitChampAnim();   // 챔프 애니메이션 설정
    virtual void InitStateMachine();
    virtual void CheckStateMachine();

    virtual void EnterIdleState() override;
    virtual void EnterTraceState() override;
    virtual void EnterAttackState() override;
    virtual void EnterSkillState() override;
    virtual void EnterUltimateState() override;
    virtual void EnterDeadState() override;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void render();

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

public:
    CLONE(CArcherScript);
    CArcherScript();
    CArcherScript(const CArcherScript& _Origin);
    ~CArcherScript();

    friend class CArrowScript;
};

