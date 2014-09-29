#include "CMessageBox.h"
#include "CForm.h"
CMessageBox::CMessageForm::CMessageForm(const string &title, const SMALL_RECT &rect, CMessageBox::MessageBoxButtons type, const string &message):CDialogForm(title,rect)
{
	int bottom = m_rect.Bottom - 2;
	int top = bottom - 2;
	int width = 10;
	int left;
	//38字符一行
	SMALL_RECT rectLabel = {rect.Left + 2, rect.Top + 2, rect.Right - 2,rect.Top + 2};
	CForm *pForm = new CStatic(message,rectLabel);
	m_controls.push_back(pForm);
	switch (type)
	{
	case OK:
		{
			left = ((m_rect.Right - m_rect.Left + 1) - width) / 2 + m_rect.Left;
			SMALL_RECT rect={left,top,left + width - 1,bottom};
			m_pBtnOK = new CButton("确定",rect,CButton::ID_OK);
			m_controls.push_back(m_pBtnOK);
			break;
		}
	case YESNO:
		{
			left = ((m_rect.Right - m_rect.Left + 1) - (width * 2 + 2)) / 2 + m_rect.Left;
			left = left % 2 != 0?left + 1:left; 
			SMALL_RECT rect1={left,top,left + width - 1,bottom};
			SMALL_RECT rect2={rect1.Right + 1 + 2,top,rect1.Right + 1 + 2 + width -1,bottom};
			m_pBtnOK = new CButton("是",rect1,CButton::ID_OK);
			m_pBtnNO = new CButton("否",rect2,CButton::ID_OK);
			m_controls.push_back(m_pBtnOK);
			m_controls.push_back(m_pBtnNO);
			break;
		}
	case YESNOCANCEL:
		{
			left = ((m_rect.Right - m_rect.Left + 1) - (width * 3 + 4)) / 2 + m_rect.Left;
			left = left % 2 != 0?left + 1:left; 
			SMALL_RECT rect1={left,top,left + width - 1,bottom};
			SMALL_RECT rect2={rect1.Right + 1 + 2,top,rect1.Right + 1 + 2 + width - 1,bottom};
			SMALL_RECT rect3={rect2.Right + 1 + 2,top,rect2.Right + 1 + 2 + width - 1,bottom};
			m_pBtnOK = new CButton("是",rect1,CButton::ID_OK);
			m_pBtnNO = new CButton("否",rect2,CButton::ID_OK);
			m_pBtnCancel = new CButton("取消",rect3,CButton::ID_OK);
			m_controls.push_back(m_pBtnOK);
			m_controls.push_back(m_pBtnNO);
			m_controls.push_back(m_pBtnCancel);
			break;
		}
	}
}	
CMessageBox::DialogResult CMessageBox::Show(const string & message,const string &title,MessageBoxButtons type)
{

	SMALL_RECT rect ={0,0,79,24};
	rect.Left = 80 / 4 ;
	rect.Top = 25 / 4;
	rect.Right = 80 / 4 *3 - 1;
	rect.Bottom = 25 / 4 * 3;
	COORD size = {rect.Right - rect.Left + 1 ,rect.Bottom - rect.Top + 1};
	COORD pos ={0,0};
	SMALL_RECT temp = rect;
	//temp.Right+=2;
	CHAR_INFO buffer[81 * 26];
	//保存控制台指定区域的数据
	SaveWindow(size,pos,temp);
	CMessageForm messageForm(title,rect,type,message);
	int index = messageForm.Load(0);
	//恢复控制台指定区域的数据
	RetWindow();
	switch (index)
	{
	case 0:
		return YES;
	case 1:
		return NO;
	case 2:
		return CANCEL;
	}
	return YES;
}
void CMessageBox::CMessageForm::OnOk()
{
	m_exit = true;
}
CMessageBox::CMessageForm::~CMessageForm()
{

}