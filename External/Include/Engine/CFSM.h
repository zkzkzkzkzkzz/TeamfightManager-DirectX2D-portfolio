#pragma once
#include "CAsset.h"

#include "CState.h"
#include "CBlackboard.h"

class CStateMachine;

class CFSM :
    public CAsset
{
private:
    CFSM*           m_Master;
    CStateMachine*  m_StateMachine; // FSM을 사용하는 StateMachine

    map<wstring, CState*>   m_mapState;
    CBlackboard*            m_Blackboard;
    
    CState* m_CurState;

public:
    void AddState(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);

    void SetState(const wstring& _strState);
    void SetStateMachine(CStateMachine* _SM)
    {
        if (m_Master)
            m_StateMachine = _SM;
    }

    CFSM* GetFSMInstance();
    CStateMachine* GetStateMachine() { return m_StateMachine; }
    void ChangeState(const wstring& _strStateName);

public:
    void finaltick();

    CLONE_DISABLE(CFSM);

public:
    CFSM(bool _bEngine = false);
    ~CFSM();
};

