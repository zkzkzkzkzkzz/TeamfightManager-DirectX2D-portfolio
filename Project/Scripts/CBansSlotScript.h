#pragma once
#include <Engine\CScript.h>

#include "CGamerScript.h"
#include "CBanpickLevel.h"

class CGameObject;

class CBansSlotScript :
    public CScript
{
private:
    CGameObject*    m_BanSlot;
    CGameObject*    m_Champ;
    CGameObject*    m_Text;
    CBanpickLevel*  m_Level;

    bool m_IsSpawn;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetBanChampSlot(CGameObject* _champ) { m_BanSlot = _champ; }
    CGameObject* GetBanChampSlot() { return m_BanSlot; }

    void SetChampToBanSlot();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CBansSlotScript);
    CBansSlotScript();
    CBansSlotScript(const CBansSlotScript& _Origin);
    ~CBansSlotScript();
};

