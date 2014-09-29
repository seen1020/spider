#ifndef _CThreadPool_H_
#define _CThreadPool_H_
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#include <cassert>
#include <vector>
#include <queue>
#include "CThread.h"
#include <windows.h>
using namespace std;
class CThreadPool
{
public:
	//dwNum 线程池规模
	CThreadPool(DWORD dwNum = 4);
	~CThreadPool();
	//调整线程池规模
	int AdjustSize(int iNum);
	//调用线程池
	void Call(void (*pFunc)(void  *), void *pPara = NULL);
	//调用线程池
	inline void Call(CThread * p, void *pPara = NULL)
	{
		Call(CallProc, new CallProcPara(p, pPara));
	}
	//结束线程池, 并同步等待
	bool EndAndWait(DWORD dwWaitTime = INFINITE);
	bool Wait(DWORD dwWaitTime = INFINITE);
	//结束线程池
	void Stop();
	void End();
	inline DWORD Size()
	{
		return (DWORD)_lThreadNum;
	}
	inline DWORD GetRunningSize()
	{
		return (DWORD)_lRunningNum;
	}
	bool IsRunning();
	void Suspend();
	void Rusume();
protected:
	//工作线程
	static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL);
	//调用用户对象虚函数
	static void CallProc(void *pPara) ;
	//用户对象结构
	struct CallProcPara  
	{
		CThread* _pObj;//用户对象 
		void *_pPara;//用户参数
		CallProcPara(CThread* p, void *pPara) : _pObj(p), _pPara(pPara) { };
	};
	//用户函数结构
	struct JobItem 
	{
		void (*_pFunc)(void  *);//函数
		void *_pPara; //参数
		JobItem(void (*pFunc)(void  *) = NULL, void *pPara = NULL) : _pFunc(pFunc), _pPara(pPara) { };
	};
	//线程池中的线程结构
	struct ThreadItem
	{
		HANDLE _Handle; //线程句柄
		CThreadPool *_pThis;  //线程池的指针
		DWORD _dwLastBeginTime; //最后一次运行开始时间
		DWORD _dwCount; //运行次数
		bool _fIsRunning;
		ThreadItem(CThreadPool *pthis) : _pThis(pthis), _Handle(NULL), _dwLastBeginTime(0), _dwCount(0), _fIsRunning(false) { };
		~ThreadItem()
		{
			if(_Handle)
			{
				CloseHandle(_Handle);
				_Handle = NULL;
			}
		}
	};
	std::queue<JobItem *> _JobQueue;  //工作队列
	std::vector<ThreadItem *>  _ThreadVector; //线程数据
	CRITICAL_SECTION _csThreadVector, _csWorkQueue; //工作队列临界, 线程数据临界
	HANDLE _EventEnd,_EventActive,_EventFinished, _EventComplete, _SemaphoreCall, _SemaphoreDel;//结束通知, 完成事件, 工作信号， 删除线程信号
	long _lThreadNum, _lRunningNum; //线程数, 运行的线程数
	bool m_exit;
};
#endif //_ThreadPool_H_