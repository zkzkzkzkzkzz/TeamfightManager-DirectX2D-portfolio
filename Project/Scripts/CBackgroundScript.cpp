#include "pch.h"
#include "CBackgroundScript.h"

#include <Engine\CKeyMgr.h>

CBackgroundScript::CBackgroundScript()
	: CScript(SCRIPT_TYPE::BACKGROUNDSCRIPT)
{
}

CBackgroundScript::~CBackgroundScript()
{
}

void CBackgroundScript::tick()
{
	Vec3 vDir = Light2D()->GetDir();
	float vAngle = Light2D()->GetAngle();
	if (KEY_PRESSED(J))
	{
		vDir = Rotate(vDir, XM_PI / 2.f * DT);
	}
	if (KEY_PRESSED(K))
	{
		vDir = Rotate(vDir, -XM_PI / 2.f * DT);
	}

	if (KEY_PRESSED(M))
	{
		vAngle += DT * 3.f;
	}

	if (KEY_PRESSED(N))
	{
		vAngle += DT * -3.f;
	}

	Light2D()->SetDir(vDir);
	Light2D()->SetAngle(vAngle);
}

Vec3 CBackgroundScript::Rotate(Vec3 _vDir, float _angle)
{
	Vec3 vRotateDir = Vec3(cosf(_angle) * _vDir.x - sinf(_angle) * _vDir.y
		, sinf(_angle) * _vDir.x + cosf(_angle) * _vDir.y, _vDir.z);

	return vRotateDir;
}

void CBackgroundScript::SaveToFile(FILE* _File)
{
}

void CBackgroundScript::LoadFromFile(FILE* _File)
{
}
