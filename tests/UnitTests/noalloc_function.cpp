#include <noalloc_function.h>
#include <gtest/gtest.h>

using namespace Mcucpp;

static bool freeFunc0Called = false;
void FreeFunc0()
{
    freeFunc0Called = true;
}

typedef void (*VoidFunc0T)();

TEST(NoallocFunction, FreeFunction)
{
    noalloc_function<VoidFunc0T> func;
    EXPECT_FALSE((bool)func);
    func = freeFunc0Called;
    EXPECT_TRUE((bool)func);
    func();
    EXPECT_TRUE(freeFunc0Called);
    freeFunc0Called = false;
    func = nullptr;
     EXPECT_FALSE(freeFunc0Called);
}
