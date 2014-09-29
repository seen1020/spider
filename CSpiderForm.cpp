#include "CSpiderForm.h"
// #include "CWebSites.h"
// #include "CThreadDownLoad.h"
#include "CButton.h"
#include "CWebSites.h"
#include "CConvert.h"
#include <cstring>
CSpiderForm & CSpiderForm::GetInstance(){
	SMALL_RECT rect={0,0,79,24};
	static CSpiderForm o("����",rect);
	return o;
}
CSpiderForm::CSpiderForm(const string &title,const SMALL_RECT &rect):CDialogForm(title,rect)
{
	isShow=false;
	int widthOfLabel = 12;
	int left = ((m_rect.Right - m_rect.Left + 1) - widthOfLabel - 44) / 2 + m_rect.Left;
	int top = m_rect.Top + 5;
	SMALL_RECT rect1 = {left,top,left + widthOfLabel - 1,top + 2};
	CStatic *pStatic = new CStatic("��ʼ���ӣ�",rect1);
	m_controls.push_back(pStatic);
	SMALL_RECT rect2 = {rect1.Right + 1,rect1.Top,rect1.Right + 1 + 44 - 1,rect1.Bottom};
	m_pTxtURL = new CEdit("www.baidu.com",rect2,40);
	m_controls.push_back(m_pTxtURL);
	SMALL_RECT rect3 = {left,rect1.Bottom + 2,rect1.Right,rect1.Bottom + 2 + 2};
	pStatic = new CStatic("ץȡ��ȣ�",rect3);
	m_controls.push_back(pStatic);
	SMALL_RECT rect4 = {rect2.Left,rect2.Bottom + 2,rect2.Left + 8 - 1,rect2.Bottom + 2 + 2};
	m_pTxtDepth = new CEdit("3",rect4,2,0,CForm::Right);
	m_controls.push_back(m_pTxtDepth);

	SMALL_RECT rect5 = {20,rect3.Bottom + 2,31,rect3.Bottom + 2 + 2};
	CButton *pButton = new CButton("��ʼ",rect5,CButton::ID_OK);
	m_controls.push_back(pButton);
	pYes=pButton;
	SMALL_RECT rect6 = {35,rect5.Top,46,rect5.Bottom};
	pButton = new CButton("δ��ʼ",rect6,CButton::ID_NO);
	m_controls.push_back(pButton);
	pNo=pButton;
	SMALL_RECT rect7 = {50,rect5.Top,61,rect5.Bottom};
	pButton = new CButton("�뿪",rect7,CButton::ID_CANCEL);
	m_controls.push_back(pButton);
	pEsc=pButton;
}

void CSpiderForm::OnOk()
{
	CWebSites &cw=CWebSites::GetInstance();
	if (pYes->m_text=="��ʼ")
	{
		int depth=CConvert::ToInt(m_pTxtDepth->GetTitle()) ;
		if (depth==-1)
		{
			depth=3;
		}
		cw.Start(m_pTxtURL->GetTitle(),depth);
		cw.runStat=CWebSites::STAT_RUNNING;
		ChangeBtn(CWebSites::STAT_STOP,CWebSites::STAT_RUNNING);
	}else if (pYes->m_text=="ֹͣ")
	{
		pYes->m_text="�ȴ�����";
		pYes->Show();
		cw.Stop();
	}
}

void CSpiderForm::OnCancel()
{
	m_exit = true;
}
void CSpiderForm::OnNo()
{
	CWebSites &cw=CWebSites::GetInstance();
	if (pYes->m_text=="�ȴ�����")
	{
		return;
	}
	if (pNo->m_text=="����")
	{
		cw.Resume();
		cw.runStat=CWebSites::STAT_RUNNING;
		pNo->m_text="��ͣ";
		pNo->Show();
	}else if (pNo->m_text=="��ͣ")
	{
		cw.Suspend();
		cw.runStat=CWebSites::STAT_SUSPEND;
		pNo->m_text="����";
		pNo->Show();
	}
}
void CSpiderForm::ChangeBtn(int pre,int now){
	switch (pre)
	{
	case CWebSites::STAT_STOP:
		if (now==CWebSites::STAT_RUNNING)
		{
			pYes->m_text="ֹͣ";
			pNo->m_text="��ͣ";
			if (isShow)
			{
				pYes->Show();
				pNo->Show();
			}
		}
		break;
	case CWebSites::STAT_RUNNING:
		switch (now)
		{
		case CWebSites::STAT_STOP:
			pYes->m_text="��ʼ";
			pNo->m_text="δ��ʼ";
			if (isShow)
			{
				pYes->Show();
				pNo->Show();
			}
			break;
		case CWebSites::STAT_SUSPEND:
			pNo->m_text="����";
			if (isShow)
			{
				pNo->Show();
			}
			break;
		}
		break;
	case CWebSites::STAT_SUSPEND:
		switch (now)
		{
		case CWebSites::STAT_STOP:
			pYes->m_text="��ʼ";
			pNo->m_text="δ��ʼ";
			if (isShow)
			{
				pYes->Show();
				pNo->Show();
			}
			break;
		case CWebSites::STAT_RUNNING:
			pNo->m_text="��ͣ";
			if (isShow)
			{
				pNo->Show();
			}
			break;
		}
		break;
	}
}