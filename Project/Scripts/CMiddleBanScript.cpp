#include "pch.h"
#include "CMiddleBanScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include "CBanpickLevel.h"

CMiddleBanScript::CMiddleBanScript()
	: CScript(MIDDLEBANSCRIPT)
	, m_Arrow(nullptr)
	, m_Level(nullptr)
	, m_Text(nullptr)
{
}

CMiddleBanScript::CMiddleBanScript(const CMiddleBanScript& _Origin)
	: CScript(MIDDLEBANSCRIPT)
	, m_Arrow(nullptr)
	, m_Level(nullptr)
	, m_Text(nullptr)
{
}

CMiddleBanScript::~CMiddleBanScript()
{
}

void CMiddleBanScript::begin()
{
	Transform()->SetRelativePos(Vec3(0.f, 240.f, -10.f));
	Transform()->SetRelativeScale(Vec3(1280.f, 80.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BanPickMiddleBan.mtrl"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);

	InitArrow();
	InitText();

	m_Level = (CBanpickLevel*)CLevelMgr::GetInst()->GetCurrentLevel();
}

void CMiddleBanScript::tick()
{
	BANPICK_STATE state = m_Level->GetCurBanPickState();

	switch (state)
	{
	case BANPICK_STATE::NONE:
		break;
	case BANPICK_STATE::BLUEBAN:
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		m_Arrow->Transform()->SetRelativePos(Vec3(-380.f, 0.f, -10.f));
		m_Arrow->Animator2D()->Play(L"TurnArrowBlue");
		m_Text->TextRender()->SetString(L"Pick a Champion you want to ban.");
		m_Text->TextRender()->SetFontSize(30.f);
		m_Text->TextRender()->SetOffsetPos(Vec3(-250.f, -12.f, -10.f));
		break;
	case BANPICK_STATE::REDBAN:
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		m_Arrow->Transform()->SetRelativePos(Vec3(380.f, 0.f, -10.f));
		m_Arrow->Animator2D()->Play(L"TurnArrowRed");
		m_Text->TextRender()->SetString(L"Red Team is picking the champion to ban.");
		m_Text->TextRender()->SetFontSize(25.f);
		m_Text->TextRender()->SetOffsetPos(Vec3(-291.f, -12.f, -10.f));
		break;
	case BANPICK_STATE::BLUEPICK1:
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		m_Arrow->Transform()->SetRelativePos(Vec3(-380.f, 0.f, -10.f));
		m_Arrow->Animator2D()->Play(L"TurnArrowBlue");
		m_Text->TextRender()->SetString(L"Pick a Champion you want to use.");
		m_Text->TextRender()->SetFontSize(30.f);
		m_Text->TextRender()->SetOffsetPos(Vec3(-250.f, -12.f, -10.f));
		break;
	case BANPICK_STATE::REDPICK1:
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		m_Arrow->Transform()->SetRelativePos(Vec3(380.f, 0.f, -10.f));
		m_Arrow->Animator2D()->Play(L"TurnArrowRed");
		m_Text->TextRender()->SetString(L"Red Team is picking the champion(s) to use.");
		m_Text->TextRender()->SetFontSize(25.f);
		m_Text->TextRender()->SetOffsetPos(Vec3(-291.f, -12.f, -10.f));
		break;
	case BANPICK_STATE::BLUEPICK2:
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		m_Arrow->Transform()->SetRelativePos(Vec3(-380.f, 0.f, -10.f));
		m_Arrow->Animator2D()->Play(L"TurnArrowBlue");
		m_Text->TextRender()->SetString(L"Pick a Champion you want to use.");
		m_Text->TextRender()->SetFontSize(30.f);
		m_Text->TextRender()->SetOffsetPos(Vec3(-250.f, -12.f, -10.f));
		break;
	case BANPICK_STATE::REDPICK2:
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		m_Arrow->Transform()->SetRelativePos(Vec3(380.f, 0.f, -10.f));
		m_Arrow->Animator2D()->Play(L"TurnArrowRed");
		m_Text->TextRender()->SetString(L"Red Team is picking the champion(s) to use.");
		m_Text->TextRender()->SetFontSize(25.f);
		m_Text->TextRender()->SetOffsetPos(Vec3(-291.f, -12.f, -10.f));
		break;
	case BANPICK_STATE::BATTLE:
		break;
	case BANPICK_STATE::DONE:
		break;
	case BANPICK_STATE::END:
		break;
	default:
		break;
	}
}

void CMiddleBanScript::InitArrow()
{
	m_Arrow = new CGameObject;
	m_Arrow->SetName(L"TurnArrow");
	m_Arrow->AddComponent(new CTransform);
	m_Arrow->AddComponent(new CMeshRender);
	m_Arrow->AddComponent(new CAnimator2D);
	m_Arrow->Transform()->SetRelativePos(Vec3(-380.f, 0.f, -10.f));
	m_Arrow->Transform()->SetRelativeScale(Vec3(64.f, 42.f, 1.f));
	m_Arrow->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_Arrow->MeshRender()->SetMaterial(GetOwner()->MeshRender()->GetDynamicMaterial());
	m_Arrow->Animator2D()->LoadAnimation(L"animdata\\TurnArrowBlue.txt");
	m_Arrow->Animator2D()->LoadAnimation(L"animdata\\TurnArrowRed.txt");
	GetOwner()->AddChild(m_Arrow);
}

void CMiddleBanScript::InitText()
{
	m_Text = new CGameObject;
	m_Text->SetName(L"MiddleBanText");
	m_Text->AddComponent(new CTransform);
	m_Text->AddComponent(new CTextRender);
	m_Text->TextRender()->SetFont(L"Galmuri14");
	m_Text->TextRender()->SetFontSize(30.f);
	m_Text->TextRender()->SetOffsetPos(Vec3(-250.f, -12.f, -10.f));
	m_Text->TextRender()->SetFontColor(255, 255, 255, 255);
	GetOwner()->AddChild(m_Text);
}
