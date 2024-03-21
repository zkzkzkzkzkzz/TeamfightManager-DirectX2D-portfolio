#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CFontMgr.h"


CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_fTime(0.f)
	, m_bLock(true)
	, m_szText{}
	, m_font{}
{	

}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init()
{
	// 초당 빈도
	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_CurCount);

	m_EngineDeltaTime = m_DeltaTime = double(m_CurCount.QuadPart - m_PrevCount.QuadPart) / double(m_Frequency.QuadPart);

	if (m_bLock)
	{
		m_DeltaTime = 0.f;
	}

	m_PrevCount = m_CurCount;

	// DT 보정
	if ((1.f / 60.f) < m_DeltaTime)
		m_DeltaTime = (1.f / 60.f);


	// 시간 누적 ==> 1초마다 if 구문 실행
	m_fTime += m_EngineDeltaTime;
	if (1.f <= m_fTime)
	{
		swprintf_s(m_szText, 256, L"DeltaTime : %f, FPS : %d, Font : %s", m_DeltaTime, m_iCall, m_font);

		m_iCall = 0;
		m_fTime = 0.f;
	}

	++m_iCall;

	g_global.g_dt = m_DeltaTime;
	g_global.g_time += m_DeltaTime;
}

void CTimeMgr::render()
{
	m_font = L"Galmuri14";
	// 폰트 출력
	CFontMgr::GetInst()->DrawFont(m_szText, m_font, 10.f, 30.f, 16, FONT_RGBA(255, 30, 30, 255));
}
