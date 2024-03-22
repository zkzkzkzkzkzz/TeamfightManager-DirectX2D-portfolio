#pragma once
#include <Engine\CGameObject.h>


class CGamer :
    public CGameObject
{
private:
    // 소속 팀

    wstring m_GamerName;    // 선수 이름
    
    int m_GamerATK; // 선수 공격력
    int m_GamerDEF; // 선수 방어력
    int m_GamerAge; // 선수 나이

    Ptr<CTexture>   m_HairTex;
    Ptr<CTexture>   m_BodyTex;

public:
    void begin();
    void tick();
    virtual void finaltick() override;
    void render();

public:
    CLONE(CGamer);
    CGamer();
    CGamer(const CGamer& _Origin);
    ~CGamer();
};

