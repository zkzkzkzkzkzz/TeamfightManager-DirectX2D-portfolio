#include "pch.h"
#include "CBansSlotScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include "CChampSlotScript.h"

CBansSlotScript::CBansSlotScript()
	: CScript(BANSSLOTSCRIPT)
	, m_BanSlot(nullptr)
	, m_Champ(nullptr)
	, m_Text(nullptr)
	, m_Level(nullptr)
	, m_IsSpawn(false)
{
}

CBansSlotScript::CBansSlotScript(const CBansSlotScript& _Origin)
	: CScript(BANSSLOTSCRIPT)
	, m_BanSlot(nullptr)
	, m_Champ(nullptr)
	, m_Text(nullptr)
	, m_Level(nullptr)
	, m_IsSpawn(false)
{
}

CBansSlotScript::~CBansSlotScript()
{
}

void CBansSlotScript::begin()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);

	m_Level = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();
}

void CBansSlotScript::tick()
{
	if (L"BanSlot1" == GetOwner()->GetName() && nullptr != m_BanSlot && !m_IsSpawn)
	{
		CHAMP_LIST list = m_BanSlot->GetScript<CChampSlotScript>()->GetChampList();

		SetChampToBanSlot();

		switch (list)
		{
		case CHAMP_LIST::ARCHER:
			break;
		case CHAMP_LIST::FIGHTER:
			m_Champ->Transform()->SetRelativePos(Vec3(0.f, 13.f, -10.f));
			m_Champ->Transform()->SetRelativeScale(Vec3(45.f, 65.f, 1.f));
			m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
			m_Text->TextRender()->SetString(L"Fighter");
			m_Text->TextRender()->SetOffsetPos(Vec3(-28.f, 26.f, -10.f));
			break;
		case CHAMP_LIST::KNIGHT:
			break;
		case CHAMP_LIST::MONK:
			break;
		case CHAMP_LIST::NINJA:
			break;
		case CHAMP_LIST::PRIEST:
			m_Champ->Transform()->SetRelativePos(Vec3(0.f, 13.f, -10.f));
			m_Champ->Transform()->SetRelativeScale(Vec3(40.f, 65.f, 1.f));
			m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 5);
			m_Text->TextRender()->SetString(L"Priest");
			m_Text->TextRender()->SetOffsetPos(Vec3(-25.f, 26.f, -10.f));
			break;
		case CHAMP_LIST::END:
			break;
		default:
			break;
		}
	}
	else if (L"BanSlot2" == GetOwner()->GetName() && nullptr != m_BanSlot && !m_IsSpawn)
	{
		CHAMP_LIST list = m_BanSlot->GetScript<CChampSlotScript>()->GetChampList();

		SetChampToBanSlot();

		switch (list)
		{
		case CHAMP_LIST::ARCHER:
			break;
		case CHAMP_LIST::FIGHTER:
			m_Champ->Transform()->SetRelativePos(Vec3(0.f, 13.f, -10.f));
			m_Champ->Transform()->SetRelativeScale(Vec3(45.f, 65.f, 1.f));
			m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
			m_Text->TextRender()->SetString(L"Fighter");
			m_Text->TextRender()->SetOffsetPos(Vec3(-28.f, 26.f, -10.f));
			break;
		case CHAMP_LIST::KNIGHT:
			break;
		case CHAMP_LIST::MONK:
			break;
		case CHAMP_LIST::NINJA:
			break;
		case CHAMP_LIST::PRIEST:
			m_Champ->Transform()->SetRelativePos(Vec3(0.f, 13.f, -10.f));
			m_Champ->Transform()->SetRelativeScale(Vec3(40.f, 65.f, 1.f));
			m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 5);
			m_Text->TextRender()->SetString(L"Priest");
			m_Text->TextRender()->SetOffsetPos(Vec3(-25.f, 26.f, -10.f));
			break;
		case CHAMP_LIST::END:
			break;
		default:
			break;
		}
	}
}


void CBansSlotScript::SetChampToBanSlot()
{
	m_Champ = new CGameObject;
	m_Champ->SetName(L"BanChamp");
	m_Champ->AddComponent(new CTransform);
	m_Champ->AddComponent(new CMeshRender);
	m_Champ->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Champ->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BanPickChampIcon.mtrl"));
	GetOwner()->AddChild(m_Champ);

	m_Text = new CGameObject;
	m_Text->AddComponent(new CTransform);
	m_Text->AddComponent(new CTextRender);
	m_Text->TextRender()->SetFont(L"Galmuri14");
	m_Text->TextRender()->SetFontSize(15.f);
	m_Text->TextRender()->SetFontColor(255, 255, 255, 255);
	GetOwner()->AddChild(m_Text);

	m_IsSpawn = true;
}
