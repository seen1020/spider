#include "CDialogForm.h"
#include "CStatic.h"
#include "CLoginForm.h"
#include <iostream>
using namespace std;
int main(){
// 	char *ss="�õ�";
// 	printf("%s",ss+1);
// 	CForm::StartConsole();
// 	CForm::ShowCursor(FALSE);
// 	CForm::ChgAttribute(CForm::Blue,CForm::White);
// 	SMALL_RECT rect={0,0,79,24};
// 	CForm::ClearWindow(rect);
// 	CDialogForm cd("���ǵ�����",rect);
// 	cd.Show();
// 	SMALL_RECT rect2={30,20,36,21};
// 	CStatic sct("�û���",rect2);
// 	sct.Show();
	// 	CForm::CloseConsole();
	CForm::StartConsole();
	SMALL_RECT rect = {0,0,79,24};
	CLoginForm loginForm("��¼",rect);

	loginForm.Load(0);

	CForm::CloseConsole();
	return 0;
}