// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "DirectXMatrixStack.h"

#include <cstdio>

using namespace DirectX;

namespace
{
    typedef bool (*TestFN)();

    struct TestInfo
    {
        const char* name;
        TestFN func;
    };
}

bool Test01();

TestInfo g_Tests[] =
{
    { "MatrixStack", Test01 },
};


//-------------------------------------------------------------------------------------
int main()
{
    size_t nPass = 0;
    size_t nFail = 0;

    for (size_t i = 0; i < (sizeof(g_Tests) / sizeof(TestInfo)); ++i)
    {
        printf("%s: ", g_Tests[i].name);

        if (g_Tests[i].func())
        {
            ++nPass;
            printf("PASS\n");
        }
        else
        {
            ++nFail;
            printf("FAIL\n");
        }
    }

    printf("Ran %zu tests, %zu pass, %zu fail\n", nPass + nFail, nPass, nFail);

    return (nFail == 0);
}


//-------------------------------------------------------------------------------------
bool Test01()
{
    bool success = true;

    {
        MatrixStack stack;

        if (!XMMatrixIsIdentity(stack.Top()))
        {
            printf("ERROR: Identity failed\n");
            success = false;
        }

        if (stack.Size() != 1)
        {
            printf("ERROR: Size failed\n");
            success = false;
        }

        stack.Push();
        stack.Push();

        if (stack.Size() != 3)
        {
            printf("ERROR: 2xPush, Size failed\n");
            success = false;
        }

        stack.Pop();

        if (stack.Size() != 2)
        {
            printf("ERROR: Pop, Size failed\n");
            success = false;
        }
    }

    return success;
}
