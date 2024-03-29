#include "pch.h"
#include "CEffectScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CAnimator2D.h>

CEffectScript::CEffectScript()
	: CScript(EFFECTSCRIPT)
	, m_Pos{}
	, m_Scale{}
	, m_EffectName{}
	, m_AccTime(0.f)
	, m_Duration(0.f)
{
}

CEffectScript::CEffectScript(const CEffectScript& _Origin)
	: CScript(EFFECTSCRIPT)
	, m_Pos{}
	, m_Scale{}
	, m_EffectName{}
	, m_AccTime(0.f)
	, m_Duration(0.f)
{
}

CEffectScript::~CEffectScript()
{
}



void CEffectScript::begin()
{
	Transform()->SetRelativePos(m_Pos);
	Transform()->SetRelativeScale(m_Scale);
	Transform()->SetRelativeRotation(m_Rotation);
	
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffectMtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	if (Animator2D())
	{
		wstring RelativePath = L"animdata\\" + m_EffectName + L".txt";
		Animator2D()->LoadAnimation(RelativePath);
		Animator2D()->Play(m_EffectName, false);
	}
}

void CEffectScript::tick()
{
	m_Duration += DT;

	if (m_AccTime <= m_Duration)
	{
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
}


void CEffectScript::SetEffectInfo(Vec3 _Pos, Vec3 _Scale, Vec3 _Rotation, const wstring& _Name, float _time)
{
	m_Pos = _Pos;
	m_Scale = _Scale;
	m_Rotation = _Rotation;
	m_EffectName = _Name;
	m_AccTime = _time;
}