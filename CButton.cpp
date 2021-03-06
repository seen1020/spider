//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CButton.cpp
//  @ Date : 2013/9/12
//  @ Author : 
//
//


#include "CButton.h"

CButton::CButton() {

}

CButton::CButton(string title, const SMALL_RECT &rect, int type):CForm(title,rect) {
	m_type=type;
	m_iFrame=CForm::Single;
}

void CButton::Show() {
	ClearWindow(m_rect);
	DrawFrame(m_iFrame);
	PrintTitle(CForm::Middle);
}

int CButton::Input(int key) {
	char ch;
	int index = 0;
	while(true)
	{
		ch = _getch();
		if (ch == ESC || ch == '\t')
		{
			break;
		}
		else if ( ch == ENTER)
		{
			ch = m_type;
			break;
		}
		if (ch == -32 || ch == 0)
		{
			ch = _getch();
			if (ch == UP || ch == DOWN)
			{
				break;
			}
		}
	}
	return ch;
}

void CButton::FocusOn(){
	SetCursorPos(1,1);
	ShowCursor(false);
	m_iFrame=CForm::Double;
	Show();
}
void CButton::FocusOff(){
	m_iFrame=CForm::Single;
	Show();
}
