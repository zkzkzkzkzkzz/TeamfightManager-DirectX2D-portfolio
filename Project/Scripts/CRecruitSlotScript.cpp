#include "pch.h"
#include "CRecruitSlotScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>
#include <Engine\components.h>
#include <Engine\CFontMgr.h>

CRecruitSlotScript::CRecruitSlotScript()
	: CScript(RECRUITSLOTSCRIPT)
	, m_Slot(nullptr)
	, m_SlotNTex(nullptr)
	, m_SlotHTex(nullptr)
	, m_SlotCurTex(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_RecruitBtn(nullptr)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_CurImg(nullptr)
{
}

CRecruitSlotScript::CRecruitSlotScript(const CRecruitSlotScript& _Origin)
	: CScript(RECRUITSLOTSCRIPT)
	, m_Slot(nullptr)
	, m_SlotNTex(_Origin.m_SlotNTex)
	, m_SlotHTex(_Origin.m_SlotHTex)
	, m_SlotCurTex(_Origin.m_SlotCurTex)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
	, m_RecruitBtn(nullptr)
	, m_NormalImg(_Origin.m_NormalImg)
	, m_HoverImg(_Origin.m_HoverImg)
	, m_CurImg(_Origin.m_CurImg)
{
}

CRecruitSlotScript::~CRecruitSlotScript()
{
}


void CRecruitSlotScript::begin()
{
	m_SlotNTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_slot_bg.png",
													L"texture\\Lobby\\recruitment\\recruit_slot_bg.png");

	m_SlotHTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\recruitment\\recruit_slot_bg_hover.png",
													L"texture\\Lobby\\recruitment\\recruit_slot_bg_hover.png");

	m_SlotCurTex = m_SlotNTex;

	GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->Transform()->SetRelativeScale(Vec3(302.f, 392.f, 1.f));

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RecruitmentMtrl"));
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	CGameObject* pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitNameText.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);

	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitNameSlotIndex.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);

	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitExplain1.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitExplain2.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitExplain3.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitExplain4.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitExplain5.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);

	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitTime.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);
	pNewObj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\RecruitCost.prefab")->Instatiate();
	pNewObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.f));
	GetOwner()->GetParent()->AddChild(pNewObj);
	GamePlayStatic::SpawnGameObject(pNewObj, 5);
	pNewObj->SetActive(false);
}

void CRecruitSlotScript::tick()
{
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
	m_SlotCurTex = m_SlotHTex;
}

void CRecruitSlotScript::OnUnHovered()
{
	m_SlotCurTex = m_SlotNTex;
}

void CRecruitSlotScript::LBtnUp()
{
	m_SlotCurTex = m_SlotHTex;
}

void CRecruitSlotScript::LBtnReleased()
{
	m_SlotCurTex = m_SlotHTex;
}

void CRecruitSlotScript::LBtnClicked()
{
	m_SlotCurTex = m_SlotHTex;
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
