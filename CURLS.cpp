#include "CURLS.h"
CURLS::CURLS(const string & url){
	m_URL=url;
	pageType=0;
	SplitURL();
}
CURLS::CURLS(const char * url){
	m_URL=url;
	pageType=0;
	SplitURL();
}
void CURLS::SplitURL(){
	int p=0,res;
	res=m_URL.find("://",p);
	if (res!=string::npos)
	{
		res+=3;
	}else{
		res=0;
	}
	p=res;
	res=m_URL.find("/",p);
	string ht;
	if (res!=string::npos)
	{
		ht=m_URL.substr(p,res-p);
		m_path=m_URL.substr(res+1);
	}else{
		ht=m_URL.substr(p);
		m_path="";
	}
	p=0;
	res=ht.find(":",p);
	if (res!=string::npos)
	{
		m_host=ht.substr(p,res);
		m_port= atoi(ht.substr(res+1).c_str());
	}else {
		m_host=ht;
		m_port=80;
	}
	char *s=new char[m_host.size()+m_path.size()+20];
	sprintf(s,"http://%s:%d/%s",m_host.c_str(),m_port,m_path.c_str());
	m_URL=s;
	delete[]s;
}
int CURLS::GetPort()const{
	return m_port;
}
const string & CURLS::GetHost()const{
	return m_host;
}
const string & CURLS::GetURL()const{
	return m_URL;
}
const string & CURLS::GetPath()const{
	return m_path;
}
bool CURLS::operator<(const CURLS &url)const{
	// 		if (m_port<url.m_port)
	// 		{
	// 			return true;
	// 		}
	// 		if (m_port==url.m_port)
	// 		{
	// 			if (m_host<url.m_host)
	// 			{
	// 				return true;
	// 			}else if (m_host==url.m_host)
	// 			{
	// 				return m_path<url.m_path;
	// 			}
	// 		}
	return m_URL<url.m_URL;
}