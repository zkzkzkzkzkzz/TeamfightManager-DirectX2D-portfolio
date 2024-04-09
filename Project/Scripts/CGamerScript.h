#pragma once

#include <Engine/CScript.h>
#include "CChampScript.h"

#include "CBanpickLevel.h"

#define GETGAMER(TYPE) TYPE->GetScript<CGamerScript>()

enum class GAMER_LEVEL
{
    NONE,
    CHOBO,
    JOONGSOO,
    GOSU,
    END,
};

class CGameObject;

class CGamerScript :
    public CScript
{
protected:
    GAMER_LEVEL     m_GamerLevel;       // 해당 선수의 레벨
    TEAM            m_Team;             // 소속 팀
    CHAMP_LIST      m_SelectedChamp;    // 선수가 플레이 중인 챔피언

    wstring m_GamerName;    // 선수 이름

    int m_GamerATK; // 선수 공격력
    int m_GamerDEF; // 선수 방어력

    Ptr<CTexture>   m_Tex;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetGamerLevel(GAMER_LEVEL _Lv) { m_GamerLevel = _Lv; }
    GAMER_LEVEL GetGamerLevel() { return m_GamerLevel; }

    void SetGamerInfo(int _ATK, int _DEF, TEAM _team = TEAM::NONE);

    int GetATK() { return m_GamerATK; }
    int GetDEF() { return m_GamerDEF; }

    void SetGamerName(const wstring& _name) { m_GamerName = _name; }
    wstring GetGamerName() { return m_GamerName; }

    void SetSelectedChamp(CHAMP_LIST _Champ) { m_SelectedChamp = _Champ; }
    CHAMP_LIST GetSelectedChamp() { return m_SelectedChamp; }

    void SetGamerTeam(TEAM _color) { m_Team = _color; }
    TEAM GetGamerTeam() { return m_Team; }

    void SetTexture(const wstring& _str);
    Ptr<CTexture> GetGamerTexture() { return m_Tex; }

public:
    void SetPlayableGamer();
    void InsertGamerList();
    void InsertRecruitList();
    void IsPlayableGamer();
    void SetGamerPos();


public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CGamerScript);
    CGamerScript();
    CGamerScript(UINT _ScriptType);
    CGamerScript(const CGamerScript& _Origin);
    ~CGamerScript();
};

