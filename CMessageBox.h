#ifndef CMESSAGEBOX_H
#define CMESSAGEBOX_H
#include "CDialogForm.h"
#include "CButton.h"
#include "CStatic.h"
#include <string>
#include <IOSTREAM>
using namespace std;
class CMessageBox:CForm
{
public:
	//ʹ��ö����ߴ���Ŀɶ��ԣ���Ҳ���Լ��ʹ������
	enum MessageBoxButtons{OK = 0, YESNO, YESNOCANCEL};
	enum DialogResult{ YES = 0, NO,CANCEL};
	static DialogResult Show(const string & message = "Error",const string &title = "Info",MessageBoxButtons type = OK);
protected:
private:
	//�ڲ������Ƕ����
	class CMessageForm;
};
class CMessageBox::CMessageForm:public CDialogForm
{
public:
	CMessageForm(const string &title, const SMALL_RECT &rect, CMessageBox::MessageBoxButtons type, const string &message);
	virtual void OnOk();
	~CMessageForm();
protected:
	CButton *m_pBtnOK;
	CButton *m_pBtnNO;
	CButton *m_pBtnCancel;
private:
};
#endif

