#pragma once
#include <Engine\CScript.h>

typedef  void(*BtnCallBack)(void);
typedef  void(CEntity::* DelegateFunc)(void);


class CSquadBtnScript :
    public CScript
{
private:
    Ptr<CTexture>   m_NormalImg;
    Ptr<CTexture>   m_HoverImg;
    Ptr<CTexture>   m_CurImg;

    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    bool    m_isBtnOpen;    // 부모 버튼이 눌렸는가
    bool    m_isOpen;       // 자기 자신이 눌렸는가

    float   m_Time;
    float   m_Duration;

    // 콜백 (전역함수 포인터) 
    BtnCallBack     m_CallBackFunc;

    // 델리게이트(맴버함수 포인터, 객체)
    CEntity* m_Inst;
    DelegateFunc    m_Delegate;

public:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetNormalImg(Ptr<CTexture> _NormalImg) { m_NormalImg = _NormalImg; }
    void SetHoverImg(Ptr<CTexture> _HoverImg) { m_HoverImg = _HoverImg; }

    void SetCallBack(BtnCallBack _CallBack) { m_CallBackFunc = _CallBack; }

    // SetDeletage((CEntity*)m_pOwner->m_pGear, (DelegateFunc)&CGear_PlayLevel::PlayMusic);
    void SetDeletage(CEntity* _Inst, DelegateFunc _Func) { m_Inst = _Inst; m_Delegate = _Func; }

public:
    void OnHovered();
    void OnUnHovered();

    void LBtnUp();

    void LBtnReleased();
    void LBtnClicked();

    void OpenBtn();
    void CloseBtn();

public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CSquadBtnScript);
    CSquadBtnScript();
    CSquadBtnScript(const CSquadBtnScript& _Other);
    ~CSquadBtnScript();
};
