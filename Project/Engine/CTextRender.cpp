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
	if (GetOwner()->GetParent())
	{
		Vec3 vPos = GetOwner()->GetParent()->Transform()->GetWorldPos();

		Vec2 vResolution = CEngine::GetInst()->GetResolution();

		vPos.x += vResolution.x / 2.f;
		vPos.y = -(vPos.y - vResolution.y / 2.f);

		m_TextInfo.m_FontPos.x = vPos.x + m_TextInfo.m_OffsetPos.x;
		m_TextInfo.m_FontPos.y = vPos.y + m_TextInfo.m_OffsetPos.y;
	}
	else if (GetOwner() && nullptr == GetOwner()->GetParent())
	{
		Vec3 vPos = GetOwner()->Transform()->GetWorldPos();

		Vec2 vResolution = CEngine::GetInst()->GetResolution();

		vPos.x += vResolution.x / 2.f;
		vPos.y = -(vPos.y - vResolution.y / 2.f);

		m_TextInfo.m_FontPos.x = vPos.x + m_TextInfo.m_OffsetPos.x;
		m_TextInfo.m_FontPos.y = vPos.y + m_TextInfo.m_OffsetPos.y;
	}
	//	vPos = GetOwner()->Transform()->GetWorldPos();
}

void CTextRender::render()
{
	if (0 != m_TextInfo.m_String.size())
		CFontMgr::GetInst()->DrawFont(m_TextInfo.m_String.c_str(), m_TextInfo.m_Font,
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
	wstring GameText;
	GameText = m_TextInfo.m_String;
	SaveWString(GameText, _File);

	wstring GameFont;
	GameFont = m_TextInfo.m_Font;
	SaveWString(GameFont, _File);

	fwrite(&m_TextInfo.m_FontPos, sizeof(Vec3), 1, _File);
	fwrite(&m_TextInfo.m_OffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_TextInfo.m_FontSize, sizeof(float), 1, _File);
	fwrite(&m_TextInfo.m_FontColor, sizeof(UINT), 1, _File);
	fwrite(&m_TextInfo.m_Flags, sizeof(UINT), 1, _File);	
}

void CTextRender::LoadFromFile(FILE* _File)
{
	wstring GameText;
	LoadWString(GameText, _File);
	if (!GameText.empty())
	{
		m_TextInfo.m_String = GameText;
	}

	wstring GameFont;
	LoadWString(GameFont, _File);
	if (!GameFont.empty())
	{
		m_TextInfo.m_Font = GameFont;
	}

	fread(&m_TextInfo.m_FontPos, sizeof(Vec3), 1, _File);
	fread(&m_TextInfo.m_OffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_TextInfo.m_FontSize, sizeof(float), 1, _File);
	fread(&m_TextInfo.m_FontColor, sizeof(UINT), 1, _File);
	fread(&m_TextInfo.m_Flags, sizeof(UINT), 1, _File);
}
