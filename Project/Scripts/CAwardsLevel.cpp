#include "pch.h"
#include "CAwardsLevel.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>

#include "CCursorScript.h"
#include "CEffectScript.h"

CAwardsLevel::CAwardsLevel()
	: m_idx(TEXT_INDEX::NONE)
	, m_Particle{}
	, m_ParticleTime(0.f)
	, m_SpawnParticle(false)
	, m_ParticleCount(0)
{
	InitUI();
}

CAwardsLevel::CAwardsLevel(const CAwardsLevel& _Origin)
	: m_idx(_Origin.m_idx)
	, m_Particle{}
	, m_ParticleTime(0.f)
	, m_SpawnParticle(false)
	, m_ParticleCount(0)
{
	InitUI();
}

CAwardsLevel::~CAwardsLevel()
{
}

void CAwardsLevel::begin()
{
	CGameObject* BG = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\AwardsBG.prefab")->Instatiate();
	AddObject(BG, 2);

	CGameObject* TextBox = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_TextBox_1.prefab")->Instatiate();
	AddObject(TextBox, 2);

	CGameObject* LeftCurtain = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Curtain_Left_2.prefab")->Instatiate();
	AddObject(LeftCurtain, 2);

	CGameObject* RightCurtain = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Curtain_Right_2.prefab")->Instatiate();
	AddObject(RightCurtain, 2);

	CGameObject* Mc = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_MC_1.prefab")->Instatiate();
	AddObject(Mc, 7);

	CGameObject* Manager = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_Manager_1.prefab")->Instatiate();
	AddObject(Manager, 7);
	Manager->SetActive(false);

	CGameObject* Chobo = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_Chobo_1.prefab")->Instatiate();
	AddObject(Chobo, 7);
	Chobo->SetActive(false);

	CGameObject* JoongSoo = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_JoongSoo_2.prefab")->Instatiate();
	AddObject(JoongSoo, 7);
	JoongSoo->SetActive(false);

	CGameObject* Gosu = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Awards_Gosu_2.prefab")->Instatiate();
	AddObject(Gosu, 7);
	Gosu->SetActive(false);

	CLevel::begin();
}

void CAwardsLevel::tick()
{
	CLevel::tick();

	m_idx = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"TextBox")->GetScript<CTextBoxScript>()->GetTextIdx();

	if (TEXT_INDEX::TEXT2 == m_idx)
	{
		CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager")->SetActive(true);
		CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Chobo")->SetActive(true);
		CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"JoongSoo")->SetActive(true);
		CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Gosu")->SetActive(true);
	}
	else if (TEXT_INDEX::TEXT3 == m_idx || TEXT_INDEX::TEXT4 == m_idx)
	{
		m_ParticleTime += DT;

		if (!m_SpawnParticle && m_ParticleTime > 1.f)
		{
			SpawnParticle();
			m_SpawnParticle = true;
			m_ParticleTime = 0.f;
		}
		else if (m_SpawnParticle && m_ParticleTime > 2.f)
		{
			for (size_t i = 0; i < m_Particle.size(); ++i)
			{
				GamePlayStatic::DestroyGameObject(m_Particle[i]);
			}

			m_SpawnParticle = false;
			m_ParticleTime = 0.f;
		}
	}
}

void CAwardsLevel::finaltick()
{
	CLevel::finaltick();
}

void CAwardsLevel::SpawnEffect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _anim, float _time, bool _repeat, Vec3 _offset)
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



void CAwardsLevel::InitUI()
{
	GetLayer(0)->SetName(L"Default");
	GetLayer(1)->SetName(L"Light");
	GetLayer(2)->SetName(L"Background");
	GetLayer(4)->SetName(L"Cursor");
	GetLayer(6)->SetName(L"Effect");
	GetLayer(7)->SetName(L"Awards");
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
	pCamObj->Camera()->LayerCheck(6, false);
	pCamObj->Camera()->LayerCheck(7, false);
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

	// Effect Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"EffectCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(2);
	pCamObj->Camera()->LayerCheck(6, true);
	AddObject(pCamObj, 0);

	// Awards Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"AwardsCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(3);
	pCamObj->Camera()->LayerCheck(7, true);
	AddObject(pCamObj, 0);

	// 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light2D");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);
	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(1.f, 1.f, 1.f));
	AddObject(pLight, 1);

	CGameObject* pCursor = new CGameObject;
	pCursor->SetName(L"Cursor");
	pCursor->AddComponent(new CTransform);
	pCursor->AddComponent(new CMeshRender);
	pCursor->AddComponent(new CCursorScript);
	AddObject(pCursor, 4);

}

void CAwardsLevel::SpawnParticle()
{
	// Particle  생성
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"Particle");
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);
	pParticleObj->Transform()->SetRelativePos(Vec3(-300.f, 0.f, 100.f));
	pParticleObj->ParticleSystem()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\Particle1.png"
																					, L"texture\\particle\\Particle1.png"));
	AddObject(pParticleObj, 7);
	m_Particle.push_back(pParticleObj);

	pParticleObj = pParticleObj->Clone();
	pParticleObj->ParticleSystem()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\Particle2.png"
																					, L"texture\\particle\\Particle2.png"));
	AddObject(pParticleObj, 7);
	m_Particle.push_back(pParticleObj);

	pParticleObj = pParticleObj->Clone();
	pParticleObj->ParticleSystem()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\Particle3.png"
																					, L"texture\\particle\\Particle3.png"));
	AddObject(pParticleObj, 7);
	m_Particle.push_back(pParticleObj);

	pParticleObj = pParticleObj->Clone();
	pParticleObj->ParticleSystem()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\Particle4.png"
																					, L"texture\\particle\\Particle4.png"));
	AddObject(pParticleObj, 7);
	m_Particle.push_back(pParticleObj);

	pParticleObj = pParticleObj->Clone();
	pParticleObj->ParticleSystem()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\Particle5.png"
																					, L"texture\\particle\\Particle5.png"));
	AddObject(pParticleObj, 7);
	m_Particle.push_back(pParticleObj);
}
