//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CEdit.h
//  @ Date : 2013/9/12
//  @ Author : 
//
//


#if !defined(_CEDIT_H)
#define _CEDIT_H

#include "CForm.h"

class CEdit : public CForm {
public:
	int Input(int key);
	void Show();
	void PrintTitle(int alignment);
	CEdit(string title, const SMALL_RECT &rect, int maxLen, int type=Text, int aligment=Left);
	CEdit();
	void FocusOn();
	void FocusOff();
protected:
	int m_maxLength;
	int m_type;
	int m_alignment;
};

#endif  //_CEDIT_H
