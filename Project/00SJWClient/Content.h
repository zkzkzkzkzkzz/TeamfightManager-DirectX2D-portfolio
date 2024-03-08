#pragma once
#include "UI.h"

class TreeUI;

class Content :
    public UI
{
private:
    TreeUI*         m_Tree;

    vector<wstring> m_vecAssetFileName;

public:
    void ReloadContent();
    void ResetContent();
    void SelectAsset(DWORD_PTR _Node);

public:
    virtual void render_update() override;

private:
    void FindFileName(const wstring& _Directory);
    ASSET_TYPE GetAssetTypeByExt(const path& _relativePath);

public:
    Content();
    ~Content();
};

