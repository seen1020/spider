#ifndef CTHREAD_H
#define CTHREAD_H
class CThread  //工作基类
{
public:
	//供线程池调用的虚函数
	virtual void Run(void *pPara) = 0;
};
#endif