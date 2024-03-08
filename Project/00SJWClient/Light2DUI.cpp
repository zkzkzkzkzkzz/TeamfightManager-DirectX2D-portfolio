#include "pch.h"
#include "Light2DUI.h"

#include <Engine\CLight2D.h>

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0.f, 405.f));
	SetComponentTitle("Light2D");
}

Light2DUI::~Light2DUI()
{
}
 
void Light2DUI::render_update()
{
	ComponentUI::render_update();

	tLightInfo info = GetTargetObject()->Light2D()->GetLightInfo();

	static int LightType = (int)info.LightType;
	Vec3 vColor = info.vColor;
	Vec3 vAmbient = info.vAmbient;
	Vec3 vWorldDir = info.vWorldDir;
	float fRadius = info.fRadius;
	float fAngle = info.fAngle;

	static float Dir = 1.f;
	vWorldDir = AngleToVector(Dir);

	
	ImGui::Text("Light Type");
	ImGui::SameLine(0, 20); ImGui::PushItemWidth(150);
	ImGui::Combo("##LightType", &LightType, "DIRECTIONAL\0POINT\0SPOT\0\0");

	if (LightType == 0)
		ImGui::BeginDisabled();

	ImGui::Text("Light Color");
	ImGui::SameLine(0, 13); ImGui::PushItemWidth(200);
	ImGui::ColorPicker3("##LightColor", vColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

	if (LightType == 0)
		ImGui::EndDisabled();

	// ==================================================

	if (LightType == 1 || LightType == 2)
		ImGui::BeginDisabled();

	ImGui::Text("Ambient");
	ImGui::SameLine(0, 41); ImGui::PushItemWidth(250);
	ImGui::ColorEdit3("##LightAmbient", vAmbient);

	if (LightType == 1 || LightType == 2)
		ImGui::EndDisabled();

	// ==================================================

	if (LightType == 0 || LightType == 1)
		ImGui::BeginDisabled();

	ImGui::Text("LightDir");
	ImGui::SameLine(0, 34); ImGui::PushItemWidth(200);
	ImGui::SliderAngle("##LightDir", &Dir, 0.f, 360.f);

	ImGui::Text("Angle");
	ImGui::SameLine(0, 55); ImGui::PushItemWidth(200);
	ImGui::SliderAngle("##LightAngle", &fAngle, 0.f, 360.f);

	if (LightType == 0 || LightType == 1)
		ImGui::EndDisabled();

	// ==================================================

	if (LightType == 0)
		ImGui::BeginDisabled();

	ImGui::Text("Radius");
	ImGui::SameLine(0, 48); ImGui::PushItemWidth(200);
	ImGui::DragFloat("##LightRadius", &fRadius);

	if (LightType == 0)
		ImGui::EndDisabled();

	GetTargetObject()->Light2D()->SetLightType((LIGHT_TYPE)LightType);
	GetTargetObject()->Light2D()->SetLightColor(vColor);
	GetTargetObject()->Light2D()->SetAmbient(vAmbient);
	GetTargetObject()->Light2D()->SetDir(vWorldDir);
	GetTargetObject()->Light2D()->SetRadius(fRadius);
	GetTargetObject()->Light2D()->SetAngle(fAngle);
}


Vec3 Light2DUI::AngleToVector(float _angle)
{
	float x = cosf(_angle);
	float y = sinf(_angle);

	return Vec3(x, y, 0.f);
}



//Vec4	vColor;		// 광원의 순수 색상
//Vec4	vSpecular;	// 반사광 - 광원이 물체 표면에 반사된 색상
//Vec4	vAmbient;	// 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛

//Vec3	vWorldPos;	// 광원의 위치
//Vec3	vWorldDir;	// 광원이 향하는 방향
//float	fRadius;	// 광원의 영향 반경, 거리 정보
//float	fAngle;		// 광원의 범위 각도

//int	LightType;	// 광원 타입