#include "pch.h"
#include "CArrowScript.h"

#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CMesh.h>
#include <Engine\CMaterial.h>

#include "CArcherScript.h"

CArrowScript::CArrowScript()
	: CScript(ARROWSCRIPT)
	, m_Tex(nullptr)
	, m_Target(nullptr)
	, m_Speed(50.f)
{
}

CArrowScript::CArrowScript(const CArrowScript& _Origin)
	: CScript(ARROWSCRIPT)
	, m_Tex(_Origin.m_Tex)
	, m_Target(nullptr)
	, m_Speed(50.f)
{
}

CArrowScript::~CArrowScript()
{
}



void CArrowScript::begin()
{
	m_Tex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\arrow.png",
												L"texture\\Champ\\arrow.png");


	Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	Transform()->SetRelativeScale(Vec3(16.f, 16.f, 0.f));
	Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProjectileMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Collider2D()->SetOffsetPos(Vec2(0.f, -0.05f));
	Collider2D()->SetOffsetScale(Vec2(1.f, 0.5f));
	Collider2D()->SetAbsolute(false);
}

void CArrowScript::tick()
{
	m_Target = GetOwner()->GetParent()->GetScript<CArcherScript>()->m_Target;

	if (m_Target)
	{
		Vec3 vDir = m_Target->Transform()->GetRelativePos() - Transform()->GetRelativePos();
		vDir.Normalize();

		Vec3 vRot = Transform()->GetRelativeRotation();
		float fDot = vDir.x;
		float fAngle = acosf(fDot);
		vRot.z = fAngle;
		if (0.f == GetOwner()->GetParent()->Transform()->GetRelativeRotation().y)
		{
			float vPosX = Transform()->GetRelativePos().x + vDir.x * DT * m_Speed;
			float vPosY = Transform()->GetRelativePos().y + vDir.y * DT * m_Speed;
			Transform()->SetRelativePos(Vec3(vPosX, vPosY, 0));
			Transform()->SetRelativeRotation(Vec3(vRot));
		}
		//else
		//{
		//	float vPosX = Transform()->GetRelativePos().x - vDir.x * DT * m_Speed;
		//	float vPosY = Transform()->GetRelativePos().y + vDir.y * DT * m_Speed;
		//	Transform()->SetRelativePos(Vec3(vPosX, vPosY, 0));
		//	Transform()->SetRelativeRotation(Vec3(vRot));
		//}


	}

	render();
}

void CArrowScript::render()
{
	if (nullptr != m_Tex)
		MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Tex);
}


void CArrowScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (GetOwner()->GetParent() != _OtherObj)
		int a = 0;

}

void CArrowScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CArrowScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

Vec3 CArrowScript::Rotate(Vec3 _vDir, float _angle)
{
	Vec3 vRotateDir = Vec3(cosf(_angle) * _vDir.x - sinf(_angle) * _vDir.y
		, sinf(_angle) * _vDir.x + cosf(_angle) * _vDir.y, _vDir.z);

	return vRotateDir;
}