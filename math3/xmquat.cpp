//-------------------------------------------------------------------------------------
// xmquat.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

HRESULT Test139(LogProxy* pLog)
{
//XMQuaternionBaryCentric and XMQuaternionBaryCentricV 
    HRESULT ret = S_OK;

    //XMQuaternionBaryCentric
    {
        XMVECTORF32 q1 = {{1,0,0,0}};
        XMVECTORF32 q2 = {{0,1,0,0}};
        XMVECTORF32 q3 = {{0,0,1,0}};
        float fv[] = {0,1,0,1};
        float gv[] = {0,0,1,-1};
        XMVECTORF32 chk[] = {q1,q2,q3,q1};
        XMVECTOR r;
        COMPARISON c;

        float f,g;
        XMVECTOR check;

        int k;
        for(k = 0; k < countof(fv); k++) {
            r = XMQuaternionBaryCentric(q1, q2, q3, fv[k], gv[k]);
            c = CompareXMVECTOR(r,chk[k],4);
            if(c > WITHIN4096) {
                printe("%s: f:%f g:%f (%d)\n", TestName, fv[k], gv[k], c);
                dqe(q1);dqe(q2);dqe(q3);dqe(r);dqe(chk[k]);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName, c);
            }
        }

        for(k = 0; k < 10; k++) {
            for(int i = 0; i < 4; i++) {
                q1.v = XMVectorSetByIndex(q1,((float)rand()) / 2000.f - 8.f,i);
                q2.v = XMVectorSetByIndex(q2,((float)rand()) / 2000.f - 8.f,i);
                q3.v = XMVectorSetByIndex(q3,((float)rand()) / 2000.f - 8.f,i);
            }
            f = ((float)rand()) / 2000.f - 8.f;
            g = ((float)rand()) / 2000.f - 8.f;
            r = XMQuaternionBaryCentric(q1,q2,q3,f,g);
            check = XMQuaternionSlerp(XMQuaternionSlerp(q1,q2,f+g),XMQuaternionSlerp(q1,q3,f+g),g/(f+g));
            c = CompareXMVECTOR(r,check,4);
            if(c > WITHIN4096) {
                printe("%s: f:%f g:%f (%d)\n", TestName, f, g, c);
                dqe(q1);dqe(q2);dqe(q3);dqe(r);dqe(check);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName, c);
            }
        }
    }

    //XMQuaternionBaryCentricV
    {
        XMVECTORF32 q1 = {{1,0,0,0}};
        XMVECTORF32 q2 = {{0,1,0,0}};
        XMVECTORF32 q3 = {{0,0,1,0}};
        float fv[] = {0,1,0,1};
        float gv[] = {0,0,1,-1};
        XMVECTORF32 chk[] = {q1,q2,q3,q1};
        XMVECTOR r;
        COMPARISON c;

        XMVECTOR F = {},G = {};
        XMVECTOR check;

        int k;
        for(k = 0; k < countof(fv); k++) {
            r = XMQuaternionBaryCentric(q1, q2, q3, fv[k], gv[k]);
            c = CompareXMVECTOR(r,chk[k],4);
            if(c > WITHIN4096) {
                printe("%s: f:%f g:%f (%d)\n", TestName, fv[k], gv[k], c);
                dqe(q1);dqe(q2);dqe(q3);dqe(r);dqe(chk[k]);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName, c);
            }
        }

        for(k = 0; k < 10; k++) {
            float f = ((float)rand()) / 2000.f - 8.f;
            float g = ((float)rand()) / 2000.f - 8.f;
            for(int i = 0; i < 4; i++) {
                q1.v = XMVectorSetByIndex(q1,((float)rand()) / 2000.f - 8.f,i);
                q2.v = XMVectorSetByIndex(q2,((float)rand()) / 2000.f - 8.f,i);
                q3.v = XMVectorSetByIndex(q3,((float)rand()) / 2000.f - 8.f,i);
                F = XMVectorSetByIndex(F,f,i);
                G = XMVectorSetByIndex(G,g,i);
            }
            r = XMQuaternionBaryCentricV(q1,q2,q3,F,G);
            check = XMQuaternionSlerp(XMQuaternionSlerp(q1,q2,f+g),XMQuaternionSlerp(q1,q3,f+g),g/(f+g));
            c = CompareXMVECTOR(r,check,4);
            if(c > WITHIN4096) {
                printe("%s: f:%f g:%f (%d)\n", TestName, f, g, c);
                dqe(q1);dqe(q2);dqe(q3);dqe(r);dqe(check);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName, c);
            }
        }
    }
    
    return ret;
}
HRESULT Test140(LogProxy* pLog)
{
//XMQuaternionConjugate 
    XMVECTORF32 check={};
    HRESULT ret = S_OK;
    COMPARISON c;
    int i,k;

    for(k = 0; k < 10; k++) {
        XMVECTOR v = GetRandomVector16();
        for(i = 0; i < 4; i++) {
            if(i != 3) check.v = XMVectorSetByIndex(check,-XMVectorGetByIndex(v,i),i);
            else check.v = XMVectorSetByIndex(check,XMVectorGetByIndex(v,i),i);
        }
        XMVECTOR r = XMQuaternionConjugate(v);
        c = CompareXMVECTOR(r,check,4);
        if(c > WITHINEPSILON) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test141(LogProxy* pLog)
{
//XMQuaternionDot 
    XMVECTORF32 l={};
    XMVECTORF32 v={};
    COMPARISON c = EXACT, temp;
    HRESULT ret = S_OK;
    int i, j;
    for(i = 0; i < 10; i++) {
        for(j = 0; j < 4; j++) {
            l.v = XMVectorSetByIndex(l,((float)rand()) / 1000.f,j);
            v.v = XMVectorSetByIndex(v,((float)rand()) / 1000.f,j);
        }
        XMVECTOR r = XMQuaternionDot(l, v);
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(l)*XMVectorGetX(v)+  XMVectorGetY(l)*XMVectorGetY(v)+ XMVectorGetZ(l)*XMVectorGetZ(v)+  XMVectorGetW(l)*XMVectorGetW(v));
        temp = CompareXMVECTOR(r, check,4);
        c = std::max(c,temp);
        if( temp > WITHIN4096) {
            printe ("%s: %f %f %f %f dot %f %f %f %f = %f ... %f (%d)\n",
                TestName, XMVectorGetX(l),XMVectorGetY(l),XMVectorGetZ(l),XMVectorGetW(l),
                XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),XMVectorGetX(r),XMVectorGetX(check), temp);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, temp);
        }
    }
    return ret;
}
HRESULT Test142(LogProxy* pLog)
{
//XMQuaternionEqual 
    XMVECTORF32 v1={}, v2={};
    BOOL r, check;
    int i,j;
    HRESULT ret = S_OK;
    for(j = 0; j < 16; j++) {
        for(i = 0; i < 4; i++) {
            float ftemp = (float)rand();
            v1.v = XMVectorSetByIndex(v1,ftemp,i);
            v2.v = XMVectorSetByIndex(v2,ftemp,i);
        }
        check = TRUE;
        if(j & 8) { v1.v = XMVectorSetX(v1,XMVectorGetX(v2)+.01f); check = FALSE; } else {}
        if(j & 4) { v1.v = XMVectorSetY(v1,XMVectorGetY(v2)+.01f); check = FALSE; } else {}
        if(j & 2) { v1.v = XMVectorSetZ(v1,XMVectorGetZ(v2)+.01f); check = FALSE; } else {}
        if(j & 1) { v1.v = XMVectorSetW(v1,XMVectorGetW(v2)+.01f); check = FALSE; } else {}
        r = XMQuaternionEqual(v1,v2);
        if(r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n",TestName,j,i,r,check);
            ret = MATH_FAIL;
        }
        check = TRUE;
        for(i = 0; i < 4; i++) {
            float ftemp = (float)rand();
            v1.v = XMVectorSetByIndex(v1,ftemp,i);
            v2.v = XMVectorSetByIndex(v2,ftemp,i);
        }
        if(j & 8) { v1.v = XMVectorSetX(v1,XMVectorGetX(v2)-.01f); check = FALSE; } else {}
        if(j & 4) { v1.v = XMVectorSetY(v1,XMVectorGetY(v2)-.01f); check = FALSE; } else {}
        if(j & 2) { v1.v = XMVectorSetZ(v1,XMVectorGetZ(v2)-.01f); check = FALSE; } else {}
        if(j & 1) { v1.v = XMVectorSetW(v1,XMVectorGetW(v2)-.01f); check = FALSE; } else {}
        r = XMQuaternionEqual(v1,v2);
        if(r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n",TestName,j,i,r,check);
            ret = MATH_FAIL;
        }
    }
    return ret;	
}
HRESULT Test143(LogProxy* pLog)
{
//XMQuaternionExp 
    COMPARISON c;
    HRESULT ret = S_OK;

    XMVECTORF32 q1[15] = {
        {{0,0,0,1234}}, {{Pi,0,0,2345}}, {{3*Pi,0,0,3456}},
    };
    XMVECTOR r, check;

    for(int k = 0; k < countof(q1); k++) {
        if(k > 2) {
            for(int i = 0; i < 4; i++) {
                q1[k].v = XMVectorSetByIndex(q1[k],((float)rand()) / 2000.f - 8.f,i);
            }
            q1[k].v = 2 * XMQuaternionNormalize(q1[k]);
            q1[k].v = XMVectorSetW(q1[k],0);
        }

        r = XMQuaternionExp(q1[k]);
        check = ScalarQuatExp(q1[k]);
        c = CompareXMVECTOR(r,check,4);
        if((c > WITHINBIGEPSILON)) {
            printe("%s: (%d)\n", TestName, c);
            dqe(q1[k]);dqe(r); dqe(check);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName,c);
        }
    }

    return ret;
}
HRESULT Test144(LogProxy* pLog)
{
//XMQuaternionIdentity 
    XMVECTORF32 check = {{0,0,0,1}};
    XMVECTOR r = XMQuaternionIdentity();
    COMPARISON c = CompareXMVECTOR(r,check,4);
    if(c != EXACT) {
        printe("%s: %f %f %f %f ... %f %f %f %f \n",
            TestName, XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
            XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
        return MATH_FAIL;
    } else {
        return S_OK;
    }
}
HRESULT Test145(LogProxy* pLog)
{
//XMQuaternionInverse 
    XMVECTORF32 v = {{.5,.5,.5,.5}};
    XMVECTORF32 check = {{-.5,-.5,-.5,.5}};
    XMVECTOR r = XMQuaternionInverse(v);
    HRESULT ret = S_OK;
    COMPARISON c = CompareXMVECTOR(r,check,4);
    if(c > WITHIN4096) {
        printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
            TestName, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
            XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
            XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
        ret = MATH_FAIL;
    }

    v.v = check.v = XMVectorZero();
    r = XMQuaternionInverse(v);
    c = CompareXMVECTOR(r,check,4);
    if(c > EXACT) {
        printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
            TestName,
            XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
            XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
            XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
        ret = MATH_FAIL;
    }

    
    for(int k = 0; k < 15; k++) {
        v.v = GetRandomVector16();
        XMVECTOR len = XMVectorReplicate(XMVectorGetX(v)*XMVectorGetX(v)+XMVectorGetY(v)*XMVectorGetY(v)+XMVectorGetZ(v)*XMVectorGetZ(v)+XMVectorGetW(v)*XMVectorGetW(v));
        FLOAT checkx = -XMVectorGetX(v)/XMVectorGetX(len);
        FLOAT checky = -XMVectorGetY(v)/XMVectorGetY(len);
        FLOAT checkz = -XMVectorGetZ(v)/XMVectorGetZ(len);
        FLOAT checkw =  XMVectorGetW(v)/XMVectorGetW(len);
        check.v = XMVectorSet(checkx,checky,checkz,checkw);
        r = XMQuaternionInverse(v);
        c = CompareXMVECTOR(r,check,4);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test146(LogProxy* pLog)
{
//XMQuaternionIsIdentity 
    BOOL check, r;
    float x,y,z,w;
    XMVECTOR v;
    HRESULT ret = S_OK;

    for(x = -1; x <= 1; x += 1) {
        for(y = -1; y <= 1; y += 1) {
            for(z = -1; z <= 1; z += 1) {
                for(w = -1; w <= 1; w += 1) {
                    if(x == 0 && y == 0 && z == 0 && w == 1) {
                        check = TRUE;
                    } else {
                        check = FALSE;
                    }
                    v = XMVectorSet(x,y,z,w);
                    r = XMQuaternionIsIdentity(v);
                    if(r != check) {
                        printe("%s: %f %f %f %f = %d ... %d\n",
                            TestName, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v), r,check);
                        ret = MATH_FAIL;
                    }
                }
            }
        }
    }
    r = XMQuaternionIsIdentity(XMQuaternionIdentity());
    if(r != TRUE) {
        printe("%s: of XMQuaternionIdentity() == %d ... TRUE\n",
            TestName, r);
        ret = MATH_FAIL;
    }
    return ret;
}
HRESULT Test147(LogProxy* pLog)
{
//XMQuaternionIsInfinite 
    HRESULT ret = S_OK;
    static const float f[] = {_INF, -_INF, _Q_NAN, 1,-1};
    int r,g,b,a;
    XMVECTOR col;
    BOOL res, check;
    for(r = 0; r < countof(f); r++) {
        for(g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for(b = 0; b < countof(f); b++) {
                for(a = 0; a < countof(f); a++) {
                    col = XMVectorSet(f[r],f[g],f[b],f[a]);
                    check = (r < 2 || g < 2 || b < 2 || a < 2) ? TRUE : FALSE;
                    res = XMQuaternionIsInfinite(col);
                    if(res != check) {
                        printe("\n%s: %f %f %f %f = %d ... %d\n", 
                            TestName, XMVectorGetX(col),XMVectorGetY(col),XMVectorGetZ(col),XMVectorGetW(col), res, check);
                        ret = MATH_FAIL;
                    } else {
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
HRESULT Test148(LogProxy* pLog)
{
//XMQuaternionIsNaN 
    HRESULT ret = S_OK;
    static const float f[] = {_Q_NAN, _NAN, _INF, 1,-1};
    int r,g,b,a;
    XMVECTOR col;
    BOOL res, check;
    for(r = 0; r < countof(f); r++) {
        for(g = 0; g < countof(f); g++) {
            printi("\n%s: ", TestName);
            for(b = 0; b < countof(f); b++) {
                for(a = 0; a < countof(f); a++) {
                    col= XMVectorSet(f[r],f[g],f[b],f[a]);
                    check = (r < 2 || g < 2 || b < 2 || a < 2) ? TRUE : FALSE;
                    res = XMQuaternionIsNaN(col);
                    if(res != check) {
                        printe("\n%s: %f %f %f %f = %d ... %d\n", 
                            TestName, XMVectorGetX(col),XMVectorGetY(col),XMVectorGetZ(col),XMVectorGetW(col), res, check);
                        ret = MATH_FAIL;
                    } else {
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

HRESULT Test149(LogProxy* pLog)
{
//XMQuaternionLength 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMQuaternionLength(XMVectorZero());
    c = CompareXMVECTOR(r,XMVectorZero(),4);
    if(c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r));
        ret = MATH_FAIL;
    } else {
        printi("%s: 0 = 0\n", TestName);
    }
    for(k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate(sqrtf(XMVectorGetX(v1)*XMVectorGetX(v1) + XMVectorGetY(v1)*XMVectorGetY(v1) + XMVectorGetZ(v1)*XMVectorGetZ(v1) + XMVectorGetW(v1)*XMVectorGetW(v1)));
        r = XMQuaternionLength(v1);
        c = CompareXMVECTOR(r,check,4);
        if(c > WITHIN4096) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName,
                XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName,c);
        }
    }
    return ret;
}
HRESULT Test150(LogProxy* pLog)
{
//XMQuaternionLengthSq 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR r = XMQuaternionLengthSq(XMVectorZero());
    c = CompareXMVECTOR(r,XMVectorZero(),4);
    if(c > EXACT) {
        printe("%s: 0: %f %f %f %f ... 0\n",
            TestName, XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r));
        ret = MATH_FAIL;
    } else {
        printi("%s: 0 = 0\n", TestName);
    }
    for(k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate((XMVectorGetX(v1)*XMVectorGetX(v1) + XMVectorGetY(v1)*XMVectorGetY(v1) + XMVectorGetZ(v1)*XMVectorGetZ(v1) + XMVectorGetW(v1)*XMVectorGetW(v1)));
        r = XMQuaternionLengthSq(v1);
        c = CompareXMVECTOR(r,check,4);
        if(c > WITHIN4096) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName,c);
        }
    }
    return ret;
}

HRESULT Test151(LogProxy* pLog)
{
//XMQuaternionLn 
    COMPARISON c;
    HRESULT ret = S_OK;

    XMVECTORF32 q1[15] = {
        {{23,48,-125,2}}, {{15,19,23, 1}}, {{-321,432,21,-1}},
        {{.8f, .4f, sqrtf(.2f),0}},
    };
    XMVECTOR r, check;

    for(int k = 0; k < countof(q1); k++) {
        if(k > 3) {
            for(int i = 0; i < 4; i++) {
                q1[k].v = XMVectorSetByIndex(q1[k],((float)rand()) / 2000.f - 8.f,i);
            }
            q1[k].v = XMQuaternionNormalize(q1[k]);
        }

        r = XMQuaternionLn(q1[k]);
        check = ScalarQuatLn(q1[k]);
        c = CompareXMVECTOR(r,check,4);
        if((c > WITHINBIGEPSILON)) {
            printe("%s: (%d)\n", TestName, c);
            dqe(q1[k]);dqe(r); dqe(check);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName,c);
        }
    }

    return ret;
}
HRESULT Test152(LogProxy* pLog)
{
//XMQuaternionMultiply 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1={}, v2={};
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 4; j++) {
            v1.v = XMVectorSetByIndex(v1,(float)rand() / 1000.f,j);
            v2.v = XMVectorSetByIndex(v2,(float)rand() / 1000.f,j);
        }
        FLOAT checkx = XMVectorGetW(v2) * XMVectorGetX(v1) + XMVectorGetX(v2) * XMVectorGetW(v1) + XMVectorGetY(v2) * XMVectorGetZ(v1) - XMVectorGetZ(v2) * XMVectorGetY(v1);
        FLOAT checky = XMVectorGetW(v2) * XMVectorGetY(v1) - XMVectorGetX(v2) * XMVectorGetZ(v1) + XMVectorGetY(v2) * XMVectorGetW(v1) + XMVectorGetZ(v2) * XMVectorGetX(v1);
        FLOAT checkz = XMVectorGetW(v2) * XMVectorGetZ(v1) + XMVectorGetX(v2) * XMVectorGetY(v1) - XMVectorGetY(v2) * XMVectorGetX(v1) + XMVectorGetZ(v2) * XMVectorGetW(v1);
        FLOAT checkw = XMVectorGetW(v2) * XMVectorGetW(v1) - XMVectorGetX(v2) * XMVectorGetX(v1) - XMVectorGetY(v2) * XMVectorGetY(v1) - XMVectorGetZ(v2) * XMVectorGetZ(v1);
        XMVECTOR check = XMVectorSet(checkx,checky,checkz,checkw);
        XMVECTOR r = XMQuaternionMultiply(v1, v2);
        c = CompareXMVECTOR(r,check,4);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                XMVectorGetX(v2),XMVectorGetY(v2),XMVectorGetZ(v2),XMVectorGetW(v2),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test153(LogProxy* pLog)
{
//XMQuaternionNormalize and XMQuaternionNormalizeEst
    HRESULT ret = S_OK;

    //XMQuaternionNormalize
    {
        int i,k;
        COMPARISON c;
        XMVECTORF32 check={};
        float rcplength;

        for(k = 0; k < 10; k++) {
            XMVECTOR v1 = GetRandomVector16();
            rcplength = 1.f/sqrtf(XMVectorGetX(v1)*XMVectorGetX(v1) + XMVectorGetY(v1)*XMVectorGetY(v1) + XMVectorGetZ(v1)*XMVectorGetZ(v1) + XMVectorGetW(v1)*XMVectorGetW(v1));
            for(i = 0; i < 4; i++) {
                check.v = XMVectorSetByIndex(check,XMVectorGetByIndex(v1,i) * rcplength,i);
            }

            XMVECTOR r = XMQuaternionNormalize(v1);
            c = CompareXMVECTOR(r,check,4);
            if(c > WITHIN4096) {
                printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName,
                    XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                    XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                    XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName,c);
            }
        }
    }    

    //XMQuaternionNormalizeEst
    {
        int i,k;
        COMPARISON c;
        XMVECTORF32 check={};
        float rcplength;

        for(k = 0; k < 10; k++) {
            XMVECTOR v1 = GetRandomVector16();
            rcplength = 1.f/sqrtf(XMVectorGetX(v1)*XMVectorGetX(v1) + XMVectorGetY(v1)*XMVectorGetY(v1) + XMVectorGetZ(v1)*XMVectorGetZ(v1) + XMVectorGetW(v1)*XMVectorGetW(v1));
            for(i = 0; i < 4; i++) {
                check.v = XMVectorSetByIndex(check,XMVectorGetByIndex(v1,i) * rcplength,i);
            }

            XMVECTOR r = XMQuaternionNormalizeEst(v1);
            c = CompareXMVECTOR(r,check,4);
            if(c > WITHINHUGEEPSILON) {
                printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName,
                    XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                    XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                    XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName,c);
            }
        }
    }    


    return ret;
}
HRESULT Test154(LogProxy* pLog)
{
//XMQuaternionNotEqual 
    XMVECTORF32 v1={}, v2={};
    BOOL r, check;
    int i,j;
    HRESULT ret = S_OK;
    for(j = 0; j < 16; j++) {
        for(i = 0; i < 4; i++) {
            float ftemp = (float)rand();
            v1.v = XMVectorSetByIndex(v1,ftemp,i);
            v2.v = XMVectorSetByIndex(v2,ftemp,i);
        }
        if(j & 8) { v1.v = XMVectorSetX(v1,XMVectorGetX(v2)+.01f);  } else {}
        if(j & 4) { v1.v = XMVectorSetY(v1,XMVectorGetY(v2)+.01f);  } else {}
        if(j & 2) { v1.v = XMVectorSetZ(v1,XMVectorGetZ(v2)+.01f);  } else {}
        if(j & 1) { v1.v = XMVectorSetW(v1,XMVectorGetW(v2)+.01f);  } else {}
        r = XMQuaternionNotEqual(v1,v2);
        check = (j) ? TRUE : FALSE;
        if(r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n",TestName,j,i,r,check);
            ret = MATH_FAIL;
        }

        for(i = 0; i < 4; i++) {
            float ftemp = (float)rand();
            v1.v = XMVectorSetByIndex(v1,ftemp,i);
            v2.v = XMVectorSetByIndex(v2,ftemp,i);
        }
        if(j & 8) { v1.v = XMVectorSetX(v1,XMVectorGetX(v2)-.01f); } else {}
        if(j & 4) { v1.v = XMVectorSetY(v1,XMVectorGetY(v2)-.01f); } else {}
        if(j & 2) { v1.v = XMVectorSetZ(v1,XMVectorGetZ(v2)-.01f); } else {}
        if(j & 1) { v1.v = XMVectorSetW(v1,XMVectorGetW(v2)-.01f); } else {}
        check = (j) ? TRUE : FALSE;
        r = XMQuaternionNotEqual(v1,v2);
        if(r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n",TestName,j,i,r,check);
            ret = MATH_FAIL;
        }
    }
    return ret;	
}
HRESULT Test155(LogProxy* pLog)
{
//XMQuaternionReciprocalLength 
    int k;
    HRESULT ret = S_OK;
    COMPARISON c;

    for(k = 0; k < 10; k++) {
        XMVECTOR v1 = GetRandomVector16();
        XMVECTOR check = XMVectorReplicate(1.f/sqrtf(XMVectorGetX(v1)*XMVectorGetX(v1) + XMVectorGetY(v1)*XMVectorGetY(v1) + XMVectorGetZ(v1)*XMVectorGetZ(v1) + XMVectorGetW(v1)*XMVectorGetW(v1)));
        XMVECTOR r = XMQuaternionReciprocalLength(v1);
        c = CompareXMVECTOR(r,check,4);
        if(c > WITHIN4096) {
            printe("%s: %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName,c);
        }
    }
    return ret;
}
HRESULT Test156(LogProxy* pLog)
{
//XMQuaternionRotationAxis 
    HRESULT ret = S_OK;
    XMVECTOR m,check;
    XMVECTOR v;
    float angle;
    COMPARISON c;

    for(int k = 0; k < 10; k++) {
        v = XMVectorSet(((float)rand()) / 4000.f - 4.f,((float)rand()) / 4000.f - 4.f,((float)rand()) / 4000.f - 4.f,(float)rand());
        angle = ((float)rand()) / 2000.f - 8.f;
        XMVECTOR v1 = v * (sinf(angle/2.f) / sqrtf(XMVectorGetX(v)*XMVectorGetX(v)+XMVectorGetY(v)*XMVectorGetY(v)+XMVectorGetZ(v)*XMVectorGetZ(v)));
        check = XMVectorSet(XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),cosf(angle/2.f));
        m = XMQuaternionRotationAxis(v,angle);
        c = CompareXMVECTOR(m,check,4);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                angle,
                XMVectorGetX(m),XMVectorGetY(m),XMVectorGetZ(m),XMVectorGetW(m),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test157(LogProxy* pLog)
{
//XMQuaternionRotationMatrix 
    COMPARISON c;
    HRESULT ret = S_OK;
    XMVECTORF32 q2,oq={};
    for(int k = 0; k < 10000; k++) {
        for(int i = 0; i < 4; i++) {
            oq.v = XMVectorSetByIndex(oq,((float)rand()) / 2000.f - 8.f,i);
        }
        oq.v = XMQuaternionNormalize(oq);
        XMMATRIX m = XMMatrixRotationQuaternion(oq);
        q2.v = XMQuaternionRotationMatrix(m);
        c = CompareXMVECTOR(q2, oq, 4);
        if(c > WITHIN4096) {
            COMPARISON c2 = CompareXMVECTOR(-q2, oq, 4);
            if(c2 > WITHIN100EPSILON) {
                printe("%s: %f %f %f %f -> %f %f %f %f (%d)\n",
                    TestName, XMVectorGetX(oq),XMVectorGetY(oq),XMVectorGetZ(oq),XMVectorGetW(oq),
                    XMVectorGetX(q2),XMVectorGetY(q2),XMVectorGetZ(q2),XMVectorGetW(q2),std::min(c,c2));
                ret = MATH_FAIL;
            }
        }
    }
    return ret;
}
HRESULT Test158(LogProxy* pLog)
{
//XMQuaternionRotationNormal 
    HRESULT ret = S_OK;
    XMVECTOR m,check;
    XMVECTOR v;
    float angle;
    COMPARISON c;

    for(int k = 0; k < 10; k++) {
        v = XMVectorSet(((float)rand()) / 4000.f - 4.f,((float)rand()) / 4000.f - 4.f,((float)rand()) / 4000.f - 4.f,(float)rand());
        v *= 1.f/sqrtf(XMVectorGetX(v)*XMVectorGetX(v)+XMVectorGetY(v)*XMVectorGetY(v)+XMVectorGetZ(v)*XMVectorGetZ(v));
        angle = ((float)rand()) / 2000.f - 8.f;
        XMVECTOR v1 = v * sinf(angle/2.f);
        check = XMVectorSet(XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),cosf(angle/2.f));
        m = XMQuaternionRotationAxis(v,angle);
        c = CompareXMVECTOR(m,check,4);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f  %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                angle,
                XMVectorGetX(m),XMVectorGetY(m),XMVectorGetZ(m),XMVectorGetW(m),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test159(LogProxy* pLog)
{
//XMQuaternionRotationRollPitchYaw 
    HRESULT ret = S_OK;
    XMVECTOR m, check;
    for(int k = 0; k < 15; k++) {
        float y,p,r;
        y = ((float)rand()) / 4000.f;
        p = ((float)rand()) / 4000.f;
        r = ((float)rand()) / 4000.f;
        float cy, sy, cp, sp, cr, sr;
        sy = sinf(y); cy = cosf(y);
        sp = sinf(p); cp = cosf(p);
        sr = sinf(r); cr = cosf(r);

        XMVECTORF32 checkx = {{cosf(p/2.f),0,0,sinf(p/2.f)}};
        XMVECTORF32 checky = {{0,cosf(y/2.f),0,-sinf(y/2.f)}};
        XMVECTORF32 checkz = {{0,0,cosf(r/2.f),-sinf(r/2.f)}};

        check = XMQuaternionMultiply(XMQuaternionMultiply(checkz,checkx),checky);

        m = XMQuaternionRotationRollPitchYaw(p,y,r);
        
        COMPARISON c = CompareXMVECTOR(m,check,4);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f (%d)\n", TestName, p,y,r, c);
            dqe(m); printe("...\n"); dqe(check);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test160(LogProxy* pLog)
{
//XMQuaternionRotationRollPitchYawFromVector 
    HRESULT ret = S_OK;
    XMVECTOR m, check;
    for(int k = 0; k < 15; k++) {
        float y,p,r;
        y = ((float)rand()) / 4000.f;
        p = ((float)rand()) / 4000.f;
        r = ((float)rand()) / 4000.f;
        float cy, sy, cp, sp, cr, sr;
        sy = sinf(y); cy = cosf(y);
        sp = sinf(p); cp = cosf(p);
        sr = sinf(r); cr = cosf(r);

        XMVECTORF32 checkx = {{cosf(p/2.f),0,0,sinf(p/2.f)}};
        XMVECTORF32 checky = {{0,cosf(y/2.f),0,-sinf(y/2.f)}};
        XMVECTORF32 checkz = {{0,0,cosf(r/2.f),-sinf(r/2.f)}};

        check = XMQuaternionMultiply(XMQuaternionMultiply(checkz,checkx),checky);

        m = XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(p,y,r,_Q_NAN));
        
        COMPARISON c = CompareXMVECTOR(m,check,4);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f (%d)\n", TestName, p,y,r, c);
            dqe(m); printe("...\n"); dqe(check);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}


HRESULT Test161(LogProxy* pLog)
{
//XMQuaternionSlerp and XMQuaternionSlerpV
    HRESULT ret = S_OK;

    //XMQuaternionSlerp
    {
        COMPARISON c,c2;
        XMVECTORF32 q1[15] = {
            {{sqrtf(.5f),0,0,sqrtf(.5f)}}, {{sqrtf(.5f),0,0,sqrtf(.5f)}},{{sqrtf(.5f),0,0,sqrtf(.5f)}},
        };
        XMVECTORF32 q2[15] = {
             {{0,1,0,0}}, {{0,1,0,0}}, {{0,1,0,0}},
        };
        float t[15] = {
            0, 1, .5f
        };
        XMVECTOR r, check;

        for(int k = 0; k < countof(q1); k++) {
            if(k > 2) {
                for(int i = 0; i < 4; i++) {
                    q1[k].v = XMVectorSetByIndex(q1[k],((float)rand()) / 2000.f - 8.f,i);
                    q2[k].v = XMVectorSetByIndex(q2[k],((float)rand()) / 2000.f - 8.f,i);
                }
                q1[k].v = XMQuaternionNormalize(q1[k]);
                q2[k].v = XMQuaternionNormalize(q2[k]);
                t[k] = ((float)rand()) / 2000.f - 8.f;
            }

            r = XMQuaternionSlerp(q1[k], q2[k], t[k]);
            check = ScalarQuatSlerp(q1[k], q2[k], t[k]);
            c = CompareXMVECTOR(r,check,4);
            c2 = CompareXMVECTOR(r,-check,4);
            if((c > WITHINBIGEPSILON) && (c2 > WITHINBIGEPSILON)) {
                printe("%s: t: %f. (%d)\n", TestName, t[k],std::min(c,c2));
                dqe(q1[k]); dqe(q2[k]); dqe(r); dqe(check);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName,std::min(c,c2));
            }
        }
    }

    //XMQuaternionSlerpV
    {
        COMPARISON c,c2;
        XMVECTORF32 q1[15] = {
            {{sqrtf(.5f),0,0,sqrtf(.5f)}}, {{sqrtf(.5f),0,0,sqrtf(.5f)}},{{sqrtf(.5f),0,0,sqrtf(.5f)}},
        };
        XMVECTORF32 q2[15] = {
             {{0,1,0,0}}, {{0,1,0,0}}, {{0,1,0,0}},
        };
        XMVECTORF32 T[15] = {
            {0,0,0,0}, {1,1,1,1}, {.5f,.5f,.5f,.5f}
        };
        XMVECTOR r, check;

        for(int k = 0; k < countof(q1); k++) {
            if(k > 2) {
                float f = ((float)rand()) / 2000.f - 8.f;
                for(int i = 0; i < 4; i++) {
                    q1[k].v = XMVectorSetByIndex(q1[k],((float)rand()) / 2000.f - 8.f,i);
                    q2[k].v = XMVectorSetByIndex(q2[k],((float)rand()) / 2000.f - 8.f,i);
                    T[k].v = XMVectorSetByIndex(T[k],f, i);
                }
                q1[k].v = XMQuaternionNormalize(q1[k]);
                q2[k].v = XMQuaternionNormalize(q2[k]);
            }

            r = XMQuaternionSlerpV(q1[k], q2[k], T[k]);
            check = ScalarQuatSlerp(q1[k], q2[k], XMVectorGetX(T[k]));
            c = CompareXMVECTOR(r,check,4);
            c2 = CompareXMVECTOR(r,-check,4);
            if((c > WITHINBIGEPSILON) && (c2 > WITHINBIGEPSILON)) {
                printe("%s: t: %f. (%d)\n", TestName, XMVectorGetX(T[k]),std::min(c,c2));
                dqe(q1[k]); dqe(q2[k]); dqe(r); dqe(check);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName,std::min(c,c2));
            }
        }
    }

    return ret;
}
HRESULT Test162(LogProxy* pLog)
{
//XMQuaternionSquad and XMQuaternionSquadV
    HRESULT ret = S_OK;

    //XMQuaternionSquad
    {
        COMPARISON c;

        XMVECTORF32 q1={}, q2={}, q3={}, q4={};
        float t;

        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < 4; i++) {
                q1.v = XMVectorSetByIndex(q1,((float)rand()) / 2000.f - 8.f,i);
                q2.v = XMVectorSetByIndex(q2,((float)rand()) / 2000.f - 8.f,i);
                q3.v = XMVectorSetByIndex(q3,((float)rand()) / 2000.f - 8.f,i);
                q4.v = XMVectorSetByIndex(q4,((float)rand()) / 2000.f - 8.f,i);
            }
            t = ((float)rand()) / 2000.f - 8.f;
            q1.v = XMQuaternionNormalize(q1);
            q2.v = XMQuaternionNormalize(q2);
            q3.v = XMQuaternionNormalize(q3);
            q4.v = XMQuaternionNormalize(q4);

            XMVECTOR r = XMQuaternionSquad(q1,q2,q3,q4,t);
            XMVECTOR check = XMQuaternionSlerp(XMQuaternionSlerp(q1,q4,t), XMQuaternionSlerp(q2,q3,t),2*t*(1-t));

            c = CompareXMVECTOR(r,check,4);
            if((c > WITHINBIGEPSILON)) {
                printe("%s: (%d)\n", TestName, c);
                dqe(q1);dqe(q2);dqe(q3);dqe(q4); dqe(r); dqe(check);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName,c);
            }
        }
    }

    //XMQuaternionSquadV
    {
        COMPARISON c;

        XMVECTORF32 q1={}, q2={}, q3={}, q4={};
        XMVECTORF32 T = {};

        for(int k = 0; k < 15; k++) {
            float t = ((float)rand()) / 2000.f - 8.f;
            for(int i = 0; i < 4; i++) {
                q1.v = XMVectorSetByIndex(q1,((float)rand()) / 2000.f - 8.f,i);
                q2.v = XMVectorSetByIndex(q2,((float)rand()) / 2000.f - 8.f,i);
                q3.v = XMVectorSetByIndex(q3,((float)rand()) / 2000.f - 8.f,i);
                q4.v = XMVectorSetByIndex(q4,((float)rand()) / 2000.f - 8.f,i);
                T.v = XMVectorSetByIndex(T,t,i);                
            }
            
            q1.v = XMQuaternionNormalize(q1);
            q2.v = XMQuaternionNormalize(q2);
            q3.v = XMQuaternionNormalize(q3);
            q4.v = XMQuaternionNormalize(q4);

            XMVECTOR r = XMQuaternionSquadV(q1,q2,q3,q4,T);
            XMVECTOR check = XMQuaternionSlerp(XMQuaternionSlerp(q1,q4,t), XMQuaternionSlerp(q2,q3,t),2*t*(1-t));

            c = CompareXMVECTOR(r,check,4);
            if((c > WITHINBIGEPSILON)) {
                printe("%s: (%d)\n", TestName, c);
                dqe(q1);dqe(q2);dqe(q3);dqe(q4); dqe(r); dqe(check);
                ret = MATH_FAIL;
            } else {
                printi("%s: %d\n", TestName,c);
            }
        }
    }    
    
    return ret;
}
HRESULT Test163(LogProxy* pLog)
{
//XMQuaternionSquadSetup 
    XMVECTOR q1,q2,q3,q4;
    XMVECTOR t1,t2,t3,t4;
    XMVECTOR cha,chb,chc;
    XMVECTOR ra,rb,rc;
    int k;
    COMPARISON ca,cb,cc;
    HRESULT ret = S_OK;
    for(k = 0; k < 20; k++) {
        t1 = q1 = XMQuaternionNormalize(GetRandomVector16());
        t2 = q2 = XMQuaternionNormalize(GetRandomVector16());
        t3 = q3 = XMQuaternionNormalize(GetRandomVector16());
        t4 = q4 = XMQuaternionNormalize(GetRandomVector16());
        if(XMVectorGetX(XMQuaternionLengthSq(t1+t2))<XMVectorGetX(XMQuaternionLengthSq(t1-t2))) t1=-t1;
        if(XMVectorGetX(XMQuaternionLengthSq(t2+t3))<XMVectorGetX(XMQuaternionLengthSq(t2-t3))) t3=-t3;
        if(XMVectorGetX(XMQuaternionLengthSq(t3+t4))<XMVectorGetX(XMQuaternionLengthSq(t3-t4))) t4=-t4;

        cha=XMQuaternionMultiply(t2,ScalarQuatExp(-.25f*(ScalarQuatLn(XMQuaternionMultiply(XMQuaternionInverse(t2),t3))+ScalarQuatLn(XMQuaternionMultiply(XMQuaternionInverse(t2),t1)))));
        chb=XMQuaternionMultiply(t3,ScalarQuatExp(-.25f*(ScalarQuatLn(XMQuaternionMultiply(XMQuaternionInverse(t3),t4))+ScalarQuatLn(XMQuaternionMultiply(XMQuaternionInverse(t3),t2)))));
        chc=t3;

        XMQuaternionSquadSetup(&ra,&rb,&rc,q1,q2,q3,q4);

        ca = CompareXMVECTOR(ra, cha, 4);
        cb = CompareXMVECTOR(rb, chb, 4);
        cc = CompareXMVECTOR(rc, chc, 4);

        if(ca > WITHINBIGGEREPSILON || cb > WITHINBIGGEREPSILON || cc > WITHINBIGGEREPSILON) { 
            printe("%s: (%d, %d, %d)\n", TestName, ca,cb,cc);
            dqe(q1);dqe(q2);dqe(q3);dqe(q4);printe("=");dqe(ra);dqe(rb);dqe(rc);printe("...");dqe(cha);dqe(chb);dqe(chc);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d %d %d\n", TestName, ca,cb,cc);
        }
    }

    return ret;
}
HRESULT Test164(LogProxy* pLog)
{
//XMQuaternionToAxisAngle 
    COMPARISON c,c2;
    HRESULT ret = S_OK;
    bool bNegated = false;
    XMVECTORF32 q={};
    float angle, achk;
    for(int k = 0; k < 15; k++) {
        bNegated = false;
        for(int i = 0; i < 4; i++) {
            q.v = XMVectorSetByIndex(q,((float)rand()) / 2000.f - 8.f,i);
        }
        q.v = XMQuaternionNormalize(q);
        achk = 2*acosf(XMVectorGetW(q));
        XMVECTOR check = q;
        if(achk == 0) check = XMVectorZero();
        
        XMVECTOR r;
        XMQuaternionToAxisAngle(&r,&angle,q);

        c = CompareXMVECTOR(r,check,3);
        if(c > WITHINHUGEEPSILON) {
            c2 = CompareXMVECTOR(-r, check,3);
            if(c2 > WITHINHUGEEPSILON) {
                XMFLOAT4 qa, ra, checka;
                XMStoreFloat4(&qa, q);
                XMStoreFloat4(&ra, r);
                XMStoreFloat4(&checka, check);
                printe("%s: %f %f %f %f = %f %f %f %f, %f ... %f %f %f %f, %f (%d)\n",
                    TestName, qa.x,qa.y,qa.z,qa.w,
                    ra.x,ra.y,ra.z,ra.w,
                    angle,
                    checka.x,checka.y,checka.z,checka.w,achk,std::min(c,c2));
                ret = MATH_FAIL;
            } else {
                bNegated = true;
            }
        }
        c = Compare(angle * (bNegated?-1.f:1.f),achk);
        if(c > WITHINBIGGEREPSILON) {
            XMFLOAT4 qa, ra, checka;
            XMStoreFloat4(&qa, q);
            XMStoreFloat4(&ra, r);
            XMStoreFloat4(&checka, check);
            printe("%s: %f %f %f %f = %f %f %f %f, %f ... %f %f %f %f, %f (%d)\n",
                TestName,
                qa.x,qa.y,qa.z,qa.w,
                ra.x,ra.y,ra.z,ra.w,
                angle,
                checka.x,checka.y,checka.z,checka.w,achk,c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
