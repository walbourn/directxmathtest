//-------------------------------------------------------------------------------------
// xmvec.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

#ifdef _MSC_VER
// C5246: 'anonymous struct or union': the initialization of a subobject should be wrapped in brace
#pragma warning(disable: 4619 4616 5246)
#endif

static_assert(sizeof(XMVECTOR) == 16, "Unexpected structure size");

struct IntPairs_t {
    int x;
    int y;
};

struct FloatPairs_t {
    float x;
    float y;
};

HRESULT Test278(LogProxy* pLog)
{
    //XMVectorAbs
    HRESULT ret = S_OK;
    static const XMVECTORF32 v[] = {
        {0,1,2,3}, {0,1,2,3},
        {0,-1,2,-3},{0,1,2,3},
        {-1000,100,-10,1},{1000,100,10,1},
        {-c_INF,c_INF,0,-.00000000001f}, {c_INF,c_INF,0,.00000000001f}
    };
    for (int k = 0; k < countof(v); k += 2) {
        XMVECTOR r = XMVectorAbs(v[k]);
        COMPARISON c = CompareXMVECTOR(r, v[k + 1], 4);
        if (c > WITHINEPSILON) {
            XMVECTOR v1 = v[k], check = v[k + 1];
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test279(LogProxy* pLog)
{
    //XMVectorACos
    HRESULT ret = S_OK;
    COMPARISON c;

    XMVECTORF32 v1 = { {-1, 0, 1, 1} };
    XMVECTOR r;

    r = XMVectorACos(v1);
    XMVECTORF32 check = { {Pi, Pi / 2.f,0,0} };
    c = CompareXMVECTOR(r, check, 4);
    if (c > WITHIN10EPSILON) {
        printe("%s: %.0f, %.0f, %.0f %.0f: %f %f %f %f, MUST BE %f %f %f %f. %d\n",
            TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }

    XMVECTORF32 v = { {-1, -.5, 0, .5} };
    XMVECTOR adder = XMVectorReplicate(.03125);
    for (float vw = XMVectorGetW(v); ; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); float vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); vw = XMVectorGetW(v);
        if (vw > 1.0f)
            break;
        r = XMVectorACos(v);
        XMVECTORF32 check2 = { {acosf(vx),acosf(vy),acosf(vz),acosf(vw)} };
        COMPARISON worstc = EXACT;
        for (int i = 0; i < 4; i++) {
            c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i));
            worstc = std::max(c, worstc);
            if (c > WITHIN10EPSILON) {
                printe("%s: (%f) = %f ... %f   %d\n", TestName, XMVectorGetByIndex(v, i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i), c);
                ret = MATH_FAIL;
            }
        }
        printi("%s: %f %d\n", TestName, vx, worstc);
    }

    // Range for arc-cosine is [-1,1]
    for (float f = -1; f <= 1; f += .01f) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorACos(v);
        XMVECTOR check = XMVectorReplicate(acosf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}



HRESULT Test280(LogProxy* pLog)
{
    //XMVectorACosEst
    HRESULT ret = S_OK;
    COMPARISON c;

    XMVECTORF32 v1 = { {-1, 0, 1, 1} };
    XMVECTOR r;

    r = XMVectorACosEst(v1);
    XMVECTORF32 check = { {Pi, Pi / 2.f,0,0} };
    c = CompareXMVECTOR(r, check, 4);
    if (c > WITHINBIGEPSILON) {
        printe("%s: %.0f, %.0f, %.0f %.0f: %f %f %f %f, MUST BE %f %f %f %f. %d\n",
            TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }
    XMVECTORF32 v = { {-1, -.5, 0, .5} };
    XMVECTOR adder = XMVectorReplicate(.03125);
    for (float vw = XMVectorGetW(v); ; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); float vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); vw = XMVectorGetW(v);
        if (vw > 1.0f)
            break;
        r = XMVectorACosEst(v);
        XMVECTORF32 check2 = { {acosf(vx),acosf(vy),acosf(vz),acosf(vw)} };
        COMPARISON worstc = EXACT;
        for (int i = 0; i < 4; i++) {
            c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i));
            worstc = std::max(c, worstc);
            if (c > WITHINBIGGEREPSILON) {
                printe("%s: (%f) = %f ... %f   %d\n", TestName, XMVectorGetByIndex(v, i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i), c);
                ret = MATH_FAIL;
            }
        }
        printi("%s: %f %d\n", TestName, vx, worstc);
    }

    // Range for arc-cosine is [-1,1]
    for (float f = -1; f <= 1; f += .01f) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorACosEst(v);
        XMVECTOR check = XMVectorReplicate(acosf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGGEREPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}



HRESULT Test281(LogProxy* pLog)
{
    //XMVectorAdd
    //XMVECTOR operator +=, +
    COMPARISON c = EXACT;
    HRESULT ret = S_OK;
    static const XMVECTORF32 f[] = {
        {0,0,0,0},  {0,0,0,0},  {0,0,0,0},
        {.125,.25,-.75,1000},  {-.3125,0,90,-2},  {-.1875,.25,89.25,998},
        {.1f,.2f,.3f,.4f},  {.5f,.6f,.7f,.8f},  {.6f,.8f,1,1.2f}
    };
    for (int i = 0; i < static_cast<int>(std::size(f)); i += 3) {
        XMVECTOR v1 = f[i];
        XMVECTOR v2 = f[i + 1];
        XMVECTOR check = f[i + 2];
        XMVECTOR r = XMVectorAdd(v1, v2);
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        XMVECTOR r2 = v1 + v2;
        XMVECTOR r3 = v1;
        r3 += v2;
#endif
        COMPARISON temp = CompareXMVECTOR(r, check, 4);
        printi("%s: %d\n", TestName, temp);
        c = std::max(c, temp);
        if (temp > WITHINEPSILON) {
            printe("%f %f %f %f + %f %f %f %f = %f %f %f %f... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        temp = CompareXMVECTOR(r2, check, 4);
        printi("%s: (+) %d\n", TestName, temp);
        c = std::max(c, temp);
        if (temp > WITHINEPSILON) {
            printe("%f %f %f %f + %f %f %f %f = %f %f %f %f... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r2), XMVectorGetY(r2), XMVectorGetZ(r2), XMVectorGetW(r2),
                XMVectorGetZ(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        temp = CompareXMVECTOR(r3, check, 4);
        printi("%s: (+=) %d\n", TestName, temp);
        c = std::max(c, temp);
        if (temp > WITHINEPSILON) {
            printe("%f %f %f %f + %f %f %f %f = %f %f %f %f... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r3), XMVectorGetY(r3), XMVectorGetZ(r3), XMVectorGetW(r3),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
#endif
    }
    return ret;
}

HRESULT Test296(LogProxy* pLog)
{
    //XMVectorSum
    HRESULT ret = S_OK;
    static const XMVECTORF32 f[] = {
        { 0,0,0,0 },{ 0,0,0,0 },
        { .125,.25,-.75,1000 },{ 999.625f,999.625f,999.625f,999.625f },
        { -.3125,0,90,-2 },{ 87.6875f,87.6875f,87.6875f,87.6875f },
        { -.1875,.25,89.25,998 },{ 1087.3125f, 1087.3125f, 1087.3125f, 1087.3125 },
        {.1f,.2f,.3f,.4f },{ 1.f,1.f,1.f,1.f },
        { .5f,.6f,.7f,.8f },{ 2.6f,2.6f,2.6f,2.6f },
        { .6f,.8f,1,1.2f },{ 3.6f,3.6f,3.6f,3.6f }
    };

    for (int i = 0; i < static_cast<int>(std::size(f)); i += 2) {
        XMVECTOR v = f[i];
        XMVECTOR check = f[i + 1];
        XMVECTOR r = XMVectorSum(v);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN2EPSILON)
        {
            printe("%s: %f + %f + %f + %f = %f %f %f %f... %f %f %f %f\n",
                TestName,
                XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test282(LogProxy* pLog)
{
    //XMVectorAddAngles
    const XMVECTORF32 v[] = {
        {{0,0,0,0}},{{0,0,0,0}}, {{0,0,0,0}},
         {{0,0,0,0}},{{-Pi + .1f,Pi - .1f,Pi / 2.f,.1f}},{{-Pi + .1f,Pi - .1f,Pi / 2.f,.1f}},
         {{0,0,0,0}},{{-Pi + .1f,Pi - .1f,-Pi / 2.f,-.1f}},{{-Pi + .1f,Pi - .1f,-Pi / 2.f,-.1f}},
        {{-.5f,-1,-2,+1}},{{0,0,0,-Pi + 2}},{{-.5f, -1, -2, -Pi + 3}},
        {{Pi + .6f, -2 * Pi + 1, 2 * Pi - 2, -Pi - 3}},{{Pi - .1f,0,-2 * Pi + 4,-Pi + 2}},{{.5f,1,2,-1}},
    };
    COMPARISON c;
    HRESULT ret = S_OK;
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    for (int k = 0; k < countof(v); k += 3) {
        v1 = v[k]; v2 = v[k + 1]; check = v[k + 2];
        XMVECTOR r = XMVectorAddAngles(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test283(LogProxy* pLog)
{
    //XMVectorASin
    HRESULT ret = S_OK;
    COMPARISON c;

    XMVECTORF32 v1 = { {-1, 0, 0, 1} };
    XMVECTOR r;

    r = XMVectorASin(v1);
    XMVECTORF32 check = { {-Pi / 2, 0,0,Pi / 2.f} };
    c = CompareXMVECTOR(r, check, 4);
    if (c > WITHIN10EPSILON) {
        printe("%s: %.0f, %.0f, %.0f %.0f: %f %f %f %f, MUST BE %f %f %f %f. %d\n",
            TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }

    XMVECTORF32 v = { {-1, -.5, 0, .5} };
    XMVECTOR adder = XMVectorReplicate(.03125);
    for (float vw = XMVectorGetW(v); ; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); float vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); vw = XMVectorGetW(v);
        if (vw > 1.0f)
            break;
        r = XMVectorASin(v);
        XMVECTORF32 check2 = { {asinf(vx),asinf(vy),asinf(vz),asinf(vw)} };
        COMPARISON worstc = EXACT;
        for (int i = 0; i < 4; i++) {
            c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i));
            worstc = std::max(c, worstc);
            if (c > WITHIN10EPSILON) {
                printe("%s: (%f) = %f ... %f   %d\n", TestName, XMVectorGetByIndex(v, i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i), c);
                ret = MATH_FAIL;
            }
        }
        //printi("%s: %f %d\n", TestName, XMVectorGetX(v), worstc);
    }

    // Range for arc-sine is [-1,1]
    for (float f = -1; f <= 1; f += .01f) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorASin(v);
        XMVECTOR check = XMVectorReplicate(asinf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test284(LogProxy* pLog)
{
    //XMVectorASinEst
    HRESULT ret = S_OK;
    COMPARISON c;

    XMVECTORF32 v1 = { {-1, 0, 0, 1} };
    XMVECTOR r;

    r = XMVectorASinEst(v1);
    XMVECTORF32 check = { {-Pi / 2, 0,0,Pi / 2.f} };
    c = CompareXMVECTOR(r, check, 4);
    if (c > WITHINBIGEPSILON) {
        printe("%s: %.0f, %.0f, %.0f %.0f: %f %f %f %f, MUST BE %f %f %f %f. %d\n",
            TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }

    XMVECTORF32 v = { {-1, -.5, 0, .5} };
    XMVECTOR adder = XMVectorReplicate(.03125);
    for (float vw = XMVectorGetW(v); ; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); float vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); vw = XMVectorGetW(v);
        if (vw > 1.0f)
            break;
        r = XMVectorASinEst(v);
        XMVECTORF32 check2 = { {asinf(vx),asinf(vy),asinf(vz),asinf(vw)} };
        COMPARISON worstc = EXACT;
        for (int i = 0; i < 4; i++) {
            c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i));
            worstc = std::max(c, worstc);
            if (c > WITHINBIGGEREPSILON) {
                printe("%s: (%f) = %f ... %f   %d\n", TestName, XMVectorGetByIndex(v, i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i), c);
                ret = MATH_FAIL;
            }
        }
        printi("%s: %f %d\n", TestName, vx, worstc);
    }

    // Range for arc-sine is [-1,1]
    for (float f = -1; f <= 1; f += .01f) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorASinEst(v);
        XMVECTOR check = XMVectorReplicate(asinf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGGEREPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test285(LogProxy* pLog)
{
    //XMVectorATan
    HRESULT ret = S_OK;
    COMPARISON c;

    /* atn(-10000) must be near -pi/2 . atn(10000) must be near pi/2, atn(0)==0 */
    XMVECTORF32 v1 = { {-10000, 0, 0, 10000} };
    XMVECTOR r;

    r = XMVectorATan(v1);
    XMVECTORF32 check = { {atanf(-10000.f), 0,0,atanf(10000.f)} };
    c = CompareXMVECTOR(r, check, 4);
    if (c > WITHIN4096) {
        printe("%s: %.0f, %.0f, %.0f %.0f: %f %f %f %f, MUST BE %f %f %f %f. %d\n",
            TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }

    XMVECTORF32 v = { {-100, -1, 0, 5} };
    XMVECTORF32 adder = { {5, .05f, .05f, 5} };
    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        r = XMVectorATan(v);
        XMVECTORF32 check2 = { {atanf(vx),atanf(vy),atanf(vz),atanf(vw)} };
        COMPARISON worstc = EXACT;
        for (int i = 0; i < 4; i++) {
            c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i));
            worstc = std::max(c, worstc);
            if (c > WITHINBIGGEREPSILON) {
                printe("%s: (%f) = %f ... %f   %d\n", TestName, XMVectorGetByIndex(v, i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i), c);
                ret = MATH_FAIL;
            }
        }
        printi("%s: %f %d\n", TestName, vx, worstc);
    }

    // Range for arc-tangent is [-INF,+INF]
    for (float f = -10; f <= 10; f += .01f) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorATan(v);
        XMVECTOR check = XMVectorReplicate(atanf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGGEREPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

inline bool SignBit(float f)
{
    // C11 signbit is not available in VS 2010 CRT
    auto ptr = reinterpret_cast<const uint32_t*>(&f);
    return (*ptr & 0x80000000) != 0;
}

HRESULT Test286(LogProxy* pLog)
{
    //XMVectorATan2
    HRESULT ret = S_OK;
    COMPARISON c;

    /* atn(-10000) must be near -pi/2 . atn(10000) must be near pi/2, atn(0)==0 */
    XMVECTORF32 v1 = { {-10000, 1, 1, 10000} };
    XMVECTORF32 v2 = { {0, -10000, 10000, 0} };
    XMVECTOR r;

    r = XMVectorATan2(v1, v2);
    XMVECTORF32 check = { {-Pi / 2.f, atan2f(1.f, -10000.f), atan2f(1.f, 10000.f), Pi / 2.f} };
    c = CompareXMVECTOR(r, check, 4);
    if (c > WITHIN4096) {
        printe("%s: [%.0f %.0f %.0f %.0f] / [%f.0 %f.0 %f.0 %f.0]: %f %f %f %f...%f %f %f %f %d\n",
            TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
            XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }

    XMVECTORF32 v = { {-100, -1, 0, 5} };
    XMVECTORF32 adder = { {5, .05f, .05f, 5} };
    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        v2.v = GetRandomVector16();
        r = XMVectorATan2(v, v2);
        XMVECTORF32 check2 = { {atan2f(vx, XMVectorGetX(v2)),atan2f(vy, XMVectorGetY(v2)),atan2f(vz,XMVectorGetZ(v2)),atan2f(vw,XMVectorGetW(v2))} };
        COMPARISON worstc = EXACT;
        for (int i = 0; i < 4; i++) {
            c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i));
            worstc = std::max(c, worstc);
            if (c > WITHINBIGEPSILON) {
                printe("%s(B): %f/%f = %f ... %f   %d\n", TestName, XMVectorGetByIndex(v, i), XMVectorGetByIndex(v2, i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i), c);
                ret = MATH_FAIL;
            }
        }
        printi("%s: %f %f %f %f: %d\n", TestName, vx, vy, vz, vw, worstc);
    }

#ifndef __clang__
    // Test special cases around 0, -0, +INF, -INF
    {
#if defined(_M_FP_FAST) && defined(_XM_NO_INTRINSICS_)
        static const float yval[] = { 0.f, -0.f, 0.f, -0.f, 0.f, -0.f,  0.f, -0.f, 1.f, -1.f };
        static const float xval[] = { 0.f, -0.f, -0.f, 0.f, 1.f,  1.f, -1.f, -1.f, 0.f,  0.f };
#else
        static const float yval[] = { 0.f, -0.f,  0.f, -0.f, 0.f, -0.f,  0.f, -0.f, 1.f, -1.f, -1.f,   1.f,  -1.f, c_INF, c_INF, -c_INF, -c_INF };
        static const float xval[] = { 0.f, -0.f, -0.f,  0.f, 1.f,  1.f, -1.f, -1.f, 0.f,  0.f, c_INF, -c_INF, -c_INF,  1.f, -1.f,   1.f,  -1.f };
#endif
        for (size_t i = 0; i < std::size(yval); ++i)
        {
            float y = yval[i];  float x = xval[i];
            XMVECTORF32 vx = { {x,x,x,x} };
            XMVECTORF32 vy = { {y,y,y,y} };
            r = XMVectorATan2(vy, vx);
            XMVECTOR check = XMVectorReplicate(atan2f(y, x));
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINBIGEPSILON)
            {
                printe("%s(C): %f/%f = %f %f %f %f...%f %f %f %f %d\n", TestName, y, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
            else if (SignBit(XMVectorGetX(r)) != SignBit(XMVectorGetX(check)))
            {
                printe("%s(C): %f/%f = %f %f %f %f...%f %f %f %f\n", TestName, y, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
                ret = MATH_FAIL;
            }
        }
    }

#ifndef _XM_NO_INTRINSICS_
    // Explicit tests to work around bugs in CRT atan2 (four x86 cases, and one ARM case)
    {
        static const float yval[] = { c_INF,    -c_INF,        c_INF,         -c_INF,          1.f };
        static const float xval[] = { c_INF,    -c_INF,        -c_INF,        c_INF,           c_INF };
        static const float result[] = { Pi / 4.f,  -3.f * Pi / 4.f,  3.f * Pi / 4.f,   -Pi / 4.f,        0.f };

        for (size_t i = 0; i < std::size(yval); ++i)
        {
            float y = yval[i];  float x = xval[i];
            XMVECTORF32 vx = { {x,x,x,x} };
            XMVECTORF32 vy = { {y,y,y,y} };
            r = XMVectorATan2(vy, vx);
            XMVECTOR check = XMVectorReplicate(result[i]);
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINBIGEPSILON)
            {
                printe("%s: %f/%f = %f %f %f %f...%f %f %f %f %d\n", TestName, y, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }
#endif
#endif // !__clang__

    // Range for arc-tangent is [-INF,+INF]
    for (float x = -10; x <= 10; x += .1f) {
        for (float y = -10; y <= 10; y += .1f) {
            XMVECTORF32 vx = { {x,x,x,x} };
            XMVECTORF32 vy = { {y,y,y,y} };
            r = XMVectorATan2(vy, vx);
            XMVECTOR check = XMVectorReplicate(atan2f(y, x));
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINBIGEPSILON)
            {

                printe("%s: %f/%f = %f %f %f %f...%f %f %f %f %d\n", TestName, y, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}
HRESULT Test287(LogProxy* pLog)
{
    //XMVectorATan2Est
    HRESULT ret = S_OK;
    COMPARISON c;

    /* atn(-10000) must be near -pi/2 . atn(10000) must be near pi/2, atn(0)==0 */
    XMVECTORF32 v1 = { {-10000, 1, 1, 10000} };
    XMVECTORF32 v2 = { {0, -10000, 10000, 0} };
    XMVECTOR r;

    r = XMVectorATan2Est(v1, v2);
    XMVECTORF32 check = { {-Pi / 2.f, atan2f(1.f, -10000.f), atan2f(1.f, 10000.f), Pi / 2.f} };
    c = CompareXMVECTOR(r, check, 4);
    if (c > WITHINHUGEEPSILON) {
        printe("%s: [%.0f %.0f %.0f %.0f] / [%f.0 %f.0 %f.0 %f.0]: %f %f %f %f...%f %f %f %f %d\n",
            TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
            XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }

    XMVECTORF32 v = { {-100, -1, 0, 5} };
    XMVECTORF32 adder = { {5, .05f, .05f, 5} };
    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        v2.v = GetRandomVector16();
        r = XMVectorATan2Est(v, v2);
        XMVECTORF32 check2 = { {atan2f(vx, XMVectorGetX(v2)),atan2f(vy, XMVectorGetY(v2)),atan2f(vz,XMVectorGetZ(v2)),atan2f(vw,XMVectorGetW(v2))} };
        COMPARISON worstc = EXACT;
        for (int i = 0; i < 4; i++) {
            c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i));
            worstc = std::max(c, worstc);
            if (c > WITHINHUGEEPSILON) {
                printe("%s(B): %f/%f = %f ... %f   %d\n", TestName, XMVectorGetByIndex(v, i), XMVectorGetByIndex(v2, i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i), c);
                ret = MATH_FAIL;
            }
        }
        printi("%s: %f %f %f %f: %d\n", TestName, vx, vy, vz, vw, worstc);
    }

    // Test special cases around 0, -0, +INF, -INF
    {
#if defined(_M_FP_FAST) && defined(_XM_NO_INTRINSICS_)
        static const float yval[] = { 0.f, -0.f,  0.f, -0.f, 0.f, -0.f,  0.f, -0.f, 1.f, -1.f };
        static const float xval[] = { 0.f, -0.f, -0.f,  0.f, 1.f,  1.f, -1.f, -1.f, 0.f,  0.f };
#else
        static const float yval[] = { 0.f, -0.f,  0.f, -0.f, 0.f, -0.f,  0.f, -0.f, 1.f, -1.f, -1.f,   1.f,  -1.f, c_INF, c_INF, -c_INF, -c_INF };
        static const float xval[] = { 0.f, -0.f, -0.f,  0.f, 1.f,  1.f, -1.f, -1.f, 0.f,  0.f, c_INF, -c_INF, -c_INF,  1.f, -1.f,   1.f,  -1.f };
#endif
        for (size_t i = 0; i < std::size(yval); ++i)
        {
            float y = yval[i];  float x = xval[i];
            XMVECTORF32 vx = { {x,x,x,x} };
            XMVECTORF32 vy = { {y,y,y,y} };
            r = XMVectorATan2Est(vy, vx);
            XMVECTOR check = XMVectorReplicate(atan2f(y, x));
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINHUGEEPSILON)
            {
                printe("%s(C): %f/%f = %f %f %f %f...%f %f %f %f %d\n", TestName, y, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
            else if (SignBit(XMVectorGetX(r)) != SignBit(XMVectorGetX(check)))
            {
                printe("%s(C): %f/%f = %f %f %f %f...%f %f %f %f\n", TestName, y, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
                ret = MATH_FAIL;
            }
        }
    }

#ifndef _XM_NO_INTRINSICS_
    // Explicit tests to work around bugs in CRT atan2 (four x86 cases, and one ARM case)
    {
        static const float yval[] = { c_INF,    -c_INF,        c_INF,         -c_INF,          1.f };
        static const float xval[] = { c_INF,    -c_INF,        -c_INF,        c_INF,           c_INF };
        static const float result[] = { Pi / 4.f,  -3.f * Pi / 4.f,  3.f * Pi / 4.f,   -Pi / 4.f,        0.f };

        for (size_t i = 0; i < std::size(yval); ++i)
        {
            float y = yval[i];  float x = xval[i];
            XMVECTORF32 vx = { {x,x,x,x} };
            XMVECTORF32 vy = { {y,y,y,y} };
            r = XMVectorATan2Est(vy, vx);
            XMVECTOR check = XMVectorReplicate(result[i]);
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINHUGEEPSILON)
            {
                printe("%s: %f/%f = %f %f %f %f...%f %f %f %f %d\n", TestName, y, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }
#endif

    // Range for arc-tangent is [-INF,+INF]
    for (float x = -10; x <= 10; x += .1f) {
        for (float y = -10; y <= 10; y += .1f) {
            XMVECTORF32 vx = { {x,x,x,x} };
            XMVECTORF32 vy = { {y,y,y,y} };
            r = XMVectorATan2Est(vy, vx);
            XMVECTOR check = XMVectorReplicate(atan2f(y, x));
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINHUGEEPSILON)
            {

                printe("%s: %f/%f = %f %f %f %f...%f %f %f %f %d\n", TestName, y, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}

HRESULT Test288(LogProxy* pLog)
{
    //XMVectorATanEst
    HRESULT ret = S_OK;
    COMPARISON c;

    /* atn(-10000) must be near -pi/2 . atn(10000) must be near pi/2, atn(0)==0 */
    XMVECTORF32 v1 = { {-10000, 0, 0, 10000} };
    XMVECTOR r;

    r = XMVectorATanEst(v1);
    XMVECTORF32 check = { {-atanf(10000.f), 0,0,atanf(10000.f)} };
    c = CompareXMVECTOR(r, check, 4);
    if (c > WITHIN4096) {
        printe("%s: %.0f, %.0f, %.0f %.0f: %f %f %f %f, MUST BE %f %f %f %f. %d\n",
            TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }

    XMVECTORF32 v = { {-100, -1, 0, 5} };
    XMVECTORF32 adder = { {5, .05f, .05f, 5} };
    //	XMVECTORF32 v = {{0, .25, 1, 5}};
    //	XMVECTOR adder = {{.03125, .09375, .125, 2}};
    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        r = XMVectorATanEst(v);
        XMVECTORF32 check2 = { {atanf(vx),atanf(vy),atanf(vz),atanf(vw)} };
        COMPARISON worstc = EXACT;
        for (int i = 0; i < 4; i++) {
            c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i));
            worstc = std::max(c, worstc);
            if (c > WITHINHUGEEPSILON) {
                printe("%s: (%f) = %f ... %f   %d\n", TestName, XMVectorGetByIndex(v, i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(check2, i), c);
                ret = MATH_FAIL;
            }
        }
        printi("%s: %d\n", TestName, worstc); //%f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v), XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r), XMVectorGetX(check2),XMVectorGetY(check2),XMVectorGetZ(check2),XMVectorGetW(check2),worstc);
    }

    // Range for arc-tangent is [-INF,+INF]
    for (float f = -10; f <= 10; f += .01f) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorATanEst(v);
        XMVECTOR check = XMVectorReplicate(atanf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINHUGEEPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test289(LogProxy* pLog)
{
    //XMVectorBaryCentric and XMVectorBaryCentricV
    XMVECTORF32 q1 = { {1,0,0,0} };
    XMVECTORF32 q2 = { {0,1,0,0} };
    XMVECTORF32 q3 = { {0,0,1,0} };
    float fv[] = { 0,1,0,1 };
    float gv[] = { 0,0,1,-1 };
    XMVECTORF32 chk[] = { q1,q2,q3,{{1,1,-1,0}} };
    XMVECTOR r;
    COMPARISON c;
    HRESULT ret = S_OK;
    float f, g;
    XMVECTOR check;

    int k;
    //XMVectorBaryCentric
    for (k = 0; k < countof(fv); k++) {
        r = XMVectorBaryCentric(q1, q2, q3, fv[k], gv[k]);
        c = CompareXMVECTOR(r, chk[k], 4);
        if (c > WITHIN100EPSILON) {
            printe("%s: f:%f g:%f (%d)\n", TestName, fv[k], gv[k], c);
            dqe(q1); dqe(q2); dqe(q3); dqe(r); dqe(chk[k]);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    for (k = 0; k < 10; k++) {
        for (int i = 0; i < 4; i++) {
            q1.v = XMVectorSetByIndex(q1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q2.v = XMVectorSetByIndex(q2, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q3.v = XMVectorSetByIndex(q3, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        f = ((float)XM_RAND()) / 2000.f - 8.f;
        g = ((float)XM_RAND()) / 2000.f - 8.f;
        r = XMVectorBaryCentric(q1, q2, q3, f, g);
        // q1+f*(q2-q1)+g*(q3-q1)
        check = XMVectorAdd(
            q1,
            XMVectorAdd(
                XMVectorScale(XMVectorSubtract(q2, q1), f),
                XMVectorScale(XMVectorSubtract(q3, q1), g)));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: f:%f g:%f (%d)\n", TestName, f, g, c);
            dqe(q1); dqe(q2); dqe(q3); dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    //XMVectorBaryCentricV
    XMVECTORF32 F = { {1,1,1,1} };
    XMVECTORF32 G = { {1,1,1,1} };
    for (k = 0; k < 10; k++) {
        for (int i = 0; i < 4; i++) {
            q1.v = XMVectorSetByIndex(q1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q2.v = XMVectorSetByIndex(q2, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q3.v = XMVectorSetByIndex(q3, ((float)XM_RAND()) / 2000.f - 8.f, i);
            F.v = XMVectorSetByIndex(F, ((float)XM_RAND()) / 2000.f - 8.f, i);
            G.v = XMVectorSetByIndex(G, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        r = XMVectorBaryCentricV(q1, q2, q3, F, G);
        // q1+F*(q2-q1.v)+G*(q3-q1.v);
        check = XMVectorAdd(q1,
            XMVectorAdd(
                XMVectorMultiply(F, XMVectorSubtract(q2, q1)),
                XMVectorMultiply(G, XMVectorSubtract(q3, q1))));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: (%d)\n", TestName, c);
            dqe(F); dqe(G);
            dqe(q1); dqe(q2); dqe(q3); dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test290(LogProxy* pLog)
{
    //XMVectorCatmullRom
    HRESULT ret = S_OK;
    int k = 0;
    do {
        // Generate 4 random vectors
        XMVECTORF32 qarray[4];
        int i = 0;
        do {
            int j = 0;
            do {
                qarray[i].f[j] = ((float)XM_RAND()) / 2000.f - 8.f;
            } while (++j < 4);
        } while (++i < 4);
        // Load them up!
        XMVECTOR q1 = qarray[0];
        XMVECTOR q2 = qarray[1];
        XMVECTOR q3 = qarray[2];
        XMVECTOR q4 = qarray[3];
        float f = ((float)XM_RAND()) / 2000.f - 8.f;
        XMVECTOR r = XMVectorCatmullRom(q1, q2, q3, q4, f);
        XMVECTORF32 xcheck = {
            0.5f * ((-f * f * f + 2 * f * f - f) * qarray[0].f[0] + (3 * f * f * f - 5 * f * f + 2) * qarray[1].f[0] + (-3 * f * f * f + 4 * f * f + f) * qarray[2].f[0] + (f * f * f - f * f) * qarray[3].f[0]),
            0.5f * ((-f * f * f + 2 * f * f - f) * qarray[0].f[1] + (3 * f * f * f - 5 * f * f + 2) * qarray[1].f[1] + (-3 * f * f * f + 4 * f * f + f) * qarray[2].f[1] + (f * f * f - f * f) * qarray[3].f[1]),
            0.5f * ((-f * f * f + 2 * f * f - f) * qarray[0].f[2] + (3 * f * f * f - 5 * f * f + 2) * qarray[1].f[2] + (-3 * f * f * f + 4 * f * f + f) * qarray[2].f[2] + (f * f * f - f * f) * qarray[3].f[2]),
            0.5f * ((-f * f * f + 2 * f * f - f) * qarray[0].f[3] + (3 * f * f * f - 5 * f * f + 2) * qarray[1].f[3] + (-3 * f * f * f + 4 * f * f + f) * qarray[2].f[3] + (f * f * f - f * f) * qarray[3].f[3])
        };
        XMVECTOR check = xcheck;
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: f:%f (%d)\n", TestName, f, c);
            dqe(q1); dqe(q2); dqe(q3); dqe(q4); dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    } while (++k < 10);
    return ret;
}

HRESULT Test291(LogProxy* pLog)
{
    //XMVectorCeiling
    HRESULT ret = S_OK;

    const XMVECTORF32 f[] = {
        {{0,0,0,0}},  {{0,0,0,0}},
        {{-2, -1.99f, -1.50f, -1.01f}}, {{-2,-1,-1,-1}},
        {{-1, -0.99f, -0.50f, -0.01f}}, {{-1,-0.f,-0.f,-0.f}},
        {{ 0 ,  .99f,   .50f,   .01f}}, {{ 0, 1, 1, 1}},
        {{40 ,40.99f, 40.50f, 40.01f}}, {{40,41,41,41}},
        {8388608.f, 8388609.f, 8388607.f, 8388607.5f }, {8388608.f, 8388609.f, 8388607.f, 8388608.f },
        {-8388608.f, -8388609.f, -8388607.f, -8388607.5f }, {-8388608.f, -8388609.f, -8388607.f, -8388607.f },
        {11387829.f, 11387829.f, 11387829.f, 11387829.f}, {11387829.f, 11387829.f, 11387829.f, 11387829.f},
        {-8419647.f, -8419647.f, -8419647.f, -8419647.f}, {-8419647.f, -8419647.f, -8419647.f, -8419647.f},
        {2147483520.f, 2147483520.f, 2147483520.f, 2147483520.f }, {2147483520.f, 2147483520.f, 2147483520.f, 2147483520.f },
        {-2147483520.f, -2147483520.f, -2147483520.f, -2147483520.f }, {-2147483520.f, -2147483520.f, -2147483520.f, -2147483520.f },
        {{8000000000.0f, 8000000000.0f, 8000000000.0f, 8000000000.0f }}, {{8000000000.0f, 8000000000.0f, 8000000000.0f, 8000000000.0f }},
        {   c_INF,   c_INF,   c_INF,   c_INF}, {   c_INF,   c_INF,   c_INF,   c_INF},
        {  -c_INF,  -c_INF,  -c_INF,  -c_INF}, {  -c_INF,  -c_INF,  -c_INF,  -c_INF},
        { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN}, { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN},
    };

    for (size_t i = 0; i < std::size(f); i += 2) {
        XMVECTOR v1 = f[i];
        XMVECTOR check = f[i + 1];
        XMVECTOR r = XMVectorCeiling(v1);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f : %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }

    for (int j = -256; j <= 256; ++j)
    {
        float num = static_cast<float>(j);
        XMVECTORF32 v1 = { num, num + 0.1f, num + 0.5f, num + 0.9f };
        XMVECTORF32 chk = { ceilf(num), ceilf(num + 0.1f), ceilf(num + 0.5f), ceilf(num + 0.9f) };
        XMVECTOR r = XMVectorCeiling(v1);
        COMPARISON c = CompareXMVECTOR(r, chk, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
            ret = MATH_FAIL;
        }
    }

    for (float num = -1.f; num <= 1.f; num += 0.01f)
    {
        XMVECTORF32 v1 = { num, num + 0.1f, num + 0.5f, num + 0.9f };
        XMVECTORF32 chk = { ceilf(num), ceilf(num + 0.1f), ceilf(num + 0.5f), ceilf(num + 0.9f) };
        XMVECTOR r = XMVectorCeiling(v1);
        COMPARISON c = CompareXMVECTOR(r, chk, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test292(LogProxy* pLog)
{
    //XMVectorClamp
    HRESULT ret = S_OK;

    static const XMVECTORF32 v[] = {
        {{ 1,2,3,4}}, {{ 0,1,2,3}}, {{2,3,4,5}}, {{1,2,3,4}},
        {{ 1,2,3,4}}, {{ 2,3,4,5}}, {{2,3,4,5}}, {{2,3,4,5}},
        {{ 1,2,3,4}}, {{ 2,3,4,5}}, {{5,6,7,8}}, {{2,3,4,5}},
        {{ 1,2,3,4}}, {{-1,0,1,2}}, {{0,1,2,3}}, {{0,1,2,3}},
        {{-1,-2,-3,-4}}, {{-2,-3,-4,-5}}, {{-0.f,-1,-2,-3}}, {{-1,-2,-3,-4}},
        {{-1,-2,-3,-4}}, {{-2,-3,-4,-5}}, {{-2,-3,-4,-5}}, {{-2,-3,-4,-5}},
        {{-1,-2,-3,-4}}, {{-5,-6,-7,-8}}, {{-2,-3,-4,-5}}, {{-2,-3,-4,-5}},
        {{-1,-2,-3,-4}}, {{-0.f,-1,-2,-3}}, {{ 1,-0.f,-1,-2}}, {{-0.f,-1,-2,-3}},
        {{10,30,-.25, 8}}, {{12, 20, -1, 7}}, {{13.5, 21.5, 1, 8}},{{12,21.5,-.25,8}},
    };
    XMVECTOR r;
    for (int k = 0; k < countof(v); k += 4) {
        XMVECTOR v1 = v[k], v2 = v[k + 1], v3 = v[k + 2], check = v[k + 3];
        r = XMVectorClamp(v[k], v[k + 1], v[k + 2]);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), XMVectorGetW(v3),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

#if !defined(_M_FP_FAST) || !defined(_XM_NO_INTRINSICS_)
    {
        static const XMVECTORF32 vnan = { { { c_Q_NAN, c_NAN, c_Q_NAN, c_NAN } } };
        static const XMVECTORF32 check = { { { c_NAN, c_NAN, c_NAN, c_NAN } } };
        XMVECTOR vmin = g_XMZero;
        XMVECTOR vmax = g_XMOne;
        r = XMVectorClamp(vnan, vmin, vmax);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > EXACT)
        {
            printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                XMVectorGetX(vnan), XMVectorGetY(vnan), XMVectorGetZ(vnan), XMVectorGetW(vnan),
                XMVectorGetX(vmin), XMVectorGetY(vmin), XMVectorGetZ(vmin), XMVectorGetW(vmin),
                XMVectorGetX(vmax), XMVectorGetY(vmax), XMVectorGetZ(vmax), XMVectorGetW(vmax),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);

            ret = MATH_FAIL;
        }
    }
#endif

    return ret;
}
HRESULT Test293(LogProxy* pLog)
{
    //XMVectorCos
    XMVECTORF32 v = { {-Pi / 2.f - 20, -Pi, 0, Pi / 2.f + 20} };
    XMVECTORF32 adder = { {1, .05f, .05f, 1} };
    XMVECTOR r;
    COMPARISON c;
    HRESULT ret = S_OK;
    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        r = XMVectorCos(v);
        XMVECTORF32 check = { {cosf(vx),cosf(vy),cosf(vz),cosf(vw)} };
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096 || (fabs(XMVectorGetX(r)) > 1) || (fabs(XMVectorGetY(r)) > 1) || (fabs(XMVectorGetZ(r)) > 1) || (fabs(XMVectorGetW(r)) > 1)) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    for (float f = UC_START; f <= UC_END; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorCos(v);
        XMVECTOR check = XMVectorReplicate(cosf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test294(LogProxy* pLog)
{
    //XMVectorCosEst
    //	XMVECTORF32 v = {{-Pi/2.f - 20, -Pi/2.f, 0, Pi/2.f + 20}};
    //	XMVECTOR adder = {{1, .05f, .05f, 1}};
    XMVECTORF32 v = { {-Pi, -Pi / 2.f, 0, Pi / 2.f} };
    XMVECTORF32 adder = { {.05f, .05f, .05f, .05f} };
    XMVECTOR r;
    COMPARISON c;
    HRESULT ret = S_OK;

    const COMPARISON threshold = WITHIN4096;

    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        r = XMVectorCosEst(v);
        XMVECTORF32 check = { {cosf(vx),cosf(vy),cosf(vz),cosf(vw)} };
        c = CompareXMVECTOR(r, check, 4);
        if (c > threshold || (fabs(XMVectorGetX(r)) > 1) || (fabs(XMVectorGetY(r)) > 1) || (fabs(XMVectorGetZ(r)) > 1) || (fabs(XMVectorGetW(r)) > 1)) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    for (float f = -Pi; f <= Pi; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorCosEst(v);
        XMVECTOR check = XMVectorReplicate(cosf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > threshold)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}


HRESULT Test295(LogProxy* pLog)
{
    //XMVectorCosH
    HRESULT ret = S_OK;
    XMVECTORF32 v = { {-120, -4, 0, 4} };
    XMVECTORF32 adder = { {116.f / 26.f, 4.f / 26.f, 4.f / 26.f, 116.f / 26.f} };
    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder)) {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        XMVECTORF32 check = { {coshf(vx),coshf(vy),coshf(vz),coshf(vw)} };
        XMVECTOR r = XMVectorCosH(v);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %.2f: %f ... %f    %.2f: %f ... %f\n%.2f: %f ... %f   %.2f: %f ... %f (%d)\n",
                TestName, vx, XMVectorGetX(r), XMVectorGetX(check), vy, XMVectorGetY(r), XMVectorGetY(check), vz, XMVectorGetZ(r), XMVectorGetZ(check), vw, XMVectorGetW(r), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    // Range for hyperbolic cosine is [-INF,+INF]
    for (float f = -4 * Pi; f <= 4 * Pi; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        XMVECTOR r = XMVectorCosH(v);
        XMVECTOR check = XMVectorReplicate(coshf(f));
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test297(LogProxy* pLog)
{
    //XMVectorEqual
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check.v = XMVectorSetIntX(check, 0); }
        else { check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check.v = XMVectorSetIntY(check, 0); }
        else { check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); check.v = XMVectorSetIntZ(check, 0); }
        else { check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); check.v = XMVectorSetIntW(check, 0); }
        else { check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        XMVECTOR r = XMVectorEqual(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check.v = XMVectorSetIntX(check, 0); }
        else { check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check.v = XMVectorSetIntY(check, 0); }
        else { check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); check.v = XMVectorSetIntZ(check, 0); }
        else { check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); check.v = XMVectorSetIntW(check, 0); }
        else { check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        r = XMVectorEqual(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test298(LogProxy* pLog)
{
    //XMVectorExp2
    HRESULT ret = S_OK;
    {
        XMVECTORF32 v = { {-150, -4, 0, 4} };
        XMVECTORF32 adder = { {146.f / 26.f, 4.f / 26.f, 4.f / 26.f, 146.f / 26.f} };
        for (float vy = XMVectorGetY(v); vy <= 0.01f; v.v = XMVectorAdd(v.v, adder)) {
            float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
            XMVECTORF32 check = { {exp2f(vx),exp2f(vy),exp2f(vz),exp2f(vw)} };
            XMVECTOR r = XMVectorExp2(v);
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > WITHIN4096) {
                printe("%s 2: %.2f: %f ... %f    %.2f: %f ... %f\n%.2f: %f ... %f   %.2f: %f ... %f\n",
                    TestName, vx, XMVectorGetX(r), XMVectorGetX(check), vy, XMVectorGetY(r), XMVectorGetY(check), vz, XMVectorGetZ(r), XMVectorGetZ(check), vw, XMVectorGetW(r), XMVectorGetW(check));
                ret = MATH_FAIL;
            }
            else {
                printi("%s 2: %d\n", TestName, c);
            }

            // Check XMVectorExp alias
            r = XMVectorExp(v);
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHIN4096) {
                printe("%s: %.2f: %f ... %f    %.2f: %f ... %f\n%.2f: %f ... %f   %.2f: %f ... %f\n",
                    TestName, vx, XMVectorGetX(r), XMVectorGetX(check), vy, XMVectorGetY(r), XMVectorGetY(check), vz, XMVectorGetZ(r), XMVectorGetZ(check), vw, XMVectorGetW(r), XMVectorGetW(check));
                ret = MATH_FAIL;
            }
            else {
                printi("%s: %d\n", TestName, c);
            }
        }
    }

    //XMVectorExp10
    {
        XMVECTORF32 v = { {-150, -4, 0, 4} };
        XMVECTORF32 adder = { {146.f / 26.f, 4.f / 26.f, 4.f / 26.f, 146.f / 26.f} };
        for (float vy = XMVectorGetY(v); vy <= 0.01f; v.v = XMVectorAdd(v.v, adder)) {
            float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
            XMVECTORF32 check = { {powf(10.f, vx),powf(10.f, vy),powf(10.f, vz),powf(10.f, vw)} };
            XMVECTOR r = XMVectorExp10(v);
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > WITHIN4096) {
                printe("%s 10: %.2f: %f ... %f    %.2f: %f ... %f\n%.2f: %f ... %f   %.2f: %f ... %f\n",
                    TestName, vx, XMVectorGetX(r), XMVectorGetX(check), vy, XMVectorGetY(r), XMVectorGetY(check), vz, XMVectorGetZ(r), XMVectorGetZ(check), vw, XMVectorGetW(r), XMVectorGetW(check));
                ret = MATH_FAIL;
            }
            else {
                printi("%s E: %d\n", TestName, c);
            }
        }
    }

    //XMVectorExpE
    {
        XMVECTORF32 v = { {-150, -4, 0, 4} };
        XMVECTORF32 adder = { {146.f / 26.f, 4.f / 26.f, 4.f / 26.f, 146.f / 26.f} };
        for (float vy = XMVectorGetY(v); vy <= 0.01f; v.v = XMVectorAdd(v.v, adder)) {
            float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
            XMVECTORF32 check = { {expf(vx),expf(vy),expf(vz),expf(vw)} };
            XMVECTOR r = XMVectorExpE(v);
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > WITHIN4096) {
                printe("%s E: %.2f: %f ... %f    %.2f: %f ... %f\n%.2f: %f ... %f   %.2f: %f ... %f\n",
                    TestName, vx, XMVectorGetX(r), XMVectorGetX(check), vy, XMVectorGetY(r), XMVectorGetY(check), vz, XMVectorGetZ(r), XMVectorGetZ(check), vw, XMVectorGetW(r), XMVectorGetW(check));
                ret = MATH_FAIL;
            }
            else {
                printi("%s E: %d\n", TestName, c);
            }
        }
    }

    return ret;
}


HRESULT Test300(LogProxy* pLog)
{
    //XMVectorFloor
    HRESULT ret = S_OK;

    const XMVECTORF32 f[] = {
        {{0,0,0,0}},  {{0,0,0,0}},
        {{-2, -1.99f, -1.50f, -1.01f}}, {{-2,-2,-2,-2}},
        {{-1, -0.99f, -0.50f, -0.01f}}, {{-1,-1,-1,-1}},
        {{ 0 ,  .99f,   .50f,   .01f}}, {{ 0, 0, 0, 0}},
        {{40 ,40.99f, 40.50f, 40.01f}}, {{40,40,40,40}},
        {8388608.f, 8388609.f, 8388607.f, 8388607.5f }, {8388608.f, 8388609.f, 8388607.f, 8388607.f },
        {-8388608.f, -8388609.f, -8388607.f, -8388607.5f }, {-8388608.f, -8388609.f, -8388607.f, -8388608.f },
        {11387829.f, 11387829.f, 11387829.f, 11387829.f}, {11387829.f, 11387829.f, 11387829.f, 11387829.f},
        {-8419647.f, -8419647.f, -8419647.f, -8419647.f}, {-8419647.f, -8419647.f, -8419647.f, -8419647.f},
        {2147483520.f, 2147483520.f, 2147483520.f, 2147483520.f }, {2147483520.f, 2147483520.f, 2147483520.f, 2147483520.f },
        {-2147483520.f, -2147483520.f, -2147483520.f, -2147483520.f }, {-2147483520.f, -2147483520.f, -2147483520.f, -2147483520.f },
        {{8000000000.0f, 8000000000.0f, 8000000000.0f, 8000000000.0f }}, {{8000000000.0f, 8000000000.0f, 8000000000.0f, 8000000000.0f }},
        {   c_INF,   c_INF,   c_INF,   c_INF}, {   c_INF,   c_INF,   c_INF,   c_INF},
        {  -c_INF,  -c_INF,  -c_INF,  -c_INF}, {  -c_INF,  -c_INF,  -c_INF,  -c_INF},
        { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN}, { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN},
    };

    for (size_t i = 0; i < std::size(f); i += 2) {
        XMVECTOR v1 = f[i];
        XMVECTOR check = f[i + 1];
        XMVECTOR r = XMVectorFloor(v1);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f : %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }

    for (int j = -256; j <= 256; ++j)
    {
        float num = static_cast<float>(j);
        XMVECTORF32 v1 = { num, num + 0.1f, num + 0.5f, num + 0.9f };
        XMVECTORF32 chk = { floorf(num), floorf(num + 0.1f), floorf(num + 0.5f), floorf(num + 0.9f) };
        XMVECTOR r = XMVectorFloor(v1);
        COMPARISON c = CompareXMVECTOR(r, chk, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
            ret = MATH_FAIL;
        }
    }

    for (float num = -1.f; num <= 1.f; num += 0.01f)
    {
        XMVECTORF32 v1 = { num, num + 0.1f, num + 0.5f, num + 0.9f };
        XMVECTORF32 chk = { floorf(num), floorf(num + 0.1f), floorf(num + 0.5f), floorf(num + 0.9f) };
        XMVECTOR r = XMVectorFloor(v1);
        COMPARISON c = CompareXMVECTOR(r, chk, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test301(LogProxy* pLog)
{
    //XMVectorGreater
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntX(check, 0); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntY(check, 0); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntZ(check, 0); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntW(check, 0); }
        XMVECTOR r = XMVectorGreater(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check.v = XMVectorSetIntX(check, 0); }
        else { check.v = XMVectorSetIntX(check, 0); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check.v = XMVectorSetIntY(check, 0); }
        else { check.v = XMVectorSetIntY(check, 0); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); check.v = XMVectorSetIntZ(check, 0); }
        else { check.v = XMVectorSetIntZ(check, 0); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); check.v = XMVectorSetIntW(check, 0); }
        else { check.v = XMVectorSetIntW(check, 0); }
        r = XMVectorGreater(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test302(LogProxy* pLog)
{
    //XMVectorGreaterOrEqual
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        XMVECTOR r = XMVectorGreaterOrEqual(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check.v = XMVectorSetIntX(check, 0); }
        else { check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check.v = XMVectorSetIntY(check, 0); }
        else { check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); check.v = XMVectorSetIntZ(check, 0); }
        else { check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); check.v = XMVectorSetIntW(check, 0); }
        else { check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        r = XMVectorGreaterOrEqual(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }

    }
    return ret;
}
HRESULT Test303(LogProxy* pLog)
{
    //XMVectorHermite
    XMVECTORF32 q1 = { {1,0,0,0} };
    XMVECTORF32 q2 = { {0,1,0,0} };
    XMVECTORF32 q3 = { {0,0,1,0} };
    XMVECTORF32 q4 = { {0,0,1,0} };
    XMVECTOR r;
    COMPARISON c;
    HRESULT ret = S_OK;
    float f;
    XMVECTOR check;
    int k;
    /*
    float fv[] = {0,1,0,1};
    XMVECTORF32 chk[] = {q1,q2,q3,{{1,1,-1,0}}};
    for(k = 0; k < countof(fv); k++) {
        r = XMVectorBaryCentric(q1, q2, q3, fv[k], gv[k]);
        c = CompareXMVECTOR(r,chk[k],4);
        if(c > WITHIN100EPSILON) {
            printe("%s: f:%f g:%f (%d)\n", TestName, fv[k], gv[k], c);
            dqe(q1);dqe(q2);dqe(q3);dqe(r);dqe(chk[k]);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    */

    for (k = 0; k < 10; k++) {
        for (int i = 0; i < 4; i++) {
            q1.v = XMVectorSetByIndex(q1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q2.v = XMVectorSetByIndex(q2, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q3.v = XMVectorSetByIndex(q3, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q4.v = XMVectorSetByIndex(q4, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        f = ((float)XM_RAND()) / 2000.f - 8.f;
        r = XMVectorHermite(q1, q2, q3, q4, f);
        // (2*f*f*f-3*f*f+1)*q1+(f*f*f-2*f*f+f)*q2+(-2*f*f*f+3*f*f)*q3+(f*f*f-f*f)*q4;
        check = XMVectorAdd(
            XMVectorScale(q1, 2 * f * f * f - 3 * f * f + 1),
            XMVectorAdd(
                XMVectorScale(q2, f * f * f - 2 * f * f + f),
                XMVectorAdd(
                    XMVectorScale(q3, -2 * f * f * f + 3 * f * f),
                    XMVectorScale(q4, f * f * f - f * f))));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: f:%f (%d)\n", TestName, f, c);
            dqe(q1); dqe(q2); dqe(q3); dqe(q4); dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test304(LogProxy* pLog)
{
    //XMVectorIsInfinite
    HRESULT ret = S_OK;
    static const float f[] = { c_INF, -c_INF, c_Q_NAN, 1,-1 };
    int r, g, b, a;
    XMVECTOR col;
    XMVECTOR res, check;
    for (r = 0; r < countof(f); r++) {
        for (g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for (b = 0; b < countof(f); b++) {
                for (a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r], f[g], f[b], f[a]);
                    check = XMVectorSetInt(((r < 2) ? 0xffffffff : 0),
                        ((g < 2) ? 0xffffffff : 0),
                        ((b < 2) ? 0xffffffff : 0),
                        ((a < 2) ? 0xffffffff : 0));
                    res = XMVectorIsInfinite(col);
                    for (int i = 0; i < 4; i++) {
                        if (XMVectorGetIntByIndex(res, i) != XMVectorGetIntByIndex(check, i)) {
                            printe("\n%s: %f %f %f %f = %x %x %x %x ... %x %x %x %x\n",
                                TestName,
                                XMVectorGetX(col), XMVectorGetY(col), XMVectorGetZ(col), XMVectorGetW(col),
                                XMVectorGetIntX(res), XMVectorGetIntY(res), XMVectorGetIntZ(res), XMVectorGetIntW(res),
                                XMVectorGetIntX(check), XMVectorGetIntY(check), XMVectorGetIntZ(check), XMVectorGetIntW(check));
                            ret = MATH_FAIL;
                        }
                    }
                }
            }
        }
    }
    printi("\n");
    return ret;
}

ISNAN_TEST_BEGIN
HRESULT Test305(LogProxy* pLog)
{
    //XMVectorIsNaN
    HRESULT ret = S_OK;
    static const float f[] = { c_Q_NAN, c_NAN, c_INF, 1,-1 };
    int r, g, b, a;
    XMVECTOR col;
    XMVECTOR res, check;
    for (r = 0; r < countof(f); r++) {
        for (g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for (b = 0; b < countof(f); b++) {
                for (a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r], f[g], f[b], f[a]);
                    check = XMVectorSetInt(((r < 2) ? 0xffffffff : 0),
                        ((g < 2) ? 0xffffffff : 0),
                        ((b < 2) ? 0xffffffff : 0),
                        ((a < 2) ? 0xffffffff : 0));
                    res = XMVectorIsNaN(col);
                    for (int i = 0; i < 4; i++) {
                        if (XMVectorGetIntByIndex(res, i) != XMVectorGetIntByIndex(check, i)) {
                            printe("\n%s: %f %f %f %f = %x %x %x %x ... %x %x %x %x\n",
                                TestName,
                                XMVectorGetX(col), XMVectorGetY(col), XMVectorGetZ(col), XMVectorGetW(col),
                                XMVectorGetIntX(res), XMVectorGetIntY(res), XMVectorGetIntZ(res), XMVectorGetIntW(res),
                                XMVectorGetIntX(check), XMVectorGetIntY(check), XMVectorGetIntZ(check), XMVectorGetIntW(check));
                            ret = MATH_FAIL;
                        }
                    }
                }
            }
        }
    }
    printi("\n");
    return ret;
}
ISNAN_TEST_BEGIN

HRESULT Test306(LogProxy* pLog)
{
    //XMVectorLerp and XMVectorLerpV
    XMVECTORF32 q1 = { {1,0,0,0} };
    XMVECTORF32 q2 = { {0,1,0,0} };

    XMVECTOR r;
    COMPARISON c;
    HRESULT ret = S_OK;
    float f;
    XMVECTOR check;
    int k;

    //XMVectorLerp
    for (k = 0; k < 10; k++) {
        for (int i = 0; i < 4; i++) {
            q1.v = XMVectorSetByIndex(q1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q2.v = XMVectorSetByIndex(q2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        f = ((float)XM_RAND()) / 2000.f - 8.f;
        r = XMVectorLerp(q1, q2, f);
        // q1+f*(q2-q1)
        check = XMVectorAdd(q1, XMVectorScale(XMVectorSubtract(q2, q1), f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: f:%f (%d)\n", TestName, f, c);
            dqe(q1); dqe(q2); dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    //XMVectorLerpV
    XMVECTORF32 T = { {1,1,1,1} };
    for (k = 0; k < 10; k++) {
        for (int i = 0; i < 4; i++) {
            q1.v = XMVectorSetByIndex(q1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            q2.v = XMVectorSetByIndex(q2, ((float)XM_RAND()) / 2000.f - 8.f, i);
            T.v = XMVectorSetByIndex(T, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        r = XMVectorLerpV(q1, q2, T);
        // q1+T*(q2-q1);
        check = XMVectorAdd(q1, XMVectorMultiply(T, XMVectorSubtract(q2, q1)));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: f: (%d)\n", TestName, c);
            dqe(T); dqe(q1); dqe(q2); dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }


    return ret;
}
HRESULT Test307(LogProxy* pLog)
{
    //XMVectorLess
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check.v = XMVectorSetIntX(check, 0); }
        else { check.v = XMVectorSetIntX(check, 0); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check.v = XMVectorSetIntY(check, 0); }
        else { check.v = XMVectorSetIntY(check, 0); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); check.v = XMVectorSetIntZ(check, 0); }
        else { check.v = XMVectorSetIntZ(check, 0); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); check.v = XMVectorSetIntW(check, 0); }
        else { check.v = XMVectorSetIntW(check, 0); }
        XMVECTOR r = XMVectorLess(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntX(check, 0); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntY(check, 0); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntZ(check, 0); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntW(check, 0); }
        r = XMVectorLess(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }

    }
    return ret;
}
HRESULT Test308(LogProxy* pLog)
{
    //XMVectorLessOrEqual
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check.v = XMVectorSetIntX(check, 0); }
        else { check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check.v = XMVectorSetIntY(check, 0); }
        else { check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); check.v = XMVectorSetIntZ(check, 0); }
        else { check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); check.v = XMVectorSetIntW(check, 0); }
        else { check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        XMVECTOR r = XMVectorLessOrEqual(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        r = XMVectorLessOrEqual(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }

    }
    return ret;
}


HRESULT Test309(LogProxy* pLog)
{
    //XMVectorLog2
    HRESULT ret = S_OK;
    {
        XMVECTORF32 v = { {.01f, .1f, 1, 10} };
        XMVECTORF32 adder = { {.009f, .09f, .9f, 14.25f} };
        XMVECTORF32 check = {};

        for (float vy = XMVectorGetY(v); vy < 1; v.v = XMVectorAdd(v.v, adder)) {
            float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
            for (int i = 0; i < 4; i++) {
                check.v = XMVectorSetByIndex(check, log2f(XMVectorGetByIndex(v, i)), i);
            }
            XMVECTOR r = XMVectorLog2(v);
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINBIGEPSILON) {
                printe("%s 2: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }

            // Check alias
            r = XMVectorLog(v);
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINBIGEPSILON) {
                printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    //XMVectorLog10
    {
        XMVECTORF32 v = { {.01f, .1f, 1, 10} };
        XMVECTORF32 adder = { {.009f, .09f, .9f, 14.25f} };
        XMVECTORF32 check = {};

        for (float vy = XMVectorGetY(v); vy < 1; v.v = XMVectorAdd(v.v, adder)) {
            float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
            for (int i = 0; i < 4; i++) {
                check.v = XMVectorSetByIndex(check, log10f(XMVectorGetByIndex(v, i)), i);
            }
            XMVECTOR r = XMVectorLog10(v);
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINBIGEPSILON) {
                printe("%s 10: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    //XMVectorLogE
    {
        XMVECTORF32 v = { {.01f, .1f, 1, 10} };
        XMVECTORF32 adder = { {.009f, .09f, .9f, 14.25f} };
        XMVECTORF32 check = {};

        for (float vy = XMVectorGetY(v); vy < 1; v.v = XMVectorAdd(v.v, adder)) {
            float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
            for (int i = 0; i < 4; i++) {
                check.v = XMVectorSetByIndex(check, logf(XMVectorGetByIndex(v, i)), i);
            }
            XMVECTOR r = XMVectorLogE(v);
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINBIGEPSILON) {
                printe("%s E: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}

HRESULT Test311(LogProxy* pLog)
{
    //XMVectorMaximize deprecated for XNAMath except for the xboxmath compat header; use XMVectorMax instead
    HRESULT ret = S_OK;
    static const XMVECTORF32 v[] = {
        {0,0,0,0}, {0,0,0,0},{0,0,0,0},
        {5,10,15,20},{-5,-10,-16,-19},{5,10,15,20},
        {5,-10,15,-20},{-5,10,-16,19},{5,10,15,19},
        {-5,4,3.5,5},{4,3,4,6.5},{4,4,4,6.5}
    };
    for (int k = 0; k < countof(v); k += 3) {
        XMVECTOR r = XMVectorMax(v[k], v[k + 1]);
        COMPARISON c = CompareXMVECTOR(r, v[k + 2], 4);
        if (c > WITHIN2EPSILON) {
            XMVECTOR v1 = v[k], v2 = v[k + 1], check = v[k + 2];
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test312(LogProxy* pLog)
{
    //XMVectorMinimize deprecated for XNAMath except for the xboxmath compat header; use XMVectorMin instead
    HRESULT ret = S_OK;
    static const XMVECTORF32 v[] = {
        {0,0,0,0}, {0,0,0,0},{0,0,0,0},
        {5,10,15,20},{-5,-10,-16,-19},{-5,-10,-16,-19},
        {5,-10,15,-20},{-5,10,-16,19},{-5,-10,-16,-20},
        {-5,4,3.5,5},{4,3,4,6},{-5,3,3.5,5}
    };
    for (int k = 0; k < countof(v); k += 3) {
        XMVECTOR r = XMVectorMin(v[k], v[k + 1]);
        COMPARISON c = CompareXMVECTOR(r, v[k + 2], 4);
        if (c > WITHIN2EPSILON) {
            XMVECTOR v1 = v[k], v2 = v[k + 1], check = v[k + 2];
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test313(LogProxy* pLog)
{
    //XMVectorMod
    HRESULT ret = S_OK;
    static const XMVECTORF32 v[] = {
        {5.01f, 5,5,5}, {5,4,3,2}, {.01f,1,2,1},
        {8.5, .001f, 3, -.5}, {4, 1, 8, 1}, {.5,.001f,3,-.5}
    };
    for (int k = 0; k < countof(v); k += 3) {
        XMVECTOR r = XMVectorMod(v[k], v[k + 1]);
        COMPARISON c = CompareXMVECTOR(r, v[k + 2], 4);
        if (c > WITHIN4096) {
            XMVECTOR v1 = v[k], v2 = v[k + 1], check = v[k + 2];
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test314(LogProxy* pLog)
{
    //XMVectorModAngles
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r, check;
    XMVECTORF32 start = { {-Pi + .00001f,-Pi + .4f,-Pi + .8f,-Pi + 1.2f} };
    XMVECTORF32 Piv = { {Pi,Pi,Pi,Pi} };
    XMVECTORF32 adder = { {1.6f,1.6f,1.6f,1.6f} };
    XMVECTORF32 hundred = { {100,100,100,100} };
    XMVECTORF32 adder2 = { {8.5f, 9.f, 10.f, 10.5f} };
    XMVECTOR v = start;
    for (; ; v = XMVectorAdd(v, adder)) {
        float vx = XMVectorGetX(v);
        if (vx >= Pi)
            break;
        XMVECTOR t = XMVectorSubtract(v, XMVectorSubtract(Piv, Piv));
        r = XMVectorModAngles(t);
        check = v;
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            XMFLOAT4 ta, ra, checka;
            XMStoreFloat4(&ta, t);
            XMStoreFloat4(&ra, r);
            XMStoreFloat4(&checka, check);
            printe("%s(0): %f %f %f %f = %f %f %f %f... %f %f %f %f (%d)\n",
                TestName, ta.x, ta.y, ta.z, ta.w,
                ra.x, ra.y, ra.z, ra.w,
                checka.x, checka.y, checka.z, checka.w, c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
        t = XMVectorAdd(v, XMVectorAdd(Piv, Piv));
        r = XMVectorModAngles(t);
        check = v;
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            XMFLOAT4 ta, ra, checka;
            XMStoreFloat4(&ta, t);
            XMStoreFloat4(&ra, r);
            XMStoreFloat4(&checka, check);
            printe("%s(1): %f %f %f %f = %f %f %f %f... %f %f %f %f (%d)\n",
                TestName,
                ta.x, ta.y, ta.z, ta.w,
                ra.x, ra.y, ra.z, ra.w,
                checka.x, checka.y, checka.z, checka.w, c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    printi("\n");
    v = XMVectorNegate(hundred);
    for (float vz = XMVectorGetZ(v); vz < 100; v = XMVectorAdd(v, adder2)) {
        float vx = XMVectorGetX(v); float vy = XMVectorGetY(v); vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        r = XMVectorModAngles(v);
        FLOAT checkx = vx - ((floorf((vx + Pi) / (Pi + Pi))) * (Pi + Pi));
        FLOAT checky = vy - ((floorf((vy + Pi) / (Pi + Pi))) * (Pi + Pi));
        FLOAT checkz = vz - ((floorf((vz + Pi) / (Pi + Pi))) * (Pi + Pi));
        FLOAT checkw = vw - ((floorf((vw + Pi) / (Pi + Pi))) * (Pi + Pi));
        check = XMVectorSet(checkx, checky, checkz, checkw);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s(3): %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test315(LogProxy* pLog)
{
    //XMVectorMultiply
    //XMVECTOR operator *=, *
    HRESULT ret = S_OK;
#if defined(_M_FP_FAST) && defined(_XM_NO_INTRINSICS_)
    // Under /fp:fast, it is possible for the compiler to optimize
    //  (a / 100.f) * (b / 100.f)
    // to
    //  (a * b) * ONE_TEN_THOUSANDTHS
    // The maximum difference between the first and second expressions exceeds WITHIN100EPSILON
    // for 32-bit floats, so use WITHIN4096 instead.
    constexpr auto epsilon = WITHIN4096;
#else
    constexpr auto epsilon = WITHIN100EPSILON;
#endif
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            v1.v = XMVectorSetByIndex(v1, (float)XM_RAND() / 100.f, j);
            v2.v = XMVectorSetByIndex(v2, (float)XM_RAND() / 100.f, j);
            check.v = XMVectorSetByIndex(check, XMVectorGetByIndex(v1, j) * XMVectorGetByIndex(v2, j), j);
        }
        XMVECTOR r = XMVectorMultiply(v1, v2);
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        XMVECTOR r3 = v1;
        XMVECTOR r2 = v1 * v2;
        r3 *= v2;
#endif
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > epsilon)
        {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        c = CompareXMVECTOR(r2, check, 4);
        if (c > epsilon)
        {
            printe("%s (*): %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r2), XMVectorGetY(r2), XMVectorGetZ(r2), XMVectorGetW(r2),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
        c = CompareXMVECTOR(r3, check, 4);
        if (c > epsilon)
        {
            printe("%s (*=): %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r3), XMVectorGetY(r3), XMVectorGetZ(r3), XMVectorGetW(r3),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
#endif
    }
    return ret;
}
HRESULT Test316(LogProxy* pLog)
{
    //XMVectorMultiplyAdd
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;
    XMVECTORF32 check = { {.25, 21, 31, 43} };
    XMVECTORF32 v1 = { {1,2,3,4} }, v2 = { {.5,6,7,8} }, v3 = { {-.25, 9,10,11} };
    c = CompareXMVECTOR(r = XMVectorMultiplyAdd(v1, v2, v3), check, 4);
    if (c > WITHIN100EPSILON) {
        printe("%s: %f %f %f %f * %f %f %f %f + %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
            TestName,
            XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
            XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
            XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), XMVectorGetW(v3),
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            v1.v = XMVectorSetByIndex(v1, (float)XM_RAND() / 100.f, j);
            v2.v = XMVectorSetByIndex(v2, (float)XM_RAND() / 100.f, j);
            v3.v = XMVectorSetByIndex(v3, (float)XM_RAND() / 100.f, j);
            check.v = XMVectorSetByIndex(check, XMVectorGetByIndex(v1, j) * XMVectorGetByIndex(v2, j) + XMVectorGetByIndex(v3, j), j);
        }
        r = XMVectorMultiplyAdd(v1, v2, v3);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f * %f %f %f %f + %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), XMVectorGetW(v3),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test317(LogProxy* pLog)
{
    //XMVectorNearEqual
    int i, j, k;
    HRESULT ret = S_OK;
    /*/
        XMVECTOR r;
        XMVECTORF32 v1 = {{-20, -2, 2, 20}};
        XMVECTORF32 e  = {{.03125, .125, .5, 2}};
        static const XMVECTORI32 neg1 = {-1,-1,-1,-1};
        XMVECTOR check[5];
        check[0] = check[4] = XMVectorZero();
        check[1] = check[2] = check[3] = neg1;

        i=0;
        for(XMVECTOR v2 = v1 - e - e; XMVectorGetX(v2) <= XMVectorGetX(v1) + XMVectorGetX(e) + XMVectorGetX(e); v2 += e) {
            r = XMVectorNearEqual(v1,v2,e);
            for(j = 0; j < 4; j++) {
                if(XMVectorGetIntByIndex(check[i],j) != XMVectorGetIntByIndex(r,j)) {
                    printe("%s: i%ld, j%ld, %.2f, %.2f, e%.2f: %x ... %x\n", TestName, i,j,XMVectorGetByIndex(v1,j),XMVectorGetByIndex(v2,j),
                        XMVectorGetByIndex(e,j),XMVectorGetIntByIndex(r,j),XMVectorGetIntByIndex(check[i],j));
                    ret= MATH_FAIL;
                }
            }
            i++;
        }
        return ret;
    /*/
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    XMVECTORF32 e = {};
    for (j = 0; j < 16; j++) {
        for (k = 0; k < 2; k++) {
            for (i = 0; i < 4; i++) {
                v1.v = XMVectorSetByIndex(v1, (float)(XM_RAND()) / 1000.f, i);
                e.v = XMVectorSetByIndex(e, (float)(XM_RAND()) / 10000.f, i);
                v2.v = XMVectorSetByIndex(v2, XMVectorGetByIndex(v1, i) + ((k & 1) ? 1.f : -1.f) * ((j & (1 << i)) ? (XMVectorGetByIndex(e, i) + XMVectorGetByIndex(e, i)) : (XMVectorGetByIndex(e, i) / 2.f)), i);
                check.v = XMVectorSetIntByIndex(check, (j & (1 << i)) ? 0 : 0xffffffff, i);
            }
            XMVECTOR r = XMVectorNearEqual(v1, v2, e);
            bool newfail = false;
            for (i = 0; i < 4; i++) {
                if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                    printe("%s: j%d k%d i%d  %f, %f, %f = %x ... %x\n",
                        TestName, j, k, i, XMVectorGetByIndex(v1, i), XMVectorGetByIndex(v2, i), XMVectorGetByIndex(e, i),
                        XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                    ret = MATH_FAIL;
                    newfail = true;
                }
                else {
                    printi("%s: 0\n", TestName);
                    //"j%ld k%ld i%ld  %f, %f, %f = %x ... %x\n",
                    //TestName,j,k,i, XMVectorGetByIndex(v1,i),XMVectorGetByIndex(v2,i),XMVectorGetByIndex(e,i),
                    //  XMVectorGetIntByIndex(r,i),XMVectorGetIntByIndex(check,i));
                }
            }
            if (!newfail) {
                //				printi("%s: j%ld k%ld (0)\n", TestName, j,k);
            }
        }
    }
    return ret;
    /**/
}
HRESULT Test318(LogProxy* pLog)
{
    //XMVectorNegate
    //XMVECTOR operator -
    static const XMVECTORF32 v[] = {
        {1,2,0,.004f}, {-1,-2,-0.f,-.004f},
        {-5,-6.3f,-7,-8},{5,6.3f,7,8},
        {9,-10,11.75,-12},{-9,10,-11.75,12}
    };
    HRESULT ret = S_OK;
    XMVECTOR r;
    for (size_t i = 0; i < std::size(v); i += 2) {
        r = XMVectorNegate(v[i]);
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        XMVECTOR r2 = -v[i];
#endif
        COMPARISON c = CompareXMVECTOR(r, v[i + 1], 4);
        if (c != EXACT)
        {
            printe("%s: %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                XMVectorGetX(v[i]), XMVectorGetY(v[i]), XMVectorGetZ(v[i]), XMVectorGetW(v[i]),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(v[i + 1]), XMVectorGetY(v[i + 1]), XMVectorGetZ(v[i + 1]), XMVectorGetW(v[i + 1]), c);
            ret = MATH_FAIL;
        }
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        c = CompareXMVECTOR(r2, v[i + 1], 4);
        if (c != EXACT)
        {
            printe("%s (-): %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                XMVectorGetX(v[i]), XMVectorGetY(v[i]), XMVectorGetZ(v[i]), XMVectorGetW(v[i]),
                XMVectorGetX(r2), XMVectorGetY(r2), XMVectorGetZ(r2), XMVectorGetW(r2),
                XMVectorGetX(v[i + 1]), XMVectorGetY(v[i + 1]), XMVectorGetZ(v[i + 1]), XMVectorGetW(v[i + 1]), c);
            ret = MATH_FAIL;
        }
#endif
    }

    return ret;
}
HRESULT Test319(LogProxy* pLog)
{
    //XMVectorNegativeMultiplySubtract
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;
    XMVECTORF32 check = { {-.75, -3,-11,-21} };
    XMVECTORF32 v1 = { {1,2,3,4} }, v2 = { {.5,6,7,8} }, v3 = { {-.25, 9,10,11} };
    c = CompareXMVECTOR(r = XMVectorNegativeMultiplySubtract(v1, v2, v3), check, 4);
    if (c > WITHIN100EPSILON) {
        printe("%s: %f %f %f %f * %f %f %f %f + %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
            TestName,
            XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
            XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
            XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), XMVectorGetW(v3),
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
        ret = MATH_FAIL;
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            v1.v = XMVectorSetByIndex(v1, (float)XM_RAND() / 100.f, j);
            v2.v = XMVectorSetByIndex(v2, (float)XM_RAND() / 100.f, j);
            v3.v = XMVectorSetByIndex(v3, (float)XM_RAND() / 100.f, j);
            check.v = XMVectorSetByIndex(check, XMVectorGetByIndex(v3, j) - (XMVectorGetByIndex(v1, j) * XMVectorGetByIndex(v2, j)), j);
        }
        r = XMVectorNegativeMultiplySubtract(v1, v2, v3);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: -%f %f %f %f * %f %f %f %f + %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), XMVectorGetW(v3),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test320(LogProxy* pLog)
{
    //XMVectorNotEqual
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntX(check, 0); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntY(check, 0); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntZ(check, 0); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntW(check, 0); }
        XMVECTOR r = XMVectorNotEqual(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check.v = XMVectorSetIntX(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntX(check, 0); }
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check.v = XMVectorSetIntY(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntY(check, 0); }
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); check.v = XMVectorSetIntZ(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntZ(check, 0); }
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); check.v = XMVectorSetIntW(check, (uint32_t)-1); }
        else { check.v = XMVectorSetIntW(check, 0); }
        r = XMVectorNotEqual(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}

template<uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW> HRESULT PTest(LogProxy* pLog)
{
    XMVECTORF32 a = { 0, 1, 2, 3 };
    XMVECTORF32 b = { 4, 5, 6, 7 };
    XMVECTORF32 chk = { (float)PermuteX, (float)PermuteY, (float)PermuteZ, (float)PermuteW };

    HRESULT ret = S_OK;

    // Test template form
    XMVECTOR r = XMVectorPermute< PermuteX, PermuteY, PermuteZ, PermuteW >(a, b);
    if (CompareXMVECTOR(r, chk, 4) != EXACT)
    {
        printe("%s: (T) %u %u %u %u = %f %f %f %f ... %f %f %f %f\n",
            TestName, PermuteX, PermuteY, PermuteZ, PermuteW,
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
        ret = MATH_FAIL;
    }

    // Test function form
    r = XMVectorPermute(a, b, PermuteX, PermuteY, PermuteZ, PermuteW);
    if (CompareXMVECTOR(r, chk, 4) != EXACT)
    {
        printe("%s: (F) %u %u %u %u = %f %f %f %f ... %f %f %f %f\n",
            TestName, PermuteX, PermuteY, PermuteZ, PermuteW,
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
        ret = MATH_FAIL;
    }

    return ret;
}

HRESULT Test321(LogProxy* pLog)
{
    //XMVectorPermute
    XMVECTORF32 v1 = { {0,1,2,3} }, v2 = { {4,5,6,7} };
    XMVECTORF32 check = {};
    uint32_t in[4];
    HRESULT ret = S_OK;
    for (int k = 0; k < 8; k++) {
        for (int i = 0; i < 4; i++) {
            in[i] = XM_RAND() & 7;
            check.v = XMVectorSetByIndex(check, (float)(in[i]), i);
        }
        XMVECTOR r = XMVectorPermute(v1, v2, in[0], in[1], in[2], in[3]);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c != EXACT) {
            printe("%s: %u %u %u %u = %f %f %f %f ... %f %f %f %f\n",
                TestName, in[0], in[1], in[2], in[3],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }

    HRESULT r = PTest<0, 1, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 5, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<4, 1, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 5, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 5, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 1, 6, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 1, 6, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 5, 6, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 5, 6, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 1, 2, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 1, 2, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 5, 2, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 5, 2, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 1, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 1, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 5, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 0, 0, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<1, 1, 1, 1>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<2, 2, 2, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<3, 3, 3, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 4, 4, 4>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<5, 5, 5, 5>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<6, 6, 6, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<7, 7, 7, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 1, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<3, 2, 1, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 5, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<7, 6, 5, 4>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 2, 4, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<3, 1, 5, 4>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<7, 6, 1, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 5, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 7, 1, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 1, 2, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<3, 2, 7, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<5, 2, 1, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<4, 1, 6, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 1, 4, 5>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<1, 0, 4, 5>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 1, 5, 4>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<1, 0, 5, 4>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<2, 3, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<3, 2, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<2, 3, 7, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<3, 2, 7, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 1, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<1, 0, 6, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<0, 1, 7, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<1, 0, 7, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<3, 2, 4, 5>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<2, 3, 5, 4>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<3, 2, 5, 4>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 4, 2, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<1, 5, 3, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 4, 1, 5>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<2, 6, 3, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<0, 2, 4, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<1, 3, 5, 7>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<1, 2, 3, 4>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<2, 3, 4, 5>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = PTest<3, 4, 5, 6>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = PTest<6, 7, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    return ret;
}

HRESULT Test323(LogProxy* pLog)
{
    //XMVectorReciprocal
    XMVECTORF32 v = { {-100, -1, .001f, 10} };
    XMVECTORF32 adder = { {5.1f, .051f, .9f, 14.25f} };
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    COMPARISON c;

    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder)) {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        for (int i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check, 1 / XMVectorGetByIndex(v, i), i);
        }
        XMVECTOR r = XMVectorReciprocal(v);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

#ifndef __clang__
    // Test special cases around 0, -0, +INF, -INF, c_Q_NAN
    {
#if defined(_M_FP_FAST) && defined(_XM_NO_INTRINSICS_)
        static const float xval[] = { c_Q_NAN };
        static const float cval[] = { c_Q_NAN };
#else
        static const float xval[] = { 0.f,  -0.f, c_INF, -c_INF, c_Q_NAN };
        static const float cval[] = { c_INF, -c_INF,  0.f,  -0.f, c_Q_NAN };
#endif

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVectorReciprocal(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
            c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: 1/%f = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }
#endif // !__clang__

    return ret;
}
HRESULT Test324(LogProxy* pLog)
{
    //XMVectorReciprocalEst
    XMVECTORF32 v = { {-100, -1, .001f, 10} };
    XMVECTORF32 adder = { {5.1f, .051f, .9f, 14.25f} };
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    COMPARISON c;

    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder)) {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        for (int i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check, 1 / XMVectorGetByIndex(v, i), i);
        }
        XMVECTOR r = XMVectorReciprocalEst(v);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN1_256) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    // Test special cases around 0, -0, +INF, -INF, c_Q_NAN
    {
#if defined(_M_FP_FAST)  && defined(_XM_NO_INTRINSICS_)
        static const float xval[] = { c_Q_NAN };
        static const float cval[] = { c_Q_NAN };
#else
        static const float xval[] = { 0.f,  -0.f, c_INF, -c_INF, c_Q_NAN };
        static const float cval[] = { c_INF, -c_INF,  0.f,  -0.f, c_Q_NAN };
#endif

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVectorReciprocalEst(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
            c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: 1/%f = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}
HRESULT Test325(LogProxy* pLog)
{
    //XMVectorReciprocalSqrt
    XMVECTORF32 v = { {-100, -1, .001f, 10} };
    XMVECTORF32 adder = { {5.1f, .051f, .9f, 14.25f} };
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    COMPARISON c;

    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder)) {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        for (int i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check, 1.f / sqrtf(XMVectorGetByIndex(v, i)), i);
        }
        XMVECTOR r = XMVectorReciprocalSqrt(v);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

#if !defined(__GNUC__) && !defined(__INTEL_LLVM_COMPILER)
    // Test special cases around 0, -0, +INF, -INF, c_Q_NAN
    {

        // The VMX128 implementation of the rsqrt always returns c_Q_NAN for all specials
#if (defined(_XENON) || defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC)) && !defined(_XM_NO_INTRINSICS_)
        static const float xval[] = { 0.f,     -0.f,   c_INF,  -c_INF, c_Q_NAN };
        static const float cval[] = { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN };
#elif defined(_M_FP_FAST)  && defined(_XM_NO_INTRINSICS_)
        static const float xval[] = { c_Q_NAN };
        static const float cval[] = { c_Q_NAN };
#else
        static const float xval[] = { 0.f,     -0.f,   c_INF,  -c_INF, c_Q_NAN };
        static const float cval[] = { c_INF,    -c_INF,      0, c_Q_NAN, c_Q_NAN };
#endif

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVectorReciprocalSqrt(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
            c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: 1/sqrt(%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }
#endif // !__GNUC__

    return ret;
}
HRESULT Test326(LogProxy* pLog)
{
    //XMVectorReciprocalSqrtEst
    XMVECTORF32 v = { {-100, -1, .001f, 10} };
    XMVECTORF32 adder = { {5.1f, .051f, .9f, 14.25f} };
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    COMPARISON c;

    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder)) {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        for (int i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check, 1.f / sqrtf(XMVectorGetByIndex(v, i)), i);
        }
        XMVECTOR r = XMVectorReciprocalSqrtEst(v);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN1_256) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    // Test special cases around 0, -0, +INF, -INF, c_Q_NAN
    {
#if defined(_M_FP_FAST)  && defined(_XM_NO_INTRINSICS_)
        static const float xval[] = { c_Q_NAN };
        static const float cval[] = { c_Q_NAN };
#else
        static const float xval[] = { 0.f,  -0.f, c_INF, -c_INF, c_Q_NAN };
        static const float cval[] = { c_INF, -c_INF,    0, c_Q_NAN, c_Q_NAN };
#endif

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVectorReciprocalSqrtEst(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
            c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: 1/sqrt(%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}
HRESULT Test327(LogProxy* pLog)
{
    HRESULT ret = S_OK;
    XMVECTOR v;
    static const float f[] = { 0, 1, -.28f, c_Q_NAN, c_INF };
    static const uint32_t n[] = { 0, 1, (uint32_t)-1, 0xabcdef21, 0x8000000, 0xffff0000 };

    //XMVectorReplicate
    {
        for (int k = 0; k < countof(f); k++)
        {
            v = XMVectorReplicate(f[k]);
            for (int i = 0; i < 4; i++)
            {
                if (XMVectorGetIntByIndex(v, i) != *(const uint32_t*)&f[k])
                {
                    printe("%s(0): %x ... %x\n", TestName, *(const uint32_t*)&f[k], XMVectorGetIntByIndex(v, i));
                    ret = MATH_FAIL;
                }
            }
        }
    }

    //XMVectorReplicatePtr
    {
        for (int k = 0; k < countof(f); k++)
        {
            v = XMVectorReplicatePtr(&f[k]);
            for (int i = 0; i < 4; i++)
            {
                if (XMVectorGetIntByIndex(v, i) != *(const uint32_t*)&f[k])
                {
                    printe("%s(1): %x ... %x\n", TestName, *(const uint32_t*)&f[k], XMVectorGetIntByIndex(v, i));
                    ret = MATH_FAIL;
                }
            }
        }
    }

    //XMVectorReplicateInt
    {
        for (int k = 0; k < countof(f); k++)
        {
            v = XMVectorReplicateInt(n[k]);
            for (int i = 0; i < 4; i++)
            {
                if (XMVectorGetIntByIndex(v, i) != *(const uint32_t*)&n[k])
                {
                    printe("%s(2): %x ... %x\n", TestName, *(const uint32_t*)&n[k], XMVectorGetIntByIndex(v, i));
                    ret = MATH_FAIL;
                }
            }
        }
    }

    //XMVectorReplicateIntPtr
    {
        for (int k = 0; k < countof(f); k++)
        {
            v = XMVectorReplicateIntPtr(&n[k]);
            for (int i = 0; i < 4; i++)
            {
                if (XMVectorGetIntByIndex(v, i) != *(const uint32_t*)&n[k])
                {
                    printe("%s(2): %x ... %x\n", TestName, *(const uint32_t*)&n[k], XMVectorGetIntByIndex(v, i));
                    ret = MATH_FAIL;
                }
            }
        }
    }

    return ret;
}

inline float RoundToNearest(float x)
{
    // Round to nearest (even) a.k.a. banker's rounding
    float i = floorf(x);
    x -= i;
    if (x < 0.5f)
        return i;
    if (x > 0.5f)
        return i + 1.f;

    float int_part;
    modff(i / 2.f, &int_part);
    if ((2.f * int_part) == i)
    {
        return i;
    }

    return i + 1.f;
}

HRESULT Test328(LogProxy* pLog)
{
    //XMVectorRound
    HRESULT ret = S_OK;

    static const XMVECTORF32 f[] = {
        {0,0,0,0},  {0,0,0,0},
        { 0.5f, 1.5f, -0.5f, -1.5f },{ 0.f, 2.f, -0.f, -2.f },  // Round to nearest (even) a.k.a. banker's rounding
        {-2, -1.99f, -1.49f, -1.01f}, {-2,-2,-1,-1},
        {-1, -0.99f, -0.51f, -0.01f}, {-1,-1,-1,-0},
        { 0 ,  .99f,   .51f,   .01f}, { 0, 1, 1, 0},
        {40.49f,40.51f, 40.49f, 40.01f}, {40,41,40,40},
        {8388608.f, 8388609.f, 8388607.f, 8388607.5f }, {8388608.f, 8388609.f, 8388607.f, 8388608.f },
        {-8388608.f, -8388609.f, -8388607.f, -8388607.5f }, {-8388608.f, -8388609.f, -8388607.f, -8388608.f },
        {11387829.f, 11387829.f, 11387829.f, 11387829.f}, {11387829.f, 11387829.f, 11387829.f, 11387829.f},
        {-8419647.f, -8419647.f, -8419647.f, -8419647.f}, {-8419647.f, -8419647.f, -8419647.f, -8419647.f},
        {2147483520.f, 2147483520.f, 2147483520.f, 2147483520.f }, {2147483520.f, 2147483520.f, 2147483520.f, 2147483520.f },
        {-2147483520.f, -2147483520.f, -2147483520.f, -2147483520.f }, {-2147483520.f, -2147483520.f, -2147483520.f, -2147483520.f },
        {8000000000.0f, 8000000000.0f, 8000000000.0f, 8000000000.0f }, {8000000000.0f, 8000000000.0f, 8000000000.0f, 8000000000.0f },
        {   c_INF,   c_INF,   c_INF,   c_INF}, {   c_INF,   c_INF,   c_INF,   c_INF},
        {  -c_INF,  -c_INF,  -c_INF,  -c_INF}, {  -c_INF,  -c_INF,  -c_INF,  -c_INF},
        { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN}, { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN},
    };
    for (size_t i = 0; i < std::size(f); i += 2) {
        XMVECTOR v1 = f[i];
        XMVECTOR check = f[i + 1];
        XMVECTOR r = XMVectorRound(v1);
        COMPARISON c = CompareXMVECTOR(r, check, 4);

        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }

    for (int j = -256; j <= 256; ++j)
    {
        float num = static_cast<float>(j);
        XMVECTORF32 v1 = { num, num + 0.1f, num + 0.5f, num + 0.9f };
        XMVECTORF32 chk = { RoundToNearest(num), RoundToNearest(num + 0.1f), RoundToNearest(num + 0.5f), RoundToNearest(num + 0.9f) };
        XMVECTOR r = XMVectorRound(v1);
        COMPARISON c = CompareXMVECTOR(r, chk, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
            ret = MATH_FAIL;
        }
    }

    for (float num = -1.f; num <= 1.f; num += 0.01f)
    {
        XMVECTORF32 v1 = { num, num + 0.1f, num + 0.5f, num + 0.9f };
        XMVECTORF32 chk = { RoundToNearest(num), RoundToNearest(num + 0.1f), RoundToNearest(num + 0.5f), RoundToNearest(num + 0.9f) };
        XMVECTOR r = XMVectorRound(v1);
        COMPARISON c = CompareXMVECTOR(r, chk, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test329(LogProxy* pLog)
{
    //XMVectorSaturate
    COMPARISON c = EXACT;
    HRESULT ret = S_OK;
    static const XMVECTORF32 f[] = {
        {-1,0,1,2},  {0,0,1,1},
        {-100, 10,.5,.001f}, {0,1,.5,.001f},
        {.3f,.25,.75f,1}, {.3f,.25,.75f,1}
    };
    for (size_t i = 0; i < std::size(f); i += 2) {
        XMVECTOR v1 = f[i];
        XMVECTOR check = f[i + 1];
        XMVECTOR r = XMVectorSaturate(v1);
        COMPARISON temp = CompareXMVECTOR(r, check, 4);
        printi("%s: %d\n", TestName, temp);
        c = std::max(c, temp);
        if (temp > EXACT) {
            printe("%f %f %f %f : %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test330(LogProxy* pLog)
{
    //XMVectorScale
    //XMVECTOR operator *=S, *S, S*
    HRESULT ret = S_OK;
    static const XMVECTORF32 f[] = {
        {-1,0,1,2},  {-1,0,1,2},
        {-100, 10,.5,.001f}, {12.5, -1.25, -.0625, -.000125f},
        {.125f,.25,.75f,1}, {12.5,25,75,100}
    };
    static const float s[] = { 1,0, -.125f, 0,100 ,0 };
    for (size_t i = 0; i < std::size(f); i += 2) {
        XMVECTOR v1 = f[i];
        XMVECTOR check = f[i + 1];
        XMVECTOR r = XMVectorScale(v1, s[i]);
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        XMVECTOR r2 = v1 * s[i];
        XMVECTOR r3 = s[i] * v1;
        XMVECTOR r4 = v1;
        r4 *= s[i];
#endif
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN100EPSILON)
        {
            printe("%s: %f %f %f %f, %f : %f %f %f %f ... %f %f %f %f\n", TestName,
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), s[i],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        c = CompareXMVECTOR(r2, check, 4);
        if (c > WITHIN100EPSILON)
        {
            printe("%s (*S): %f %f %f %f, %f : %f %f %f %f ... %f %f %f %f\n", TestName,
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), s[i],
                XMVectorGetX(r2), XMVectorGetY(r2), XMVectorGetZ(r2), XMVectorGetW(r2),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
        c = CompareXMVECTOR(r3, check, 4);
        if (c > WITHIN100EPSILON)
        {
            printe("%s (S*): %f, %f %f %f %f : %f %f %f %f ... %f %f %f %f\n", TestName,
                s[i], XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r3), XMVectorGetY(r3), XMVectorGetZ(r3), XMVectorGetW(r3),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
        c = CompareXMVECTOR(r4, check, 4);
        if (c > WITHIN100EPSILON)
        {
            printe("%s (*=S): %f %f %f %f, %f : %f %f %f %f ... %f %f %f %f\n", TestName,
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), s[i],
                XMVectorGetX(r4), XMVectorGetY(r4), XMVectorGetZ(r4), XMVectorGetW(r4),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
#endif
    }
    return ret;
}
HRESULT Test331(LogProxy* pLog)
{
    //XMVectorSelect
    static const XMVECTORU32 zero = { 0,0,0,0 };
    static const XMVECTORU32 negone = { 0xffffffff,0xffffffff,0xffffffff,0xffffffff };
    int p[4];
    HRESULT ret = S_OK;
    for (int j = 0; j < 5; j++) {
        int i;
        for (i = 0; i < 4; i++) {
            p[i] = XM_RAND() + (XM_RAND() << 14) + (XM_RAND() << 28);
        }
        XMVECTOR vecp = XMVectorSetInt(p[0], p[1], p[2], p[3]);
        XMVECTORU32 r1;
        r1.v = XMVectorSelect(zero, negone, vecp);
        for (i = 0; i < 4; i++) {
            if (r1.u[i] != (uint32_t)p[i]) {
                printe("%s: (%d) 0x00000000 0xFFFFFFFF %x = %x ... %x\n", TestName, i,
                    static_cast<unsigned int>(p[i]), r1.u[i], static_cast<unsigned int>(p[i]));
                ret = MATH_FAIL;
            }
        }
    }
    for (int j = 0; j < 10; j++) {
        XMVECTORU32 v0;
        XMVECTORU32 v1;
        int i;
        for (i = 0; i < 4; i++) {
            v0.u[i] = XM_RAND() + (XM_RAND() << 14) + (XM_RAND() << 28);
            v1.u[i] = XM_RAND() + (XM_RAND() << 14) + (XM_RAND() << 28);
            p[i] = XM_RAND() + (XM_RAND() << 14) + (XM_RAND() << 28);
        }
        XMVECTOR vecp = XMVectorSetInt(p[0], p[1], p[2], p[3]);
        XMVECTORU32 r1;
        r1.v = XMVectorSelect(v0, v1, vecp);
        for (i = 0; i < 4; i++) {
            if (r1.u[i] != ((v0.u[i] & ~p[i]) | (v1.u[i] & p[i]))) {
                printe("%s: (%d) %x %x %x = %x ... %x\n", TestName, i,
                    v0.u[i], v1.u[i], static_cast<unsigned int>(p[i]), r1.u[i], ((v0.u[i] & ~p[i]) | (v1.u[i] & p[i])));
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}
HRESULT Test332(LogProxy* pLog)
{
    //XMVectorSelectControl
    static const XMVECTORF32 v[] = { {1,2,3,4}, {-10,-c_INF,c_Q_NAN,0} };
    HRESULT ret = S_OK;
    for (int k = 0; k < 16; k++) {
        XMVECTOR p = XMVectorSelectControl(k & 1, (k & 2) >> 1, (k & 4) >> 2, (k & 8) >> 3);
        XMVECTOR r = XMVectorSelect(v[0], v[1], p);
        for (int i = 0; i < 4; i++) {
            COMPARISON c = Compare(XMVectorGetByIndex(r, i), XMVectorGetByIndex(v[1 & (k >> i)], i));
            if (c != EXACT) {
                printe("%s: k%d i%d %f %f = %f ... %f\n",
                    TestName, k, i, XMVectorGetByIndex(v[0], i), XMVectorGetByIndex(v[1], i), XMVectorGetByIndex(r, i), XMVectorGetByIndex(v[1 & (k >> 1)], i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test333(LogProxy* pLog)
{
    //XMVectorSet
    static const float f[] = { -20, 5, 0, 1, c_Q_NAN, -c_INF, 3, -.123f };
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float p0 = f[XM_RAND() & 7];
        float p1 = f[XM_RAND() & 7];
        float p2 = f[XM_RAND() & 7];
        float p3 = f[XM_RAND() & 7];
        XMVECTOR v = XMVectorSet(p1, p3, p0, p2);
        XMVECTORF32 check = { p1,p3,p0,p2 };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}


HRESULT Test334(LogProxy* pLog)
{
    //XMVectorSin
    XMVECTORF32 v = { {-Pi / 2.f - 20, -Pi / 2.f, 0, Pi / 2.f + 20} };
    XMVECTORF32 adder = { {1, .05f, .05f, 1} };
    COMPARISON c;
    HRESULT ret = S_OK;
    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder))
    {
        XMVECTOR r = XMVectorSin(v);
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        XMVECTORF32 check = { {sinf(vx),sinf(vy),sinf(vz),sinf(vw)} };
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096 || (fabs(XMVectorGetX(r)) > 1) || (fabs(XMVectorGetY(r)) > 1) || (fabs(XMVectorGetZ(r)) > 1) || (fabs(XMVectorGetW(r)) > 1)) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    if (XMVectorGetW(v) < 20) {
        printe("\n\nERROR!!! Compiler bug 1843 has struck again\n\n");
        ret = MATH_FAIL;
    }

    for (float f = UC_START; f <= UC_END; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        XMVECTOR r = XMVectorSin(v);
        XMVECTOR check = XMVectorReplicate(sinf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test335(LogProxy* pLog)
{
    //XMVectorSinCos
    XMVECTORF32 v = { {-Pi / 2.f - 20, -Pi / 2.f, 0, Pi / 2.f + 20} };
    XMVECTORF32 adder = { {1, .05f, .05f, 1} };
    //	XMVECTORF32 v = {{-Pi, -Pi/2.f, 0, Pi/2.f}};
    //	XMVECTOR adder = {{.05f, .05f, .05f, .05f}};
    XMVECTOR rs, rc;
    COMPARISON c, c2;
    HRESULT ret = S_OK;

    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder))
    {
        XMVectorSinCos(&rs, &rc, v);
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        XMVECTORF32 checks = { {sinf(vx),sinf(vy),sinf(vz),sinf(vw)} };
        XMVECTORF32 checkc = { {cosf(vx),cosf(vy),cosf(vz),cosf(vw)} };
        c = CompareXMVECTOR(rs, checks, 4);
        c2 = CompareXMVECTOR(rc, checkc, 4);
        if (c > WITHIN4096 || c2 > WITHIN4096 || (fabs(XMVectorGetX(rs)) > 1) || (fabs(XMVectorGetY(rs)) > 1) || (fabs(XMVectorGetZ(rs)) > 1) || (fabs(XMVectorGetW(rs)) > 1) || (fabs(XMVectorGetX(rc)) > 1) || (fabs(XMVectorGetY(rc)) > 1) || (fabs(XMVectorGetZ(rc)) > 1) || (fabs(XMVectorGetW(rc)) > 1)) {
            printe("%s: %f %f %f %f = %f %f %f %f, %f %f %f %f\n  ... %f %f %f %f, %f %f %f %f(%d %d)\n",
                TestName,
                vx, vy, vz, vw,
                XMVectorGetX(rs), XMVectorGetY(rs), XMVectorGetZ(rs), XMVectorGetW(rs),
                XMVectorGetX(rc), XMVectorGetY(rc), XMVectorGetZ(rc), XMVectorGetW(rc),
                XMVectorGetX(checks), XMVectorGetY(checks), XMVectorGetZ(checks), XMVectorGetW(checks),
                XMVectorGetX(checkc), XMVectorGetY(checkc), XMVectorGetZ(checkc), XMVectorGetW(checkc), c, c2);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d %d\n", TestName, c, c2);
        }
    }

    for (float f = UC_START; f <= UC_END; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        XMVectorSinCos(&rs, &rc, v);
        XMVECTOR checks = XMVectorReplicate(sinf(f));
        XMVECTOR checkc = XMVectorReplicate(cosf(f));
        c = CompareXMVECTOR(rs, checks, 4);
        c2 = CompareXMVECTOR(rc, checkc, 4);
        if (c > WITHIN10EPSILON || c2 > WITHIN10EPSILON)
        {
            printe("%s: (%f) = %f %f %f %f, %f %f %f %f\n  ... %f %f %f %f, %f %f %f %f(%d %d)\n",
                TestName, f,
                XMVectorGetX(rs), XMVectorGetY(rs), XMVectorGetZ(rs), XMVectorGetW(rs),
                XMVectorGetX(rc), XMVectorGetY(rc), XMVectorGetZ(rc), XMVectorGetW(rc),
                XMVectorGetX(checks), XMVectorGetY(checks), XMVectorGetZ(checks), XMVectorGetW(checks),
                XMVectorGetX(checkc), XMVectorGetY(checkc), XMVectorGetZ(checkc), XMVectorGetW(checkc), c, c2);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test336(LogProxy* pLog)
{
    //XMVectorSinCosEst
    //	XMVECTORF32 v = {{-Pi/2.f - 20, -Pi/2.f, 0, Pi/2.f + 20}};
    //	XMVECTOR adder = {{1, .05f, .05f, 1}};
    XMVECTORF32 v = { {-Pi, -Pi / 2.f, 0, Pi / 2.f} };
    XMVECTORF32 adder = { {.05f, .05f, .05f, .05f} };
    XMVECTOR rs, rc;
    COMPARISON c, c2;
    HRESULT ret = S_OK;

    const COMPARISON threshold = WITHIN4096;

    for (float vw = XMVectorGetW(v); ; v.v = XMVectorAdd(v.v, adder))
    {
        float vx = XMVectorGetX(v); float vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); vw = XMVectorGetW(v);
        if (vw >= XM_PI)
            break;
        XMVectorSinCosEst(&rs, &rc, v);
        XMVECTORF32 checks = { {sinf(vx),sinf(vy),sinf(vz),sinf(vw)} };
        XMVECTORF32 checkc = { {cosf(vx),cosf(vy),cosf(vz),cosf(vw)} };
        c = CompareXMVECTOR(rs, checks, 4);
        c2 = CompareXMVECTOR(rc, checkc, 4);
        if (c > threshold || c2 > threshold || (fabs(XMVectorGetX(rs)) > 1) || (fabs(XMVectorGetY(rs)) > 1) || (fabs(XMVectorGetZ(rs)) > 1) || (fabs(XMVectorGetW(rs)) > 1) || (fabs(XMVectorGetX(rc)) > 1) || (fabs(XMVectorGetY(rc)) > 1) || (fabs(XMVectorGetZ(rc)) > 1) || (fabs(XMVectorGetW(rc)) > 1)) {
            printe("%s: %f %f %f %f = %f %f %f %f, %f %f %f %f\n  ... %f %f %f %f, %f %f %f %f(%d %d)\n",
                TestName, vx, vy, vz, vw,
                XMVectorGetX(rs), XMVectorGetY(rs), XMVectorGetZ(rs), XMVectorGetW(rs),
                XMVectorGetX(rc), XMVectorGetY(rc), XMVectorGetZ(rc), XMVectorGetW(rc),
                XMVectorGetX(checks), XMVectorGetY(checks), XMVectorGetZ(checks), XMVectorGetW(checks),
                XMVectorGetX(checkc), XMVectorGetY(checkc), XMVectorGetZ(checkc), XMVectorGetW(checkc), c, c2);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d %d\n", TestName, c, c2);
        }
    }

    for (float f = -Pi; f < Pi; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        XMVectorSinCosEst(&rs, &rc, v);
        XMVECTOR checks = XMVectorReplicate(sinf(f));
        XMVECTOR checkc = XMVectorReplicate(cosf(f));
        c = CompareXMVECTOR(rs, checks, 4);
        c2 = CompareXMVECTOR(rc, checkc, 4);
        if (c > threshold || c2 > threshold)
        {
            printe("%s: (%f) = %f %f %f %f, %f %f %f %f\n  ... %f %f %f %f, %f %f %f %f(%d %d)\n",
                TestName, f,
                XMVectorGetX(rs), XMVectorGetY(rs), XMVectorGetZ(rs), XMVectorGetW(rs),
                XMVectorGetX(rc), XMVectorGetY(rc), XMVectorGetZ(rc), XMVectorGetW(rc),
                XMVectorGetX(checks), XMVectorGetY(checks), XMVectorGetZ(checks), XMVectorGetW(checks),
                XMVectorGetX(checkc), XMVectorGetY(checkc), XMVectorGetZ(checkc), XMVectorGetW(checkc), c, c2);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test337(LogProxy* pLog)
{
    //XMVectorSinEst
    //	XMVECTORF32 v = {{-Pi/2.f - 20, -Pi/2.f, 0, Pi/2.f + 20}};
    //	XMVECTOR adder = {{1, .05f, .05f, 1}};
    static const XMVECTORF32 vstart = { -Pi, -Pi / 2.f, 0, Pi / 2.f };
    static const XMVECTORF32 adder = { .05f, .05f, .05f, .05f };
    XMVECTOR v = vstart;
    XMVECTOR r;
    COMPARISON c;
    HRESULT ret = S_OK;

    const COMPARISON threshold = WITHIN4096;

    for (float vw = XMVectorGetW(v); ; v = XMVectorAdd(v, adder))
    {
        float vx = XMVectorGetX(v); float vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); vw = XMVectorGetW(v);
        if (vw >= XM_PI)
            break;
        r = XMVectorSinEst(v);
        XMVECTOR check = XMVectorSet(sinf(vx), sinf(vy), sinf(vz), sinf(vw));
        c = CompareXMVECTOR(r, check, 4);
        if (c > threshold || (fabs(XMVectorGetX(r)) > 1) || (fabs(XMVectorGetY(r)) > 1) || (fabs(XMVectorGetZ(r)) > 1) || (fabs(XMVectorGetW(r)) > 1)) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, vx, vy, vz, vw, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    for (float f = -Pi; f <= Pi; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorSinEst(v);
        XMVECTOR check = XMVectorReplicate(sinf(f));
        c = CompareXMVECTOR(r, check, 4);
        if (c > threshold)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test338(LogProxy* pLog)
{
    //XMVectorSinH
    HRESULT ret = S_OK;
    static const XMVECTORF32 vstart = { -120, -4, 0, 4 };
    static const XMVECTORF32 adder = { 116.f / 26.f, 4.f / 26.f, 4.f / 26.f, 116.f / 26.f };
    XMVECTOR v = vstart;
    for (float vy = XMVectorGetY(v); vy < 0; v = XMVectorAdd(v, adder)) {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        XMVECTOR check = XMVectorSet(sinhf(vx), sinhf(vy), sinhf(vz), sinhf(vw));
        XMVECTOR r = XMVectorSinH(v);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %.2f: %f ... %f    %.2f: %f ... %f\n%.2f: %f ... %f   %.2f: %f ... %f (%d)\n",
                TestName, vx, XMVectorGetX(r), XMVectorGetX(check), vy, XMVectorGetY(r), XMVectorGetY(check), vz, XMVectorGetZ(r), XMVectorGetZ(check), vw, XMVectorGetW(r), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    // Range for hyperbolic sine is [-INF,+INF]
    for (float f = -4 * Pi; f <= 4 * Pi; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        XMVECTOR r = XMVectorSinH(v);
        XMVECTOR check = XMVectorReplicate(sinhf(f));
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test340(LogProxy* pLog)
{
    //XMVectorSplatW
    static const XMVECTORF32 v[] = {
        {1,2,0,.004f}, {.004f,.004f,.004f,.004f},
        {-5,-6.3f,-7,-8},{-8,-8,-8,-8},
        {-10,11.75,-12,sqrtf(-1.f)},{sqrtf(-1.f),sqrtf(-1.f),sqrtf(-1.f),sqrtf(-1.f)}
    };
    HRESULT ret = S_OK;
    XMVECTOR r;
    for (size_t i = 0; i < std::size(v); i += 2) {
        COMPARISON c = CompareXMVECTOR(r = XMVectorSplatW(v[i]), v[i + 1], 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[i]), XMVectorGetY(v[i]), XMVectorGetZ(v[i]), XMVectorGetW(v[i]),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(v[i + 1]), XMVectorGetY(v[i + 1]), XMVectorGetZ(v[i + 1]), XMVectorGetW(v[i + 1]), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test341(LogProxy* pLog)
{
    //XMVectorSplatX
    static const XMVECTORF32 v[] = {
        {1,2,0,.004f}, {1,1,1,1},
        {-5,-6.3f,-7,-8},{-5,-5,-5,-5},
        {sqrtf(-1.f),-10,11.75,-12},{sqrtf(-1.f),sqrtf(-1.f),sqrtf(-1.f),sqrtf(-1.f)}
    };
    HRESULT ret = S_OK;
    XMVECTOR r;
    for (size_t i = 0; i < std::size(v); i += 2) {
        COMPARISON c = CompareXMVECTOR(r = XMVectorSplatX(v[i]), v[i + 1], 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[i]), XMVectorGetY(v[i]), XMVectorGetZ(v[i]), XMVectorGetW(v[i]),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(v[i + 1]), XMVectorGetY(v[i + 1]), XMVectorGetZ(v[i + 1]), XMVectorGetW(v[i + 1]), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test342(LogProxy* pLog)
{
    //XMVectorSplatY
    static const XMVECTORF32 v[] = {
        {1,2,0,.004f}, {2,2,2,2},
        {-5,-6.3f,-7,-8},{-6.3f,-6.3f,-6.3f,-6.3f},
        {-10,sqrtf(-1.f),11.75,-12},{sqrtf(-1.f),sqrtf(-1.f),sqrtf(-1.f),sqrtf(-1.f)}
    };
    HRESULT ret = S_OK;
    XMVECTOR r;
    for (size_t i = 0; i < std::size(v); i += 2) {
        COMPARISON c = CompareXMVECTOR(r = XMVectorSplatY(v[i]), v[i + 1], 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[i]), XMVectorGetY(v[i]), XMVectorGetZ(v[i]), XMVectorGetW(v[i]),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(v[i + 1]), XMVectorGetY(v[i + 1]), XMVectorGetZ(v[i + 1]), XMVectorGetW(v[i + 1]), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test343(LogProxy* pLog)
{
    //XMVectorSplatZ
    static const XMVECTORF32 v[] = {
        {1,2,0,.004f}, {0,0,0,0},
        {-5,-6.3f,-7,-8},{-7,-7,-7,-7},
        {-10,11.75,sqrtf(-1.f),-12},{sqrtf(-1.f),sqrtf(-1.f),sqrtf(-1.f),sqrtf(-1.f)}
    };
    HRESULT ret = S_OK;
    XMVECTOR r;
    for (size_t i = 0; i < std::size(v); i += 2) {
        COMPARISON c = CompareXMVECTOR(r = XMVectorSplatZ(v[i]), v[i + 1], 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[i]), XMVectorGetY(v[i]), XMVectorGetZ(v[i]), XMVectorGetW(v[i]),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(v[i + 1]), XMVectorGetY(v[i + 1]), XMVectorGetZ(v[i + 1]), XMVectorGetW(v[i + 1]), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test344(LogProxy* pLog)
{
    //XMVectorSqrt
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;
    XMVECTORF32 v = { {0, 1, 100, 10000} };
    XMVECTORF32 adder = { {.01f, 1.f, 100.2f, 10300.1f} };
    printi("%s: ", TestName);
    for (int i = 0; i < 100; i++) {
        XMVECTORF32 check = { {sqrtf(XMVectorGetX(v)), sqrtf(XMVectorGetY(v)),sqrtf(XMVectorGetZ(v)),sqrtf(XMVectorGetW(v))} };
        r = XMVectorSqrt(v);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("\n%s: %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%ld  ", c);
        }
        v.v = XMVectorAdd(v.v, adder);
    }
    printi("\n");

#ifndef __clang__
    // Test special cases around 0, -0, +INF, -INF
    {
        static const float xval[] = { 0.f, -0.f, c_INF,  -c_INF };
        static const float cval[] = { 0.f, -0.f, c_INF, c_Q_NAN };

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            r = XMVectorSqrt(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
            c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: sqrt(%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }
#endif // !__clang__

    return ret;
}
HRESULT Test345(LogProxy* pLog)
{
    //XMVectorSqrtEst
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;
    XMVECTORF32 v = { {0, 1, 100, 10000} };
    XMVECTORF32 adder = { {.01f, 1.f, 100.2f, 10300.1f} };
    printi("%s: ", TestName);
    for (int i = 0; i < 100; i++) {
        XMVECTORF32 check = { {sqrtf(XMVectorGetX(v)), sqrtf(XMVectorGetY(v)),sqrtf(XMVectorGetZ(v)),sqrtf(XMVectorGetW(v))} };
        r = XMVectorSqrtEst(v);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("\n%s: %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%ld  ", c);
        }
        v.v = XMVectorAdd(v.v, adder);
    }
    printi("\n");

#ifndef __clang__
    // Test special cases around 0, -0, +INF, -INF
    {
        static const float xval[] = { 0.f, -0.f, c_INF,  -c_INF };

        // The VMX128 implementation of the sqrt estimate doesn't handle -c_INF consistent with the other implementations
#if defined(_XENON) && !defined(_XM_NO_INTRINSICS_)
        static const float cval[] = { 0.f, -0.f, c_INF, -c_INF };
#else
        static const float cval[] = { 0.f, -0.f, c_INF, c_Q_NAN };
#endif

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            r = XMVectorSqrtEst(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
            c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: sqrt(%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }
#endif // !__clang__

    return ret;
}
HRESULT Test346(LogProxy* pLog)
{
    //XMVectorSubtract
    //XMVECTOR operator -=, -
    COMPARISON c = EXACT;
    HRESULT ret = S_OK;
    static const XMVECTORF32 f[] = {
        {0,0,0,0},  {0,0,0,0},  {0,0,0,0},
        {-.1875,.25,89.25,998},  {.125,.25,-.75,1000},  {-.3125,0,90,-2},
        {.6f,.8f,1,1.2f},       {.1f,.2f,.3f,.4f},  {.5f,.6f,.7f,.8f}
    };
    for (size_t i = 0; i < std::size(f); i += 3) {
        XMVECTOR v1 = f[i];
        XMVECTOR v2 = f[i + 1];
        XMVECTOR check = f[i + 2];
        XMVECTOR r = XMVectorSubtract(v1, v2);
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        XMVECTOR r2 = v1 - v2;
        XMVECTOR r3 = v1;
        r3 -= v2;
#endif
        COMPARISON temp = CompareXMVECTOR(r, check, 4);
        printi("%s: %d\n", TestName, temp);
        c = std::max(c, temp);
        if (temp > WITHINEPSILON) {
            printe("%f %f %f %f - %f %f %f %f = %f %f %f %f... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        temp = CompareXMVECTOR(r2, check, 4);
        printi("%s: (-) %d\n", TestName, temp);
        c = std::max(c, temp);
        if (temp > WITHINEPSILON) {
            printe("%f %f %f %f - %f %f %f %f = %f %f %f %f... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r2), XMVectorGetY(r2), XMVectorGetZ(r2), XMVectorGetW(r2),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        temp = CompareXMVECTOR(r3, check, 4);
        printi("%s: (-=) %d\n", TestName, temp);
        c = std::max(c, temp);
        if (temp > WITHINEPSILON) {
            printe("%f %f %f %f - %f %f %f %f = %f %f %f %f... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r3), XMVectorGetY(r3), XMVectorGetZ(r3), XMVectorGetW(r3),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
#endif
    }
    return ret;
}
HRESULT Test347(LogProxy* pLog)
{
    //XMVectorSubtractAngles
    static const XMVECTORF32 v[] = {
        {0,0,0,0},{0,0,0,0}, {0,0,0,0},
        {Pi + .1f,Pi - .1f,Pi / 2.f,.1f}, {0,0,0,0},{-Pi + .1f,Pi - .1f,Pi / 2.f,.1f},
        {-Pi + .1f,-Pi - .1f,-Pi / 2.f,-.1f}, {0,0,0,0},{-Pi + .1f,Pi - .1f,-Pi / 2.f,-.1f},
        {2 * Pi - .5f, 2 * Pi - 1, 2 * Pi - 2, -1 * Pi + 3},{-.5f,-1,-2,+1},{0,0,0,-Pi + 2},
        {.5f,1,2,-1},{Pi + .6f, -2 * Pi + 1, 2 * Pi - 2, -Pi - 3},{Pi - .1f,0,-2 * Pi + 4,-Pi + 2}
    };
    COMPARISON c;
    HRESULT ret = S_OK;
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    for (int k = 0; k < countof(v); k += 3) {
        v1 = v[k]; v2 = v[k + 1]; check = v[k + 2];
        XMVECTOR r = XMVectorSubtractAngles(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test348(LogProxy* pLog)
{
    //XMVectorTan
    XMVECTORF32 v = { {0,.3f,.6f,.9f} };
    XMVECTORF32 adder = { {.03f,.03f,.03f,.03f} };
    HRESULT ret = S_OK;
    for (int i = 0; i < 10; i++) {
        COMPARISON c = EXACT, maxc = EXACT;
        XMVECTORF32 check = { {tanf(XMVectorGetX(v)),tanf(XMVectorGetY(v)),tanf(XMVectorGetZ(v)),tanf(XMVectorGetW(v))} };
        XMVECTOR r = XMVectorTan(v);
        for (int j = 0; j < 4; j++) {
            if ((XMVectorGetByIndex(r, j) > 1000) && (XMVectorGetByIndex(check, j) > 1000)) {
                c = EXACT;
            }
            else {
                c = Compare(XMVectorGetByIndex(r, j), XMVectorGetByIndex(check, j));
            }
            maxc = std::max(maxc, c);
        }
        if (maxc > WITHIN4096) {
            printe("%s: %f %f %f %f   (%d)\n %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        v.v = XMVectorAdd(v.v, adder);
    }

    for (float f = UC_START; f <= UC_END; f += UC_STEP) {
        // tengent blows up at Pi/2 frequency points, which need to be skipped
        if (Compare(f, Pi / 2.f) <= WITHIN4096
            || Compare(f, -Pi / 2.f) <= WITHIN4096
            || Compare(f, (3 * Pi) / 2.f) <= WITHIN4096
            || Compare(f, -(3 * Pi) / 2.f) <= WITHIN4096)
            continue;

        XMVECTORF32 v = { {f,f,f,f} };
        XMVECTOR r = XMVectorTan(v);
        XMVECTOR check = XMVectorReplicate(tanf(f));
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test349(LogProxy* pLog)
{
    //XMVectorTanEst
    XMVECTORF32 v = { {0,.4f,.8f,1.2f} };
    XMVECTORF32 adder = { {.04f,.04f,.04f,.04f} };
    XMVECTOR r;
    HRESULT ret = S_OK;
    for (int i = 0; i < 20; i++) {
        COMPARISON c, maxc = EXACT;
        XMVECTORF32 check = { {tanf(XMVectorGetX(v)),tanf(XMVectorGetY(v)),tanf(XMVectorGetZ(v)),tanf(XMVectorGetW(v))} };
        r = XMVectorTanEst(v);
        for (int j = 0; j < 4; j++) {
            if ((XMVectorGetByIndex(r, j) > 1000) && (XMVectorGetByIndex(check, j) > 1000)) {
                c = EXACT;
            }
            else {
                c = Compare(XMVectorGetByIndex(r, j), XMVectorGetByIndex(check, j));
            }
            maxc = std::max(maxc, c);
        }
        if (maxc > WITHIN1_16) {
            printe("%s: %f %f %f %f   (%d)\n %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), maxc, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, maxc);
        }
        v.v = XMVectorAdd(v.v, adder);
    }

    for (float f = UC_START; f <= UC_END; f += UC_STEP) {
        // tangent blows up at Pi/2 frequency points, which need to be skipped
        if (Compare(f, Pi / 2.f) <= WITHIN4096
            || Compare(f, -Pi / 2.f) <= WITHIN4096
            || Compare(f, (3 * Pi) / 2.f) <= WITHIN4096
            || Compare(f, -(3 * Pi) / 2.f) <= WITHIN4096)
            continue;

        XMVECTORF32 v = { {f,f,f,f} };
        r = XMVectorTanEst(v);
        XMVECTOR check = XMVectorReplicate(tanf(f));
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN1_16)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test350(LogProxy* pLog)
{
    //XMVectorTanH
    HRESULT ret = S_OK;
    XMVECTORF32 v = { {-120, -4, 0, 4} };
    XMVECTORF32 adder = { {116.f / 26.f, 4.f / 26.f, 4.f / 26.f, 116.f / 26.f} };
    for (float vy = XMVectorGetY(v); vy < 0; v.v = XMVectorAdd(v.v, adder)) {
        float vx = XMVectorGetX(v); vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        XMVECTORF32 check = { {tanhf(vx),tanhf(vy),tanhf(vz),tanhf(vw)} };
        XMVECTOR r = XMVectorTanH(v);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %.2f: %f ... %f    %.2f: %f ... %f\n%.2f: %f ... %f   %.2f: %f ... %f\n",
                TestName, XMVectorGetX(v), XMVectorGetX(r), XMVectorGetX(check), XMVectorGetY(v), XMVectorGetY(r), XMVectorGetY(check), XMVectorGetZ(v), XMVectorGetZ(r), XMVectorGetZ(check), XMVectorGetW(v), XMVectorGetW(r), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    // Range for hyperbolic tangent is [-INF,+INF]
    for (float f = -4 * Pi; f <= 4 * Pi; f += UC_STEP) {
        XMVECTORF32 v = { {f,f,f,f} };
        XMVECTOR r = XMVectorTanH(v);
        XMVECTOR check = XMVectorReplicate(tanhf(f));
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096)
        {
            printe("%s: (%f) = %f %f %f %f...%f %f %f %f %d\n", TestName, f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

inline float RintF(float x)
{
    return float(int(x));
}

HRESULT Test352(LogProxy* pLog)
{
    //XMVectorTruncate
    HRESULT ret = S_OK;

    static const XMVECTORF32 f[] = {
        {0,0,0,0},  {0,0,0,0},
        {-2, -1.99f, -1.50f, -1.01f}, {-2,-1,-1,-1},
        {-1, -0.99f, -0.50f, -0.01f}, {-1,-0.f,-0.f,-0.f},
        { 0 ,  .99f,   .50f,   .01f}, { 0, 0, 0, 0},
        {40 ,40.99f, 40.50f, 40.01f}, {40,40,40,40},
        {-0.f,-0.f,-0.f,-0.f},  {0,0,0,0},
        {8388608.f, 8388609.f, 8388607.f, 8388607.5f }, {8388608.f, 8388609.f, 8388607.f, 8388607.f },
        {-8388608.f, -8388609.f, -8388607.f, -8388607.5f }, {-8388608.f, -8388609.f, -8388607.f, -8388607.f },
        {11387829.f, 11387829.f, 11387829.f, 11387829.f}, {11387829.f, 11387829.f, 11387829.f, 11387829.f},
        {-8419647.f, -8419647.f, -8419647.f, -8419647.f}, {-8419647.f, -8419647.f, -8419647.f, -8419647.f},
        {2147483520.f, 2147483520.f, 2147483520.f, 2147483520.f }, {2147483520.f, 2147483520.f, 2147483520.f, 2147483520.f },
        {-2147483520.f, -2147483520.f, -2147483520.f, -2147483520.f }, {-2147483520.f, -2147483520.f, -2147483520.f, -2147483520.f },
        {8000000000.0f, 8000000000.0f, 8000000000.0f, 8000000000.0f }, {8000000000.0f, 8000000000.0f, 8000000000.0f, 8000000000.0f },
        {   c_INF,   c_INF,   c_INF,   c_INF}, {   c_INF,   c_INF,   c_INF,   c_INF},
        {  -c_INF,  -c_INF,  -c_INF,  -c_INF}, {  -c_INF,  -c_INF,  -c_INF,  -c_INF},
        { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN}, { c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN},
    };

    for (size_t i = 0; i < std::size(f); i += 2) {
        XMVECTOR v1 = f[i];
        XMVECTOR check = f[i + 1];
        XMVECTOR r = XMVectorTruncate(v1);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%s: %f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n", TestName,
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check));
            ret = MATH_FAIL;
        }
    }

    for (int j = -256; j <= 256; ++j)
    {
        float num = static_cast<float>(j);
        XMVECTORF32 v1 = { num, num + 0.1f, num + 0.5f, num + 0.9f };
        XMVECTORF32 chk = { RintF(num), RintF(num + 0.1f), RintF(num + 0.5f), RintF(num + 0.9f) };
        XMVECTOR r = XMVectorTruncate(v1);
        COMPARISON c = CompareXMVECTOR(r, chk, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
            ret = MATH_FAIL;
        }
    }

    for (float num = -1.f; num <= 1.f; num += 0.01f)
    {
        XMVECTORF32 v1 = { num, num + 0.1f, num + 0.5f, num + 0.9f };
        XMVECTORF32 chk = { RintF(num), RintF(num + 0.1f), RintF(num + 0.5f), RintF(num + 0.9f) };
        XMVECTOR r = XMVectorTruncate(v1);
        COMPARISON c = CompareXMVECTOR(r, chk, 4);
        printi("%s: %d\n", TestName, c);
        if (c > EXACT) {
            printe("%f %f %f %f (%d): %f %f %f %f ... %f %f %f %f\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), c, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test353(LogProxy* pLog)
{
    //XMVectorZero
    XMVECTOR z = XMVectorZero();
    if (XMVectorGetX(z) != 0 || XMVectorGetY(z) != 0 || XMVectorGetZ(z) != 0 || XMVectorGetW(z) != 0)
    {
        printe("%s: XMVectorZero failed. %f %f %f %f\n", TestName,
            XMVectorGetX(z), XMVectorGetY(z), XMVectorGetZ(z), XMVectorGetW(z));
        return MATH_FAIL;
    }
    return S_OK;
}

HRESULT Test440(LogProxy* pLog)
{
    //XMVectorEqualInt
    HRESULT ret = S_OK;
    for (int k = 0; k < 16; k++) {
        XMVECTORF32 v1 = {}, v2 = {};
        XMVECTORF32 check = {};
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, XM_RAND() + (XM_RAND() << 14) + (XM_RAND() << 28), i);
            v2.v = XMVectorSetIntByIndex(v2, (k & (1 << i)) ? XMVectorGetIntByIndex(v1, i) : XMVectorGetIntByIndex(v1, i) + 1 + XM_RAND(), i);
            check.v = XMVectorSetIntByIndex(check, (k & (1 << i)) ? 0xffffffff : 0, i);
        }
        XMVECTOR r = XMVectorEqualInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: (%d), %x %x = %x ... %x\n",
                    TestName, i, XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i), XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}

HRESULT Test441(LogProxy* pLog)
{
    //XMVectorNotEqualInt
    HRESULT ret = S_OK;
    for (int k = 0; k < 16; k++) {
        XMVECTORF32 v1 = {}, v2 = {};
        XMVECTORF32 check = {};
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, XM_RAND() + (XM_RAND() << 14) + (XM_RAND() << 28), i);
            v2.v = XMVectorSetIntByIndex(v2, (k & (1 << i)) ? XMVectorGetIntByIndex(v1, i) : XMVectorGetIntByIndex(v1, i) + 1 + XM_RAND(), i);
            check.v = XMVectorSetIntByIndex(check, (k & (1 << i)) ? 0 : 0xffffffff, i);
        }
        XMVECTOR r = XMVectorNotEqualInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: (%d), %x %x = %x ... %x\n",
                    TestName, i, XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i),
                    XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}


HRESULT Test451(LogProxy* pLog)
{
    //XMVectorSetInt
    static const uint32_t c[] = { 0,0xffffffff,0x80000000, 1234567890,5,13245,0x7fffffff,0x58270000 };
    XMVECTOR r;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        r = XMVectorSetInt(c[(k & 7)], c[((k + 1) & 7)], c[((k + 2) & 7)], c[((k + 3) & 7)]);
        for (int i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != c[((k + i) & 7)]) {
                printe("%s: %d %d: %x ... %x\n", TestName, k, i, XMVectorGetIntByIndex(r, i), c[((k + i) & 7)]);
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test452(LogProxy* pLog)
{
    //XMVectorTrueInt
    XMVECTOR v = XMVectorZero();
    v = XMVectorTrueInt();
    HRESULT ret = S_OK;
    for (int i = 0; i < 4; i++) {
        if (XMVectorGetIntByIndex(v, i) != 0xffffffff) {
            printe("%s: %d: %x ... 0xffffffff\n", TestName, i, XMVectorGetIntByIndex(v, i));
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test453(LogProxy* pLog)
{
    //XMVectorFalseInt
    XMVECTOR v = XMVectorTrueInt();
    v = XMVectorFalseInt();
    HRESULT ret = S_OK;
    for (int i = 0; i < 4; i++) {
        if (XMVectorGetIntByIndex(v, i) != 0) {
            printe("%s: %d: %x ... 0\n", TestName, i, XMVectorGetIntByIndex(v, i));
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test454(LogProxy* pLog)
{
    //XMVectorEqualInt
    static const uint32_t c[] = { 0,0xffffffff,0x80000000, 1234567890,5,13245,0x7fffffff,0x58270000 };
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 80; k++) {
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, c[(k + i) & 7], i);
            v2.v = XMVectorSetIntByIndex(v2, c[XM_RAND() & 7], i);
            check.v = XMVectorSetIntByIndex(check, (XMVectorGetIntByIndex(v1, i) == XMVectorGetIntByIndex(v2, i)) ? 0xffffffff : 0, i);
        }
        XMVECTOR r = XMVectorEqualInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: %d %d: %x, %x = %x ... %x\n", TestName, k, i, XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i),
                    XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test455(LogProxy* pLog)
{
    //XMVectorNotEqualInt
    static const uint32_t c[] = { 0,0xffffffff,0x80000000, 1234567890,5,13245,0x7fffffff,0x58270000 };
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 80; k++) {
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, c[(k + i) & 7], i);
            v2.v = XMVectorSetIntByIndex(v2, c[XM_RAND() & 7], i);
            check.v = XMVectorSetIntByIndex(check, (XMVectorGetIntByIndex(v1, i) != XMVectorGetIntByIndex(v2, i)) ? 0xffffffff : 0, i);
        }
        XMVECTOR r = XMVectorNotEqualInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: %d %d: %x, %x = %x ... %x\n", TestName, k, i, XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i),
                    XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test456(LogProxy* pLog)
{
    //XMVectorAndInt
    static const uint32_t c[] = { 0,0xffffffff,0x80000000, 1234567890,5,13245,0x7fffffff,0x58270000 };
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 80; k++) {
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, c[(k + i) & 7], i);
            v2.v = XMVectorSetIntByIndex(v2, c[XM_RAND() & 7], i);
            check.v = XMVectorSetIntByIndex(check, XMVectorGetIntByIndex(v1, i) & XMVectorGetIntByIndex(v2, i), i);
        }
        XMVECTOR r = XMVectorAndInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: %d %d: %x, %x = %x ... %x\n", TestName, k, i, XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i),
                    XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test457(LogProxy* pLog)
{
    //XMVectorAndCInt
    static const uint32_t c[] = { 0,0xffffffff,0x80000000, 1234567890,5,13245,0x7fffffff,0x58270000 };
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 8; k++) {
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, (k < countof(c)) ? c[k] : XM_RAND() + (XM_RAND() << 15) + (XM_RAND() << 20), i);
            v2.v = XMVectorSetIntByIndex(v2, (k < countof(c) * 2) ? c[(k * 3) % countof(c)] : XM_RAND() + (XM_RAND() << 15) + (XM_RAND() << 20), i);
            check.v = XMVectorSetIntByIndex(check, XMVectorGetIntByIndex(v1, i) & (~XMVectorGetIntByIndex(v2, i)), i);
        }
        XMVECTOR r = XMVectorAndCInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: %d %d: %x, %x = %x ... %x\n", TestName, k, i, XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i),
                    XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test458(LogProxy* pLog)
{
    //XMVectorOrInt
    static const uint32_t c[] = { 0,0xffffffff,0x80000000, 1234567890,5,13245,0x7fffffff,0x58270000 };
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 80; k++) {
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, c[(k + i) & 7], i);
            v2.v = XMVectorSetIntByIndex(v2, c[XM_RAND() & 7], i);
            check.v = XMVectorSetIntByIndex(check, XMVectorGetIntByIndex(v1, i) | XMVectorGetIntByIndex(v2, i), i);
        }
        XMVECTOR r = XMVectorOrInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: %d %d: %x, %x = %x ... %x\n", TestName, k, i, XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i),
                    XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test459(LogProxy* pLog)
{
    //XMVectorNorInt
    static const uint32_t c[] = { 0,0xffffffff,0x80000000, 1234567890,5,13245,0x7fffffff,0x58270000 };
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 80; k++) {
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, c[(k + i) & 7], i);
            v2.v = XMVectorSetIntByIndex(v2, c[XM_RAND() & 7], i);
            check.v = XMVectorSetIntByIndex(check, (~XMVectorGetIntByIndex(v1, i)) & (~XMVectorGetIntByIndex(v2, i)), i);
        }
        XMVECTOR r = XMVectorNorInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: %d %d: %x, %x = %x ... %x\n", TestName, k, i,
                    XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i), XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test460(LogProxy* pLog)
{
    //XMVectorXorInt
    static const uint32_t c[] = { 0,0xffffffff,0x80000000, 1234567890,5,3422613245,0x7fffffff,0x58270000 };
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 80; k++) {
        int i;
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetIntByIndex(v1, c[(k + i) & 7], i);
            v2.v = XMVectorSetIntByIndex(v2, c[XM_RAND() & 7], i);
            check.v = XMVectorSetIntByIndex(check, XMVectorGetIntByIndex(v1, i) ^ XMVectorGetIntByIndex(v2, i), i);
        }
        XMVECTOR r = XMVectorXorInt(v1, v2);
        for (i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(r, i) != XMVectorGetIntByIndex(check, i)) {
                printe("%s: %d %d: %x, %x = %x ... %x\n", TestName, k, i, XMVectorGetIntByIndex(v1, i), XMVectorGetIntByIndex(v2, i),
                    XMVectorGetIntByIndex(r, i), XMVectorGetIntByIndex(check, i));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}

HRESULT Test467(LogProxy* pLog)
{
    //XMVectorEqualR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (3 * 3 * 3 * 3); i++) {
        float v1x;
        float v1y;
        float v1z;
        float v1w;
        float v2x;
        float v2y;
        float v2z;
        float v2w;
        v1x = 2.0f;
        v2x = 3.0f - static_cast<float>(i % 3);  // 0->3, 1->2, 2->1
        v1y = 3.0f;
        v2y = 4.0f - static_cast<float>((i / 3) % 3);  // 0->4, 1->3, 2->2
        v1z = 4.0f;
        v2z = 5.0f - static_cast<float>((i / 9) % 3);  // 0->5, 1->4, 2->3
        v1w = 5.0f;
        v2w = 6.0f - static_cast<float>((i / 27) % 3); // 0->6, 1->5, 2->4
        uint32_t crcheck = 0;
        if (v1x == v2x && v1y == v2y && v1z == v2z && v1w == v2w) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x != v2x && v1y != v2y && v1z != v2z && v1w != v2w) crcheck |= XM_CRMASK_CR6FALSE;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t cr;
        XMVECTOR r = XMVectorEqualR(&cr, v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = CR %x ... %x\n", TestName, v1x, v1y, v1z, v1w, v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
        XMVECTOR check = XMVectorEqual(v1, v2);
        if ((XMVectorGetIntX(r) != XMVectorGetIntX(check)) ||
            (XMVectorGetIntY(r) != XMVectorGetIntY(check)) ||
            (XMVectorGetIntZ(r) != XMVectorGetIntZ(check)) ||
            (XMVectorGetIntW(r) != XMVectorGetIntW(check))) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x %x %x %x ... %x %x %x %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w,
                XMVectorGetIntX(r), XMVectorGetIntY(r), XMVectorGetIntZ(r), XMVectorGetIntW(r),
                XMVectorGetIntX(check), XMVectorGetIntY(check), XMVectorGetIntZ(check), XMVectorGetIntW(check));
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test468(LogProxy* pLog)
{
    //XMVectorEqualIntR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (4 * 4 * 4 * 4); i++) {
        static const IntPairs_t PairsX[4] = {
            {2,3},{2,2},{2,1},{-2,-1}
        };
        static const IntPairs_t PairsY[4] = {
            {3,4},{3,3},{3,2},{-3,-2}
        };
        static const IntPairs_t PairsZ[4] = {
            {4,5},{4,4},{4,3},{-4,-3}
        };
        static const IntPairs_t PairsW[4] = {
            {5,6},{5,5},{5,4},{-1,-1}
        };
        int v1x;
        int v1y;
        int v1z;
        int v1w;
        int v2x;
        int v2y;
        int v2z;
        int v2w;
        const IntPairs_t* pEntry = &PairsX[i & 3];
        v1x = pEntry->x;
        v2x = pEntry->y;
        pEntry = &PairsY[(i >> 2) & 3];
        v1y = pEntry->x;
        v2y = pEntry->y;
        pEntry = &PairsZ[(i >> 4) & 3];
        v1z = pEntry->x;
        v2z = pEntry->y;
        pEntry = &PairsW[(i >> 6) & 3];
        v1w = pEntry->x;
        v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSetInt(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSetInt(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x == v2x && v1y == v2y && v1z == v2z && v1w == v2w) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x != v2x && v1y != v2y && v1z != v2z && v1w != v2w) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr;
        XMVECTOR r = XMVectorEqualIntR(&cr, v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %x %x %x %x, %x %x %x %x = %x ... %x\n", TestName,
                static_cast<unsigned int>(v1x), static_cast<unsigned int>(v1y), static_cast<unsigned int>(v1z), static_cast<unsigned int>(v1w),
                static_cast<unsigned int>(v2x), static_cast<unsigned int>(v2y), static_cast<unsigned int>(v2z), static_cast<unsigned int>(v2w),
                cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
        XMVECTOR check = XMVectorEqualInt(v1, v2);
        if ((XMVectorGetIntX(r) != XMVectorGetIntX(check)) ||
            (XMVectorGetIntY(r) != XMVectorGetIntY(check)) ||
            (XMVectorGetIntZ(r) != XMVectorGetIntZ(check)) ||
            (XMVectorGetIntW(r) != XMVectorGetIntW(check))) {
            printe("%s: %x %x %x %x, %x %x %x %x = %x %x %x %x ... %x %x %x %x\n", TestName,
                static_cast<unsigned int>(v1x), static_cast<unsigned int>(v1y), static_cast<unsigned int>(v1z), static_cast<unsigned int>(v1w),
                static_cast<unsigned int>(v2x), static_cast<unsigned int>(v2y), static_cast<unsigned int>(v2z), static_cast<unsigned int>(v2w),
                XMVectorGetIntX(r), XMVectorGetIntY(r), XMVectorGetIntZ(r), XMVectorGetIntW(r),
                XMVectorGetIntX(check), XMVectorGetIntY(check), XMVectorGetIntZ(check), XMVectorGetIntW(check));
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test469(LogProxy* pLog)
{
    //XMVectorGreaterR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (3 * 3 * 3 * 3); i++) {
        float v1x;
        float v1y;
        float v1z;
        float v1w;
        float v2x;
        float v2y;
        float v2z;
        float v2w;
        v1x = 2.0f;
        v2x = 3.0f - static_cast<float>(i % 3);       // 0->3, 1->2, 2->1
        v1y = 3.0f;
        v2y = 4.0f - static_cast<float>((i / 3) % 3);   // 0->4, 1->3, 2->2
        v1z = 4.0f;
        v2z = 5.0f - static_cast<float>((i / 9) % 3);   // 0->5, 1->4, 2->3
        v1w = 5.0f;
        v2w = 6.0f - static_cast<float>((i / 27) % 3);  // 0->6, 1->5, 2->4
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x > v2x&& v1y > v2y&& v1z > v2z&& v1w > v2w) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x <= v2x && v1y <= v2y && v1z <= v2z && v1w <= v2w) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr;
        XMVECTOR r = XMVectorGreaterR(&cr, v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = CR %x ... %x\n", TestName, v1x, v1y, v1z, v1w, v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
        XMVECTOR check = XMVectorGreater(v1, v2);
        if ((XMVectorGetIntX(r) != XMVectorGetIntX(check)) ||
            (XMVectorGetIntY(r) != XMVectorGetIntY(check)) ||
            (XMVectorGetIntZ(r) != XMVectorGetIntZ(check)) ||
            (XMVectorGetIntW(r) != XMVectorGetIntW(check))) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x %x %x %x ... %x %x %x %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w,
                XMVectorGetIntX(r), XMVectorGetIntY(r), XMVectorGetIntZ(r), XMVectorGetIntW(r),
                XMVectorGetIntX(check), XMVectorGetIntY(check), XMVectorGetIntZ(check), XMVectorGetIntW(check));
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test470(LogProxy* pLog)
{
    //XMVectorGreaterOrEqualR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (3 * 3 * 3 * 3); i++) {
        float v1x;
        float v1y;
        float v1z;
        float v1w;
        float v2x;
        float v2y;
        float v2z;
        float v2w;
        v1x = 2.0f;
        v2x = 3.0f - static_cast<float>(i % 3);       // 0->3, 1->2, 2->1
        v1y = 3.0f;
        v2y = 4.0f - static_cast<float>((i / 3) % 3);   // 0->4, 1->3, 2->2
        v1z = 4.0f;
        v2z = 5.0f - static_cast<float>((i / 9) % 3);   // 0->5, 1->4, 2->3
        v1w = 5.0f;
        v2w = 6.0f - static_cast<float>((i / 27) % 3);  // 0->6, 1->5, 2->4
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x >= v2x && v1y >= v2y && v1z >= v2z && v1w >= v2w) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x < v2x && v1y < v2y && v1z < v2z && v1w < v2w) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr;
        XMVECTOR r = XMVectorGreaterOrEqualR(&cr, v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = CR %x ... %x\n", TestName, v1x, v1y, v1z, v1w, v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
        XMVECTOR check = XMVectorGreaterOrEqual(v1, v2);
        if ((XMVectorGetIntX(r) != XMVectorGetIntX(check)) ||
            (XMVectorGetIntY(r) != XMVectorGetIntY(check)) ||
            (XMVectorGetIntZ(r) != XMVectorGetIntZ(check)) ||
            (XMVectorGetIntW(r) != XMVectorGetIntW(check))) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x %x %x %x ... %x %x %x %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w,
                XMVectorGetIntX(r), XMVectorGetIntY(r), XMVectorGetIntZ(r), XMVectorGetIntW(r),
                XMVectorGetIntX(check), XMVectorGetIntY(check), XMVectorGetIntZ(check), XMVectorGetIntW(check));
            ret = MATH_FAIL;
        }

    }
    return ret;
}
HRESULT Test471(LogProxy* pLog)
{
    //XMVectorInBounds
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (8 * 8 * 8); i++) {
        static const FloatPairs_t TableX[8] = {
            {2,3},{2,1},{2,-3},{2,-1},{-2,3},{-2,1},{-2,-3},{-2,-1}
        };
        static const FloatPairs_t TableY[8] = {
            {3,4},{3,2},{3,-4},{3,-2},{-3,4},{-3,2},{-3,-4},{-3,-2}
        };
        static const FloatPairs_t TableZ[8] = {
            {4,5},{4,3},{4,-5},{4,-3},{-4,5},{-4,3},{-4,-5},{-4,-3}
        };
        static const FloatPairs_t TableW[8] = {
            {5,6},{5,4},{5,-6},{5,-4},{-5,6},{-5,4},{-5,-6},{-5,-4}
        };
        const FloatPairs_t* pWork = &TableX[i & 7];
        float v1x = pWork->x;
        float v2x = pWork->y;
        pWork = &TableY[(i >> 3) & 7];
        float v1y = pWork->x;
        float v2y = pWork->y;
        pWork = &TableZ[(i >> 6) & 7];
        float v1z = pWork->x;
        float v2z = pWork->y;
        pWork = &TableW[XM_RAND() & 7];
        float v1w = pWork->x;
        float v2w = pWork->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        int checkx = ((v1x <= v2x) && (v1x >= -v2x)) ? 0xFFFFFFFF : 0;
        int checky = ((v1y <= v2y) && (v1y >= -v2y)) ? 0xFFFFFFFF : 0;
        int checkz = ((v1z <= v2z) && (v1z >= -v2z)) ? 0xFFFFFFFF : 0;
        int checkw = ((v1w <= v2w) && (v1w >= -v2w)) ? 0xFFFFFFFF : 0;
        XMVECTOR check = XMVectorSetInt(checkx, checky, checkz, checkw);
        XMVECTOR r = XMVectorInBounds(v1, v2);
        for (int j = 0; j < 4; j++) {
            if (XMVectorGetIntByIndex(r, j) != XMVectorGetIntByIndex(check, j)) {
                printe("%s: %f, %f: %x ... %x\n", TestName, XMVectorGetByIndex(v1, j), XMVectorGetByIndex(v2, j),
                    XMVectorGetIntByIndex(r, j), XMVectorGetIntByIndex(check, j));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test472(LogProxy* pLog)
{
    //XMVectorInBoundsR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (8 * 8 * 8); i++) {
        static const FloatPairs_t TableX[8] = {
            {2,3},{2,1},{2,-3},{2,-1},{-2,3},{-2,1},{-2,-3},{-2,-1}
        };
        static const FloatPairs_t TableY[8] = {
            {3,4},{3,2},{3,-4},{3,-2},{-3,4},{-3,2},{-3,-4},{-3,-2}
        };
        static const FloatPairs_t TableZ[8] = {
            {4,5},{4,3},{4,-5},{4,-3},{-4,5},{-4,3},{-4,-5},{-4,-3}
        };
        static const FloatPairs_t TableW[8] = {
            {5,6},{5,4},{5,-6},{5,-4},{-5,6},{-5,4},{-5,-6},{-5,-4}
        };
        const FloatPairs_t* pWork = &TableX[i & 7];
        float v1x = pWork->x;
        float v2x = pWork->y;
        pWork = &TableY[(i >> 3) & 7];
        float v1y = pWork->x;
        float v2y = pWork->y;
        pWork = &TableZ[(i >> 6) & 7];
        float v1z = pWork->x;
        float v2z = pWork->y;
        pWork = &TableW[XM_RAND() & 7];
        float v1w = pWork->x;
        float v2w = pWork->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        int checkx = ((v1x <= v2x) && (v1x >= -v2x)) ? 0xFFFFFFFF : 0;
        int checky = ((v1y <= v2y) && (v1y >= -v2y)) ? 0xFFFFFFFF : 0;
        int checkz = ((v1z <= v2z) && (v1z >= -v2z)) ? 0xFFFFFFFF : 0;
        int checkw = ((v1w <= v2w) && (v1w >= -v2w)) ? 0xFFFFFFFF : 0;
        XMVECTOR check = XMVectorSetInt(checkx, checky, checkz, checkw);
        uint32_t crcheck = 0;
        if (checkx & checky & checkz & checkw) {
            crcheck = XM_CRMASK_CR6BOUNDS;
        }
        uint32_t cr;
        XMVECTOR r = XMVectorInBoundsR(&cr, v1, v2);
        for (int j = 0; j < 4; j++) {
            if (XMVectorGetIntByIndex(r, j) != XMVectorGetIntByIndex(check, j)) {
                printe("%s: %f, %f: %x ... %x\n", TestName, XMVectorGetByIndex(v1, j), XMVectorGetByIndex(v2, j),
                    XMVectorGetIntByIndex(r, j), XMVectorGetIntByIndex(check, j));
                ret = MATH_FAIL;
            }
        }
        if ((cr & XM_CRMASK_CR6BOUNDS) != crcheck) {
            printe("%f %f %f %f, %f %f %f %f = %x ... %x\n",
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6BOUNDS, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

template<uint32_t SwizzleX, uint32_t SwizzleY, uint32_t SwizzleZ, uint32_t SwizzleW> HRESULT STest(LogProxy* pLog)
{
    XMVECTORF32 v = { 0, 1, 2, 3 };
    XMVECTORF32 chk = { (float)SwizzleX, (float)SwizzleY, (float)SwizzleZ, (float)SwizzleW };

    HRESULT ret = S_OK;

    // Test template form
    XMVECTOR r = XMVectorSwizzle<SwizzleX, SwizzleY, SwizzleZ, SwizzleW>(v);
    if (CompareXMVECTOR(r, chk, 4) != EXACT)
    {
        printe("%s: (T) %u %u %u %u = %f %f %f %f ... %f %f %f %f\n",
            TestName, SwizzleX, SwizzleY, SwizzleZ, SwizzleW,
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
        ret = MATH_FAIL;
    }

    // Test function form
    r = XMVectorSwizzle(v, SwizzleX, SwizzleY, SwizzleZ, SwizzleW);
    if (CompareXMVECTOR(r, chk, 4) != EXACT)
    {
        printe("%s: (F) %u %u %u %u = %f %f %f %f ... %f %f %f %f\n",
            TestName, SwizzleX, SwizzleY, SwizzleZ, SwizzleW,
            XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
            XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk));
        ret = MATH_FAIL;
    }

    return ret;
}

HRESULT Test493(LogProxy* pLog)
{
    //XMVectorSwizzle
    HRESULT ret = S_OK;
    {
        static const XMVECTORF32 v1mem = { 3,-4,5,6.1f };
        XMVECTOR v = v1mem;

#define SW(a,b,c,d) XMVectorSwizzle(v, a,b,c,d), XMVectorSet(XMVectorGetByIndex(v,a),XMVectorGetByIndex(v,b),XMVectorGetByIndex(v,c),XMVectorGetByIndex(v,d))
#define SW2(a,b) SW(a,b,0,0),SW(a,b,0,1),SW(a,b,0,2),SW(a,b,0,3),SW(a,b,1,0),SW(a,b,1,1),SW(a,b,1,2),SW(a,b,1,3),SW(a,b,2,0),SW(a,b,2,1),SW(a,b,2,2),SW(a,b,2,3),SW(a,b,3,0),SW(a,b,3,1),SW(a,b,3,2),SW(a,b,3,3)
        XMVECTOR r[] = {
            //        XMVectorSwizzle(v, 0,0,0,0)
                    SW2(0,0), SW2(0,1), SW2(0,2), SW2(0,3), SW2(1,0), SW2(1,1), SW2(1,2), SW2(1,3), SW2(2,0), SW2(2,1), SW2(2,2), SW2(2,3), SW2(3,0), SW2(3,1), SW2(3,2), SW2(3,3)
        };
#undef SW
#undef SW2

        for (int i = 0; i < countof(r); i += 2) {
            for (int j = 0; j < 4; j++) {
                if (XMVectorGetByIndex(r[i], j) != XMVectorGetByIndex(r[i + 1], j)) {
                    printe("%s: 3.0 -4.0 5.0 6.10, %d %d %d %d = %f %f %f %f ... %f %f %f %f", TestName, (i >> 7) & 3, (i >> 5) & 3, (i >> 3) & 3, (i >> 1) & 3,
                        XMVectorGetX(r[i]), XMVectorGetY(r[i]), XMVectorGetZ(r[i]), XMVectorGetW(r[i]),
                        XMVectorGetX(r[i + 1]), XMVectorGetY(r[i + 1]), XMVectorGetZ(r[i + 1]), XMVectorGetW(r[i + 1]));
                    ret = MATH_FAIL;
                }
            }
        }
    }

    HRESULT r = STest<0, 1, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<0, 0, 0, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<1, 1, 1, 1>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<2, 2, 2, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<3, 3, 3, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<0, 1, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<3, 2, 1, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<1, 0, 3, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<0, 1, 0, 1>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<2, 3, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<1, 0, 1, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<3, 2, 3, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<0, 1, 3, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<1, 0, 2, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<2, 3, 1, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<3, 2, 0, 1>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<3, 2, 1, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<0, 0, 2, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<1, 1, 3, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<0, 0, 1, 1>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<2, 2, 3, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<0, 2, 0, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<1, 3, 1, 3>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    r = STest<1, 2, 3, 0>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<2, 3, 0, 1>(pLog); if (FAILED(r)) ret = MATH_FAIL;
    r = STest<3, 0, 1, 2>(pLog); if (FAILED(r)) ret = MATH_FAIL;

    return ret;
}
HRESULT Test494(LogProxy* pLog)
{
    //XMVectorRotateLeft
    XMVECTORF32 v = { 0,1,2,3 };
    XMVECTOR r;
    XMVECTORF32 check[4] = { {0,1,2,3}, {1,2,3,0}, {2,3,0,1}, {3,0,1,2} };
    HRESULT ret = S_OK;

    for (uint32_t elements = 0; elements < 4; ++elements)
    {
        r = XMVectorRotateLeft(v, elements);
        if (XMVector4NotEqual(r, check[elements])) {
            printe("%s: %f %f %f %f, %u = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                elements,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[elements]), XMVectorGetY(check[elements]), XMVectorGetZ(check[elements]), XMVectorGetW(check[elements]));
            ret = MATH_FAIL;
        }
    }

    // Template form
    {
        r = XMVectorRotateLeft<0>(v);
        if (XMVector4NotEqual(r, check[0])) {
            printe("%s: %f %f %f %f, <0> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[0]), XMVectorGetY(check[0]), XMVectorGetZ(check[0]), XMVectorGetW(check[0]));
            ret = MATH_FAIL;
        }
        r = XMVectorRotateLeft<1>(v);
        if (XMVector4NotEqual(r, check[1])) {
            printe("%s: %f %f %f %f, <1> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[1]), XMVectorGetY(check[1]), XMVectorGetZ(check[1]), XMVectorGetW(check[1]));
            ret = MATH_FAIL;
        }
        r = XMVectorRotateLeft<2>(v);
        if (XMVector4NotEqual(r, check[2])) {
            printe("%s: %f %f %f %f, <2> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[2]), XMVectorGetY(check[2]), XMVectorGetZ(check[2]), XMVectorGetW(check[2]));
            ret = MATH_FAIL;
        }
        r = XMVectorRotateLeft<3>(v);
        if (XMVector4NotEqual(r, check[3])) {
            printe("%s: %f %f %f %f, <3> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[3]), XMVectorGetY(check[3]), XMVectorGetZ(check[3]), XMVectorGetW(check[3]));
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test495(LogProxy* pLog)
{
    //XMVectorRotateRight
    XMVECTORF32 v = { 0,1,2,3 };
    XMVECTOR r;
    XMVECTORF32 check[4] = { {0,1,2,3}, {3,0,1,2}, {2,3,0,1}, {1,2,3,0} };
    HRESULT ret = S_OK;

    for (uint32_t elements = 0; elements < 4; ++elements)
    {
        r = XMVectorRotateRight(v, elements);
        if (XMVector4NotEqual(r, check[elements])) {
            printe("%s: %f %f %f %f, %u = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                elements,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[elements]), XMVectorGetY(check[elements]), XMVectorGetZ(check[elements]), XMVectorGetW(check[elements]));
            ret = MATH_FAIL;
        }
    }

    // Template form
    {
        r = XMVectorRotateRight<0>(v);
        if (XMVector4NotEqual(r, check[0])) {
            printe("%s: %f %f %f %f, <0> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[0]), XMVectorGetY(check[0]), XMVectorGetZ(check[0]), XMVectorGetW(check[0]));
            ret = MATH_FAIL;
        }
        r = XMVectorRotateRight<1>(v);
        if (XMVector4NotEqual(r, check[1])) {
            printe("%s: %f %f %f %f, <1> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[1]), XMVectorGetY(check[1]), XMVectorGetZ(check[1]), XMVectorGetW(check[1]));
            ret = MATH_FAIL;
        }
        r = XMVectorRotateRight<2>(v);
        if (XMVector4NotEqual(r, check[2])) {
            printe("%s: %f %f %f %f, <2> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[2]), XMVectorGetY(check[2]), XMVectorGetZ(check[2]), XMVectorGetW(check[2]));
            ret = MATH_FAIL;
        }
        r = XMVectorRotateRight<3>(v);
        if (XMVector4NotEqual(r, check[3])) {
            printe("%s: %f %f %f %f, <3> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[3]), XMVectorGetY(check[3]), XMVectorGetZ(check[3]), XMVectorGetW(check[3]));
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test502(LogProxy* pLog)
{
    //XMVector3NormalizeEst
    int i, k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1 = {}, check = {};
    float rcplength;

    for (k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        rcplength = 1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1));
        for (i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check, XMVectorGetByIndex(v1, i) * rcplength, i);
        }

        XMVECTOR r = XMVector3NormalizeEst(v1);
        c = CompareXMVECTOR(r, check, 3);
        if (c > WITHINHUGEEPSILON) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    // Test special cases around 0, -0, +INF, -INF, c_Q_NAN
    {
        static const float xval[] = { 0.f,  -0.f, c_INF, -c_INF, c_Q_NAN };

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVector3NormalizeEst(v);
            XMVECTOR check = XMVectorSplatQNaN();
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: %f = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}


HRESULT Test503(LogProxy* pLog)
{
    //XMVector2NormalizeEst
    HRESULT ret = S_OK;
    int k;
    for (k = 0; k < 10; k++) {
        float v1x = GetRandomFloat16();
        float v1y = GetRandomFloat16();
        XMVECTORF32 v1;
        v1.f[0] = v1x;
        v1.f[1] = v1y;
        v1.f[2] = c_Q_NAN;
        v1.f[3] = c_Q_NAN;
        float rcplength = 1.0f / sqrtf(v1x * v1x + v1y * v1y);
        XMVECTORF32 check = {
            v1x * rcplength,
            v1y * rcplength,
            c_Q_NAN,
            c_Q_NAN
        };
        XMVECTOR r = XMVector2NormalizeEst(v1);
        COMPARISON c = CompareXMVECTOR(r, check, 2);
        if (c > WITHINHUGEEPSILON) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                v1.f[0], v1.f[1], v1.f[2], v1.f[3],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                check.f[0], check.f[1], check.f[2], check.f[3], c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    // Test special cases around 0, -0, +INF, -INF, c_Q_NAN
    {
        static const float xval[] = { 0.f,  -0.f, c_INF, -c_INF, c_Q_NAN };

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVector2NormalizeEst(v);
            XMVECTOR check = XMVectorSplatQNaN();
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: %f = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}

HRESULT Test504(LogProxy* pLog)
{
    //XMVectorSplatQNaN
    HRESULT ret = S_OK;
    XMVECTOR v = XMVectorSplatQNaN();
    XMVECTOR check = XMVectorSet(c_Q_NAN, c_Q_NAN, c_Q_NAN, c_Q_NAN);
    COMPARISON c = CompareXMVECTOR(v, check, 4);
    if (c != EXACT)
    {
        printe("%s: c_Q_NAN c_Q_NAN c_Q_NAN c_Q_NAN ... %f %f %f %f\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v));
        ret = MATH_FAIL;
    }
    return ret;
}

HRESULT Test505(LogProxy* pLog)
{
    //XMVectorSplatInfinity
    HRESULT ret = S_OK;
    XMVECTOR v = XMVectorSplatInfinity();
    XMVECTOR check = XMVectorSet(c_INF, c_INF, c_INF, c_INF);
    COMPARISON c = CompareXMVECTOR(v, check, 4);
    if (c != EXACT)
    {
        printe("%s: c_INF c_INF c_INF c_INF ... %f %f %f %f\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v));
        ret = MATH_FAIL;
    }
    return ret;
}

HRESULT Test506(LogProxy* pLog)
{
    //XMVectorPow and XMVectorPowEst
    int k;
    HRESULT ret = S_OK;

    //XMVectorPow
    {
        COMPARISON c;
        XMVECTORF32 v1 = {}, v2 = {}, check = {};
        for (k = 0; k < 10; k++) {
            v1.v = GetRandomVector16();
            v2.v = XMVectorSet(7, 5, -3, -4);
            check.v = XMVectorSet(powf(XMVectorGetX(v1), XMVectorGetX(v2)),
                powf(XMVectorGetY(v1), XMVectorGetY(v2)),
                powf(XMVectorGetZ(v1), XMVectorGetZ(v2)),
                powf(XMVectorGetW(v1), XMVectorGetW(v2)));

            XMVECTOR r = XMVectorPow(v1, v2);
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINBIGGEREPSILON) {
                printe("%s: %f %f %f %f , %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                    XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
            else {
                printi("%s: %d\n", TestName, c);
            }
        }
    }

    return ret;
}

HRESULT Test507(LogProxy* pLog)
{
    //XMVectorHermiteV

    /*
        XMVECTOR T2 = XMVectorMultiply(T, T);
        XMVECTOR T3 = XMVectorMultiply(T , T2);

        XMVECTOR P0 = XMVectorReplicate(2.0f * T3.v[0] - 3.0f * T2.v[0] + 1.0f);
        XMVECTOR T0 = XMVectorReplicate(T3.v[1] - 2.0f * T2.v[1] + T.v[1]);
        XMVECTOR P1 = XMVectorReplicate(-2.0f * T3.v[2] + 3.0f * T2.v[2]);
        XMVECTOR T1 = XMVectorReplicate(T3.v[3] - T2.v[3]);

        Result = XMVectorMultiply(P0, Position0);
        Result = XMVectorMultiplyAdd(T0, Tangent0, Result);
        Result = XMVectorMultiplyAdd(P1, Position1, Result);
        Result = XMVectorMultiplyAdd(T1, Tangent1, Result);

    */
    XMVECTOR pos0 = {};
    XMVECTOR tan0 = {};
    XMVECTOR pos1 = {};
    XMVECTOR tan1 = {};
    XMVECTOR r, T;
    COMPARISON c;
    HRESULT ret = S_OK;
    float f0, f1, f2, f3;
    XMVECTOR check;
    int k;




    for (k = 0; k < 10; k++) {
        for (int i = 0; i < 4; i++) {
            pos0 = XMVectorSetByIndex(pos0, ((float)XM_RAND()) / 2000.f - 8.f, i);
            tan0 = XMVectorSetByIndex(tan0, ((float)XM_RAND()) / 2000.f - 8.f, i);
            pos1 = XMVectorSetByIndex(pos1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            tan1 = XMVectorSetByIndex(tan1, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }



        f0 = ((float)XM_RAND()) / 2000.f - 8.f;
        f1 = ((float)XM_RAND()) / 2000.f - 8.f;
        f2 = ((float)XM_RAND()) / 2000.f - 8.f;
        f3 = ((float)XM_RAND()) / 2000.f - 8.f;
        T = XMVectorSet(f0, f1, f2, f3);

        r = XMVectorHermiteV(pos0, tan0, pos1, tan1, T);

        //generate *correct* result
        {
            XMVECTOR T2 = XMVectorMultiply(T, T);
            XMVECTOR T3 = XMVectorMultiply(T, T2);

            FLOAT P0 = 2.0f * XMVectorGetX(T3) - 3.0f * XMVectorGetX(T2) + 1.0f;
            FLOAT T0 = XMVectorGetY(T3) - 2.0f * XMVectorGetY(T2) + XMVectorGetY(T);
            FLOAT P1 = -2.0f * XMVectorGetZ(T3) + 3.0f * XMVectorGetZ(T2);
            FLOAT T1 = XMVectorGetW(T3) - XMVectorGetW(T2);
            check = XMVectorSet(P0 * XMVectorGetX(pos0) + T0 * XMVectorGetX(tan0) + P1 * XMVectorGetX(pos1) + T1 * XMVectorGetX(tan1),
                P0 * XMVectorGetY(pos0) + T0 * XMVectorGetY(tan0) + P1 * XMVectorGetY(pos1) + T1 * XMVectorGetY(tan1),
                P0 * XMVectorGetZ(pos0) + T0 * XMVectorGetZ(tan0) + P1 * XMVectorGetZ(pos1) + T1 * XMVectorGetZ(tan1),
                P0 * XMVectorGetW(pos0) + T0 * XMVectorGetW(tan0) + P1 * XMVectorGetW(pos1) + T1 * XMVectorGetW(tan1));
        }

        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: f0:%f  f1:%f  f2:%f  f3:%f (%d)\n", TestName, f0, f1, f2, f3, c);
            dqe(pos0); dqe(tan0); dqe(pos1); dqe(tan1); dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}


HRESULT Test508(LogProxy* pLog)
{
    //XMVectorCatmullRomV
    HRESULT ret = S_OK;
    int k = 0;
    do {
        // Generate 4 random vectors
        XMVECTORF32 qarray[5];
        int i = 0;
        do {
            int j = 0;
            do {
                qarray[i].f[j] = ((float)XM_RAND()) / 2000.f - 8.f;
            } while (++j < 4);
        } while (++i < 5);
        // Load them up!
        XMVECTOR q1 = qarray[0];
        XMVECTOR q2 = qarray[1];
        XMVECTOR q3 = qarray[2];
        XMVECTOR q4 = qarray[3];
        XMVECTOR f = qarray[4];
        XMVECTOR r = XMVectorCatmullRomV(q1, q2, q3, q4, f);
        float fx = qarray[4].f[0];
        float fy = qarray[4].f[1];
        float fz = qarray[4].f[2];
        float fw = qarray[4].f[3];
        XMVECTORF32 xcheck = {
            0.5f * ((-fx * fx * fx + 2 * fx * fx - fx) * qarray[0].f[0] + (3 * fx * fx * fx - 5 * fx * fx + 2) * qarray[1].f[0] + (-3 * fx * fx * fx + 4 * fx * fx + fx) * qarray[2].f[0] + (fx * fx * fx - fx * fx) * qarray[3].f[0]),
            0.5f * ((-fy * fy * fy + 2 * fy * fy - fy) * qarray[0].f[1] + (3 * fy * fy * fy - 5 * fy * fy + 2) * qarray[1].f[1] + (-3 * fy * fy * fy + 4 * fy * fy + fy) * qarray[2].f[1] + (fy * fy * fy - fy * fy) * qarray[3].f[1]),
            0.5f * ((-fz * fz * fz + 2 * fz * fz - fz) * qarray[0].f[2] + (3 * fz * fz * fz - 5 * fz * fz + 2) * qarray[1].f[2] + (-3 * fz * fz * fz + 4 * fz * fz + fz) * qarray[2].f[2] + (fz * fz * fz - fz * fz) * qarray[3].f[2]),
            0.5f * ((-fw * fw * fw + 2 * fw * fw - fw) * qarray[0].f[3] + (3 * fw * fw * fw - 5 * fw * fw + 2) * qarray[1].f[3] + (-3 * fw * fw * fw + 4 * fw * fw + fw) * qarray[2].f[3] + (fw * fw * fw - fw * fw) * qarray[3].f[3])
        };
        XMVECTOR check = xcheck;
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: fx:%f,fy:%f,fz:%f,fw:%f (%d)\n", TestName, fx, fy, fz, fw, c);
            dqe(q1); dqe(q2); dqe(q3); dqe(q4); dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    } while (++k < 10);
    return ret;
}



HRESULT Test570(LogProxy* pLog)
{
    //XMVectorSplatOne
    HRESULT ret = S_OK;
    XMVECTOR v = XMVectorSplatOne();
    XMVECTOR check = XMVectorSet(1.f, 1.f, 1.f, 1.f);
    COMPARISON c = CompareXMVECTOR(v, check, 4);
    if (c != EXACT)
    {
        printe("%s: 1.0 1.0 1.0 1.0 ... %f %f %f %f\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v));
        ret = MATH_FAIL;
    }
    return ret;
}



HRESULT Test571(LogProxy* pLog)
{
    //XMVectorSplatEpsilon
    static const float epsilon = 1.192092896e-7f;

    HRESULT ret = S_OK;
    XMVECTOR v = XMVectorSplatEpsilon();
    XMVECTOR check = XMVectorSet(epsilon, epsilon, epsilon, epsilon);
    COMPARISON c = CompareXMVECTOR(v, check, 4);
    if (c != EXACT)
    {
        printe("%s: %f %f %f %f ... %f %f %f %f\n",
            TestName, epsilon, epsilon, epsilon, epsilon, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v));
        ret = MATH_FAIL;
    }
    return ret;
}


HRESULT Test572(LogProxy* pLog)
{
    //XMVectorSplatSignMask
    HRESULT ret = S_OK;
    XMVECTOR v = XMVectorSplatSignMask();
    XMVECTOR check = XMVectorSetInt(0x80000000, 0x80000000, 0x80000000, 0x80000000);
    COMPARISON c = CompareXMVECTOR(v, check, 4);
    if (c != EXACT)
    {
        printe("%s: 0x80000000 0x80000000 0x80000000 0x80000000 ... %x %x %x %x\n",
            TestName, XMVectorGetIntX(v), XMVectorGetIntY(v), XMVectorGetIntZ(v), XMVectorGetIntW(v));
        ret = MATH_FAIL;
    }
    return ret;
}


HRESULT Test573(LogProxy* pLog)
{
    //XMVectorMergeXY
    HRESULT ret = S_OK;
    XMVECTOR v1 = XMVectorSet(1.f, 2.f, 3.f, 4.f);
    XMVECTOR v2 = XMVectorSet(5.f, 6.f, 7.f, 8.f);
    XMVECTOR check = XMVectorSet(1.f, 5.f, 2.f, 6.f);
    XMVECTOR v = XMVectorMergeXY(v1, v2);
    COMPARISON c = CompareXMVECTOR(v, check, 4);
    if (c != EXACT)
    {
        printe("%s: %f %f %f %f @ %f %f %f %f = %f %f %f %f ... %f %f %f %f\n",
            TestName,
            XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
            XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
            XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check),
            XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v));
        ret = MATH_FAIL;
    }
    return ret;
}


HRESULT Test574(LogProxy* pLog)
{
    //XMVectorMergeZW
    HRESULT ret = S_OK;
    XMVECTOR v1 = XMVectorSet(1.f, 2.f, 3.f, 4.f);
    XMVECTOR v2 = XMVectorSet(5.f, 6.f, 7.f, 8.f);
    XMVECTOR check = XMVectorSet(3.f, 7.f, 4.f, 8.f);
    XMVECTOR v = XMVectorMergeZW(v1, v2);
    COMPARISON c = CompareXMVECTOR(v, check, 4);
    if (c != EXACT)
    {
        printe("%s: %f %f %f %f @ %f %f %f %f = %f %f %f %f ... %f %f %f %f\n",
            TestName,
            XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
            XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
            XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check),
            XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v));
        ret = MATH_FAIL;
    }
    return ret;
}

HRESULT Test579(LogProxy* pLog)
{
    //XMVectorSetBinaryConstant
    HRESULT ret = S_OK;

    {
        XMVECTOR v = XMVectorSetBinaryConstant(1, 1, 1, 1);
        XMVECTORF32 check = { 1.f, 1.f, 1.f, 1.f, };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s(1): %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }

    {
        XMVECTOR v = XMVectorSetBinaryConstant(0, 1, 0, 1);
        XMVECTORF32 check = { 0, 1.f, 0, 1.f, };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s(1): %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }

    {
        XMVECTOR v = XMVectorSetBinaryConstant(1, 0, 0, 1);
        XMVECTORF32 check = { 1.f, 0, 0, 1.f };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s(2): %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }

    {
        XMVECTOR v = XMVectorSetBinaryConstant(0, 0, 1, 1);
        XMVECTORF32 check = { 0, 0, 1.f, 1.f };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s(3): %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test580(LogProxy* pLog)
{
    //XMVectorSplatConstant / XMVectorSplatConstantInt
    HRESULT ret = S_OK;

    // For VMX128 this is an intrinsics that requires immediate values, so can't use a loop

#define _VALUE_ 1
    {
        XMVECTOR v = XMVectorSplatConstant(_VALUE_, 0);
        XMVECTORF32 check = { (FLOAT)_VALUE_, (FLOAT)_VALUE_, (FLOAT)_VALUE_, (FLOAT)_VALUE_ };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }

    {
        XMVECTOR v = XMVectorSplatConstantInt(_VALUE_);
        for (int i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(v, i) != _VALUE_) {
                printe("%s: %d: %x ... 0x1\n", TestName, i, XMVectorGetIntByIndex(v, i));
                ret = MATH_FAIL;
            }
        }
    }
#undef _VALUE_

#define _VALUE_ -1
    {
        XMVECTOR v = XMVectorSplatConstant(_VALUE_, 0);
        XMVECTORF32 check = { (FLOAT)_VALUE_, (FLOAT)_VALUE_, (FLOAT)_VALUE_, (FLOAT)_VALUE_ };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }

    {
        XMVECTOR v = XMVectorSplatConstantInt(_VALUE_);
        for (int i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(v, i) != _VALUE_) {
                printe("%s: %d: %x ... 0x1\n", TestName, i, XMVectorGetIntByIndex(v, i));
                ret = MATH_FAIL;
            }
        }
    }
#undef _VALUE_

#define _VALUE_ 15
    {
        XMVECTOR v = XMVectorSplatConstant(_VALUE_, 0);
        XMVECTORF32 check = { (FLOAT)_VALUE_, (FLOAT)_VALUE_, (FLOAT)_VALUE_, (FLOAT)_VALUE_ };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }

    {
        XMVECTOR v = XMVectorSplatConstantInt(_VALUE_);
        for (int i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(v, i) != _VALUE_) {
                printe("%s: %d: %x ... 0x1\n", TestName, i, XMVectorGetIntByIndex(v, i));
                ret = MATH_FAIL;
            }
        }
    }
#undef _VALUE_

#define _VALUE_ -16
    {
        XMVECTOR v = XMVectorSplatConstant(_VALUE_, 0);
        XMVECTORF32 check = { (FLOAT)_VALUE_, (FLOAT)_VALUE_, (FLOAT)_VALUE_, (FLOAT)_VALUE_ };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c != EXACT) {
            printe("%s: %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }

    {
        XMVECTOR v = XMVectorSplatConstantInt(_VALUE_);
        for (int i = 0; i < 4; i++) {
            if (XMVectorGetIntByIndex(v, i) != _VALUE_) {
                printe("%s: %d: %x ... 0x1\n", TestName, i, XMVectorGetIntByIndex(v, i));
                ret = MATH_FAIL;
            }
        }
    }
#undef _VALUE_

#define _VALUE_ -16
#define _DIVVALUE_ 1
    {
        XMVECTOR v = XMVectorSplatConstant(_VALUE_, _DIVVALUE_);
        const float div = (float)(1 << _DIVVALUE_);
        XMVECTORF32 check = { (FLOAT)_VALUE_ / div, (FLOAT)_VALUE_ / div, (FLOAT)_VALUE_ / div, (FLOAT)_VALUE_ / div };
        COMPARISON c = CompareXMVECTOR(v, check, 4);
        if (c > WITHIN10EPSILON) {
            printe("%s: %f %f %f %f = %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            ret = MATH_FAIL;
        }
    }
#undef _VALUE_
#undef _DIVVALUE_

    return ret;
}

HRESULT Test581(LogProxy* pLog)
{
    //XMVectorShiftLeft
    XMVECTORF32 v1 = { 0,1,2,3 };
    XMVECTORF32 v2 = { 4,5,6,7 };
    XMVECTOR r;
    XMVECTORF32 check[4] = { {0,1,2,3}, {1,2,3,4}, {2,3,4,5}, {3,4,5,6} };
    HRESULT ret = S_OK;

    for (uint32_t elements = 0; elements < 4; ++elements)
    {
        r = XMVectorShiftLeft(v1, v2, elements);
        if (XMVector4NotEqual(r, check[elements])) {
            printe("%s: %f %f %f %f, %u = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                elements,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[elements]), XMVectorGetY(check[elements]), XMVectorGetZ(check[elements]), XMVectorGetW(check[elements]));
            ret = MATH_FAIL;
        }
    }

    // Template form
    {
        r = XMVectorShiftLeft<0>(v1, v2);
        if (XMVector4NotEqual(r, check[0])) {
            printe("%s: %f %f %f %f, <0> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[0]), XMVectorGetY(check[0]), XMVectorGetZ(check[0]), XMVectorGetW(check[0]));
            ret = MATH_FAIL;
        }
        r = XMVectorShiftLeft<1>(v1, v2);
        if (XMVector4NotEqual(r, check[1])) {
            printe("%s: %f %f %f %f, <1> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[1]), XMVectorGetY(check[1]), XMVectorGetZ(check[1]), XMVectorGetW(check[1]));
            ret = MATH_FAIL;
        }
        r = XMVectorShiftLeft<2>(v1, v2);
        if (XMVector4NotEqual(r, check[2])) {
            printe("%s: %f %f %f %f, <2> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[2]), XMVectorGetY(check[2]), XMVectorGetZ(check[2]), XMVectorGetW(check[2]));
            ret = MATH_FAIL;
        }
        r = XMVectorShiftLeft<3>(v1, v2);
        if (XMVector4NotEqual(r, check[3])) {
            printe("%s: %f %f %f %f, <3> = %f %f %f %f ... %f %f %f %f\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[3]), XMVectorGetY(check[3]), XMVectorGetZ(check[3]), XMVectorGetW(check[3]));
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test592(LogProxy* pLog)
{
    //XMVectorDivide
    //XMVECTOR operator /=, /, /=S, /S
    HRESULT ret = S_OK;
    XMVECTORF32 v1 = {}, v2 = {};
    XMVECTORF32 check = {}, check2 = {};

    for (int i = 0; i < 10; i++) {
        float s = (float)XM_RAND() / 100.f;
        s = (s > 0.f) ? (s + 1.0f) : (s - 1.0f); // Ensure non-zero
        for (int j = 0; j < 4; j++) {
            float x = (float)XM_RAND() / 100.f;
            float y = (float)XM_RAND() / 100.f;
            v1.v = XMVectorSetByIndex(v1, x, j);
            v2.v = XMVectorSetByIndex(v2, y, j);
            check.v = XMVectorSetByIndex(check, x / y, j);
            check2.v = XMVectorSetByIndex(check2, x / s, j);
        }
        XMVECTOR r = XMVectorDivide(v1, v2);
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        XMVECTOR r2 = v1 / v2;
        XMVECTOR r3 = v1;
        r3 /= v2;
        XMVECTOR r4 = v1 / s;
        XMVECTOR r5 = v1;
        r5 /= s;
#endif
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096)
        {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
#ifndef _XM_NO_XMVECTOR_OVERLOADS_
        c = CompareXMVECTOR(r2, check, 4);
        if (c > WITHIN4096)
        {
            printe("%s (/): %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r2), XMVectorGetY(r2), XMVectorGetZ(r2), XMVectorGetW(r2),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
        c = CompareXMVECTOR(r3, check, 4);
        if (c > WITHIN4096)
        {
            printe("%s (/=): %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r3), XMVectorGetY(r3), XMVectorGetZ(r3), XMVectorGetW(r3),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
        c = CompareXMVECTOR(r4, check2, 4);
        if (c > WITHIN4096)
        {
            printe("%s (/S): %f %f %f %f, %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), s,
                XMVectorGetX(r4), XMVectorGetY(r4), XMVectorGetZ(r4), XMVectorGetW(r4),
                XMVectorGetX(check2), XMVectorGetY(check2), XMVectorGetZ(check2), XMVectorGetW(check2), c);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
        c = CompareXMVECTOR(r5, check2, 4);
        if (c > WITHIN4096)
        {
            printe("%s (/=S): %f %f %f %f, %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1), s,
                XMVectorGetX(r5), XMVectorGetY(r5), XMVectorGetZ(r5), XMVectorGetW(r5),
                XMVectorGetX(check2), XMVectorGetY(check2), XMVectorGetZ(check2), XMVectorGetW(check2), c);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
#endif
    }

    // Test special cases around 0, -0, +INF, -INF
#if (!defined(_M_FP_FAST) || !defined(_XM_NO_INTRINSICS_)) && !defined(__clang__)
    {
        static const float yval[] = { 1.0f, 1.0f,  1.0f, 1.0f,  /*-1.0f, -1.0f,*/ -1.0f, -1.0f };
        static const float xval[] = { 0.0f, -0.0f, c_INF, -c_INF, /* 0.0f, -0.0f,*/  c_INF, -c_INF };
        static const float cval[] = { c_INF, -c_INF, 0.0f, -0.0f, /*-c_INF, c_INF,*/   0.0f, -0.0f };

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            float y = yval[i];
            float chk = cval[i];
            XMVECTOR v1 = XMVectorReplicate(y);
            XMVECTORF32 v2 = { {x,x,x,x} };
            XMVECTOR r = XMVectorDivide(v1, v2);
            XMVECTORF32 check = { {chk,chk,chk,chk} };
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: %f/%f (%zu)= %f %f %f %f...%f %f %f %f %d\n", TestName, y, x, i,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }
#endif

    return ret;
}
