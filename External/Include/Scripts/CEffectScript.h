#pragma once
#include <Engine\CScript.h>

#define GETEFFECT(TYPE) TYPE->GetScript<CEffectScript>()

class CEffectScript :
    public CScript
{
private:
    Vec3    m_Pos;      // 이펙트 스폰 위치
    Vec3    m_Scale;    // 이펙트 크기
    Vec3    m_Rotation; // 이펙트 회전 여부

    wstring m_EffectName;   // 이펙트 경로

    float   m_AccTime;  // 이펙트 재생 시간
    float   m_Duration; // 이펙트 현재 재생 시간
    bool    m_Repeat;   // 이펙트 반복 여부

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetEffectInfo(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _Name, float _time, bool m_Repeat = false);

    void SetEffectPos(Vec3 _Pos) { m_Pos = _Pos; }
    void SetEffectScale(Vec3 _Scale) { m_Scale = _Scale; }
    void SetEffectPath(const wstring& _Name) { m_EffectName = _Name; }
    void SetEffectTime(float _time) { m_AccTime = _time; }

    Vec3 GetEffectPos() { return m_Pos ;}
    Vec3 GetEffectScale() { return m_Scale ; }
    wstring GetEffectPath() { return m_EffectName; }
    float GetEffectTime() { return m_AccTime; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CEffectScript);
    CEffectScript();
    CEffectScript(const CEffectScript& _Origin);
    ~CEffectScript();
};

