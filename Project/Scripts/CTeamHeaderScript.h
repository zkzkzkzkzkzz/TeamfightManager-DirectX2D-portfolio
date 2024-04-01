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

    CGameObject*    m_BlueScore;
    wstring         m_BScore;
    CGameObject*    m_RedScore;
    wstring         m_RScore;

    CGameObject*    m_BlueTeamName;
    CGameObject*    m_RedTeamName;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void InitBlueTeam();
    void InitRedTeam();
    void InitTimer();
    void InitTeamName();
    
    void CheckScore();
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

