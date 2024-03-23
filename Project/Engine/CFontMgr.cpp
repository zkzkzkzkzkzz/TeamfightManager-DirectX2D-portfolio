#include "pch.h"
#include "CFontMgr.h"

#include "CDevice.h"

CFontMgr::CFontMgr()
	: m_pFW1Factory(nullptr)
	, m_pFontWrapper(nullptr)
	, m_FontInfo{}
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_pFW1Factory)
		m_pFW1Factory->Release();

	if (nullptr != m_pFontWrapper)
		m_pFontWrapper->Release();
}

void CFontMgr::init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Silver", &m_pFontWrapper)))
	{
		assert(NULL);
	}
}

void CFontMgr::DrawFont(const wchar_t* _pStr, LPCWSTR _font, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_pStr, // String
		_font, // FontFamily
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawFont(const wchar_t* _pStr, LPCWSTR _font, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, UINT _flags)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_pStr, // String
		_font, // FontFamily
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE | _flags     // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawFont(const wchar_t* _pStr, wstring _font, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, UINT _flags)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_pStr, // String
		_font.c_str(), // FontFamily
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE | _flags     // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawFont(tTextInfo _info)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_info.m_String.c_str(), // String
		_info.m_Font.c_str(), // FontFamily
		_info.m_FontSize,// Font size
		_info.m_OffsetPos.x,// X position
		_info.m_OffsetPos.y,// Y position
		_info.m_FontColor,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE | _info.m_Flags     // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
