#pragma once
#include <Engine\CLevel.h>

enum class BANPICK_STATE
{
    NONE,
    BLUEBAN,
    REDBAN,
    BLUEPICK1,
    REDPICK1,
    BLUEPICK2,
    REDPICK2,
    READY,
    BATTLE,
    DONE,
    END,
};

enum class CHAMP_LIST
{
    ARCHER,
    FIGHTER,
    KNIGHT,
    MONK,
    NINJA,
    PRIEST,
    END,
};

class CBanpickLevel :
    public CLevel
{
private:
    float m_EnemyTime;

    vector<CGameObject*> m_BlueTeam;
    vector<CGameObject*> m_RedTeam;

    vector<CGameObject*> m_BanPickSlot;

    BANPICK_STATE m_CurState;

public:
    void SetBanPickState(BANPICK_STATE _state) { m_CurState = _state; }
    BANPICK_STATE GetCurBanPickState() { return m_CurState; }

    void SetEnemyTime(float _time) { m_EnemyTime = _time; }
    float GetEnemyTime() { return m_EnemyTime; }

    void InitUI();
    void InitBattleStadium();

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;

    void SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat = false, Vec3 _offset = Vec3(0.f, 0.f, 0.f));

public:
    CLONE(CBanpickLevel);
    CBanpickLevel();
    CBanpickLevel(const CBanpickLevel& _Origin);
    ~CBanpickLevel();
};

