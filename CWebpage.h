#ifndef CWEBPAGE_H
#define CWEBPAGE_H
#pragma warning(disable:4786)
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include "CURLS.h"

using namespace std;
struct Lable 
{
	string url;
	int s,t;
	int type;
	void init();
	bool operator<(const Lable&b)const;
};
class CWebpage
{
public:
	CWebpage(const CURLS &url,const string &content);
	CWebpage(const string &url);
	CWebpage(const char *url);
	CWebpage(const CURLS &url);
	const CURLS& getURL();
	//set<string> &GetAnthorsFromPage(const string &content);
	void MakeAnthors();
	set<CURLS> &GetAnthorsFromPage();
	set<CURLS> &GetAnthorsFromPage(const string &content);
	bool Save(const string &folderName,int count);
	void UpdateURL();
	string m_content;	//网页内容
	bool finished;
	string GetPageName();
protected:
	CURLS m_URL;	//网页地址
	vector<Lable> m_labels;
	set<CURLS> m_anchors;
	//可要可不要
	void GetLabels(const string &element,const string &attribute);
	void GetLabels(const string &element,const string &attribute,const string &content);
private:
	char filename[1000];
};

#endif
