//-------------------------------------------------------------------------------------
// xmvec234.cpp - DirectXMath Test Suite
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

#if (defined(_M_IX86) || defined(_M_X64)) && !defined(_XM_NO_INTRINSICS_)
#define XM_CRMASK_CR6       0x000000F0
#define XM_CRMASK_CR6TRUE   0x00000080
#define XM_CRMASK_CR6FALSE  0x00000020
#define XM_CRMASK_CR6BOUNDS XM_CRMASK_CR6FALSE
#endif

struct IntPairs_t {
    int x;
    int y;
};

struct FloatPairs_t {
    float x;
    float y;
};

HRESULT Test193(LogProxy* pLog)
{
    //XMVector2AngleBetweenNormals 
    HRESULT ret = S_OK;
    COMPARISON c;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVectorSetZ(v1, c_Q_NAN);
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        v2.v = XMVectorSetZ(v2, c_Q_NAN);
        v2.v = XMVectorSetW(v2, c_Q_NAN);
        v1.v = XMVector2Normalize(v1);
        v2.v = XMVector2Normalize(v2);
        XMVECTOR check = XMVectorReplicate(acosf(XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2)));
        XMVECTOR r = XMVector2AngleBetweenNormals(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test194(LogProxy* pLog)
{
    //XMVector2AngleBetweenVectors 
    HRESULT ret = S_OK;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVectorSetZ(v1, c_Q_NAN);
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        v2.v = XMVectorSetZ(v2, c_Q_NAN);
        v2.v = XMVectorSetW(v2, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(acosf((XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2)) / (sqrtf((XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1)) * (XMVectorGetX(v2) * XMVectorGetX(v2) + XMVectorGetY(v2) * XMVectorGetY(v2))))));
        XMVECTOR r = XMVector2AngleBetweenVectors(v1, v2);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}

HRESULT Test195(LogProxy* pLog)
{
    //XMVector2ClampLength and XMVector2ClampLengthV 
    static const XMVECTORF32 v[] = {
        {2,2,c_Q_NAN,c_Q_NAN},{2,2,c_Q_NAN,c_Q_NAN}, {2,2,c_Q_NAN,c_Q_NAN},     {0,0,c_Q_NAN,c_Q_NAN}, {c_INF,c_INF,c_Q_NAN,c_Q_NAN},{3,4,c_Q_NAN,c_Q_NAN}, {5,12,c_Q_NAN,c_Q_NAN}
    };
    static const float n[] = {
        0,      0,        c_INF,       4,       4,            1,       13
    };

    static const XMVECTORF32 N[] = {
        {0,0,0,0}, {0,0,0,0}, {c_INF, c_INF, c_INF, c_INF}, {4, 4 ,4, 4}, {4, 4, 4, 4}, {1,1,1,1}, {13, 13, 13, 13}
    };

    static const float x[] = {
        0,      c_INF,     c_INF,       5,       4,            2.5,     16
    };

    static const XMVECTORF32 X[] = {
        {0,0,0,0}, {c_INF, c_INF, c_INF, c_INF}, {c_INF, c_INF, c_INF, c_INF}, {5, 5, 5, 5}, {4, 4, 4, 4}, {2.5, 2.5, 2.5, 2.5}, {16, 16, 16, 16}
    };
    static const XMVECTORF32 check[] = {
        {0,0},{2,2},{c_INF,c_INF},{0,0}, {c_INF,c_INF},{1.5,2},{5,12}
    };
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;

    //XMVector2ClampLength
    for (k = 0; k < countof(v); k++) {
        r = XMVector2ClampLength(v[k], n[k], x[k]);
        c = CompareXMVECTOR(r, check[k], 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[k]), XMVectorGetY(v[k]), XMVectorGetZ(v[k]), XMVectorGetW(v[k]),
                n[k], x[k],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[k]), XMVectorGetY(check[k]), XMVectorGetZ(check[k]), XMVectorGetW(check[k]), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    //XMVector2ClampLengthV
    for (k = 0; k < countof(v); k++) {
        r = XMVector2ClampLengthV(v[k], N[k], X[k]);
        c = CompareXMVECTOR(r, check[k], 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[k]), XMVectorGetY(v[k]), XMVectorGetZ(v[k]), XMVectorGetW(v[k]),
                n[k], x[k],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[k]), XMVectorGetY(check[k]), XMVectorGetZ(check[k]), XMVectorGetW(check[k]), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test196(LogProxy* pLog)
{
    //XMVector2Cross 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        v2.v = GetRandomVector16();
        v1.v = XMVectorSetZ(v1, c_Q_NAN);
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        v2.v = XMVectorSetZ(v2, c_Q_NAN);
        v2.v = XMVectorSetW(v2, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(v1) * XMVectorGetY(v2) - XMVectorGetY(v1) * XMVectorGetX(v2));
        XMVECTOR r = XMVector2Cross(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test197(LogProxy* pLog)
{
    //XMVector2Dot 
    XMVECTORF32 l = {};
    XMVECTORF32 v = {};
    COMPARISON c = EXACT, temp;
    HRESULT ret = S_OK;
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 4; j++) {
            l.v = XMVectorSetByIndex(l, ((float)XM_RAND()) / 1000.f, j);
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 1000.f, j);
        }
        l.v = XMVectorSetZ(l, c_Q_NAN);
        v.v = XMVectorSetZ(v, c_Q_NAN);
        l.v = XMVectorSetW(l, c_Q_NAN);
        v.v = XMVectorSetW(v, c_Q_NAN);
        XMVECTOR r = XMVector2Dot(l, v);
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(l) * XMVectorGetX(v) + XMVectorGetY(l) * XMVectorGetY(v));
        temp = CompareXMVECTOR(r, check, 4);
        c = std::max(c, temp);
        if (temp > WITHIN4096) {
            printe("%s: %f %f %f %f dot %f %f %f %f = %f ... %f (%d)\n",
                TestName, XMVectorGetX(l), XMVectorGetY(l), XMVectorGetZ(l), XMVectorGetW(l),
                XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetX(check), temp);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, temp);
        }
    }
    return ret;
}
HRESULT Test198(LogProxy* pLog)
{
    //XMVector2Equal 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        check = TRUE;
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector2Equal(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = TRUE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        r = XMVector2Equal(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test199(LogProxy* pLog)
{
    //XMVector2Greater 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector2Greater(v1, v2);
        check = ((j & 12) == 12) ? TRUE : FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        check = FALSE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        r = XMVector2Greater(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test200(LogProxy* pLog)
{
    //XMVector2GreaterOrEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector2GreaterOrEqual(v1, v2);
        check = TRUE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = (j & 12) ? FALSE : TRUE;
        r = XMVector2GreaterOrEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test201(LogProxy* pLog)
{
    //XMVector2IntersectLine 
    static const XMVECTORF32 v1[] = {
        {2,2},{0,0},{2,2},{0,0},{-1,-1},     {-1,-1},       {-3,1}
    };
    static const XMVECTORF32 v2[] = {
        {0,0},{2,2},{0,0},{2,2},{ 1, 2},     { 1, 2},       { 4,4}
    };
    static const XMVECTORF32 v3[] = {
        {0,2},{2,0},{2,0},{0,2},{ 1, 2},     { 1, 0},       { 9,3}
    };
    static const XMVECTORF32 v4[] = {
        {2,0},{0,2},{0,2},{2,0},{-1,-1},     { 3, 3},       {10,5}
    };
    static const XMVECTORF32 check[] = {
        {1,1},{1,1},{1,1},{1,1},{c_INF,c_INF},{c_Q_NAN,c_Q_NAN},{11,7}
    };
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;

    for (k = 0; k < countof(v1); k++) {
        r = XMVector2IntersectLine(v1[k], v2[k], v3[k], v4[k]);
        c = CompareXMVECTOR(r, check[k], 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: (%d)", TestName, c);
            dqe(v1[k]); dqe(v2[k]); dqe(v3[k]); dqe(v4[k]); printe("="); dqe(r); printe("..."); dqe(check[k]);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test202(LogProxy* pLog)
{
    //XMVector2IsInfinite 
    HRESULT ret = S_OK;
    static const float f[] = { c_INF, -c_INF, c_Q_NAN, 1,-1 };
    int r, g, b, a;
    XMVECTOR col;
    BOOL res, check;
    for (r = 0; r < countof(f); r++) {
        for (g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for (b = 0; b < countof(f); b++) {
                for (a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r], f[g], f[b], f[a]);
                    check = (r < 2 || g < 2) ? TRUE : FALSE;
                    res = XMVector2IsInfinite(col);
                    if (res != check) {
                        printe("\n%s: %f %f %f %f = %d ... %d\n",
                            TestName, XMVectorGetX(col), XMVectorGetY(col), XMVectorGetZ(col), XMVectorGetW(col), res, check);
                        ret = MATH_FAIL;
                    }
                    else {
                        //printi("%d  ", res);
                    }
                }
            }
        }
    }
    printi("\n");
    return ret;
}

ISNAN_TEST_BEGIN
HRESULT Test203(LogProxy* pLog)
{
    //XMVector2IsNaN 
    HRESULT ret = S_OK;
    static const float f[] = { c_Q_NAN, c_NAN, c_INF, 1,-1 };
    int r, g, b, a;
    XMVECTOR col;
    BOOL res, check;
    for (r = 0; r < countof(f); r++) {
        for (g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for (b = 0; b < countof(f); b++) {
                for (a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r], f[g], f[b], f[a]);
                    check = (r < 2 || g < 2) ? TRUE : FALSE;
                    res = XMVector2IsNaN(col);
                    if (res != check) {
                        printe("\n%s: %f %f %f %f = %d ... %d\n",
                            TestName, XMVectorGetX(col), XMVectorGetY(col), XMVectorGetZ(col), XMVectorGetW(col), res, check);
                        ret = MATH_FAIL;
                    }
                    else {
                        //printi("%d  ", res);
                    }
                }
            }
        }
    }
    printi("\n");
    return ret;
}
ISNAN_TEST_END

HRESULT Test204(LogProxy* pLog)
{
    //XMVector2Length 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMVector2Length(XMVectorSet(0, 0, c_Q_NAN, c_Q_NAN));
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate(sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1)));
        r = XMVector2Length(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}
HRESULT Test205(LogProxy* pLog)
{
    //XMVector2LengthSq 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMVector2LengthSq(XMVectorSet(0, 0, c_Q_NAN, c_Q_NAN));
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1));
        r = XMVector2LengthSq(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}
HRESULT Test206(LogProxy* pLog)
{
    //XMVector2Less 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector2Less(v1, v2);
        check = FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = ((j & 12) == 12) ? TRUE : FALSE;
        r = XMVector2Less(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test207(LogProxy* pLog)
{
    //XMVector2LessOrEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector2LessOrEqual(v1, v2);
        check = (j & 12) ? FALSE : TRUE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = TRUE;
        r = XMVector2LessOrEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test208(LogProxy* pLog)
{
    //XMVector2LinePointDistance 
    static const XMVECTORF32 v1[] = {
        {2,2,c_Q_NAN,c_Q_NAN},{0,0,c_Q_NAN,c_Q_NAN},{2,2},{0,0},{0,0},{1,1}
    };
    static const XMVECTORF32 v2[] = {
        {0,0,c_Q_NAN,c_Q_NAN},{2,2,c_Q_NAN,c_Q_NAN},{0,0},{2,2},{3,4},{4,5}
    };
    static const XMVECTORF32 v3[] = {
        {1,1,c_Q_NAN,c_Q_NAN},{1,1,c_Q_NAN,c_Q_NAN},{2,2},{0,2},{10,5},{3,12}
    };

    static const float check[] = {
        0,0,0,sqrtf(2.f),5,5
    };
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;

    for (k = 0; k < countof(v1); k++) {
        r = XMVector2LinePointDistance(v1[k], v2[k], v3[k]);
        c = CompareXMVECTOR(r, XMVectorReplicate(check[k]), 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: (%d)\n", TestName, c);
            dqe(v1[k]); dqe(v2[k]); dqe(v3[k]); printe("="); dqe(r); printe("..."); dqe(XMVectorReplicate(check[k]));
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test209(LogProxy* pLog)
{
    //XMVector2NearEqual 
    HRESULT ret = S_OK;
    XMVECTORF32 v1 = {}, v2 = {}, e = {};
    BOOL r, check;
    int i, j, k;
    for (j = 0; j < 16; j++) {
        for (k = 0; k < 2; k++) {
            for (i = 0; i < 4; i++) {
                v1.v = XMVectorSetByIndex(v1, (float)(XM_RAND()) / 1000.f, i);
                e.v = XMVectorSetByIndex(e, (float)(XM_RAND()) / 10000.f, i);
                v2.v = XMVectorSetByIndex(v2, XMVectorGetByIndex(v1, i) + ((k & 1) ? 1.f : -1.f) * ((j & (1 << i)) ? (XMVectorGetByIndex(e, i) + XMVectorGetByIndex(e, i)) : (XMVectorGetByIndex(e, i) / 2.f)), i);
            }
            check = ((j & 3) ? FALSE : TRUE);
            r = XMVector2NearEqual(v1, v2, e);
            if (r != check) {
                printe("%s: j%d k%d %f %f %f %f, %f %f %f %f, %f %f %f %f = %x ... %x\n",
                    TestName, j, k, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                    XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                    XMVectorGetX(e), XMVectorGetY(e), XMVectorGetZ(e), XMVectorGetW(e), r, check);
                ret = MATH_FAIL;
            }
            else {
                //				printi("%s: j%d k%d %f %f %f %f, %f %f %f %f, %f %f %f %f = %x ... %x\n",
                //					TestName,j,k, XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                //                  XMVectorGetX(v2),XMVectorGetY(v2),XMVectorGetZ(v2),XMVectorGetW(v2),
                //                  XMVectorGetX(e),XMVectorGetY(e),XMVectorGetZ(e),XMVectorGetW(e), r, check);
                printi("%s: j%d k%d (0)\n", TestName, j, k);
            }
        }
    }
    return ret;
}
HRESULT Test210(LogProxy* pLog)
{
    //XMVector2Normalize 
    int i, k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1 = {}, check = {};
    float rcplength;

    for (k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        v1.v = XMVectorSetZ(v1, c_Q_NAN);
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        rcplength = 1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1));
        for (i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check, XMVectorGetByIndex(v1, i) * rcplength, i);
        }

        XMVECTOR r = XMVector2Normalize(v1);
        c = CompareXMVECTOR(r, check, 2);
        if (c > WITHIN4096) {
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
#ifdef __INTEL_LLVM_COMPILER
        static const float xval[] = { 0.f,  -0.f,   c_INF,  -c_INF };
        static const float cval[] = { 0.f,   0.f, c_Q_NAN, c_Q_NAN };
#else
        static const float xval[] = { 0.f,  -0.f,   c_INF,  -c_INF, c_Q_NAN };
        static const float cval[] = { 0.f,   0.f, c_Q_NAN, c_Q_NAN, c_Q_NAN };
#endif

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVector2Normalize(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
            c = CompareXMVECTOR(r, check, 4);
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
HRESULT Test211(LogProxy* pLog)
{
    //XMVector2NotEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector2NotEqual(v1, v2);
        check = (j & 12) ? TRUE : FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = (j & 12) ? TRUE : FALSE;
        r = XMVector2NotEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test212(LogProxy* pLog)
{
    //XMVector2Orthogonal 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;
    XMVECTOR v;
    for (int k = 0; k < 15; k++) {
        FLOAT vx = GetRandomFloat16();
        FLOAT vy = GetRandomFloat16();
        v = XMVectorSet(vx, vy, c_Q_NAN, c_Q_NAN);
        r = XMVector2Orthogonal(v);
        XMVECTORF32 check = { {-XMVectorGetY(v), XMVectorGetX(v)} };
        float checkdot = XMVectorGetX(v) * XMVectorGetX(r) + XMVectorGetY(v) * XMVectorGetY(r);
        c = CompareXMVECTOR(r, check, 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f = %f %f ... %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetX(r), XMVectorGetY(r), XMVectorGetX(check), XMVectorGetY(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
        c = Compare(checkdot, 0);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f = %f %f. Dot(%f) ... 0\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetX(r), XMVectorGetY(r), checkdot);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test213(LogProxy* pLog)
{
    //XMVector2ReciprocalLength 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1 = {};

    for (k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        v1.v = XMVectorSetZ(v1, c_Q_NAN);
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1)));
        XMVECTOR r = XMVector2ReciprocalLength(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}
HRESULT Test214(LogProxy* pLog)
{
    //XMVector2Reflect 
    XMVECTORF32 v1 = {}, v2 = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    int i, k;
    for (k = 0; k < 15; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVectorSetZ(v1, c_Q_NAN);
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        v2.v = XMVectorSetZ(v2, c_Q_NAN);
        v2.v = XMVectorSetW(v2, c_Q_NAN);
        // Result = Incident - (2 * dot(Incident, Normal)) * Normal
        XMVECTOR check = XMVectorSubtract(
            v1,
            XMVectorScale(
                v2,
                2 * (XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2))));
        XMVECTOR r = XMVector2Reflect(v1, v2);
        c = CompareXMVECTOR(r, check, 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f %f %f %f  = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test215(LogProxy* pLog)
{
    //XMVector2Refract and XMVector2RefractV
    HRESULT ret = S_OK;
    int k;
    //XMVector2Refract
    for (k = 0; k < 15; k++) {
        XMVECTORF32 xv1;
        XMVECTORF32 xv2;
        int i = 0;
        do {
            xv1.f[i] = ((float)XM_RAND()) / 2000.f - 8.f;
            xv2.f[i] = ((float)XM_RAND()) / 2000.f - 8.f;
        } while (++i < 2);
        xv1.f[2] = c_Q_NAN;
        xv1.f[3] = c_Q_NAN;
        xv2.f[2] = c_Q_NAN;
        xv2.f[3] = c_Q_NAN;
        float f = ((float)XM_RAND()) / 2000.f - 8.f;
        // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
        // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
        float fdot = (xv1.f[0] * xv2.f[0]) + (xv1.f[1] * xv2.f[1]);
        float term1 = 1.0f - (f * f * (1.0f - fdot * fdot));
        XMVECTOR check;
        if (term1 < 0) {
            check = XMVectorZero();
        }
        else {
            float f2 = (f * fdot + sqrtf(term1));
            check = XMVectorSet(f * xv1.f[0] - xv2.f[0] * f2, f * xv1.f[1] - xv2.f[1] * f2, c_Q_NAN, c_Q_NAN);
        }
        XMVECTOR r = XMVector2Refract(xv1, xv2, f);
        COMPARISON c = CompareXMVECTOR(r, check, 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f %f %f %f  %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                xv1.f[0], xv1.f[1], xv1.f[2], xv1.f[3],
                xv2.f[0], xv2.f[1], xv2.f[2], xv2.f[3], f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }//notest (need to check more for total internal refraction,
    //and also for sqrt(0) cases

    //XMVector2RefractV
    for (k = 0; k < 15; k++) {
        XMVECTORF32 xv1;
        XMVECTORF32 xv2;
        XMVECTORF32 xV;
        int i = 0;
        do {
            xv1.f[i] = ((float)XM_RAND()) / 2000.f - 8.f;
            xv2.f[i] = ((float)XM_RAND()) / 2000.f - 8.f;
        } while (++i < 2);
        xv1.f[2] = c_Q_NAN;
        xv1.f[3] = c_Q_NAN;
        xv2.f[2] = c_Q_NAN;
        xv2.f[3] = c_Q_NAN;
        i = 0;
        do {
            xV.f[i] = ((float)XM_RAND()) / 2000.f - 8.f;
        } while (++i < 4);
        // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
        // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
        float fdot = (xv1.f[0] * xv2.f[0]) + (xv1.f[1] * xv2.f[1]);
        XMVECTORF32 Vsqrt2;
        Vsqrt2.f[0] = 1.0f - (xV.f[0] * xV.f[0] * (1.0f - fdot * fdot));
        Vsqrt2.f[1] = 1.0f - (xV.f[1] * xV.f[1] * (1.0f - fdot * fdot));
        Vsqrt2.f[2] = c_Q_NAN;
        Vsqrt2.f[3] = c_Q_NAN;
        XMVECTOR check;
        float termx;
        float termy;
        // This is the correct behavior for the bad refraction cases
        // when dealing with negative numbers being passed to sqrtf()
        if (Vsqrt2.f[0] < 0) {
            termx = 0.0f;
        }
        else {
            termx = xV.f[0] * xv1.f[0] - xv2.f[0] * (xV.f[0] * fdot + sqrtf(Vsqrt2.f[0]));
        }
        if (Vsqrt2.f[1] < 0) {
            termy = 0.0f;
        }
        else {
            termy = xV.f[1] * xv1.f[1] - xv2.f[1] * (xV.f[1] * fdot + sqrtf(Vsqrt2.f[1]));
        }
        check = XMVectorSet(termx, termy, c_Q_NAN, c_Q_NAN);
        XMVECTOR r = XMVector2RefractV(xv1, xv2, xV);
        COMPARISON c = CompareXMVECTOR(r, check, 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s(V): %f %f %f %f  %f %f %f %f    %f  %f %f %f= %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                xv1.f[0], xv1.f[1], xv1.f[2], xv1.f[3],
                xv2.f[0], xv2.f[1], xv2.f[2], xv2.f[3],
                xV.f[0], xV.f[1], xV.f[2], xV.f[3],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }//notest (need to check more for total internal refraction,
    //and also for sqrt(0) cases
    return ret;
}

HRESULT Test216(LogProxy* pLog)
{
    //XMVector2Transform 
    XMVECTORF32 v = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float tmp[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
            }
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        v.v = XMVectorSetW(v, c_Q_NAN);
        v.v = XMVectorSetZ(v, c_Q_NAN);
        FLOAT checkx = XMVectorGetX(v) * tmp[0][0] + XMVectorGetY(v) * tmp[1][0] + tmp[3][0];
        FLOAT checky = XMVectorGetX(v) * tmp[0][1] + XMVectorGetY(v) * tmp[1][1] + tmp[3][1];
        FLOAT checkz = XMVectorGetX(v) * tmp[0][2] + XMVectorGetY(v) * tmp[1][2] + tmp[3][2];
        FLOAT checkw = XMVectorGetX(v) * tmp[0][3] + XMVectorGetY(v) * tmp[1][3] + tmp[3][3];
        XMVECTOR check = XMVectorSet(checkx, checky, checkz, checkw);
        XMVECTOR r = XMVector2Transform(v, m);
        c = CompareXMVECTOR(r, check, 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f *   (%d)\n", TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); dqe(r); printe("..."); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test217(LogProxy* pLog)
{
    //XMVector2TransformCoord 
    XMVECTORF32 v = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float tmp[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
            }
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v.v = XMVectorSetW(v, c_Q_NAN);
        v.v = XMVectorSetZ(v, c_Q_NAN);
        FLOAT checkx = XMVectorGetX(v) * tmp[0][0] + XMVectorGetY(v) * tmp[1][0] + tmp[3][0];
        FLOAT checky = XMVectorGetX(v) * tmp[0][1] + XMVectorGetY(v) * tmp[1][1] + tmp[3][1];
        FLOAT checkz = XMVectorGetX(v) * tmp[0][2] + XMVectorGetY(v) * tmp[1][2] + tmp[3][2];
        FLOAT checkw = XMVectorGetX(v) * tmp[0][3] + XMVectorGetY(v) * tmp[1][3] + tmp[3][3];
        checkx *= (1 / (checkw));
        checky *= (1 / (checkw));
        checkz *= (1 / (checkw));
        checkw *= (1 / (checkw));
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        XMVECTOR check = XMVectorSet(checkx, checky, checkz, checkw);
        XMVECTOR r = XMVector2TransformCoord(v, m);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f *   (%d)\n", TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); dqe(r); printe("..."); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test218(LogProxy* pLog)
{
    //XMVector2TransformCoordStream 
    const uint32_t dwDataSize = 10000;
    const uint32_t dwNumItems = 200;
    static const int ins[] = { 8,16,20,32,24 };
    static const int outs[] = { 8,16,32,44,28 };
    XMVECTOR v[dwNumItems];
    XMVECTOR check[dwNumItems];
    HRESULT ret = S_OK;

    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;

    int n;
    BOOL OutWriteCombined = FALSE;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;

    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
    {
        AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
        if (!pbReal1 || !pbSandbox1)
            goto Cleanup;

        for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
            if (!pbReal3 || !pbSandbox3)
                goto Cleanup;

            for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
            {
                for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                {
                    float tmp[4][4];
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
                        }
                        for (n = 0; n < dwNumItems; n++) {
                            v[n] = XMVectorSetByIndex(v[n], ((float)XM_RAND()) / 2000.f - 8.f, i);
                            v[n] = XMVectorSetZ(v[n], c_Q_NAN);
                            v[n] = XMVectorSetW(v[n], c_Q_NAN);
                        }
                    }
                    for (n = 0; n < dwNumItems; n++) {
                        XMVECTOR vv = v[n];
                        FLOAT checkx = XMVectorGetX(vv) * tmp[0][0] + XMVectorGetY(vv) * tmp[1][0] + tmp[3][0];
                        FLOAT checky = XMVectorGetX(vv) * tmp[0][1] + XMVectorGetY(vv) * tmp[1][1] + tmp[3][1];
                        FLOAT checkz = XMVectorGetX(vv) * tmp[0][2] + XMVectorGetY(vv) * tmp[1][2] + tmp[3][2];
                        FLOAT checkw = XMVectorGetX(vv) * tmp[0][3] + XMVectorGetY(vv) * tmp[1][3] + tmp[3][3];
                        checkx *= (1 / (checkw));
                        checky *= (1 / (checkw));
                        checkz *= (1 / (checkw));
                        check[n] = XMVectorSet(checkx, checky, checkz, 1);

                    }
                    fillsandbox(pbSandbox1, dwDataSize, v, 16, ins[InStrideIndex], dwNumItems);
                    fillsandbox(pbSandbox2, dwDataSize, check, 16, outs[OutStrideIndex], dwNumItems);
                    initsandbox(pbSandbox3, dwDataSize);

#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                    XMMATRIX m(&tmp[0][0]);
                    const XMFLOAT2* pointer = XMVector2TransformCoordStream((XMFLOAT2*)&pbSandbox3[0], outs[OutStrideIndex], (const XMFLOAT2*)&pbSandbox1[0], ins[InStrideIndex], dwNumItems, m);

                    if (pointer != (const XMFLOAT2*)&pbSandbox3[0]) {
                        printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                        ret = MATH_FAIL;
                    }
                    if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 16, dwNumItems, dwDataSize, 2, WITHIN1_256)) {
                        printe("%s: ins:%d, outs:%d, count:%u\n", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                        ret = MATH_FAIL;
                    }
                }//end "for OutStride"
            }//end  "for InStride"
            FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
            pbReal3 = pbSandbox3 = nullptr;
        }//end "for OutAlignIndex"
        FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
        pbReal1 = pbSandbox1 = nullptr;
    }//end "for InAlignIndex"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
    return ret;
}
HRESULT Test219(LogProxy* pLog)
{
    //XMVector2TransformNormal 
    XMVECTORF32 v = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float tmp[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
            }
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v.v = XMVectorSetW(v, c_Q_NAN);
        v.v = XMVectorSetZ(v, c_Q_NAN);
        FLOAT checkx = XMVectorGetX(v) * tmp[0][0] + XMVectorGetY(v) * tmp[1][0];
        FLOAT checky = XMVectorGetX(v) * tmp[0][1] + XMVectorGetY(v) * tmp[1][1];
        FLOAT checkz = XMVectorGetX(v) * tmp[0][2] + XMVectorGetY(v) * tmp[1][2];
        FLOAT checkw = XMVectorGetX(v) * tmp[0][3] + XMVectorGetY(v) * tmp[1][3];
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        XMVECTOR check = XMVectorSet(checkx, checky, checkz, checkw);
        XMVECTOR r = XMVector2TransformNormal(v, m);
        c = CompareXMVECTOR(r, check, 2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f *   (%d)\n", TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); dqe(r); printe("..."); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test220(LogProxy* pLog)
{
    //XMVector2TransformNormalStream 
    const uint32_t dwDataSize = 10000;
    const uint32_t dwNumItems = 200;
    static const int ins[] = { 8, 16, 32, 24 };
    static const int outs[] = { 8, 16, 32, 44,28 };
    XMFLOAT2  v[dwNumItems];
    XMFLOAT2 check[dwNumItems];
    HRESULT ret = S_OK;
    int n;

    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;

    BOOL OutWriteCombined = FALSE;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;

    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
    {
        AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
        if (!pbReal1 || !pbSandbox1)
            goto Cleanup;

        for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
            if (!pbReal3 || !pbSandbox3)
                goto Cleanup;

            for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
            {
                for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                {
                    float tmp[4][4];
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
                        }
                    }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                    XMMATRIX m(&tmp[0][0]);

                    for (n = 0; n < dwNumItems; n++)
                    {
                        v[n].x = ((float)XM_RAND()) / 2000.f - 8.f;
                        v[n].y = ((float)XM_RAND()) / 2000.f - 8.f;
                    }

                    for (n = 0; n < dwNumItems; n++) {
                        check[n].x = v[n].x * tmp[0][0] + v[n].y * tmp[1][0];
                        check[n].y = v[n].x * tmp[0][1] + v[n].y * tmp[1][1];

                    }
                    fillsandbox(pbSandbox1, dwDataSize, v, 8, ins[InStrideIndex], dwNumItems);
                    fillsandbox(pbSandbox2, dwDataSize, check, 8, outs[OutStrideIndex], dwNumItems);
                    initsandbox(pbSandbox3, dwDataSize);
                    const XMFLOAT2* pointer = XMVector2TransformNormalStream((XMFLOAT2*)&pbSandbox3[0], outs[OutStrideIndex], (const XMFLOAT2*)&pbSandbox1[0], ins[InStrideIndex], dwNumItems, m);

                    if (pointer != (const XMFLOAT2*)&pbSandbox3[0]) {
                        printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                        ret = MATH_FAIL;
                    }
                    if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 8, dwNumItems, dwDataSize, 2))
                    {
                        printe("%s: ins:%d, outs:%d, count:%u\n", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                        ret = MATH_FAIL;
                    }

                }//end "for OutStride"
            }//end  "for InStride"
            FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
            pbReal3 = pbSandbox3 = nullptr;
        }//end "for OutAlignIndex"
        FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
        pbReal1 = pbSandbox1 = nullptr;
    }//end "for InAlignIndex"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);


    return ret;
}
HRESULT Test221(LogProxy* pLog)
{
    //XMVector2TransformStream and XMVector2TransformStreamNC
    HRESULT ret = S_OK;

    const uint32_t dwDataSize = 10000;
    const uint32_t dwNumItems = 200;
    static const int ins[] = { 8,12,16,20,32,24 };
    static const int outs[] = { 16,32,44,28 };
    XMVECTOR v[dwNumItems] = {};
    XMVECTOR check[dwNumItems] = {};
    int n;

    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;


    BOOL OutWriteCombined = FALSE;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;

    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (OutWriteCombined = 0; OutWriteCombined < 1; OutWriteCombined++)
    {
        for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
            if (!pbReal1 || !pbSandbox1)
                goto Cleanup;

            for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
            {
                AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
                if (!pbReal3 || !pbSandbox3)
                    goto Cleanup;

                for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
                {
                    for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                    {
                        float tmp[4][4];
                        for (int i = 0; i < 4; i++) {
                            for (int j = 0; j < 4; j++) {
                                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
                            }
                            for (n = 0; n < dwNumItems; n++) {
                                v[n] = XMVectorSetByIndex(v[n], ((float)XM_RAND()) / 2000.f - 8.f, i);
                                v[n] = XMVectorSetZ(v[n], c_Q_NAN);
                                v[n] = XMVectorSetW(v[n], c_Q_NAN);
                            }
                        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                        XMMATRIX m(&tmp[0][0]);
                        for (n = 0; n < dwNumItems; n++) {
                            XMVECTOR vv = v[n];
                            FLOAT checkx = XMVectorGetX(vv) * tmp[0][0] + XMVectorGetY(vv) * tmp[1][0] + tmp[3][0];
                            FLOAT checky = XMVectorGetX(vv) * tmp[0][1] + XMVectorGetY(vv) * tmp[1][1] + tmp[3][1];
                            FLOAT checkz = XMVectorGetX(vv) * tmp[0][2] + XMVectorGetY(vv) * tmp[1][2] + tmp[3][2];
                            FLOAT checkw = XMVectorGetX(vv) * tmp[0][3] + XMVectorGetY(vv) * tmp[1][3] + tmp[3][3];
                            check[n] = XMVectorSet(checkx, checky, checkz, checkw);
                        }
                        fillsandbox(pbSandbox1, dwDataSize, v, 16, ins[InStrideIndex], dwNumItems);
                        fillsandbox(pbSandbox2, dwDataSize, check, 16, outs[OutStrideIndex], dwNumItems);
                        initsandbox(pbSandbox3, dwDataSize);
                        const XMFLOAT4* pointer = nullptr;

                        if (!OutWriteCombined)
                        {
                            pointer = XMVector2TransformStream((XMFLOAT4*)&pbSandbox3[0], outs[OutStrideIndex], (const XMFLOAT2*)&pbSandbox1[0], ins[InStrideIndex], dwNumItems, m);
                        }

                        if (pointer != (const XMFLOAT4*)&pbSandbox3[0]) {
                            printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                            ret = MATH_FAIL;
                        }
                        if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 16, dwNumItems, dwDataSize, 4)) {
                            printe("%s: ins:%d, outs:%d, count:%u\n", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                            ret = MATH_FAIL;
                        }
                    }//end "for OutStride"
                }//end  "for InStride"
                FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
                pbReal3 = pbSandbox3 = nullptr;
            }//end "for OutAlignIndex"
            FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
            pbReal1 = pbSandbox1 = nullptr;
        }//end "for InAlignIndex"
    }//end "for OutWriteCombined"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);

    return ret;
}
HRESULT Test222(LogProxy* pLog)
{
    //XMVector3AngleBetweenNormals 
    HRESULT ret = S_OK;
    COMPARISON c;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVector3Normalize(v1);
        v2.v = XMVector3Normalize(v2);
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        v2.v = XMVectorSetW(v2, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(acosf(XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2)));
        XMVECTOR r = XMVector3AngleBetweenNormals(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetX(v2), XMVectorGetW(v2),
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

//
// Currently the test qualifies XMVector3AngleBetweenVectors using random constants.
// This value was reported by a customer and it caused invalid output (Q_NAN)
// Bug 71410. This hard coded unit test is to ensure bug 71410 never returns.
//

static XMVECTORF32 TestValues223[2] = {
    {-0.0263490081f,-0.114045918f,-0.226352930f,c_Q_NAN},
    {-0.0504247323f,-0.218104258f,-0.432600230f,c_Q_NAN}
};

HRESULT Test223(LogProxy* pLog)
{
    //XMVector3AngleBetweenVectors
    HRESULT ret = S_OK;
    // Note: The initialization of v1image is unneccessary
    // it was added to get around a "potentially unitialized
    // data" compiler warning.
    XMVECTORF32 v1image = {};
    XMVECTORF32 v2image = {};
    const XMVECTORF32* pTest223 = TestValues223;
    int k;
    for (k = 0; k < 10 + 1; k++) {
        XMVECTOR v1, v2;
        // Check for the hard coded tests
        if (k < 1) {
            v1 = pTest223[0].v;
            v2 = pTest223[1].v;
            v1image.v = v1;		// Store in memory copy
            v2image.v = v2;
            pTest223 += 2;
        }
        else {
            int i;
            // Prepare the random tests
            for (i = 0; i < 4; i++) {
                v1image.f[i] = (static_cast<float>(XM_RAND()) / 2000.f) - 8.f;
                v2image.f[i] = (static_cast<float>(XM_RAND()) / 2000.f) - 8.f;
            }
            v1image.f[3] = c_Q_NAN;
            v2image.f[3] = c_Q_NAN;
            v1 = v1image.v;
            v2 = v2image.v;
        }
        float fcheck = acosf((v1image.f[0] * v2image.f[0] + v1image.f[1] * v2image.f[1] + v1image.f[2] * v2image.f[2]) /
            (sqrtf((v1image.f[0] * v1image.f[0] + v1image.f[1] * v1image.f[1] + v1image.f[2] * v1image.f[2]) *
            (v2image.f[0] * v2image.f[0] + v2image.f[1] * v2image.f[1] + v2image.f[2] * v2image.f[2]))));
        XMVECTORF32 check = { fcheck,fcheck,fcheck,fcheck };
        XMVECTOR r = XMVector3AngleBetweenVectors(v1, v2);
        COMPARISON c = CompareXMVECTOR(r, check.v, 4);
        if (c > WITHINBIGEPSILON) {
            XMVECTORF32 rimage;
            rimage.v = r;
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                v1image.f[0], v1image.f[1], v1image.f[2], v1image.f[3],
                v2image.f[0], v2image.f[1], v2image.f[2], v2image.f[3],
                rimage.f[0], rimage.f[1], rimage.f[2], rimage.f[3],
                check.f[0], check.f[1], check.f[2], check.f[3], c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}

HRESULT Test224(LogProxy* pLog)
{
    //XMVector3ClampLength and XMVector3ClampLengthV
    static const XMVECTORF32 v[] = {
        {2,2,2,c_Q_NAN},{2,2,2}, {2,2,2},        {0,0,0}, {c_INF,c_INF,c_INF},{3,4,12}, {5,12,84}
    };
    static const float n[] = {
        0,         0,         c_INF,            4,          4,                 1,         85
    };
    static const XMVECTORF32 N[] = {
        {0,0,0,0}, {0,0,0,0}, {c_INF, c_INF, c_INF, c_INF}, {4,4,4,4}, {4,4,4,4}, {1,1,1,1}, {85,85,85,85}
    };
    static const float x[] = {
        0,        c_INF,      c_INF,             5,          4,                 6.5,       90
    };
    static const XMVECTORF32 X[] = {
        {0,0,0,0}, {c_INF, c_INF, c_INF, c_INF}, {c_INF, c_INF, c_INF, c_INF}, {5,5,5,5}, {4,4,4,4}, {6.5, 6.5, 6.5, 6.5}, {90,90,90,90}
    };
    static const XMVECTORF32 check[] = {
        {0,0,0},{2,2,2},{c_INF,c_INF,c_INF},{0,0,0}, {c_INF,c_INF,c_INF},{1.5,2,6},{5,12,84}
    };
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;

    //XMVector3ClampLength
    for (k = 0; k < countof(v); k++) {
        r = XMVector3ClampLength(v[k], n[k], x[k]);
        c = CompareXMVECTOR(r, check[k], 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[k]), XMVectorGetY(v[k]), XMVectorGetZ(v[k]), XMVectorGetW(v[k]),
                n[k], x[k],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[k]), XMVectorGetY(check[k]), XMVectorGetZ(check[k]), XMVectorGetW(check[k]), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    //XMVector3ClampLengthV
    for (k = 0; k < countof(v); k++) {
        r = XMVector3ClampLengthV(v[k], N[k], X[k]);
        c = CompareXMVECTOR(r, check[k], 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[k]), XMVectorGetY(v[k]), XMVectorGetZ(v[k]), XMVectorGetW(v[k]),
                n[k], x[k],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[k]), XMVectorGetY(check[k]), XMVectorGetZ(check[k]), XMVectorGetW(check[k]), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test225(LogProxy* pLog)
{
    //XMVector3ComponentsFromNormal 
    HRESULT ret = S_OK;
    COMPARISON c;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 3; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        v2.v = XMVectorSetW(v2, c_Q_NAN);

        float dot = XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2);
        XMVECTOR check1 = XMVectorScale(v2, dot);
        XMVECTOR check2 = XMVectorSubtract(v1, check1);
        XMVECTOR r1, r2;
        XMVector3ComponentsFromNormal(&r1, &r2, v1, v2);
        c = CompareXMVECTOR(r1, check1, 3);
        if (c > WITHIN4096) {
            printe("%s: (1) %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r1), XMVectorGetY(r1), XMVectorGetZ(r1), XMVectorGetW(r1),
                XMVectorGetX(check1), XMVectorGetY(check1), XMVectorGetZ(check1), XMVectorGetW(check1), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
        c = CompareXMVECTOR(r2, check2, 3);
        if (c > WITHIN4096) {
            printe("%s: (1) %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r2), XMVectorGetY(r2), XMVectorGetZ(r2), XMVectorGetW(r2),
                XMVectorGetX(check2), XMVectorGetY(check2), XMVectorGetZ(check2), XMVectorGetW(check2), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test226(LogProxy* pLog)
{
    //XMVector3Cross 
    HRESULT ret = S_OK;
    COMPARISON c;
    int k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        v2.v = GetRandomVector16();
        XMVECTORF32 check = { {XMVectorGetY(v1) * XMVectorGetZ(v2) - XMVectorGetZ(v1) * XMVectorGetY(v2), XMVectorGetZ(v1) * XMVectorGetX(v2) - XMVectorGetX(v1) * XMVectorGetZ(v2), XMVectorGetX(v1) * XMVectorGetY(v2) - XMVectorGetY(v1) * XMVectorGetX(v2), 0} };
        XMVECTOR r = XMVector3Cross(v1, v2);
        c = CompareXMVECTOR(r, check, 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f x %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test227(LogProxy* pLog)
{
    //XMVector3Dot 
    XMVECTORF32 l = {};
    XMVECTORF32 v = {};
    COMPARISON c = EXACT, temp;
    HRESULT ret = S_OK;
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 4; j++) {
            l.v = XMVectorSetByIndex(l, ((float)XM_RAND()) / 1000.f, j);
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 1000.f, j);
        }
        l.v = XMVectorSetW(l, c_Q_NAN);
        v.v = XMVectorSetW(v, c_Q_NAN);
        XMVECTOR r = XMVector3Dot(l, v);
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(l) * XMVectorGetX(v) + XMVectorGetY(l) * XMVectorGetY(v) + XMVectorGetZ(l) * XMVectorGetZ(v));
        temp = CompareXMVECTOR(r, check, 4);
        c = std::max(c, temp);
        if (temp > WITHIN4096) {
            printe("%s: %f %f %f %f dot %f %f %f %f = %f ... %f (%d)\n",
                TestName, XMVectorGetX(l), XMVectorGetY(l), XMVectorGetZ(l), XMVectorGetW(l),
                XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetX(check), temp);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, temp);
        }
    }
    return ret;
}
HRESULT Test228(LogProxy* pLog)
{
    //XMVector3Equal 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j < 2) v2.v = XMVectorSetW(v2, c_Q_NAN);
        check = TRUE;
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); check = FALSE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, c_Q_NAN); }
        else {}
        r = XMVector3Equal(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = TRUE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); check = FALSE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, c_Q_NAN); }
        else {}
        r = XMVector3Equal(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test229(LogProxy* pLog)
{
    //XMVector3Greater 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector3Greater(v1, v2);
        check = ((j & 14) == 14) ? TRUE : FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        check = FALSE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        r = XMVector3Greater(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test230(LogProxy* pLog)
{
    //XMVector3GreaterOrEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector3GreaterOrEqual(v1, v2);
        check = TRUE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = (j & 14) ? FALSE : TRUE;
        r = XMVector3GreaterOrEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test231(LogProxy* pLog)
{
    //XMVector3IsInfinite 
    HRESULT ret = S_OK;
    static const float f[] = { c_INF, -c_INF, c_Q_NAN, 1,-1 };
    int r, g, b, a;
    XMVECTOR col;
    BOOL res, check;
    for (r = 0; r < countof(f); r++) {
        for (g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for (b = 0; b < countof(f); b++) {
                for (a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r], f[g], f[b], f[a]);
                    check = (r < 2 || g < 2 || b < 2) ? TRUE : FALSE;
                    res = XMVector3IsInfinite(col);
                    if (res != check) {
                        printe("\n%s: %f %f %f %f = %d ... %d\n",
                            TestName, XMVectorGetX(col), XMVectorGetY(col), XMVectorGetZ(col), XMVectorGetW(col), res, check);
                        ret = MATH_FAIL;
                    }
                    else {
                        //printi("%d  ", res);
                    }
                }
            }
        }
    }
    printi("\n");
    return ret;
}

ISNAN_TEST_BEGIN
HRESULT Test232(LogProxy* pLog)
{
    //XMVector3IsNaN 
    HRESULT ret = S_OK;
    static const float f[] = { c_Q_NAN, c_NAN, c_INF, 1,-1 };
    int r, g, b, a;
    XMVECTOR col;
    BOOL res, check;
    for (r = 0; r < countof(f); r++) {
        for (g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for (b = 0; b < countof(f); b++) {
                for (a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r], f[g], f[b], f[a]);
                    check = (r < 2 || g < 2 || b < 2) ? TRUE : FALSE;
                    res = XMVector3IsNaN(col);
                    if (res != check) {
                        printe("\n%s: %f %f %f %f = %d ... %d\n",
                            TestName, XMVectorGetX(col), XMVectorGetY(col), XMVectorGetZ(col), XMVectorGetW(col), res, check);
                        ret = MATH_FAIL;
                    }
                    else {
                        //printi("%d  ", res);
                    }
                }
            }
        }
    }
    printi("\n");
    return ret;
}
ISNAN_TEST_END

HRESULT Test233(LogProxy* pLog)
{
    //XMVector3Length 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1 = {};
    XMVECTOR r = XMVector3Length(XMVectorZero());
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1)));
        r = XMVector3Length(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}
HRESULT Test234(LogProxy* pLog)
{
    //XMVector3LengthSq 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1 = {};
    XMVECTOR r = XMVector3LengthSq(XMVectorZero());
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate((XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1)));
        r = XMVector3LengthSq(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}
HRESULT Test235(LogProxy* pLog)
{
    //XMVector3Less 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector3Less(v1, v2);
        check = FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = ((j & 14) == 14) ? TRUE : FALSE;
        r = XMVector3Less(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test236(LogProxy* pLog)
{
    //XMVector3LessOrEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector3LessOrEqual(v1, v2);
        check = (j & 14) ? FALSE : TRUE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = TRUE;
        r = XMVector3LessOrEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test237(LogProxy* pLog)
{
    //XMVector3LinePointDistance 
    static const XMVECTORF32 v1[] = {
        {2,2,2,c_Q_NAN},{0,0,0},{0,2,0},{6,2,-3},{0,0,0},{0,0, 0},{1,1,1}
    };
    static const XMVECTORF32 v2[] = {
        {0,0,0,c_Q_NAN},{2,2,2},{2,0,2},{1,2, 4},{2,2,2},{3,4, 0},{4,5,1}
    };
    static const XMVECTORF32 v3[] = {
        {1,1,1,c_Q_NAN},{1,1,1},{1,1,1},{6,2,-3},{0,2,4},{10,5,0},{3,12,1}
    };

    static const float check[] = {
        0,0,0,0,sqrtf(8.f),5,5
    };
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;

    for (k = 0; k < countof(v1); k++) {
        r = XMVector3LinePointDistance(v1[k], v2[k], v3[k]);
        c = CompareXMVECTOR(r, XMVectorReplicate(check[k]), 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: (%d)\n", TestName, c);
            dqe(v1[k]); dqe(v2[k]); dqe(v3[k]); printe("="); dqe(r); printe("..."); dqe(XMVectorReplicate(check[k]));
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;

}
HRESULT Test238(LogProxy* pLog)
{
    //XMVector3NearEqual 
    HRESULT ret = S_OK;
    XMVECTORF32 v1 = {}, v2 = {}, e = {};
    BOOL r, check;
    int i, j, k;
    for (j = 0; j < 16; j++) {
        for (k = 0; k < 2; k++) {
            for (i = 0; i < 4; i++) {
                v1.v = XMVectorSetByIndex(v1, (float)(XM_RAND()) / 1000.f, i);
                e.v = XMVectorSetByIndex(e, (float)(XM_RAND()) / 10000.f, i);
                v2.v = XMVectorSetByIndex(v2, XMVectorGetByIndex(v1, i) + ((k & 1) ? 1.f : -1.f) * ((j & (1 << i)) ? (XMVectorGetByIndex(e, i) + XMVectorGetByIndex(e, i)) : (XMVectorGetByIndex(e, i) / 2.f)), i);
            }
            check = ((j & 7) ? FALSE : TRUE);
            r = XMVector3NearEqual(v1, v2, e);
            if (r != check) {
                printe("%s: j%d k%d %f %f %f %f, %f %f %f %f, %f %f %f %f = %x ... %x\n",
                    TestName, j, k, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                    XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                    XMVectorGetX(e), XMVectorGetY(e), XMVectorGetZ(e), XMVectorGetW(e), r, check);
                ret = MATH_FAIL;
            }
            else {
                //				printi("%s: j%d k%d %f %f %f %f, %f %f %f %f, %f %f %f %f = %x ... %x\n",
                //					TestName,j,k, XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                //                  XMVectorGetX(v2),XMVectorGetY(v2),XMVectorGetZ(v2),XMVectorGetW(v2),
                //                  XMVectorGetX(e),XMVectorGetY(e),XMVectorGetZ(e),XMVectorGetW(e), r, check);
                printi("%s: j%d k%d (0)\n", TestName, j, k);
            }
        }
    }
    return ret;
}
HRESULT Test239(LogProxy* pLog)
{
    //XMVector3Normalize 
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

        XMVECTOR r = XMVector3Normalize(v1);
        c = CompareXMVECTOR(r, check, 3);
        if (c > WITHIN4096) {
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
#ifdef __INTEL_LLVM_COMPILER
        static const float xval[] = { 0.f,  -0.f,   c_INF,  -c_INF };
        static const float cval[] = { 0.f,   0.f, c_Q_NAN, c_Q_NAN };
#else
        static const float xval[] = { 0.f,  -0.f,   c_INF,  -c_INF, c_Q_NAN };
        static const float cval[] = { 0.f,   0.f, c_Q_NAN, c_Q_NAN, c_Q_NAN };
#endif

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVector3Normalize(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
            c = CompareXMVECTOR(r, check, 4);
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
HRESULT Test240(LogProxy* pLog)
{
    //XMVector3NotEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector3NotEqual(v1, v2);
        check = (j & 14) ? TRUE : FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = (j & 14) ? TRUE : FALSE;
        r = XMVector3NotEqual(v1, v2);
        if (r != check) {
            printi("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test241(LogProxy* pLog)
{
    //XMVector3Orthogonal 
    HRESULT ret = S_OK;
    int k;
    XMVECTOR zero = XMVectorZero();

    for (k = 1; k < (3 * 3 * 3); k++) {
        static const float Table[3] = { 0.0f,-1.0f,1.0f };
        float vx = Table[k % 3];
        float vy = Table[(k / 3) % 3];
        float vz = Table[(k / 9)];    // It's %3 by default since it's max is 27
        XMVECTOR v = XMVectorSet(vx, vy, vz, c_Q_NAN);
        XMVECTOR r = XMVector3Orthogonal(v);
        float rx = XMVectorGetX(r);
        float ry = XMVectorGetY(r);
        float rz = XMVectorGetZ(r);
        float dot = vx * rx + vy * ry + vz * rz;

        COMPARISON c = Compare(dot, 0);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %+.0f %+.0f %+.0f = %+.0f %+.0f %+.0f. Dot: %f ... 0.0f\n",
                TestName,
                vx, vy, vz,
                rx, ry, rz, dot);
            ret = MATH_FAIL;
        }
        c = CompareXMVECTOR(r, zero, 3);
        if (c <= WITHINBIGEPSILON) {
            printi("%s: error: zero vector produced:  %+.0f %+.0f %+.0f =  %+.0f %+.0f %+.0f\n",
                TestName,
                vx, vy, vz,
                rx, ry, rz);
            ret = MATH_FAIL;
        }
    }

    for (k = 0; k < 10; k++) {
        XMVECTOR v = GetRandomVector16();
        XMVECTOR r = XMVector3Orthogonal(v);
        float vx = XMVectorGetX(v);
        float vy = XMVectorGetY(v);
        float vz = XMVectorGetZ(v);
        float rx = XMVectorGetX(r);
        float ry = XMVectorGetY(r);
        float rz = XMVectorGetZ(r);
        float dot = vx * rx + vy * ry + vz * rz;
        COMPARISON c = Compare(dot, 0);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f = %f %f %f. Dot: %f ... 0\n",
                TestName,
                vx, vy, vz,
                rx, ry, rz, dot);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}

HRESULT Test242(LogProxy* pLog)
{
    //XMVector3Project 
    float vx;
    float vy;
    float vw;
    float vh;
    float nz;
    float xz;
    XMMATRIX pr;
    XMMATRIX vi;
    XMMATRIX wo;

    XMVECTOR v;
    XMVECTOR r;
    XMVECTOR check;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMMATRIX temp;

    for (int i = 0; i < 16; i++) {
        vx = (float)(XM_RAND() % 1000);
        vy = (float)(XM_RAND() % 1000);
        vw = (float)(XM_RAND() % 1000);
        vh = (float)(XM_RAND() % 1000);
        nz = GetRandomFloat16();
        xz = GetRandomFloat16();
        pr = GetRandomMatrix4();
        vi = GetRandomMatrix4();
        wo = GetRandomMatrix4();
        v = GetRandomVector16();
        v = XMVectorSetW(v, c_Q_NAN);

        temp = XMMatrixIdentity();
        if (i & 2) {
            temp *= wo;
        }
        if (i & 4) {
            temp *= vi;
        }
        if (i & 8) {
            temp *= pr;
        }
        check = XMVector3TransformCoord(v, temp);
        //		if(i&1) {
        check = XMVectorSetX(check, (XMVectorGetX(check) + 1.0f) * 0.5f * (float)vw + (float)vx);
        check = XMVectorSetY(check, (-XMVectorGetY(check) + 1.0f) * 0.5f * (float)vh + (float)vy);
        check = XMVectorSetZ(check, XMVectorGetZ(check) * (xz - nz) + nz);
        //		}

        r = XMVector3Project(v, vx, vy, vw, vh, nz, xz,
            (i & 8) ? pr : XMMatrixIdentity(), (i & 4) ? vi : XMMatrixIdentity(),
            (i & 2) ? wo : XMMatrixIdentity());

        c = CompareXMVECTOR(r, check, 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %d: (%d)\n", TestName, i, c);
            dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d %d\n", TestName, i, c);
        }
    }
    return ret;
}
HRESULT Test243(LogProxy* pLog)
{
    const uint32_t dwDataSize = 10000;
    const uint32_t dwNumItems = 200;

    //XMVector3ProjectStream 
    static const int ins[] = { 12,16,20,32,24, };
    static const int outs[] = { 12,16,32,44,28 };

    float vx;
    float vy;
    float vw;
    float vh;
    float nz;
    float xz;
    XMMATRIX pr;
    XMMATRIX vi;
    XMMATRIX wo;
    XMMATRIX temp;

    XMVECTOR v[dwNumItems];
    XMVECTOR check[dwNumItems];
    HRESULT ret = S_OK;
    int i, n;

    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;

    BOOL OutWriteCombined = FALSE;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;

    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
    {
        AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
        if (!pbReal1 || !pbSandbox1)
            goto Cleanup;

        for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
            if (!pbReal3 || !pbSandbox3)
                goto Cleanup;

            for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
            {
                for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                {
                    i = XM_RAND() % 16;

                    vx = (float)(XM_RAND() % 1000);
                    vy = (float)(XM_RAND() % 1000);
                    vw = (float)(XM_RAND() % 1000);
                    vh = (float)(XM_RAND() % 1000);
                    nz = GetRandomFloat16();
                    xz = GetRandomFloat16();
                    pr = GetRandomMatrix4();
                    vi = GetRandomMatrix4();
                    wo = GetRandomMatrix4();

                    temp = XMMatrixIdentity();
                    if (i & 2) {
                        temp *= wo;
                    }
                    if (i & 4) {
                        temp *= vi;
                    }
                    if (i & 8) {
                        temp *= pr;
                    }

                    for (n = 0; n < dwNumItems; n++) {
                        v[n] = GetRandomVector16();
                        v[n] = XMVectorSetW(v[n], c_Q_NAN);
                        check[n] = XMVector3TransformCoord(v[n], temp);
                        check[n] = XMVectorSetX(check[n], (XMVectorGetX(check[n]) + 1.0f) * 0.5f * (float)vw + (float)vx);
                        check[n] = XMVectorSetY(check[n], (-XMVectorGetY(check[n]) + 1.0f) * 0.5f * (float)vh + (float)vy);
                        check[n] = XMVectorSetZ(check[n], XMVectorGetZ(check[n]) * (xz - nz) + nz);
                    }
                    fillsandbox(pbSandbox1, dwDataSize, v, 16, ins[InStrideIndex], dwNumItems);
                    fillsandbox(pbSandbox2, dwDataSize, check, 16, outs[OutStrideIndex], dwNumItems);
                    initsandbox(pbSandbox3, dwDataSize);

                    const XMFLOAT3* pointer = XMVector3ProjectStream((XMFLOAT3*)&pbSandbox3[0],
                        outs[OutStrideIndex], (const XMFLOAT3*)&pbSandbox1[0], ins[InStrideIndex], dwNumItems,
                        vx, vy, vw, vh, nz, xz,
                        (i & 8) ? pr : XMMatrixIdentity(), (i & 4) ? vi : XMMatrixIdentity(),
                        (i & 2) ? wo : XMMatrixIdentity());

                    if (pointer != (const XMFLOAT3*)&pbSandbox3[0]) {
                        printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                        ret = MATH_FAIL;
                    }
                    if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 16, dwNumItems, dwDataSize, 3, WITHIN1_64))
                    {
                        printe("%s: ins:%d, outs:%d, count:%u", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                        ret = MATH_FAIL;
                    }
                }//end "for OutStride"
            }//end  "for InStride"
            FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
            pbReal3 = pbSandbox3 = nullptr;
        }//end "for OutAlignIndex"
        FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
        pbReal1 = pbSandbox1 = nullptr;
    }//end "for InAlignIndex"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
    return ret;
}
HRESULT Test244(LogProxy* pLog)
{
    //XMVector3ReciprocalLength 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1 = {};

    for (k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1)));
        XMVECTOR r = XMVector3ReciprocalLength(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}

HRESULT Test245(LogProxy* pLog)
{
    //XMVector3Reflect 
    XMVECTORF32 v1 = {}, v2 = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    int i, k;
    for (k = 0; k < 15; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVectorSetW(v1, c_Q_NAN);
        v2.v = XMVectorSetW(v2, c_Q_NAN);
        // Result = Incident - (2 * dot(Incident, Normal)) * Normal
        float dot = XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2);
        XMVECTOR check = XMVectorSubtract(v1, XMVectorScale(v2, 2 * dot));
        XMVECTOR r = XMVector3Reflect(v1, v2);
        c = CompareXMVECTOR(r, check, 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f %f %f %f  = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test246(LogProxy* pLog)
{
    //XMVector3Refract and XMVector3RefractV
    XMVECTORF32 v1 = {}, v2 = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    float f;
    int i, k;

    //XMVector3Refract
    {
        for (k = 0; k < 15; k++) {
            for (i = 0; i < 4; i++) {
                v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
                v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
            }
            v1.v = XMVectorSetW(v1, c_Q_NAN);
            v2.v = XMVectorSetW(v2, c_Q_NAN);
            f = ((float)XM_RAND()) / 2000.f - 8.f;
            // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
            // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
            float dotIN = XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2);
            float scale = (f * dotIN + sqrtf(1 - f * f * (1 - dotIN * dotIN)));
            XMVECTOR check = XMVectorSubtract(XMVectorScale(v1, f), XMVectorScale(v2, scale));
            if ((1 - f * f * (1 - dotIN * dotIN)) < 0) check = XMVectorZero();
            XMVECTOR r = XMVector3Refract(v1, v2, f);
            c = CompareXMVECTOR(r, check, 3);
            if (c > WITHINBIGEPSILON) {
                printe("%s (%d): %f %f %f %f  %f %f %f %f  %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, k, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                    XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2), f,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
            else {
                printi("%s: %d\n", TestName, c);
            }
        }//notest (need to check more for total internal refraction,
        //and also for sqrt(0) cases
    }

    //XMVector3RefractV
    {
        XMVECTORF32 V = { 1,1,1,1 };
        for (k = 0; k < 15; k++) {
            for (i = 0; i < 4; i++) {
                v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
                v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
                V.v = XMVectorSetByIndex(V, std::max(((float)XM_RAND()) / 2000.f - 8.f, 0.f), i);
            }

            v1.v = XMVectorSetW(v1, c_Q_NAN);
            v2.v = XMVectorSetW(v2, c_Q_NAN);

            // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
            // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
            float dotIN = XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2);
            XMVECTOR Vsqrt2 = XMVectorSubtract(g_XMOne, XMVectorMultiply(V, XMVectorScale(V, 1 - dotIN * dotIN)));
            XMVECTOR check;
            if ((XMVectorGetX(Vsqrt2) < 0) && (XMVectorGetY(Vsqrt2) < 0) && (XMVectorGetZ(Vsqrt2) < 0) && (XMVectorGetW(Vsqrt2) < 0))
            {
                check = XMVectorZero();
            }
            else
            {
                // V*v1-v2*(V*dotIN+ XMVectorSqrt(Vsqrt2));
                check = XMVectorSubtract(XMVectorMultiply(V, v1), XMVectorMultiply(v2, XMVectorAdd(XMVectorScale(V, dotIN), XMVectorSqrt(Vsqrt2))));
            }
            XMVECTOR r = XMVector3RefractV(v1, v2, V);
            c = CompareXMVECTOR(r, check, 3);
            if (c > WITHINBIGEPSILON) {
                printe("%s (%d): %f %f %f %f  %f %f %f %f    %f  %f %f %f= %f %f %f %f ... %f %f %f %f (V %d)\n",
                    TestName, k, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                    XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                    XMVectorGetX(V), XMVectorGetY(V), XMVectorGetZ(V), XMVectorGetW(V),
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
            else {
                printi("%s: %d\n", TestName, c);
            }
        }//notest (need to check more for total internal refraction,
        //and also for sqrt(0) cases
    }

    return ret;
}
HRESULT Test247(LogProxy* pLog)
{
    //XMVector3Transform 
    XMVECTORF32 v = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float tmp[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
            }
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        v.v = XMVectorSetW(v, c_Q_NAN);
        FLOAT checkx = XMVectorGetX(v) * tmp[0][0] + XMVectorGetY(v) * tmp[1][0] + XMVectorGetZ(v) * tmp[2][0] + tmp[3][0];
        FLOAT checky = XMVectorGetX(v) * tmp[0][1] + XMVectorGetY(v) * tmp[1][1] + XMVectorGetZ(v) * tmp[2][1] + tmp[3][1];
        FLOAT checkz = XMVectorGetX(v) * tmp[0][2] + XMVectorGetY(v) * tmp[1][2] + XMVectorGetZ(v) * tmp[2][2] + tmp[3][2];
        FLOAT checkw = XMVectorGetX(v) * tmp[0][3] + XMVectorGetY(v) * tmp[1][3] + XMVectorGetZ(v) * tmp[2][3] + tmp[3][3];
        XMVECTOR check = XMVectorSet(checkx, checky, checkz, checkw);
        XMVECTOR r = XMVector3Transform(v, m);
        c = CompareXMVECTOR(r, check, 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f *   (%d)\n", TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); dqe(r); printe("..."); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test248(LogProxy* pLog)
{
    //XMVector3TransformCoord 
    XMVECTORF32 v = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float tmp[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
            }
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        v.v = XMVectorSetW(v, c_Q_NAN);
        FLOAT checkx = XMVectorGetX(v) * tmp[0][0] + XMVectorGetY(v) * tmp[1][0] + XMVectorGetZ(v) * tmp[2][0] + tmp[3][0];
        FLOAT checky = XMVectorGetX(v) * tmp[0][1] + XMVectorGetY(v) * tmp[1][1] + XMVectorGetZ(v) * tmp[2][1] + tmp[3][1];
        FLOAT checkz = XMVectorGetX(v) * tmp[0][2] + XMVectorGetY(v) * tmp[1][2] + XMVectorGetZ(v) * tmp[2][2] + tmp[3][2];
        FLOAT checkw = XMVectorGetX(v) * tmp[0][3] + XMVectorGetY(v) * tmp[1][3] + XMVectorGetZ(v) * tmp[2][3] + tmp[3][3];
        checkx *= (1 / (checkw));
        checky *= (1 / (checkw));
        checkz *= (1 / (checkw));
        XMVECTOR check = XMVectorSet(checkx, checky, checkz, 1);

        XMVECTOR r = XMVector3TransformCoord(v, m);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f *   (%d)\n", TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); dqe(r); printe("..."); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test249(LogProxy* pLog)
{
    const uint32_t dwDataSize = 5000;
    const uint32_t dwNumItems = 50;

    //XMVector3TransformCoordStream 
    static const int ins[] = { 12,16,20,32,24 };
    static const int outs[] = { 12,16,32,44,28 };
    XMVECTOR v[dwNumItems];
    XMVECTOR check[dwNumItems];
    HRESULT ret = S_OK;
    int n;
    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;
    BOOL OutWriteCombined = FALSE;

    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (OutWriteCombined = 0; OutWriteCombined < 2; OutWriteCombined++)
    {
        for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
            if (!pbReal1 || !pbSandbox1)
                goto Cleanup;

            for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
            {
                AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
                if (!pbReal3 || !pbSandbox3)
                    goto Cleanup;
                for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
                {
                    for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                    {
                        float tmp[4][4];
                        for (int i = 0; i < 4; i++)
                        {
                            //Fill up a matrix (m) with random junk.
                            for (int j = 0; j < 4; j++)
                            {
                                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
                            }

                            //Fill up all (dwNumItems) vectors (v) with stuff.
                            for (n = 0; n < dwNumItems; n++)
                            {
                                v[n] = XMVectorSetByIndex(v[n], ((float)XM_RAND()) / 2000.f - 8.f, i);
                                v[n] = XMVectorSetW(v[n], c_Q_NAN);
                            }
                        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                        XMMATRIX m(&tmp[0][0]);
                        for (n = 0; n < dwNumItems; n++)
                        {
                            XMVECTOR vv = v[n];
                            FLOAT checkx = XMVectorGetX(vv) * tmp[0][0] + XMVectorGetY(vv) * tmp[1][0] + XMVectorGetZ(vv) * tmp[2][0] + tmp[3][0];
                            FLOAT checky = XMVectorGetX(vv) * tmp[0][1] + XMVectorGetY(vv) * tmp[1][1] + XMVectorGetZ(vv) * tmp[2][1] + tmp[3][1];
                            FLOAT checkz = XMVectorGetX(vv) * tmp[0][2] + XMVectorGetY(vv) * tmp[1][2] + XMVectorGetZ(vv) * tmp[2][2] + tmp[3][2];
                            FLOAT checkw = XMVectorGetX(vv) * tmp[0][3] + XMVectorGetY(vv) * tmp[1][3] + XMVectorGetZ(vv) * tmp[2][3] + tmp[3][3];
                            checkx *= (1 / (checkw));
                            checky *= (1 / (checkw));
                            checkz *= (1 / (checkw));
                            check[n] = XMVectorSet(checkx, checky, checkz, 1);
                        }
                        fillsandbox(pbSandbox1, dwDataSize, v, 16, ins[InStrideIndex], dwNumItems);
                        fillsandbox(pbSandbox2, dwDataSize, check, 16, outs[OutStrideIndex], dwNumItems);
                        initsandbox(pbSandbox3, dwDataSize);
                        const XMFLOAT3* pointer = XMVector3TransformCoordStream((XMFLOAT3*)&pbSandbox3[0], outs[OutStrideIndex], (const XMFLOAT3*)&pbSandbox1[0], ins[InStrideIndex], dwNumItems, m);

                        if (pointer != (const XMFLOAT3*)&pbSandbox3[0]) {
                            printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                            ret = MATH_FAIL;
                        }
                        if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 16, dwNumItems, dwDataSize, 3, WITHIN1_64))
                        {
                            printe("%s: ins:%d, outs:%d, count:%u\n", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                            ret = MATH_FAIL;
                        }
                    } //end "for OutStrideIndex"
                }//end "for InStrideIndex"
                FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
                pbReal3 = pbSandbox3 = nullptr;
            }//end "for OutAlignIndex"
            FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
            pbReal1 = pbSandbox1 = nullptr;
        }//end "for InAlignIndex"
    }//end "for OutWriteCombined"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);

    return ret;
}
HRESULT Test250(LogProxy* pLog)
{
    //XMVector3TransformNormal 
    XMVECTORF32 v = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float tmp[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
            }
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        v.v = XMVectorSetW(v, c_Q_NAN);
        FLOAT checkx = XMVectorGetX(v) * tmp[0][0] + XMVectorGetY(v) * tmp[1][0] + XMVectorGetZ(v) * tmp[2][0];
        FLOAT checky = XMVectorGetX(v) * tmp[0][1] + XMVectorGetY(v) * tmp[1][1] + XMVectorGetZ(v) * tmp[2][1];
        FLOAT checkz = XMVectorGetX(v) * tmp[0][2] + XMVectorGetY(v) * tmp[1][2] + XMVectorGetZ(v) * tmp[2][2];
        FLOAT checkw = XMVectorGetX(v) * tmp[0][3] + XMVectorGetY(v) * tmp[1][3] + XMVectorGetZ(v) * tmp[2][3];
        XMVECTOR check = XMVectorSet(checkx, checky, checkz, checkw);
        XMVECTOR r = XMVector3TransformNormal(v, m);
        c = CompareXMVECTOR(r, check, 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f *   (%d)\n", TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); dqe(r); printe("..."); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test251(LogProxy* pLog)
{
    //XMVector3TransformNormalStream 
    const uint32_t dwDataSize = 10000;
    const uint32_t dwNumItems = 200;

    static const int ins[] = { 12,16,20,32,24 };
    static const int outs[] = { 12,16,32,44,28 };
    XMVECTOR v[dwNumItems];
    XMVECTOR check[dwNumItems];
    HRESULT ret = S_OK;
    int n;

    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;

    BOOL OutWriteCombined = FALSE;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;


    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
    {
        AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
        if (!pbReal1 || !pbSandbox1)
            goto Cleanup;

        for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
            if (!pbReal3 || !pbSandbox3)
                goto Cleanup;


            for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
            {
                for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                {
                    float tmp[4][4];
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
                        }
                        for (n = 0; n < dwNumItems; n++) {
                            v[n] = XMVectorSetByIndex(v[n], ((float)XM_RAND()) / 2000.f - 8.f, i);
                            v[n] = XMVectorSetW(v[n], c_Q_NAN);
                        }
                    }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                    XMMATRIX m(&tmp[0][0]);
                    for (n = 0; n < dwNumItems; n++) {
                        XMVECTOR vv = v[n];
                        FLOAT checkx = XMVectorGetX(vv) * tmp[0][0] + XMVectorGetY(vv) * tmp[1][0] + XMVectorGetZ(vv) * tmp[2][0];
                        FLOAT checky = XMVectorGetX(vv) * tmp[0][1] + XMVectorGetY(vv) * tmp[1][1] + XMVectorGetZ(vv) * tmp[2][1];
                        FLOAT checkz = XMVectorGetX(vv) * tmp[0][2] + XMVectorGetY(vv) * tmp[1][2] + XMVectorGetZ(vv) * tmp[2][2];
                        check[n] = XMVectorSet(checkx, checky, checkz, XMVectorGetW(check[n]));
                    }
                    fillsandbox(pbSandbox1, dwDataSize, v, 16, ins[InStrideIndex], dwNumItems);
                    fillsandbox(pbSandbox2, dwDataSize, check, 16, outs[OutStrideIndex], dwNumItems);
                    initsandbox(pbSandbox3, dwDataSize);
                    const XMFLOAT3* pointer = XMVector3TransformNormalStream((XMFLOAT3*)&pbSandbox3[0], outs[OutStrideIndex], (const XMFLOAT3*)&pbSandbox1[0], ins[InStrideIndex], dwNumItems, m);

                    if (pointer != (const XMFLOAT3*)&pbSandbox3[0]) {
                        printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                        ret = MATH_FAIL;
                    }
                    if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 16, dwNumItems, dwDataSize, 3)) {
                        printe("%s: ins:%d, outs:%d, count:%u\n", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                        printmatrixe(m);
                        ret = MATH_FAIL;
                    }
                }//end "for OutStride"
            }//end  "for InStride"
            FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
            pbReal3 = pbSandbox3 = nullptr;
        }//end "for OutAlignIndex"
        FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
        pbReal1 = pbSandbox1 = nullptr;
    }//end "for InAlignIndex"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);

    return ret;
}



HRESULT Test252(LogProxy* pLog)
{
    const uint32_t dwDataSize = 5000;
    const uint32_t dwNumItems = 50;
    //XMVector3TransformStream and XMVector3TransformStreamNC
    static const int ins[] = { 12, 16,20,32,24 };
    static const int outs[] = { 16, 16,32,44,28 };
    XMVECTOR v[dwNumItems];
    XMVECTOR check[dwNumItems];
    HRESULT ret = S_OK;
    int n;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;
    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    BOOL OutWriteCombined = FALSE;
    BOOL TestingNCVersion = FALSE;

    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (TestingNCVersion = 0; TestingNCVersion < 1; TestingNCVersion++)
    {
        for (OutWriteCombined = 0; OutWriteCombined < 2; OutWriteCombined++)
        {
            for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
            {
                AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
                if (!pbReal1 || !pbSandbox1)
                    goto Cleanup;

                for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
                {
                    AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
                    if (!pbReal3 || !pbSandbox3)
                        goto Cleanup;


                    for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
                    {
                        for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                        {

                            //wc memory and misaligned data don't mix.
                            if (OutWriteCombined)
                                if (!TestingNCVersion)
                                    if (outs[OutStrideIndex] % 16 || g_iStartAlignments[OutAlignIndex] % 16)
                                        continue;

                            //Fill up a matrix (m) with random junk.
                            float tmp[4][4];
                            for (int i = 0; i < 4; i++)
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
                                }

                                //Fill up all (dwNumItems) vectors (v) with stuff.
                                for (n = 0; n < dwNumItems; n++)
                                {
                                    v[n] = XMVectorSetByIndex(v[n], ((float)XM_RAND()) / 2000.f - 8.f, i);
                                    v[n] = XMVectorSetW(v[n], c_Q_NAN);
                                }
                            }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                            XMMATRIX m(&tmp[0][0]);

                            //Now fill out the "answers".
                            for (n = 0; n < dwNumItems; n++)
                            {
                                XMVECTOR vv = v[n];
                                FLOAT checkx = XMVectorGetX(vv) * tmp[0][0] + XMVectorGetY(vv) * tmp[1][0] + XMVectorGetZ(vv) * tmp[2][0] + tmp[3][0];
                                FLOAT checky = XMVectorGetX(vv) * tmp[0][1] + XMVectorGetY(vv) * tmp[1][1] + XMVectorGetZ(vv) * tmp[2][1] + tmp[3][1];
                                FLOAT checkz = XMVectorGetX(vv) * tmp[0][2] + XMVectorGetY(vv) * tmp[1][2] + XMVectorGetZ(vv) * tmp[2][2] + tmp[3][2];
                                FLOAT checkw = XMVectorGetX(vv) * tmp[0][3] + XMVectorGetY(vv) * tmp[1][3] + XMVectorGetZ(vv) * tmp[2][3] + tmp[3][3];
                                check[n] = XMVectorSet(checkx, checky, checkz, checkw);

                            }

                            //Now spread these out into a sandbox
                            fillsandbox(pbSandbox1, dwDataSize, v, 16, ins[InStrideIndex], dwNumItems);
                            fillsandbox(pbSandbox2, dwDataSize, check, 16, outs[OutStrideIndex], dwNumItems);
                            initsandbox(pbSandbox3, dwDataSize);
                            const XMFLOAT4* pointer = nullptr;
                            if (!TestingNCVersion)
                                pointer = XMVector3TransformStream((XMFLOAT4*)&pbSandbox3[0], outs[OutStrideIndex], (const XMFLOAT3*)&pbSandbox1[0], ins[InStrideIndex], dwNumItems, m);

                            if (pointer != (const XMFLOAT4*)&pbSandbox3[0]) {
                                printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                                ret = MATH_FAIL;
                            }
                            if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 16, dwNumItems, dwDataSize, 4)) {
                                printe("%s: ins:%d, outs:%d, count:%u\n", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                                ret = MATH_FAIL;
                            }
                        }
                    }
                    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
                    pbReal3 = pbSandbox3 = nullptr;
                }//end "for OutAlignIndex"
                FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
                pbReal1 = pbSandbox1 = nullptr;
            }//end "for InAlignIndex"
        }//end "for OutWriteCombined"
    }//end "TestingNCVersion"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
    return ret;
}



HRESULT Test253(LogProxy* pLog)
{
    //XMVector3Unproject 
    float vx;
    float vy;
    float vw;
    float vh;
    float nz;
    float xz;
    XMMATRIX pr;
    XMMATRIX vi;
    XMMATRIX wo;

    XMVECTOR v;
    XMVECTOR r;
    XMVECTOR check;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMMATRIX temp;

    for (int i = 0; i < 16; i++) {
        vx = (float)(XM_RAND() % 1000);
        vy = (float)(XM_RAND() % 1000);
        vw = (float)(XM_RAND() % 1000);
        vh = (float)(XM_RAND() % 1000);
        nz = GetRandomFloat16();
        xz = GetRandomFloat16();
        pr = GetRandomMatrix4();
        vi = GetRandomMatrix4();
        wo = GetRandomMatrix4();
        v = GetRandomVector16();
        v = XMVectorSetW(v, c_Q_NAN);

        temp = XMMatrixIdentity();
        if (i & 2) {
            temp *= wo;
        }
        if (i & 4) {
            temp *= vi;
        }
        if (i & 8) {
            temp *= pr;
        }
        temp = XMMatrixInverse(&check, temp);
        check = v;
        if (i & 1) {
            check = XMVectorSetX(check, (XMVectorGetX(v) - (float)vx) / (float)vw * 2 - 1);
            check = XMVectorSetY(check, -((XMVectorGetY(v) - (float)vy) / (float)vh * 2 - 1));
            check = XMVectorSetZ(check, (XMVectorGetZ(v) - nz) / (xz - nz));
        }
        check = XMVector3TransformCoord(check, temp);

        r = XMVector3Unproject(v, (i & 1) ? (float)vx : -1, (i & 1) ? (float)vy : 1, (i & 1) ? (float)vw : 2,
            (i & 1) ? (float)vh : -2, (i & 1) ? nz : 0, (i & 1) ? xz : 1,
            (i & 8) ? pr : XMMatrixIdentity(), (i & 4) ? vi : XMMatrixIdentity(),
            (i & 2) ? wo : XMMatrixIdentity());

        c = CompareXMVECTOR(r, check, 3);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %d: (%d)\n", TestName, i, c);
            dqe(r); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d %d\n", TestName, i, c);
        }
    }
    return ret;
}
HRESULT Test254(LogProxy* pLog)
{
    //XMVector3UnprojectStream 
    const uint32_t dwDataSize = 10000;
    const uint32_t dwNumItems = 200;


    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;
    static const int ins[] = { 12,16,20,32,24, };
    static const int outs[] = { 12,16,32,44,28 };

    float vx;
    float vy;
    float vw;
    float vh;
    float nz;
    float xz;
    XMMATRIX pr;
    XMMATRIX vi;
    XMMATRIX wo;
    XMMATRIX temp;
    XMVECTOR v[dwNumItems];
    XMVECTOR check[dwNumItems];
    HRESULT ret = S_OK;

    BOOL OutWriteCombined = FALSE;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;
    int i, n;

    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
    {
        AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
        if (!pbReal1 || !pbSandbox1)
            goto Cleanup;

        for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
            if (!pbReal3 || !pbSandbox3)
                goto Cleanup;

            for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
            {
                for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                {

                    i = XM_RAND() % 16;
                    vx = (float)(XM_RAND() % 1000);
                    vy = (float)(XM_RAND() % 1000);
                    vw = (float)(XM_RAND() % 1000);
                    vh = (float)(XM_RAND() % 1000);
                    nz = GetRandomFloat16();
                    xz = GetRandomFloat16();
                    pr = GetRandomMatrix4();
                    vi = GetRandomMatrix4();
                    wo = GetRandomMatrix4();

                    temp = XMMatrixIdentity();
                    if (i & 2) {
                        temp *= wo;
                    }
                    if (i & 4) {
                        temp *= vi;
                    }
                    if (i & 8) {
                        temp *= pr;
                    }
                    XMVECTOR blah;
                    temp = XMMatrixInverse(&blah, temp);

                    for (n = 0; n < dwNumItems; n++) {
                        v[n] = GetRandomVector16();
                        v[n] = XMVectorSetW(v[n], c_Q_NAN);
                        check[n] = v[n];
                        if (i & 1) {
                            check[n] = XMVectorSetX(check[n], (XMVectorGetX(v[n]) - (float)vx) / (float)vw * 2 - 1);
                            check[n] = XMVectorSetY(check[n], -((XMVectorGetY(v[n]) - (float)vy) / (float)vh * 2 - 1));
                            check[n] = XMVectorSetZ(check[n], (XMVectorGetZ(v[n]) - nz) / (xz - nz));
                        }
                        check[n] = XMVector3TransformCoord(check[n], temp);
                    }
                    fillsandbox(pbSandbox1, dwDataSize, v, 16, ins[InStrideIndex], dwNumItems);
                    fillsandbox(pbSandbox2, dwDataSize, check, 16, outs[OutStrideIndex], dwNumItems);
                    initsandbox(pbSandbox3, dwDataSize);

                    const XMFLOAT3* pointer = XMVector3UnprojectStream((XMFLOAT3*)&pbSandbox3[0], outs[OutStrideIndex], (const XMFLOAT3*)&pbSandbox1[0], ins[InStrideIndex],
                        dwNumItems, (i & 1) ? (float)vx : -1, (i & 1) ? (float)vy : 1, (i & 1) ? (float)vw : 2,
                        (i & 1) ? (float)vh : -2, (i & 1) ? nz : 0, (i & 1) ? xz : 1,
                        (i & 8) ? pr : XMMatrixIdentity(), (i & 4) ? vi : XMMatrixIdentity(),
                        (i & 2) ? wo : XMMatrixIdentity());

                    if (pointer != (const XMFLOAT3*)&pbSandbox3[0]) {
                        printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                        ret = MATH_FAIL;
                    }
                    if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 16, dwNumItems, dwDataSize, 3, WITHIN1_256)) {
                        printe("%s: ins:%d, outs:%d, count:%u\n", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                        ret = MATH_FAIL;
                    }
                }//end "for OutstrideIndex"
            }//end "for InStrideIndex"
            FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
            pbReal3 = pbSandbox3 = nullptr;
        }//end "for OutAlignIndex"
        FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
        pbReal1 = pbSandbox1 = nullptr;
    }//end "for InAlignIndex"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
    return ret;
}



HRESULT Test255(LogProxy* pLog)
{
    //XMVector4AngleBetweenNormals 
    HRESULT ret = S_OK;
    COMPARISON c;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVector4Normalize(v1);
        v2.v = XMVector4Normalize(v2);
        XMVECTOR check = XMVectorReplicate(acosf(XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2) + XMVectorGetW(v1) * XMVectorGetW(v2)));
        XMVECTOR r = XMVector4AngleBetweenNormals(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test256(LogProxy* pLog)
{
    //XMVector4AngleBetweenVectors 
    HRESULT ret = S_OK;
    COMPARISON c;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        XMVECTOR check = XMVectorReplicate(acosf((XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2) + XMVectorGetW(v1) * XMVectorGetW(v2)) / (sqrtf((XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1) + XMVectorGetW(v1) * XMVectorGetW(v1)) * (XMVectorGetX(v2) * XMVectorGetX(v2) + XMVectorGetY(v2) * XMVectorGetY(v2) + XMVectorGetZ(v2) * XMVectorGetZ(v2) + XMVectorGetW(v2) * XMVectorGetW(v2))))));
        XMVECTOR r = XMVector4AngleBetweenVectors(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test257(LogProxy* pLog)
{
    //XMVector4ClampLength and XMVector4ClampLengthV
    static const XMVECTORF32 v[] = {
        {2,2,2,2},{2,3,-4,5}, {-1,2,2,4},          {0,0,0,0}, {c_INF,c_INF,c_INF,c_INF},{3,4,12,84}, {5,12,84,3612}
    };
    static const float n[] = {
        0,        0,          c_INF,                4,          4,                   1,           3613
    };
    static const XMVECTORF32 N[] = {
        {0,0,0,0}, {0,0,0,0}, {c_INF,c_INF,c_INF,c_INF}, {4,4,4,4}, {4,4,4,4}, {1,1,1,1}, {3613,3613,3613,3613}
    };
    static const float x[] = {
        0,        c_INF,       c_INF,                5,          4,                   42.5,        3700
    };
    static const XMVECTORF32 X[] = {
        {0,0,0,0}, {c_INF,c_INF,c_INF,c_INF}, {c_INF,c_INF,c_INF,c_INF}, {5,5,5,5}, {4,4,4,4}, {42.5, 42.5, 42.5, 42.5}, {3700,3700,3700,3700}
    };
    static const XMVECTORF32 check[] = {
        {0,0,0,0},{2,3,-4,5},{-c_INF,c_INF,c_INF,c_INF},{0,0,0,0}, {c_INF,c_INF,c_INF,c_INF},{1.5,2,6,42},{5,12,84,3612}
    };
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;

    //XMVector4ClampLength
    for (k = 0; k < countof(v); k++) {
        r = XMVector4ClampLength(v[k], n[k], x[k]);
        c = CompareXMVECTOR(r, check[k], 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[k]), XMVectorGetY(v[k]), XMVectorGetZ(v[k]), XMVectorGetW(v[k]), n[k], x[k],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[k]), XMVectorGetY(check[k]), XMVectorGetZ(check[k]), XMVectorGetW(check[k]), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    //XMVector4ClampLengthV
    for (k = 0; k < countof(v); k++) {
        r = XMVector4ClampLengthV(v[k], N[k], X[k]);
        c = CompareXMVECTOR(r, check[k], 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[k]), XMVectorGetY(v[k]), XMVectorGetZ(v[k]), XMVectorGetW(v[k]), n[k], x[k],
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check[k]), XMVectorGetY(check[k]), XMVectorGetZ(check[k]), XMVectorGetW(check[k]), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test258(LogProxy* pLog)
{
    //XMVector4Cross 
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {}, v3 = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    float dot;

    for (k = 0; k < 15; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 4000.f - 4.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 4000.f - 4.f, i);
            v3.v = XMVectorSetByIndex(v3, ((float)XM_RAND()) / 4000.f - 4.f, i);
        }
        FLOAT checkx = XMVectorGetY(v1) * (XMVectorGetZ(v2) * XMVectorGetW(v3) - XMVectorGetZ(v3) * XMVectorGetW(v2)) -
            XMVectorGetZ(v1) * (XMVectorGetY(v2) * XMVectorGetW(v3) - XMVectorGetY(v3) * XMVectorGetW(v2)) +
            XMVectorGetW(v1) * (XMVectorGetY(v2) * XMVectorGetZ(v3) - XMVectorGetY(v3) * XMVectorGetZ(v2));

        FLOAT checky = XMVectorGetX(v1) * (XMVectorGetZ(v3) * XMVectorGetW(v2) - XMVectorGetZ(v2) * XMVectorGetW(v3)) -
            XMVectorGetZ(v1) * (XMVectorGetX(v3) * XMVectorGetW(v2) - XMVectorGetX(v2) * XMVectorGetW(v3)) +
            XMVectorGetW(v1) * (XMVectorGetX(v3) * XMVectorGetZ(v2) - XMVectorGetX(v2) * XMVectorGetZ(v3));

        FLOAT checkz = XMVectorGetX(v1) * (XMVectorGetY(v2) * XMVectorGetW(v3) - XMVectorGetY(v3) * XMVectorGetW(v2)) -
            XMVectorGetY(v1) * (XMVectorGetX(v2) * XMVectorGetW(v3) - XMVectorGetX(v3) * XMVectorGetW(v2)) +
            XMVectorGetW(v1) * (XMVectorGetX(v2) * XMVectorGetY(v3) - XMVectorGetX(v3) * XMVectorGetY(v2));

        FLOAT checkw = XMVectorGetX(v1) * (XMVectorGetY(v3) * XMVectorGetZ(v2) - XMVectorGetY(v2) * XMVectorGetZ(v3)) -
            XMVectorGetY(v1) * (XMVectorGetX(v3) * XMVectorGetZ(v2) - XMVectorGetX(v2) * XMVectorGetZ(v3)) +
            XMVectorGetZ(v1) * (XMVectorGetX(v3) * XMVectorGetY(v2) - XMVectorGetX(v2) * XMVectorGetY(v3));
        XMVECTOR check = XMVectorSet(checkx, checky, checkz, checkw);
        XMVECTOR r = XMVector4Cross(v1, v2, v3);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: ", TestName);
            dqe(v1); dqe(v2); dqe(v3); printe("="); dqe(r); printe("..."); dqe(check); printe("%d\n", c);
            ret = MATH_FAIL;
        }
        dot = XMVectorGetX(XMVector4Dot(v1, r));
        c = Compare(dot, 0);
        if (c > WITHINBIGEPSILON) {
            printe("%s: ", TestName);
            dqe(v1); dqe(v2); dqe(v3); printe("="); dqe(r); printe("dot1: %f ... 0\n", dot);
            ret = MATH_FAIL;
        }
        dot = XMVectorGetX(XMVector4Dot(v2, r));
        c = Compare(dot, 0);
        if (c > WITHINBIGEPSILON) {
            printe("%s: ", TestName);
            dqe(v1); dqe(v2); dqe(v3); printe("="); dqe(r); printe("dot2: %f ... 0\n", dot);
            ret = MATH_FAIL;
        }
        dot = XMVectorGetX(XMVector4Dot(v3, r));
        c = Compare(dot, 0);
        if (c > WITHINBIGEPSILON) {
            printe("%s: ", TestName);
            dqe(v1); dqe(v2); dqe(v3); printe("="); dqe(r); printe("dot3: %f ... 0\n", dot);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test259(LogProxy* pLog)
{
    //XMVector4Dot 
    XMVECTORF32 l = {};
    XMVECTORF32 v = {};
    COMPARISON c = EXACT, temp;
    HRESULT ret = S_OK;
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 4; j++) {
            l.v = XMVectorSetByIndex(l, ((float)XM_RAND()) / 1000.f, j);
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 1000.f, j);
        }
        XMVECTOR r = XMVector4Dot(l, v);
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(l) * XMVectorGetX(v) + XMVectorGetY(l) * XMVectorGetY(v) + XMVectorGetZ(l) * XMVectorGetZ(v) + XMVectorGetW(l) * XMVectorGetW(v));
        temp = CompareXMVECTOR(r, check, 4);
        c = std::max(c, temp);
        if (temp > WITHIN4096) {
            printe("%s: %f %f %f %f dot %f %f %f %f = %f ... %f (%d)\n",
                TestName, XMVectorGetX(l), XMVectorGetY(l), XMVectorGetZ(l), XMVectorGetW(l),
                XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetX(check), temp);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, temp);
        }
    }
    return ret;
}
HRESULT Test260(LogProxy* pLog)
{
    //XMVector4Equal 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        check = TRUE;
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); check = FALSE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); check = FALSE; }
        else {}
        r = XMVector4Equal(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = TRUE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); check = FALSE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); check = FALSE; }
        else {}
        r = XMVector4Equal(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test261(LogProxy* pLog)
{
    //XMVector4Greater 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector4Greater(v1, v2);
        check = (j == 15) ? TRUE : FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        check = FALSE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        r = XMVector4Greater(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test262(LogProxy* pLog)
{
    //XMVector4GreaterOrEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector4GreaterOrEqual(v1, v2);
        check = TRUE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = (j) ? FALSE : TRUE;
        r = XMVector4GreaterOrEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test263(LogProxy* pLog)
{
    //XMVector4IsInfinite 
    HRESULT ret = S_OK;
    static const float f[] = { c_INF, -c_INF, c_Q_NAN, 1,-1 };
    int r, g, b, a;
    XMVECTOR col;
    BOOL res, check;
    for (r = 0; r < countof(f); r++) {
        for (g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for (b = 0; b < countof(f); b++) {
                for (a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r], f[g], f[b], f[a]);
                    check = (r < 2 || g < 2 || b < 2 || a < 2) ? TRUE : FALSE;
                    res = XMVector4IsInfinite(col);
                    if (res != check) {
                        printe("\n%s: %f %f %f %f = %d ... %d\n",
                            TestName, XMVectorGetX(col), XMVectorGetY(col), XMVectorGetZ(col), XMVectorGetW(col), res, check);
                        ret = MATH_FAIL;
                    }
                }
            }
        }
    }
    printi("\n");
    return ret;
}

ISNAN_TEST_BEGIN
HRESULT Test264(LogProxy* pLog)
{
    //XMVector4IsNaN 
    HRESULT ret = S_OK;
    static const float f[] = { c_Q_NAN, c_NAN, c_INF, 1,-1 };
    int r, g, b, a;
    XMVECTOR col;
    BOOL res, check;
    for (r = 0; r < countof(f); r++) {
        for (g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for (b = 0; b < countof(f); b++) {
                for (a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r], f[g], f[b], f[a]);
                    check = (r < 2 || g < 2 || b < 2 || a < 2) ? TRUE : FALSE;
                    res = XMVector4IsNaN(col);
                    if (res != check) {
                        printe("\n%s: %f %f %f %f = %d ... %d\n",
                            TestName, XMVectorGetX(col), XMVectorGetY(col), XMVectorGetZ(col), XMVectorGetW(col), res, check);
                        ret = MATH_FAIL;
                    }
                    else {
                        //printi("%d  ", res);
                    }
                }
            }
        }
    }
    printi("\n");
    return ret;
}
ISNAN_TEST_END

HRESULT Test265(LogProxy* pLog)
{
    //XMVector4Length 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMVector4Length(XMVectorZero());
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate(sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1) + XMVectorGetW(v1) * XMVectorGetW(v1)));
        XMVECTOR r = XMVector4Length(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}
HRESULT Test266(LogProxy* pLog)
{
    //XMVector4LengthSq 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMVector4LengthSq(XMVectorZero());
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate((XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1) + XMVectorGetW(v1) * XMVectorGetW(v1)));
        r = XMVector4LengthSq(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}
HRESULT Test267(LogProxy* pLog)
{
    //XMVector4Less 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector4Less(v1, v2);
        check = FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = (j == 15) ? TRUE : FALSE;
        r = XMVector4Less(v1, v2);
        if (r != check) {
            printi("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test268(LogProxy* pLog)
{
    //XMVector4LessOrEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector4LessOrEqual(v1, v2);
        check = (j) ? FALSE : TRUE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = TRUE;
        r = XMVector4LessOrEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test269(LogProxy* pLog)
{
    //XMVector4NearEqual 
    HRESULT ret = S_OK;
    XMVECTORF32 v1 = {}, v2 = {}, e = {};
    BOOL r, check;
    int i, j, k;
    for (j = 0; j < 16; j++) {
        for (k = 0; k < 2; k++) {
            for (i = 0; i < 4; i++) {
                v1.v = XMVectorSetByIndex(v1, (float)(XM_RAND()) / 1000.f, i);
                e.v = XMVectorSetByIndex(e, (float)(XM_RAND()) / 10000.f, i);
                v2.v = XMVectorSetByIndex(v2, XMVectorGetByIndex(v1, i) + ((k & 1) ? 1.f : -1.f) * ((j & (1 << i)) ? (XMVectorGetByIndex(e, i) + XMVectorGetByIndex(e, i)) : (XMVectorGetByIndex(e, i) / 2.f)), i);
            }
            check = ((j) ? FALSE : TRUE);
            r = XMVector4NearEqual(v1, v2, e);
            if (r != check) {
                printe("%s: j%d k%d %f %f %f %f, %f %f %f %f, %f %f %f %f = %x ... %x\n",
                    TestName, j, k, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                    XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                    XMVectorGetX(e), XMVectorGetY(e), XMVectorGetZ(e), XMVectorGetW(e), r, check);
                ret = MATH_FAIL;
            }
            else {
                //				printi("%s: j%d k%d %f %f %f %f, %f %f %f %f, %f %f %f %f = %x ... %x\n",
                //					TestName,j,k, XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                //                  XMVectorGetX(v2),XMVectorGetY(v2),XMVectorGetZ(v2),XMVectorGetW(v2),
                //                  XMVectorGetX(e),XMVectorGetY(e),XMVectorGetZ(e),XMVectorGetW(e), r, check);
                printi("%s: j%d k%d (0)\n", TestName, j, k);
            }
        }
    }
    return ret;
}
HRESULT Test270(LogProxy* pLog)
{
    //XMVector4Normalize and  XMVector4NormalizeEst
    HRESULT ret = S_OK;

    //XMVector4Normalize
    {
        int i, k;
        COMPARISON c;
        XMVECTORF32 check = {};
        float rcplength;

        for (k = 0; k < 10; k++) {
            XMVECTOR v1 = GetRandomVector16();
            rcplength = 1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1) + XMVectorGetW(v1) * XMVectorGetW(v1));
            for (i = 0; i < 4; i++) {
                check.v = XMVectorSetByIndex(check, XMVectorGetByIndex(v1, i) * rcplength, i);
            }

            XMVECTOR r = XMVector4Normalize(v1);
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHIN4096) {
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
    }

    // Test special cases around 0, -0, +INF, -INF, c_Q_NAN
    {
#ifdef __INTEL_LLVM_COMPILER
        static const float xval[] = { 0.f,  -0.f,   c_INF,  -c_INF };
        static const float cval[] = { 0.f,   0.f, c_Q_NAN, c_Q_NAN };
#else
        static const float xval[] = { 0.f,  -0.f,   c_INF,  -c_INF, c_Q_NAN };
        static const float cval[] = { 0.f,   0.f, c_Q_NAN, c_Q_NAN, c_Q_NAN };
#endif

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVector4Normalize(v);
            XMVECTOR check = XMVectorReplicate(cval[i]);
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

    //XMVector4NormalizeEst
    {
        int i, k;
        COMPARISON c;
        XMVECTORF32 check = {};
        float rcplength;

        for (k = 0; k < 10; k++) {
            XMVECTOR v1 = GetRandomVector16();
            rcplength = 1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1) + XMVectorGetW(v1) * XMVectorGetW(v1));
            for (i = 0; i < 4; i++) {
                check.v = XMVectorSetByIndex(check, XMVectorGetByIndex(v1, i) * rcplength, i);
            }

            XMVECTOR r = XMVector4NormalizeEst(v1);
            c = CompareXMVECTOR(r, check, 4);
            if (c > WITHINHUGEEPSILON) {
                printe("%s: Est %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
            else {
                printi("%s: %d\n", TestName, c);
            }
        }
    }

    // Test special cases around 0, -0, +INF, -INF, c_Q_NAN
    {
        static const float xval[] = { 0.f,  -0.f, c_INF, -c_INF, c_Q_NAN };

        for (size_t i = 0; i < std::size(xval); ++i)
        {
            float x = xval[i];
            XMVECTOR v = XMVectorReplicate(x);
            XMVECTOR r = XMVector4NormalizeEst(v);
            XMVECTOR check = XMVectorSplatQNaN();
            COMPARISON c = CompareXMVECTOR(r, check, 4);
            if (c > EXACT)
            {
                printe("%s: Est %f = %f %f %f %f...%f %f %f %f %d\n", TestName, x,
                    XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                    XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}
HRESULT Test271(LogProxy* pLog)
{
    //XMVector4NotEqual 
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) + .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) + .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) + .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) + .01f); }
        else {}
        r = XMVector4NotEqual(v1, v2);
        check = (j) ? TRUE : FALSE;
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }

        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetX(v1, XMVectorGetX(v2) - .01f); }
        else {}
        if (j & 4) { v1.v = XMVectorSetY(v1, XMVectorGetY(v2) - .01f); }
        else {}
        if (j & 2) { v1.v = XMVectorSetZ(v1, XMVectorGetZ(v2) - .01f); }
        else {}
        if (j & 1) { v1.v = XMVectorSetW(v1, XMVectorGetW(v2) - .01f); }
        else {}
        check = (j) ? TRUE : FALSE;
        r = XMVector4NotEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test272(LogProxy* pLog)
{
    //XMVector4Orthogonal 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r;
    XMVECTOR v;
    for (int k = 0; k < 15; k++) {
        v = GetRandomVector16();
        r = XMVector4Orthogonal(v);
        XMVECTORF32 check = { {XMVectorGetZ(v), XMVectorGetW(v), -XMVectorGetX(v), -XMVectorGetY(v)} };
        float checkdot = XMVectorGetX(v) * XMVectorGetX(r) + XMVectorGetY(v) * XMVectorGetY(r) + XMVectorGetZ(v) * XMVectorGetZ(r) + XMVectorGetW(v) * XMVectorGetW(r);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
        c = Compare(checkdot, 0);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f = %f %f %f %f. dot(%f) ... 0 (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), checkdot, c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test273(LogProxy* pLog)
{
    //XMVector4ReciprocalLength 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;

    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate(1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1) + XMVectorGetW(v1) * XMVectorGetW(v1)));
        XMVECTOR r = XMVector4ReciprocalLength(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
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
    return ret;
}
HRESULT Test274(LogProxy* pLog)
{
    //XMVector4Reflect 
    XMVECTORF32 v1 = {}, v2 = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    int i, k;
    for (k = 0; k < 15; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        // Result = Incident - (2 * dot(Incident, Normal)) * Normal
        float dot = XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2) + XMVectorGetW(v1) * XMVectorGetW(v2);
        XMVECTOR check = XMVectorSubtract(v1, XMVectorScale(v2, 2 * dot));
        XMVECTOR r = XMVector4Reflect(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f %f %f %f  = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test275(LogProxy* pLog)
{
    //XMVector4Refract and XMVector4RefractV 
    XMVECTORF32 v1 = {}, v2 = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    float f;
    int i, k;

    //XMVector4Refract
    for (k = 0; k < 15; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        f = ((float)XM_RAND()) / 2000.f - 8.f;
        // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
        // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
        float dot = XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2) + XMVectorGetW(v1) * XMVectorGetW(v2);
        XMVECTOR check = XMVectorSubtract(
            XMVectorScale(v1, f),
            XMVectorScale(v2, f * dot + sqrtf(1 - f * f * (1 - dot * dot))));
        if ((1 - f * f * (1 - dot * dot)) < 0)
        {
            check = XMVectorZero();
        }
        XMVECTOR r = XMVector4Refract(v1, v2, f);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f %f %f %f  %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2), f,
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }//notest (need to check more for total internal refraction,
    //and also for sqrt(0) cases

    //XMVector4RefractV
    XMVECTORF32 V = { 1,1,1,1 };
    for (k = 0; k < 15; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
            V.v = XMVectorSetByIndex(V, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }

        // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
        // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
        float dot = XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2) + XMVectorGetW(v1) * XMVectorGetW(v2);
        XMVECTOR Vsqrt2 = XMVectorSubtract(g_XMOne, XMVectorMultiply(V, XMVectorScale(V, 1 - dot * dot)));
        XMVECTOR check;
        if (XMVectorGetX(Vsqrt2) < 0 || XMVectorGetY(Vsqrt2) < 0 || XMVectorGetZ(Vsqrt2) < 0 || XMVectorGetW(Vsqrt2) < 0)
        {
            continue;
        }
        else
        {
            // V * v1 - v2 * (V * dot + XMVectorSqrt(Vsqrt2));
            check = XMVectorSubtract(
                XMVectorMultiply(V, v1),
                XMVectorMultiply(
                    v2,
                    XMVectorAdd(XMVectorScale(V, dot), XMVectorSqrt(Vsqrt2))));
        }
        XMVECTOR r = XMVector4RefractV(v1, v2, V);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f %f %f %f    %f  %f %f %f= %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(V), XMVectorGetY(V), XMVectorGetZ(V), XMVectorGetW(V),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }//notest (need to check more for total internal refraction,
    //and also for sqrt(0) cases

    return ret;
}
HRESULT Test276(LogProxy* pLog)
{
    //XMVector4Transform 
    XMVECTORF32 v = {};
    COMPARISON c;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float tmp[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
            }
            v.v = XMVectorSetByIndex(v, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        FLOAT checkx = XMVectorGetX(v) * tmp[0][0] + XMVectorGetY(v) * tmp[1][0] + XMVectorGetZ(v) * tmp[2][0] + XMVectorGetW(v) * tmp[3][0];
        FLOAT checky = XMVectorGetX(v) * tmp[0][1] + XMVectorGetY(v) * tmp[1][1] + XMVectorGetZ(v) * tmp[2][1] + XMVectorGetW(v) * tmp[3][1];
        FLOAT checkz = XMVectorGetX(v) * tmp[0][2] + XMVectorGetY(v) * tmp[1][2] + XMVectorGetZ(v) * tmp[2][2] + XMVectorGetW(v) * tmp[3][2];
        FLOAT checkw = XMVectorGetX(v) * tmp[0][3] + XMVectorGetY(v) * tmp[1][3] + XMVectorGetZ(v) * tmp[2][3] + XMVectorGetW(v) * tmp[3][3];
        XMVECTOR check = XMVectorSet(checkx, checky, checkz, checkw);
        XMVECTOR r = XMVector4Transform(v, m);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f *   (%d)\n", TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); dqe(r); printe("..."); dqe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test277(LogProxy* pLog)
{
    //XMVector4TransformStream 
    const uint32_t dwDataSize = 10000;
    const uint32_t dwNumItems = 200;

    static const int ins[] = { 16,20,32,24, };
    static const int outs[] = { 16,32,44,28 };
    XMVECTOR v[dwNumItems];
    XMVECTOR check[dwNumItems];
    HRESULT ret = S_OK;
    int n;

    uint8_t* pbSandbox1 = nullptr;
    uint8_t* pbSandbox2 = nullptr;
    uint8_t* pbSandbox3 = nullptr;
    uint8_t* pbReal1 = nullptr;
    uint8_t* pbReal2 = nullptr;
    uint8_t* pbReal3 = nullptr;

    BOOL OutWriteCombined = FALSE;
    int InAlignIndex = 0;
    int OutAlignIndex = 0;
    int InStrideIndex = 0;
    int OutStrideIndex = 0;


    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
    {
        AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbReal1, (LPVOID*)&pbSandbox1);
        if (!pbReal1 || !pbSandbox1)
            goto Cleanup;

        for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbReal3, (LPVOID*)&pbSandbox3);
            if (!pbReal3 || !pbSandbox3)
                goto Cleanup;

            for (InStrideIndex = 0; InStrideIndex < countof(ins); InStrideIndex++)
            {
                for (OutStrideIndex = 0; OutStrideIndex < countof(outs); OutStrideIndex++)
                {
                    float tmp[4][4];
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            tmp[i][j] = ((float)XM_RAND()) / 2000.f - 8.f;
                        }
                        for (n = 0; n < dwNumItems; n++) {
                            v[n] = XMVectorSetByIndex(v[n], ((float)XM_RAND()) / 2000.f - 8.f, i);
                        }
                    }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                    XMMATRIX m(&tmp[0][0]);
                    for (n = 0; n < dwNumItems; n++) {
                        XMVECTOR vv = v[n];
                        FLOAT checkx = XMVectorGetX(vv) * tmp[0][0] + XMVectorGetY(vv) * tmp[1][0] + XMVectorGetZ(vv) * tmp[2][0] + XMVectorGetW(vv) * tmp[3][0];
                        FLOAT checky = XMVectorGetX(vv) * tmp[0][1] + XMVectorGetY(vv) * tmp[1][1] + XMVectorGetZ(vv) * tmp[2][1] + XMVectorGetW(vv) * tmp[3][1];
                        FLOAT checkz = XMVectorGetX(vv) * tmp[0][2] + XMVectorGetY(vv) * tmp[1][2] + XMVectorGetZ(vv) * tmp[2][2] + XMVectorGetW(vv) * tmp[3][2];
                        FLOAT checkw = XMVectorGetX(vv) * tmp[0][3] + XMVectorGetY(vv) * tmp[1][3] + XMVectorGetZ(vv) * tmp[2][3] + XMVectorGetW(vv) * tmp[3][3];
                        check[n] = XMVectorSet(checkx, checky, checkz, checkw);

                    }
                    fillsandbox(pbSandbox1, dwDataSize, v, 16, ins[InStrideIndex], dwNumItems);
                    fillsandbox(pbSandbox2, dwDataSize, check, 16, outs[OutStrideIndex], dwNumItems);
                    initsandbox(pbSandbox3, dwDataSize);
                    const XMFLOAT4* pointer = XMVector4TransformStream((XMFLOAT4*)&pbSandbox3[0], outs[OutStrideIndex], (const XMFLOAT4*)&pbSandbox1[0], ins[InStrideIndex], dwNumItems, m);

                    if (pointer != (const XMFLOAT4*)&pbSandbox3[0]) {
                        printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                        ret = MATH_FAIL;
                    }
                    if (!checksandbox(pLog, pbSandbox3, pbSandbox2, outs[OutStrideIndex], 16, dwNumItems, dwDataSize, 4)) {
                        printe("%s: ins:%d, outs:%d, count:%u\n", TestName, ins[InStrideIndex], outs[OutStrideIndex], dwNumItems);
                        ret = MATH_FAIL;
                    }
                }//end "for OutStride"
            }//end  "for InStride"
            FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
            pbReal3 = pbSandbox3 = nullptr;
        }//end "for OutAlignIndex"
        FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
        pbReal1 = pbSandbox1 = nullptr;
    }//end "for InAlignIndex"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);
    return ret;
}

HRESULT Test442(LogProxy* pLog)
{
    //XMVector2AngleBetweenNormalsEst
    HRESULT ret = S_OK;
    COMPARISON c;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVector2Normalize(v1);
        v2.v = XMVector2Normalize(v2);
        XMVECTOR check = XMVectorReplicate(acosf(XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2)));
        XMVECTOR r = XMVector2AngleBetweenNormalsEst(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGGEREPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}

HRESULT Test443(LogProxy* pLog)
{
    //XMVector3AngleBetweenNormalsEst 
    HRESULT ret = S_OK;
    COMPARISON c;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVector3Normalize(v1);
        v2.v = XMVector3Normalize(v2);
        XMVECTOR check = XMVectorReplicate(acosf(XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2)));
        XMVECTOR r = XMVector3AngleBetweenNormalsEst(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGGEREPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}

HRESULT Test444(LogProxy* pLog)
{
    //XMVector4AngleBetweenNormalsEst 
    HRESULT ret = S_OK;
    COMPARISON c;
    int i, k;
    XMVECTORF32 v1 = {}, v2 = {};
    for (k = 0; k < 10; k++) {
        for (i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1, ((float)XM_RAND()) / 2000.f - 8.f, i);
            v2.v = XMVectorSetByIndex(v2, ((float)XM_RAND()) / 2000.f - 8.f, i);
        }
        v1.v = XMVector4Normalize(v1);
        v2.v = XMVector4Normalize(v2);
        XMVECTOR check = XMVectorReplicate(acosf(XMVectorGetX(v1) * XMVectorGetX(v2) + XMVectorGetY(v1) * XMVectorGetY(v2) + XMVectorGetZ(v1) * XMVectorGetZ(v2) + XMVectorGetW(v1) * XMVectorGetW(v2)));
        XMVECTOR r = XMVector4AngleBetweenNormalsEst(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGGEREPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}


HRESULT Test445(LogProxy* pLog)
{
    //XMVector2ReciprocalLengthEst
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;

    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        v1 = XMVectorSetZ(v1, c_Q_NAN);
        v1 = XMVectorSetW(v1, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1)));
        XMVECTOR r = XMVector2ReciprocalLengthEst(v1);
        c = CompareXMVECTOR(r, check, 4);
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
    return ret;
}
HRESULT Test446(LogProxy* pLog)
{
    //XMVector2LengthEst
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMVector2LengthEst(XMVectorZero());
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        v1 = XMVectorSetZ(v1, c_Q_NAN);
        v1 = XMVectorSetW(v1, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1)));
        r = XMVector2LengthEst(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN1_256) {
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
    return ret;
}
HRESULT Test447(LogProxy* pLog)
{
    //XMVector3ReciprocalLengthEst
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;

    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        v1 = XMVectorSetW(v1, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1)));
        XMVECTOR r = XMVector3ReciprocalLengthEst(v1);
        c = CompareXMVECTOR(r, check, 4);
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
    return ret;
}
HRESULT Test448(LogProxy* pLog)
{
    //XMVector3LengthEst
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMVector3LengthEst(XMVectorZero());
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        v1 = XMVectorSetW(v1, c_Q_NAN);
        XMVECTOR check = XMVectorReplicate(sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1)));
        r = XMVector3LengthEst(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN1_256) {
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
    return ret;
}
HRESULT Test449(LogProxy* pLog)
{
    //XMVector4ReciprocalLengthEst
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;

    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate(1.f / sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1) + XMVectorGetW(v1) * XMVectorGetW(v1)));
        XMVECTOR r = XMVector4ReciprocalLengthEst(v1);
        c = CompareXMVECTOR(r, check, 4);
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
    return ret;
}
HRESULT Test450(LogProxy* pLog)
{
    //XMVector4LengthEst
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMVector4LengthEst(XMVectorZero());
    c = CompareXMVECTOR(r, XMVectorZero(), 4);
    if (c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r));
        ret = MATH_FAIL;
    }
    else {
        printi("%s: 0 = 0\n", TestName);
    }
    for (k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate(sqrtf(XMVectorGetX(v1) * XMVectorGetX(v1) + XMVectorGetY(v1) * XMVectorGetY(v1) + XMVectorGetZ(v1) * XMVectorGetZ(v1) + XMVectorGetW(v1) * XMVectorGetW(v1)));
        r = XMVector4LengthEst(v1);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN1_256) {
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
    return ret;
}


HRESULT Test461(LogProxy* pLog)
{
    //XMVector2EqualInt
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        check = TRUE;
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) + 1); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) + 1); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) + 1); }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) + 1); }
        else {}
        r = XMVector2EqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = TRUE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) - 1); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) - 1); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) - 1); }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) - 1); }
        else {}
        r = XMVector2EqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test462(LogProxy* pLog)
{
    //XMVector2NotEqualInt
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        check = FALSE;
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) + 1); check = TRUE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) + 1); check = TRUE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) + 1); }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) + 1); }
        else {}
        r = XMVector2NotEqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = FALSE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) - 1); check = TRUE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) - 1); check = TRUE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) - 1); }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) - 1); }
        else {}
        r = XMVector2NotEqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test463(LogProxy* pLog)
{
    //XMVector3EqualInt
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        check = TRUE;
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) + 1); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) + 1); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) + 1); check = FALSE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) + 1); }
        else {}
        r = XMVector3EqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = TRUE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) - 1); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) - 1); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) - 1); check = FALSE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) - 1); }
        else {}
        r = XMVector3EqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test464(LogProxy* pLog)
{
    //XMVector3NotEqualInt
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        check = FALSE;
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) + 1); check = TRUE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) + 1); check = TRUE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) + 1); check = TRUE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) + 1); }
        else {}
        r = XMVector3NotEqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = FALSE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) - 1); check = TRUE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) - 1); check = TRUE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) - 1); check = TRUE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) - 1); }
        else {}
        r = XMVector3NotEqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test465(LogProxy* pLog)
{
    //XMVector4EqualInt
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        check = TRUE;
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) ^ 1); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) ^ 1); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) ^ 1); check = FALSE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) ^ 1); check = FALSE; }
        else {}
        r = XMVector4EqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = TRUE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) ^ -1); check = FALSE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) ^ -1); check = FALSE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) ^ -1); check = FALSE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) ^ -1); check = FALSE; }
        else {}
        r = XMVector4EqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test466(LogProxy* pLog)
{
    //XMVector4NotEqualInt
    XMVECTORF32 v1 = {}, v2 = {};
    BOOL r, check;
    int i, j;
    HRESULT ret = S_OK;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        check = FALSE;
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) + 1); check = TRUE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) + 1); check = TRUE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) + 1); check = TRUE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) + 1); check = TRUE; }
        else {}
        r = XMVector4NotEqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
        check = FALSE;
        for (i = 0; i < 4; i++) {
            float ftemp = (float)XM_RAND();
            v1.v = XMVectorSetByIndex(v1, ftemp, i);
            v2.v = XMVectorSetByIndex(v2, ftemp, i);
        }
        if (j & 8) { v1.v = XMVectorSetIntX(v1, XMVectorGetIntX(v2) - 1); check = TRUE; }
        else {}
        if (j & 4) { v1.v = XMVectorSetIntY(v1, XMVectorGetIntY(v2) - 1); check = TRUE; }
        else {}
        if (j & 2) { v1.v = XMVectorSetIntZ(v1, XMVectorGetIntZ(v2) - 1); check = TRUE; }
        else {}
        if (j & 1) { v1.v = XMVectorSetIntW(v1, XMVectorGetIntW(v2) - 1); check = TRUE; }
        else {}
        r = XMVector4NotEqualInt(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test473(LogProxy* pLog)
{
    //XMVector2EqualR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x == v2x && v1y == v2y) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x != v2x && v1y != v2y) crcheck |= XM_CRMASK_CR6FALSE;

        uint32_t cr = XMVector2EqualR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test474(LogProxy* pLog)
{
    //XMVector2EqualIntR
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
        const FloatPairs_t* pEntry = &TableX[i & 7];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 8) & 7];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 64) /* &7 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() & 7];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x == v2x && v1y == v2y) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x != v2x && v1y != v2y) crcheck |= XM_CRMASK_CR6FALSE;

        uint32_t cr = XMVector2EqualIntR(v1, v2);
        if ((cr & XM_CRMASK_CR6BOUNDS) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w,
                cr & XM_CRMASK_CR6BOUNDS, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test475(LogProxy* pLog)
{
    //XMVector2GreaterR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x > v2x&& v1y > v2y) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x <= v2x && v1y <= v2y) crcheck |= XM_CRMASK_CR6FALSE;

        uint32_t cr = XMVector2GreaterR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w,
                cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test476(LogProxy* pLog)
{
    //XMVector2GreaterOrEqualR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x >= v2x && v1y >= v2y) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x < v2x && v1y < v2y) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector2GreaterOrEqualR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w,
                cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test477(LogProxy* pLog)
{
    //XMVector2InBounds
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
        const FloatPairs_t* pEntry = &TableX[i & 7];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 8) & 7];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 64) /* &7 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() & 7];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        BOOL check = ((v1x <= v2x && v1x >= -v2x) && (v1y <= v2y && v1y >= -v2y)) ? TRUE : FALSE;
        BOOL r = XMVector2InBounds(v1, v2);
        if (r != check) {
            printe("%s: %f %f %f %f, %f %f %f %f: %d ... %d\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test479(LogProxy* pLog)
{
    //XMVector3EqualR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x == v2x && v1y == v2y && v1z == v2z) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x != v2x && v1y != v2y && v1z != v2z) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector3EqualR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test480(LogProxy* pLog)
{
    //XMVector3EqualIntR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (8 * 8 * 8); i++) {
        static const IntPairs_t TableX[8] = {
            {2,3},{2,1},{2,-3},{2,-1},{-2,3},{-2,1},{-2,-3},{-2,-1}
        };
        static const IntPairs_t TableY[8] = {
            {3,4},{3,2},{3,-4},{3,-2},{-3,4},{-3,2},{-3,-4},{-3,-2}
        };
        static const IntPairs_t TableZ[8] = {
            {4,5},{4,3},{4,-5},{4,-3},{-4,5},{-4,3},{-4,-5},{-4,-3}
        };
        static const IntPairs_t TableW[8] = {
            {5,6},{5,4},{5,-6},{5,-4},{-5,6},{-5,4},{-5,-6},{-5,-4}
        };
        const IntPairs_t* pEntry = &TableX[i & 7];
        int v1x = pEntry->x;
        int v2x = pEntry->y;
        pEntry = &TableY[(i / 8) & 7];
        int v1y = pEntry->x;
        int v2y = pEntry->y;
        pEntry = &TableZ[(i / 64) /* &7 */];
        int v1z = pEntry->x;
        int v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() & 7];
        int v1w = pEntry->x;
        int v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSetInt(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSetInt(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x == v2x && v1y == v2y && v1z == v2z) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x != v2x && v1y != v2y && v1z != v2z) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector3EqualIntR(v1, v2);
        if ((cr & XM_CRMASK_CR6BOUNDS) != crcheck) {
            printe("%s: %d %d %d %d, %d %d %d %d = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6BOUNDS, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test481(LogProxy* pLog)
{
    //XMVector3GreaterR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x > v2x&& v1y > v2y&& v1z > v2z) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x <= v2x && v1y <= v2y && v1z <= v2z) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector3GreaterR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test482(LogProxy* pLog)
{
    //XMVector3GreaterOrEqualR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x >= v2x && v1y >= v2y && v1z >= v2z) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x < v2x && v1y < v2y && v1z < v2z) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector3GreaterOrEqualR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test483(LogProxy* pLog)
{
    //XMVector3InBounds
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
        const FloatPairs_t* pEntry = &TableX[i & 7];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 8) & 7];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 64) /* &7 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() & 7];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        BOOL check = ((v1x <= v2x && v1x >= -v2x) && (v1y <= v2y && v1y >= -v2y) && (v1z <= v2z && v1z >= -v2z)) ? TRUE : FALSE;
        BOOL r = XMVector3InBounds(v1, v2);
        if (r != check) {
            printe("%s: %f %f %f %f, %f %f %f %f: %d ... %d\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test485(LogProxy* pLog)
{
    //XMVector4EqualR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x == v2x && v1y == v2y && v1z == v2z && v1w == v2w) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x != v2x && v1y != v2y && v1z != v2z && v1w != v2w) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector4EqualR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test486(LogProxy* pLog)
{
    //XMVector4EqualIntR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (8 * 8 * 8); i++) {
        static const IntPairs_t TableX[8] = {
            {2,3},{2,1},{2,-3},{2,-1},{-2,3},{-2,1},{-2,-3},{-2,-1}
        };
        static const IntPairs_t TableY[8] = {
            {3,4},{3,2},{3,-4},{3,-2},{-3,4},{-3,2},{-3,-4},{-3,-2}
        };
        static const IntPairs_t TableZ[8] = {
            {4,5},{4,3},{4,-5},{4,-3},{-4,5},{-4,3},{-4,-5},{-4,-3}
        };
        static const IntPairs_t TableW[8] = {
            {5,6},{5,4},{5,-6},{5,-4},{-5,6},{-5,4},{-5,-6},{-5,-4}
        };
        const IntPairs_t* pEntry = &TableX[i & 7];
        int v1x = pEntry->x;
        int v2x = pEntry->y;
        pEntry = &TableY[(i / 8) & 7];
        int v1y = pEntry->x;
        int v2y = pEntry->y;
        pEntry = &TableZ[(i / 64) /* &7 */];
        int v1z = pEntry->x;
        int v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() & 7];
        int v1w = pEntry->x;
        int v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSetInt(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSetInt(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x == v2x && v1y == v2y && v1z == v2z && v1w == v2w) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x != v2x && v1y != v2y && v1z != v2z && v1w != v2w) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector4EqualIntR(v1, v2);
        if ((cr & XM_CRMASK_CR6BOUNDS) != crcheck) {
            printe("%s: %d %d %d %d, %d %d %d %d = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w,
                cr & XM_CRMASK_CR6BOUNDS, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test487(LogProxy* pLog)
{
    //XMVector4GreaterR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x > v2x&& v1y > v2y&& v1z > v2z&& v1w > v2w) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x <= v2x && v1y <= v2y && v1z <= v2z && v1w <= v2w) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector4GreaterR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test488(LogProxy* pLog)
{
    //XMVector4GreaterOrEqualR
    HRESULT ret = S_OK;
    int i;
    for (i = 0; i < (6 * 6 * 6); i++) {
        static const FloatPairs_t TableX[6] = {
            {2,3},{2,2},{2,1},{-2,-3},{-2,-2},{-2,-1}
        };
        static const FloatPairs_t TableY[6] = {
            {3,4},{3,3},{3,2},{-3,-4},{-3,-3},{-3,-2}
        };
        static const FloatPairs_t TableZ[6] = {
            {4,5},{4,4},{4,3},{-4,-5},{-4,-4},{-4,-3}
        };
        static const FloatPairs_t TableW[6] = {
            {5,6},{5,5},{5,4},{-5,-6},{-5,-5},{-5,-4}
        };
        const FloatPairs_t* pEntry = &TableX[i % 6];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 6) % 6];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 36) /* %6 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() % 6];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        uint32_t crcheck = 0;
        if (v1x >= v2x && v1y >= v2y && v1z >= v2z && v1w >= v2w) crcheck |= XM_CRMASK_CR6TRUE;
        if (v1x < v2x && v1y < v2y && v1z < v2z && v1w < v2w) crcheck |= XM_CRMASK_CR6FALSE;
        uint32_t cr = XMVector4GreaterOrEqualR(v1, v2);
        if ((cr & XM_CRMASK_CR6) != crcheck) {
            printe("%s: %f %f %f %f, %f %f %f %f = %x ... %x\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w,
                cr & XM_CRMASK_CR6, crcheck);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test489(LogProxy* pLog)
{
    //XMVector4InBounds
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
        const FloatPairs_t* pEntry = &TableX[i & 7];
        float v1x = pEntry->x;
        float v2x = pEntry->y;
        pEntry = &TableY[(i / 8) & 7];
        float v1y = pEntry->x;
        float v2y = pEntry->y;
        pEntry = &TableZ[(i / 64) /* &7 */];
        float v1z = pEntry->x;
        float v2z = pEntry->y;
        pEntry = &TableW[XM_RAND() & 7];
        float v1w = pEntry->x;
        float v2w = pEntry->y;
        XMVECTOR v1 = XMVectorSet(v1x, v1y, v1z, v1w);
        XMVECTOR v2 = XMVectorSet(v2x, v2y, v2z, v2w);
        BOOL check = ((v1x <= v2x && v1x >= -v2x) && (v1y <= v2y && v1y >= -v2y) && (v1z <= v2z && v1z >= -v2z) && (v1w <= v2w && v1w >= -v2w)) ? TRUE : FALSE;
        BOOL r = XMVector4InBounds(v1, v2);
        if (r != check) {
            printe("%s: %f %f %f %f, %f %f %f %f: %d ... %d\n", TestName,
                v1x, v1y, v1z, v1w,
                v2x, v2y, v2z, v2w, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test491(LogProxy* pLog)
{
    //XMVector3Rotate
    XMVECTOR v1, v2, k1, r, check;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        v1 = GetRandomVector16();
        v2 = GetRandomVector16();
        v1 = XMVectorSetW(v1, c_Q_NAN);
        v2 = XMVectorSetW(v2, c_Q_NAN);
        k1 = v1;
        k1 = XMVectorSetW(k1, 0);
        check = XMQuaternionMultiply(XMQuaternionMultiply(XMQuaternionConjugate(v2), k1), v2);
        r = XMVector3Rotate(v1, v2);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
HRESULT Test492(LogProxy* pLog)
{
    //XMVector3InverseRotate
    XMVECTOR v1, v2, k1, r, check;
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        v1 = GetRandomVector16();
        v2 = GetRandomVector16();
        v1 = XMVectorSetW(v1, c_Q_NAN);
        v2 = XMVectorSetW(v2, c_Q_NAN);
        k1 = v1;
        k1 = XMVectorSetW(k1, 0);
        check = XMQuaternionMultiply(XMQuaternionMultiply(v2, k1), XMQuaternionConjugate(v2));
        r = XMVector3InverseRotate(v1, v2);
        COMPARISON c = CompareXMVECTOR(r, check, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
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
    return ret;
}
