//-------------------------------------------------------------------------------------
// xmcolor.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

HRESULT Test049(LogProxy* pLog)
{
    //XMColorAdjustContrast 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 check = {};
    XMVECTORF32 v = {};
    float f;
    for (int k = 0; k < 15; k++) {
        f = GetRandomFloat(3.0f) - 1;
        for (int i = 0; i < 4; i++) {
            v.v = XMVectorSetByIndex(v, GetRandomFloat(3.0f) - 1, i);
            check.v = XMVectorSetByIndex(check, ((XMVectorGetByIndex(v, i) - .5f) * f) + .5f, i);
        }
        check.v = XMVectorSetW(check, XMVectorGetW(v));

        XMVECTOR r = XMColorAdjustContrast(v, f);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN100EPSILON) {
            printe("%s: %f %f %f %f, %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), f,
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
HRESULT Test050(LogProxy* pLog)
{
    //XMColorAdjustSaturation 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 check = {};
    XMVECTORF32 v = {};
    float f;
    for (int k = 0; k < 15; k++) {
        f = GetRandomFloat(3.0f) - 1;
        int i;
        for (i = 0; i < 4; i++) {
            v.v = XMVectorSetByIndex(v, GetRandomFloat(3.0f) - 1, i);
        }
        float lum = 0.2125f * XMVectorGetX(v) + 0.7154f * XMVectorGetY(v) + 0.0721f * XMVectorGetZ(v);
        for (i = 0; i < 3; i++) {
            check.v = XMVectorSetByIndex(check, ((XMVectorGetByIndex(v, i) - lum) * f) + lum, i);
        }
        check.v = XMVectorSetW(check, XMVectorGetW(v));

        XMVECTOR r = XMColorAdjustSaturation(v, f);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN100EPSILON) {
            printe("%s: %f %f %f %f, %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), f,
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
HRESULT Test051(LogProxy* pLog)
{
    //XMColorEqual 
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
        r = XMColorEqual(v1, v2);
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
        r = XMColorEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test052(LogProxy* pLog)
{
    //XMColorGreater 
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
        r = XMColorGreater(v1, v2);
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
        r = XMColorGreater(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test053(LogProxy* pLog)
{
    //XMColorGreaterOrEqual 
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
        r = XMColorGreaterOrEqual(v1, v2);
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
        r = XMColorGreaterOrEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test054(LogProxy* pLog)
{
    //XMColorIsInfinite 
    HRESULT ret = S_OK;
    static const float f[] = { _INF, -_INF, _Q_NAN, 1,-1 };
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
                    res = XMColorIsInfinite(col);
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
HRESULT Test055(LogProxy* pLog)
{
    //XMColorIsNaN 
    HRESULT ret = S_OK;
    static const float f[] = { _Q_NAN, _NAN, _INF, 1,-1,605,1.0f - (1.0f / (65536.0f * 32768.0f)) };
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
                    res = XMColorIsNaN(col);
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

HRESULT Test056(LogProxy* pLog)
{
    //XMColorLess 
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
        r = XMColorLess(v1, v2);
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
        r = XMColorLess(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test057(LogProxy* pLog)
{
    //XMColorLessOrEqual 
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
        r = XMColorLessOrEqual(v1, v2);
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
        r = XMColorLessOrEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test058(LogProxy* pLog)
{
    //XMColorModulate 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 check = {};
    XMVECTORF32 v1 = {}, v2 = {};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            v1.v = XMVectorSetByIndex(v1, (float)XM_RAND() / 100.f, j);
            v2.v = XMVectorSetByIndex(v2, (float)XM_RAND() / 100.f, j);
            check.v = XMVectorSetByIndex(check, XMVectorGetByIndex(v1, j) * XMVectorGetByIndex(v2, j), j);
        }
        XMVECTOR r = XMColorModulate(v1, v2);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN2EPSILON) {
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
HRESULT Test059(LogProxy* pLog)
{
    //XMColorNegative 
    const XMVECTORF32 v[] = {
        {{1,.5,0,.004f}}, {{0,.5,1,.004f}},
        {{.3125,.75,100,.8f}},{{.6875,.25,-99,.8f}},
        {{.9f,.1f,.25,.12f}},{{.1f,.9f,.75,.12f}}
    };
    HRESULT ret = S_OK;
    XMVECTOR r;
    for (int i = 0; i < sizeof(v) / sizeof(v[0]); i += 2) {
        COMPARISON c = CompareXMVECTOR(r = XMColorNegative(v[i]), v[i + 1], 4);
        if (c > WITHIN2EPSILON) {
            printe("%s: %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v[i]), XMVectorGetY(v[i]), XMVectorGetZ(v[i]), XMVectorGetW(v[i]),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(v[i + 1]), XMVectorGetY(v[i + 1]), XMVectorGetZ(v[i + 1]), XMVectorGetW(v[i + 1]), c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test060(LogProxy* pLog)
{
    //XMColorNotEqual 
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
        r = XMColorNotEqual(v1, v2);
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
        r = XMColorNotEqual(v1, v2);
        if (r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n", TestName, j, i, r, check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

namespace
{
    void rgb2hsl(float r, float g, float b, float& h, float& s, float& l)
    {
        float rgb_min = std::min(r, std::min(g, b));
        float rgb_max = std::max(r, std::max(g, b));

        l = (rgb_min + rgb_max) / 2.0f;

        float d = rgb_max - rgb_min;

        if (fabs(d) < TESTEPSILON)
        {
            h = s = 0;
        }
        else
        {
            s = (l < 0.5f) ? (d / (rgb_min + rgb_max)) : (d / (2.f - rgb_min - rgb_max));

            if (r == rgb_max)
                h = (g - b) / d;
            else if (g == rgb_max)
                h = 2.0f + (b - r) / d;
            else
                h = 4.0f + (r - g) / d;
            h /= 6.f;
            if (h < 0)
                h += 1.f;
        }
    }
}

HRESULT Test603(LogProxy* pLog)
{
    // XMColorRGBToHSL/XMColorHSLToRGB
    HRESULT ret = S_OK;

    XMVECTOR colors[11];
    colors[0] = Colors::Violet;
    colors[1] = Colors::Plum;
    colors[2] = Colors::Silver;
    colors[3] = Colors::Black;
    colors[4] = Colors::White;
    colors[5] = Colors::Red;
    colors[6] = Colors::Green;
    colors[7] = Colors::Blue;
    colors[8] = Colors::Magenta;
    colors[9] = Colors::Cyan;
    colors[10] = Colors::Yellow;

    for (uint32_t i = 0; i < sizeof(colors) / sizeof(XMVECTOR); ++i)
    {
        XMVECTOR color = colors[i];

        XMVECTOR hsl = XMColorRGBToHSL(color);

        float h, s, l;
        rgb2hsl(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), h, s, l);
        XMVECTOR chk = XMVectorSet(h, s, l, 1.0f);

        COMPARISON c = CompareXMVECTOR(hsl, chk, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed RGB to HSL for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(hsl), XMVectorGetY(hsl), XMVectorGetZ(hsl), XMVectorGetW(hsl),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgb = XMColorHSLToRGB(hsl);

        c = CompareXMVECTOR(rgb, color, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed HSL to RGB %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f  (%d)\n", TestName, i,
                XMVectorGetX(hsl), XMVectorGetY(hsl), XMVectorGetZ(hsl), XMVectorGetW(hsl),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgbo = XMVectorSetW(color, 0.5f);

        hsl = XMColorRGBToHSL(rgbo);

        chk = XMVectorSetW(chk, 0.5f);

        c = CompareXMVECTOR(hsl, chk, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed RGB to HSL A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                XMVectorGetX(hsl), XMVectorGetY(hsl), XMVectorGetZ(hsl), XMVectorGetW(hsl),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        rgb = XMColorHSLToRGB(hsl);

        c = CompareXMVECTOR(rgb, rgbo, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed HSL to RGB A 0.5 %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(hsl), XMVectorGetY(hsl), XMVectorGetZ(hsl), XMVectorGetW(hsl),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

namespace
{
    void rgb2hsv(float r, float g, float b, float& h, float& s, float& v)
    {
        float rgb_min = std::min(r, std::min(g, b));
        float rgb_max = std::max(r, std::max(g, b));

        v = rgb_max;
        if (fabs(v) < TESTEPSILON)
        {
            h = s = 0;
        }
        else
        {
            r /= v;
            g /= v;
            b /= v;

            rgb_min = std::min(r, std::min(g, b));
            rgb_max = std::max(r, std::max(g, b));

            s = rgb_max - rgb_min;

            if (fabs(s) < TESTEPSILON)
            {
                h = 0;
            }
            else
            {
                r = (r - rgb_min) / (rgb_max - rgb_min);
                g = (g - rgb_min) / (rgb_max - rgb_min);
                b = (b - rgb_min) / (rgb_max - rgb_min);

                rgb_min = std::min(r, std::min(g, b));
                rgb_max = std::max(r, std::max(g, b));

                if (fabs(rgb_max - r) < TESTEPSILON)
                {
                    h = 60.f * (g - b);
                    if (h < 0)
                        h += 360.0f;
                }
                else if (fabs(rgb_max - g) < TESTEPSILON)
                {
                    h = 120.f + 60.f * (b - r);
                }
                else
                {
                    h = 240.f + 60.f * (r - g);
                }

                h /= 360.f;
            }
        }
    }
}

HRESULT Test604(LogProxy* pLog)
{
    // XMColorRGBToHSV/XMColorHSVToRGB
    HRESULT ret = S_OK;

    XMVECTOR colors[11];
    colors[0] = Colors::DodgerBlue;
    colors[1] = Colors::Indigo;
    colors[2] = Colors::Silver;
    colors[3] = Colors::Black;
    colors[4] = Colors::White;
    colors[5] = Colors::Red;
    colors[6] = Colors::Green;
    colors[7] = Colors::Blue;
    colors[8] = Colors::Magenta;
    colors[9] = Colors::Cyan;
    colors[10] = Colors::Yellow;

    for (uint32_t i = 0; i < sizeof(colors) / sizeof(XMVECTOR); ++i)
    {
        XMVECTOR color = colors[i];

        XMVECTOR hsv = XMColorRGBToHSV(color);

        float h, s, v;
        rgb2hsv(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), h, s, v);
        XMVECTOR chk = XMVectorSet(h, s, v, 1.0f);

        COMPARISON c = CompareXMVECTOR(hsv, chk, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed RGB to HSV for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(hsv), XMVectorGetY(hsv), XMVectorGetZ(hsv), XMVectorGetW(hsv),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgb = XMColorHSVToRGB(hsv);

        c = CompareXMVECTOR(rgb, color, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed HSV to RGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(hsv), XMVectorGetY(hsv), XMVectorGetZ(hsv), XMVectorGetW(hsv),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgbo = XMVectorSetW(color, 0.5f);

        hsv = XMColorRGBToHSV(rgbo);

        chk = XMVectorSetW(chk, 0.5f);

        c = CompareXMVECTOR(hsv, chk, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed RGB to HSV A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                XMVectorGetX(hsv), XMVectorGetY(hsv), XMVectorGetZ(hsv), XMVectorGetW(hsv),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        rgb = XMColorHSVToRGB(hsv);

        c = CompareXMVECTOR(rgb, rgbo, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed HSV to RGB A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(hsv), XMVectorGetY(hsv), XMVectorGetZ(hsv), XMVectorGetW(hsv),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

namespace
{
    void rgb2yuv(float r, float g, float b, float& y, float& u, float& v)
    {
        y = 0.299f * r + 0.587f * g + 0.114f * b;
        u = 0.492f * (b - y);
        v = 0.877f * (r - y);
    }
}

HRESULT Test605(LogProxy* pLog)
{
    // XMColorRGBToYUV/XMColorYUVToRGB
    HRESULT ret = S_OK;

    XMVECTOR colors[11];
    colors[0] = Colors::PaleGreen;
    colors[1] = Colors::RoyalBlue;
    colors[2] = Colors::Silver;
    colors[3] = Colors::Black;
    colors[4] = Colors::White;
    colors[5] = Colors::Red;
    colors[6] = Colors::Green;
    colors[7] = Colors::Blue;
    colors[8] = Colors::Magenta;
    colors[9] = Colors::Cyan;
    colors[10] = Colors::Yellow;

    for (uint32_t i = 0; i < sizeof(colors) / sizeof(XMVECTOR); ++i)
    {
        XMVECTOR color = colors[i];

        XMVECTOR yuv = XMColorRGBToYUV(color);

        float y, u, v;
        rgb2yuv(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), y, u, v);
        XMVECTOR chk = XMVectorSet(y, u, v, 1.0f);

        COMPARISON c = CompareXMVECTOR(yuv, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed RGB to YUV for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgb = XMColorYUVToRGB(yuv);

        c = CompareXMVECTOR(rgb, color, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed YUV to RGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgbo = XMVectorSetW(color, 0.5f);

        yuv = XMColorRGBToYUV(rgbo);

        chk = XMVectorSetW(chk, 0.5f);

        c = CompareXMVECTOR(yuv, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed RGB to YUV A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        rgb = XMColorYUVToRGB(yuv);

        c = CompareXMVECTOR(rgb, rgbo, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed YUV to RGB A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

namespace
{
    void rgb2yuv709(float r, float g, float b, float& y, float& u, float& v)
    {
        y = 0.2126f * r + 0.7152f * g + 0.0722f * b;
        u = 0.470f * (b - y);
        v = 0.781f * (r - y);
    }
}

HRESULT Test606(LogProxy* pLog)
{
    // XMColorRGBToYUV_HD/XMColorYUVToRGB_HD
    HRESULT ret = S_OK;

    XMVECTOR colors[11];
    colors[0] = Colors::PaleGreen;
    colors[1] = Colors::RoyalBlue;
    colors[2] = Colors::Silver;
    colors[3] = Colors::Black;
    colors[4] = Colors::White;
    colors[5] = Colors::Red;
    colors[6] = Colors::Green;
    colors[7] = Colors::Blue;
    colors[8] = Colors::Magenta;
    colors[9] = Colors::Cyan;
    colors[10] = Colors::Yellow;

    for (uint32_t i = 0; i < sizeof(colors) / sizeof(XMVECTOR); ++i)
    {
        XMVECTOR color = colors[i];

        XMVECTOR yuv = XMColorRGBToYUV_HD(color);

        float y, u, v;
        rgb2yuv709(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), y, u, v);
        XMVECTOR chk = XMVectorSet(y, u, v, 1.0f);

        COMPARISON c = CompareXMVECTOR(yuv, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed RGB to YUV709 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgb = XMColorYUVToRGB_HD(yuv);

        c = CompareXMVECTOR(rgb, color, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed YUV709 to RGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgbo = XMVectorSetW(color, 0.5f);

        yuv = XMColorRGBToYUV_HD(rgbo);

        chk = XMVectorSetW(chk, 0.5f);

        c = CompareXMVECTOR(yuv, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed RGB to YUV709 A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        rgb = XMColorYUVToRGB_HD(yuv);

        c = CompareXMVECTOR(rgb, rgbo, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed YUV709 to RGB A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

namespace
{
    void rgb2xyz(float r, float g, float b, float& x, float& y, float& z)
    {
        x = (0.4887180f * r + 0.3106803f * g + 0.2006017f * b) / 0.17697f;
        y = (0.1762044f * r + 0.8129847f * g + 0.0108109f * b) / 0.17697f;
        z = (0.0102048f * g + 0.9897952f * b) / 0.17697f;
    }
}

HRESULT Test607(LogProxy* pLog)
{
    // XMColorRGBToXYZ/XMColorXYZToRGB
    HRESULT ret = S_OK;

    XMVECTOR colors[11];
    colors[0] = Colors::DarkSalmon;
    colors[1] = Colors::DodgerBlue;
    colors[2] = Colors::Silver;
    colors[3] = Colors::Black;
    colors[4] = Colors::White;
    colors[5] = Colors::Red;
    colors[6] = Colors::Green;
    colors[7] = Colors::Blue;
    colors[8] = Colors::Magenta;
    colors[9] = Colors::Cyan;
    colors[10] = Colors::Yellow;

    for (uint32_t i = 0; i < sizeof(colors) / sizeof(XMVECTOR); ++i)
    {
        XMVECTOR color = colors[i];

        XMVECTOR xyz = XMColorRGBToXYZ(color);

        float x, y, z;
        rgb2xyz(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), x, y, z);
        XMVECTOR chk = XMVectorSet(x, y, z, 1.0f);

        COMPARISON c = CompareXMVECTOR(xyz, chk, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed RGB to XYZ for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(xyz), XMVectorGetY(xyz), XMVectorGetZ(xyz), XMVectorGetW(xyz),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgb = XMColorXYZToRGB(xyz);

        c = CompareXMVECTOR(rgb, color, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed XYZ to RGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(xyz), XMVectorGetY(xyz), XMVectorGetZ(xyz), XMVectorGetW(xyz),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgbo = XMVectorSetW(color, 0.5f);

        xyz = XMColorRGBToXYZ(rgbo);

        chk = XMVectorSetW(chk, 0.5f);

        c = CompareXMVECTOR(xyz, chk, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed RGB to XYZ A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                XMVectorGetX(xyz), XMVectorGetY(xyz), XMVectorGetZ(xyz), XMVectorGetW(xyz),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        rgb = XMColorXYZToRGB(xyz);

        c = CompareXMVECTOR(rgb, rgbo, 4);
        if (c > WITHIN10EPSILON)
        {
            printe("%s: failed XYZ to RGB A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(xyz), XMVectorGetY(xyz), XMVectorGetZ(xyz), XMVectorGetW(xyz),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

namespace
{
    void srgb2xyz(float r, float g, float b, float& x, float& y, float& z)
    {
        float rl = (r <= 0.04045f) ? (r / 12.92f) : powf((r + 0.055f) / (1.055f), 2.4f);
        float gl = (g <= 0.04045f) ? (g / 12.92f) : powf((g + 0.055f) / (1.055f), 2.4f);
        float bl = (b <= 0.04045f) ? (b / 12.92f) : powf((b + 0.055f) / (1.055f), 2.4f);

        x = 0.4124f * rl + 0.3576f * gl + 0.1805f * bl;
        y = 0.2126f * rl + 0.7152f * gl + 0.0722f * bl;
        z = 0.0193f * rl + 0.1192f * gl + 0.9505f * bl;
    }
}

HRESULT Test608(LogProxy* pLog)
{
    // XMColorXYZToSRGB/XMColorSRGBToXYZ
    HRESULT ret = S_OK;

    XMVECTOR colors[11];
    colors[0] = Colors::DarkSalmon;
    colors[1] = Colors::DodgerBlue;
    colors[2] = Colors::Silver;
    colors[3] = Colors::Black;
    colors[4] = Colors::White;
    colors[5] = Colors::Red;
    colors[6] = Colors::Green;
    colors[7] = Colors::Blue;
    colors[8] = Colors::Magenta;
    colors[9] = Colors::Cyan;
    colors[10] = Colors::Yellow;

    for (uint32_t i = 0; i < sizeof(colors) / sizeof(XMVECTOR); ++i)
    {
        XMVECTOR color = colors[i];

        XMVECTOR xyz = XMColorSRGBToXYZ(color);

        float x, y, z;
        srgb2xyz(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), x, y, z);
        XMVECTOR chk = XMVectorSet(x, y, z, 1.0f);

        COMPARISON c = CompareXMVECTOR(xyz, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed sRGB to XYZ for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(xyz), XMVectorGetY(xyz), XMVectorGetZ(xyz), XMVectorGetW(xyz),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgb = XMColorXYZToSRGB(xyz);

        c = CompareXMVECTOR(rgb, color, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed XYZ to sRGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(xyz), XMVectorGetY(xyz), XMVectorGetZ(xyz), XMVectorGetW(xyz),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgbo = XMVectorSetW(color, 0.5f);

        xyz = XMColorSRGBToXYZ(rgbo);

        chk = XMVectorSetW(chk, 0.5f);

        c = CompareXMVECTOR(xyz, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed sRGB to XYZ A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                XMVectorGetX(xyz), XMVectorGetY(xyz), XMVectorGetZ(xyz), XMVectorGetW(xyz),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        rgb = XMColorXYZToSRGB(xyz);

        c = CompareXMVECTOR(rgb, rgbo, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed XYZ to sRGB A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(xyz), XMVectorGetY(xyz), XMVectorGetZ(xyz), XMVectorGetW(xyz),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

namespace
{
    float EncodeSRGB(float f)
    {
        if (f <= 0.0f)
            return 0.0f;
        else if (f >= 1.0f)
            return 1.0f;
        else if (f < 0.0031308f)
            return 12.92f * f;
        else
            return 1.055f * powf(f, 1.0f / 2.4f) - 0.055f;
    }

    float DecodeSRGB(float f)
    {
        if (f <= 0.0f)
            return 0.0f;
        else if (f >= 1.0f)
            return 1.0f;
        else if (f <= 0.04045f)
            return f / 12.92f;
        else
            return powf((f + 0.055f) / 1.055f, 2.4f);
    }
}

HRESULT Test610(LogProxy* pLog)
{
    // XMColorRGBToSRGB/XMColorSRGBToRGB
    HRESULT ret = S_OK;

    XMVECTOR colors[11];
    colors[0] = Colors::DarkSalmon;
    colors[1] = Colors::DodgerBlue;
    colors[2] = Colors::Silver;
    colors[3] = Colors::Black;
    colors[4] = Colors::White;
    colors[5] = Colors::Red;
    colors[6] = Colors::Green;
    colors[7] = Colors::Blue;
    colors[8] = Colors::Magenta;
    colors[9] = Colors::Cyan;
    colors[10] = Colors::Yellow;

    for (uint32_t i = 0; i < sizeof(colors) / sizeof(XMVECTOR); ++i)
    {
        XMVECTOR color = colors[i];

        XMVECTOR srgb = XMColorRGBToSRGB(color);

        float r = EncodeSRGB(XMVectorGetX(color));
        float g = EncodeSRGB(XMVectorGetY(color));
        float b = EncodeSRGB(XMVectorGetZ(color));
        XMVECTOR chk = XMVectorSet(r, g, b, 1.0f);

        COMPARISON c = CompareXMVECTOR(srgb, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed RGB to sRGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(srgb), XMVectorGetY(srgb), XMVectorGetZ(srgb), XMVectorGetW(srgb),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        r = DecodeSRGB(XMVectorGetX(srgb));
        g = DecodeSRGB(XMVectorGetY(srgb));
        b = DecodeSRGB(XMVectorGetZ(srgb));
        XMVECTOR chk2 = XMVectorSet(r, g, b, 1.f);

        c = CompareXMVECTOR(color, chk2, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (2): failed RGB to sRGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(srgb), XMVectorGetY(srgb), XMVectorGetZ(srgb), XMVectorGetW(srgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(chk2), XMVectorGetY(chk2), XMVectorGetZ(chk2), XMVectorGetW(chk2),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgb = XMColorSRGBToRGB(srgb);

        c = CompareXMVECTOR(rgb, color, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed sRGB to RGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(srgb), XMVectorGetY(srgb), XMVectorGetZ(srgb), XMVectorGetW(srgb),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgbo = XMVectorSetW(color, 0.5f);

        srgb = XMColorRGBToSRGB(rgbo);

        chk = XMVectorSetW(chk, 0.5f);

        c = CompareXMVECTOR(srgb, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed RGB to sRGB A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                XMVectorGetX(srgb), XMVectorGetY(srgb), XMVectorGetZ(srgb), XMVectorGetW(srgb),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        rgb = XMColorSRGBToRGB(srgb);

        c = CompareXMVECTOR(rgb, rgbo, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed sRGB to RGB A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(srgb), XMVectorGetY(srgb), XMVectorGetZ(srgb), XMVectorGetW(srgb),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

namespace
{
    void rgb2yuv2020(float r, float g, float b, float& y, float& u, float& v)
    {
        y = 0.2627f * r + 0.6780f * g + 0.0593f * b;
        u = 0.4625f * (b - y);
        v = 0.8341f * (r - y);
    }
}

HRESULT Test612(LogProxy* pLog)
{
    // XMColorRGBToYUV_UHD/XMColorYUVToRGB_UHD
    HRESULT ret = S_OK;

    XMVECTOR colors[11];
    colors[0] = Colors::PaleGreen;
    colors[1] = Colors::RoyalBlue;
    colors[2] = Colors::Silver;
    colors[3] = Colors::Black;
    colors[4] = Colors::White;
    colors[5] = Colors::Red;
    colors[6] = Colors::Green;
    colors[7] = Colors::Blue;
    colors[8] = Colors::Magenta;
    colors[9] = Colors::Cyan;
    colors[10] = Colors::Yellow;

    for (uint32_t i = 0; i < sizeof(colors) / sizeof(XMVECTOR); ++i)
    {
        XMVECTOR color = colors[i];

        XMVECTOR yuv = XMColorRGBToYUV_UHD(color);

        float y, u, v;
        rgb2yuv2020(XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), y, u, v);
        XMVECTOR chk = XMVectorSet(y, u, v, 1.0f);

        COMPARISON c = CompareXMVECTOR(yuv, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed RGB to YUV2020 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgb = XMColorYUVToRGB_UHD(yuv);

        c = CompareXMVECTOR(rgb, color, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed YUV2020 to RGB for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(color), XMVectorGetY(color), XMVectorGetZ(color), XMVectorGetW(color),
                c);
            ret = MATH_FAIL;
        }

        XMVECTOR rgbo = XMVectorSetW(color, 0.5f);

        yuv = XMColorRGBToYUV_UHD(rgbo);

        chk = XMVectorSetW(chk, 0.5f);

        c = CompareXMVECTOR(yuv, chk, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed RGB to YUV2020 A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(chk), XMVectorGetY(chk), XMVectorGetZ(chk), XMVectorGetW(chk),
                c);
            ret = MATH_FAIL;
        }

        rgb = XMColorYUVToRGB_UHD(yuv);

        c = CompareXMVECTOR(rgb, rgbo, 4);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: failed YUV2020 to RGB A 0.5 for %d - %f %f %f %f: %f %f %f %f ... %f %f %f %f (%d)\n", TestName, i,
                XMVectorGetX(yuv), XMVectorGetY(yuv), XMVectorGetZ(yuv), XMVectorGetW(yuv),
                XMVectorGetX(rgb), XMVectorGetY(rgb), XMVectorGetZ(rgb), XMVectorGetW(rgb),
                XMVectorGetX(rgbo), XMVectorGetY(rgbo), XMVectorGetZ(rgbo), XMVectorGetW(rgbo),
                c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
