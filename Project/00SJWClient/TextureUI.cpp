#include "pch.h"
#include "TextureUI.h"

#include <Engine\CTexture.h>

TextureUI::TextureUI()
	: AssetUI("Texture", "##Texture", ASSET_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}


void TextureUI::render_update()
{
	AssetUI::render_update();

	// 해당 텍스처 이미지 출력
	Ptr<CTexture> pTex = (CTexture*)GetAsset().Get();

    string strPath = ToString(pTex->GetRelativePath());

    ImGui::Text("Texture");
    ImGui::SameLine();
    ImGui::InputText("##TexName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);

    // 해상도 정보 출력
    float width = pTex->GetWidth();
    float height = pTex->GetHeight();

    ImGui::Text("Width");
    ImGui::SameLine(0, 22); ImGui::PushItemWidth(150);
    ImGui::InputFloat("##TextureWidth", &width, 0.f, 0.f, "%.0f", ImGuiInputTextFlags_ReadOnly);

    ImGui::Text("Height");
    ImGui::SameLine(0, 15); ImGui::PushItemWidth(150);
    ImGui::InputFloat("##TextureHeight", &height, 0.f, 0.f, "%.0f", ImGuiInputTextFlags_ReadOnly);

    static bool use_text_color_for_tint = false;
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image(pTex->GetSRV().Get(), ImVec2(300, 300), uv_min, uv_max, tint_col, border_col);
}