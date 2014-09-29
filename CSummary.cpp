#include "CSummary.h"
CSummary CSummary::o;
#include "CWebSites.h"
CSummary::CSummary(){
	me.Enter();
	totalDown=totalUp=total2=speed=t=nSuccess=nFail=0;
	hThread = CreateThread(NULL,0,Thread,NULL,0,NULL);
	me.Leave();
}
CSummary::~CSummary(){
	CloseHandle(hThread);
}
DWORD CALLBACK CSummary::Thread(PVOID pvoid)
{
	MSG msg;
	PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);
	UINT timerid = SetTimer(NULL,111,1000,TimerProc);
	BOOL bRet;
	while ((bRet = GetMessage(&msg,NULL,0,0))!=0)
	{
		if (bRet == -1)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	KillTimer(NULL,timerid);
	return 0;
}
void WINAPI CSummary::TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime){
	CSummary &cs=getInstance();
	cs.me.Enter();
	cs.speed=cs.totalDown-cs.total2;
	cs.total2=cs.totalDown;
	CWebSites &cw=CWebSites::GetInstance();
	if (cw.runStat==CWebSites::STAT_RUNNING)
	{
		cs.t++;
	}
// 	printf("%d\n",cs.speed);
// 	printf("%d\n",cs.t);
	cs.me.Leave();
}
CSummary& CSummary::getInstance(){
	return o;
}
void CSummary::init(){
	me.Enter();
	totalDown=totalUp=total2=speed=t=nSuccess=nFail=0;
	me.Leave();
}
int CSummary::addTotalUp(int x){
	me.Enter();
	totalUp+=x;
	int ans=totalUp;
	me.Leave();
	return ans;
}
int CSummary::addTotalDown(int x){
	me.Enter();
	totalDown+=x;
	int ans=totalDown;
	me.Leave();
	return ans;
}
int CSummary::addNSuccess(int x){
	me.Enter();
	nSuccess+=x;
	int ans=nSuccess;
	me.Leave();
	return ans;
}
int CSummary::addNFail(int x){
	me.Enter();
	nFail+=x;
	int ans=nFail;
	me.Leave();
	return ans;
}
int CSummary::getTime(){
	me.Enter();
	int ans=t;
	me.Leave();
	return ans;
}
int CSummary::getSpeed(){
	me.Enter();
	int ans=speed;
	me.Leave();
	return ans;
}