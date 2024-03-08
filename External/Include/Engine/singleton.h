#pragma once

template<typename T>
class CSingleton
{
private:
	static T* m_This;

public:
	static T* GetInst()
	{
		if (nullptr == m_This)
		{
			m_This = new T;
		}

		return m_This;
	}

	static void Destroy()
	{
		if (nullptr != m_This)
		{
			delete m_This;
			m_This = nullptr;
		}
	}

public:
	CSingleton()
	{
		// 메인함수 종료될 때 인자에 있는 함수를 호출하는 함수
		typedef void(*FUNC_TYPE)(void);
		atexit((FUNC_TYPE)(&CSingleton::Destroy));
	}
	~CSingleton()
	{

	}
};

template<typename T>
T* CSingleton<T>::m_This = nullptr;