#include "pch.h"
#include "GameTextUI.h"

#include <Engine\CFontMgr.h>
#include <Engine\CTextRender.h>

GameTextUI::GameTextUI()
	: ComponentUI("GameText", "##GameText", COMPONENT_TYPE::GAMETEXT)
{
	SetSize(ImVec2(0.f, 135.f));
	SetComponentTitle("GameText");
}

GameTextUI::~GameTextUI()
{
}

void GameTextUI::render_update()
{
	ComponentUI::render_update();

	Vec3 vPos = GetTargetObject()->TextRender()->GetOffsetPos();
	float size = GetTargetObject()->TextRender()->GetFontSize();
	static int FontColor = GetTargetObject()->TextRender()->GetFontColor();

	static int R = FontColor & 255;
	FontColor >> 8;
	static int G = FontColor & 255;
	FontColor >> 8;
	static int B = FontColor & 255;
	FontColor >> 8;

	ImGui::Text("Font Offset");
	ImGui::SameLine(0, 13);	ImGui::PushItemWidth(70);
	ImGui::Text("x"); ImGui::SameLine(); ImGui::DragFloat("##Posx", &vPos.x); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine(); ImGui::DragFloat("##Posy", &vPos.y); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine(); ImGui::DragFloat("##Posz", &vPos.z);

	ImGui::Text("Font Size");
	ImGui::SameLine(0, 42); ImGui::PushItemWidth(70);
	ImGui::DragFloat("##FontSize", &size);

	ImGui::Text("Font Color");
	ImGui::SameLine(0, 20); ImGui::PushItemWidth(70);
	ImGui::Text("r"); ImGui::SameLine(); ImGui::DragInt("##r", &R); ImGui::SameLine();
	ImGui::Text("g"); ImGui::SameLine(); ImGui::DragInt("##g", &G); ImGui::SameLine();
	ImGui::Text("b"); ImGui::SameLine(); ImGui::DragInt("##b", &B);

	GetTargetObject()->TextRender()->SetOffsetPos(vPos);
	GetTargetObject()->TextRender()->SetFontSize(size);
	GetTargetObject()->TextRender()->SetFontColor((UINT)R, (UINT)G, (UINT)B, 255);

	ImGui::Text("Text");
	char Text[256] = {};
	string temp = ToString(GetTargetObject()->TextRender()->GetString());
	
	for (size_t i = 0; i < temp.length(); ++i)
	{
		Text[i] = temp[i];
	}

	ImGui::SameLine(0, 77);
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##Text", Text, 100);
	GetTargetObject()->TextRender()->SetString(ToWString(Text));

	string fonttype = ToString(GetTargetObject()->TextRender()->GetFont());
	static int type = 0;

	if ("Galmuri9" == fonttype)
	{
		type = (int)FONT_TYPE::Galmuri9;
	}
	else if ("Galmuri11" == fonttype)
	{
		type = (int)FONT_TYPE::Galmuri11;
	}
	else if ("Galmuri14" == fonttype)
	{
		type = (int)FONT_TYPE::Galmuri14;
	}
	else if ("Silver" == fonttype)
	{
		type = (int)FONT_TYPE::Silver;
	}

	ImGui::Text("Font Type");
	ImGui::SameLine(0, 42);	ImGui::PushItemWidth(100);
	ImGui::Combo("##FontType", &type, "Galmuri9\0Galmuri11\0Galmuri14\0Silver\0\0");

	switch (type)
	{
	case 0:
		GetTargetObject()->TextRender()->SetFont(ToWString("Galmuri9"));
		break;
	case 1:
		GetTargetObject()->TextRender()->SetFont(ToWString("Galmuri11"));
		break;
	case 2:
		GetTargetObject()->TextRender()->SetFont(ToWString("Galmuri14"));
		break;
	case 3:
		GetTargetObject()->TextRender()->SetFont(ToWString("Silver"));
		break;
	default:
		break;
	}
}
