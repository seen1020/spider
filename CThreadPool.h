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
	//dwNum �̳߳ع�ģ
	CThreadPool(DWORD dwNum = 4);
	~CThreadPool();
	//�����̳߳ع�ģ
	int AdjustSize(int iNum);
	//�����̳߳�
	void Call(void (*pFunc)(void  *), void *pPara = NULL);
	//�����̳߳�
	inline void Call(CThread * p, void *pPara = NULL)
	{
		Call(CallProc, new CallProcPara(p, pPara));
	}
	//�����̳߳�, ��ͬ���ȴ�
	bool EndAndWait(DWORD dwWaitTime = INFINITE);
	bool Wait(DWORD dwWaitTime = INFINITE);
	//�����̳߳�
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
	//�����߳�
	static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL);
	//�����û������麯��
	static void CallProc(void *pPara) ;
	//�û�����ṹ
	struct CallProcPara  
	{
		CThread* _pObj;//�û����� 
		void *_pPara;//�û�����
		CallProcPara(CThread* p, void *pPara) : _pObj(p), _pPara(pPara) { };
	};
	//�û������ṹ
	struct JobItem 
	{
		void (*_pFunc)(void  *);//����
		void *_pPara; //����
		JobItem(void (*pFunc)(void  *) = NULL, void *pPara = NULL) : _pFunc(pFunc), _pPara(pPara) { };
	};
	//�̳߳��е��߳̽ṹ
	struct ThreadItem
	{
		HANDLE _Handle; //�߳̾��
		CThreadPool *_pThis;  //�̳߳ص�ָ��
		DWORD _dwLastBeginTime; //���һ�����п�ʼʱ��
		DWORD _dwCount; //���д���
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
	std::queue<JobItem *> _JobQueue;  //��������
	std::vector<ThreadItem *>  _ThreadVector; //�߳�����
	CRITICAL_SECTION _csThreadVector, _csWorkQueue; //���������ٽ�, �߳������ٽ�
	HANDLE _EventEnd,_EventActive,_EventFinished, _EventComplete, _SemaphoreCall, _SemaphoreDel;//����֪ͨ, ����¼�, �����źţ� ɾ���߳��ź�
	long _lThreadNum, _lRunningNum; //�߳���, ���е��߳���
	bool m_exit;
};
#endif //_ThreadPool_H_