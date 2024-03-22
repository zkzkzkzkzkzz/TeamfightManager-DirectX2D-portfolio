#include "pch.h"
#include "CGamer.h"



CGamer::CGamer()
	: m_GamerName{}
	, m_HairTex(nullptr)
	, m_BodyTex(nullptr)
	, m_GamerATK(0)
	, m_GamerDEF(0)
	, m_GamerAge(0)
{
}

CGamer::CGamer(const CGamer& _Origin)
	: m_GamerName{}
	, m_HairTex(_Origin.m_HairTex)
	, m_BodyTex(_Origin.m_BodyTex)
	, m_GamerATK(_Origin.m_GamerATK)
	, m_GamerDEF(_Origin.m_GamerDEF)
	, m_GamerAge(_Origin.m_GamerAge)
{
}

CGamer::~CGamer()
{
}

void CGamer::begin()
{
}

void CGamer::tick()
{
}

void CGamer::finaltick()
{
}

void CGamer::render()
{
}
