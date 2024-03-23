#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>

#include "ParamUI.h"
#include "CImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"


MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRender", "##MeshRender", COMPONENT_TYPE::MESHRENDER)
{
	SetSize(ImVec2(0.f, 150.f));
	SetComponentTitle("MeshRender");
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTarget = GetTargetObject();
	CMeshRender* pMeshRender = pTarget->MeshRender();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();

	//if (pMesh == nullptr || pMtrl == nullptr)
	//{
	//	return;
	//}

	string meshname, mtrlname;

	if (nullptr != pMesh)
	{
		meshname = ToString(pMesh->GetKey()).c_str();
	}
	if (nullptr != pMtrl)
	{
		mtrlname = ToString(pMtrl->GetKey()).c_str();
	}

	ImGui::Text("Mesh");
	ImGui::SameLine(0, 46);
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##MeshName", (char*)meshname.c_str(), meshname.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	// 메쉬 드랍 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (ASSET_TYPE::MESH == pAsset->GetType())
			{
				GetTargetObject()->MeshRender()->SetMesh((CMesh*)pAsset);
			}
		}

		ImGui::EndDragDropTarget();
	}
	if (ImGui::Button("##MeshBtn", ImVec2(40, 20)))
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMeshName;
		CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MESH, vecMeshName);

		pListUI->AddString(vecMeshName);
		//pListUI->SetDbClickCallBack(MeshSelect);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshRenderUI::MeshSelect);
		pListUI->Activate();
	}


	ImGui::Text("Material");
	ImGui::SameLine(0, 18);
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##Material", (char*)mtrlname.c_str(), mtrlname.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	// 머테리얼 드랍 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (ASSET_TYPE::MATERIAL == pAsset->GetType())
			{
				GetTargetObject()->MeshRender()->SetMaterial((CMaterial*)pAsset);
			}
		}

		ImGui::EndDragDropTarget();
	}
	if (ImGui::Button("##MtrlBtn", ImVec2(40, 20)))
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMtrlName;
		CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MATERIAL, vecMtrlName);

		pListUI->AddString(vecMtrlName);
		//pListUI->SetDbClickCallBack(MaterialSelect);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshRenderUI::MaterialSelect);
		pListUI->Activate();
	}

	if (GetTargetObject()->MeshRender() && GetTargetObject()->MeshRender()->GetMaterial().Get())
	{

		Ptr<CMaterial> pMtrl = GetTargetObject()->MeshRender()->GetMaterial();
		Ptr<CGraphicsShader> pShader = GetTargetObject()->MeshRender()->GetMaterial()->GetShader();
		string strShaderName;
		if (nullptr != pShader)
		{
			strShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());
		}

		ImGui::Text("Shader");
		ImGui::SameLine(0, 32); ImGui::SetNextItemWidth(200);
		ImGui::InputText("##ShaderName", (char*)strShaderName.c_str(), strShaderName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::Separator(); ImGui::Spacing();
		ImGui::Text("Material Parameter"); ImGui::Spacing();

		// Shader Parameter
		if (nullptr == pShader)
			return;

		const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();
		for (size_t i = 0; i < vecScalarParam.size(); ++i)
		{
			switch (vecScalarParam[i].Type)
			{
			case SCALAR_PARAM::INT_0:
			case SCALAR_PARAM::INT_1:
			case SCALAR_PARAM::INT_2:
			case SCALAR_PARAM::INT_3:
				ParamUI::Param_INT((int*)GetTargetObject()->MeshRender()->GetMaterial()->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
				break;
			case SCALAR_PARAM::FLOAT_0:
			case SCALAR_PARAM::FLOAT_1:
			case SCALAR_PARAM::FLOAT_2:
			case SCALAR_PARAM::FLOAT_3:
				ParamUI::Param_FLOAT((float*)GetTargetObject()->MeshRender()->GetMaterial()->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
				break;
			case SCALAR_PARAM::VEC2_0:
			case SCALAR_PARAM::VEC2_1:
			case SCALAR_PARAM::VEC2_2:
			case SCALAR_PARAM::VEC2_3:
				ParamUI::Param_VEC2((Vec2*)GetTargetObject()->MeshRender()->GetMaterial()->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
				break;
			case SCALAR_PARAM::VEC4_0:
			case SCALAR_PARAM::VEC4_1:
			case SCALAR_PARAM::VEC4_2:
			case SCALAR_PARAM::VEC4_3:
				ParamUI::Param_VEC4((Vec4*)GetTargetObject()->MeshRender()->GetMaterial()->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
				break;
			case SCALAR_PARAM::MAT_0:
			case SCALAR_PARAM::MAT_1:
			case SCALAR_PARAM::MAT_2:
			case SCALAR_PARAM::MAT_3:
				break;
			}
		}
	}
}

void MeshRenderUI::MeshSelect(DWORD_PTR _ptr)
{
	string strMesh = (char*)_ptr;
	wstring strMeshName = ToWString(strMesh);

	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strMeshName);

	GetTargetObject()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::MaterialSelect(DWORD_PTR _ptr)
{
	string strMtrl = (char*)_ptr;
	wstring strMtrlName = ToWString(strMtrl);

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(strMtrlName);

	GetTargetObject()->MeshRender()->SetMaterial(pMtrl);
}

void MeshRenderUI::ScalarParamSelect()
{




}
