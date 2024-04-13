#pragma once
#include <Engine\CScript.h>

#include "CGamerScript.h"
#include "CBanpickLevel.h"
enum class SLOT_STATE
{
    NONE,
    PICK,
    BAN,
    END,
};

class CGameObject;

class CChampSlotScript :
    public CScript
{
private:
    CHAMP_LIST      m_list;
    CGameObject*    m_Champ;        // 현재 슬롯에 배치할 챔피언
    TEAM            m_TeamColor;    // 자신을 클릭한 선수의 팀
    SLOT_STATE      m_SlotState;    // 슬롯 상태
    CBanpickLevel*  m_Level;


    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    bool    m_IsSelected;

    CGameObject* m_EffectObj;
    bool    m_EffectSpawn;
    float   m_EffectTime;

    CGameObject* m_ToolTip;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetChampList(CHAMP_LIST _list) { m_list = _list; }
    CHAMP_LIST GetChampList() { return m_list; }

    void SetSlotChamp(CGameObject* _champ) { m_Champ = _champ; }
    CGameObject* GetSlotChamp() { return m_Champ; }

    void SetSlotTeamColor(TEAM _team) { m_TeamColor = _team; }
    TEAM GetSlotTeamColor() { return m_TeamColor; }

    void SetSlotState(SLOT_STATE _state) { m_SlotState = _state; }
    SLOT_STATE GetSlotState() { return m_SlotState; }

public:
    void CheckMousePos();

    void OnHovered();
    void OnUnHovered();
    void LBtnClicked();

    void SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat = false, Vec3 _offset = Vec3(0.f, 0.f, 0.f));

    void InitEffectObj();
    void InitChampToSlot();
    void CreateChamp();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CChampSlotScript);
    CChampSlotScript();
    CChampSlotScript(const CChampSlotScript& _Origin);
    ~CChampSlotScript();
};

