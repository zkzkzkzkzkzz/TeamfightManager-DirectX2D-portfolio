#pragma once
#include <Engine\CScript.h>

#include "CTextBoxScript.h"

class CCurtainScript :
    public CScript
{
private:
    TEXT_INDEX m_idx;

    bool m_bTime;
    float m_Time;
    bool m_bUIPos;
    float m_UIPosTime;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void OpenLeftCurtain();
    void OpenRightCurtain();

    void CloseLeftCurtain();
    void CloseRightCurtain();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CCurtainScript);
    CCurtainScript();
    CCurtainScript(const CCurtainScript& _Origin);
    ~CCurtainScript();
};

