#ifndef CSTATISTICS_H
#define CSTATISTICS_H
#include "CDialogForm.h"
#include <string>
#include "CStatic.h"
#include "CCriticalSection.h"
#include "CSummary.h"
class CStatistics:public CDialogForm
{
public:
	CStatistics(const string &title,const SMALL_RECT &rect);
	~CStatistics();
	void finished();
	virtual void OnOk();
	virtual void OnNo();
	CStatic* CreateItem(string s,int len1,int len2,int x,int y);
protected:
	static CStatistics* o;
	CCriticalSection cc;
	CButton *pYes,*pEsc;
	HANDLE hThread;
	bool killed;
	CStatic *nSuccess,*nFail,*totalUp,*totalDown,*usedTime,*speed;
	static DWORD CALLBACK Thread(PVOID pvoid);
	static void WINAPI TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime);
private:
};
#endif