#pragma once
#include "CRenderComponent.h"


class CTextRender :
    public CRenderComponent
{
private:
	tTextInfo	m_TextInfo;

public:
	void SetString(const wstring& _str) { m_TextInfo.m_String = _str; }
	void SetFont(wstring _FontType) { m_TextInfo.m_Font = _FontType; }
	void SetFontSize(float _Size) { m_TextInfo.m_FontSize = _Size; }
	void SetOffsetPos(Vec3 _Offset) { m_TextInfo.m_OffsetPos = _Offset; }
	void SetFlags(UINT _flags) { m_TextInfo.m_Flags = _flags; }

	float GetFontSize() { return m_TextInfo.m_FontSize; }
	Vec3 GetOffsetPos() { return m_TextInfo.m_OffsetPos; }
	UINT GetFlags() { return m_TextInfo.m_Flags; }
	UINT GetFontColor() { return m_TextInfo.m_FontColor; }
	const wstring& GetString() { return m_TextInfo.m_String; }
	const wstring& GetFont() { return m_TextInfo.m_Font; }

	void SetFontColor(UINT R, UINT G, UINT B, UINT A);
	void TextInit(wstring _FontType, float _FontSize, UINT _FontColor, UINT _flags = 0);

public:
	virtual void UpdateData() override {}
    virtual void finaltick() override;
    virtual void render() override;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CTextRender);
    CTextRender();
    ~CTextRender();
};
