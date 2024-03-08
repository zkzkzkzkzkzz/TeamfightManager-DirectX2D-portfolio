#pragma once
#include "UI.h"

class TreeUI;
class TreeNode;


class Outliner :
    public UI
{
private:
    TreeUI*     m_Tree;

public:
    void ResetCurrentLevel();

private:
    void AddObjectToTree(TreeNode* _Node, CGameObject* _Object);
    void SelectObject(DWORD_PTR _Node);
    void DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source);

public:
    virtual void render_update() override;

public:
    Outliner();
    ~Outliner();
};

