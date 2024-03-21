#include "pch.h"
#include "CLobbyHdScript.h"

#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CAssetMgr.h>

CLobbyHdScript::CLobbyHdScript()
	: CScript(LOBBYHDSCRIPT)
	, m_Logo(nullptr)
	, m_LogoTex(nullptr)
	, m_szText{}
	, m_font{}
{	 
}	

CLobbyHdScript::CLobbyHdScript(const CLobbyHdScript& _Other)
	: CScript(LOBBYHDSCRIPT)
	, m_Logo(nullptr)
	, m_LogoTex(_Other.m_LogoTex)
	, m_szText{}
	, m_font{}
{
}

CLobbyHdScript::~CLobbyHdScript()
{
	if (m_Logo)
		m_Logo->Destroy();
}


void CLobbyHdScript::begin()
{
	m_Logo = new CGameObject;
	m_Logo->SetName(L"TeamLogo");
	m_Logo->AddComponent(new CTransform);
	m_Logo->AddComponent(new CMeshRender);
	m_Logo->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Logo->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LogoMtrl"));
	m_Logo->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
	m_Logo->Transform()->SetRelativePos(Vec3(-600.f, 2.f, -10.f));
	m_Logo->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 0.f));

	m_LogoTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Lobby\\header\\captains.png", L"texture\\Lobby\\header\\captains.png");
	m_Logo->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_LogoTex);
	GetOwner()->AddChild(m_Logo);
}

void CLobbyHdScript::tick()
{
}

void CLobbyHdScript::render()
{
}
