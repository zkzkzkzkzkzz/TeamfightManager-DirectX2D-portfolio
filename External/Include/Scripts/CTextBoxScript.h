#pragma once
#include <Engine\CScript.h>

class CGameObject;

enum class TEXT_INDEX
{
    NONE,
    TEXT0,
    TEXT1,
    TEXT2,
    TEXT3,
    TEXT4,
    END,
};

class CTextBoxScript :
    public CScript
{
private:
    TEXT_INDEX  m_idx;

    CGameObject* m_Text;
    CGameObject* m_SecondText;
    CGameObject* m_SubText;
    CGameObject* m_Gold;
    CGameObject* m_Mouse;

    CGameObject* m_TextEffect;

    CGameObject* m_Effect;

    float m_TextTime;

    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetTextIdx(TEXT_INDEX _idx) { m_idx = _idx; }
    TEXT_INDEX GetTextIdx() { return m_idx; }

    void SetTextByIndex();
    void SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat = false, Vec3 _offset = Vec3(0.f, 0.f, 0.f));


    void CheckMousePos();
    void LBtnClicked();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CTextBoxScript);
    CTextBoxScript();
    CTextBoxScript(const CTextBoxScript& _Origin);
    ~CTextBoxScript();
};

