#include "pch.h"
#include "CTextRender.h"

#include "CEngine.h"
#include "CGameObject.h"
#include "CTransform.h"

#include "CFontMgr.h"


CTextRender::CTextRender()
	: CRenderComponent(COMPONENT_TYPE::GAMETEXT)
	, m_TextInfo{}
{
}

CTextRender::~CTextRender()
{
}


void CTextRender::finaltick()
{
	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetWorldPos();

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	vPos.x += vResolution.x / 2.f;
	vPos.y = -(vPos.y - vResolution.y / 2.f);

	m_TextInfo.m_FontPos.x = vPos.x + m_TextInfo.m_OffsetPos.x;
	m_TextInfo.m_FontPos.y = vPos.y + m_TextInfo.m_OffsetPos.y;
}

void CTextRender::render()
{
	if (0 != m_TextInfo.m_String.size())
		CFontMgr::GetInst()->DrawFont(m_TextInfo.m_String.c_str(), m_TextInfo.m_Font.c_str(),
									m_TextInfo.m_FontPos.x, m_TextInfo.m_FontPos.y, m_TextInfo.m_FontSize,
									m_TextInfo.m_FontColor, m_TextInfo.m_Flags);
}



void CTextRender::SetFontColor(UINT R, UINT G, UINT B, UINT A)
{
	m_TextInfo.m_FontColor = FONT_RGBA(R, G, B, A);
}

void CTextRender::TextInit(wstring _FontType, float _FontSize, UINT _FontColor, UINT _flags)
{
	m_TextInfo.m_Font = _FontType;
	m_TextInfo.m_FontSize = _FontSize;
	m_TextInfo.m_FontColor = _FontColor;
	m_TextInfo.m_Flags = _flags;
}

void CTextRender::SaveToFile(FILE* _File)
{
}

void CTextRender::LoadFromFile(FILE* _File)
{
}
