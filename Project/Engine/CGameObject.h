#pragma once
#include "CEntity.h"

#define GET_COMPONENT(Type, TYPE) class C##Type* Type() {return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE];}

class CComponent;
class CRenderComponent;
class CScript;
class CCamera;
class CCollider2D;
class CLight2D;
class CStateMachine;

class CGameObject :
    public CEntity
{
private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;

    vector<CScript*>        m_vecScript;
    vector<CGameObject*>    m_vecChild;

    CGameObject*            m_Parent;

    int                     m_iLayerIdx;    // 오브젝트가 소속되어있는 레이어의 인덱스

    bool                    m_bDead;
    bool                    m_Active;

public:
    void begin();
    void tick();
    virtual void finaltick();
    void render();

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(StateMachine, STATEMACHINE);
    
public:
    void AddComponent(CComponent* _Component);
    void AddChild(CGameObject* _Child);
   
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    CGameObject* GetParent() { return m_Parent; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }

    void SetLayerIdx(int _Idx) { m_iLayerIdx = _Idx; }
    int GetLayerIdx() { return m_iLayerIdx; }

    const vector<CScript*>& GetScripts() { return m_vecScript; }

    template<typename T>
    T* GetScript()
    {
        for (size_t i = 0; i < m_vecScript.size(); ++i)
        {
            if (dynamic_cast<T*>(m_vecScript[i]))
            {
                return (T*)m_vecScript[i];
            }

            return nullptr;
        }
    }

    int DisconnectWithParent();
    int DisconnectWithLayer();

    void SetActive(bool _active) { m_Active = _active; }
    bool IsActive() { return m_Active; }
    bool IsDead() { return m_bDead; }

    void Destroy();

    bool IsAncestor(CGameObject* _Other);

public:
    CLONE(CGameObject);
    CGameObject();
    CGameObject(const CGameObject& _OriginObject);
    ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
};

