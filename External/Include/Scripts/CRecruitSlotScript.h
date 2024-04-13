#pragma once
#include <Engine\CScript.h>

typedef  void(*BtnCallBack)(void);
typedef  void(CEntity::* DelegateFunc)(void);

class CGameObject;

class CRecruitSlotScript :
    public CScript
{
private:
    // 영입 슬롯
    CGameObject*    m_Slot;
    Ptr<CTexture>   m_SlotNTex;
    Ptr<CTexture>   m_SlotHTex;
    Ptr<CTexture>   m_SlotSTex;
    Ptr<CTexture>   m_SlotSHTex;
    Ptr<CTexture>   m_SlotDTex;
    Ptr<CTexture>   m_SlotDHTex;
    Ptr<CTexture>   m_SlotCurTex;

    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    CGameObject*    m_RecruitBtn;
    Ptr<CTexture>   m_NormalImg;
    Ptr<CTexture>   m_HoverImg;
    Ptr<CTexture>   m_CurImg;

    CGameObject*    m_Text;
    vector<CGameObject*>    m_DefaultText;
    vector<CGameObject*>    m_NoneText;
    vector<CGameObject*>    m_SearchText;
    vector<CGameObject*>    m_DoneText;

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

    void CheckMousePos();

public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CRecruitSlotScript);
    CRecruitSlotScript();
    CRecruitSlotScript(const CRecruitSlotScript& _Origin);
    ~CRecruitSlotScript();
};

