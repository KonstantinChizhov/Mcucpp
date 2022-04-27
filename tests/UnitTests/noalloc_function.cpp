#include <noalloc_function.h>
#include <gtest/gtest.h>
#include <functional>

using namespace Mcucpp;

static bool freeFunc0Called = false;
static bool lambdaCalled = false;

void FreeFunc0()
{
    freeFunc0Called = true;
}

typedef void (*VoidFunc0T)();

TEST(NoallocFunction, FreeFunction)
{

    noalloc_function<void()> func;
    EXPECT_FALSE((bool)func);
    func = FreeFunc0;
    EXPECT_TRUE((bool)func);
    func();
    EXPECT_TRUE(freeFunc0Called);
    freeFunc0Called = false;
    func = nullptr;
    func();
    EXPECT_FALSE((bool)func);
    EXPECT_FALSE(freeFunc0Called);

    VoidFunc0T fPtr = FreeFunc0;

    std::function<void()> func2{fPtr};
    EXPECT_TRUE((bool)func2);
}

TEST(NoallocFunction, NonCapturingLambda)
{
    noalloc_function<void()> func([]()
                                  { lambdaCalled = true; });
    EXPECT_TRUE((bool)func);

    func();
    EXPECT_TRUE(lambdaCalled);
    lambdaCalled = false;
    func = nullptr;
    func();
    EXPECT_FALSE((bool)func);
    EXPECT_FALSE(lambdaCalled);
}

TEST(NoallocFunction, CapturingLambda)
{
    volatile bool called = true;
    volatile int intVar = 10;
    volatile int intOutVar = 8;
    volatile int intOut2Var = 6;

    auto lambda = [called, intVar, &intOutVar, &intOut2Var]()
    {lambdaCalled = called && intVar > 2; intOutVar = 3; intOut2Var=7; };

    noalloc_function<void()> func(lambda);

    EXPECT_TRUE((bool)func);

    func();
    EXPECT_EQ(intOutVar, 3);
    EXPECT_EQ(intOut2Var, 7);
    EXPECT_TRUE(lambdaCalled);

    lambdaCalled = false;
    func = nullptr;
    func();
    EXPECT_FALSE((bool)func);
    EXPECT_FALSE(lambdaCalled);
}

struct TestClass
{
    bool called = false;
    void Func() { called = true; }
};

TEST(NoallocFunction, ClassFunction)
{
    TestClass object;

    noalloc_function<void()> func = bind_s(&TestClass::Func, object);

    EXPECT_TRUE((bool)func);

    func();

    EXPECT_TRUE(object.called);

    object.called = false;
    func = nullptr;
    func();
    EXPECT_FALSE((bool)func);
    EXPECT_FALSE(object.called);
}

struct ClassWithCallback
{
    int a = -1;

    noalloc_function<int(int)> callback;
    void Action()
    {
        auto temp = callback;
        a = temp(42);
    }
};

TEST(NoallocFunction, CopyAssign)
{
    ClassWithCallback object;
    object.callback = [](int v)
    { return v + 1; };

    object.Action();
    EXPECT_EQ(object.a, 43);
}