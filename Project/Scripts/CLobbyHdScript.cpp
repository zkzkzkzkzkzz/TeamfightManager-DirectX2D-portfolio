#include "pch.h"
#include "CLobbyHdScript.h"

#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>

#include <Engine\CFontMgr.h>

CLobbyHdScript::CLobbyHdScript()
	: CScript(LOBBYHDSCRIPT)
	, m_Logo(nullptr)
	, m_LogoTex(nullptr)
	, m_TeamText(nullptr)
	, m_ScoreText(nullptr)
{	 
}	

CLobbyHdScript::CLobbyHdScript(const CLobbyHdScript& _Other)
	: CScript(LOBBYHDSCRIPT)
	, m_Logo(nullptr)
	, m_LogoTex(_Other.m_LogoTex)
	, m_TeamText(nullptr)
	, m_ScoreText(nullptr)
{
}

CLobbyHdScript::~CLobbyHdScript()
{
}


void CLobbyHdScript::begin()
{
	m_Logo = new CGameObject;
	m_Logo->SetName(L"TeamLogo");
	m_Logo->AddComponent(new CTransform);
	m_Logo->AddComponent(new CMeshRender);
	m_Logo->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Logo->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LogoMtrl"));
	m_Logo->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_Logo->Transform()->SetRelativePos(Vec3(-600.f, 2.f, -10.f));
	m_Logo->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 0.f));

	//m_LogoTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\captains.png", L"texture\\Lobby\\header\\captains.png");
	//m_Logo->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_LogoTex);
	GetOwner()->AddChild(m_Logo);

	m_TeamText = new CGameObject;
	m_TeamText->AddComponent(new CTransform);
	m_TeamText->AddComponent(new CTextRender);
	m_TeamText->Transform()->SetRelativePos(Vec3(0.f, 0.f, -100.f));
	m_TeamText->TextRender()->SetString(L"AR49");
	m_TeamText->TextRender()->TextInit(L"Galmuri11", 40.f, FONT_RGBA(255, 255, 255, 255));
	m_TeamText->TextRender()->SetOffsetPos(Vec3(-550.f, -25.f, 0.f));
	GetOwner()->AddChild(m_TeamText);

	m_ScoreText = new CGameObject;
	m_ScoreText->AddComponent(new CTransform);
	m_ScoreText->AddComponent(new CTextRender);
	m_ScoreText->Transform()->SetRelativePos(Vec3(0.f, 0.f, -100.f));
	m_ScoreText->TextRender()->SetString(L"#1 / 0W 0L +0");
	m_ScoreText->TextRender()->TextInit(L"Silver", 23.f, FONT_RGBA(255, 255, 255, 255));
	m_ScoreText->TextRender()->SetOffsetPos(Vec3(-550.f, 10.f, 0.f));
	GetOwner()->AddChild(m_ScoreText);
}

void CLobbyHdScript::tick()
{
}

void CLobbyHdScript::render()
{
}
