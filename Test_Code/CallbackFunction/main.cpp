#include <iostream>
using namespace std;
 //回调函数
//为什么要这样做呢？为什么不把定义好的函数放在头文件中呢？
//因为可以作为参数的函数A的名称和实现可以被抽象出来，也就是在写函数B的时候，
//不需要管函数A叫什么名字，怎么实现，只要函数A的返回类型是一定的就可以了（泛型的话连这个也不需要）。
int callbackFunc(int i)
{
	 cout<<i;
	 return 1;
}
//参数1位函数指针，参数2位回调函数的参数类型
void CallBack(int (*calls)(int),int j)
{
	calls(j);
}
int main()
{
	CallBack(callbackFunc,9);
	system("pause");
	return 0;
}