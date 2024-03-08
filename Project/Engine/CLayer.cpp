#include "pch.h"
#include "CLayer.h"

#include "CGC.h"
#include "CGameObject.h"


CLayer::CLayer()
	: m_iLayerIdx(-1)
{
}

CLayer::CLayer(const CLayer& _OriginLayer)
	: CEntity(_OriginLayer)
	, m_iLayerIdx(_OriginLayer.m_iLayerIdx)
{
	for (size_t i = 0; i < _OriginLayer.m_vecParent.size(); ++i)
	{
		AddObject(_OriginLayer.m_vecParent[i]->Clone(), false);
	}
}

CLayer::~CLayer()
{
	Delete_Vec(m_vecParent);
}


void CLayer::begin()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->begin();
	}
}

void CLayer::tick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->tick();
	}
}

void CLayer::finaltick()
{
	vector<CGameObject*>::iterator iter = m_vecParent.begin();

	for (; iter != m_vecParent.end(); )
	{
		(*iter)->finaltick();

		if ((*iter)->IsDead())
		{
			CGC::GetInst()->Add(*iter);
			iter = m_vecParent.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}


void CLayer::DetachGameObject(CGameObject* _Object)
{
	assert(!(-1 == _Object->m_iLayerIdx || _Object->m_iLayerIdx != m_iLayerIdx));

	// 부모 오브젝트가 있는 경우
	if (nullptr != _Object->GetParent())
	{
		_Object->m_iLayerIdx = -1;
	}

	// 최상위 부모 오브젝트였을 경우
	else
	{
		vector<CGameObject*>::iterator iter = m_vecParent.begin();

		for (; iter != m_vecParent.end(); ++iter)
		{
			if (*iter == _Object)
			{
				m_vecParent.erase(iter);
				_Object->m_iLayerIdx = -1;
				return;
			}
		}
	}

	assert(nullptr);
}


// _bMove : true
// 레이어에 입력되는 Object가 자식이 있는 경우, 자식까지 모두 동일한 레이어로 넘어온다
// _bMove : false
// 레이어에 입력되는 Object의 자식은 해당 레이어로 같이 넘어오지 않는다
// 단, 자식 오브젝트가 레이어 소속이 없는 경우(-1)에만 같이 변경한다
void CLayer::AddObject(CGameObject* _Object, bool _bMove)
{
	// 오브젝트가 최상위 부모일 때
	if (nullptr == _Object->GetParent())
	{
		// 이미 다른 레이어에 소속되어 있었을 때
		if (-1 != _Object->m_iLayerIdx)
		{
			// 기존 레이어의 Parent 벡터에서 제거되어야 한다
			_Object->DisconnectWithLayer();
		}

		// 최상위 부모 오브젝트는 m_vecParent에서 가리키도록 한다
		m_vecParent.push_back(_Object);
	}

	// 오브젝트의 레이어 소속을 현재 레이어로 변경
	// 자식 오브젝트들의 레이어 소속 변경
	list<CGameObject*> queue;
	queue.push_back(_Object);

	// 레이어에 입력되는 오브젝트 및 그 밑에 달린 자식들까지 모두 확인
	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
		{
			queue.push_back(pObject->m_vecChild[i]);
		}

		// 최초 입력 오브젝트는 무조건 해당 레이어 소속으로 변경
		if (pObject == _Object)
		{
			pObject->m_iLayerIdx = m_iLayerIdx;
		}
		else
		{
			// _bMove가 true일 때
			if (_bMove)
			{
				pObject->m_iLayerIdx = m_iLayerIdx;
			}
			// _bMove가 false여도 자식 오브젝트가 어떠한 레이어에도 소속되어있지 않을 때
			else if(-1 == pObject->m_iLayerIdx)
			{
				pObject->m_iLayerIdx = m_iLayerIdx;
			}
		}
	}
}
