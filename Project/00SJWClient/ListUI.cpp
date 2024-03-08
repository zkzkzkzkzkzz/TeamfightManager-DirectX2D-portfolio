#include "pch.h"
#include "ListUI.h"


ListUI::ListUI()
	: UI("", "##List")
	, m_CallBackFunc(nullptr)
{
	Deactivate();
	SetModal(true);
}

ListUI::~ListUI()
{
}


void ListUI::render_update()
{
	static int item_current_idx = 0;

	if (ImGui::BeginListBox("##ListBox", ImVec2(-FLT_MIN, 0)))
	{
        for (int i = 0; i < m_vecStr.size(); ++i)
        {
            const bool is_selected = (item_current_idx == i);

            if (ImGui::Selectable(m_vecStr[i].c_str(), is_selected))
            {
                item_current_idx = i;
            }

            // 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }

            // 리스트 항목 중 더블클릭이 발생한다면            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                m_strDBClicked = m_vecStr[i];

                // 등록된 콜백 함수가 있으면 호출
                if (nullptr != m_CallBackFunc)
                {
                    m_CallBackFunc((DWORD_PTR)m_strDBClicked.c_str());
                }

                // 등록된 델리게이트 함수가 있으면 호출
                if (m_pUI && m_Func)
                {
                    (m_pUI->*m_Func)((DWORD_PTR)m_strDBClicked.c_str());
                }

                Deactivate();
            }

        }
        ImGui::EndListBox();
	}
}

void ListUI::Deactivate()
{
    UI::Deactivate();

    m_vecStr.clear();

    ImGui::SetWindowFocus(nullptr);
}

