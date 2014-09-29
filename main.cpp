#include "CDialogForm.h"
#include "CStatic.h"
#include "CLoginForm.h"
#include <iostream>
using namespace std;
int main(){
// 	char *ss="得到";
// 	printf("%s",ss+1);
// 	CForm::StartConsole();
// 	CForm::ShowCursor(FALSE);
// 	CForm::ChgAttribute(CForm::Blue,CForm::White);
// 	SMALL_RECT rect={0,0,79,24};
// 	CForm::ClearWindow(rect);
// 	CDialogForm cd("你们的萨芬",rect);
// 	cd.Show();
// 	SMALL_RECT rect2={30,20,36,21};
// 	CStatic sct("用户名",rect2);
// 	sct.Show();
	// 	CForm::CloseConsole();
	CForm::StartConsole();
	SMALL_RECT rect = {0,0,79,24};
	CLoginForm loginForm("登录",rect);

	loginForm.Load(0);

	CForm::CloseConsole();
	return 0;
}