#pragma once
#include <Engine\CScript.h>

typedef  void(*BtnCallBack)(void);
typedef  void(CEntity::* DelegateFunc)(void);

class CGameObject;

enum class RECRUIT_STATE
{
    NONE,
    SEARCH,
    DONE,
    CLOSE,
    END,
};

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
    Ptr<CTexture>   m_DNormalImg;
    Ptr<CTexture>   m_DHoverImg;
    Ptr<CTexture>   m_RCurImg;
    CGameObject*    m_RText;
    CGameObject*    m_CoinText;

    vector<CGameObject*>    m_vecText;

    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    bool    m_RbMouseOn;
    bool    m_RbMouseOn_Prev;
    bool    m_RbMouseLBtnDown;

    RECRUIT_STATE   m_RState;
    bool            m_bStateChange;

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
    void SetState(RECRUIT_STATE _state) { m_RState = _state; }
    RECRUIT_STATE GetState() { return m_RState; }
    bool IsChange() { return m_bStateChange; }

    void RecruitGamer();

public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CRecruitmentScript);
    CRecruitmentScript();
    CRecruitmentScript(const CRecruitmentScript& _Origin);
    ~CRecruitmentScript();

    friend class CRecruitmentBtnScript;
};

