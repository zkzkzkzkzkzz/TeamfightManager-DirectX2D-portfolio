#include "pch.h"
#include "CArrowScript.h"

#include <Engine\CGameObject.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CMesh.h>
#include <Engine\CMaterial.h>

#include "CChampScript.h"
#include "CArcherScript.h"

CArrowScript::CArrowScript()
	: CScript(ARROWSCRIPT)
	, m_Tex(nullptr)
	, m_Target(nullptr)
	, m_Shooter(nullptr)
	, m_Speed(240.f)
	, m_Pos{}
	, m_Rotation{}
	, m_prevPos{}
{
}

CArrowScript::CArrowScript(const CArrowScript& _Origin)
	: CScript(ARROWSCRIPT)
	, m_Tex(_Origin.m_Tex)
	, m_Shooter(nullptr)
	, m_Target(nullptr)
	, m_Speed(240.f)
	, m_Pos{}
	, m_Rotation{}
	, m_prevPos{}
{
}

CArrowScript::~CArrowScript()
{
}



void CArrowScript::begin()
{
	m_Tex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Champ\\arrow.png",
												L"texture\\Champ\\arrow.png");

	m_Pos = m_Shooter->Transform()->GetRelativePos();
	m_prevPos = m_Target->Transform()->GetRelativePos();

	if (0.f == m_Shooter->Transform()->GetRelativeRotation().y)
		m_Pos.x += 15.f;
	else if (XM_PI == m_Shooter->Transform()->GetRelativeRotation().y)
		m_Pos.x -= 15.f;

	Transform()->SetRelativePos(Vec3(m_Pos));
	Transform()->SetRelativeScale(Vec3(16.f, 16.f, 0.f));
	Transform()->SetRelativeRotation(Vec3(m_Shooter->Transform()->GetRelativeRotation()));
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ProjectileMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	Collider2D()->SetOffsetPos(Vec2(0.f, -0.05f));
	Collider2D()->SetOffsetScale(Vec2(1.f, 0.5f));
	Collider2D()->SetAbsolute(false);

}

void CArrowScript::tick()
{
	if (m_Target->IsActive())
	{
		Vec3 vDir = m_Target->Transform()->GetRelativePos() - m_Pos;
		vDir.Normalize();

		if (vDir.x >= 0.f)
		{
			float fDot = vDir.x;
			float fAngle = acosf(fDot);

			Vec3 vRot = Transform()->GetRelativeRotation();

			if (vDir.y >= 0.f)
				vRot.z = fAngle;
			else if (vDir.y < 0.f)
				vRot.z = -fAngle;
		
			float vPosX = Transform()->GetRelativePos().x + vDir.x * DT * m_Speed;
			float vPosY = Transform()->GetRelativePos().y + vDir.y * DT * m_Speed;
			Transform()->SetRelativePos(Vec3(vPosX, vPosY, 0));
			Transform()->SetRelativeRotation(Vec3(vRot));
		}
		else if (vDir.x < 0.f)
		{
			float fDot = -vDir.x;
			float fAngle = acosf(fDot);

			Vec3 vRot = Transform()->GetRelativeRotation();
			
			if (vDir.y >= 0.f)
				vRot.z = -fAngle;
			else if (vDir.y < 0.f)
				vRot.z = fAngle;

			float vPosX = Transform()->GetRelativePos().x + vDir.x * DT * m_Speed;
			float vPosY = Transform()->GetRelativePos().y + vDir.y * DT * m_Speed;
			Transform()->SetRelativePos(Vec3(vPosX, vPosY, 0));
			Transform()->SetRelativeRotation(Vec3(vRot));
		}
	}
	else
	{
		Vec3 vDir = m_prevPos - m_Pos;
		vDir.Normalize();

		if (vDir.x >= 0.f)
		{
			float fDot = vDir.x;
			float fAngle = acosf(fDot);

			Vec3 vRot = Transform()->GetRelativeRotation();

			if (vDir.y >= 0.f)
				vRot.z = fAngle;
			else if (vDir.y < 0.f)
				vRot.z = -fAngle;

			float vPosX = Transform()->GetRelativePos().x + vDir.x * DT * m_Speed;
			float vPosY = Transform()->GetRelativePos().y + vDir.y * DT * m_Speed;
			Transform()->SetRelativePos(Vec3(vPosX, vPosY, 0));
			Transform()->SetRelativeRotation(Vec3(vRot));
		}
		else if (vDir.x < 0.f)
		{
			float fDot = -vDir.x;
			float fAngle = acosf(fDot);

			Vec3 vRot = Transform()->GetRelativeRotation();

			if (vDir.y >= 0.f)
				vRot.z = -fAngle;
			else if (vDir.y < 0.f)
				vRot.z = fAngle;

			float vPosX = Transform()->GetRelativePos().x + vDir.x * DT * m_Speed;
			float vPosY = Transform()->GetRelativePos().y + vDir.y * DT * m_Speed;
			Transform()->SetRelativePos(Vec3(vPosX, vPosY, 0));
			Transform()->SetRelativeRotation(Vec3(vRot));
		}

		if (m_prevPos.x - 10.f <= Transform()->GetRelativePos().x && m_prevPos.x + 10.f >= Transform()->GetRelativePos().x
			&& m_prevPos.y- 10.f <= Transform()->GetRelativePos().y && m_prevPos.y + 10.f >= Transform()->GetRelativePos().y)
		{
			GamePlayStatic::DestroyGameObject(GetOwner());
		}
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
	if (m_Shooter != _OtherObj && m_Target == _OtherObj)
	{
		m_Target->GetScript<CChampScript>()->Damaged(m_Shooter, m_Target);
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}

void CArrowScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CArrowScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}
