#include "pch.h"
#include "CKeyMgr.h"
#include "CEngine.h"

int g_KeySync[(UINT)KEY::KEY_END] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4,
	VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,	

	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',

	VK_TAB,	VK_LSHIFT, VK_LMENU, VK_LCONTROL, VK_SPACE,

	VK_ESCAPE, VK_RETURN, VK_BACK, VK_DELETE,

	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,

	VK_LBUTTON,	VK_RBUTTON,
};


CKeyMgr::CKeyMgr()
{
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::init()
{
	// Capaticy 를 미리 확보
	m_vecKeyData.reserve((size_t)KEY::KEY_END);

	// 데이터 개수를 늘림
	//m_vecKeyData.resize(KEY::KEY_END);

	for (UINT i = 0; i < (UINT)KEY::KEY_END; ++i)
	{
		m_vecKeyData.push_back(FKeyData{(KEY)i, KEY_STATE::NONE, false});	
	}
}

void CKeyMgr::tick()
{	 
	if (nullptr == GetFocus())
	{
		for (size_t i = 0; i < m_vecKeyData.size(); ++i)
		{
			if (KEY_STATE::TAP == m_vecKeyData[i].eState)
			{
				m_vecKeyData[i].eState = KEY_STATE::PRESSED;
			}
			else if (KEY_STATE::PRESSED == m_vecKeyData[i].eState)
			{
				m_vecKeyData[i].eState = KEY_STATE::RELEASED;
			}
			else if (KEY_STATE::RELEASED == m_vecKeyData[i].eState)
			{
				m_vecKeyData[i].eState = KEY_STATE::NONE;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecKeyData.size(); ++i)
		{
			if (GetAsyncKeyState(g_KeySync[(int)m_vecKeyData[i].eKey]) & 0x8001)
			{
				// 이번 프레임에 해당 키가 눌려있다.
				if (m_vecKeyData[i].bPressed)
				{
					// 이전에도 눌려있었다 ==> 계속 눌림 상태
					m_vecKeyData[i].eState = KEY_STATE::PRESSED;
				}
				else
				{
					// 이전에는 눌려있지 않았다 ==> 막 눌림 상태
					m_vecKeyData[i].eState = KEY_STATE::TAP;
				}

				m_vecKeyData[i].bPressed = true;
			}
			else
			{
				// 이번 프레임에 해당키는 안 눌려 있다
				if (m_vecKeyData[i].bPressed)
				{
					// 이전 프레임에는 눌려 있었다 ==> 막 뗌
					m_vecKeyData[i].eState = KEY_STATE::RELEASED;
				}
				else
				{
					m_vecKeyData[i].eState = KEY_STATE::NONE;
				}

				m_vecKeyData[i].bPressed = false;
			}
		}

		// 마우스 이전 좌표 저장
		m_vMousePrevPos = m_vMousePos;

		// 마우스 좌표 계산
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(CEngine::GetInst()->GetMainWind(), &pt);
		m_vMousePos = Vec2((float)pt.x, (float)pt.y);

		// 마우스가 이동한 방향
		m_vMouseDrag = m_vMousePos - m_vMousePrevPos;
	}
}