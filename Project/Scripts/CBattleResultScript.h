#pragma once
#include <Engine\CScript.h>

#include "CBanpickLevel.h"

class CGameObject;

class CBattleResultScript :
    public CScript
{
private:
    CBanpickLevel* m_Level;

    bool m_bUIPos;
    float m_UIPosTime;

    bool m_bOpen;
    float m_OpenTime;

    CGameObject* m_WinText;
    CGameObject* m_BlueScoreText;
    CGameObject* m_RedScoreText;
    CGameObject* m_MiddleText;
    CGameObject* m_BlueTeamText;
    CGameObject* m_RedTeamText;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void InitResultText();

    void OpenResult();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CBattleResultScript);
    CBattleResultScript();
    CBattleResultScript(const CBattleResultScript& _Origin);
    ~CBattleResultScript();
};

