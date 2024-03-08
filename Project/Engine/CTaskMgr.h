#pragma once
#include "singleton.h"

enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Adress
	CREATE_OBJECT,

	// Param1 : Object Address
	DELETE_OBJECT,

	// Param1 : Asset Address
	ADD_ASSET,

	// Param1 : Asset Type, Parma2 : Asset Address
	DELETE_ASSET,

	// Param1 : Level, Param2 : LEVEL_STATE
	CHANGE_LEVELSTATE,

	CHANGE_NAME,

	// Param1 : Level Address, Param2 : Next Level Start State
	CHANGE_LEVEL,

	// Param1 : Parent Object, Param2 : Child Object
	ADD_CHILD,

	// Param1 : Parent Object, Param2 : Child Object
	DISCONNECT_PARENT,
};

struct tTask
{
	TASK_TYPE	Type;
	UINT_PTR	Param_1;
	UINT_PTR	Param_2;
};

class CTaskMgr :
    public CSingleton<CTaskMgr>
{
    SINGLE(CTaskMgr);

private:
    vector<tTask>   m_vecTask;

	bool	m_bCreateObject;
	bool	m_bDeleteObject;
	bool	m_bAssetChange;
	bool	m_bNameChange;

	int		m_DeleteFrameCount;

public:
	void tick();
	void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }

	bool GetObjectEvent() { return m_bCreateObject || m_bDeleteObject; }
	bool GetAssetEvent() { return m_bAssetChange; }
	bool GetNameEvent() { return m_bNameChange; }

private:
	void Clear();
};

