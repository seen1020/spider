//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CLoginForm.h
//  @ Date : 2013/9/12
//  @ Author : 
//
//


#if !defined(_CMAINMENUFORM_H)
#define _CMAINMENUFORM_H

#include "CDialogForm.h"
#include "CButton.h"
#include "CStatic.h"
#include "CEdit.h"
class CMainMenuForm : public CDialogForm {
public:
	CMainMenuForm(const string& title,const SMALL_RECT &rect);
	void OnOk();
	void OnCancel();
protected:
	vector<string> m_menu;
private:
	int errorNum;
};

#endif  //_CLOGINFORM_H