#pragma once
#include "singleton.h"

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper"))
#endif


#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)


struct tFontInfo
{
    wstring sztext;
    LPCWSTR font;
    float   fPosX;
    float   fPosY;
    float   FontSize;
    UINT    Color;
    float   fDuration;
};


class CFontMgr :
    public CSingleton<CFontMgr>
{
    SINGLE(CFontMgr);

private:
    IFW1Factory* m_pFW1Factory;
    IFW1FontWrapper* m_pFontWrapper;

    tFontInfo m_FontInfo;

public:
    void init();
    void DrawFont(const wchar_t* _pStr, LPCWSTR _font, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
    void DrawFont(const wchar_t* _pStr, LPCWSTR _font, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, UINT _flags);
    void DrawFont(tTextInfo _info);
};
