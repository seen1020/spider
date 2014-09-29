#ifndef CCRITICALSECTION_H
#define CCRITICALSECTION_H
#include <WINDOWS.H>
class CCriticalSection
{
public:
	CCriticalSection()
	{
		InitializeCriticalSection(&m_criticalSection);
	}
	~CCriticalSection()
	{
		DeleteCriticalSection(&m_criticalSection);	
	}
	void Enter()
	{
		EnterCriticalSection(&m_criticalSection);
	}
	void Leave()
	{
		LeaveCriticalSection(&m_criticalSection);
	}
protected:
	CRITICAL_SECTION m_criticalSection;
private:
};
#endif

