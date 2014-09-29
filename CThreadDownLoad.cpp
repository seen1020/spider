#include "CThreadDownLoad.h"
#include "COptionsForm.h"
#include "CWebSites.h"
#include "CSummary.h"
CThreadDownLoad::CThreadDownLoad(){
	curl_global_init(CURL_GLOBAL_DEFAULT);
}
void CThreadDownLoad::Run(void *param)
{
	char errorBuffer[CURL_ERROR_SIZE];
	CURL *conn=NULL;
	CURLcode code;
	if (!init(conn,errorBuffer))
	{
		fprintf(stderr, "Connection initializion failed\n");
	}
	CWebSites &cw=CWebSites::GetInstance();
	int *idx=(int *)param;
	CWebpage page(cw.m_waitUrls2[*idx]);
	delete idx;
// 	printf("%s\n",page->getURL().GetURL().c_str());
	code = curl_easy_setopt(conn, CURLOPT_URL, page.getURL().GetURL().c_str());
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
		return ;
	}
	page.m_content="";
	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &(page.m_content));
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
		return ;
	}
	CSummary &summary=CSummary::getInstance();
	summary.addTotalUp(page.getURL().GetURL().size());
	code =curl_easy_perform(conn);
	if (code != CURLE_OK)
	{
// 		fprintf(stderr, "Failed to get '%s' [%s]\n", page->getURL().GetURL().c_str(), errorBuffer);
		summary.addNFail(1);
		return ;
	}
	page.finished=true;
	curl_easy_cleanup(conn);
	summary.addNSuccess(1);
	set<CURLS> &ap=page.GetAnthorsFromPage();
	cw.InsertUrls(ap);
	cw.m_csOfCount.Enter();
	COptionsForm &co=COptionsForm::GetInstance();
	string folderName=co.GetPath();
	cw.m_csOfUrl.Enter();
	if (co.GetPort())
	{
		page.UpdateURL();
	}
	page.Save(folderName,cw.m_count);
	cw.m_csOfUrl.Leave();
	cw.m_count++;
	cw.m_csOfCount.Leave();
}
bool CThreadDownLoad::init(CURL *&conn,char errorBuffer[])
{
	conn = curl_easy_init();
	CURLcode code;
	if (conn == NULL)
	{
		fprintf(stderr, "Failed to create CURL connection\n");
		exit(EXIT_FAILURE);
	}
	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set error buffer [%d]\n", code);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 10);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, CThreadDownLoad::writer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		return false;
	}
	int t=COptionsForm::GetInstance().GetTimeOut();
	code = curl_easy_setopt(conn, CURLOPT_TIMEOUT_MS, t);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		return false;
	}
	return true;
}
int CThreadDownLoad::writer(char *data, size_t size, size_t nmemb, string *writerData)
{
	unsigned long sizes = size * nmemb;
	CSummary &summary=CSummary::getInstance();
	summary.addTotalDown(sizes);
	if (writerData == NULL) return 0;
	writerData->append(data, sizes);
	return sizes;
}
CThreadDownLoad & CThreadDownLoad::GetInstance(){
	static CThreadDownLoad o;
	return o;
}
using namespace std;
char *s[]={"http://blog.csdn.net/dadalan/article/details/3031478",
"http://www.dedecms8.com/templets/ad/wshl.png",
"www.baidu.com",
"http://curl.haxx.se/libcurl/c/curl_easy_cleanup.html",
"http://blog.csdn.net/dadalan/article/details/3031478",
"http://open.sina.com.cn/school/id_57/",
"http://www.myexception.cn/program/599647.html",
"http://www.myexception.cn/other/1391133.html",
"http://www.4399.com/",
"http://wenwen.soso.com/z/q440308948.htm"};
#define N 2
int main1(){ 
	CThreadDownLoad & cd = CThreadDownLoad::GetInstance();
	CThreadPool cp(3);
	CWebpage* webs[N];
	for (int i=0;i<N;i++)
	{
		webs[i]=new CWebpage(s[i]);
	}
	int a=clock();
	for (int i=0;i<N;i++)
	{
		cp.Call(&cd,webs[i]);
	}
	cp.EndAndWait();
// 	printf("%d\n",clock()-a);
// 	cd.Run((void*)webs[0]);
	char fname[20];
	for (int i=0;i<N;i++)
	{
		sprintf(fname,"e:/nima/%d.html",i);
		set<CURLS> &ap=webs[i]->GetAnthorsFromPage();
// 		for (set<CURLS>::iterator j=ap.begin();j!=ap.end();j++)
// 		{
// 			printf("\t%s\n",j->GetURL().c_str());
// 		}
// 		printf("\n\n\n");
		webs[i]->Save("e:/nima",i);
	}
	for (int i=0;i<N;i++)
	{
		delete webs[i];
	}
	return 0;
}