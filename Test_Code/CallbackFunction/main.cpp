#include <iostream>
using namespace std;
 //�ص�����
//ΪʲôҪ�������أ�Ϊʲô���Ѷ���õĺ�������ͷ�ļ����أ�
//��Ϊ������Ϊ�����ĺ���A�����ƺ�ʵ�ֿ��Ա����������Ҳ������д����B��ʱ��
//����Ҫ�ܺ���A��ʲô���֣���ôʵ�֣�ֻҪ����A�ķ���������һ���ľͿ����ˣ����͵Ļ������Ҳ����Ҫ����
int callbackFunc(int i)
{
	 cout<<i;
	 return 1;
}
//����1λ����ָ�룬����2λ�ص������Ĳ�������
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