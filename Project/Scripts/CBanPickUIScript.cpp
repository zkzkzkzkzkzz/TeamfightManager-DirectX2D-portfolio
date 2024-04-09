#include "pch.h"
#include "CBanPickUIScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

#include "CBanpickLevel.h"
#include "CMiddleBanScript.h"
#include "CChampSlotScript.h"

CBanPickUIScript::CBanPickUIScript()
	: CScript(BANPICKUISCRIPT)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
{
}

CBanPickUIScript::CBanPickUIScript(const CBanPickUIScript& _Origin)
	: CScript(BANPICKUISCRIPT)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
{
}

CBanPickUIScript::~CBanPickUIScript()
{
}

void CBanPickUIScript::begin()
{
	for (size_t i = 0; i < (UINT)CHAMP_LIST::END; ++i)
	{
		CGameObject* Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\BanPickChampSlot_2.prefab")->Instatiate();
		Obj->GetScript<CChampSlotScript>()->SetChampList((CHAMP_LIST)i);

		Obj->Transform()->SetRelativePos(Vec3(-367.f + (80.f * i), 122.f, -10.f));
		CTGMgr::GetInst()->G_ChampSlot.push_back(Obj);
		GetOwner()->AddChild(Obj);
	}

	CGameObject* MiddleBan = new CGameObject;
	MiddleBan->SetName(L"MiddleBan");
	MiddleBan->AddComponent(new CTransform);
	MiddleBan->AddComponent(new CMeshRender);
	MiddleBan->AddComponent(new CMiddleBanScript);
	GetOwner()->AddChild(MiddleBan);
}

void CBanPickUIScript::tick()
{
	if (!m_bUIPos)
		SetSlotPos();
}


void CBanPickUIScript::SetSlotPos()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.13f)
	{
		vPos.y += 4500.f * DT * BtwTime;
		if (vPos.y >= -24.f)
			vPos.y = -24.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.13f && m_UIPosTime < 1.f)
	{
		vPos.y += 680.f * DT * BtwTime;
		if (vPos.y >= -24.f)
			vPos.y = -24.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = -24.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}
