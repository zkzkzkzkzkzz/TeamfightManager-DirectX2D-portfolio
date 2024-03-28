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
	, m_Speed(50.f)
	, m_Pos{}
	, m_Rotation{}
{
}

CArrowScript::CArrowScript(const CArrowScript& _Origin)
	: CScript(ARROWSCRIPT)
	, m_Tex(_Origin.m_Tex)
	, m_Shooter(nullptr)
	, m_Target(nullptr)
	, m_Speed(50.f)
	, m_Pos{}
	, m_Rotation{}
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
	if (m_Target)
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
		int HP = m_Target->GetScript<CChampScript>()->GetChampHP();
		int DEF = m_Target->GetScript<CChampScript>()->GetChampDEF();
		int ATK = m_Shooter->GetScript<CChampScript>()->GetChampATK();

		m_Target->GetScript<CChampScript>()->Damaged(HP, ATK, DEF);
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}

void CArrowScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CArrowScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	int a = 0;
}
