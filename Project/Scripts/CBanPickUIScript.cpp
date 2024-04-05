#include "pch.h"
#include "CBanPickUIScript.h"

CBanPickUIScript::CBanPickUIScript()
	: CScript(BANPICKUISCRIPT)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
{
}

CBanPickUIScript::CBanPickUIScript(const CBanPickUIScript& _Origin)
	: CScript(BANPICKUISCRIPT)
	, m_bUIPos(false)
	, m_UIPosTime(0.f)
{
}

CBanPickUIScript::~CBanPickUIScript()
{
}

void CBanPickUIScript::begin()
{
}

void CBanPickUIScript::tick()
{
	if (!m_bUIPos)
		SetSlotPos();
}


void CBanPickUIScript::SetSlotPos()
{
	m_UIPosTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float BtwTime = 1.f - m_UIPosTime;

	if (m_UIPosTime < 0.13f)
	{
		vPos.y += 4500.f * DT * BtwTime;
		if (vPos.y >= 0.f)
			vPos.y = 0.f;

		Transform()->SetRelativePos(vPos);
	}
	else if (m_UIPosTime >= 0.13f && m_UIPosTime < 1.f)
	{
		vPos.y += 660.f * DT * BtwTime;
		if (vPos.y >= 0.f)
			vPos.y = 0.f;

		Transform()->SetRelativePos(vPos);
	}
	else
	{
		vPos.y = 0.f;
		Transform()->SetRelativePos(vPos);
		m_UIPosTime = 0.f;
		m_bUIPos = true;
	}
}
