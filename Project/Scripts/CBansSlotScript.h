#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CBansSlotScript :
    public CScript
{
private:
    CGameObject*    m_BanChamp;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetBanChamp(CGameObject* _champ) { m_BanChamp = _champ; }
    CGameObject* GetBanChamp() { return m_BanChamp; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CBansSlotScript);
    CBansSlotScript();
    CBansSlotScript(const CBansSlotScript& _Origin);
    ~CBansSlotScript();
};

