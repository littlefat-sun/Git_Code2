#include <iostream>
using namespace std;

//抽象策略
class AbstractStrategy
{
public:
	virtual void algorithmInterface()=0;
};
//策略A
class ConcreteStrategyA: public AbstractStrategy
{
public:
	virtual void algorithmInterface(){ cout<<"strategyA"<<endl; }
};
//策略B
class ConcreteStrategyB: public AbstractStrategy
{
public:
	virtual void algorithmInterface(){ cout<<"strategyB"<<endl; }
};
//////
class Context
{
public:
	//传入具体的策略对象
	Context(AbstractStrategy* s)
	{
		 strategy=s;
	}
	//调用对应的策略对象的算法
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