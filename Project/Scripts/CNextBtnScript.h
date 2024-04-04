#pragma once
#include <Engine\CScript.h>

typedef  void(*BtnCallBack)(void);
typedef  void(CEntity::* DelegateFunc)(void);

class CNextBtnScript :
    public CScript
{
private:
    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    // 콜백 (전역함수 포인터) 
    BtnCallBack     m_CallBackFunc;

    // 델리게이트(맴버함수 포인터, 객체)
    CEntity* m_Inst;
    DelegateFunc    m_Delegate;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    virtual void begin() override;
    virtual void tick() override;

    void SetCallBack(BtnCallBack _CallBack) { m_CallBackFunc = _CallBack; }

    // SetDeletage((CEntity*)m_pOwner->m_pGear, (DelegateFunc)&CGear_PlayLevel::PlayMusic);
    void SetDeletage(CEntity* _Inst, DelegateFunc _Func) { m_Inst = _Inst; m_Delegate = _Func; }

public:
    void OnHovered();
    void OnUnHovered();
    void LBtnClicked();

    void CheckMousePos();

public:
    CLONE(CNextBtnScript);
    CNextBtnScript();
    CNextBtnScript(const CNextBtnScript& _Other);
    ~CNextBtnScript();
};

