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
	//使用枚举提高代码的可读性，你也可以坚持使用整型
	enum MessageBoxButtons{OK = 0, YESNO, YESNOCANCEL};
	enum DialogResult{ YES = 0, NO,CANCEL};
	static DialogResult Show(const string & message = "Error",const string &title = "Info",MessageBoxButtons type = OK);
protected:
private:
	//内部类或者嵌套类
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

