#pragma once
#include <Engine/CScript.h>

class CGameObject;

class CLobbyHdScript :
    public CScript
{
private:
    CGameObject*    m_Logo;
    Ptr<CTexture>   m_LogoTex;
    wchar_t			m_szText[256];
    LPCWSTR			m_font;

public:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CLobbyHdScript);
    CLobbyHdScript();
    CLobbyHdScript(const CLobbyHdScript& _Other);
    ~CLobbyHdScript();
};

