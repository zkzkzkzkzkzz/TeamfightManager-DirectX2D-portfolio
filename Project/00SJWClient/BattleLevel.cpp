#include "pch.h"
#include "BattleLevel.h"

#include <Engine\CCollisionMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>
#include <Engine\CFSM.h>

#include <Engine\CMesh.h>
#include <Engine\CGraphicsShader.h>
#include <Engine\CTexture.h>
#include <Engine\CSetColorShader.h>
#include <Engine\CFontMgr.h>

#include "CLevelSaveLoad.h"

#include <Scripts\CTGMgr.h>
#include <Scripts\CChampScript.h>
#include <Scripts\CArcherScript.h>
#include <Scripts\CFighterScript.h>
#include <Scripts\CKnightScript.h>
#include <Scripts\CMonkScript.h>
#include <Scripts\CNinjaScript.h>
#include <Scripts\CPriestScript.h>

#include <Scripts\CIdleState.h>
#include <Scripts\CTraceState.h>
#include <Scripts\CAttackState.h>
#include <Scripts\CSkillState.h>
#include <Scripts\CUltimateState.h>
#include <Scripts\CDeadState.h>

void BattleLevel::CreateTempLevel()
{
	CLevel* pTempLevel = new CLevel;

	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Light");
	pTempLevel->GetLayer(2)->SetName(L"Background");
	pTempLevel->GetLayer(3)->SetName(L"Champ");
	pTempLevel->GetLayer(4)->SetName(L"Cursor");
	pTempLevel->GetLayer(5)->SetName(L"Projectile");
	pTempLevel->GetLayer(6)->SetName(L"Effect");
	pTempLevel->GetLayer(31)->SetName(L"UI");

	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck(3, 3);
	CCollisionMgr::GetInst()->LayerCheck(5, 3);

	Ptr<CFSM> pFSM = new CFSM(false);

	pFSM->AddState(L"Idle", new CIdleState);
	pFSM->AddState(L"Trace", new CTraceState);
	pFSM->AddState(L"Attack", new CAttackState);
	pFSM->AddState(L"Skill", new CSkillState);
	pFSM->AddState(L"Ultimate", new CUltimateState);
	pFSM->AddState(L"Dead", new CDeadState);

	CAssetMgr::GetInst()->AddAsset<CFSM>(L"ChampFSM", pFSM.Get());

	pFSM = new CFSM(false);

	pFSM->AddState(L"Idle", new CIdleState);
	pFSM->AddState(L"Trace", new CTraceState);
	pFSM->AddState(L"Attack", new CAttackState);
	pFSM->AddState(L"Dead", new CDeadState);

	CAssetMgr::GetInst()->AddAsset<CFSM>(L"SummonFSM", pFSM.Get());

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
	pCamObj->Camera()->LayerCheck(3, false);
	pCamObj->Camera()->LayerCheck(5, false);
	pTempLevel->AddObject(pCamObj, 0);

	// BG Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"BGCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(2, true);
	pTempLevel->AddObject(pCamObj, 0);

	// Champ Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"ChampCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(2);
	pCamObj->Camera()->LayerCheck(3, true);
	pTempLevel->AddObject(pCamObj, 0);

	// Projectile Camera 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"ProjectileCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	pCamObj->Camera()->SetCameraPriority(3);
	pCamObj->Camera()->LayerCheck(5, true);
	pTempLevel->AddObject(pCamObj, 0);

	// 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light2D");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(1.f, 1.f, 1.f));

	pTempLevel->AddObject(pLight, 1);


	CGameObject* Stadium = new CGameObject;
	Stadium->SetName(L"Stadium");
	Stadium->AddComponent(new CTransform);
	Stadium->AddComponent(new CMeshRender);
	Stadium->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1000.f));
	Stadium->Transform()->SetRelativeScale(Vec3(1427.f, 1029.f, 1.f));
	Stadium->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Stadium->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StadiumMtrl"));
	Stadium->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\stadium.png",
															L"texture\\Champ\\stadium.png");
 	Stadium->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	pTempLevel->AddObject(Stadium, 2);

	CGameObject* BTmgr = new CGameObject;
	BTmgr->SetName(L"BTMgr");
	BTmgr->AddComponent(new CTransform);
	BTmgr->AddComponent(new CChampScript);
	GETCHAMP(BTmgr)->SetTeam(TEAM::NONE);
	pTempLevel->AddObject(BTmgr, 3);

	CGameObject* Champ = new CGameObject;
	Champ->SetName(L"Archer");
	Champ->AddComponent(new CTransform);
	Champ->AddComponent(new CMeshRender);
	Champ->AddComponent(new CCollider2D);
	Champ->AddComponent(new CAnimator2D);
	Champ->AddComponent(new CStateMachine);
	Champ->AddComponent(new CArcherScript);
	Champ->GetScript<CArcherScript>()->SetTeam(TEAM::BLUE);
	pTempLevel->AddObject(Champ, 3);

	Champ = new CGameObject;
	Champ->SetName(L"Knight");
	Champ->AddComponent(new CTransform);
	Champ->AddComponent(new CMeshRender);
	Champ->AddComponent(new CCollider2D);
	Champ->AddComponent(new CAnimator2D);
	Champ->AddComponent(new CStateMachine);
	Champ->AddComponent(new CKnightScript);
	Champ->GetScript<CKnightScript>()->SetTeam(TEAM::BLUE);
	pTempLevel->AddObject(Champ, 3);

	//Champ = new CGameObject;
	//Champ->SetName(L"Fighter");
	//Champ->AddComponent(new CTransform);
	//Champ->AddComponent(new CMeshRender);
	//Champ->AddComponent(new CCollider2D);
	//Champ->AddComponent(new CAnimator2D);
	//Champ->AddComponent(new CStateMachine);
	//Champ->AddComponent(new CFighterScript);
	//Champ->GetScript<CFighterScript>()->SetTeam(TEAM::RED);
	//pTempLevel->AddObject(Champ, 3);

	Champ = new CGameObject;
	Champ->SetName(L"Monk");
	Champ->AddComponent(new CTransform);
	Champ->AddComponent(new CMeshRender);
	Champ->AddComponent(new CCollider2D);
	Champ->AddComponent(new CAnimator2D);
	Champ->AddComponent(new CStateMachine);
	Champ->AddComponent(new CMonkScript);
	Champ->GetScript<CMonkScript>()->SetTeam(TEAM::RED);
	pTempLevel->AddObject(Champ, 3);

	Champ = new CGameObject;
	Champ->SetName(L"Ninja");
	Champ->AddComponent(new CTransform);
	Champ->AddComponent(new CMeshRender);
	Champ->AddComponent(new CCollider2D);
	Champ->AddComponent(new CAnimator2D);
	Champ->AddComponent(new CStateMachine);
	Champ->AddComponent(new CNinjaScript);
	Champ->GetScript<CNinjaScript>()->SetTeam(TEAM::RED);
	pTempLevel->AddObject(Champ, 3);

	//Champ = new CGameObject;
	//Champ->SetName(L"Priest");
	//Champ->AddComponent(new CTransform);
	//Champ->AddComponent(new CMeshRender);
	//Champ->AddComponent(new CCollider2D);
	//Champ->AddComponent(new CAnimator2D);
	//Champ->AddComponent(new CStateMachine);
	//Champ->AddComponent(new CPriestScript);
	//Champ->GetScript<CPriestScript>()->SetTeam(TEAM::RED);
	//pTempLevel->AddObject(Champ, 3);

	// 레벨 플레이
	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);
}
