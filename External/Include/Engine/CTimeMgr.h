#pragma once




class CTimeMgr
	: public CSingleton<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	// Larget Integer 는 그냥 8바이트 long long 타입 정수로 취급
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;
	double			m_DeltaTime;
	double			m_EngineDeltaTime;

	UINT			m_iCall;
	float			m_fTime;

	bool			m_bLock;

public:
	double GetDeltaTime() { return m_DeltaTime; }
	double GetEngineDeltaTime() { return m_EngineDeltaTime; }

	void LockDeltaTime(bool _Lock) { m_bLock = _Lock; }

public:
	void init();
	void tick();
};

