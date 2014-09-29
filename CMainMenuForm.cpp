//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CLoginForm.cpp
//  @ Date : 2013/9/12
//  @ Author : 
//
//



#include "CMainMenuForm.h"
#include "COptionsForm.h"
#include "CButton.h"
#include "CSpiderForm.h"
#include "CStatistics.h"
CMainMenuForm::CMainMenuForm(const string& title,const SMALL_RECT &rect):CDialogForm(title,rect)
{
	char *ss[]={"网络爬虫","下载统计","网页快照","修改配置信息","退出"};
	int n=sizeof(ss)/4;
	int mlen=0;
	for (int i=0;i<n;i++)
	{
		m_menu.push_back(ss[i]);
		mlen=max(mlen,m_menu[i].size());
	}
	int x = (m_rect.Right - (mlen+8) + 1)/2;
	int y = 3;
	CButton * pBnt;
	SMALL_RECT rect2;
	rect2.Left=x;
	rect2.Right=(x+(mlen+8))|1;
	rect2.Top=y;
	rect2.Bottom=y+2;
	for (int i=0;i<n-1;i++)
	{
		rect2.Top+=3;
		rect2.Bottom+=3;
		pBnt=new CButton(m_menu[i],rect2,CButton::ID_OK);
		m_controls.push_back(pBnt);
	}
	rect2.Top+=3;
	rect2.Bottom+=3;
	pBnt=new CButton(m_menu[n-1],rect2,CButton::ID_CANCEL);
	m_controls.push_back(pBnt);
}

void CMainMenuForm::OnOk()
{
	
	if (m_controls[m_index]->m_text==m_menu[0])
	{
		SaveWindow();
		SMALL_RECT rect={0,0,79,24};
		CSpiderForm &sf=CSpiderForm::GetInstance();
		sf.isShow=true;
		sf.Load(0);
		sf.isShow=false;
		RetWindow();
	}else if (m_controls[m_index]->m_text==m_menu[1])
	{
		SaveWindow();
		SMALL_RECT rect={0,0,79,24};
		CStatistics  statistic("下载实时数据统计",rect);
		statistic.Load(0);
		statistic.finished();
		RetWindow();
	}else if (m_controls[m_index]->m_text==m_menu[2])
	{
	}else if (m_controls[m_index]->m_text==m_menu[3])
	{
		SaveWindow();
		COptionsForm & option=COptionsForm::GetInstance();
		option.Load(0);
		RetWindow();
	}else if (m_controls[m_index]->m_text==m_menu[4])
	{
	}
}
void CMainMenuForm::OnCancel()
{
	CWebSites &cw=CWebSites::GetInstance();
	cw.Stop();
	while (cw.runStat!=CWebSites::STAT_STOP)
	{
		Sleep(20);
	}
	m_exit=true;
}
