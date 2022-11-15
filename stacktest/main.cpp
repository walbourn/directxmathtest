// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "DirectXMatrixStack.h"

#include <cstdio>
#include <type_traits>


using namespace DirectX;

static_assert(!std::is_copy_constructible<MatrixStack>::value, "Copy ctor)");
static_assert(!std::is_copy_assignable<MatrixStack>::value, "Copy Assign)");
static_assert(std::is_nothrow_move_constructible<MatrixStack>::value, "Move Ctor.");
static_assert(std::is_nothrow_move_assignable<MatrixStack>::value, "Move Assign.");

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
bool Test02();
bool Test03();
bool Test04();
bool Test05();
bool Test06();

TestInfo g_Tests[] =
{
    { "MatrixStack", Test01 },
    { "MatrixMultiply", Test02 },
    { "RotateX/Y/Z", Test03 },
    { "Rotate", Test04 },
    { "Scale", Test05 },
    { "Translate", Test06 }
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

    return (nFail == 0) ? 0 : 1;
}


//-------------------------------------------------------------------------------------
bool Test01()
{
    bool success = true;

    {
        MatrixStack stack;

        if (!XMMatrixIsIdentity(stack.Top()))
        {
            printf("ERROR: Top failed\n");
            success = false;
        }

        assert(stack.GetTop() != nullptr);
        if (!XMMatrixIsIdentity(*stack.GetTop()))
        {
            printf("ERROR: GetTop failed\n");
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

    {
        MatrixStack stack;

        for (size_t j = 0; j < 138; ++j)
        {
            stack.Push();

            XMMATRIX m;            
            m.r[0] = XMVectorSetW(g_XMIdentityR0, float(j));
            m.r[1] = XMVectorSetZ(g_XMIdentityR1, float(j));
            m.r[2] = XMVectorSetY(g_XMIdentityR2, float(j));
            m.r[3] = XMVectorSetX(g_XMIdentityR3, float(j));

            stack.LoadMatrix(m);

            const auto& top = stack.Top();
            if (!XMVector4Equal(top.r[0], m.r[0])
                || !XMVector4Equal(top.r[1], m.r[1])
                || !XMVector4Equal(top.r[2], m.r[2])
                || !XMVector4Equal(top.r[3], m.r[3]))
            {
                printf("ERROR: Push with growth failed\n");
                success = false;
            }
        }

        if (stack.Size() != 139)
        {
            printf("ERROR: Push with growth failed (%zu .. 139)\n", stack.Size());
            success = false;
        }

        for (size_t j = 139; j > 121; --j)
        {
            stack.Pop();
        }

        if (stack.Size() != 121)
        {
            printf("ERROR: Pop failed (%zu .. 121)\n", stack.Size());
                success = false;
        }
    }

    return success;
}


//-------------------------------------------------------------------------------------
bool Test02()
{
    bool success = true;

    {
        MatrixStack stack;

        XMMATRIX m = XMMatrixIdentity();
        m.r[0] = g_XMTwo;
        m.r[2] = g_XMTwoPi;

        stack.Push();
        stack.MultiplyMatrix(m);

        {
            const auto& top = stack.Top();
            if (!XMVector4Equal(top.r[0], m.r[0])
                || !XMVector4Equal(top.r[1], m.r[1])
                || !XMVector4Equal(top.r[2], m.r[2])
                || !XMVector4Equal(top.r[3], m.r[3]))
            {
                printf("ERROR: MultiplyMatrix failed\n");
                success = false;
            }
        }

        stack.LoadIdentity();

        stack.MultiplyMatrixLocal(m);

        {
            const auto& top = stack.Top();
            if (!XMVector4Equal(top.r[0], m.r[0])
                || !XMVector4Equal(top.r[1], m.r[1])
                || !XMVector4Equal(top.r[2], m.r[2])
                || !XMVector4Equal(top.r[3], m.r[3]))
            {
                printf("ERROR: MultiplyMatrixLocal failed\n");
                success = false;
            }
        }

        stack.MultiplyMatrix(m);

        {
            const auto& top = stack.Top();

            static const XMVECTORF32 s_check0 = { { { 16.5663719f, 18.5663719f, 16.5663719f, 18.5663719f } } };
            static const XMVECTORF32 s_check2 = { { { 52.0447922f, 58.3279762f, 52.0447922f, 58.3279762f } } };

            if (!XMVector4NearEqual(top.r[0], s_check0, g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], g_XMIdentityR1, g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], s_check2, g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], g_XMIdentityR3, g_XMEpsilon))
            {
                printf("ERROR: MultiplyMatrixLocal failed\n");
                success = false;
            }
        }
    }

    return success;
}


//-------------------------------------------------------------------------------------
bool Test03()
{
    bool success = true;

    // RotateX
    {
        MatrixStack stack;

        constexpr float angle = XM_PIDIV2;
        stack.RotateX(angle);

        {
            XMMATRIX check = XMMatrixRotationX(angle);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateX failed\n");
                success = false;
            }
        }
    }

    {
        MatrixStack stack;

        constexpr float angle = XM_PIDIV2;
        stack.RotateXLocal(angle);

        {
            XMMATRIX check = XMMatrixRotationX(angle);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateXLocal failed\n");
                success = false;
            }
        }
    }

    // RotateY
    {
        MatrixStack stack;

        constexpr float angle = XM_PIDIV2;
        stack.RotateY(angle);

        {
            XMMATRIX check = XMMatrixRotationY(angle);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateY failed\n");
                success = false;
            }
        }
    }

    {
        MatrixStack stack;

        constexpr float angle = XM_PIDIV2;
        stack.RotateYLocal(angle);

        {
            XMMATRIX check = XMMatrixRotationY(angle);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateYLocal failed\n");
                success = false;
            }
        }
    }

    // RotateZ
    {
        MatrixStack stack;

        constexpr float angle = XM_PIDIV2;
        stack.RotateZ(angle);

        {
            XMMATRIX check = XMMatrixRotationZ(angle);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateZ failed\n");
                success = false;
            }
        }
    }

    {
        MatrixStack stack;

        constexpr float angle = XM_PIDIV2;
        stack.RotateZLocal(angle);

        {
            XMMATRIX check = XMMatrixRotationZ(angle);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateZLocal failed\n");
                success = false;
            }
        }
    }

    return success;
}


//-------------------------------------------------------------------------------------
bool Test04()
{
    bool success = true;

    // RotateAxis
    {
        MatrixStack stack;

        XMVECTOR axis = XMVector4Normalize(g_XMOne);

        constexpr float angle = XM_PIDIV2;
        stack.RotateAxis(axis, angle);

        {
            XMMATRIX check = XMMatrixRotationAxis(axis, angle);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateAxis failed\n");
                success = false;
            }
        }
    }

    {
        MatrixStack stack;

        XMVECTOR axis = XMVector4Normalize(g_XMOne);

        constexpr float angle = XM_PIDIV2;
        stack.RotateAxisLocal(axis, angle);

        {
            XMMATRIX check = XMMatrixRotationAxis(axis, angle);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateAxisLocal failed\n");
                success = false;
            }
        }
    }

    // RotateRollPitchYaw
    {
        MatrixStack stack;

        constexpr float x = XM_PIDIV4;
        constexpr float y = XM_PIDIV2;
        constexpr float z = XM_2PI;
        stack.RotateRollPitchYaw(x, y, z);

        {
            XMMATRIX check = XMMatrixRotationRollPitchYaw(x, y, z);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateRollPitchYaw failed\n");
                success = false;
            }
        }
    }

    {
        MatrixStack stack;

        constexpr float x = XM_PIDIV4;
        constexpr float y = XM_PIDIV2;
        constexpr float z = XM_2PI;
        stack.RotateRollPitchYawLocal(x, y, z);

        {
            XMMATRIX check = XMMatrixRotationRollPitchYaw(x, y, z);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateRollPitchYawLocal faile\n");
                success = false;
            }
        }
    }

    // RotateByQuaternion
    {
        MatrixStack stack;

        XMVECTOR quat = XMVector4Normalize(g_XMOne);
        stack.RotateByQuaternion(quat);

        {
            XMMATRIX check = XMMatrixRotationQuaternion(quat);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateByQuaternion failed\n");
                success = false;
            }
        }
    }

    {
        MatrixStack stack;

        XMVECTOR quat = XMVector4Normalize(g_XMOne);
        stack.RotateByQuaternionLocal(quat);

        {
            XMMATRIX check = XMMatrixRotationQuaternion(quat);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: RotateByQuaternionLocal failed\n");
                success = false;
            }
        }
    }

    return success;
}


//-------------------------------------------------------------------------------------
bool Test05()
{
    bool success = true;

    {
        MatrixStack stack;

        constexpr float x = 2.f;
        constexpr float y = 7.f;
        constexpr float z = 12.f;
        stack.Scale(x, y, z);

        {
            XMMATRIX check = XMMatrixScaling(x, y, z);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: Scale failed\n");
                success = false;
            }
        }
    }

    {
        MatrixStack stack;

        constexpr float x = 2.f;
        constexpr float y = 7.f;
        constexpr float z = 12.f;
        stack.ScaleLocal(x, y, z);

        {
            XMMATRIX check = XMMatrixScaling(x, y, z);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: ScaleLocal failed\n");
                success = false;
            }
        }
    }

    return success;
}


//-------------------------------------------------------------------------------------
bool Test06()
{
    bool success = true;

    {
        MatrixStack stack;

        constexpr float x = 2.f;
        constexpr float y = 7.f;
        constexpr float z = 12.f;
        stack.Translate(x, y, z);

        {
            XMMATRIX check = XMMatrixTranslation(x, y, z);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: Translate failed\n");
                success = false;
            }
        }
    }

    {
        MatrixStack stack;

        constexpr float x = 2.f;
        constexpr float y = 7.f;
        constexpr float z = 12.f;
        stack.TranslateLocal(x, y, z);

        {
            XMMATRIX check = XMMatrixTranslation(x, y, z);
            const auto& top = stack.Top();
            if (!XMVector4NearEqual(top.r[0], check.r[0], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[1], check.r[1], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[2], check.r[2], g_XMEpsilon)
                || !XMVector4NearEqual(top.r[3], check.r[3], g_XMEpsilon))
            {
                printf("ERROR: TranslateLocal failed\n");
                success = false;
            }
        }
    }

    return success;
}
