#include "pch.h"
#include "EditAnimator.h"

#include "imgui_internal.h"

#include <Engine\CAnimator2D.h>
#include <Engine\CAnim.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CTexture.h>
#include <fstream>
#include <Engine\func.h>

// SimpleMath
#include <Engine\SimpleMath.h>
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


EditAnimator::EditAnimator()
	: UI("Edit Animation", "##EditAnimation")
	, m_pAtlasTex(nullptr)
	, m_textureID(nullptr)
	, m_imTexId(nullptr)
	, m_ImageSize(0, 0)
	, m_width(1.f)
	, m_height(1.f)
	, m_IsPlaying(false)
	, m_AccTime(0.f)
	, m_CurFrame(0)
	, m_Loop(true)
{
	Deactivate();
	SetModal(true);
}

EditAnimator::~EditAnimator()
{
}

void EditAnimator::render_update()
{
	if (nullptr == m_TargetObject)
		return;

	// 스프라이트 띄우기
	if (ImGui::Button("Select Sprite"))
	{
		OpenFildDialog();
	}

	float Childwidth = 600;
	float Childheight = 520;

	float tapheight = ImGui::GetTextLineHeightWithSpacing();
	float scrollwidth = ImGui::GetStyle().ScrollbarSize;

	ImGui::BeginChild("Sprite View", ImVec2(Childwidth + scrollwidth, Childheight + tapheight), true, ImGuiWindowFlags_HorizontalScrollbar);

	static float zoomFactor = 1.f;

	if (ImGui::GetIO().MouseWheel != 0.f && ImGui::GetIO().KeyCtrl)
	{
		zoomFactor += ImGui::GetIO().MouseWheel * 0.1f;
		zoomFactor = max(0.1f, min(zoomFactor, 2.f));
	}


	if (m_textureID != nullptr)
	{
		float width = m_pAtlasTex->GetWidth();
		float height = m_pAtlasTex->GetHeight();

		m_ImageSize = ImVec2(width * zoomFactor, height * zoomFactor);
		ImGui::Image(m_imTexId, m_ImageSize);
	}

	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("Animation Editor", ImVec2(Childwidth + scrollwidth, Childheight + tapheight), true);

	static ImVec2 selectedRegion = ImVec2(0.f, 0.f);
	static ImVec2 sliceSize = ImVec2(0.f, 0.f);
	static ImVec2 offset = ImVec2(0.f, 0.f);
	static ImVec2 vBackground = ImVec2(0.f, 0.f);
	static float Duration = 0.f;

	ImGui::Text("LeftTop");
	ImGui::SameLine(0, 31); ImGui::PushItemWidth(120.f);
	ImGui::InputFloat2("##vLeftTop", &selectedRegion.x, "%.0f");

	ImGui::Text("SliceSize");
	ImGui::SameLine(0, 17); ImGui::PushItemWidth(120.f);
	ImGui::InputFloat2("##SliceSize", &sliceSize.x, "%.0f");

	ImGui::Text("Offset");
	ImGui::SameLine(0, 38); ImGui::PushItemWidth(120.f);
	ImGui::InputFloat2("##OffsetPos", &offset.x, "%.0f");
	ImGui::SameLine();
	// 상 버튼
	if (ImGui::ArrowButton("##Top", ImGuiDir_Up))
	{
		offset.y -= 1.0f;
	}
	// 하 버튼
	ImGui::SameLine();
	if (ImGui::ArrowButton("##Bottom", ImGuiDir_Down))
	{
		offset.y += 1.0f;
	}
	// 좌 버튼
	ImGui::SameLine();
	if (ImGui::ArrowButton("##Left", ImGuiDir_Left))
	{
		offset.x -= 1.0f;
	}
	// 우 버튼
	ImGui::SameLine();
	if (ImGui::ArrowButton("##Right", ImGuiDir_Right))
	{
		offset.x += 1.0f;
	}

	ImGui::Text("Background");
	ImGui::SameLine(0, 10); ImGui::PushItemWidth(120.f);
	ImGui::InputFloat2("##Background", &vBackground.x, "%.0f");

	ImGui::Text("Duration");
	ImGui::SameLine(0, 24); ImGui::PushItemWidth(80.f);
	ImGui::InputFloat("##Duration", &Duration, 0.f, 0.f, "%.2f");

	ImGui::Text("Anim Name");
	static char AnimName[200] = {};
	ImGui::SameLine(0, 17); ImGui::PushItemWidth(120.f);
	ImGui::InputText("##AnimName", AnimName, 128);


	if (ImGui::Button("Add Frame"))
	{
		tAnimFrm frm = {};

		frm.vLeftTop = ImVec2ToVector2(selectedRegion);
		frm.vSlice = ImVec2ToVector2(sliceSize);
		frm.vOffset = ImVec2ToVector2(offset);
		frm.vBackground = ImVec2ToVector2(vBackground);
		frm.Duration = Duration;

		m_vecAnimFrm.push_back(frm);
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete Frame"))
	{
		if (!m_vecAnimFrm.empty())
		{
			m_vecAnimFrm.pop_back();
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Play Animation"))
	{
		m_IsPlaying = true;
		m_AccTime = 0.f;
		m_CurFrame = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop Animation"))
	{
		m_IsPlaying = false;
		m_AccTime = 0.f;
		m_CurFrame = 0;
	}

	if (ImGui::Button("Clear Animation"))
	{
		m_vecAnimFrm.clear();
	}

	ImGui::SameLine();
	ImGui::Checkbox("Repeat Animation", &m_Loop);

	if (m_IsPlaying && !m_vecAnimFrm.empty())
	{
		m_AccTime += ImGui::GetIO().DeltaTime;

		// 현재 프레임의 Duration 이상 경과하면 다음 프레임으로 이동
		if (m_AccTime >= m_vecAnimFrm[m_CurFrame].Duration)
		{
			m_AccTime = 0.0f;
			m_CurFrame += 1;
		
			if (m_CurFrame >= (int)m_vecAnimFrm.size())
			{
				if (m_Loop)
				{
					m_CurFrame = 0;
				}
				else
				{
					m_IsPlaying = false;
				}
			}
		}
	}


	ImGui::BeginChild("Selected Region", ImVec2(vBackground.x +scrollwidth, vBackground.y + tapheight), true, ImGuiWindowFlags_NoScrollbar);
	ImVec2 uv0 = ImVec2(selectedRegion.x / m_width, selectedRegion.y / m_height);
	ImVec2 uv1 = ImVec2((selectedRegion.x + sliceSize.x) / m_width, (selectedRegion.y + sliceSize.y) / m_height);
	ImVec2 regionSize = sliceSize;
	ImGui::Text("UV0: %.2f, %.2f", uv0.x, uv0.y);
	ImGui::Text("UV1: %.2f, %.2f", uv1.x, uv1.y);

	// 중앙 정렬을 위한 계산
	float offsetX = (ImGui::GetWindowSize().x - regionSize.x)* 0.5f;
	float offsetY = (ImGui::GetWindowSize().y - regionSize.y) * 0.5f;
	ImVec2 regionPos = ImVec2(offsetX + scrollwidth / 2, offsetY - tapheight / 2);

	regionPos.x += offset.x;
	regionPos.y += offset.y;

	// 이미지 중앙에 정렬하여 출력
	ImGui::SetCursorPos(regionPos);

	ImGui::Image(m_imTexId, regionSize, uv0, uv1);

	ImGui::EndChild();

	ImGui::SameLine();
	if (ImGui::Button("Save Animation"))
	{
		m_AnimName = ToWString(AnimName);
		GetTargetObject()->Animator2D()->CreateFrm(m_AnimName, m_pAtlasTex, m_vecAnimFrm);
		GetTargetObject()->Animator2D()->SaveAnimations(L"animdata");
		MessageBox(nullptr, L"Animation Saved", L"Animation Saved", MB_OK);
	}

	ImGui::EndChild();

	if (!m_vecAnimFrm.empty())
	{
		for (const auto& frame : m_vecAnimFrm)
		{
			ImGui::Text("LeftTop: %.0f, %.0f", frame.vLeftTop.x, frame.vLeftTop.y); ImGui::SameLine();
			ImGui::Text("Slice: %.0f, %.0f", frame.vSlice.x, frame.vSlice.y); ImGui::SameLine();
			ImGui::Text("Offset: %.0f, %.0f", frame.vOffset.x, frame.vOffset.y); ImGui::SameLine();
			ImGui::Text("Background: %.0f, %.0f", frame.vBackground.x, frame.vBackground.y); ImGui::SameLine();
			ImGui::Text("Duration: %.2f", frame.Duration);
			ImGui::Separator();
		}
	}

	
	if (m_IsPlaying && !m_vecAnimFrm.empty())
	{
		selectedRegion = ImVec2(m_vecAnimFrm[m_CurFrame].vLeftTop.x, m_vecAnimFrm[m_CurFrame].vLeftTop.y);
		sliceSize = ImVec2(m_vecAnimFrm[m_CurFrame].vSlice.x, m_vecAnimFrm[m_CurFrame].vSlice.y);
		offset = ImVec2(m_vecAnimFrm[m_CurFrame].vOffset.x, m_vecAnimFrm[m_CurFrame].vOffset.y);
		vBackground = ImVec2(m_vecAnimFrm[m_CurFrame].vBackground.x, m_vecAnimFrm[m_CurFrame].vBackground.y);
		Duration = m_vecAnimFrm[m_CurFrame].Duration;
	}
}

void EditAnimator::Deactivate()
{
	UI::Deactivate();

	m_IsPlaying = false;
	m_AccTime = 0.f;
	m_CurFrame = 0;

	ImGui::SetWindowFocus(nullptr);
}



void EditAnimator::SetTaretObject(CGameObject* _Object)
{
	m_TargetObject = _Object;
}

void EditAnimator::OpenFildDialog()
{
	OPENFILENAME ofn;
	wchar_t szFile[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile) / sizeof(*szFile);
	ofn.lpstrFilter = L"All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		const wstring filePath = szFile;
		LoadSpriteFromFile(filePath);
	}
}

void EditAnimator::LoadSpriteFromFile(const wstring& filePath)
{
	wstring fileName = GetFileFromPath(filePath);
	wstring strkey = GetFileNameFromPath(filePath);

	m_pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(strkey, fileName);

	if (m_pAtlasTex != nullptr)
	{
		m_width = (float)(m_pAtlasTex->GetWidth());
		m_height = (float)(m_pAtlasTex->GetHeight());

		m_textureID = m_pAtlasTex->GetSRV();
		m_imTexId = (ImTextureID)m_textureID.Get();
	}
}

wstring EditAnimator::GetFileFromPath(const wstring& filePath)
{
	size_t found = filePath.find(L"content\\");

	if (found != wstring::npos)
	{
		return filePath.substr(found + 8);
	}

	return filePath;
}

wstring EditAnimator::GetFileNameFromPath(const wstring& filePath)
{
	size_t found = filePath.find_last_of(L"\\/");

	if (found != std::wstring::npos)
	{
		return filePath.substr(found + 1);
	}
	return filePath;
}
