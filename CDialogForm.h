//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CDialogForm.h
//  @ Date : 2013/9/12
//  @ Author : 
//
//


#if !defined(_CDIALOGFORM_H)
#define _CDIALOGFORM_H

#include "CForm.h"
#include "CStatic.h"
#include "CEdit.h"
#include "CButton.h"
#include <vector>
#include "CWebSites.h"

class CDialogForm : public CForm {
public:
	CDialogForm();
	CDialogForm(string title, const SMALL_RECT &rect);
	virtual void OnOk();
	virtual void OnNo();
	virtual void OnCancel();
	void SetControlFocus(int p);
	void DoWillKeyDown(char key);
	~CDialogForm();
	void Show();
	void PrintTitle(int alignment=CForm::Middle);
	int Input(int key);
	virtual int Load(int key);
	void createLableEdit(string s,int len1,int len2,int x,int y,CEdit*&ret);
	vector<CForm *> &CDialogForm::GetControls()
	{
		return m_controls;
	}
protected:
	static SMALL_RECT defaultRect;
	bool m_exit;
	int m_index;
	vector<CForm *> m_controls;
};

#endif  //_CDIALOGFORM_H
