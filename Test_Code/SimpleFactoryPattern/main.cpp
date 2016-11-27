#include <iostream>
#include <cstring>
using namespace std;



class AbstractOperator
{

public:
	virtual double getResult()	 const{return 0;}
	void setParam1(double p)
	{
		param1=p;
	}
	double getParam1()
	{
		return param1;
	}
	void setParam2(double p)
	{
		param2=p;
	}
	double getParam2()
	{
		return param2;
	}
public:
	double param1;
	double param2;

};
class AddOperator:public AbstractOperator
{
public:
	virtual double getResult()const
	{
		return param1+param2;
	}

};
class ClassFactory
{
public:
	static AbstractOperator* createOperator(int str)
	{
		AbstractOperator* ap=NULL;
		switch(str)
		{
		case 0:
			{
				ap=new AddOperator;
				break;
			}
		case 1:
			{
			}
		}
		return ap;
	}
};
//½çÃæ 
int main()
{

	AbstractOperator* ab=NULL;
	ab=ClassFactory::createOperator(0);
	ab->param1=10.0;
	ab->param2=12;
	cout<<ab->getResult()<<endl;


	system("pause");
	return 0;
}