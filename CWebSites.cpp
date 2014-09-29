#include "CWebSites.h"
#include <SSTREAM>
#include "CThreadPool.h"
#include "CSummary.h"
#include "COptionsForm.h"
#include "CMatch.h"
#include "CSpiderForm.h"
using namespace std;
CWebSites& CWebSites::GetInstance()
{
	static CWebSites webSites;
	return webSites;
}
CWebSites::CWebSites()
{
	m_exit=false;
	m_active=true;
	m_runable=false;
	isRunning=false;
	m_SemaphoreStart = CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
	m_SemaphoreRusume =  CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
	m_pool.AdjustSize(1);
	m_pool.Call(this);
}
CWebSites::~CWebSites(){
	m_exit=true;
	ReleaseSemaphore(m_SemaphoreStart, 1, NULL);
	ReleaseSemaphore(m_SemaphoreRusume, 1, NULL);
	m_pool.EndAndWait();
	PageClear(0);
	PageClear(1);
}
void CWebSites::InsertUrl(const CURLS &url)
{
	m_csOfUrl.Enter();
	if (m_allUrls.find(url)==m_allUrls.end())
	{
		GetFName(url);
		m_waitUrls.push_back(url);
	}
	m_csOfUrl.Leave();
}
void CWebSites::InsertUrls(const set<CURLS> &urls)
{
	set<CURLS>::const_iterator it = urls.begin();
	COptionsForm &co=COptionsForm::GetInstance();
	m_csOfUrl.Enter();
	for (;it!=urls.end();it++)
	{
		const CURLS &cu=*it;
		if (m_allUrls.find(*it)==m_allUrls.end())//判断插入是否成功
		{
			if (!CMatch::judge(cu.GetHost(),co.GetServerIP()))
			{
				continue;
			}
			GetFName(*it);
			m_waitUrls.push_back(*it);
		}
	}
	m_csOfUrl.Leave();
}
unsigned int CWebSites::GetUrlsSize()
{
	return m_waitUrls.size();
}
string CWebSites::GetWebpageName()
{
	m_csOfCount.Enter();
	m_count++;
	m_csOfCount.Leave();
	ostringstream ostr;
	ostr<<m_count<<".html";
	return ostr.str();
}
void CWebSites::Clear()
{
	m_waitUrls.clear();
	//m_waitUrls.clear();
	m_allUrls.clear();
}
void CWebSites::SuspendStat(CThreadPool &cp){
	if (!m_active)
	{
		WaitForSingleObject(m_SemaphoreRusume,INFINITE);
		m_active=true;
	}
}
void CWebSites::DelFile (string path)//path为绝对路径
{
	for (int i=0;i<path.size();i++)
	{
		if (path.at(i)=='/')
		{
			path.at(i)='\\';
		}
	}
	string cmd="del "+path+" /f /s /q > dellog.log 2>delerr.log";
	system(cmd.c_str());
}
void CWebSites::Run(void *pPara){
	CThreadDownLoad & cd = CThreadDownLoad::GetInstance();
	while (!m_exit)
	{
		if (!m_runable)
		{
			WaitForSingleObject(m_SemaphoreStart,INFINITE);
			m_runable=true;
		}
		if (m_exit)
		{
			break;
		}
		COptionsForm &co=COptionsForm::GetInstance();
		CThreadPool cp(co.GetNthread());
		p_pool=&cp;
		m_csRunning.Enter();
		isRunning=true;
		m_csRunning.Leave();
		m_finished=false;
		int i;

		for (i=0;!m_exit&&!m_finished&&i<m_depth&&m_waitUrls.size()>0;i++)
		{
// 			PageClear(i);
			int n=m_waitUrls.size();
// 			if (n>100)
// 			{
// 				n=100;
// 			}
			m_waitUrls2.clear();
			for (int j=0;!m_exit&&j<n;j++)
			{
// 				NewPage(i,m_waitUrls[j]);
				m_waitUrls2.push_back(m_waitUrls[j]);
				SuspendStat(cp);
			}
			m_waitUrls.clear();
			for (int j=0;!m_exit&&j<n;j++)
			{
// 				pcw=m_webPages[i%2][j];

// 				pcw=new CWebpage(m_waitUrls[j]);
				int *idx=new int;
				*idx=j;
				cp.Call(&cd,(void *)(idx));
				SuspendStat(cp);
			}
			if (!m_exit)
			{
				cp.Wait();
			}
// 			for (int j=0;j<n;j++)
// 			{
// 				if (m_webPages[i%2][j]->finished==false)
// 				{
// 					continue;
// 				}
// 				set<CURLS> &ap=m_webPages[i%2][j]->GetAnthorsFromPage();
// 				InsertUrls(ap);
// 				SuspendStat(cp);
// 			}
			SuspendStat(cp);
// 			if (i!=0)
// 			{
// 				n=m_webPages[(i-1)%2].size();
// 				for (int j=0;j<n;j++)
// 				{
// 					CWebpage *pcw=m_webPages[1-i%2][j];
// 					if (pcw->finished==false)
// 					{
// 						continue;
// 					}
// 					m_csOfCount.Enter();
// 					char folderName[]="e:/nima/";
// 					int folderNamelen=strlen(folderName);
// 					if (folderNamelen&&folderName[folderNamelen-1]=='/')
// 					{
// 						folderName[folderNamelen-1]=0;
// 					}
// 					pcw->Save(folderName,m_count);
// 					m_count++;
// 					m_csOfCount.Leave();
// 					SuspendStat(cp);
// 				}
// 			}
		}
// 		if (i!=0)
// 		{
// 			int n=m_webPages[(i-1)%2].size();
// 			for (int j=0;j<n;j++)
// 			{
// 				CWebpage *pcw=m_webPages[1-i%2][j];
// 				if (pcw->finished==false)
// 				{
// 					continue;
// 				}
// 				char folderName[]="e:/nima/";
// 				int folderNamelen=strlen(folderName);
// 				if (folderNamelen&&folderName[folderNamelen-1]=='/')
// 				{
// 					folderName[folderNamelen-1]=0;
// 				}
// 				m_csOfCount.Enter();
// 				pcw->Save(folderName,m_count);
// 				m_count++;
// 				m_csOfCount.Leave();
// 				SuspendStat(cp);
// 			}
// 		}
		m_csRunning.Enter();
		isRunning=false;
		m_csRunning.Leave();
		m_runable=false;
		int tStat=runStat;
		runStat=STAT_STOP;
		CSpiderForm & spider=CSpiderForm::GetInstance();
		spider.ChangeBtn(tStat,runStat);
		CSummary &summary=CSummary::getInstance();
		summary.init();
// 		printf("任务执行完毕");
	}
}
void CWebSites::Start(string url,int depth){
	m_csOfCount.Enter();
	m_depth=depth;
	m_count=0;
	m_allUrls.clear();
	m_fildId=0;
	m_waitUrls.clear();
	PageClear(0);
	PageClear(1);
	CURLS cu(url);
	InsertUrl(cu);
	m_csOfCount.Leave();
	CSummary &summary=CSummary::getInstance();
	COptionsForm &co=COptionsForm::GetInstance();
	summary.init();
	DelFile(co.GetPath());
	ReleaseSemaphore(m_SemaphoreStart, 1, NULL);
}
void CWebSites::Suspend(){
	m_csActive.Enter();
	m_active=false;
	m_csActive.Leave();
	m_csRunning.Enter();
	if (isRunning)
	{
		p_pool->Suspend();
	}
	m_csRunning.Leave();
}
void CWebSites::Resume(){
	ReleaseSemaphore(m_SemaphoreRusume, 1, NULL);
	m_csRunning.Enter();
	if (isRunning)
	{
		p_pool->Rusume();
	}
	m_csRunning.Leave();
}
void CWebSites::Stop(){
	m_csRunning.Enter();
	m_finished=true;
	Resume();
	if (isRunning)
	{
		p_pool->Stop();
	}
	m_csRunning.Leave();
}
void CWebSites::PageClear(int p){
	p%=2;
	for (int i=0;i<m_webPages[p].size();i++)
	{
		delete m_webPages[p][i];
	}
	m_webPages[p].clear();
}
CWebpage * CWebSites::NewPage(int p,string s){
	p%=2;
	CWebpage *pp=new CWebpage(s);
	m_webPages[p].push_back(pp);
	return pp;
}
CWebpage * CWebSites::NewPage(int p,CURLS s){
	p%=2;
	CWebpage *pp=new CWebpage(s);
	m_webPages[p].push_back(pp);
	return pp;
}
string CWebSites::GetFName(const CURLS &curl){
	if (m_allUrls.find(curl)!=m_allUrls.end())
	{
		return m_allUrls[curl];
	}
	string url=curl.GetURL();
// 	bool x=false;
// 	int i,j;
// 	for (i=url.size()-2,j=0;j<4&&i>=0&&url.at(i)!='/';i--,j++)
// 	{
// 		if (url.at(i)=='.')
// 		{
// 			x=true;break;
// 		}
// 	}
	string pageName="";
// 	if (!x)
// 	{
// 		pageName= "";
// 	}else{
// 		i=url.find_last_of('/',string::npos);
// 		pageName= url.substr(i+1);
// 	}
	string suf="";
	switch (curl.pageType)
	{
	case CURLS::TYPE_HTML:suf="html";break;
	case CURLS::TYPE_CSS:suf="css";break;
	case CURLS::TYPE_JS:suf="js";break;
	case CURLS::TYPR_IMG:suf="jpg";break;
	}
	if (pageName=="")
	{
		if (m_fildId)
		{
			char fk[50];
			sprintf(fk,"%d.%s",m_fildId,suf.c_str());
			pageName=fk;
		}else{
			pageName="main."+suf;
		}
	}
	m_fildId++;
	m_allUrls[curl]=pageName;
	return pageName;
}
int main2(){
	CWebSites &cw=CWebSites::GetInstance();
	char c;
	while (cin>>c&&c!='e')
	{
		switch(c){
			case 's':cw.Start("www.baidu.com",3);break;
			case 't':cw.Suspend();break;
			case 'r':cw.Resume();break;
			case 'k':cw.Stop();break;
		}
	}
	return 0;
}