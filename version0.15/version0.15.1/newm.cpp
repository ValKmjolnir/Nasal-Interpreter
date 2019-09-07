#include<iostream>
using namespace std;
class a
{
	public:
		int A;
		virtual void setA(int t)=0;
		virtual int returnA()=0;
};
class b:public a
{
	public:
		void setA(int t)
		{
			A=t;
			return;
		}
		int returnA()
		{
			return A;
		}
};
int main()
{
	b m;
	m.setA(1);
	m.returnA();
	return 0;
}
