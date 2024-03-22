#pragma once
#include <Engine\CScript.h>

enum class Font_Type
{
	Galmuri9,
	Galmuri11,
	Galmuri14,
	Silver,
	END
};

class CTextScript :
    public CScript
{
private:
	wstring		m_Fonts[(UINT)Font_Type::END];
	wstring		m_Font;
	wstring		m_String;

	Vec3	m_FontPos;
	Vec3	m_OffsetPos;

	float	m_FontSize;
	UINT	m_FontColor;
	UINT	m_Flags;

	float	m_Time;
	float	m_Duration;

public:
	virtual void begin() override;
	virtual void tick() override;
	void CountTextPos();
	void render();

public:
	void SetFont(Font_Type _FontType) { m_Font = m_Fonts[(UINT)_FontType]; }
	void SetFontSize(float _Size) { m_FontSize = _Size; }
	void SetOffsetPos(Vec3 _Offset) { m_OffsetPos = _Offset; }
	void SetString(const wstring& _str) { m_String = _str; }
	void SetFlags(UINT _flags) { m_Flags = _flags; }
	void SetDuration(float _duration) { m_Duration = _duration; }

	float GetFontSize() { return m_FontSize; }
	Vec3 GetOffsetPos() { return m_OffsetPos; }

	void SetFontColor(UINT R, UINT G, UINT B, UINT A);
	void TextInit(Font_Type _FontType, Vec3 _OffsetPos, float _FontSize, UINT _FontColor);


	virtual void SaveToFile(FILE* _File) override {}
	virtual void LoadFromFile(FILE* _File) override {}

public:
	CLONE(CTextScript);
	CTextScript();
	CTextScript(const CTextScript& _Origin);
	~CTextScript();
};

