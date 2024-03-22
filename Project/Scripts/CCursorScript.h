#pragma once
#include <Engine\CScript.h>


class CCursorScript :
    public CScript
{
private:
    Vec2            m_CursorSize;
    Ptr<CTexture>   m_CursorTex;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {};

public:
    CLONE_DISABLE(CCursorScript);
    CCursorScript();
    ~CCursorScript();
};

