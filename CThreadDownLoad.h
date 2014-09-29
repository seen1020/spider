#ifndef CTHREADDOWNLOAD
#define CTHREADDOWNLOAD
#include "CThread.h"
#include "CWebpage.h"
#include "CThreadPool.h"
#include "curl/curl.h"
#include <iostream>
#pragma comment(lib, "libcurl_imp.lib")  
using namespace std;

class CThreadDownLoad:public CThread
{
public:
	void Run(void *param);
	bool init(CURL *&conn,char errorBuffer[]);
	static int writer(char *data, size_t size, size_t nmemb, string *writerData);
	static CThreadDownLoad & GetInstance();
protected:
public:
	CThreadDownLoad();
};
#endif