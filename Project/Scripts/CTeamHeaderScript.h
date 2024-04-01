#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CTeamHeaderScript :
    public CScript
{
private:
    Ptr<CTexture>   m_HeaderTex;

    CGameObject*    m_BlueTeam;
    Ptr<CTexture>   m_BlueTeamLogo;
    CGameObject*    m_RedTeam;
    Ptr<CTexture>   m_RedTeamLogo;

    CGameObject*    m_Time;
    wstring         m_strTime;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    int CheckTime();
    void SetTimer(int _CurTime);

public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CTeamHeaderScript);
    CTeamHeaderScript();
    CTeamHeaderScript(const CTeamHeaderScript& _Origin);
    ~CTeamHeaderScript();
};

