#include "CStatistics.h"
// #include "CWebSites.h"
// #include "CThreadDownLoad.h"
#include "CButton.h"
#include <cstring>
CStatistics* CStatistics::o;
CStatistics::CStatistics(const string &title,const SMALL_RECT &rect):CDialogForm(title,rect)
{
	o=this;
	killed=false;
	m_rect.Left=m_rect.Top=0;
	m_rect.Right=79;m_rect.Bottom=24;
	m_text="下载统计";
	int x = (rect.Left+rect.Right)/2-16;
	int y=4;
	int len1=18,len2=18;
	nSuccess=CreateItem("下载成功数量：",len1,len2,x,y);
	y+=2;
	nFail=CreateItem("下载失败数量：",len1,len2,x,y);
	y+=2;
	totalUp=CreateItem("发送字节：",len1,len2,x,y);
	y+=2;
	totalDown=CreateItem("下载字节：",len1,len2,x,y);
	y+=2;
	speed=CreateItem("速度（K/S）：",len1,len2,x,y);
	y+=2;
	usedTime=CreateItem("用时：",len1,len2,x,y);
	usedTime->SetTitle("00:00:00");
	y+=4;
	SMALL_RECT rect5 = {x-3,y,x+10-4,y+2};
	CWebSites &cw=CWebSites::GetInstance();
	if (cw.runStat==CWebSites::STAT_RUNNING)
	{
		pYes=new CButton("暂停",rect5,CButton::ID_OK);
	}else if (cw.runStat==CWebSites::STAT_SUSPEND)
	{
		pYes=new CButton("继续",rect5,CButton::ID_OK);
	}else {
		pYes=new CButton("未开始",rect5,CButton::ID_OK);

	}
	CForm* pform=pYes;
	m_controls.push_back(pform);

	SMALL_RECT rect6 = {rect5.Left+25,rect5.Top,rect5.Right+25,rect5.Bottom};
	pform=new CButton("退出",rect6,CButton::ID_CANCEL);
	m_controls.push_back(pform);
	hThread = CreateThread(NULL,0,Thread,NULL,0,NULL);
}
CStatistics::~CStatistics(){
	if (killed==0)
	{
		finished();
	}
}
void CStatistics::finished(){
	killed=true;
	WaitForSingleObject(hThread,INFINITE);
	CloseHandle(hThread);
}

void CStatistics::OnOk()
{
	CWebSites &cw=CWebSites::GetInstance();
	if (cw.runStat==CWebSites::STAT_STOP)
	{
		return ;
	}
	if (pYes->m_text=="继续")
	{
		cw.Resume();
		cw.runStat=CWebSites::STAT_RUNNING;
		pYes->m_text="暂停";
		pYes->Show();
	}else if (pYes->m_text=="暂停")
	{
		cw.Suspend();
		cw.runStat=CWebSites::STAT_SUSPEND;
		pYes->m_text="继续";
		pYes->Show();
	}
}
void CStatistics::OnNo()
{
	m_exit=true;
}
DWORD CALLBACK CStatistics::Thread(PVOID pvoid)
{
	MSG msg;
	PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);
	UINT timerid = SetTimer(NULL,111,1000,TimerProc);
	BOOL bRet;
	while (!o->killed&&(bRet = GetMessage(&msg,NULL,0,0))!=0)
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
void WINAPI CStatistics::TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime){
	char s[20];
	CStatistics & sta=*o;
	sta.cc.Enter();
	CSummary &cs=CSummary::getInstance();
	int x=cs.addNSuccess(0);
	sprintf(s,"%d",x);
	sta.nSuccess->SetTitle(s);
	x=cs.addNFail(0);
	sprintf(s,"%d",x);
	sta.nFail->SetTitle(s);
	x=cs.addTotalUp(0);
	sprintf(s,"%.2f",x/1024.0);
	sta.totalUp->SetTitle(s);
	x=cs.addTotalDown(0);
	sprintf(s,"%.2f",x/1024.0);
	sta.totalDown->SetTitle(s);
	x=cs.getSpeed();
	sprintf(s,"%.2f",x/1024.0);
	sta.speed->SetTitle(s);
	x=cs.getTime();

	int h=x/3600,m=(x/60)%60,sec=x%60;
	string s2="";
	if(h<10) s2+="0";
	sprintf(s,"%d:",h);
	s2+=s;
	if(m<10) s2+="0";
	sprintf(s,"%d:",m);
	s2+=s;
	if(sec<10) s2+="0";
	sprintf(s,"%d",sec);
	s2+=s;
	sta.usedTime->SetTitle(s2);
	sta.nSuccess->Show();
	sta.nFail->Show();
	sta.totalDown->Show();
	sta.totalUp->Show();
	sta.speed->Show();
	sta.usedTime->Show();
	sta.cc.Leave();
}
CStatic* CStatistics::CreateItem(string s,int len1,int len2,int x,int y){
	SMALL_RECT rect1 = {x,y,x+len1,y};
	CForm *pform=new CStatic(s,rect1);
	m_controls.push_back(pform);
	if (len2%2)
	{
		len2++;
	}
	SMALL_RECT rect2 = {rect1.Right+1,rect1.Top,rect1.Right+len2,rect1.Bottom};
	CStatic *ret=new CStatic("0",rect2);
	pform=ret;
	m_controls.push_back(pform);
	return ret;
}