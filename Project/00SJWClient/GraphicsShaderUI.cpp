#include "pch.h"
#include "GraphicsShaderUI.h"

GraphicsShaderUI::GraphicsShaderUI()
	: AssetUI("GraphicsShader", "##GraphicsShader", ASSET_TYPE::GRAPHICS_SHADER)
{
}

GraphicsShaderUI::~GraphicsShaderUI()
{
}

void GraphicsShaderUI::render_update()
{
	AssetUI::render_update();
}
