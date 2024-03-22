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
	static Vec3 fontcolor = Vec3(0.f, 0.f, 0.f);

	ImGui::Text("Font Offset");
	ImGui::SameLine(0, 13);	ImGui::PushItemWidth(70);
	ImGui::Text("x"); ImGui::SameLine(); ImGui::DragFloat("##Posx", &vPos.x); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine(); ImGui::DragFloat("##Posy", &vPos.y);

	ImGui::Text("Font Size");
	ImGui::SameLine(0, 42); ImGui::PushItemWidth(70);
	ImGui::DragFloat("##FontSize", &size);

	ImGui::Text("Font Color");
	ImGui::SameLine(0, 20); ImGui::PushItemWidth(70);
	ImGui::Text("r"); ImGui::SameLine(); ImGui::InputFloat("##r", &fontcolor.x, 0.f, 0.f, "%.0f"); ImGui::SameLine();
	ImGui::Text("g"); ImGui::SameLine(); ImGui::InputFloat("##g", &fontcolor.y, 0.f, 0.f, "%.0f"); ImGui::SameLine();
	ImGui::Text("b"); ImGui::SameLine(); ImGui::InputFloat("##b", &fontcolor.z, 0.f, 0.f, "%.0f");

	GetTargetObject()->TextRender()->SetOffsetPos(vPos);
	GetTargetObject()->TextRender()->SetFontSize(size);
	GetTargetObject()->TextRender()->SetFontColor((UINT)fontcolor.x, (UINT)fontcolor.y, (UINT)fontcolor.z, 255);

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
