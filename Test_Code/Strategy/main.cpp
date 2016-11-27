#include <iostream>
using namespace std;

//�������
class AbstractStrategy
{
public:
	virtual void algorithmInterface()=0;
};
//����A
class ConcreteStrategyA: public AbstractStrategy
{
public:
	virtual void algorithmInterface(){ cout<<"strategyA"<<endl; }
};
//����B
class ConcreteStrategyB: public AbstractStrategy
{
public:
	virtual void algorithmInterface(){ cout<<"strategyB"<<endl; }
};
//////
class Context
{
public:
	//�������Ĳ��Զ���
	Context(AbstractStrategy* s)
	{
		 strategy=s;
	}
	//���ö�Ӧ�Ĳ��Զ�����㷨
	void contextInterface()
	{
		strategy->algorithmInterface();
	}
private:
	AbstractStrategy* strategy;
};
int main()
{
	Context* context1=new Context(new ConcreteStrategyA);
	context1->contextInterface();
	Context* context2=new Context(new ConcreteStrategyB);
	context2->contextInterface();

	system("pause");
	return 0;
}