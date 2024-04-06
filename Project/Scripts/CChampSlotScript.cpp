#include "pch.h"
#include "CChampSlotScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include "CBanpickLevel.h"
#include "CEffectScript.h"

CChampSlotScript::CChampSlotScript()
	: CScript(CHAMPSLOTSCRIPT)
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
{
}

CChampSlotScript::CChampSlotScript(const CChampSlotScript& _Origin)
	: CScript(CHAMPSLOTSCRIPT)
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
{
}

CChampSlotScript::~CChampSlotScript()
{
}


void CChampSlotScript::begin()
{
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	m_Level = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();

	m_EffectObj = new CGameObject;
	m_EffectObj->AddComponent(new CTransform);
	m_EffectObj->AddComponent(new CMeshRender);
	m_EffectObj->AddComponent(new CAnimator2D);

	Vec3 vPos = Transform()->GetRelativePos();

	m_EffectObj->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y, vPos.z - 10.f));
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
	}
}

void CChampSlotScript::OnUnHovered()
{
	m_EffectObj->Animator2D()->Stop();
	m_EffectObj->Animator2D()->Play(L"ChampSlotIdle");
}

void CChampSlotScript::LBtnClicked()
{
	if ((BANPICK_STATE::BLUEPICK1 == m_Level->GetCurBanPickState() || BANPICK_STATE::BLUEPICK2 == m_Level->GetCurBanPickState())
		&& SLOT_STATE::NONE == m_SlotState)
	{
		m_EffectObj->Animator2D()->Stop();
		m_EffectObj->Animator2D()->Play(L"ChampSlotIdle");
	}
	else if ((BANPICK_STATE::BLUEBAN == m_Level->GetCurBanPickState() && SLOT_STATE::NONE == m_SlotState))
	{
		SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeScale(), Transform()->GetRelativeRotation()
					, L"BanAnim", 0.2, false, Vec3(0.f, 0.f, -10.f));
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
