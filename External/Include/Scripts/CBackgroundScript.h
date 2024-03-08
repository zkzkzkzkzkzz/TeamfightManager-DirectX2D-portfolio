#pragma once
#include <Engine\CScript.h>

class CBackgroundScript :
    public CScript
{
public:
    virtual void tick() override;

    Vec3 Rotate(Vec3 _vDir, float _angle);


    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CBackgroundScript);

public:
    CBackgroundScript();
    ~CBackgroundScript();
};

