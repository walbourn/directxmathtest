//-------------------------------------------------------------------------------------
// xmmat.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

static_assert(sizeof(XMMATRIX) == 64, "Unexpected structure size");

static_assert(std::is_nothrow_copy_assignable<XMMATRIX>::value, "Copy Assign.");
static_assert(std::is_nothrow_copy_constructible<XMMATRIX>::value, "Copy Ctor.");
static_assert(std::is_nothrow_move_constructible<XMMATRIX>::value, "Move Ctor.");
static_assert(std::is_nothrow_move_assignable<XMMATRIX>::value, "Move Assign.");

HRESULT Test084(LogProxy* pLog)
{
    //XMMatrixAffineTransformation
    XMVECTOR s;
    XMVECTOR o;
    XMVECTOR q;
    XMVECTOR t;
    XMMATRIX r;
    XMMATRIX check;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMMATRIX temp;

    s = XMVectorSet(2, 8, 11, c_Q_NAN);
    o = XMVectorSet(5, 7, -3, c_Q_NAN);
    q = XMQuaternionRotationAxis(XMVectorSet(1, 2, 3, 0), 2.3f);
    t = XMVectorSet(-4, 6, 1, c_Q_NAN);
    for (int i = 0; i < 8; i++) {

        r = XMMatrixAffineTransformation(s, o, q, t);

        check = XMMatrixIdentity();
        check.r[0] = XMVectorSetX(check.r[0], XMVectorGetX(check.r[0]) * XMVectorGetX(s));
        check.r[1] = XMVectorSetY(check.r[1], XMVectorGetY(check.r[1]) * XMVectorGetY(s));
        check.r[2] = XMVectorSetZ(check.r[2], XMVectorGetZ(check.r[2]) * XMVectorGetZ(s));
        temp = XMMatrixRotationQuaternion(q);
        check.r[3] -= XMVectorSet(XMVectorGetX(o), XMVectorGetY(o), XMVectorGetZ(o), 0);
        check *= temp;
        check.r[3] += XMVectorSet(XMVectorGetX(o), XMVectorGetY(o), XMVectorGetZ(o), 0);
        check.r[3] += XMVectorSet(XMVectorGetX(t), XMVectorGetY(t), XMVectorGetZ(t), 0);

        c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %d: (%d)\n", TestName, i, c);
            printmatrixe(r); printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d %d\n", TestName, i, c);
        }
    }
    return ret;
}
HRESULT Test085(LogProxy* pLog)
{
    //XMMatrixAffineTransformation2D
    XMVECTOR s;
    XMVECTOR o;
    float q;
    XMVECTOR t;
    XMMATRIX r;
    XMMATRIX check;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMMATRIX temp;

    s = XMVectorSet(2, 8, c_Q_NAN, c_Q_NAN);
    o = XMVectorSet(5, 7, c_Q_NAN, c_Q_NAN);
    q = .35f;
    t = XMVectorSet(-4, 6, c_Q_NAN, c_Q_NAN);
    for (int i = 0; i < 8; i++) {

        r = XMMatrixAffineTransformation2D(s, o, q, t);

        check = XMMatrixIdentity();
        check.r[0] = XMVectorSetX(check.r[0], XMVectorGetX(check.r[0]) * XMVectorGetX(s));
        check.r[1] = XMVectorSetY(check.r[1], XMVectorGetY(check.r[1]) * XMVectorGetY(s));
        temp = XMMatrixRotationZ(q);
        check.r[3] -= XMVectorSet(XMVectorGetX(o), XMVectorGetY(o), 0, 0);
        check *= temp;
        check.r[3] += XMVectorSet(XMVectorGetX(o), XMVectorGetY(o), 0, 0);
        check.r[3] += XMVectorSet(XMVectorGetX(t), XMVectorGetY(t), 0, 0);

        c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %d: (%d)\n", TestName, i, c);
            printmatrixe(r); printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d %d\n", TestName, i, c);
        }
    }
    return ret;
}

float getdet(int size, const float* a)
{
    float t[16];
    int it = 0;
    float res = 0;
    float sign = 1;
    int i, x, y;
    if (size == 1) return *a;
    for (i = 0; i < size; i++) {
        for (x = 0; x < size; x++) {
            if (x == i) x++;
            if (x == size) break;
            for (y = 1; y < size; y++) {
                if (it > 15) break;
                t[it++] = a[y * size + x];
            }
        }
        //float gd = getdet(size-1,t);
        //printi ("size: %d, i: %d, gd: %f\n",size,i,gd);
        res += sign * a[i] * getdet(size - 1, t);
        sign = -sign;
        it = 0;
    }
    return res;
}

float getinv(int size, const float* a, float* b)
{
    int i, j, x, y;
    float buf[16];
    float det = getdet(size, a);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            int ibuf = 0;
            for (x = 0; x < size; x++) {
                if (x == j) continue;
                for (y = 0; y < size; y++) {
                    if (y == i) continue;
                    buf[ibuf++] = a[y * size + x];
                }
            }
            float sign = ((i & 1) == (j & 1)) ? 1.f : -1.f;
            b[j * size + i] = sign * getdet(size - 1, buf) / det;

        }
    }
    return det;
}

HRESULT Test086(LogProxy* pLog)
{
    //XMMatrixDeterminant
    int i, j, k;
    HRESULT ret = S_OK;
    for (k = 0; k < 10; k++) {
        float tmp[4][4];
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 100.f;
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        COMPARISON c;
        XMVECTOR check = XMVectorReplicate(getdet(4, &tmp[0][0]));
        XMVECTOR r = XMMatrixDeterminant(m);
        c = CompareXMVECTOR(r, check, 4);
        if (c > WITHIN4096) {
            printe("%s: \n", TestName);
            printmatrixe(m);
            printe("%f %f %f %f ... %f %f %f %f (%d)\n",
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r),
                XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: (%d)\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test087(LogProxy* pLog)
{
    //XMMatrixIdentity
    XMMATRIX m;
    m = XMMatrixIdentity();
    XMVECTORF32 check[4] = { {{{1,0,0,0}}},{{{0,1,0,0}}},{{{0,0,1,0}}},{{{0,0,0,1}}} };
    if (CompareXMVECTOR(m.r[0], check[0], 4) ||
        CompareXMVECTOR(m.r[1], check[1], 4) ||
        CompareXMVECTOR(m.r[2], check[2], 4) ||
        CompareXMVECTOR(m.r[3], check[3], 4))
    {
        printe("%s: ", TestName);
        printmatrixe(m);
        return MATH_FAIL;
    }
    return S_OK;
}


HRESULT Test088(LogProxy* pLog)
{
    //XMMatrixInverse
    HRESULT ret = S_OK;
    XMVECTOR det;

    XMMATRIX id = XMMatrixIdentity();
    XMMATRIX r = XMMatrixInverse(&det, id);
    if (!XMVector4NearEqual(r.r[0], id.r[0], g_XMEpsilon)
        || !XMVector4NearEqual(r.r[1], id.r[1], g_XMEpsilon)
        || !XMVector4NearEqual(r.r[2], id.r[2], g_XMEpsilon)
        || !XMVector4NearEqual(r.r[3], id.r[3], g_XMEpsilon))
    {
        printe("%s: of Identity != Identity\n", TestName);
        printmatrixe(r);
        ret = MATH_FAIL;
    }
    if (fabs(XMVectorGetX(det) - 1.f) > TESTEPSILON)
    {
        printe("%s: Identity det = %.8g, ... 1\n", TestName, XMVectorGetX(det));
    }

    float tmp[4][4];
    tmp[0][0] = 1.0208333333333333333f;
    tmp[0][1] = -1.25;
    tmp[0][2] = 1.1666666666666666666f;
    tmp[0][3] = -3;
    tmp[1][0] = -.4791666666666666666f;
    tmp[1][1] = .75;
    tmp[1][2] = -.8333333333333333333f;
    tmp[1][3] = 2;
    tmp[2][0] = .1875;
    tmp[2][1] = -.25;
    tmp[2][2] = .5;
    tmp[2][3] = -1;
    tmp[3][0] = -.2083333333333333333f;
    tmp[3][1] = .5;
    tmp[3][2] = -.6666666666666666666f;
    tmp[3][3] = 1;
    float detchk = getdet(4, &(tmp[0][0]));
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
    XMMATRIX m(&tmp[0][0]);

    //static const float blah[] = {1,-.01f,23,10};
    //float detchk = getdet(2, blah);
    XMVECTOR detchkv = XMVectorReplicate(detchk);
    float tmpc[4][4];
    tmpc[0][0] = 4;
    tmpc[0][1] = 8;
    tmpc[0][2] = 4;
    tmpc[0][3] = 0;
    tmpc[1][0] = 1;
    tmpc[1][1] = 4;
    tmpc[1][2] = 7;
    tmpc[1][3] = 2;
    tmpc[2][0] = 1;
    tmpc[2][1] = 5;
    tmpc[2][2] = 4;
    tmpc[2][3] = -3;
    tmpc[3][0] = 1;
    tmpc[3][1] = 3;
    tmpc[3][2] = 0;
    tmpc[3][3] = -2;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
    XMMATRIX ch(&tmpc[0][0]);

    r = XMMatrixInverse(&det, m);
    COMPARISON c = CompareXMVECTOR(det, detchkv, 4);
    if (c > WITHIN4096) {
        printe("%s: det %f ... %f (%d)\n", TestName,
            XMVectorGetX(det), XMVectorGetX(detchkv), c);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: det (%d)\n", TestName, c);
    }
    c = CompareXMMATRIX(r, ch);
    if (c > WITHIN4096) {
        printe("%s: (%d)\n", TestName, c);
        printmatrixe(m); printe("=\n "); printmatrixe(r); printe("...\n "); printmatrixe(ch);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: (%d)\n", TestName, c);
    }

    int i, j, k;
    for (k = 0; k < 8; k++) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                tmp[i][j] = ((float)XM_RAND()) / 1000.f;
            }
        }
        detchk = getinv(4, &tmp[0][0], &tmpc[0][0]);
        XMMATRIX m2(&tmp[0][0]);
        XMMATRIX chk2(&tmpc[0][0]);
        r = XMMatrixInverse(&det, m2);
        detchkv = XMVectorReplicate(detchk);
        c = CompareXMVECTOR(det, detchkv, 4);
        if (c > WITHIN4096) {
            printe("%s: det %f ... %f (%d)\n", TestName,
                XMVectorGetX(det), XMVectorGetX(detchkv), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: det (%d)\n", TestName, c);
        }
        c = CompareXMMATRIX(r, chk2);
        if (c > WITHIN4096) {
            printe("%s: (%d) %i\n", TestName, c, k);
            printmatrixe(m2); printe("=\n "); printmatrixe(r); printe("...\n "); printmatrixe(chk2);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: (%d)\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test089(LogProxy* pLog)
{
    //XMMatrixIsIdentity
    int x, y, x1, y1;
    BOOL r = FALSE;
    HRESULT ret = S_OK;

    for (x1 = 0; x1 < 4; x1++) {
        for (y1 = 0; y1 < 4; y1++) {
            float tmp[4][4];
            for (x = 0; x < 4; x++) {
                for (y = 0; y < 4; y++) {
                    tmp[x][y] = (x == y);
                }
            }
            tmp[x1][y1] = (x1 != y1);
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
            XMMATRIX m(&tmp[0][0]);
            r = XMMatrixIsIdentity(m);
            if (r) {
                printe("%s: failed at x=%d,y=%d. r=%d\n", TestName, x1, y1, r);
                ret = MATH_FAIL;
            }
        }
    }
    printi("%s: %d\n", TestName, 10 * (FALSE != r));

    r = XMMatrixIsIdentity(XMMatrixIdentity());
    printi("%s: %d\n", TestName, 10 * (TRUE != r));
    if (r != TRUE) {
        printe("%s: of XMMatrixIdentity: Should be TRUE, is is %d\n", TestName, r);
        ret = MATH_FAIL;
    }
    return ret;
}
HRESULT Test090(LogProxy* pLog)
{
    //XMMatrixIsInfinite
    HRESULT ret = S_OK;
    BOOL r;
    int i, j, x, y;
    static const float f[] = { 1, c_INF };
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            float tmp[4][4];
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    tmp[i][j] = f[0];
                }
            }
            tmp[x][y] = f[1];

#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
            XMMATRIX m(&tmp[0][0]);
            r = XMMatrixIsInfinite(m);
            if (!r) {
                printe("%s: %d %d = %d ... TRUE\n",
                    TestName, x, y, r);
                ret = MATH_FAIL;
            }
        }
    }

    float tmp[4][4];
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            tmp[x][y] = (((float)XM_RAND()) / 100.f) - 16.f;
        }
    }
    tmp[XM_RAND() & 3][XM_RAND() & 3] = c_Q_NAN;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
    XMMATRIX m2(&tmp[0][0]);
    r = XMMatrixIsInfinite(m2);
    if (r) {
        printe("%s: %d ... FALSE\n", TestName, r);
        printmatrixe(m2);
        ret = MATH_FAIL;
    }
    return ret;
}

ISNAN_TEST_BEGIN
HRESULT Test091(LogProxy* pLog)
{
    //XMMatrixIsNaN
    HRESULT ret = S_OK;
    BOOL r;
    int i, j, x, y;
    static const float f[] = { 1, c_NAN, c_Q_NAN };
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            float tmp[4][4];
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    tmp[i][j] = f[0];
                }
            }
            tmp[x][y] = ((x & 1) == (y & 1)) ? f[1] : f[2];

#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
            XMMATRIX m(&tmp[0][0]);
            r = XMMatrixIsNaN(m);
            if (!r) {
                printe("%s: %d %d = %d ... TRUE\n",
                    TestName, x, y, r);
                ret = MATH_FAIL;
            }
        }
    }
    float tmp[4][4];
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            tmp[x][y] = (((float)XM_RAND()) / 100.f) - 16.f;
        }
    }
    tmp[XM_RAND() & 3][XM_RAND() & 3] = c_INF;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
    XMMATRIX m2(&tmp[0][0]);
    r = XMMatrixIsNaN(m2);
    if (r) {
        printe("%s: %d ... FALSE\n", TestName, r);
        printmatrixe(m2);
        ret = MATH_FAIL;
    }
    return ret;
}
ISNAN_TEST_END

HRESULT Test092(LogProxy* pLog)
{
    //XMMatrixLookAtLH
    static const XMVECTORF32 x[] = { {{{1,0,0,c_Q_NAN}}}, {{{-0.227921f,-0.341882f,-0.911685f,c_Q_NAN}}} };
    static const XMVECTORF32 y[] = { {{{0,1,0,c_Q_NAN}}}, {{{-0.082918f, 0.939743f,-0.331674f,c_Q_NAN}}} };
    static const XMVECTORF32 z[] = { {{{0,0,1,c_Q_NAN}}}, {{{ 0.970142f, 0.000000f,-0.242536f,c_Q_NAN}}} };
    static const XMVECTORF32 eyeloc[] = { {{{0,0,0,c_Q_NAN}}},{{{5,6,7,c_Q_NAN}}} };
    static const XMVECTORF32 widentity = { { {0.0f,0.0f,0.0f,1.0f } } };
    static const float scalefocus[] = { 1, 15.3f };
    static const float scaleup[] = { 1, .75f };
    HRESULT ret = S_OK;

    int k;
    for (k = 0; k < countof(x); k++) {
        XMMATRIX check;
        check.r[0] = x[k];
        check.r[1] = y[k];
        check.r[2] = z[k];
        check.r[3] = widentity;
        check = XMMatrixTranspose(check);

        for (int e = 0; e < countof(eyeloc); e++) {
            XMVECTOR eye = eyeloc[e];
            XMVECTOR negeye = XMVectorNegate(eye);
            XMVECTOR dx = XMVector3Dot(x[k], negeye);
            XMVECTOR dy = XMVector3Dot(y[k], negeye);
            XMVECTOR dz = XMVector3Dot(z[k], negeye);
            check.r[3] = XMVectorSet(XMVectorGetX(dx), XMVectorGetX(dy), XMVectorGetX(dz), 1.f);
            for (int f = 0; f < countof(scalefocus); f++) {
                for (int u = 0; u < countof(scaleup); u++) {
                    XMVECTOR up = XMVectorMultiply(y[k], XMVectorSplatX(XMLoadFloat(&scaleup[u])));
                    XMVECTOR focus = XMVectorMultiply(z[k], XMVectorSplatX(XMLoadFloat(&scalefocus[f])));
                    focus = XMVectorAdd(focus, eye);
                    XMMATRIX m = XMMatrixLookAtLH(eye, focus, up);
                    COMPARISON c = CompareXMMATRIX(m, check);
                    if (c > WITHINBIGEPSILON) {
                        printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f (%d):\n",
                            TestName,
                            XMVectorGetX(eye), XMVectorGetY(eye), XMVectorGetZ(eye), XMVectorGetW(eye),
                            XMVectorGetX(focus), XMVectorGetY(focus), XMVectorGetZ(focus), XMVectorGetW(focus),
                            XMVectorGetX(up), XMVectorGetY(up), XMVectorGetZ(up), XMVectorGetW(up), c);
                        printmatrixe(m); printe("...\n");
                        printmatrixe(check);
                        ret = MATH_FAIL;
                    }
                    else {
                        printi("%s: %d\n", TestName, c);
                    }
                }
            }
        }
    }
    return ret;
}

HRESULT Test093(LogProxy* pLog)
{
    //XMMatrixLookAtRH
    static const XMVECTORF32 x[] = { {{1,0,0,c_Q_NAN}}, {{-0.227921f,-0.341882f,-0.911685f,c_Q_NAN}} };
    static const XMVECTORF32 y[] = { {{0,1,0,c_Q_NAN}}, {{-0.082918f, 0.939743f,-0.331674f,c_Q_NAN}} };
    static const XMVECTORF32 z[] = { {{0,0,1,c_Q_NAN}}, {{ 0.970142f, 0.000000f,-0.242536f,c_Q_NAN}} };

    XMMATRIX m;
    XMMATRIX check;
    XMVECTOR eye, focus, up;
    static const XMVECTORF32 eyeloc[] = { {{0,0,0,c_Q_NAN}},{{5,6,7,c_Q_NAN}} };
    static const float scalefocus[] = { 1, 15.3f };
    static const float scaleup[] = { 1, .75f };
    HRESULT ret = S_OK;

    for (int k = 0; k < countof(x); k++) {
        check = XMMatrixIdentity();
        check.r[0] = x[k]; check.r[1] = y[k]; check.r[2] = z[k];
        check = XMMatrixTranspose(check);

        for (int e = 0; e < countof(eyeloc); e++) {
            XMVECTOR d = XMVectorSet(XMVectorGetX(XMVector3Dot(x[k], XMVectorNegate(eyeloc[e]))),
                XMVectorGetY(XMVector3Dot(y[k], XMVectorNegate(eyeloc[e]))),
                XMVectorGetZ(XMVector3Dot(z[k], XMVectorNegate(eyeloc[e]))), 1);
            check.r[3] = d;
            for (int f = 0; f < countof(scalefocus); f++) {
                for (int u = 0; u < countof(scaleup); u++) {
                    up = XMVectorScale(y[k], scaleup[u]);
                    eye = eyeloc[e];
                    focus = XMVectorSubtract(eyeloc[e], XMVectorScale(z[k], scalefocus[f]));
                    m = XMMatrixLookAtRH(eye, focus, up);
                    COMPARISON c = CompareXMMATRIX(m, check);
                    if (c > WITHINBIGEPSILON) {
                        printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f (%d):\n",
                            TestName, XMVectorGetX(eye), XMVectorGetY(eye), XMVectorGetZ(eye), XMVectorGetW(eye),
                            XMVectorGetX(focus), XMVectorGetY(focus), XMVectorGetZ(focus), XMVectorGetW(focus),
                            XMVectorGetX(up), XMVectorGetY(up), XMVectorGetZ(up), XMVectorGetW(up), c);
                        printmatrixe(m); printe("...\n");
                        printmatrixe(check);
                        ret = MATH_FAIL;
                    }
                    else {
                        printi("%s: %d\n", TestName, c);
                    }
                }
            }
        }
    }

    return ret;
}
HRESULT Test094(LogProxy* pLog)
{
    //XMMatrixLookToLH
    static const XMVECTORF32 x[] = { {{1,0,0,c_Q_NAN}}, {{-0.227921f,-0.341882f,-0.911685f,c_Q_NAN}} };
    static const XMVECTORF32 y[] = { {{0,1,0,c_Q_NAN}}, {{-0.082918f, 0.939743f,-0.331674f,c_Q_NAN}} };
    static const XMVECTORF32 z[] = { {{0,0,1,c_Q_NAN}}, {{ 0.970142f, 0.000000f,-0.242536f,c_Q_NAN}} };

    XMMATRIX m;
    XMMATRIX check;
    XMVECTOR eye, to, up;
    static const XMVECTORF32 eyeloc[] = { {{0,0,0,c_Q_NAN}},{{5,6,7,c_Q_NAN}} };
    static const float scaleto[] = { 1, 15.3f };
    static const float scaleup[] = { 1, .75f };
    HRESULT ret = S_OK;

    for (int k = 0; k < countof(x); k++) {
        check = XMMatrixIdentity();
        check.r[0] = x[k]; check.r[1] = y[k]; check.r[2] = z[k];
        check = XMMatrixTranspose(check);

        for (int e = 0; e < countof(eyeloc); e++) {
            XMVECTOR d = XMVectorSet(XMVectorGetX(XMVector3Dot(x[k], XMVectorNegate(eyeloc[e]))),
                XMVectorGetY(XMVector3Dot(y[k], XMVectorNegate(eyeloc[e]))),
                XMVectorGetZ(XMVector3Dot(z[k], XMVectorNegate(eyeloc[e]))), 1);
            check.r[3] = d;
            for (int f = 0; f < countof(scaleto); f++) {
                for (int u = 0; u < countof(scaleup); u++) {
                    up = XMVectorScale(y[k], scaleup[u]);
                    eye = eyeloc[e];
                    to = XMVectorScale(z[k], scaleto[f]);
                    m = XMMatrixLookToLH(eye, to, up);
                    COMPARISON c = CompareXMMATRIX(m, check);
                    if (c > WITHINBIGEPSILON) {
                        printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f (%d):\n",
                            TestName, XMVectorGetX(eye), XMVectorGetY(eye), XMVectorGetZ(eye), XMVectorGetW(eye),
                            XMVectorGetX(to), XMVectorGetY(to), XMVectorGetZ(to), XMVectorGetW(to),
                            XMVectorGetX(up), XMVectorGetY(up), XMVectorGetZ(up), XMVectorGetW(up), c);
                        printmatrixe(m); printe("...\n");
                        printmatrixe(check);
                        ret = MATH_FAIL;
                    }
                    else {
                        printi("%s: %d\n", TestName, c);
                    }
                }
            }
        }
    }

    return ret;
}
HRESULT Test095(LogProxy* pLog)
{
    //XMMatrixLookToRH
    static const XMVECTORF32 x[] = { {{1,0,0,c_Q_NAN}}, {{-0.227921f,-0.341882f,-0.911685f,c_Q_NAN}} };
    static const XMVECTORF32 y[] = { {{0,1,0,c_Q_NAN}}, {{-0.082918f, 0.939743f,-0.331674f,c_Q_NAN}} };
    static const XMVECTORF32 z[] = { {{0,0,1,c_Q_NAN}}, {{ 0.970142f, 0.000000f,-0.242536f,c_Q_NAN}} };

    XMMATRIX m;
    XMMATRIX check;
    XMVECTOR eye, to, up;
    static const XMVECTORF32 eyeloc[] = { {{{0,0,0,c_Q_NAN}}},{{{5,6,7,c_Q_NAN}}} };
    static const float scaleto[] = { 1, 15.3f };
    static const float scaleup[] = { 1, .75f };
    HRESULT ret = S_OK;

    for (int k = 0; k < countof(x); k++) {
        check = XMMatrixIdentity();
        check.r[0] = x[k]; check.r[1] = y[k]; check.r[2] = z[k];
        check = XMMatrixTranspose(check);

        for (int e = 0; e < countof(eyeloc); e++) {
            XMVECTOR d = XMVectorSet(XMVectorGetX(XMVector3Dot(x[k], XMVectorNegate(eyeloc[e]))),
                XMVectorGetY(XMVector3Dot(y[k], XMVectorNegate(eyeloc[e]))),
                XMVectorGetZ(XMVector3Dot(z[k], XMVectorNegate(eyeloc[e]))), 1);
            check.r[3] = d;
            for (int f = 0; f < countof(scaleto); f++) {
                for (int u = 0; u < countof(scaleup); u++) {
                    up = XMVectorScale(y[k], scaleup[u]);
                    eye = eyeloc[e];
                    to = XMVectorScale(z[k], -scaleto[f]);
                    m = XMMatrixLookToRH(eye, to, up);
                    COMPARISON c = CompareXMMATRIX(m, check);
                    if (c > WITHINBIGEPSILON) {
                        printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f (%d):\n",
                            TestName, XMVectorGetX(eye), XMVectorGetY(eye), XMVectorGetZ(eye), XMVectorGetW(eye),
                            XMVectorGetX(to), XMVectorGetY(to), XMVectorGetZ(to), XMVectorGetW(to),
                            XMVectorGetX(up), XMVectorGetY(up), XMVectorGetZ(up), XMVectorGetW(up), c);
                        printmatrixe(m); printe("...\n");
                        printmatrixe(check);
                        ret = MATH_FAIL;
                    }
                    else {
                        printi("%s: %d\n", TestName, c);
                    }
                }
            }
        }
    }

    return ret;
}
HRESULT Test096(LogProxy* pLog)
{
    //XMMatrixMultiply
    //XMMATRIX operator *=, *
    XMMATRIX check;
    HRESULT ret = S_OK;
    COMPARISON c;

    {
        XMMATRIX id = XMMatrixIdentity();
        XMMATRIX m = XMMatrixMultiply(id, id);
        XMMATRIX m2 = id * id;
        if (!XMMatrixIsIdentity(m))
        {
            printe("%s: identity*identity != identity\n", TestName);
            printmatrixe(m);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: 0\n", TestName);
        }
        if (!XMMatrixIsIdentity(m2))
        {
            printe("%s (*): identity*identity != identity\n", TestName);
            printmatrixe(m2);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: 0\n", TestName);
        }
    }

    {
        float tmp[4][4];
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp[x][y] = GetRandomFloat(100.0f);
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
        XMMATRIX id = XMMatrixIdentity();
        XMMATRIX m2 = XMMatrixMultiply(id, m);
        XMMATRIX m3 = XMMatrixMultiply(m, id);
        XMMATRIX m4 = id * m;
        XMMATRIX m5 = m * id;
        if (CompareXMMATRIX(m, m2) || CompareXMMATRIX(m, m3))
        {
            printe("%s: m*identity != m\n", TestName);
            printmatrixe(m); printe(": "); printmatrixe(m2); printe("... "); printmatrixe(m3);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: 0\n", TestName);
        }
        if (CompareXMMATRIX(m, m4) || CompareXMMATRIX(m, m5))
        {
            printe("%s (*): m*identity != m\n", TestName);
            printmatrixe(m); printe(": "); printmatrixe(m4); printe("... "); printmatrixe(m5);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: 0\n", TestName);
        }
    }

    for (int k = 0; k < 3; k++)
    {
        float tmp2[4][4], tmp3[4][4];
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp2[x][y] = GetRandomFloat(100.0f);
                tmp3[x][y] = GetRandomFloat(100.0f);
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m2(&tmp2[0][0]);
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m3(&tmp3[0][0]);
        XMMATRIX m = XMMatrixMultiply(m2, m3);
        XMMATRIX m4 = m2 * m3;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                check.r[x] = XMVectorSetByIndex(check.r[x], 0, y);
                for (int i = 0; i < 4; i++) {
                    check.r[x] = XMVectorSetByIndex(check.r[x], XMVectorGetByIndex(check.r[x], y) + XMVectorGetByIndex(m2.r[x], i) * XMVectorGetByIndex(m3.r[i], y), y);
                }
            }
        }
        c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: ", TestName);
            printmatrixe(m2);
            printe("times: ");
            printmatrixe(m3);
            printe("= ");
            printmatrixe(m);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
        c = CompareXMMATRIX(m4, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: ", TestName);
            printmatrixe(m2);
            printe("times: ");
            printmatrixe(m3);
            printe("= ");
            printmatrixe(m4);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test097(LogProxy* pLog)
{
    //XMMatrixMultiplyTranspose
    HRESULT ret = S_OK;
    COMPARISON c;

    {
        XMMATRIX m = XMMatrixMultiplyTranspose(XMMatrixIdentity(), XMMatrixIdentity());
        if (!XMMatrixIsIdentity(m)) {
            printe("%s: identity*identity != identity\n", TestName);
            printmatrixe(m);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: 0\n", TestName);
        }
    }

    {
        float tmp[4][4], tmpc[4][4];
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp[x][y] = GetRandomFloat(100.0f);
                tmpc[y][x] = tmp[x][y];
            }
        }

#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m2 = XMMatrixMultiplyTranspose(XMMatrixIdentity(), m);
        XMMATRIX m3 = XMMatrixMultiplyTranspose(m, XMMatrixIdentity());
        if (CompareXMMATRIX(check, m2) || CompareXMMATRIX(check, m3))
        {
            printe("%s: m*identity != m\n", TestName);
            printmatrixe(m); printe(": "); printmatrixe(m2); printe(", "); printmatrixe(m3); printe("... "); printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: 0\n", TestName);
        }
    }

    for (int k = 0; k < 3; k++)
    {
        float tmp2[4][4], tmp3[4][4];
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp2[x][y] = GetRandomFloat(100.0f);
                tmp3[x][y] = GetRandomFloat(100.0f);
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m2(&tmp2[0][0]);
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m3(&tmp3[0][0]);
        XMMATRIX m = XMMatrixMultiplyTranspose(m2, m3);
        XMMATRIX check;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                check.r[y] = XMVectorSetByIndex(check.r[y], 0, x);
                for (int i = 0; i < 4; i++) {
                    check.r[y] = XMVectorSetByIndex(check.r[y], XMVectorGetByIndex(check.r[y], x) + tmp2[x][i] * tmp3[i][y], x);
                }
            }
        }
        c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: ", TestName);
            printmatrixe(m2);
            printe("times: ");
            printmatrixe(m3);
            printe("= ");
            printmatrixe(m);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test098(LogProxy* pLog)
{
    //XMMatrixOrthographicLH
    static const float w[] = { 1.f,10,3,.2f };
    static const float h[] = { 1.f,10,2,.5f };
    static const float nz[] = { 0.f,1.f,-1.f,5,.01f };
    static const float fz[] = { 1.f,-1.f,10,1000,.02f };

    HRESULT ret = S_OK;

    for (int k = 0; k < countof(w); k++) {
        for (int e = 0; e < countof(h); e++) {
            for (int f = 0; f < countof(nz); f++) {
                float tmpc[4][4] = {};
                tmpc[0][0] = 2 / w[k];
                tmpc[1][1] = 2 / h[e];
                tmpc[2][2] = 1 / (fz[f] - nz[f]);
                tmpc[3][2] = nz[f] / (nz[f] - fz[f]);
                tmpc[3][3] = 1.f;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixOrthographicLH(w[k], h[e], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f  @(%d)@:\n",
                        TestName, w[k], h[e], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test099(LogProxy* pLog)
{
    //XMMatrixOrthographicOffCenterLH
    static const float nz[] = { 0.f,1.f,-1.f,5,.01f };
    static const float fz[] = { 1.f,-1.f,10,1000,.02f };
    static const float l[] = { 5, -1 };
    static const float r[] = { 8, 3 };
    static const float t[] = { 10, .5 };
    static const float b[] = { 8, 3 };

    HRESULT ret = S_OK;

    for (int f = 0; f < countof(nz); f++) {
        for (int tb = 0; tb < countof(t); tb++) {
            for (int lr = 0; lr < countof(l); lr++) {
                float tmpc[4][4] = {};
                tmpc[0][0] = 2 / (r[lr] - l[lr]);
                tmpc[1][1] = 2 / (t[tb] - b[tb]);
                tmpc[2][2] = 1 / (fz[f] - nz[f]);
                tmpc[3][0] = (l[lr] + r[lr]) / (l[lr] - r[lr]);
                tmpc[3][1] = (t[tb] + b[tb]) / (b[tb] - t[tb]);
                tmpc[3][2] = nz[f] / (nz[f] - fz[f]);
                tmpc[3][3] = 1.f;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixOrthographicOffCenterLH(l[lr], r[lr], b[tb], t[tb], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f %f %f  @(%d)@:\n",
                        TestName, l[lr], r[lr], t[tb], b[tb], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test100(LogProxy* pLog)
{
    //XMMatrixOrthographicOffCenterRH
    static const float nz[] = { 0.f,1.f,-1.f,5,.01f };
    static const float fz[] = { 1.f,-1.f,10,1000,.02f };
    static const float l[] = { 5, -1 };
    static const float r[] = { 8, 3 };
    static const float t[] = { 10, .5 };
    static const float b[] = { 8, 3 };

    HRESULT ret = S_OK;

    for (int f = 0; f < countof(nz); f++) {
        for (int tb = 0; tb < countof(t); tb++) {
            for (int lr = 0; lr < countof(l); lr++) {
                float tmpc[4][4] = {};
                tmpc[0][0] = 2 / (r[lr] - l[lr]);
                tmpc[1][1] = 2 / (t[tb] - b[tb]);
                tmpc[2][2] = 1 / (nz[f] - fz[f]);
                tmpc[3][0] = (l[lr] + r[lr]) / (l[lr] - r[lr]);
                tmpc[3][1] = (t[tb] + b[tb]) / (b[tb] - t[tb]);
                tmpc[3][2] = nz[f] / (nz[f] - fz[f]);
                tmpc[3][3] = 1.f;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixOrthographicOffCenterRH(l[lr], r[lr], b[tb], t[tb], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f %f %f  @(%d)@:\n",
                        TestName, l[lr], r[lr], t[tb], b[tb], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}

HRESULT Test101(LogProxy* pLog)
{
    //XMMatrixOrthographicRH
    static const float w[] = { 1.f,10,3,.2f };
    static const float h[] = { 1.f,10,2,.5f };
    static const float nz[] = { 0.f,1.f,-1.f,5,.01f };
    static const float fz[] = { 1.f,-1.f,10,1000,.02f };

    HRESULT ret = S_OK;

    for (int k = 0; k < countof(w); k++) {
        for (int e = 0; e < countof(h); e++) {
            for (int f = 0; f < countof(nz); f++) {
                float tmpc[4][4] = {};
                tmpc[0][0] = 2 / w[k];
                tmpc[1][1] = 2 / h[e];
                tmpc[2][2] = 1 / (nz[f] - fz[f]);
                tmpc[3][2] = nz[f] / (nz[f] - fz[f]);
                tmpc[3][3] = 1.f;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixOrthographicRH(w[k], h[e], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f  @(%d)@:\n",
                        TestName, w[k], h[e], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test102(LogProxy* pLog)
{
    //XMMatrixPerspectiveFovLH
    static const float fa[] = { Pi / 2.f, Pi / 4.f, 3 * Pi / 4.f };
    static const float a[] = { 10,2,.5f };
    static const float nz[] = { 100,1,5,.01f };
    static const float fz[] = { 2,10,1000,.02f };

    HRESULT ret = S_OK;

    for (int k = 0; k < countof(fa); k++) {
        for (int e = 0; e < countof(a); e++) {
            for (int f = 0; f < countof(nz); f++) {
                float tmpc[4][4] = {};
                float co = cosf(fa[k] / 2.f); float s = sinf(fa[k] / 2.f);
                tmpc[0][0] = co / (s * a[e]);
                tmpc[1][1] = co / s;
                tmpc[2][2] = fz[f] / (fz[f] - nz[f]);
                tmpc[3][2] = nz[f] * fz[f] / (nz[f] - fz[f]);
                tmpc[2][3] = 1;
                tmpc[3][3] = 0;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixPerspectiveFovLH(fa[k], a[e], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f  @(%d)@:\n",
                        TestName, fa[k], a[e], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test103(LogProxy* pLog)
{
    //XMMatrixPerspectiveFovRH
    static const float fa[] = { Pi / 2.f, Pi / 4.f, 3 * Pi / 4.f };
    static const float a[] = { 10,2,.5f };
    static const float nz[] = { 100,1,5,.01f };
    static const float fz[] = { 2,10,1000,.02f };

    HRESULT ret = S_OK;

    for (int k = 0; k < countof(fa); k++) {
        for (int e = 0; e < countof(a); e++) {
            for (int f = 0; f < countof(nz); f++) {
                float tmpc[4][4] = {};
                float co = cosf(fa[k] / 2.f); float s = sinf(fa[k] / 2.f);
                tmpc[0][0] = co / (s * a[e]);
                tmpc[1][1] = co / s;
                tmpc[2][2] = fz[f] / (nz[f] - fz[f]);
                tmpc[3][2] = nz[f] * fz[f] / (nz[f] - fz[f]);
                tmpc[2][3] = -1;
                tmpc[3][3] = 0;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixPerspectiveFovRH(fa[k], a[e], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f  @(%d)@:\n",
                        TestName, fa[k], a[e], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test104(LogProxy* pLog)
{
    //XMMatrixPerspectiveLH
    static const float w[] = { 10,3,.2f };
    static const float h[] = { 10,2,.5f };
    static const float nz[] = { 100,1,5,.01f };
    static const float fz[] = { 2,10,1000,.02f };

    HRESULT ret = S_OK;

    for (int k = 0; k < countof(w); k++) {
        for (int e = 0; e < countof(h); e++) {
            for (int f = 0; f < countof(nz); f++) {
                float tmpc[4][4] = {};
                tmpc[0][0] = 2 * nz[f] / w[k];
                tmpc[1][1] = 2 * nz[f] / h[e];
                tmpc[2][2] = fz[f] / (fz[f] - nz[f]);
                tmpc[3][2] = nz[f] * fz[f] / (nz[f] - fz[f]);
                tmpc[2][3] = 1;
                tmpc[3][3] = 0;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixPerspectiveLH(w[k], h[e], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f  @(%d)@:\n",
                        TestName, w[k], h[e], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test105(LogProxy* pLog)
{
    //XMMatrixPerspectiveOffCenterLH
    static const float nz[] = { 100,5,.01f };
    static const float fz[] = { 2,1000,.02f };
    static const float l[] = { 5, -1 };
    static const float r[] = { 8, 3 };
    static const float t[] = { 10, .5 };
    static const float b[] = { 8, 3 };

    HRESULT ret = S_OK;

    for (int f = 0; f < countof(nz); f++) {
        for (int tb = 0; tb < countof(t); tb++) {
            for (int lr = 0; lr < countof(l); lr++) {
                float tmpc[4][4] = {};
                tmpc[0][0] = 2 * nz[f] / (r[lr] - l[lr]);
                tmpc[1][1] = 2 * nz[f] / (t[tb] - b[tb]);
                tmpc[2][0] = (l[lr] + r[lr]) / (l[lr] - r[lr]);
                tmpc[2][1] = (t[tb] + b[tb]) / (b[tb] - t[tb]);
                tmpc[2][2] = fz[f] / (fz[f] - nz[f]);
                tmpc[2][3] = 1;
                tmpc[3][2] = nz[f] * fz[f] / (nz[f] - fz[f]);
                tmpc[3][3] = 0;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixPerspectiveOffCenterLH(l[lr], r[lr], b[tb], t[tb], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f %f %f  @(%d)@:\n",
                        TestName, l[lr], r[lr], t[tb], b[tb], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test106(LogProxy* pLog)
{
    //XMMatrixPerspectiveOffCenterRH
    static const float nz[] = { 100,5,.01f };
    static const float fz[] = { 2,1000,.02f };
    static const float l[] = { 5, -1 };
    static const float r[] = { 8, 3 };
    static const float t[] = { 10, .5 };
    static const float b[] = { 8, 3 };

    HRESULT ret = S_OK;

    for (int f = 0; f < countof(nz); f++) {
        for (int tb = 0; tb < countof(t); tb++) {
            for (int lr = 0; lr < countof(l); lr++) {
                float tmpc[4][4] = {};
                tmpc[0][0] = 2 * nz[f] / (r[lr] - l[lr]);
                tmpc[1][1] = 2 * nz[f] / (t[tb] - b[tb]);
                tmpc[2][0] = (l[lr] + r[lr]) / (r[lr] - l[lr]);
                tmpc[2][1] = (t[tb] + b[tb]) / (t[tb] - b[tb]);
                tmpc[2][2] = fz[f] / (nz[f] - fz[f]);
                tmpc[2][3] = -1;
                tmpc[3][2] = nz[f] * fz[f] / (nz[f] - fz[f]);
                tmpc[3][3] = 0;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixPerspectiveOffCenterRH(l[lr], r[lr], b[tb], t[tb], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f %f %f  @(%d)@:\n",
                        TestName, l[lr], r[lr], t[tb], b[tb], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test107(LogProxy* pLog)
{
    //XMMatrixPerspectiveRH
    static const float w[] = { 10,3,.2f };
    static const float h[] = { 10,2,.5f };
    static const float nz[] = { 100,1,5,.01f };
    static const float fz[] = { 2,10,1000,.02f };

    HRESULT ret = S_OK;

    for (int k = 0; k < countof(w); k++) {
        for (int e = 0; e < countof(h); e++) {
            for (int f = 0; f < countof(nz); f++) {
                float tmpc[4][4] = {};
                tmpc[0][0] = 2 * nz[f] / w[k];
                tmpc[1][1] = 2 * nz[f] / h[e];
                tmpc[2][2] = fz[f] / (nz[f] - fz[f]);
                tmpc[3][2] = nz[f] * fz[f] / (nz[f] - fz[f]);
                tmpc[2][3] = -1;
                tmpc[3][3] = 0;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
                XMMATRIX check(&tmpc[0][0]);
                XMMATRIX m = XMMatrixPerspectiveRH(w[k], h[e], nz[f], fz[f]);
                COMPARISON c = CompareXMMATRIX(m, check);
                if (c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f  @(%d)@:\n",
                        TestName, w[k], h[e], nz[f], fz[f], c);
                    printmatrixe(m); printe("...\n");
                    printmatrixe(check);
                    ret = MATH_FAIL;
                }
                else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }
    return ret;
}
HRESULT Test108(LogProxy* pLog)
{
    //XMMatrixReflect
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        XMVECTOR v = GetRandomVector16();
        if (XMVectorGetX(v) == 0 && XMVectorGetY(v) == 0) v = XMVectorSetX(v, 2);
        float scale = XMVectorGetW(XMVector3Dot(v, v));
        scale = sqrtf(scale);

        float tmpc[4][4] = {};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmpc[i][j] = ((i == j) ? 1.f : 0.f) +
                    ((j < 3) ? (XMVectorGetByIndex(v, j) / scale) : 0) * -2.f * (XMVectorGetByIndex(v, i) / scale);
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixReflect(v);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  @(%d)@:\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

    }
    return ret;
}
HRESULT Test109(LogProxy* pLog)
{
    //XMMatrixRotationAxis
    HRESULT ret = S_OK;
    XMMATRIX m, check;
    XMVECTOR v;
    float angle;
    COMPARISON c;

    v = XMVectorSet(((float(XM_RAND()) / 4000.f) + .1f), 0, 0, 0);
    angle = 1.0f;
    check = XMMatrixRotationX(angle);
    m = XMMatrixRotationAxis(v, angle);
    c = CompareXMMATRIX(m, check);
    if (c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f @(%d)@\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle, c);
        printmatrixe(m); printe("...\n"); printmatrixe(check);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }
    v = XMVectorSet(0, ((float(XM_RAND()) / 4000.f) + .1f), 0, 0);
    angle = 2.0f;
    check = XMMatrixRotationY(angle);
    m = XMMatrixRotationAxis(v, angle);
    c = CompareXMMATRIX(m, check);
    if (c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f @(%d)@\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle, c);
        printmatrixe(m); printe("...\n"); printmatrixe(check);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }
    v = XMVectorSet(0, 0, ((float(XM_RAND()) / 4000.f) + .1f), 0);
    angle = 3.0f;
    check = XMMatrixRotationZ(angle);
    m = XMMatrixRotationAxis(v, angle);
    c = CompareXMMATRIX(m, check);
    if (c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f @(%d)@\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle, c);
        printmatrixe(m); printe("...\n"); printmatrixe(check);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }
    for (int k = 0; k < 10; k++) {
        v = XMVectorSet(((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f);
        angle = ((float)XM_RAND()) / 2000.f - 8.f;
        m = XMMatrixRotationAxis(v, angle);
        XMVECTOR v1 = v * ((((float)XM_RAND()) / 8000.f) + .1f);
        XMVECTOR r = XMVector4Transform(v1, m);
        c = CompareXMVECTOR(r, v1, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f =\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle);
            printmatrixe(m);
            printe("translating: %f %f %f %f = %f %f %f %f (%d)\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
        XMVECTOR v2 = XMVectorSet(((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f);
        r = XMVector4Transform(v2, m);
        float dot1, dot2;
        dot1 = XMVectorGetX(XMVector3Dot(v2, v1));
        dot2 = XMVectorGetX(XMVector3Dot(r, v1));
        c = Compare(dot1, dot2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f =\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle);
            printmatrixe(m);
            printe("translating: %f %f %f %f = %f %f %f %f. Dot1 = %f, Dot2 = %f. (%d)\n",
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), dot1, dot2, c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

        XMVECTOR v3 = v2 - ((XMVectorGetX(XMVector3Dot(v2, v1))) / (XMVectorGetX(XMVector3Dot(v1, v1)))) * v1;
        XMVECTOR v4 = r - ((XMVectorGetX(XMVector3Dot(r, v1))) / (XMVectorGetX(XMVector3Dot(v1, v1)))) * v1;
        float anglecheck = acosf(XMVectorGetX(XMVector3Dot(v3, v4)) / (sqrtf(XMVectorGetX(XMVector3Dot(v3, v3))) * sqrtf(XMVectorGetX(XMVector3Dot(v4, v4)))));
        c = Compare(fabsf(anglecheck), fabsf(XMScalarModAngle(angle)));
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), XMScalarModAngle(angle));
            printmatrixe(m);
            printe("rotate");
            dqe(v2); printe("= "); dqe(r);
            printe("map: "); dqe(v3); dqe(v4);
            printe("new angle %f (%d)\n", anglecheck, c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %f (%d)\n", TestName, anglecheck, c);
        }

    }
    return ret;
}
HRESULT Test110(LogProxy* pLog)
{
    //XMMatrixRotationNormal
    HRESULT ret = S_OK;
    XMMATRIX m, check;
    XMVECTOR v;
    float angle;
    COMPARISON c;

    v = XMVectorSet(1, 0, 0, c_Q_NAN);
    angle = 1.0f;
    check = XMMatrixRotationX(angle);
    m = XMMatrixRotationNormal(v, angle);
    c = CompareXMMATRIX(m, check);
    if (c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f @(%d)@\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle, c);
        printmatrixe(m); printe("...\n"); printmatrixe(check);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }
    v = XMVectorSet(0, 1, 0, c_Q_NAN);
    angle = 2.0f;
    check = XMMatrixRotationY(angle);
    m = XMMatrixRotationNormal(v, angle);
    c = CompareXMMATRIX(m, check);
    if (c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f @(%d)@\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle, c);
        printmatrixe(m); printe("...\n"); printmatrixe(check);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }
    v = XMVectorSet(0, 0, 1, c_Q_NAN);
    angle = 3.0f;
    check = XMMatrixRotationZ(angle);
    m = XMMatrixRotationNormal(v, angle);
    c = CompareXMMATRIX(m, check);
    if (c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f @(%d)@\n",
            TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle, c);
        printmatrixe(m); printe("...\n"); printmatrixe(check);
        ret = MATH_FAIL;
    }
    else {
        printi("%s: %d\n", TestName, c);
    }
    for (int k = 0; k < 10; k++) {
        v = XMVectorSet(((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f);
        float blah = 1.f / sqrtf(XMVectorGetX(v) * XMVectorGetX(v) + XMVectorGetY(v) * XMVectorGetY(v) + XMVectorGetZ(v) * XMVectorGetZ(v));
        v *= blah;
        angle = ((float)XM_RAND()) / 2000.f - 8.f;
        m = XMMatrixRotationNormal(v, angle);
        XMVECTOR v1 = v * ((((float)XM_RAND()) / 8000.f) + .1f);
        XMVECTOR r = XMVector4Transform(v1, m);
        c = CompareXMVECTOR(r, v1, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f =\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle);
            printmatrixe(m);
            printe("translating: %f %f %f %f = %f %f %f %f (%d)\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
        XMVECTOR v2 = XMVectorSet(((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f);
        r = XMVector4Transform(v2, m);
        float dot1, dot2;
        dot1 = XMVectorGetX(XMVector3Dot(v2, v1));
        dot2 = XMVectorGetX(XMVector3Dot(r, v1));
        c = Compare(dot1, dot2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f =\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), angle);
            printmatrixe(m);
            printe("translating: %f %f %f %f = %f %f %f %f. Dot1 = %f, Dot2 = %f. (%d)\n",
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), dot1, dot2, c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

        XMVECTOR v3 = v2 - ((XMVectorGetX(XMVector3Dot(v2, v1))) / (XMVectorGetX(XMVector3Dot(v1, v1)))) * v1;
        XMVECTOR v4 = r - ((XMVectorGetX(XMVector3Dot(r, v1))) / (XMVectorGetX(XMVector3Dot(v1, v1)))) * v1;
        float anglecheck = acosf(XMVectorGetX(XMVector3Dot(v3, v4)) / (sqrtf(XMVectorGetX(XMVector3Dot(v3, v3))) * sqrtf(XMVectorGetX(XMVector3Dot(v4, v4)))));
        c = Compare(fabsf(anglecheck), fabsf(XMScalarModAngle(angle)));
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), XMScalarModAngle(angle));
            printmatrixe(m);
            printe("rotate");
            dqe(v2); printe("= "); dqe(r);
            printe("map: "); dqe(v3); dqe(v4);
            printe("new angle %f (%d)\n", anglecheck, c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %f (%d)\n", TestName, anglecheck, c);
        }

    }
    return ret;
}
HRESULT Test111(LogProxy* pLog)
{
    //XMMatrixRotationQuaternion
    HRESULT ret = S_OK;
    XMVECTOR v;
    float angle;
    XMMATRIX m, check;
    COMPARISON c;
    XMVECTOR q;
    for (int k = 0; k < 10; k++) {
        v = XMVectorSet(((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f);
        float vx = XMVectorGetX(v); float vy = XMVectorGetY(v); float vz = XMVectorGetZ(v); float vw = XMVectorGetW(v);
        float blah = 1.f / sqrtf(vx * vx + vy * vy + vz * vz);
        v *= blah;
        vx = XMVectorGetX(v); vy = XMVectorGetY(v); vz = XMVectorGetZ(v); vw = XMVectorGetW(v);
        angle = ((float)XM_RAND()) / 2000.f - 8.f;
        float si = sinf(angle / 2.f);
        float co = cosf(angle / 2.f);
        q = XMVectorSet(vx * si, vy * si, vz * si, co);

        check = XMMatrixRotationNormal(v, angle);
        m = XMMatrixRotationQuaternion(q);
        c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  (%d)\n", TestName, XMVectorGetX(q), XMVectorGetY(q), XMVectorGetZ(q), XMVectorGetW(q), c);
            printmatrixe(m); printe("...\n"); printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
        XMVECTOR v1 = v * ((((float)XM_RAND()) / 8000.f) + .1f);
        XMVECTOR r = XMVector4Transform(v1, m);
        c = CompareXMVECTOR(r, v1, 4);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f =\n",
                TestName, vx, vy, vz, vw, angle);
            printmatrixe(m);
            printe("translating: %f %f %f %f = %f %f %f %f (%d)\n",
                XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), c);
        }
        else {
            printi("%s: %d\n", TestName, c);
        }
        XMVECTOR v2 = XMVectorSet(((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f, ((float)XM_RAND()) / 4000.f - 4.f);
        r = XMVector4Transform(v2, m);
        float dot1, dot2;
        dot1 = XMVectorGetX(XMVector3Dot(v2, v1));
        dot2 = XMVectorGetX(XMVector3Dot(r, v1));
        c = Compare(dot1, dot2);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f =\n",
                TestName, vx, vy, vz, vw, angle);
            printmatrixe(m);
            printe("translating: %f %f %f %f = %f %f %f %f. Dot1 = %f, Dot2 = %f. (%d)\n",
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2),
                XMVectorGetX(r), XMVectorGetY(r), XMVectorGetZ(r), XMVectorGetW(r), dot1, dot2, c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

        XMVECTOR v3 = v2 - ((XMVectorGetX(XMVector3Dot(v2, v1))) / (XMVectorGetX(XMVector3Dot(v1, v1)))) * v1;
        XMVECTOR v4 = r - ((XMVectorGetX(XMVector3Dot(r, v1))) / (XMVectorGetX(XMVector3Dot(v1, v1)))) * v1;
        float anglecheck = acosf(XMVectorGetX(XMVector3Dot(v3, v4)) / (sqrtf(XMVectorGetX(XMVector3Dot(v3, v3))) * sqrtf(XMVectorGetX(XMVector3Dot(v4, v4)))));
        c = Compare(fabsf(anglecheck), fabsf(XMScalarModAngle(angle)));
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f\n",
                TestName, vx, vy, vz, vw, XMScalarModAngle(angle));
            printmatrixe(m);
            printe("rotate");
            dqe(v2); printe("= "); dqe(r);
            printe("map: "); dqe(v3); dqe(v4);
            printe("new angle %f (%d)\n", anglecheck, c);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %f (%d)\n", TestName, anglecheck, c);
        }

    }
    return ret;
}
HRESULT Test112(LogProxy* pLog)
{
    //XMMatrixRotationRollPitchYaw
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++)
    {
        float y = ((float)XM_RAND()) / 4000.f;
        float p = ((float)XM_RAND()) / 4000.f;
        float r = ((float)XM_RAND()) / 4000.f;

        XMMATRIX checkm = XMMatrixRotationZ(r) * XMMatrixRotationX(p) * XMMatrixRotationY(y);

        XMMATRIX m = XMMatrixRotationRollPitchYaw(p, y, r);

        COMPARISON c = CompareXMMATRIX(m, checkm);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: %f %f %f (%d)\n", TestName, p, y, r, c);
            printmatrixe(m); printe("...\n"); printmatrixe(checkm);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }

    constexpr float inc = XM_PIDIV4 / 2.f;
    for (float y = -XM_2PI; y < XM_2PI; y += inc)
    {
        for (float p = -XM_2PI; p < XM_2PI; p += inc)
        {
            for (float r = -XM_2PI; r < XM_2PI; r += inc)
            {
                XMMATRIX checkm = XMMatrixRotationZ(r) * XMMatrixRotationX(p) * XMMatrixRotationY(y);

                XMMATRIX m = XMMatrixRotationRollPitchYaw(p, y, r);

                COMPARISON c = CompareXMMATRIX(m, checkm);
                if (c > WITHINBIGEPSILON)
                {
                    printe("%s: %f %f %f (%d)\n", TestName, p, y, r, c);
                    printmatrixe(m); printe("...\n"); printmatrixe(checkm);
                    ret = MATH_FAIL;
                }
                else
                {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }

    return ret;
}
HRESULT Test113(LogProxy* pLog)
{
    //XMMatrixRotationRollPitchYawFromVector
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++)
    {
        float y = ((float)XM_RAND()) / 4000.f;
        float p = ((float)XM_RAND()) / 4000.f;
        float r = ((float)XM_RAND()) / 4000.f;

        XMMATRIX checkm = XMMatrixRotationZ(r) * XMMatrixRotationX(p) * XMMatrixRotationY(y);

        XMVECTOR angles = XMVectorSet(p, y, r, c_Q_NAN);
        XMMATRIX m = XMMatrixRotationRollPitchYawFromVector(angles);

        COMPARISON c = CompareXMMATRIX(m, checkm);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: %f %f %f (%d)\n", TestName, p, y, r, c);
            printmatrixe(m); printe("...\n"); printmatrixe(checkm);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }

    constexpr float inc = XM_PIDIV4 / 2.f;
    for (float y = -XM_2PI; y < XM_2PI; y += inc)
    {
        for (float p = -XM_2PI; p < XM_2PI; p += inc)
        {
            for (float r = -XM_2PI; r < XM_2PI; r += inc)
            {
                XMMATRIX checkm = XMMatrixRotationZ(r) * XMMatrixRotationX(p) * XMMatrixRotationY(y);

                XMVECTOR angles = XMVectorSet(p, y, r, c_Q_NAN);
                XMMATRIX m = XMMatrixRotationRollPitchYawFromVector(angles);

                COMPARISON c = CompareXMMATRIX(m, checkm);
                if (c > WITHINBIGEPSILON)
                {
                    printe("%s: %f %f %f (%d)\n", TestName, p, y, r, c);
                    printmatrixe(m); printe("...\n"); printmatrixe(checkm);
                    ret = MATH_FAIL;
                }
                else
                {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }
    }

    return ret;
}
HRESULT Test114(LogProxy* pLog)
{
    //XMMatrixRotationX
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++)
    {
        float theta = GetRandomFloat16();

        float tmpc[4][4] = {};
        tmpc[0][0] = tmpc[3][3] = 1.f;
        tmpc[2][2] = tmpc[1][1] = cosf(theta);
        tmpc[2][1] = -(tmpc[1][2] = sinf(theta));
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixRotationX(theta);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: %f (%d)\n",
                TestName, theta, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }

    constexpr float inc = XM_PIDIV4 / 2.f;
    for (float theta = -XM_2PI; theta < XM_2PI; theta += inc)
    {
        float tmpc[4][4] = {};
        tmpc[0][0] = tmpc[3][3] = 1.f;
        tmpc[2][2] = tmpc[1][1] = cosf(theta);
        tmpc[2][1] = -(tmpc[1][2] = sinf(theta));
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixRotationX(theta);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: %f (%d)\n",
                TestName, theta, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test115(LogProxy* pLog)
{
    //XMMatrixRotationY
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++)
    {
        float theta = GetRandomFloat16();

        float tmpc[4][4] = {};
        tmpc[1][1] = tmpc[3][3] = 1.f;
        tmpc[2][2] = tmpc[0][0] = cosf(theta);
        tmpc[0][2] = -(tmpc[2][0] = sinf(theta));
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixRotationY(theta);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: %f (%d)\n",
                TestName, theta, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }

    constexpr float inc = XM_PIDIV4 / 2.f;
    for (float theta = -XM_2PI; theta < XM_2PI; theta += inc)
    {
        float tmpc[4][4] = {};
        tmpc[1][1] = tmpc[3][3] = 1.f;
        tmpc[2][2] = tmpc[0][0] = cosf(theta);
        tmpc[0][2] = -(tmpc[2][0] = sinf(theta));
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixRotationY(theta);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: %f (%d)\n",
                TestName, theta, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test116(LogProxy* pLog)
{
    //XMMatrixRotationZ
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++)
    {
        float theta = GetRandomFloat16();

        float tmpc[4][4] = {};
        tmpc[2][2] = tmpc[3][3] = 1.f;
        tmpc[1][1] = tmpc[0][0] = cosf(theta);
        tmpc[1][0] = -(tmpc[0][1] = sinf(theta));
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);

        XMMATRIX m = XMMatrixRotationZ(theta);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: %f (%d)\n",
                TestName, theta, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }

    constexpr float inc = XM_PIDIV4 / 2.f;
    for (float theta = -XM_2PI; theta < XM_2PI; theta += inc)
    {
        float tmpc[4][4] = {};
        tmpc[2][2] = tmpc[3][3] = 1.f;
        tmpc[1][1] = tmpc[0][0] = cosf(theta);
        tmpc[1][0] = -(tmpc[0][1] = sinf(theta));
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);

        XMMATRIX m = XMMatrixRotationZ(theta);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s: %f (%d)\n",
                TestName, theta, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else
        {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test117(LogProxy* pLog)
{
    //XMMatrixScaling
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++)
    {
        XMVECTOR v = GetRandomVector16();
        float tmpc[4][4] = {};
        for (int i = 0; i < 4; i++) {
            tmpc[i][i] = XMVectorGetByIndex(v, i);
        }
        tmpc[3][3] = 1.f;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixScaling(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

    }
    return ret;
}
HRESULT Test118(LogProxy* pLog)
{
    //XMMatrixScalingFromVector
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        XMVECTOR v = GetRandomVector16();
        float tmpc[4][4] = {};
        for (int i = 0; i < 4; i++) {
            tmpc[i][i] = XMVectorGetByIndex(v, i);
        }
        tmpc[3][3] = 1.f;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixScalingFromVector(v);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINEPSILON) {
            printe("%s: %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

    }
    return ret;
}
HRESULT Test119(LogProxy* pLog)
{
    //XMMatrixShadow
    XMVECTORF32 s = {}, l = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        s.v = GetRandomVector16();
        l.v = GetRandomVector16();
        s.v = XMVectorSetW(s, c_Q_NAN);
        l.v = XMVectorSetW(l, c_Q_NAN);
        if (XMVectorGetX(s) == 0 && XMVectorGetY(s) == 0) s.v = XMVectorSetX(s, 2);
        float scale = XMVectorGetW(XMVector3Dot(s, s));
        scale = sqrtf(scale);
        XMVECTOR vscale = XMVectorReplicate(scale);
        float dot = XMVectorGetX(XMVector4Dot(XMVectorDivide(s, vscale), l));

        float tmpc[4][4] = {};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmpc[i][j] = ((i == j) ? dot : 0) - (XMVectorGetByIndex(s, i) * XMVectorGetByIndex(l, j) / scale);
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixShadow(s, l);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f %f %f %f@(%d)@:\n",
                TestName, XMVectorGetX(s), XMVectorGetY(s), XMVectorGetZ(s), XMVectorGetW(s),
                XMVectorGetX(l), XMVectorGetY(l), XMVectorGetZ(l), XMVectorGetW(l), c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

    }
    return ret;
}
HRESULT Test120(LogProxy* pLog)
{
    //XMMatrixTransformation
    XMVECTOR so;
    XMVECTOR soq;
    XMVECTOR s;
    XMVECTOR ro;
    XMVECTOR rq;
    XMVECTOR t;

    XMMATRIX r;
    XMMATRIX check;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMMATRIX temp;
    XMMATRIX temp2 = XMMatrixIdentity();

    so = XMVectorSet(5, 7, 5, c_Q_NAN);
    soq = XMQuaternionRotationAxis(XMVectorSet(3, 2, 5, 0), 2.1f);
    s = XMVectorSet(2, 11, -3.4f, c_Q_NAN);
    ro = XMVectorSet(3, 4, 2, c_Q_NAN);
    rq = XMQuaternionRotationAxis(XMVectorSet(-1, -3, -6, 0), -2.2f);
    t = XMVectorSet(-4, 6, 2.4f, c_Q_NAN);

    for (int i = 0; i < 64; i++) {
        r = XMMatrixTransformation(
            (i & 32) ? so : XMVectorZero(), (i & 16) ? soq : XMVectorSet(0, 0, 0, 1), (i & 8) ? s : XMVectorSet(1, 1, 1, 0),
            (i & 4) ? ro : XMVectorZero(), (i & 2) ? rq : XMVectorSet(0, 0, 0, 1), (i & 1) ? t : XMVectorZero());

        check = XMMatrixIdentity();
        if (i & 32) {
            check.r[3] -= XMVectorSetW(so, 0);
        }
        if (i & 16) {
            temp2 = XMMatrixRotationQuaternion(soq);
            temp = XMMatrixTranspose(temp2);
            check *= temp;
        }
        if (i & 8) {
            temp = XMMatrixIdentity();
            temp.r[0] = XMVectorSetX(temp.r[0], XMVectorGetX(s));
            temp.r[1] = XMVectorSetY(temp.r[1], XMVectorGetY(s));
            temp.r[2] = XMVectorSetZ(temp.r[2], XMVectorGetZ(s));
            check *= temp;
        }
        if (i & 16) {
            check *= temp2;
        }
        if (i & 32) {
            check.r[3] += XMVectorSetW(so, 0);
        }

        if (i & 2) {
            if (i & 4) {
                check.r[3] -= XMVectorSetW(ro, 0);
            }
            temp = XMMatrixRotationQuaternion(rq);
            check *= temp;
            if (i & 4) {
                check.r[3] += XMVectorSetW(ro, 0);
            }
        }

        if (i & 1) {
            check.r[3] += XMVectorSetW(t, 0);
        }

        c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %d: (%d)\n", TestName, i, c);
            printmatrixe(r); printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d %d\n", TestName, i, c);
        }
    }
    return ret;
}
HRESULT Test121(LogProxy* pLog)
{
    //XMMatrixTransformation2D
    XMVECTOR so;
    float soq;
    XMVECTOR s;
    XMVECTOR ro;
    float rq;
    XMVECTOR t;

    XMMATRIX r;
    XMMATRIX check;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMMATRIX temp;
    XMMATRIX temp2 = XMMatrixIdentity();

    so = XMVectorSet(5, 7, c_Q_NAN, c_Q_NAN);
    soq = .52f;
    s = XMVectorSet(2, 11, c_Q_NAN, c_Q_NAN);
    ro = XMVectorSet(3, 4, c_Q_NAN, c_Q_NAN);
    rq = -.355f;
    t = XMVectorSet(-4, 6, c_Q_NAN, c_Q_NAN);

    for (int i = 0; i < 64; i++) {
        r = XMMatrixTransformation2D(
            (i & 32) ? so : XMVectorZero(), (i & 16) ? soq : 0, (i & 8) ? s : XMVectorSet(1, 1, 0, 0),
            (i & 4) ? ro : XMVectorZero(), (i & 2) ? rq : 0, (i & 1) ? t : XMVectorZero());

        check = XMMatrixIdentity();
        if (i & 32) {
            check.r[3] -= XMVectorSet(XMVectorGetX(so), XMVectorGetY(so), 0, 0);
        }
        if (i & 16) {
            temp2 = XMMatrixRotationZ(soq);
            temp = XMMatrixTranspose(temp2);
            check *= temp;
        }
        if (i & 8) {
            temp = XMMatrixIdentity();
            temp.r[0] = XMVectorSetX(temp.r[0], XMVectorGetX(s));
            temp.r[1] = XMVectorSetY(temp.r[1], XMVectorGetY(s));
            check *= temp;
        }
        if (i & 16) {
            check *= temp2;
        }
        if (i & 32) {
            check.r[3] += XMVectorSet(XMVectorGetX(so), XMVectorGetY(so), 0, 0);
        }

        if (i & 2) {
            if (i & 4) {
                check.r[3] -= XMVectorSet(XMVectorGetX(ro), XMVectorGetY(ro), 0, 0);
            }
            temp = XMMatrixRotationZ(rq);
            check *= temp;
            if (i & 4) {
                check.r[3] += XMVectorSet(XMVectorGetX(ro), XMVectorGetY(ro), 0, 0);
            }
        }

        if (i & 1) {
            check.r[3] += XMVectorSet(XMVectorGetX(t), XMVectorGetY(t), 0, 0);
        }

        c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON) {
            printe("%s: %d: (%d)\n", TestName, i, c);
            printmatrixe(r); printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d %d\n", TestName, i, c);
        }
    }
    return ret;
}
HRESULT Test122(LogProxy* pLog)
{
    //XMMatrixTranslation
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        XMVECTOR v = GetRandomVector16();
        float tmpc[4][4] = {};
        tmpc[0][0] = tmpc[1][1] = tmpc[2][2] = 1.f;
        for (int i = 0; i < 4; i++) {
            tmpc[3][i] = XMVectorGetByIndex(v, i);
        }
        tmpc[3][3] = 1.f;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        v = XMVectorSetW(v, c_Q_NAN);
        XMMATRIX m = XMMatrixTranslation(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINEPSILON) {
            printe("%s: %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

    }
    return ret;
}
HRESULT Test123(LogProxy* pLog)
{
    //XMMatrixTranslationFromVector
    XMVECTORF32 v = {};
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++)
    {
        v.v = GetRandomVector16();
        float tmpc[4][4] = {};
        tmpc[0][0] = tmpc[1][1] = tmpc[2][2] = 1.f;
        for (int i = 0; i < 4; i++) {
            tmpc[3][i] = XMVectorGetByIndex(v, i);
        }
        tmpc[3][3] = 1.f;
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        v.v = XMVectorSetW(v, c_Q_NAN);
        XMMATRIX m = XMMatrixTranslationFromVector(v);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c > WITHINEPSILON) {
            printe("%s: %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

    }
    return ret;
}
HRESULT Test124(LogProxy* pLog)
{
    //XMMatrixTranspose
    HRESULT ret = S_OK;
    for (int k = 0; k < 15; k++) {
        float tmp[4][4];
        float tmpc[4][4];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tmp[i][j] = GetRandomFloat16();
                tmpc[j][i] = tmp[i][j];
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX v(&tmp[0][0]);
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX check(&tmpc[0][0]);
        XMMATRIX m = XMMatrixTranspose(v);
        COMPARISON c = CompareXMMATRIX(m, check);
        if (c != EXACT) {
            printe("%s: (%d)\n", TestName, c);
            printmatrixe(v);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
        else {
            printi("%s: %d\n", TestName, c);
        }

    }
    return ret;
}

// DO NOT REMOVE, needed to complete unit test 578
#if 0
float g_Det1, g_Det2;
extern "C" {
    HRESULT WINAPI D3DXMatrixDecompose(D3DXVECTOR3* pOutScale, D3DXQUATERNION* pOutRotation,
        D3DXVECTOR3* pOutTranslation, CONST D3DXMATRIX* pM);
}

#ifndef XMRANKDECOMPOSE
#define XMRANKDECOMPOSE(a, b, c, x, y, z)      \
    if((x) < (y))                   \
    {                               \
        if((y) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 1;                \
            (c) = 0;                \
        }                           \
        else                        \
        {                           \
            (a) = 1;                \
                                    \
            if((x) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 0;            \
            }                       \
            else                    \
            {                       \
                (b) = 0;            \
                (c) = 2;            \
            }                       \
        }                           \
    }                               \
    else                            \
    {                               \
        if((x) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 0;                \
            (c) = 1;                \
        }                           \
        else                        \
        {                           \
            (a) = 0;                \
                                    \
            if((y) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 1;            \
            }                       \
            else                    \
            {                       \
                (b) = 1;            \
                (c) = 2;            \
            }                       \
        }                           \
    }
#endif

#ifndef XM_DECOMP_EPSILON
#define XM_DECOMP_EPSILON 0.0001f
#endif

HRESULT WINAPI D3DXMatrixDecompose(D3DXVECTOR3* pOutScale, D3DXQUATERNION* pOutRotation,
    D3DXVECTOR3* pOutTranslation, CONST D3DXMATRIX* pM)
{
    FLOAT fDet;
    FLOAT* pfScales;
    D3DXVECTOR3* ppvBasis[3];
    D3DXMATRIX matTemp;
    D3DXVECTOR3 pvCanonicalBasis[3];
    pvCanonicalBasis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    pvCanonicalBasis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pvCanonicalBasis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

    pOutTranslation->x = pM->_41;
    pOutTranslation->y = pM->_42;
    pOutTranslation->z = pM->_43;

    D3DXMatrixIdentity(&matTemp);

    ppvBasis[0] = (D3DXVECTOR3*)&matTemp._11;
    ppvBasis[1] = (D3DXVECTOR3*)&matTemp._21;
    ppvBasis[2] = (D3DXVECTOR3*)&matTemp._31;

    *(ppvBasis[0]) = *((D3DXVECTOR3*)&pM->_11);
    *(ppvBasis[1]) = *((D3DXVECTOR3*)&pM->_21);
    *(ppvBasis[2]) = *((D3DXVECTOR3*)&pM->_31);

    pfScales = (FLOAT*)pOutScale;

    pfScales[0] = D3DXVec3Length(ppvBasis[0]);
    pfScales[1] = D3DXVec3Length(ppvBasis[1]);
    pfScales[2] = D3DXVec3Length(ppvBasis[2]);

    uint32_t a, b, c;

    XMRANKDECOMPOSE(a, b, c, pfScales[0], pfScales[1], pfScales[2]);

    if (pfScales[a] < XM_DECOMP_EPSILON)
    {
        *(ppvBasis[a]) = pvCanonicalBasis[a];
    }


    D3DXVec3Normalize(ppvBasis[a], ppvBasis[a]);

    if (pfScales[b] < XM_DECOMP_EPSILON)
    {
        uint32_t aa, bb, cc;
        FLOAT fAbsX, fAbsY, fAbsZ;

        fAbsX = (FLOAT)fabs(ppvBasis[a]->x);
        fAbsY = (FLOAT)fabs(ppvBasis[a]->y);
        fAbsZ = (FLOAT)fabs(ppvBasis[a]->z);

        XMRANKDECOMPOSE(aa, bb, cc, fAbsX, fAbsY, fAbsZ);

        D3DXVec3Cross(ppvBasis[b], ppvBasis[a], pvCanonicalBasis + cc);
    }

    D3DXVec3Normalize(ppvBasis[b], ppvBasis[b]);

    if (pfScales[c] < XM_DECOMP_EPSILON)
    {
        D3DXVec3Cross(ppvBasis[c], ppvBasis[a], ppvBasis[b]);
    }

    D3DXVec3Normalize(ppvBasis[c], ppvBasis[c]);

    fDet = D3DXMatrixDeterminant(&matTemp);
    printf("Determinant %f\n", fDet);
    // use Kramer's rule to check for handedness of coordinate system
    if (fDet < 0.0f)
    {
        // switch coordinate system by negating the scale and inverting the basis vector on the x-axis
        pfScales[a] = -pfScales[a];
        *ppvBasis[a] = -(*ppvBasis[a]);

        fDet = -fDet;
    }

    fDet -= 1.0f;
    fDet *= fDet;

    if (XM_DECOMP_EPSILON < fDet)
    {
        printf("Found epsilon %f\n", fDet);
        //		Non-SRT matrix encountered
        return D3DERR_INVALIDCALL;
    }

    // generate the quaternion from the matrix
    D3DXQuaternionRotationMatrix(pOutRotation, &matTemp);

    return S_OK;
}
#endif

HRESULT Test575(LogProxy* pLog)
{
    //XMMatrixSet
    HRESULT ret = S_OK;

    XMMATRIX m = XMMatrixSet(1.f, 2.f, 3.f, 4.f,
        5.f, 6.f, 7.f, 8.f,
        9.f, 10.f, 11.f, 12.f,
        13.f, 14.f, 15.f, 16.f);

    XMMATRIX check;
    check.r[0] = XMVectorSet(1.f, 2.f, 3.f, 4.f);
    check.r[1] = XMVectorSet(5.f, 6.f, 7.f, 8.f);
    check.r[2] = XMVectorSet(9.f, 10.f, 11.f, 12.f);
    check.r[3] = XMVectorSet(13.f, 14.f, 15.f, 16.f);

    COMPARISON c = CompareXMMATRIX(m, check);
    if (c != EXACT)
    {
        printe("%s:\n", TestName);
        printmatrixe(m);
        printe("...\n");
        printmatrixe(check);
        ret = MATH_FAIL;
    }

    // DO NOT REMOVE, needed to complete unit test 578
#if 0
// Based on reference source
    Determinant - 0.326200
        Found epsilon 0.454006
        - 4.053139 - 3.453234 - 2.540754 0.000000
        - 4.145546 - 0.827176 0.420856 0.000000
        8.465755 - 0.590435 3.418664 0.000000
        4.707327 38.320271 0.950012 1.000000
        D3DXMatrixDecompose 5.899847 4.248163 - 9.149038 | 6.000000 7.000000 8.000000 9.0
        00000 | 4.707327 38.320271 0.950012 | FALSE
        // Based on API call using x64
        - 4.053139 - 3.453234 - 2.540754 0.000000
        - 4.145546 - 0.827176 0.420856 0.000000
        8.465755 - 0.590435 3.418664 0.000000
        4.707327 38.320271 0.950012 1.000000
        D3DXMatrixDecompose 5.899847 4.248163 - 9.149037 | 6.000000 7.000000 8.000000 9.0
        00000 | 4.707327 38.320271 0.950012 | FALSE
        // XNA Math call
        Determinant - 1.#QNAN0
        Found epsilon 0.000000
        XMMatrixDecompose:
    -4.053139 - 3.453235 - 2.540755 0.000000
        - 4.145546 - 0.827176 0.420856 0.000000
        8.465755 - 0.590435 3.418665 0.000000
        4.707324 38.320267 0.950008 1.000000
        5.899847 4.248164 9.149038 0.000000 | 0.164693 0.011497 0.750894 0.130023 | 4.70
        7324 38.320267 0.950008 1.000000 | TRUE
        ...
        5.899847 4.248164 - 9.149038 0.000000 | -0.147442 - 0.442326 - 0.891207 0.453596 |
        4.707325 38.320271 0.950011 1.000000 returned TRUE
#endif

#if 0
        static const XMVECTORF32 so = { 5,7,5,c_Q_NAN };
    XMVECTOR soq = XMQuaternionRotationAxis(XMVectorSet(3, 2, 5, 0), 2.1f);
    static const XMVECTORF32 s = { 2,11,-3.4f,c_Q_NAN };
    static const XMVECTORF32 ro = { 3,4,2,c_Q_NAN };
    XMVECTOR rq = XMQuaternionRotationAxis(XMVectorSet(-1, -3, -6, 0), -2.2f);
    static const XMVECTORF32 t = { -4,6,2.4f,c_Q_NAN };

    m = XMMatrixTransformation(so, soq, s, ro, rq, t);

    D3DXVECTOR3 dOutScale;
    D3DXQUATERNION dOutRotation(6.0f, 7.0f, 8.0f, 9.0f);
    D3DXVECTOR3 dOutTranslation;
    D3DXMatrixDecompose(&dOutScale, &dOutRotation, &dOutTranslation, (CONST D3DXMATRIX*) & m);
    printmatrixe(m);
    printe("D3DXMatrixDecompose %f %f %f | %f %f %f %f | %f %f %f | FALSE\n",
        dOutScale.x, dOutScale.y, dOutScale.z,
        dOutRotation.x, dOutRotation.y, dOutRotation.z, dOutRotation.w,
        dOutTranslation.x, dOutTranslation.y, dOutTranslation.z);
#endif
    return ret;
}

HRESULT Test578(LogProxy* pLog)
{
    //XMMatrixDecompose
    HRESULT ret = S_OK;
    static const XMVECTORF32 so = { { { 5,7,5,c_Q_NAN } } };
    XMVECTOR soq = XMQuaternionRotationAxis(XMVectorSet(3, 2, 5, 0), 2.1f);
    static const XMVECTORF32 s = { { { 2,11,-3.4f,c_Q_NAN } } };
    static const XMVECTORF32 ro = { { { 3,4,2,c_Q_NAN } } };
    XMVECTOR rq = XMQuaternionRotationAxis(XMVectorSet(-1, -3, -6, 0), -2.2f);
    static const XMVECTORF32 t = { { { -4,6,2.4f,c_Q_NAN } } };

    XMMATRIX m = XMMatrixTransformation(so, soq, s, ro, rq, t);
    XMVECTOR outScale, outRotQuat, outTrans;
    static const XMVECTORF32 checkScale = { { { 5.899847f, 4.248164f, -9.149038f, 0.453596f } } };
    static const XMVECTORF32 checkTrans = { { { 4.707325f, 38.320271f, 0.950011f, 1.000000f } } };
    // The first test must fail. The generated matrix has no quaternion
    if (!XMMatrixDecompose(&outScale, &outRotQuat, &outTrans, m))
    {
        COMPARISON c1 = CompareXMVECTOR(outScale, checkScale, 3);
        COMPARISON c2 = CompareXMVECTOR(outTrans, checkTrans, 4);
        if (c1 > WITHIN4096 || c2 > WITHIN4096)
        {
            printe("%s:\n", TestName);
            printmatrixe(m);
            printe("%f %f %f %f | %f %f %f %f | returned FALSE %d,%d\n",
                XMVectorGetX(outScale), XMVectorGetY(outScale), XMVectorGetZ(outScale), XMVectorGetW(outScale),
                XMVectorGetX(outTrans), XMVectorGetY(outTrans), XMVectorGetZ(outTrans), XMVectorGetW(outTrans), c1, c2);
            printe("%f %f %f %f | %f %f %f %f | FALSE\n",
                checkScale.f[0], checkScale.f[1], checkScale.f[2], checkScale.f[3],
                checkTrans.f[0], checkTrans.f[1], checkTrans.f[2], checkTrans.f[3]);
            ret = MATH_FAIL;
        }
    }
    else {
        printe("%s:\n", TestName);
        printmatrixe(m);
        printe("%f %f %f %f | %f %f %f %f | %f %f %f %f | returned TRUE\n...\n",
            XMVectorGetX(outScale), XMVectorGetY(outScale), XMVectorGetZ(outScale), XMVectorGetW(outScale),
            XMVectorGetX(outRotQuat), XMVectorGetY(outRotQuat), XMVectorGetZ(outRotQuat), XMVectorGetW(outRotQuat),
            XMVectorGetX(outTrans), XMVectorGetY(outTrans), XMVectorGetZ(outTrans), XMVectorGetW(outTrans));
        printe("%f %f %f %f | %f %f %f %f | FALSE\n",
            checkScale.f[0], checkScale.f[1], checkScale.f[2], checkScale.f[3],
            checkTrans.f[0], checkTrans.f[1], checkTrans.f[2], checkTrans.f[3]);
        ret = MATH_FAIL;
    }
#if 0

    static const XMVECTORF32 checkScale = { 5.899847f, 4.248164f, -9.149038f, 0.f };
    static const XMVECTORF32 checkRotQuat = { -0.147442f, -0.442326f, -0.891207f, 0.453596f };
    static const XMVECTORF32 checkTrans = { 4.707325f, 38.320271f, 0.950011f, 1.000000f };

    COMPARISON c1 = CompareXMVECTOR(outScale, checkScale, 4);
    COMPARISON c2 = CompareXMVECTOR(outRotQuat, checkRotQuat, 4);
    COMPARISON c3 = CompareXMVECTOR(outTrans, checkTrans, 4);
    if (c1 > WITHINEPSILON || c2 > WITHINEPSILON || c3 > WITHINEPSILON)
    {
        printe("%s:\n", TestName);
        printmatrixe(m);
        printe("%f %f %f %f | %f %f %f %f | %f %f %f %f | TRUE\n...\n",
            XMVectorGetX(outScale), XMVectorGetY(outScale), XMVectorGetZ(outScale), XMVectorGetW(outScale),
            XMVectorGetX(outRotQuat), XMVectorGetY(outRotQuat), XMVectorGetZ(outRotQuat), XMVectorGetW(outRotQuat),
            XMVectorGetX(outTrans), XMVectorGetY(outTrans), XMVectorGetZ(outTrans), XMVectorGetW(outTrans));
        printe("%f %f %f %f | %f %f %f %f | %f %f %f %f returned TRUE\n",
            XMVectorGetX(checkScale), XMVectorGetY(checkScale), XMVectorGetZ(checkScale), XMVectorGetW(checkScale),
            XMVectorGetX(checkRotQuat), XMVectorGetY(checkRotQuat), XMVectorGetZ(checkRotQuat), XMVectorGetW(checkRotQuat),
            XMVectorGetX(checkTrans), XMVectorGetY(checkTrans), XMVectorGetZ(checkTrans), XMVectorGetW(checkTrans));
        ret = MATH_FAIL;
    }
#endif

    return ret;
}

HRESULT Test609(LogProxy* pLog)
{
    HRESULT ret = S_OK;

    // XMMATRIX operator-
    {
        float tmp[4][4];
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp[x][y] = GetRandomFloat(100.0f);
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m(&tmp[0][0]);

        XMMATRIX check(-tmp[0][0], -tmp[0][1], -tmp[0][2], -tmp[0][3],
            -tmp[1][0], -tmp[1][1], -tmp[1][2], -tmp[1][3],
            -tmp[2][0], -tmp[2][1], -tmp[2][2], -tmp[2][3],
            -tmp[3][0], -tmp[3][1], -tmp[3][2], -tmp[3][3]);

        XMMATRIX r = -m;

        COMPARISON c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (-): (%d)\n", TestName, c);
            printmatrixe(r); printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    // XMMATRIX operator+,+=
    {
        float tmp1[4][4], tmp2[4][4];
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp1[x][y] = GetRandomFloat(100.0f);
                tmp2[x][y] = GetRandomFloat(100.0f);
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m1(&tmp1[0][0]);
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m2(&tmp2[0][0]);

        XMMATRIX check(tmp1[0][0] + tmp2[0][0], tmp1[0][1] + tmp2[0][1], tmp1[0][2] + tmp2[0][2], tmp1[0][3] + tmp2[0][3],
            tmp1[1][0] + tmp2[1][0], tmp1[1][1] + tmp2[1][1], tmp1[1][2] + tmp2[1][2], tmp1[1][3] + tmp2[1][3],
            tmp1[2][0] + tmp2[2][0], tmp1[2][1] + tmp2[2][1], tmp1[2][2] + tmp2[2][2], tmp1[2][3] + tmp2[2][3],
            tmp1[3][0] + tmp2[3][0], tmp1[3][1] + tmp2[3][1], tmp1[3][2] + tmp2[3][2], tmp1[3][3] + tmp2[3][3]);
        XMMATRIX r = m1 + m2;
        XMMATRIX r2 = m1;
        r2 += m2;

        COMPARISON c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (+): (%d)\n", TestName, c);
            printmatrixe(m1);
            printe("+: ");
            printmatrixe(m2);
            printe("= ");
            printmatrixe(r);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }

        c = CompareXMMATRIX(r2, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (+=): (%d)\n", TestName, c);
            printmatrixe(m1);
            printe("+: ");
            printmatrixe(m2);
            printe("= ");
            printmatrixe(r2);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    // XMMATRIX operator-,-=
    {
        float tmp1[4][4], tmp2[4][4];
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp1[x][y] = GetRandomFloat(100.0f);
                tmp2[x][y] = GetRandomFloat(100.0f);
            }
        }
#ifdef _MSC_VER
#pragma warning( suppress : 6385 )
#endif
        XMMATRIX m1(&tmp1[0][0]);
        XMMATRIX m2(&tmp2[0][0]);

        XMMATRIX check(tmp1[0][0] - tmp2[0][0], tmp1[0][1] - tmp2[0][1], tmp1[0][2] - tmp2[0][2], tmp1[0][3] - tmp2[0][3],
            tmp1[1][0] - tmp2[1][0], tmp1[1][1] - tmp2[1][1], tmp1[1][2] - tmp2[1][2], tmp1[1][3] - tmp2[1][3],
            tmp1[2][0] - tmp2[2][0], tmp1[2][1] - tmp2[2][1], tmp1[2][2] - tmp2[2][2], tmp1[2][3] - tmp2[2][3],
            tmp1[3][0] - tmp2[3][0], tmp1[3][1] - tmp2[3][1], tmp1[3][2] - tmp2[3][2], tmp1[3][3] - tmp2[3][3]);
        XMMATRIX r = m1 - m2;
        XMMATRIX r2 = m1;
        r2 -= m2;

        COMPARISON c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (-): (%d)\n", TestName, c);
            printmatrixe(m1);
            printe("-: ");
            printmatrixe(m2);
            printe("= ");
            printmatrixe(r);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }

        c = CompareXMMATRIX(r2, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (-=): (%d)\n", TestName, c);
            printmatrixe(m1);
            printe("-: ");
            printmatrixe(m2);
            printe("= ");
            printmatrixe(r2);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    // XMMATRIX operator*S,S*,*=S
    {
        float tmp[4][4];
        float s = GetRandomFloat(100.0f);
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp[x][y] = GetRandomFloat(100.0f);
            }
        }
        XMMATRIX m(&tmp[0][0]);

        XMMATRIX check(tmp[0][0] * s, tmp[0][1] * s, tmp[0][2] * s, tmp[0][3] * s,
            tmp[1][0] * s, tmp[1][1] * s, tmp[1][2] * s, tmp[1][3] * s,
            tmp[2][0] * s, tmp[2][1] * s, tmp[2][2] * s, tmp[2][3] * s,
            tmp[3][0] * s, tmp[3][1] * s, tmp[3][2] * s, tmp[3][3] * s);

        XMMATRIX r = m * s;
        XMMATRIX r2 = s * m;
        XMMATRIX r3 = m;
        r3 *= s;

        COMPARISON c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (*S): %f (%d)\n", TestName, s, c);
            printmatrixe(m);
            printe("= ");
            printmatrixe(r);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }

        c = CompareXMMATRIX(r2, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (S*): %f (%d)\n", TestName, s, c);
            printmatrixe(m);
            printe("= ");
            printmatrixe(r2);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }

        c = CompareXMMATRIX(r3, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (*=S): %f (%d)\n", TestName, s, c);
            printmatrixe(m);
            printe("= ");
            printmatrixe(r3);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    // XMMATRIX operator /S, /=S
    {
        float tmp[4][4];
        float s = GetRandomFloat(100.0f);
        s = (s > 0.f) ? (s + 1.0f) : (s - 1.0f); // Ensure non-zero
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                tmp[x][y] = GetRandomFloat(100.0f);
            }
        }
        XMMATRIX m(&tmp[0][0]);

        XMMATRIX check(tmp[0][0] / s, tmp[0][1] / s, tmp[0][2] / s, tmp[0][3] / s,
            tmp[1][0] / s, tmp[1][1] / s, tmp[1][2] / s, tmp[1][3] / s,
            tmp[2][0] / s, tmp[2][1] / s, tmp[2][2] / s, tmp[2][3] / s,
            tmp[3][0] / s, tmp[3][1] / s, tmp[3][2] / s, tmp[3][3] / s);

        XMMATRIX r = m / s;
        XMMATRIX r2 = m;
        r2 /= s;

        COMPARISON c = CompareXMMATRIX(r, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (/S): %f (%d)\n", TestName, s, c);
            printmatrixe(m);
            printe("= ");
            printmatrixe(r);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }

        c = CompareXMMATRIX(r2, check);
        if (c > WITHINBIGEPSILON)
        {
            printe("%s (/=S): %f (%d)\n", TestName, s, c);
            printmatrixe(m);
            printe("= ");
            printmatrixe(r2);
            printe("...");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test611(LogProxy* pLog)
{
    // XMMatrixVectorTensorProduct
    HRESULT ret = S_OK;
    XMVECTORF32 v1 = {}, v2 = {};

    for (int k = 0; k < 10; ++k)
    {
        v1.v = GetRandomVector16();
        v2.v = GetRandomVector16();

        XMFLOAT4X4 check;
        check.m[0][0] = v1.f[0] * v2.f[0];
        check.m[0][1] = v1.f[0] * v2.f[1];
        check.m[0][2] = v1.f[0] * v2.f[2];
        check.m[0][3] = v1.f[0] * v2.f[3];

        check.m[1][0] = v1.f[1] * v2.f[0];
        check.m[1][1] = v1.f[1] * v2.f[1];
        check.m[1][2] = v1.f[1] * v2.f[2];
        check.m[1][3] = v1.f[1] * v2.f[3];

        check.m[2][0] = v1.f[2] * v2.f[0];
        check.m[2][1] = v1.f[2] * v2.f[1];
        check.m[2][2] = v1.f[2] * v2.f[2];
        check.m[2][3] = v1.f[2] * v2.f[3];

        check.m[3][0] = v1.f[3] * v2.f[0];
        check.m[3][1] = v1.f[3] * v2.f[1];
        check.m[3][2] = v1.f[3] * v2.f[2];
        check.m[3][3] = v1.f[3] * v2.f[3];

        XMMATRIX chk = XMLoadFloat4x4(&check);
        XMMATRIX r = XMMatrixVectorTensorProduct(v1, v2);

        COMPARISON c = CompareXMMATRIX(r, chk);
        if (c > WITHIN4096)
        {
            printe("%s: %f %f %f %f (*) %f %f %f %f \n",
                TestName, XMVectorGetX(v1), XMVectorGetY(v1), XMVectorGetZ(v1), XMVectorGetW(v1),
                XMVectorGetX(v2), XMVectorGetY(v2), XMVectorGetZ(v2), XMVectorGetW(v2));
                        printe("= ");
            printmatrixe(r);
            printe("...");
            printmatrixe(chk);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
