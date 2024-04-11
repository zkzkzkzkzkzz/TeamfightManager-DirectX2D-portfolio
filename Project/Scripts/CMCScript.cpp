#include "pch.h"
#include "CMCScript.h"



CMCScript::CMCScript()
	: CScript(MCSCRIPT)
	, m_idx(TEXT_INDEX::END)
{
}

CMCScript::CMCScript(const CMCScript& _Origin)
	: CScript(MCSCRIPT)
	, m_idx(TEXT_INDEX::END)
{
}

CMCScript::~CMCScript()
{
}


void CMCScript::begin()
{

}

void CMCScript::tick()
{

}
