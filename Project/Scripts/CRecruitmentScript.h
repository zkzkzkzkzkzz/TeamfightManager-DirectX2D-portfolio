#pragma once
#include <Engine\CScript.h>

typedef  void(*BtnCallBack)(void);
typedef  void(CEntity::* DelegateFunc)(void);

class CGameObject;

class CRecruitmentScript :
    public CScript
{
private:
    // 영입 패널
    CGameObject*    m_Panel;
    Ptr<CTexture>   m_PanelTex;

    CGameObject*    m_CloseBtn;
    Ptr<CTexture>   m_NormalImg;
    Ptr<CTexture>   m_HoverImg;
    Ptr<CTexture>   m_CurImg;

    CGameObject*    m_RecruitBtn;
    Ptr<CTexture>   m_RNormalImg;
    Ptr<CTexture>   m_RHoverImg;
    Ptr<CTexture>   m_RCurImg;

    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    bool    m_RbMouseOn;
    bool    m_RbMouseOn_Prev;
    bool    m_RbMouseLBtnDown;

    // 콜백 (전역함수 포인터) 
    BtnCallBack     m_CallBackFunc;

    // 델리게이트(맴버함수 포인터, 객체)
    CEntity* m_Inst;
    DelegateFunc    m_Delegate;

public:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetCallBack(BtnCallBack _CallBack) { m_CallBackFunc = _CallBack; }

    // SetDeletage((CEntity*)m_pOwner->m_pGear, (DelegateFunc)&CGear_PlayLevel::PlayMusic);
    void SetDeletage(CEntity* _Inst, DelegateFunc _Func) { m_Inst = _Inst; m_Delegate = _Func; }

public:
    void OnHovered();
    void OnUnHovered();

    void LBtnUp();

    void LBtnReleased();
    void LBtnClicked();

public:
    void ROnHovered();
    void ROnUnHovered();

    void RLBtnUp();

    void RLBtnReleased();
    void RLBtnClicked();

    void CheckMousePos();
    void CheckRecruitBtnPos();

public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE_DISABLE(CRecruitmentScript);
    CRecruitmentScript();
    ~CRecruitmentScript();
};

