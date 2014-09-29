#include "CThreadPool.h"
CThreadPool::CThreadPool(DWORD dwNum ) : _lThreadNum(0), _lRunningNum(0) ,m_exit(false)
{
	InitializeCriticalSection(&_csThreadVector);
	InitializeCriticalSection(&_csWorkQueue);
	_EventComplete = CreateEvent(0, false, false, NULL);
	_EventEnd = CreateEvent(0, true, false, NULL);
	_EventActive = CreateEvent(0, true, true, NULL);
	_EventFinished = CreateEvent(0, true, false, NULL);
	_SemaphoreCall = CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
	_SemaphoreDel =  CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
	assert(_SemaphoreCall != INVALID_HANDLE_VALUE);
	assert(_EventComplete != INVALID_HANDLE_VALUE);
	assert(_EventEnd != INVALID_HANDLE_VALUE);
	assert(_EventActive != INVALID_HANDLE_VALUE);
	assert(_EventFinished != INVALID_HANDLE_VALUE);
	assert(_SemaphoreDel != INVALID_HANDLE_VALUE);
	AdjustSize(dwNum <= 0 ? 4 : dwNum);
}
CThreadPool::~CThreadPool(){
	DeleteCriticalSection(&_csWorkQueue);
	CloseHandle(_EventEnd);
	CloseHandle(_EventActive);
	CloseHandle(_EventFinished);
	CloseHandle(_EventComplete);
	CloseHandle(_SemaphoreCall);
	CloseHandle(_SemaphoreDel);
	vector<ThreadItem*>::iterator iter;
	for(iter = _ThreadVector.begin(); iter != _ThreadVector.end(); iter++)
	{
		if(*iter)
			delete *iter;
	}
	DeleteCriticalSection(&_csThreadVector);
}
int CThreadPool::AdjustSize(int iNum)
{
	if(iNum > 0)
	{
		ThreadItem *pNew;
		EnterCriticalSection(&_csThreadVector);
		for(int _i=0; _i<iNum; _i++)
		{
			_ThreadVector.push_back(pNew = new ThreadItem(this)); 
			assert(pNew);
			pNew->_Handle = CreateThread(NULL, 0, DefaultJobProc, pNew, 0, NULL);
			// set priority
			SetThreadPriority(pNew->_Handle, THREAD_PRIORITY_BELOW_NORMAL);
			assert(pNew->_Handle);
		}
		LeaveCriticalSection(&_csThreadVector);
	}
	else
	{
		iNum *= -1;
		ReleaseSemaphore(_SemaphoreDel,  iNum > _lThreadNum ? _lThreadNum : iNum, NULL);
	}
	return (int)_lThreadNum;
}
void CThreadPool::Call(void (*pFunc)(void  *), void *pPara)
{
	assert(pFunc);
	EnterCriticalSection(&_csWorkQueue);
	_JobQueue.push(new JobItem(pFunc, pPara));
	ResetEvent(_EventFinished);
	LeaveCriticalSection(&_csWorkQueue);
	ReleaseSemaphore(_SemaphoreCall, 1, NULL);
}
bool CThreadPool::EndAndWait(DWORD dwWaitTime)
{
	SetEvent(_EventEnd);
	SetEvent(_EventActive);
	bool x=WaitForSingleObject(_EventComplete, dwWaitTime) == WAIT_OBJECT_0;
	ResetEvent(_EventEnd);
	return x;
}
bool CThreadPool::Wait(DWORD dwWaitTime)
{
	HANDLE hd[2]={_EventComplete,_EventFinished};
	WaitForMultipleObjects(2,hd,false, dwWaitTime);
	ResetEvent(_EventFinished);
// 	printf("等待结束\n");
	return true;
}
DWORD WINAPI CThreadPool::DefaultJobProc(LPVOID lpParameter )
{
	ThreadItem *pThread = static_cast<ThreadItem*>(lpParameter);
	assert(pThread);
	CThreadPool *pThreadPoolObj = pThread->_pThis;
	assert(pThreadPoolObj);
	InterlockedIncrement(&pThreadPoolObj->_lThreadNum);
	HANDLE hWaitHandle[3];
	hWaitHandle[0] = pThreadPoolObj->_SemaphoreCall;
	hWaitHandle[1] = pThreadPoolObj->_SemaphoreDel;
	hWaitHandle[2] = pThreadPoolObj->_EventEnd;
	JobItem *pJob;
	bool fHasJob=true;
	for(;;)
	{
		DWORD wr = WaitForMultipleObjects(3, hWaitHandle, false, INFINITE);
		WaitForSingleObject(pThreadPoolObj->_EventActive, INFINITE);
		//响应删除线程信号
		// 			if (wr==WAIT_OBJECT_0+2)
		// 			{
		// 				printf("WaitEnd\n");
		// 			}
		if(wr == WAIT_OBJECT_0 + 1 ||pThreadPoolObj->m_exit)  
			break;
		//从队列里取得用户作业
		EnterCriticalSection(&pThreadPoolObj->_csWorkQueue);
		if(fHasJob = !pThreadPoolObj->_JobQueue.empty())
		{
			pJob = pThreadPoolObj->_JobQueue.front();
			pThreadPoolObj->_JobQueue.pop();
			assert(pJob);
		}
		LeaveCriticalSection(&pThreadPoolObj->_csWorkQueue);
		//受到结束线程信号 确定是否结束线程(结束线程信号 && 是否还有工作)
		if(wr == WAIT_OBJECT_0 + 2 && !fHasJob)  
			break;
		if(fHasJob && pJob)
		{
			InterlockedIncrement(&pThreadPoolObj->_lRunningNum);
			pThread->_dwLastBeginTime = GetTickCount();
			pThread->_dwCount++;
			pThread->_fIsRunning = true;
			pJob->_pFunc(pJob->_pPara); //运行用户作业
			delete pJob; 
			pThread->_fIsRunning = false;
			InterlockedDecrement(&pThreadPoolObj->_lRunningNum);
			EnterCriticalSection(&pThreadPoolObj->_csWorkQueue);
			if (pThreadPoolObj->_JobQueue.empty()&&pThreadPoolObj->_lRunningNum==0)
			{
				SetEvent(pThreadPoolObj->_EventFinished);
			}
			LeaveCriticalSection(&pThreadPoolObj->_csWorkQueue);
		}
	}
	//删除自身结构
	EnterCriticalSection(&pThreadPoolObj->_csThreadVector);
	pThreadPoolObj->_ThreadVector.erase(find(pThreadPoolObj->_ThreadVector.begin(), pThreadPoolObj->_ThreadVector.end(), pThread));
	LeaveCriticalSection(&pThreadPoolObj->_csThreadVector);
	delete pThread;
	InterlockedDecrement(&pThreadPoolObj->_lThreadNum);
	if(!pThreadPoolObj->_lThreadNum)  //所有线程结束
		SetEvent(pThreadPoolObj->_EventComplete);
// 	printf("线程结束\n");
	return 0;
}
void CThreadPool::CallProc(void *pPara) 
{
	CallProcPara *cp = static_cast<CallProcPara *>(pPara);
	assert(cp);
	if(cp)
	{
		cp->_pObj->Run(cp->_pPara);
		delete cp;
	}
}
bool CThreadPool::IsRunning()
{
	return _lRunningNum > 0;
}
void CThreadPool::Suspend(){
	ResetEvent(_EventActive);
}
void CThreadPool::Rusume(){
	SetEvent(_EventActive);
}
void CThreadPool::Stop()
{
	m_exit=true;
	SetEvent(_EventActive);
}
void CThreadPool::End()
{
	Stop();
	EndAndWait();
}