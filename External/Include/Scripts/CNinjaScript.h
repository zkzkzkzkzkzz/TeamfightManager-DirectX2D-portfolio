#pragma once
#include "CChampScript.h"


class CNinjaScript :
    public CChampScript
{
private:
    float   m_DealDelay;
    bool    m_DealActive;
    float   m_Skill1Delay;
    float   m_Skill2Delay;
    bool    m_Skill1Active;
    bool    m_Skill2Active;
    float   m_UltiDelay;
    bool    m_UltiActive;
    float   m_DeadDelay;

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
    virtual void render() override;


public:
    CLONE(CNinjaScript);
    CNinjaScript();
    CNinjaScript(const CNinjaScript& _Origin);
    ~CNinjaScript();
};

