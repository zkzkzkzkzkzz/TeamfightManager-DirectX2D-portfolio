#include "pch.h"
#include "CBattleStadiumScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>

#include "CGamerScript.h"
#include "CChampScript.h"

CBattleStadiumScript::CBattleStadiumScript()
	: CScript(BATTLESTADIUMSCRIPT)
	, m_bUIPos(false)
	, m_bClose(false)
	, m_UIPosTime(0.f)
	, m_CloseTime(0.f)
	, m_BlueSpawn(false)
	, m_RedSpawn(false)
	, m_BattleStartTime(0.f)
	, m_Level(nullptr)
	, m_BTMgr(nullptr)
	, m_TopWall(nullptr)
	, m_BottomWall(nullptr)
	, m_RightWall(nullptr)
	, m_LeftWall(nullptr)
{
}

CBattleStadiumScript::CBattleStadiumScript(const CBattleStadiumScript& _Origin)
	: CScript(BATTLESTADIUMSCRIPT)
	, m_bUIPos(false)
	, m_bClose(false)
	, m_UIPosTime(0.f)
	, m_CloseTime(0.f)
	, m_BlueSpawn(false)
	, m_RedSpawn(false)
	, m_BattleStartTime(0.f)
	, m_Level(nullptr)
	, m_BTMgr(nullptr)
	, m_TopWall(nullptr)
	, m_BottomWall(nullptr)
	, m_RightWall(nullptr)
	, m_LeftWall(nullptr)
{
}

CBattleStadiumScript::~CBattleStadiumScript()
{
}


void CBattleStadiumScript::begin()
{
	m_Level = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();

	Transform()->SetRelativePos(Vec3(0.f, 1000.f, 5000.f));
	Transform()->SetRelativeScale(Vec3(1800.f, 800.f, 1.f));
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyMtrl"));
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Title\\sky.png", L"texture\\Title\\sky.png");
	MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	CGameObject* Stadium = new CGameObject;
	Stadium->SetName(L"Stadium");
	Stadium->AddComponent(new CTransform);
	Stadium->AddComponent(new CMeshRender);
	Stadium->Transform()->SetRelativePos(Vec3(0.f, -100.f, -100.f));
	Stadium->Transform()->SetRelativeScale(Vec3(1502.f, 1102.f, 1.f));
	Stadium->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Stadium->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StadiumMtrl"));
	Stadium->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\stadium.png", L"texture\\Champ\\stadium.png");
	Stadium->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	GetOwner()->AddChild(Stadium);

	CGameObject* waterfall = new CGameObject;
	waterfall->SetName(L"waterfall");
	waterfall->AddComponent(new CTransform);
	waterfall->AddComponent(new CMeshRender);
	waterfall->AddComponent(new CAnimator2D);
	waterfall->Transform()->SetRelativePos(Vec3(-147.f, -345.f, 50.f));
	waterfall->Transform()->SetRelativeScale(Vec3(70.f, 222.f, 1.f));
	waterfall->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	waterfall->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"StadiumMtrl"));
	waterfall->Animator2D()->LoadAnimation(L"animdata\\WaterFall.txt");
	waterfall->Animator2D()->Play(L"WaterFall");
	Stadium->AddChild(waterfall);

	waterfall = waterfall->Clone();
	waterfall->Transform()->SetRelativePos(Vec3(147.f, -345.f, 50.f));
	Stadium->AddChild(waterfall);
	
	InitWallCollider();

	m_BTMgr = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\BTMgr.prefab")->Instatiate();
	GamePlayStatic::SpawnGameObject(m_BTMgr, 3);
	m_BTMgr->SetActive(false);
}

void CBattleStadiumScript::tick()
{
	m_BattleStartTime += DT;
	m_CloseTime += DT;

	if (BANPICK_STATE::DONE == m_Level->GetCurBanPickState() && !m_bClose)
	{
		m_CloseTime = 0.f;
		m_bClose = true;
	}

	if (BANPICK_STATE::BATTLE == m_Level->GetCurBanPickState() && !m_bUIPos)
		OpenStadium();
	else if (BANPICK_STATE::DONE == m_Level->GetCurBanPickState() && m_bUIPos && m_CloseTime > 2.f)
		CloseStadium();

	if (BANPICK_STATE::BATTLE == m_Level->GetCurBanPickState() && m_bUIPos && m_BattleStartTime > 1.f)
	{
		m_BTMgr->SetActive(true);

		if (!m_BlueSpawn)
			CreateBlueTeamChamp();

		if (!m_RedSpawn)
			CreateRedTeamChamp();
	}
}

void CBattleStadiumScript::OpenStadium()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.13f)
	{
		vPos.y -= 7000.f * DT * BtwTime;
		if (vPos.y <= 0.f)
			vPos.y = 0.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.13f && m_UIPosTime < 1.f)
	{
		vPos.y -= 680.f * DT * BtwTime;
		if (vPos.y <= 0.f)
			vPos.y = 0.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = 0.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}

void CBattleStadiumScript::CloseStadium()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.13f)
	{
		vPos.y += 7000.f * DT * BtwTime;
		if (vPos.y >= 1000.f)
			vPos.y = 1000.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.13f && m_UIPosTime < 1.f)
	{
		vPos.y += 680.f * DT * BtwTime;
		if (vPos.y >= 1000.f)
			vPos.y = 1000.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = 1000.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_BattleStartTime = 0.f;
		m_bUIPos = false;
		m_Level->SetBanPickState(BANPICK_STATE::RESULT);
	}
}

void CBattleStadiumScript::CreateBlueTeamChamp()
{
	for (size_t i = 0; i < CTGMgr::GetInst()->G_ParticipatingPlayer.size(); ++i)
	{
		CHAMP_LIST list = GETGAMER(CTGMgr::GetInst()->G_ParticipatingPlayer[i])->GetSelectedChamp();

		if (CHAMP_LIST::ARCHER == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Archer.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::BLUE);
		}
		else if (CHAMP_LIST::FIGHTER == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Fighter.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::BLUE);
		}
		else if (CHAMP_LIST::KNIGHT == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Knight.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::BLUE);
		}
		else if (CHAMP_LIST::MONK == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Monk.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::BLUE);
		}
		else if (CHAMP_LIST::NINJA == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Ninja.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::BLUE);
		}
		else if (CHAMP_LIST::PRIEST == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Priest.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::BLUE);
		}
	}

	m_BlueSpawn = true;
}

void CBattleStadiumScript::CreateRedTeamChamp()
{
	for (size_t i = 0; i < CTGMgr::GetInst()->G_TeamGorilla.size(); ++i)
	{
		CHAMP_LIST list = GETGAMER(CTGMgr::GetInst()->G_TeamGorilla[i])->GetSelectedChamp();

		if (CHAMP_LIST::ARCHER == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Archer.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::RED);
		}
		else if (CHAMP_LIST::FIGHTER == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Fighter.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::RED);
		}
		else if (CHAMP_LIST::KNIGHT == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Knight.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::RED);
		}
		else if (CHAMP_LIST::MONK == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Monk.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::RED);
		}
		else if (CHAMP_LIST::NINJA == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Ninja.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::RED);
		}
		else if (CHAMP_LIST::PRIEST == list)
		{
			CGameObject* Champ = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Priest.prefab")->Instatiate();
			GamePlayStatic::SpawnGameObject(Champ, 3);
			GETCHAMP(Champ)->SetTeam(TEAM::RED);
		}
	}

	m_RedSpawn = true;
}

void CBattleStadiumScript::InitWallCollider()
{
	m_TopWall = new CGameObject;
	m_TopWall->SetName(L"TopWallCol");
	m_TopWall->AddComponent(new CTransform);
	m_TopWall->AddComponent(new CCollider2D);
	m_TopWall->Transform()->SetRelativePos(Vec3(0.f, 195.f, 0.f));
	m_TopWall->Collider2D()->SetColliderType(COLLIDER2D_TYPE::RECT);
	m_TopWall->Collider2D()->SetAbsolute(false);
	m_TopWall->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	m_TopWall->Collider2D()->SetOffsetScale(Vec2(1000.f, 100.f));
	GamePlayStatic::SpawnGameObject(m_TopWall, 2);

	m_BottomWall = new CGameObject;
	m_BottomWall->SetName(L"BottomWallCol");
	m_BottomWall->AddComponent(new CTransform);
	m_BottomWall->AddComponent(new CCollider2D);
	m_BottomWall->Transform()->SetRelativePos(Vec3(0.f, -355.f, 0.f));
	m_BottomWall->Collider2D()->SetColliderType(COLLIDER2D_TYPE::RECT);
	m_BottomWall->Collider2D()->SetAbsolute(false);
	m_BottomWall->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	m_BottomWall->Collider2D()->SetOffsetScale(Vec2(1000.f, 100.f));
	GamePlayStatic::SpawnGameObject(m_BottomWall, 2);

	m_LeftWall = new CGameObject;
	m_LeftWall->SetName(L"LeftWallCol");
	m_LeftWall->AddComponent(new CTransform);
	m_LeftWall->AddComponent(new CCollider2D);
	m_LeftWall->Transform()->SetRelativePos(Vec3(-426.f, -80.f, 0.f));
	m_LeftWall->Collider2D()->SetColliderType(COLLIDER2D_TYPE::RECT);
	m_LeftWall->Collider2D()->SetAbsolute(false);
	m_LeftWall->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	m_LeftWall->Collider2D()->SetOffsetScale(Vec2(100.f, 500.f));
	GamePlayStatic::SpawnGameObject(m_LeftWall, 2);

	m_RightWall = new CGameObject;
	m_RightWall->SetName(L"RightWallCol");
	m_RightWall->AddComponent(new CTransform);
	m_RightWall->AddComponent(new CCollider2D);
	m_RightWall->Transform()->SetRelativePos(Vec3(427.f, -80.f, 0.f));
	m_RightWall->Collider2D()->SetColliderType(COLLIDER2D_TYPE::RECT);
	m_RightWall->Collider2D()->SetAbsolute(false);
	m_RightWall->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	m_RightWall->Collider2D()->SetOffsetScale(Vec2(100.f, 500.f));
	GamePlayStatic::SpawnGameObject(m_RightWall, 2);
}
