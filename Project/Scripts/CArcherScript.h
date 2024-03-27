#pragma once
#include "CChampScript.h"

class CArcherScript :
    public CChampScript
{
private:
    float           m_DetectRange;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetTeam(TEAM _team) { m_Team = _team; }
    TEAM GetTeamColor() { return m_Team; }

public:
    virtual void InitChampInfo();   // 챔프 정보 설정
    virtual void InitChampAnim();   // 챔프 애니메이션 설정
    virtual void InitStateMachine();

    virtual void Attack();
    virtual void AttackComplete();
    virtual void Heal();
    virtual void HealComplete();
    virtual void Skill() {};
    virtual void Ultimate() {};

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void render();

public:
    CLONE(CArcherScript);
    CArcherScript();
    CArcherScript(const CArcherScript& _Origin);
    ~CArcherScript();
};

