#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <iterator>
#include <algorithm>
#include <assert.h>
using namespace std;

//class Father
//{
//
//};
//class Child
//{
//
//};
//
//class Game {
//private:
//	static const int GameTurn ;
//	int scores[GameTurn];
//};
//  const int GameTurn = 10;
class A
{
public:
	int data;
};
template<class T>
ostream& operator<<(ostream& strm,const auto_ptr<T>& p)
{
	if (p.get() == NULL)
	{
		strm<<"NULL";
	} 
	else
	{
		strm<<*p;
	}
	return strm;
}

class CalculateAverageOfPowers
{
public:
	CalculateAverageOfPowers(float p) : acc(0), n(0), p(p)
	{

	}
	void operator() (float x)
	{
		acc += pow(x, p);
		n++; 
	}
	float getAverage() const 
	{ 
		return acc / n; 
	}
private:
	float acc;
	int   n;
	float p;
};

class TempClass
{
public:
	TempClass()
	{
		 num = 20;
	}
	void func()
	{
		cout<<"class alive!!!"<<endl;
	}
	int num;
};

class AAA
{
public:
	AAA()
	{
		ii=135;
	}
	int ii;
};
int main()
{
	TempClass* temp1 = new TempClass;
	delete temp1;
	AAA* a = new AAA;
	cout<<"num: "<<temp1->num<<endl;
	temp1->func();


	system("pause");
	return 0;
}