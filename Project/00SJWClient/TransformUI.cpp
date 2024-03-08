#include "pch.h"
#include "TransformUI.h"

#include <Engine\CTaskMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CGameObject.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>

#include "Outliner.h"

TransformUI::TransformUI()
	: ComponentUI("Transform", "##Transform", COMPONENT_TYPE::TRANSFORM)
	, m_LayerIdx(OBJECTLAYER::END)
{
	SetSize(ImVec2(0.f, 165.f));
	SetComponentTitle("Transform");
}

TransformUI::~TransformUI()
{
}

void TransformUI::render_update()
{
	ComponentUI::render_update();

	Vec3 vPos = GetTargetObject()->Transform()->GetRelativePos();
	Vec3 vScale = GetTargetObject()->Transform()->GetRelativeScale();
	Vec3 vRot = GetTargetObject()->Transform()->GetRelativeRotation();
	vRot.ToDegree();


	ImGui::Text("Position");
	ImGui::SameLine(0, 20);	ImGui::PushItemWidth(80);
	ImGui::Text("x"); ImGui::SameLine(); ImGui::DragFloat("##Posx", &vPos.x); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine(); ImGui::DragFloat("##Posy", &vPos.y); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine(); ImGui::DragFloat("##Posz", &vPos.z);

	ImGui::Text("Scale");
	ImGui::SameLine(0, 41);	ImGui::PushItemWidth(80);
	ImGui::Text("x"); ImGui::SameLine(); ImGui::DragFloat("##Scalex", &vScale.x); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine(); ImGui::DragFloat("##Scaley", &vScale.y); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine(); ImGui::DragFloat("##Scalez", &vScale.z);

	ImGui::Text("Rotation");
	ImGui::SameLine(0, 20);	ImGui::PushItemWidth(80);
	ImGui::Text("x"); ImGui::SameLine(); ImGui::DragFloat("##Rotx", &vRot.x); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine(); ImGui::DragFloat("##Roty", &vRot.y); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine(); ImGui::DragFloat("##Rotz", &vRot.z);

	vRot.ToRadian();

	GetTargetObject()->Transform()->SetRelativePos(vPos);
	GetTargetObject()->Transform()->SetRelativeScale(vScale);
	GetTargetObject()->Transform()->SetRelativeRotation(vRot);

	// Absolute 체크
	bool bAbsolute = GetTargetObject()->Transform()->IsAbsolute();
	ImGui::Text("Scale Absolute");
	ImGui::SameLine();
	ImGui::Checkbox("##TransformAbsolute", &bAbsolute);
	GetTargetObject()->Transform()->SetAbsolute(bAbsolute);



	ImGui::Separator();

	// 오브젝트 이름
	ImGui::Text("Name");
	char ObjName[200] = {};
	string temp = ToString(GetTargetObject()->GetName()).c_str();
	string prevName = temp;

	for (size_t i = 0; i < temp.length(); ++i)
	{
		ObjName[i] = temp[i];
	}
	

	ImGui::SameLine(0, 65);
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##ObjName", ObjName, 100);
	GetTargetObject()->SetName(ToWString(ObjName));

	if (prevName != string(ObjName))
	{
		GamePlayStatic::ChangeName();
	}

	// 오브젝트 레이어
	m_LayerIdx = (OBJECTLAYER)GetTargetObject()->GetLayerIdx();
	int PrevIdx = GetTargetObject()->GetLayerIdx();

	ImGui::Text("Layer");
	ImGui::SameLine(0, 58);	ImGui::PushItemWidth(80);
	if (ImGui::BeginCombo("##ObjLayer", LayerToString(m_LayerIdx).c_str()))
	{
		for (int i = (int)(OBJECTLAYER::LAYER0); i < (int)OBJECTLAYER::END; ++i)
		{
			OBJECTLAYER Layer = (OBJECTLAYER)i;

			bool isSelected = (Layer == m_LayerIdx);

			if (ImGui::Selectable(LayerToString(Layer).c_str(), isSelected))
			{
				m_LayerIdx = Layer;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();

		if (PrevIdx != (int)m_LayerIdx)
		{
			CGameObject* pTarget = GetTargetObject();
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(pTarget, (int)m_LayerIdx);
		}
	}

	GetTargetObject()->SetLayerIdx((int)m_LayerIdx);
}


string TransformUI::LayerToString(OBJECTLAYER _layer)
{
	switch (_layer)
	{
	default:
		return "Layer" + std::to_string((int)_layer - (int)OBJECTLAYER::LAYER0);
	}
}