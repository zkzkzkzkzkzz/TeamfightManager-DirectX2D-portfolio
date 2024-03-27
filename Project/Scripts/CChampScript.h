#pragma once
#include <Engine\CScript.h>

enum class TEAM
{
    NONE,
    BLUE,
    RED,
    END,
};

enum class CHAMP_TYPE
{
    WARRIOR,
    ARCHERS,
    MAGES,
    SUPPORTERS,
    ASSASSINS,
    END,
};

enum class CHAMP_STATE
{
    IDLE,
    MOVE,
    ATTACK,
    SKILL,
    ULTIMATE,
    DEAD,
    END,
};


struct tChampInfo
{
    int         HP;
    int         MP;
    int         ATK;
    int         DEF;
    float       ATKSpeed;
    int         ATKRange;
    float       MOV;
    CHAMP_TYPE  Type;
};

class CChampScript :
    public CScript
{
private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}
    
public:
    virtual void InitChampInfo() {}   // 챔프 정보 설정
    virtual void InitChampAnim() {}   // 챔프 애니메이션 설정

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
    CLONE(CChampScript);
    CChampScript();
    CChampScript(const CChampScript& _Origin);
    ~CChampScript();
};

