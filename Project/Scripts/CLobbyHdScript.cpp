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
	, m_CoinText(nullptr)
{	 
}	

CLobbyHdScript::CLobbyHdScript(const CLobbyHdScript& _Other)
	: CScript(LOBBYHDSCRIPT)
	, m_Logo(nullptr)
	, m_LogoTex(_Other.m_LogoTex)
	, m_TeamText(nullptr)
	, m_ScoreText(nullptr)
	, m_CoinText(nullptr)
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


	m_CoinText = new CGameObject;
	m_CoinText->AddComponent(new CTransform);
	m_CoinText->AddComponent(new CTextRender);
	m_CoinText->Transform()->SetRelativePos(Vec3(0.f, 0.f, -20.f));
	m_CoinText->TextRender()->SetString(ToWString(std::to_string(CTGMgr::GetInst()->G_Coin)));
	m_CoinText->TextRender()->SetFont(L"Galmuri14");
	m_CoinText->TextRender()->SetFontSize(23.f);
	m_CoinText->TextRender()->SetFontColor(255, 255, 255, 255);
	m_CoinText->TextRender()->SetOffsetPos(Vec3(567.f, -11.f, 300.f));
	GetOwner()->AddChild(m_CoinText);
}

void CLobbyHdScript::tick()
{
	m_CoinText->TextRender()->SetString(ToWString(std::to_string(CTGMgr::GetInst()->G_Coin)));
}

void CLobbyHdScript::render()
{
}
