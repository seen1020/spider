#ifndef CWEBSITES_H
#define CWEBSITES_H
#pragma  warning(disable:4786)
#include <set>
#include <map>
#include <string>
#include <vector>
#include "CThreadPool.h"
#include "CCriticalSection.h"
#include "CURLS.h"
#include "CWebpage.h"
#include "CThreadDownLoad.h"
using namespace std;
class CWebSites:public CThread
{
public:
	//Ê¹ÓÃµ¥Àý
	enum{
		TYPE_HTML,
		TYPE_CSS,
		TYPE_JS
	};
	enum{
		STAT_STOP,
		STAT_RUNNING,
		STAT_SUSPEND
	};
	static CWebSites& GetInstance();
	static CURLS &GetLocalURL(string s);
	void InsertUrls(const set<CURLS> &urls);
	void InsertUrl(const CURLS &url);
	string GetUrl();
	unsigned int GetUrlsSize();
	string GetWebpageName();
	void Clear();
	void Run(void *pPara=NULL);
	void DelFile (string path);
	void Start(string url,int depth);
	void Suspend();
	void Resume();
	void Stop();
	string GetFName(const CURLS &curl);
	map<CURLS,string> m_allUrls;
	CCriticalSection m_csRunning,m_csStat,m_csOfUrl,m_csActive,m_csOfCount,m_csOfSites;
	int m_count;
	int runStat;
	vector<CURLS> m_waitUrls,m_waitUrls2;
protected:
	void SuspendStat(CThreadPool &cp);
	vector<CWebpage *> m_webPages[2];
	void PageClear(int p);
	CWebpage * NewPage(int p,string s);
	CWebpage * NewPage(int p,CURLS s);
	int m_depth,m_fildId;
	bool m_exit,m_active,m_runable,m_finished;
	HANDLE m_SemaphoreStart,m_SemaphoreExit,m_SemaphoreRusume;
	CThreadPool m_pool;
	CThreadPool *p_pool;
	bool isRunning;
	CWebSites();
	~CWebSites();
private:

};
#endif