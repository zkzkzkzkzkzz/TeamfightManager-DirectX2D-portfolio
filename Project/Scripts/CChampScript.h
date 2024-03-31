#pragma once
#include <Engine\CScript.h>

#include "CEffectScript.h"

#define SKILLCOOLTIME 5.f
#define RESPAWNTIME 3.f
#define GETCHAMP(TYPE) TYPE->GetScript<CChampScript>()

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
    MARKSMAN,
    MAGE,
    SUPPORT,
    ASSASSIN,
    END,
};

enum class CHAMP_STATE
{
    IDLE,
    TRACE,
    ATTACK,
    SKILL,
    ULTIMATE,
    DEAD,
    END,
};

// 챔피언 기본 정보
struct tChampInfo
{
    int         MaxHP;      // 챔피언 기본 체력
    int         ATK;        // 챔피언 기본 공격력
    int         DEF;        // 챔피언 기본 방어력
    float       ATKSpeed;   // 챔피언 공격속도
    int         ATKRange;   // 챔피언 사거리
    float       MOV;        // 챔피언 이동속도
    CHAMP_TYPE  Type;       // 챔피언 타입
};

struct tChampStatus
{ 
    // 챔피언 인게임 정보
    int     HP;                 // 챔피언 현재 체력
    int     ATK;                // 챔피언 현재 체력
    int     DEF;                // 챔피언 현재 체력
    float   CoolTime_Attack;    // 챔피언 공격 쿨타임
    float   CoolTime_Skill;     // 챔피언 스킬 쿨타임
    bool    bSkillPlay;         // 챔피언 스킬 끝남 여부
    float   UltimateUseTime;    // 궁극기 사용 시간
    bool    bUltimate;          // 궁극기 사용 여부
    bool    bUltimateDone;      // 궁극기 끝남 여부
    float   RespawnTime;        // 리스폰 시간

    // 인게임 통계
    UINT    TotalDeal;
    UINT    TotalDamage;
    UINT    TotalHeal;
    UINT    KillPoint;
    UINT    DeathPoint;
    UINT    AssistPoint;
};

class CChampScript :
    public CScript
{
protected:
    CGameObject*    m_Target;
    tChampInfo      m_Info;
    tChampStatus    m_InGameStatus;
    CHAMP_STATE     m_State;
    TEAM            m_Team;

    bool    m_bAttack;
    bool    m_bRespawn;

private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}
    
public:
    virtual void SetTeam(TEAM _team) { m_Team = _team; }
    virtual TEAM GetTeamColor() { return m_Team; }

    virtual void SetChampState(CHAMP_STATE _State) { m_State = _State; }
    virtual CHAMP_STATE GetChampState() { return m_State; }

    virtual void SetTarget(CGameObject* _target) { m_Target = _target; }

    virtual void SetChampDEF(int _def) { m_InGameStatus.DEF = _def; }
    virtual void SetChampHP(int _hp) { m_InGameStatus.HP = _hp; }
    virtual void SetChampATK(int _atk) { m_InGameStatus.HP = _atk; }
    virtual void SetChampMoveSpeed(float _speed) { m_Info.MOV = _speed; }

    virtual int GetChampMaxHP() { return m_Info.MaxHP; }
    virtual int GetInGameChampHP() { return m_InGameStatus.HP; }
    virtual int GetInGameChampATK() { return m_InGameStatus.ATK; }
    virtual int GetInGameChampDEF() { return m_InGameStatus.DEF; }
    virtual float GetChampATKSpeed() { return m_Info.ATKSpeed; }
    virtual int GetChampATKRange() { return m_Info.ATKRange; }
    virtual float GetChampMoveSpeed() { return m_Info.MOV; }
    virtual CHAMP_TYPE GetChampType() { return m_Info.Type; }

    virtual bool IsAttack() { return m_bAttack; }
    virtual bool DoUltimate() { return m_InGameStatus.bUltimate; }
    virtual bool IsSkillPlay() { return m_InGameStatus.bSkillPlay; }
    virtual bool IsUltimateDone() { return m_InGameStatus.bUltimateDone; }

    virtual void Damaged(CGameObject* Attacker, CGameObject* Target, int _ExtraDmg = 0);

    virtual void SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat = false, Vec3 _offset = Vec3(0.f, 0.f, 0.f));

public:
    virtual void InitChampInfo() {}     // 챔프 정보 설정
    virtual void InitChampStatus(int _GamerATK, int _GamerDEF) {}    // 챔프 인게임 정보 설정
    virtual void InitChampAnim() {}     // 챔프 애니메이션 설정
    virtual void InitStateMachine() {}  // 챔피언 스테이트머신 설정
    virtual void CheckStateMachine() {}

    virtual void SetChampInfo(int _MaxHP, int _ATK, int _DEF, float _ATKSpeed, int _ATKRange, int _MoveSpeed, CHAMP_TYPE _Type) {};
    
    void RespawnInfo();
    
public:
    virtual void EnterIdleState() {}
    virtual void EnterTraceState() {}
    virtual void EnterAttackState() {}
    virtual void EnterSkillState() {}
    virtual void EnterUltimateState() {}
    virtual void EnterDeadState() {}

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void render();

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

public:
    CLONE(CChampScript);
    CChampScript();
    CChampScript(UINT _ScriptType);
    CChampScript(const CChampScript& _Origin);
    ~CChampScript();

    friend class CBTMgr;
};

