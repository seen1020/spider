#include "CWebpage.h"
#include "CWebSites.h"
void Lable::init(){
	url="";
	s=t=type=0;
}
bool Lable::operator<(const Lable&b)const{
	return s<b.s;
}
CWebpage::CWebpage(const CURLS &url,const string &content):m_URL(url),finished(false){
	m_content=content;
}
CWebpage::CWebpage(const string &url):m_URL(url),finished(false){
}
CWebpage::CWebpage(const char *url):m_URL(url),finished(false){
}
CWebpage::CWebpage(const CURLS &url):m_URL(url),finished(false){
}
void CWebpage::MakeAnthors(){
	string s;
	for (int i=0;i<m_labels.size();i++)
	{
		s=m_labels[i].url;
		if (string::npos==s.find("://",0))
		{
			if (s.size()!=0&&s[0]=='/')
			{
				s=m_URL.GetHost()+s;
			}else{
				const string& tmps=m_URL.GetURL();
				int p=tmps.find_last_of('/',string::npos);
				string s2=tmps.substr(0,p+1);
				s=s2+s;
			}
		}
		CURLS url(s);
		url.pageType=m_labels[i].type;
		m_anchors.insert(url);
	}
}
string CWebpage::GetPageName(){
	string url=m_URL.GetURL();
	bool x=false;
	int i,j;
	for (i=url.size()-2,j=0;j<4&&i>=0&&url.at(i)!='/';i--,j++)
	{
		if (url.at(i)=='.')
		{
			x=true;break;
		}
	}
	if (!x)
	{
		return "";
	}
	i=url.find_last_of('/',string::npos);
	return url.substr(i+1);
}
const CURLS& CWebpage::getURL(){
	return m_URL;
}
void CWebpage::GetLabels(const string &element,const string &attribute){
	GetLabels(element,attribute,m_content);
}
void CWebpage::GetLabels(const string &element,const string &attribute,const string &content){
	string lb="<"+element+" ";
	int p=0;
	vector<Lable>& resn=m_labels;
	string res1;
	Lable res2;
	int times=0;
	while ((p=content.find(lb,p))!=string::npos)
	{
		p+=lb.size();
		int p2=content.find(">",p);
		int tmp;
		if (p2==string::npos)
		{
			return ; 
		}else tmp=p2+1;
		res1=content.substr(p,p2-p);
		// 		cout<<(times++)<<endl<<res1<<endl;
		p2=res1.find(attribute,0);
		if (p2==string::npos)
		{
			continue;
		}
		p2+=attribute.size();
		char c=res1.at(p2);
		if ((c!=' ') && (c!='\t') && (c!='\n')&&(c!='='))
		{
			continue;
		}
		int p3=res1.find_first_of("\"'",p2);
		const char *cp=res1.c_str();
		if (p3==string::npos)
		{
			bool x1=true,x2=true;
			res2.init();
			for (int i=p2;cp[i]&&(((cp[i]!=' ') && (cp[i]!='\t') && (cp[i]!='\n'))||x1||x2);i++ )
			{
				if (x1&&cp[i]=='=')
				{
					x1=false;
					continue;
				}
				if(x2&&x1==false&&((cp[i]!=' ') && (cp[i]!='\t') && (cp[i]!='\n'))){
					x2=false;
					res2.s=i+p;
				}
				if (!x2)
				{
					res2.url+=cp[i];
				}
			}
			res2.t=res2.s+res2.url.size();
		}else{
			if (res1.c_str()[p3]=='\"')
			{
				int p4=res1.find("\"",p3+1);
				res2.url=res1.substr(p3+1,p4-p3-1);
				res2.s=p3+1+p;res2.t=p4+p;
			}else{
				int p4=res1.find("'",p3+1);
				res2.url=res1.substr(p3+1,p4-p3-1);
				res2.s=p3+1+p;res2.t=p4+p;
			}
		}
		resn.push_back(res2);
		p=tmp;
	}
}
set<CURLS> &CWebpage::GetAnthorsFromPage(const string &content){
	m_anchors.clear();
	m_labels.clear();
	if (m_URL.pageType!=CURLS::TYPE_HTML)
	{
		return m_anchors;
	}
	int an[]={CURLS::TYPE_HTML,CURLS::TYPE_CSS,CURLS::TYPE_JS,CURLS::TYPR_IMG};
	for (int i=0;i<4;i++)
	{
		int s=m_labels.size();
		switch(an[i]){
		case CURLS::TYPE_HTML:
			GetLabels("a","href",content);break;
		case CURLS::TYPE_CSS:
			GetLabels("link","href",content);break;
		case CURLS::TYPE_JS:
			GetLabels("script","src",content);break;
		case CURLS::TYPR_IMG:
			GetLabels("img","src",content);break;
		}
		int t=m_labels.size();
		for (int j=s;j<t;j++)
		{
			m_labels[j].type=an[i];
		}
	}
	MakeAnthors();
	return m_anchors;
}
set<CURLS> &CWebpage::GetAnthorsFromPage(){
	return GetAnthorsFromPage(m_content);
}
bool CWebpage::Save(const string &folderName,int count){

	string pageName=CWebSites::GetInstance().m_allUrls[m_URL];
	string fold="";
	switch (m_URL.pageType)
	{
	case CURLS::TYPE_HTML:fold="webpage";break;
	case CURLS::TYPE_CSS:fold="css";break;
	case CURLS::TYPE_JS:fold="js";break;
	case CURLS::TYPR_IMG:fold="img";break;
	}
	if (count)
	{
		sprintf(filename,"%s/%s/%s",folderName.c_str(),fold.c_str(),pageName.c_str());
	}else
		sprintf(filename,"%s/%s",folderName.c_str(),pageName.c_str());
// 	printf("filenams:%s\n",filename);
	FILE* pagefile = fopen(filename, "wb");
	if (pagefile) {
		fwrite(m_content.c_str(), 1, m_content.size(), pagefile);
		fclose(pagefile);
	}
	return true;
}
void CWebpage::UpdateURL(){
	CWebSites& cw=CWebSites::GetInstance();
	sort(m_labels.begin(),m_labels.end());
	string tmp=m_content;
	const char *s=tmp.c_str();
	m_content.clear();
	int p=0;
	for (int i=0;i<m_labels.size();i++)
	{
		Lable &lb=m_labels[i];
		while (p<lb.s)
		{
			m_content.push_back(s[p++]);
		}
		CURLS curl(lb.url);
		curl.pageType=lb.type;
		string des=cw.GetFName(curl);
		string fold="";
		switch (lb.type)
		{
		case CURLS::TYPE_HTML:fold="webpage";break;
		case CURLS::TYPE_CSS:fold="css";break;
		case CURLS::TYPE_JS:fold="js";break;
		case CURLS::TYPR_IMG:fold="img";break;
		}
		if (cw.m_count)
		{
			des="../"+fold+"/"+des;
		}else des=fold+"/"+des;
		for (int j=0;j<des.size();j++)
		{
			m_content.push_back(des.at(j));
		}
		p=lb.t;
	}
	while (p<tmp.size())
	{
		m_content.push_back(s[p++]);
	}
}