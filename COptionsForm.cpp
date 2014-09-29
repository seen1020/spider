#include "COptionsForm.h"
CCriticalSection COptionsForm::cs;
COptionsForm::COptionsForm(){
	m_rect.Left=m_rect.Top=0;
	m_rect.Right=79;m_rect.Bottom=24;
	m_text="修改配置文件";
	int labelWidth = 18;

	int x = (m_rect.Right - labelWidth - 30 + 1)/2;
	int y = 3;
	createLableEdit("最大线程数：",labelWidth,6,x,y,nThread);
	y+=3;
	createLableEdit("下载超时时间：",labelWidth,6,x,y,timeOut);
	y+=3;
	createLableEdit("网页保存路径：",labelWidth,30,x,y,path);
	y+=3;
	createLableEdit("网址过滤：",labelWidth,30,x,y,serverIP);
	y+=3;
	createLableEdit("重定向（1是 0否）：",labelWidth,6,x,y,port);
	SMALL_RECT rect5 = {x,y+3,x+labelWidth-1,y+5};
	CForm* pform=new CButton("保存",rect5,CButton::ID_OK);
	m_controls.push_back(pform);

	SMALL_RECT rect6 = {rect5.Left+labelWidth+12,rect5.Top,rect5.Right+12+labelWidth,rect5.Bottom};
	pform=new CButton("取消",rect6,CButton::ID_CANCEL);
	m_controls.push_back(pform);
	keys.push_back("NumThreads");
	keys.push_back("Timeout");
	keys.push_back("Path");
	keys.push_back("Port");
	keys.push_back("ServerIP");
	keys.push_back("UserName");
	keys.push_back("Password");
	char vn[50];
	for (int i=0;i<keys.size();i++)
	{
		GetPrivateProfileString(APPNAME,keys[i].c_str(),"error",vn,50,INI_FILENAME);
		m_options.insert(OptionsMap::value_type(keys[i],string(vn)));
	}
}
int COptionsForm::Load(int key){
	nThread->m_text=m_options[keys[0]];
	timeOut->m_text=m_options[keys[1]];
	path->m_text=m_options[keys[2]];
	port->m_text=m_options[keys[3]];
	serverIP->m_text=m_options[keys[4]];
	return CDialogForm::Load(key);
}
void COptionsForm::OnOk(){
	m_options[keys[0]]=nThread->m_text;
	m_options[keys[1]]=timeOut->m_text;
	m_options[keys[2]]=path->m_text;
	m_options[keys[3]]=port->m_text;
	m_options[keys[4]]=serverIP->m_text;
	for (int i=0;i<keys.size();i++)
	{
		WritePrivateProfileString(APPNAME,keys[i].c_str(),m_options[keys[i]].c_str(),INI_FILENAME);
	}
	m_exit=true;
}
const string &COptionsForm::GetValueByKey(const string &key){
	return m_options[key];
}
COptionsForm & COptionsForm::GetInstance(){
	cs.Enter();
	static COptionsForm o;
	cs.Leave();
	return o;
}
int COptionsForm::GetNthread(){
	int ans=CConvert::ToInt(m_options[keys[0]]);
	return ans;
}
int COptionsForm::GetTimeOut(){
	int ans=CConvert::ToInt(m_options[keys[1]]);
	return ans;
}
string COptionsForm::GetPath(){
	string ans= m_options[keys[2]];
	int len=ans.size();
	if (len&&ans.at(len-1)=='/')
	{
		ans=ans.substr(0,len-1);
	}
	return ans;
}
int COptionsForm::GetPort(){
	int ans=CConvert::ToInt(m_options[keys[3]]);
	return ans;
}
string COptionsForm::GetServerIP(){
	string ans= m_options[keys[4]];
	return ans;
}
string COptionsForm::GetUserName(){
	string ans= m_options[keys[5]];
	return ans;
}
string COptionsForm::GetUserPassword(){
	string ans= m_options[keys[6]];
	return ans;
}