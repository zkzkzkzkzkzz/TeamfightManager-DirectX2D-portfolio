#pragma once
#include <Engine\CGameObject.h>


class CGamer :
    public CGameObject
{
private:
    // 소속 팀
    // 선수가 플레이 중인 챔피언

    wstring m_GamerName;    // 선수 이름
    
    int m_GamerATK; // 선수 공격력
    int m_GamerDEF; // 선수 방어력
    int m_GamerAge; // 선수 나이

    bool m_IsManager;   // 감독 여부

    Ptr<CTexture>   m_HairTex;
    Ptr<CTexture>   m_BodyTex;

public:
    void begin();
    void tick();
    virtual void finaltick() override;
    void render();

public:
    void SetGamerName(wstring _name) { m_GamerName = _name; }
    void SetGamerData(int _ATK, int _DEF, int _Age);
    void SetManager(bool _b) { m_IsManager = _b; }
    
    int GetGamerATK() { return m_GamerATK; }
    int GetGamerDEF() { return m_GamerDEF; }
    int GetGamerAge() { return m_GamerAge; }
    bool IsManager() { return m_IsManager; }

public:
    CLONE(CGamer);
    CGamer();
    CGamer(const CGamer& _Origin);
    ~CGamer();
};

