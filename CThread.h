#ifndef CTHREAD_H
#define CTHREAD_H
class CThread  //��������
{
public:
	//���̳߳ص��õ��麯��
	virtual void Run(void *pPara) = 0;
};
#endif