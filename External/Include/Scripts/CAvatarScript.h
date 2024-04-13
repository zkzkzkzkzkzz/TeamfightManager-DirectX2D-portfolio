#pragma once
#include <Engine\CScript.h>

#include "CTextBoxScript.h"


class CAvatarScript :
    public CScript
{
private:
    TEXT_INDEX m_idx;
    wstring m_AvatarName;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void PlayIdleAnim();
    void PlayJumpAnim();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CAvatarScript);
    CAvatarScript();
    CAvatarScript(const CAvatarScript& _Origin);
    ~CAvatarScript();
};

