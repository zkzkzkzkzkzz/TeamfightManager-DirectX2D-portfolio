#pragma once

#include <Engine/CScript.h>

class CGameObject;

class CGamerScript :
    public CScript
{
private:
    // 소속 팀
    // 선수가 플레이 중인 챔피언

    wstring m_GamerName;    // 선수 이름

    int m_GamerATK; // 선수 공격력
    int m_GamerDEF; // 선수 방어력
    int m_GamerAge; // 선수 나이

    CGameObject*    m_Text;
    Ptr<CTexture>   m_Tex;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    int GetATK() { return m_GamerATK; }
    int GetDEF() { return m_GamerDEF; }
    int GetAge() { return m_GamerAge; }

    wstring GetGamerName() { return m_GamerName; }

public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CGamerScript);
    CGamerScript();
    CGamerScript(const CGamerScript& _Origin);
    ~CGamerScript();
};

