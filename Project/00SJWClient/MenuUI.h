#pragma once
#include "UI.h"


class MenuUI :
    public UI
{

private:
    void File();
    void Level();
    void GameObject();
    void Asset();
    void Prefab();

public:
    virtual void render() override;
    virtual void render_update() override;

public:
    MenuUI();
    ~MenuUI();
};

