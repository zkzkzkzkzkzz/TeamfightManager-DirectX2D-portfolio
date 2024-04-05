#include "pch.h"
#include "CBanpickLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>

#include "CGamerScript.h"
#include "CTeamHeaderScript.h"
#include "CTeamSlotScript.h"

CBanpickLevel::CBanpickLevel()
	: m_BlueTeam{}
	, m_RedTeam{}
{
	CTGMgr::CloneTGMgr();

	InitUI();
}

CBanpickLevel::CBanpickLevel(const CBanpickLevel& _Origin)
	: m_BlueTeam{}
	, m_RedTeam{}
{
	CTGMgr::CloneTGMgr();

	InitUI();
}

CBanpickLevel::~CBanpickLevel()
{
}


void CBanpickLevel::begin()
{
	// 배경
	CGameObject* Obj = new CGameObject;
	Obj->SetName(L"BanPickBG");
	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 8000.f));
	Obj->Transform()->SetRelativeScale(Vec3(1280.f, 800.f, 1.f));
	Obj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Obj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BanPickBG.mtrl"));
	Obj->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	AddObject(Obj, 2);

	Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\TeamHeader.prefab")->Instatiate();
	AddObject(Obj, 2);


	for (size_t i = 0; i < CTGMgr::GetInst()->G_ParticipatingPlayer.size(); ++i)
	{
		Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\BlueTeamCard_1.prefab")->Instatiate();
		Obj->GetScript<CTeamSlotScript>()->SetGamerToSlot(CTGMgr::GetInst()->G_ParticipatingPlayer[i]);

		if (0 == i)
			Obj->Transform()->SetRelativePos(Vec3(-700.f, 210.f, 1000.f));
		else
			Obj->Transform()->SetRelativePos(Vec3(-700.f, 49.f, 1000.f));
	
		AddObject(Obj, 2);
	}

	Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\BanPickUI_2.prefab")->Instatiate();
	Obj->Transform()->SetRelativePos(Vec3(0.f, -800.f, 3000.f));
	Obj->Transform()->SetRelativeScale(Vec3(1280.f, 800.f, 1.f));
	AddObject(Obj, 2);

	CLevel::begin();
}

void CBanpickLevel::tick()
{
	CLevel::tick();
}

void CBanpickLevel::finaltick()
{
	CLevel::finaltick();
}

void CBanpickLevel::InitUI()
{
	GetLayer(0)->SetName(L"Default");
	GetLayer(1)->SetName(L"Light");
	GetLayer(2)->SetName(L"Background");
	GetLayer(3)->SetName(L"Gamer");
	GetLayer(4)->SetName(L"Cursor");
	GetLayer(31)->SetName(L"UI");

	// Main Camera 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(2, false);
	AddObject(pCamObj, 0);

	// BG Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"BGCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(2, true);
	AddObject(pCamObj, 0);

	// 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light2D");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);
	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(1.f, 1.f, 1.f));
	AddObject(pLight, 1);
}