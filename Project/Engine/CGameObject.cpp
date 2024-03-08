#include "pch.h"
#include "CGameObject.h"

#include "CGC.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CComponent.h"
#include "CRenderComponent.h"

#include "CScript.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1) // 어떠한 레벨 및 레이어에도 소속되어 있지 않다
	, m_bDead(false)
{
}

CGameObject::CGameObject(const CGameObject& _OriginObject)
	: CEntity(_OriginObject)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _OriginObject.m_arrCom[i])
		{
			continue;
		}

		AddComponent(_OriginObject.m_arrCom[i]->Clone());
	}

	for (size_t i = 0; i < _OriginObject.m_vecScript.size(); ++i)
	{
		AddComponent(_OriginObject.m_vecScript[i]->Clone());
	}

	// 복사되는 GameObject는 부모만 레이어 소속을 -1로 하고
	// 자식들은 원본객체랑 동일한 레이어 소속을 유지한다
	for (size_t i = 0; i < _OriginObject.m_vecChild.size(); ++i)
	{
		CGameObject* ChildClone = _OriginObject.m_vecChild[i]->Clone();
		AddChild(ChildClone);
		ChildClone->m_iLayerIdx = _OriginObject.m_vecChild[i]->m_iLayerIdx;
	}
}

CGameObject::~CGameObject()
{
	// 명시적으로 적는다면 Delete_Array<CComponent, 14>(m_arrCom)
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChild);
}

void CGameObject::begin()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->begin();
		}
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->tick();
		}
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->tick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->finaltick();
		}
	}

	CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterGameObject(this);

	vector<CGameObject*>::iterator iter = m_vecChild.begin();

	for (; iter != m_vecChild.end();)
	{
		if ((*iter)->m_bDead)
		{
			CGC::GetInst()->Add(*iter);
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}
}

void CGameObject::render()
{
	if (nullptr != m_RenderCom)
	{
		m_RenderCom->render();
	}
}

void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE type = _Component->GetType();

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// type이 스크립트 클래스가 아닌 경우
		assert(dynamic_cast<CScript*>(_Component));

		m_vecScript.push_back((CScript*)_Component);
		_Component->m_Owner = this;
	}
	else
	{
		// 이미 type에 해당하는 컴포넌트 타입을 가지고 있을 경우
		assert(!m_arrCom[(UINT)type]);

		m_arrCom[(UINT)type] = _Component;
		_Component->m_Owner = this;

		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);

		if (nullptr != pRenderCom)
		{
			// 이미 한 종류 이상의 RenderComponent를 가지고 있는 경우
			assert(!m_RenderCom);

			m_RenderCom = pRenderCom;
		}
	}
}

//void CGameObject::DisconnectWithParent()
//{
//	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
//
//	for (; iter != m_Parent->m_vecChild.end(); ++iter)
//	{
//		if (*iter == this)
//		{
//			m_Parent->m_vecChild.erase(iter);
//			m_Parent = nullptr;
//			return;
//		}
//	}
//
//	// 부모가 없는 오브젝트에 해당 함수를 호출했거나
//	// 부모는 자식을 가리키지 않고 있는데, 자식은 부모를 가리키고 있는 경우
//	assert(nullptr);
//}
//
//void CGameObject::DisconnectWithLayer()
//{
//	if (-1 == m_iLayerIdx)
//	{
//		return;
//	}
//
//	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
//	CLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);
//	pCurLayer->DetachGameObject(this);
//}

int CGameObject::DisconnectWithParent()
{
	// 부모가 없는 오브젝트에 DisconnectWithParent 함수를 호출했으면
	if (nullptr == m_Parent)
		return -1;

	bool bSuccess = false;
	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			bSuccess = true;
			break;
		}
	}

	// 부모는 자식을 가리키기지 않고 있는데, 자식은 부모를 가리키고 있는 경우
	if (!bSuccess)
	{
		assert(nullptr);
	}

	int layeridx = m_iLayerIdx;

	m_iLayerIdx = -1;

	return layeridx;
}

int CGameObject::DisconnectWithLayer()
{
	if (-1 == m_iLayerIdx)
		return -1;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);

	int LayerIdx = m_iLayerIdx;
	pCurLayer->DetachGameObject(this);
	return LayerIdx;
}


void CGameObject::AddChild(CGameObject* _Child)
{
	if (-1 == _Child->m_iLayerIdx)
	{
		// 레벨에 속하지 않았던 오브젝트가 자식으로 들어올때는 부모의 레이어를 따라간다
		_Child->m_iLayerIdx = m_iLayerIdx;
	}
	else if (_Child->m_Parent)
	{
		// 이전 부모 오브젝트랑 연결 해제
		// 원래 레이어를 유지한다
		int LayerIdx = _Child->DisconnectWithParent();
		_Child->m_iLayerIdx = LayerIdx;
	}
	else
	{
		// 자식으로 들어오는 오브젝트가 최상위 부모타입이면
		// 소속 레이어의 Parent 오브젝트 목록에서 제거되어야 함
		// 제거되기 전의 레이어를 유지한다
		int LayerIdx = _Child->DisconnectWithLayer();
		_Child->m_iLayerIdx = LayerIdx;
	}

	// 부모 자식 연결
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}


void CGameObject::Destroy()
{
	GamePlayStatic::DestroyGameObject(this);
}

bool CGameObject::IsAncestor(CGameObject* _Other)
{
	CGameObject* pParent = m_Parent;

	while (pParent)
	{
		if (pParent == _Other)
			return true;
		
		pParent = pParent->m_Parent;
	}

	return false;
}
