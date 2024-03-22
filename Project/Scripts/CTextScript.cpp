#include "pch.h"
#include "CTextScript.h"

#include <Engine\CEngine.h>
#include <Engine\CFontMgr.h>


CTextScript::CTextScript()
	: CScript(TEXTSCRIPT)
	, m_Font{}
	, m_String{}
	, m_FontPos{}
	, m_OffsetPos{}
	, m_FontSize(100.f)
	, m_FontColor(0)
	, m_Flags(0)
	, m_Time(0.f)
	, m_Duration(0.f)
{
	m_FontColor = FONT_RGBA(255, 255, 255, 255);

	m_Fonts[(UINT)Font_Type::Galmuri9] = L"Galmuri9";
	m_Fonts[(UINT)Font_Type::Galmuri11] = L"Galmuri11";
	m_Fonts[(UINT)Font_Type::Galmuri14] = L"Galmuri14";
	m_Fonts[(UINT)Font_Type::Silver] = L"Silver";

	SetFont(Font_Type::Silver);
}

CTextScript::CTextScript(const CTextScript& _Origin)
	: CScript(TEXTSCRIPT)
	, m_Font(_Origin.m_Font)
	, m_String{}
	, m_FontPos(_Origin.m_FontPos)
	, m_OffsetPos(_Origin.m_OffsetPos)
	, m_FontSize(_Origin.m_FontSize)
	, m_FontColor(_Origin.m_FontColor)
	, m_Flags(_Origin.m_Flags)
	, m_Time(0.f)
	, m_Duration(0.f)
{
	m_FontColor = FONT_RGBA(255, 255, 255, 255);

	m_Fonts[(UINT)Font_Type::Galmuri9] = L"Galmuri9";
	m_Fonts[(UINT)Font_Type::Galmuri11] = L"Galmuri11";
	m_Fonts[(UINT)Font_Type::Galmuri14] = L"Galmuri14";
	m_Fonts[(UINT)Font_Type::Silver] = L"Silver";

	SetFont(Font_Type::Silver);
}

CTextScript::~CTextScript()
{
}


void CTextScript::begin()
{

}

void CTextScript::tick()
{
	CountTextPos();
	render();
}

void CTextScript::CountTextPos()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	m_FontPos = vPos;
	m_FontPos.y *= -1.f;

	RECT rect = {};
	GetClientRect(CEngine::GetInst()->GetMainWind(), &rect);

	float width = (float)(rect.right - rect.left);
	float height = (float)(rect.bottom - rect.top);

	m_FontPos.x += width / 2.f + m_OffsetPos.x;
	m_FontPos.y += height / 2.f - m_OffsetPos.y;
	m_FontPos.z += m_FontPos.z + m_OffsetPos.z;
}

void CTextScript::render()
{
	if (0 != m_String.size())
		CFontMgr::GetInst()->DrawFont(m_String.c_str(), m_Font.c_str(), m_FontPos.x, m_FontPos.y, m_FontSize, m_FontColor, m_Flags);
}

void CTextScript::TextInit(Font_Type _FontType, Vec3 _OffsetPos, float _FontSize, UINT _FontColor)
{
	m_Font = m_Fonts[(UINT)_FontType];
	m_OffsetPos = _OffsetPos;
	m_FontSize = _FontSize;
	m_FontColor = _FontColor;
}

void CTextScript::SetFontColor(UINT R, UINT G, UINT B, UINT A)
{
	m_FontColor = FONT_RGBA(R, G, B, A);
}