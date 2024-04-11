#pragma once
#include <Engine\CScript.h>


class CCurtainScript :
    public CScript
{
private:
    float m_Time;
    bool m_bUIPos;
    float m_UIPosTime;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void OpenLeftCurtain();
    void OpenRightCurtain();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CCurtainScript);
    CCurtainScript();
    CCurtainScript(const CCurtainScript& _Origin);
    ~CCurtainScript();
};

