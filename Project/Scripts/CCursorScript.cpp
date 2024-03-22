#include "pch.h"
#include "CCursorScript.h"

#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CMeshRender.h>

CCursorScript::CCursorScript()
	: CScript(CURSORSCRIPT)
	, m_CursorSize{}
	, m_CursorTex(nullptr)
{
}

CCursorScript::~CCursorScript()
{
}

void CCursorScript::begin()
{
	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CursorMtrl"));
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	m_CursorTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Cursor\\mouse_cursor.png", L"texture\\Cursor\\mouse_cursor.png");
	GetOwner()->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CursorTex);
	
	m_CursorSize.x = m_CursorTex->GetWidth();
	m_CursorSize.y = m_CursorTex->GetHeight();

	GetOwner()->Transform()->SetRelativeScale(Vec3(m_CursorSize.x, m_CursorSize.y, 1));
}

void CCursorScript::tick()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	vMousePos.x -= vResolution.x / 2.f;
	vMousePos.y = -(vMousePos.y - vResolution.y / 2.f);

	GetOwner()->Transform()->SetRelativePos(Vec3(vMousePos.x, vMousePos.y, 0.f));
}