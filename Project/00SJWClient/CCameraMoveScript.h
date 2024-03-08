#pragma once
#include <Engine\CScript.h>

class CCameraMoveScript :
    public CScript
{
private:
    float   m_CamSpeed;

private:
    void MoveOrthographic();
    void MovePerspective();

public:
    virtual void tick();

    virtual void SaveToFile(FILE* _File) override {};
    virtual void LoadFromFile(FILE* _File) override {};

    CLONE(CCameraMoveScript);

public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

