#include <iostream>
#include <string>
using namespace std;

//公共抽象类
class Phone
{
public:
	Phone(){};
	virtual ~Phone(){}
	virtual void ShowDecorate() = 0;
};
//具体手机类1  安卓手机
class AndroidPhone:public Phone
{
public:
	AndroidPhone(string str):phoneName(str)
	{

	}
	~AndroidPhone(){}
	void  ShowDecorate()
	{
		cout<<phoneName<<" 的装饰"<<endl;
	}

private:
	string phoneName;	//手机名称
};
//具体手机类2 诺基亚手机
class NokiaPhone:public Phone
{
public:
	NokiaPhone(string str):phoneName(str)
	{

	}
	~NokiaPhone(){}
	void ShowDecorate()
	{
		cout<<phoneName<<" 的装饰"<<endl;
	}
private:
	string phoneName;

};
//装饰类
class Decorator:public Phone 
{
public:
	Decorator(Phone* p):m_Phone(p)
	{

	}
	~Decorator(){}
	void ShowDecorate()
	{
		m_Phone->ShowDecorate();
	}
private:
	Phone* m_Phone;//要装饰的手机
};
//具体装饰类1
class DecoratorA:public Decorator
{
public:
	DecoratorA(Phone* p):Decorator(p)
	{

	}
	~DecoratorA(){}
	void ShowDecorate()
	{
		Decorator::ShowDecorate();
		addNewDecorator();
	}
private:
	void addNewDecorator()
	{
		cout<<"增加手机挂件"<<endl;
	}
};
//具体装饰类2
class DecoratorB:public Decorator
{
public:
	DecoratorB(Phone* p):Decorator(p)
	{

	}
	~DecoratorB(){}
	void ShowDecorate()
	{
		Decorator::ShowDecorate();
		addNewDecorator();
	}
private:
	void addNewDecorator()
	{
		cout<<"手机贴膜"<<endl;
	}
};
int main()
{
	Phone* phone = new AndroidPhone("华为");
	Phone* dpa = new DecoratorA(phone);
	Phone* dpb = new DecoratorB(dpa);
	dpb->ShowDecorate();
	delete dpb;
	delete dpa;
	delete phone;

	system("pause");
	return 0;
}