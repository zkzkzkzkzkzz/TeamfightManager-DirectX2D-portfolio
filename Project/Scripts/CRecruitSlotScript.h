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
    Ptr<CTexture>   m_SlotCurTex;

    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

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
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    CLONE(CRecruitSlotScript);
    CRecruitSlotScript();
    CRecruitSlotScript(const CRecruitSlotScript& _Origin);
    ~CRecruitSlotScript();
};

