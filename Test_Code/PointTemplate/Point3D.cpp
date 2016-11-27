
#include <iostream>
#include <assert.h>
#include <string.h>
using namespace std;

template<class T, int dim>
class Point
{
public:
	Point();
	Point(T coordi[dim])
	{
		for (int i=0;i<dim;i++)
		{
			_coordi[i]=coordi[i];
		}
	}
	T& operator[](int index)
	{
		assert(index<dim&&index>=0);
		return _coordi[index];
	}
private:
	T _coordi[dim];
};

template<class T, int dim>
inline ostream& operator<<( ostream& os,Point<T,dim>& pt)
{
	os<<"( ";
	for (int ii=0;ii<dim;ii++)
		os<<pt[ii]<<", ";
	os<<pt[dim-1];
	os<<" )";
	return os;
}
class VoidTest
{
public:
	operator void* ()
	{
		cout<<"a";
		return (void*)0;
	}
	bool operator !()
	{
		cout<<"c";
		return true;
	}
};
class AddressTest
{
public:
	 virtual ~AddressTest();
public:
	float x;
	float y;
	float z;
};
AddressTest::~AddressTest()
{

}
class Base1
{
public:
	Base1(){}
	void t1(){}
	virtual ~Base1(){}
	virtual void speakClearly(){};
	virtual Base1 *clone() const{cout<<"base1"<<endl;return NULL;}
protected:
	float data_Base1;
};
class Base2
{
public:
	Base2(){}
	void t2(){};
	virtual ~Base2(){}
	virtual void mumble(){}
	virtual Base2 *clone() const {cout<<"base2"<<endl;return NULL;}
protected:
	float data_Base2;
};
class Derived:public Base1,public Base2
{
public:
	Derived(){}
	void t3(){}
	virtual ~Derived(){}
	virtual Derived *clone() const{cout<<"derived"<<endl;return NULL;}
protected:
	float data_Derived;
};
int main()
{
	//int d[3]={1,2,3};
	//Point<int,3> p(d);
	//cout<<p;

	//VoidTest a;
	//if (a)
	//{
	//	cout<<"b";
	//}
	//if (!a)
	//{
	//	cout<<"d";
	//}

	//AddressTest a;
	////cout<<&a.x<<endl;
	//////cout<<(&a+(&AddressTest::x-1));
	////cout<<&a<<endl;
	////cout<<&a.y<<endl;
	////cout<<&AddressTest::x<<endl;
	////cout<<&AddressTest::y<<endl;
	////cout<<&AddressTest::z;
	//printf("%p\n",&a.x);
	//printf("%p\n",&a.y);
	//printf("%p\n",&a.z);
	//printf("%p\n",&AddressTest::x);
	//printf("%p\n",&AddressTest::y);
	//printf("%p\n",&AddressTest::z);

	Derived* p=new Derived;
	Base1 *p1=new Derived;
	system("pause");
	return 0;
}
