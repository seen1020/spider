#ifndef CSPIDERFORM_H
#define CSPIDERFORM_H
#include "CDialogForm.h"
class CSpiderForm:public CDialogForm{
public:
	CSpiderForm(const string &title,const SMALL_RECT &rect);
	virtual void OnOk();
	virtual void OnCancel();
	virtual void OnNo();
	void ChangeBtn(int pre,int now);
	static CSpiderForm & GetInstance();
	bool isShow;
protected:
	CEdit *m_pTxtURL;
	CEdit *m_pTxtDepth;
	CButton *pYes,*pNo,*pEsc;
private:
};
#endif