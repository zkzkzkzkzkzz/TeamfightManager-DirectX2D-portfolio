#include "pch.h"
#include "CChampSlotScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include "CEffectScript.h"

CChampSlotScript::CChampSlotScript()
	: CScript(CHAMPSLOTSCRIPT)
	, m_list(CHAMP_LIST::END)
	, m_Champ(nullptr)
	, m_TeamColor(TEAM::NONE)
	, m_SlotState(SLOT_STATE::NONE)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_IsSelected(false)
	, m_Level(nullptr)
	, m_EffectObj(nullptr)
	, m_EffectSpawn(false)
	, m_EffectTime(0.f)
	, m_ToolTip(nullptr)
{
}

CChampSlotScript::CChampSlotScript(const CChampSlotScript& _Origin)
	: CScript(CHAMPSLOTSCRIPT)
	, m_list(CHAMP_LIST::END)
	, m_Champ(nullptr)
	, m_TeamColor(TEAM::NONE)
	, m_SlotState(SLOT_STATE::NONE)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_IsSelected(false)
	, m_Level(nullptr)
	, m_EffectObj(nullptr)
	, m_EffectSpawn(false)
	, m_EffectTime(0.f)
	, m_ToolTip(nullptr)
{
}

CChampSlotScript::~CChampSlotScript()
{
}


void CChampSlotScript::begin()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	m_Level = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();

	InitEffectObj();

	InitChampToSlot();
}

void CChampSlotScript::tick()
{
	m_EffectTime += DT;

	CheckMousePos();
}


void CChampSlotScript::CheckMousePos()
{
	m_bMouseOn_Prev = m_bMouseOn;

	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	vMousePos.x -= vResolution.x / 2.f;
	vMousePos.y = -(vMousePos.y - vResolution.y / 2.f);

	Vec3 vWorldPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vWorldScale = GetOwner()->Transform()->GetRelativeScale();

	Vec2 vLT = Vec2(vWorldPos.x - vWorldScale.x / 2, vWorldPos.y - vWorldScale.y / 2);
	Vec2 vRB = Vec2(vWorldPos.x + vWorldScale.x / 2, vWorldPos.y + vWorldScale.y / 2);

	if (vLT.x < vMousePos.x && vMousePos.x < vRB.x
		&& vLT.y < vMousePos.y && vMousePos.y < vRB.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}

	bool bLBtnTap = KEY_TAP(LBTN);
	bool bLbtnReleased = KEY_RELEASED(LBTN);

	if (m_bMouseOn)
	{
		if (m_bMouseOn_Prev != m_bMouseOn)
			OnHovered();

		if (bLbtnReleased)
		{
			if (m_bMouseLBtnDown)
			{
				LBtnClicked();
			}
		}
		else if (bLBtnTap)
		{
			m_bMouseLBtnDown = true;
		}

		if (bLbtnReleased)
			m_bMouseLBtnDown = false;
	}
	else
	{
		if (m_bMouseOn_Prev != m_bMouseOn)
			OnUnHovered();
	}
}

void CChampSlotScript::OnHovered()
{
	if ((BANPICK_STATE::BLUEPICK1 == m_Level->GetCurBanPickState() || BANPICK_STATE::BLUEPICK2 == m_Level->GetCurBanPickState())
		&& SLOT_STATE::NONE == m_SlotState)
	{
		m_EffectObj->Animator2D()->Play(L"ChampSlotAnim");
		ShowChampInfo();

		if (nullptr != m_ToolTip)
		{
			m_ToolTip->SetActive(true);
			for (size_t i = 0; i < m_ToolTip->GetChild().size(); ++i)
			{
				m_ToolTip->GetChild()[i]->SetActive(true);
			}
		}
	}
}

void CChampSlotScript::OnUnHovered()
{
	m_EffectObj->Animator2D()->Stop();
	m_EffectObj->Animator2D()->Play(L"ChampSlotIdle");

	if (nullptr != m_ToolTip)
	{
		m_ToolTip->SetActive(false);
		for (size_t i = 0; i < m_ToolTip->GetChild().size(); ++i)
		{
			m_ToolTip->GetChild()[i]->SetActive(false);
		}
	}
}

void CChampSlotScript::LBtnClicked()
{
	if ((BANPICK_STATE::BLUEPICK1 == m_Level->GetCurBanPickState() || BANPICK_STATE::BLUEPICK2 == m_Level->GetCurBanPickState())
		&& SLOT_STATE::NONE == m_SlotState)
	{
		m_EffectObj->Animator2D()->Stop();
		m_EffectObj->Animator2D()->Play(L"ChampSlotIdle");
		m_Level->SetBanPickState(BANPICK_STATE::REDPICK1);
	}
	else if ((BANPICK_STATE::BLUEBAN == m_Level->GetCurBanPickState() || BANPICK_STATE::REDBAN == m_Level->GetCurBanPickState())
		&& SLOT_STATE::NONE == m_SlotState)
	{
		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale(), Transform()->GetRelativeRotation()
					, L"BanAnim", 0.2, false, Vec3(0.f, -25.f, -10.f));
	}
}

void CChampSlotScript::SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat, Vec3 _offset)
{
	CGameObject* effect = new CGameObject;
	effect->AddComponent(new CTransform);
	effect->AddComponent(new CMeshRender);
	effect->AddComponent(new CAnimator2D);
	effect->AddComponent(new CEffectScript);

	_Pos.x += _offset.x;
	_Pos.y += _offset.y;
	_Pos.z += _offset.z;

	GETEFFECT(effect)->SetEffectInfo(_Pos, _Scale, _Rotation, _anim, _time, _repeat);
	GamePlayStatic::SpawnGameObject(effect, 6);
}

void CChampSlotScript::InitEffectObj()
{
	m_EffectObj = new CGameObject;
	m_EffectObj->SetName(L"SlotAnimObj");
	m_EffectObj->AddComponent(new CTransform);
	m_EffectObj->AddComponent(new CMeshRender);
	m_EffectObj->AddComponent(new CAnimator2D);

	Vec3 vPos = Transform()->GetRelativePos();

	m_EffectObj->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y - 25.f, 1500.f));
	m_EffectObj->Transform()->SetRelativeScale(Transform()->GetRelativeScale());
	m_EffectObj->Transform()->SetRelativeRotation(Transform()->GetRelativeRotation());
	m_EffectObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_EffectObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffectMtrl"));
	m_EffectObj->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_EffectObj->Animator2D()->LoadAnimation(L"animdata\\ChampSlotIdle.txt");
	m_EffectObj->Animator2D()->LoadAnimation(L"animdata\\ChampSlotAnim.txt");
	m_EffectObj->Animator2D()->Play(L"ChampSlotIdle");
	GamePlayStatic::SpawnGameObject(m_EffectObj, 6);
}

void CChampSlotScript::InitChampToSlot()
{
	CGameObject* text = new CGameObject;

	switch (m_list)
	{
	case CHAMP_LIST::ARCHER:
		CreateChamp();
		m_Champ->SetName(L"Slot_Archer");
		m_Champ->Transform()->SetRelativePos(Vec3(0.f, 11.f, -10.f));
		m_Champ->Transform()->SetRelativeScale(Vec3(60.f, 75.f, 1.f));
		m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

		text->AddComponent(new CTransform);
		text->AddComponent(new CTextRender);
		text->TextRender()->SetString(L"Archer");
		text->TextRender()->SetFont(L"Galmuri14");
		text->TextRender()->SetFontSize(15.f);
		text->TextRender()->SetFontColor(255, 255, 255, 255);
		text->TextRender()->SetOffsetPos(Vec3(-28.f, 29.f, 0.f));
		GetOwner()->AddChild(text);

		m_ToolTip = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ToolTip_Archer_2.prefab")->Instatiate();
		m_ToolTip->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		GamePlayStatic::SpawnGameObject(m_ToolTip, 2);
		m_ToolTip->SetActive(false);
		for (size_t i = 0; i < m_ToolTip->GetChild().size(); ++i)
		{
			if (L"Champ_SkillIcon" == m_ToolTip->GetChild()[i]->GetName()
				|| L"Champ_UltiIcon" == m_ToolTip->GetChild()[i]->GetName())
			{
				m_ToolTip->GetChild()[i]->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
			}

			m_ToolTip->GetChild()[i]->SetActive(false);
		}
		break;
	case CHAMP_LIST::FIGHTER:
		CreateChamp();
		m_Champ->SetName(L"Slot_Fighter");
		m_Champ->Transform()->SetRelativePos(Vec3(0.f, 12.f, -10.f));
		m_Champ->Transform()->SetRelativeScale(Vec3(50.f, 75.f, 1.f));
		m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

		text->AddComponent(new CTransform);
		text->AddComponent(new CTextRender);
		text->TextRender()->SetString(L"Fighter");
		text->TextRender()->SetFont(L"Galmuri14");
		text->TextRender()->SetFontSize(15.f);
		text->TextRender()->SetFontColor(255, 255, 255, 255);
		text->TextRender()->SetOffsetPos(Vec3(-28.f, 29.f, 0.f));
		GetOwner()->AddChild(text);

		m_ToolTip = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ToolTip_Fighter_2.prefab")->Instatiate();
		m_ToolTip->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		GamePlayStatic::SpawnGameObject(m_ToolTip, 2);
		m_ToolTip->SetActive(false);
		for (size_t i = 0; i < m_ToolTip->GetChild().size(); ++i)
		{
			if (L"Champ_SkillIcon" == m_ToolTip->GetChild()[i]->GetName() 
				|| L"Champ_UltiIcon" == m_ToolTip->GetChild()[i]->GetName())
			{
				m_ToolTip->GetChild()[i]->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
			}

			m_ToolTip->GetChild()[i]->SetActive(false);
		}
		break;
	case CHAMP_LIST::KNIGHT:
		CreateChamp();
		m_Champ->SetName(L"Slot_Knight");
		m_Champ->Transform()->SetRelativePos(Vec3(0.f, 13.f, -10.f));
		m_Champ->Transform()->SetRelativeScale(Vec3(80.f, 77.f, 1.f));
		m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);

		text->AddComponent(new CTransform);
		text->AddComponent(new CTextRender);
		text->TextRender()->SetString(L"Knight");
		text->TextRender()->SetFont(L"Galmuri14");
		text->TextRender()->SetFontSize(15.f);
		text->TextRender()->SetFontColor(255, 255, 255, 255);
		text->TextRender()->SetOffsetPos(Vec3(-25.f, 29.f, 0.f));
		GetOwner()->AddChild(text);

		m_ToolTip = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ToolTip_Knight_2.prefab")->Instatiate();
		m_ToolTip->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);
		GamePlayStatic::SpawnGameObject(m_ToolTip, 2);
		m_ToolTip->SetActive(false);
		for (size_t i = 0; i < m_ToolTip->GetChild().size(); ++i)
		{
			if (L"Champ_SkillIcon" == m_ToolTip->GetChild()[i]->GetName()
				|| L"Champ_UltiIcon" == m_ToolTip->GetChild()[i]->GetName())
			{
				m_ToolTip->GetChild()[i]->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 2);
			}

			m_ToolTip->GetChild()[i]->SetActive(false);
		}
		break;
	case CHAMP_LIST::MONK:
		CreateChamp();
		m_Champ->SetName(L"Slot_Monk");
		m_Champ->Transform()->SetRelativePos(Vec3(0.f, 13.f, -10.f));
		m_Champ->Transform()->SetRelativeScale(Vec3(50.f, 75.f, 1.f));
		m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 3);

		text->AddComponent(new CTransform);
		text->AddComponent(new CTextRender);
		text->TextRender()->SetString(L"Monk");
		text->TextRender()->SetFont(L"Galmuri14");
		text->TextRender()->SetFontSize(15.f);
		text->TextRender()->SetFontColor(255, 255, 255, 255);
		text->TextRender()->SetOffsetPos(Vec3(-18.f, 29.f, -10.f));
		GetOwner()->AddChild(text);

		m_ToolTip = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ToolTip_Monk_2.prefab")->Instatiate();
		m_ToolTip->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 3);
		GamePlayStatic::SpawnGameObject(m_ToolTip, 2);
		m_ToolTip->SetActive(false);
		for (size_t i = 0; i < m_ToolTip->GetChild().size(); ++i)
		{
			if (L"Champ_SkillIcon" == m_ToolTip->GetChild()[i]->GetName()
				|| L"Champ_UltiIcon" == m_ToolTip->GetChild()[i]->GetName())
			{
				m_ToolTip->GetChild()[i]->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 3);
			}

			m_ToolTip->GetChild()[i]->SetActive(false);
		}
		break;
	case CHAMP_LIST::NINJA:
		CreateChamp();
		m_Champ->SetName(L"Slot_Ninja");
		m_Champ->Transform()->SetRelativePos(Vec3(-5.f, 9.f, -10.f));
		m_Champ->Transform()->SetRelativeScale(Vec3(65.f, 70.f, 1.f));
		m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 4);

		text->AddComponent(new CTransform);
		text->AddComponent(new CTextRender);
		text->TextRender()->SetString(L"Ninja");
		text->TextRender()->SetFont(L"Galmuri14");
		text->TextRender()->SetFontSize(15.f);
		text->TextRender()->SetFontColor(255, 255, 255, 255);
		text->TextRender()->SetOffsetPos(Vec3(-18.f, 29.f, -10.f));
		GetOwner()->AddChild(text);

		m_ToolTip = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ToolTip_Ninja_2.prefab")->Instatiate();
		m_ToolTip->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 4);
		GamePlayStatic::SpawnGameObject(m_ToolTip, 2);
		m_ToolTip->SetActive(false);
		for (size_t i = 0; i < m_ToolTip->GetChild().size(); ++i)
		{
			if (L"Champ_SkillIcon" == m_ToolTip->GetChild()[i]->GetName()
				|| L"Champ_UltiIcon" == m_ToolTip->GetChild()[i]->GetName())
			{
				m_ToolTip->GetChild()[i]->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 4);
			}

			m_ToolTip->GetChild()[i]->SetActive(false);
		}
		break;
	case CHAMP_LIST::PRIEST:
		CreateChamp();
		m_Champ->SetName(L"Slot_Priest");
		m_Champ->Transform()->SetRelativePos(Vec3(0.f, 13.f, -10.f));
		m_Champ->Transform()->SetRelativeScale(Vec3(45.f, 75.f, 1.f));
		m_Champ->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 5);

		text->AddComponent(new CTransform);
		text->AddComponent(new CTextRender);
		text->TextRender()->SetString(L"Priest");
		text->TextRender()->SetFont(L"Galmuri14");
		text->TextRender()->SetFontSize(15.f);
		text->TextRender()->SetFontColor(255, 255, 255, 255);
		text->TextRender()->SetOffsetPos(Vec3(-23.f, 29.f, -10.f));
		GetOwner()->AddChild(text);

		m_ToolTip = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ToolTip_Priest_2.prefab")->Instatiate();
		m_ToolTip->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 5);
		GamePlayStatic::SpawnGameObject(m_ToolTip, 2);
		m_ToolTip->SetActive(false);
		for (size_t i = 0; i < m_ToolTip->GetChild().size(); ++i)
		{
			if (L"Champ_SkillIcon" == m_ToolTip->GetChild()[i]->GetName()
				|| L"Champ_UltiIcon" == m_ToolTip->GetChild()[i]->GetName())
			{
				m_ToolTip->GetChild()[i]->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 5);
			}

			m_ToolTip->GetChild()[i]->SetActive(false);
		}
		break;
	case CHAMP_LIST::END:
		break;
	default:
		break;
	}
}

void CChampSlotScript::CreateChamp()
{
	m_Champ = new CGameObject;
	m_Champ->AddComponent(new CTransform);
	m_Champ->AddComponent(new CMeshRender);
	m_Champ->AddComponent(new CAnimator2D);
	m_Champ->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Champ->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BanPickChampIcon.mtrl"));
	GetOwner()->AddChild(m_Champ);
}

void CChampSlotScript::ShowChampInfo()
{

}
