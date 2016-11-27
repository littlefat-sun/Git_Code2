#include <iostream>
using namespace std;


class Base1
{
public:
	void display()
	{
		cout<<"base "<<endl;
	}

};
class Base2
{
public:
	void display()
	{
		cout<<"base "<<endl;
	}

};
class child: public Base1,public Base2
{

};
//template<class T>
//class NameObject
//{
//public:
//	NameObject(string& name,const T& value)
//	{
//		nameValue=name;
//		objectValue=value;
//	}
//private:
//	string& nameValue;
//	const T objectValue;
//};
int main()
{
	 //string newDog("persephone");
	 //string oldDog("satch");
	 //NameObject<int> p(newDog,2);
	 //NameObject<int> s(oldDog,36);
	 //p=s;
	child ch;
	ch.display();
	system("pause");
	return 0;
}