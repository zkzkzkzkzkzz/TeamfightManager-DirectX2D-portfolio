#include "pch.h"
#include "MeshDataUI.h"


MeshDataUI::MeshDataUI()
	: AssetUI("MeshData", "##MeshData", ASSET_TYPE::MESHDATA)
{
}

MeshDataUI::~MeshDataUI()
{
}

void MeshDataUI::render_update()
{
	AssetUI::render_update();
}