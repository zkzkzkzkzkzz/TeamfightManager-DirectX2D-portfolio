#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CGameObject.h>

#include <Engine\CMaterial.h>
#include <Engine\CRenderComponent.h>

CPlayerScript::CPlayerScript()
	: CScript(SCRIPT_TYPE::PLAYERSCRIPT)
	, m_Speed(300.f)
{

}

CPlayerScript::~CPlayerScript()
{
}



void CPlayerScript::begin()
{
	//Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\link.png", L"texture\\link.png");
	//Animator2D()->Create(L"test", pAtlasTex, Vec2(0.f, 0.f), Vec2(120, 130), Vec2(0.f, -60.f), Vec2(200.f, 200.f), 3, 1);
	//Animator2D()->SaveAnimations(L"animdata");

	//Animator2D()->LoadAnimation(L"animdata\\IdleDown.txt");
	//Animator2D()->LoadAnimation(L"animdata\\i.txt");
	//Animator2D()->Play(L"i");

	//GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(UP))
	{
		vPos.y += DT * m_Speed;
	}
	if (KEY_TAP(UP))
	{
		Animator2D()->Play(L"Idle_Down");
	}
	if (KEY_RELEASED(UP))
	{
		Animator2D()->Play(L"IDLE_UP");
	}


	if (KEY_PRESSED(DOWN))
	{
		vPos.y -= DT * m_Speed;
	}
	if (KEY_TAP(DOWN))
	{
		Animator2D()->Play(L"MOVE_DOWN");
	}
	if (KEY_RELEASED(DOWN))
	{
		Animator2D()->Play(L"IDLE_DOWN");
	}


	if (KEY_PRESSED(LEFT))
	{
		vPos.x -= DT * m_Speed;
	}
	if (KEY_TAP(LEFT))
	{
		Animator2D()->Play(L"MOVE_LEFT");
	}
	if (KEY_RELEASED(LEFT))
	{
		Animator2D()->Play(L"IDLE_LEFT");
	}


	if (KEY_PRESSED(RIGHT))
	{
		vPos.x += DT * m_Speed;
	}
	if (KEY_TAP(RIGHT))
	{
		Animator2D()->Play(L"MOVE_RIGHT");
	}
	if (KEY_RELEASED(RIGHT))
	{
		Animator2D()->Play(L"IDLE_RIGHT");
	}


	if (KEY_PRESSED(X))
	{
		vRot.x += DT * XM_PI;
	}

	if (KEY_PRESSED(Y))
	{
		vRot.y += DT * XM_PI;
	}

	if (KEY_PRESSED(Z))
	{
		vRot.z += DT * XM_PI;
	}

	if (KEY_TAP(SPACE))
	{
		GamePlayStatic::Play2DSound(L"sound\\DM.wav", 1, 0.5f, true);
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Ptr<CMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
