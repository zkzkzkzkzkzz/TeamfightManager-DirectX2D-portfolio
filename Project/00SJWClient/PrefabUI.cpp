#include "pch.h"
#include "PrefabUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

PrefabUI::PrefabUI()
	: AssetUI("Prefab", "##Prefab", ASSET_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::render_update()
{
	AssetUI::render_update();

	ImGui::Separator();

	Ptr<CPrefab> pPrefab = (CPrefab*)GetAsset().Get();
	string strPath = ToString(pPrefab->GetRelativePath());

	ImGui::Text("Prefab");
	ImGui::SameLine();
	ImGui::InputText("##PrefabName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);


	if (ImGui::Button("Spawn"))
	{
		CGameObject* pNewObj = pPrefab->Instatiate();
		pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
		GamePlayStatic::SpawnGameObject(pNewObj, 0);
	}
}
