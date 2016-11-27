#include <iostream>
#include <string>
using namespace std;

//����������
class Phone
{
public:
	Phone(){};
	virtual ~Phone(){}
	virtual void ShowDecorate() = 0;
};
//�����ֻ���1  ��׿�ֻ�
class AndroidPhone:public Phone
{
public:
	AndroidPhone(string str):phoneName(str)
	{

	}
	~AndroidPhone(){}
	void  ShowDecorate()
	{
		cout<<phoneName<<" ��װ��"<<endl;
	}

private:
	string phoneName;	//�ֻ�����
};
//�����ֻ���2 ŵ�����ֻ�
class NokiaPhone:public Phone
{
public:
	NokiaPhone(string str):phoneName(str)
	{

	}
	~NokiaPhone(){}
	void ShowDecorate()
	{
		cout<<phoneName<<" ��װ��"<<endl;
	}
private:
	string phoneName;

};
//װ����
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
	Phone* m_Phone;//Ҫװ�ε��ֻ�
};
//����װ����1
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
		cout<<"�����ֻ��Ҽ�"<<endl;
	}
};
//����װ����2
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
		cout<<"�ֻ���Ĥ"<<endl;
	}
};
int main()
{
	Phone* phone = new AndroidPhone("��Ϊ");
	Phone* dpa = new DecoratorA(phone);
	Phone* dpb = new DecoratorB(dpa);
	dpb->ShowDecorate();
	delete dpb;
	delete dpa;
	delete phone;

	system("pause");
	return 0;
}