#include "pch.h"
#include "GameTextUI.h"

GameTextUI::GameTextUI()
	: ComponentUI("GameText", "##GameText", COMPONENT_TYPE::GAMETEXT)
{
}

GameTextUI::~GameTextUI()
{
}

void GameTextUI::render_update()
{
	ComponentUI::render_update();
}
