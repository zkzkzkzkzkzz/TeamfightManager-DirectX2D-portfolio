#include "pch.h"
#include "CameraUI.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>


CameraUI::CameraUI()
	: ComponentUI("Camera", "##Camera", COMPONENT_TYPE::CAMERA)
	, m_LayerIdx(CAMERALAYER::EVERYTHING)
{
	SetSize(ImVec2(0.f, 300.f));
	SetComponentTitle("Camera");
}

CameraUI::~CameraUI()
{
}

void CameraUI::render_update()
{
	ComponentUI::render_update();

	static int ProjType = int(GetTargetObject()->Camera()->GetProjType());
	float fov = GetTargetObject()->Camera()->GetFOV();
	float scale = GetTargetObject()->Camera()->GetScale();
	float fFar = GetTargetObject()->Camera()->GetFar();

	Vec3 vRot = GetTargetObject()->Transform()->GetRelativeRotation();
	vRot.ToDegree();


	if (scale <= 0.f)
	{
		scale = 0.f;
	}

	ImGui::Text("PROJ TYPE");
	ImGui::SameLine(0, 30); ImGui::PushItemWidth(150);
	ImGui::Combo("##ProjType", &ProjType, "ORTHOGRAPHIC\0PERSPECTIVE\0\0");

	if (ProjType == 0)
	{
		vRot = Vec3(0.f, 0.f, 0.f);
	}

	vRot.ToRadian();
	GetTargetObject()->Transform()->SetRelativeRotation(vRot);

	// ===================================================================

	if (ProjType == 0)
		ImGui::BeginDisabled();

	ImGui::Text("FOV");
	ImGui::SameLine(0, 72); ImGui::PushItemWidth(150);
	ImGui::SliderAngle("##FOV", &fov, 0.f, 360.f);

	if (ProjType == 0)
		ImGui::EndDisabled();

	// ===================================================================

	if (ProjType == 1)
		ImGui::BeginDisabled();

	ImGui::Text("Scale");
	ImGui::SameLine(0, 58); ImGui::PushItemWidth(150);
	ImGui::DragFloat("##CameraScale", &scale);

	if (ProjType == 1)
		ImGui::EndDisabled();

	// ===================================================================

	ImGui::Text("Far");
	ImGui::SameLine(0, 72); ImGui::PushItemWidth(150);
	ImGui::DragFloat("##CameraFar", &fFar);

	// ===================================================================

	CAMERALAYER selectedlayer = m_LayerIdx;

	ImGui::Text("Layer");
	ImGui::SameLine(0, 58); ImGui::PushItemWidth(150);
	if(ImGui::BeginCombo("##CameraLayer", LayerToString(selectedlayer).c_str()))
	{
		for (int i = (int)(CAMERALAYER::NONE); i < (int)CAMERALAYER::END; ++i)
		{
			CAMERALAYER cLayer = (CAMERALAYER)i;

			bool isSelected = (cLayer == selectedlayer);

			if (ImGui::Selectable(LayerToString(cLayer).c_str(), isSelected))
			{
				selectedlayer = cLayer;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	SetLayerIdx(selectedlayer);

	switch ((int)selectedlayer)
	{
	case 0:
		GetTargetObject()->Camera()->AllLayerOff();
		break;
	case 1:
		GetTargetObject()->Camera()->LayerCheckAll();
		break;
	default:
		GetTargetObject()->Camera()->AllLayerOff();
		GetTargetObject()->Camera()->LayerCheck((int)selectedlayer - 2, true);
		break;
	}

	GetTargetObject()->Camera()->SetProjType((PROJ_TYPE)ProjType);
	GetTargetObject()->Camera()->SetFOV(fov);
	GetTargetObject()->Camera()->SetScale(scale);
	GetTargetObject()->Camera()->SetFar(fFar);
}

string CameraUI::LayerToString(CAMERALAYER _layer)
{
	switch (_layer)
	{
	case CAMERALAYER::NONE:
		return "None";
	case CAMERALAYER::EVERYTHING:
		return "Everything";
	default:
		return "Layer" + std::to_string((int)_layer - (int)CAMERALAYER::LAYER0);
	}
}


//PROJ_TYPE   m_ProjType;     // 투영 방식
//// 원근 투영(Perspective)
//float       m_FOV;          // 시야각
//// 직교 투영(Orthographic)
//float       m_Width;        // 직교투영 가로 길이
//float       m_Scale;        // 직교투영 배율
//// Both (둘 다 해당되는 멤버)
//float       m_AspectRatio;  // 종횡비, 투영 가로세로 비율
//float       m_Far;          // 투영 최대 거리