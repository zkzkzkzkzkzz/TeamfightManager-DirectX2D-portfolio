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
    vector<CGameObject*> m_BlueTeam;
    vector<CGameObject*> m_RedTeam;

    BANPICK_STATE m_CurState;

public:
    void SetBanPickState(BANPICK_STATE _state) { m_CurState = _state; }
    BANPICK_STATE GetCurBanPickState() { return m_CurState; }

    void InitUI();

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;

public:
    CLONE(CBanpickLevel);
    CBanpickLevel();
    CBanpickLevel(const CBanpickLevel& _Origin);
    ~CBanpickLevel();
};

