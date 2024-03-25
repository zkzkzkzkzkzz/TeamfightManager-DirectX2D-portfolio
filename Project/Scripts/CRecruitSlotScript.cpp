#include "pch.h"
#include "CRecruitSlotScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>
#include <Engine\components.h>
#include <Engine\CFontMgr.h>

#include "CRecruitmentScript.h"

CRecruitSlotScript::CRecruitSlotScript()
	: CScript(RECRUITSLOTSCRIPT)
	, m_Slot(nullptr)
	, m_SlotNTex(nullptr)
	, m_SlotHTex(nullptr)
	, m_SlotSTex(nullptr)
	, m_SlotSHTex(nullptr)
	, m_SlotDTex(nullptr)
	, m_SlotDHTex(nullptr)
	, m_SlotCurTex(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_RecruitBtn(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_CurImg(nullptr)
	, m_NoneText{}
	, m_SearchText{}
	, m_DoneText{}
{
}

CRecruitSlotScript::CRecruitSlotScript(const CRecruitSlotScript& _Origin)
	: CScript(RECRUITSLOTSCRIPT)
	, m_Slot(nullptr)
	, m_SlotNTex(_Origin.m_SlotNTex)
	, m_SlotHTex(_Origin.m_SlotHTex)
	, m_SlotSTex(_Origin.m_SlotSTex)
	, m_SlotSHTex(_Origin.m_SlotSHTex)
	, m_SlotDTex(_Origin.m_SlotDTex)
	, m_SlotDHTex(_Origin.m_SlotDHTex)
	, m_SlotCurTex(_Origin.m_SlotCurTex)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_RecruitBtn(nullptr)
	, m_NormalImg(_Origin.m_NormalImg)
	, m_HoverImg(_Origin.m_HoverImg)
	, m_CurImg(_Origin.m_CurImg)
	, m_NoneText{}
	, m_SearchText{}
	, m_DoneText{}
{
}

CRecruitSlotScript::~CRecruitSlotScript()
{
}


void CRecruitSlotScript::begin()
{
	m_SlotNTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_slot_bg.png",
													L"texture\\Lobby\\recruitment\\recruit_slot_bg.png");
	m_SlotSTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_search.png",
													L"texture\\Lobby\\recruitment\\recruit_search.png");
	m_SlotSHTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_search_hover.png",
													L"texture\\Lobby\\recruitment\\recruit_search_hover.png");
	m_SlotHTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_slot_bg_hover.png",
													L"texture\\Lobby\\recruitment\\recruit_slot_bg_hover.png");
	m_SlotDTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_player_slot_bg.png",
													L"texture\\Lobby\\recruitment\\recruit_player_slot_bg.png");
	m_SlotDHTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_player_slot_bg_hover.png",
													L"texture\\Lobby\\recruitment\\recruit_player_slot_bg_hover.png");
	m_SlotCurTex = m_SlotNTex;

	GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(302.f, 392.f, 1.f));

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RecruitmentMtrl"));
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	// 0
	CGameObject* pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitNameSlotIndex.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -20.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_DefaultText.push_back(pNewObj);

	// 1
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitNameText.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -20.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_DefaultText.push_back(pNewObj);

	// 2
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitExplain1.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -20.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_NoneText.push_back(pNewObj);

	// 3
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitExplain2.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -20.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_NoneText.push_back(pNewObj);

	// 4
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitTime.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -20.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_NoneText.push_back(pNewObj);
	
	// 5
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitCost.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -20.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_NoneText.push_back(pNewObj);

	// 6
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitSearchText.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -30.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->GetParent()->AddChild(pNewObj);
	m_NoneText.push_back(pNewObj);

	// 7
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\SearchingText1.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_SearchText.push_back(pNewObj);

	// 8
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\SearchingText2.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_SearchText.push_back(pNewObj);
	// 9
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\SearchingTime.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_SearchText.push_back(pNewObj);
	// 10
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\SkipText.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->GetParent()->AddChild(pNewObj);
	m_SearchText.push_back(pNewObj);
	// 11
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\DepositText.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -5.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->AddChild(pNewObj);
	m_DoneText.push_back(pNewObj);
	// 12
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\AcceptRecruitText.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -5.f));
	GamePlayStatic::SpawnGameObject(pNewObj, 2);
	GetOwner()->GetParent()->AddChild(pNewObj);
	m_DoneText.push_back(pNewObj);

	for (size_t i = 0; i < m_DefaultText.size(); ++i)
	{
		m_DefaultText[i]->SetActive(false);
	}
	for (size_t i = 0; i < m_NoneText.size(); ++i)
	{
		m_NoneText[i]->SetActive(false);
	}
	for (size_t i = 0; i < m_SearchText.size(); ++i)
	{
		m_SearchText[i]->SetActive(false);
	}
	for (size_t i = 0; i < m_DoneText.size(); ++i)
	{
		m_DoneText[i]->SetActive(false);
	}

	GetOwner()->SetActive(false);
}

void CRecruitSlotScript::tick()
{
	bool bChange = GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->IsChange();

	if (RECRUIT_STATE::NONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		GetOwner()->SetActive(true);

		for (size_t i = 0; i < m_DefaultText.size(); ++i)
		{
			m_DefaultText[i]->SetActive(true);
		}
		for (size_t j = 0; j < m_NoneText.size(); ++j)
		{
			m_NoneText[j]->SetActive(true);
		}
		for (size_t k = 0; k < m_SearchText.size(); ++k)
		{
			m_SearchText[k]->SetActive(false);
		}
		for (size_t l = 0; l < m_DoneText.size(); ++l)
		{
			m_DoneText[l]->SetActive(false);
		}
	}
	else if (RECRUIT_STATE::SEARCH == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		for (size_t i = 0; i < m_DefaultText.size(); ++i)
		{
			m_DefaultText[i]->SetActive(true);
		}
		for (size_t j = 0; j < m_NoneText.size(); ++j)
		{
			m_NoneText[j]->SetActive(false);
		}
		for (size_t k = 0; k < m_SearchText.size(); ++k)
		{
			m_SearchText[k]->SetActive(true);
		}
		for (size_t l = 0; l < m_DoneText.size(); ++l)
		{
			m_DoneText[l]->SetActive(false);
		}
	}
	else if (RECRUIT_STATE::DONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		for (size_t i = 0; i < m_DefaultText.size(); ++i)
		{
			m_DefaultText[i]->SetActive(false);
		}
		for (size_t j = 0; j < m_NoneText.size(); ++j)
		{
			m_NoneText[j]->SetActive(false);
		}
		for (size_t k = 0; k < m_SearchText.size(); ++k)
		{
			m_SearchText[k]->SetActive(false);
		}
		for (size_t l = 0; l < m_DoneText.size(); ++l)
		{
			m_DoneText[l]->SetActive(true);
		}
	}
	else if (RECRUIT_STATE::CLOSE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState() && bChange)
	{
		GetOwner()->SetActive(false);

		for (size_t i = 0; i < m_DefaultText.size(); ++i)
		{
			m_DefaultText[i]->SetActive(false);
		}
		for (size_t j = 0; j < m_NoneText.size(); ++j)
		{
			m_NoneText[j]->SetActive(false);
		}
		for (size_t k = 0; k < m_SearchText.size(); ++k)
		{
			m_SearchText[k]->SetActive(false);
		}
		for (size_t l = 0; l < m_DoneText.size(); ++l)
		{
			m_DoneText[l]->SetActive(false);
		}
	}

	CheckMousePos();

	render();
}

void CRecruitSlotScript::render()
{
	if (nullptr != m_SlotCurTex)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_1, m_SlotCurTex);

	if (nullptr != m_CurImg)
		GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
}


void CRecruitSlotScript::OnHovered()
{
	if (RECRUIT_STATE::NONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotHTex;
	}
	else if (RECRUIT_STATE::SEARCH == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotSHTex;
	}
	else if (RECRUIT_STATE::DONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotDHTex;
	}
}

void CRecruitSlotScript::OnUnHovered()
{
	if (RECRUIT_STATE::NONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotNTex;
	}
	else if (RECRUIT_STATE::SEARCH == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotSTex;
	}
	else if (RECRUIT_STATE::DONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotDTex;
	}
}

void CRecruitSlotScript::LBtnUp()
{
	if (RECRUIT_STATE::NONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotHTex;
	}
	else if (RECRUIT_STATE::SEARCH == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotSHTex;
	}
	else if (RECRUIT_STATE::DONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotDHTex;
	}
}

void CRecruitSlotScript::LBtnReleased()
{
	if (RECRUIT_STATE::NONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotHTex;
	}
	else if (RECRUIT_STATE::SEARCH == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotSHTex;
	}
	else if (RECRUIT_STATE::DONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotDHTex;
	}
}

void CRecruitSlotScript::LBtnClicked()
{
	if (RECRUIT_STATE::NONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotHTex;
	}
	else if (RECRUIT_STATE::SEARCH == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotSHTex;
	}
	else if (RECRUIT_STATE::DONE == GetOwner()->GetParent()->GetScript<CRecruitmentScript>()->GetState())
	{
		m_SlotCurTex = m_SlotDHTex;
	}
}

void CRecruitSlotScript::CheckMousePos()
{
	m_bMouseOn_Prev = m_bMouseOn;

	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	vMousePos.x -= vResolution.x / 2.f;
	vMousePos.y = -(vMousePos.y - vResolution.y / 2.f);

	Vec3 vRelativePos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vWorldPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vWorldScale = GetOwner()->Transform()->GetRelativeScale();

	Vec2 vLT = Vec2(vWorldPos.x - vWorldScale.x / 2, (vWorldPos.y + vRelativePos.y) - vWorldScale.y / 2);
	Vec2 vRB = Vec2(vWorldPos.x + vWorldScale.x / 2, (vWorldPos.y + vRelativePos.y) + vWorldScale.y / 2);

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
			LBtnUp();

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
