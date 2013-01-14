
#include <gtest.h>
#include <delegate.h>

class Foo
{
public:
	Foo()
	{
		barCalled = 0;
	}

	void Bar()
	{
		barCalled++;
	}

	void Bar1(int a1)
	{
		barCalled++;
		arg1 = a1;
	}

	void Bar2(int a1, int a2)
	{
		barCalled++;
		arg1 = a1;
		arg2 = a2;
	}

	void Bar3(int a1, int a2, int a3)
	{
		barCalled++;
		arg1 = a1;
		arg2 = a2;
		arg3 = a3;
	}

	int barCalled;
	int arg1, arg2, arg3;
};

Foo foo;

int delegateTestFuncCalled = 0;
void DelegateTestFunc()
{
	delegateTestFuncCalled++;
}

TEST(Delegate, NoArgsClassMember)
{
	foo.barCalled = 0;
	Mcucpp::Delegate<void> delegate(foo, &Foo::Bar);
	delegate();
	EXPECT_EQ(1, foo.barCalled);
}

TEST(Delegate, OneArgClassMember)
{
	foo.barCalled = 0;
	Mcucpp::Delegate1<void, int> delegate(foo, &Foo::Bar1);
	delegate(10);
	EXPECT_EQ(1, foo.barCalled);
	EXPECT_EQ(10, foo.arg1);
}

TEST(Delegate, TwoArgClassMember)
{
	foo.barCalled = 0;
	Mcucpp::Delegate2<void, int, int> delegate(foo, &Foo::Bar2);
	delegate(10, 20);
	EXPECT_EQ(1, foo.barCalled);
	EXPECT_EQ(10, foo.arg1);
	EXPECT_EQ(20, foo.arg2);
}

TEST(Delegate, ThreeArgClassMember)
{
	foo.barCalled = 0;
	Mcucpp::Delegate3<void, int, int, int> delegate(foo, &Foo::Bar3);
	delegate(10, 20, 30);
	EXPECT_EQ(1, foo.barCalled);
	EXPECT_EQ(10, foo.arg1);
	EXPECT_EQ(20, foo.arg2);
	EXPECT_EQ(30, foo.arg3);
}

TEST(Delegate, NoArgsStaticFunc)
{
	delegateTestFuncCalled = 0;
	Mcucpp::Delegate<void> delegate(DelegateTestFunc);
	delegate();
	EXPECT_EQ(1, delegateTestFuncCalled);
}