//-------------------------------------------------------------------------------------
// math3tests.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

#include <iterator>

using namespace DirectX;
using namespace DirectX::PackedVector;

const uint32_t g_dwPhysicalAttribs = 0;

const int g_iStartAlignments[3] = {4, 16, 128}; 	
    


HRESULT Test001(LogProxy* pLog)
{
// Init
    HRESULT ret = S_OK;
    XMVECTOR vv;

    XMVECTORF32 vfi = { 1.f, 2.f, 3.f, 4.f };
    XMVECTOR vf = vfi.v;

    XMVECTORU32 vii = { 10, 20, 30, 40 };
    XMVECTOR vi = vii.v;

    // XMVectorGetX/Y/Z/W
    if ( XMVectorGetX( vf ) != 1.f
         || XMVectorGetY( vf ) != 2.f
         || XMVectorGetZ( vf ) != 3.f
         || XMVectorGetW( vf ) != 4.f )
    {
        printe ("%s: XMVectorGetX/Y/Z/W failed!\n1.0 2.0 3.0 4.0 ... %f %f %f %f",
              TestName, XMVectorGetX( vf ), XMVectorGetY( vf ), XMVectorGetZ( vf ), XMVectorGetW( vf ) );
        ret = MATH_FAIL;
    }

    // XMVectorGetIntX/Y/Z/W
    if ( XMVectorGetIntX( vi ) != 10
         || XMVectorGetIntY( vi ) != 20
         || XMVectorGetIntZ( vi ) != 30
         || XMVectorGetIntW( vi ) != 40 )
    {
        printe ("%s: XMVectorGetIntX/Y/Z/W failed!\n10 20 30 40 ... %i %i %i %i",
              TestName, XMVectorGetIntX( vi ), XMVectorGetIntY( vi ), XMVectorGetIntZ( vi ), XMVectorGetIntW( vi ) );
        ret = MATH_FAIL;
    }

    // XMVectorGetByIndex
    if ( XMVectorGetByIndex( vf, 0 ) != 1.f
         || XMVectorGetByIndex( vf, 1 ) != 2.f
         || XMVectorGetByIndex( vf, 2 ) != 3.f
         || XMVectorGetByIndex( vf, 3 ) != 4.f )
    {
        printe ("%s: XMVectorGetByIndex failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorGetIntByIndex
    if ( XMVectorGetIntByIndex( vi, 0 ) != 10
         || XMVectorGetIntByIndex( vi, 1 ) != 20
         || XMVectorGetIntByIndex( vi, 2 ) != 30
         || XMVectorGetIntByIndex( vi, 3 ) != 40 )
    {
        printe ("%s: XMVectorGetIntByIndex failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetX
    vv = XMVectorSetX( vf, 100.f );
    if ( XMVectorGetX( vv ) != 100.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetX failed!\n", TestName );
        ret = MATH_FAIL;
    }
    
    // XMVectorSetY
    vv = XMVectorSetY( vf, 100.f );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 100.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetY failed!\n", TestName );
        ret = MATH_FAIL;
    }
    
    // XMVectorSetZ
    vv = XMVectorSetZ( vf, 100.f );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 100.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetZ failed!\n", TestName );
        ret = MATH_FAIL;
    }
    
    // XMVectorSetW
    vv = XMVectorSetW( vf, 100.f );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 100.f )
    {
        printe ("%s: XMVectorSetW failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntX
    vv = XMVectorSetIntX( vi, 1000 );
    if ( XMVectorGetIntX( vv ) != 1000
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntX failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntY
    vv = XMVectorSetIntY( vi, 1000 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 1000
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntY failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntZ
    vv = XMVectorSetIntZ( vi, 1000 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 1000
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntZ failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntW
    vv = XMVectorSetIntW( vi, 1000 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 1000 )
    {
        printe ("%s: XMVectorSetIntW failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetByIndex
    vv = XMVectorSetByIndex( vf, 100.f, 0 );
    if ( XMVectorGetX( vv ) != 100.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetByIndex(0) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetByIndex( vf, 100.f, 1 );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 100.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetByIndex(1) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetByIndex( vf, 100.f, 2 );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 100.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetByIndex(2) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetByIndex( vf, 100.f, 3 );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 100.f )
    {
        printe ("%s: XMVectorSetByIndex(3) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntByIndex
    vv = XMVectorSetIntByIndex( vi, 1000, 0 );
    if ( XMVectorGetIntX( vv ) != 1000
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntByIndex(0) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetIntByIndex( vi, 1000, 1 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 1000
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntByIndex(1) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetIntByIndex( vi, 1000, 2 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 1000
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntByIndex(2) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetIntByIndex( vi, 1000, 3 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 1000 )
    {
        printe ("%s: XMVectorSetIntByIndex(3) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    FLOAT fx, fy, fz, fw;

    // XMVectorGetX/Y/Z/WPtr
    XMVectorGetXPtr( &fx, vf );
    XMVectorGetYPtr( &fy, vf );
    XMVectorGetZPtr( &fz, vf );
    XMVectorGetWPtr( &fw, vf );
    if ( fx != 1.f || fy != 2.f || fz != 3.f || fw != 4.f )
    {
        printe ("%s: XMVectorGetX/Y/Z/WPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    uint32_t ix, iy, iz, iw;

    // XMVectorGetIntX/Y/Z/WPtr
    XMVectorGetIntXPtr( &ix, vi );
    XMVectorGetIntYPtr( &iy, vi );
    XMVectorGetIntZPtr( &iz, vi );
    XMVectorGetIntWPtr( &iw, vi );
    if ( ix != 10 || iy != 20 || iz != 30 || iw != 40 )
    {
        printe ("%s: XMVectorGetIntX/Y/Z/WPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorGetByIndexPtr
    XMVectorGetByIndexPtr( &fx, vf, 0 );
    XMVectorGetByIndexPtr( &fy, vf, 1 );
    XMVectorGetByIndexPtr( &fz, vf, 2 );
    XMVectorGetByIndexPtr( &fw, vf, 3 );
    if ( fx != 1.f || fy != 2.f || fz != 3.f || fw != 4.f )
    {
        printe ("%s: XMVectorGetByIndexPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorGetIntByIndexPtr
    XMVectorGetIntByIndexPtr( &ix, vi, 0 );
    XMVectorGetIntByIndexPtr( &iy, vi, 1 );
    XMVectorGetIntByIndexPtr( &iz, vi, 2 );
    XMVectorGetIntByIndexPtr( &iw, vi, 3 );
    if ( ix != 10 || iy != 20 || iz != 30 || iw != 40 )
    {
        printe ("%s: XMVectorGetIntByIndexPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetXPtr
    fx = 100.f;
    vv = XMVectorSetXPtr( vf, &fx );
    if ( XMVectorGetX( vv ) != 100.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetXPtr failed!\n100.0 2.0 3.0 4.0 ... %f %f %f %f\n",
               TestName, XMVectorGetX( vv ), XMVectorGetY( vv ), XMVectorGetZ( vv ), XMVectorGetW( vv ) );
        ret = MATH_FAIL;
    }

    // XMVectorSetYPtr
    fy = 100.f;
    vv = XMVectorSetYPtr( vf, &fy );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 100.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetYPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetZPtr
    fz = 100.f;
    vv = XMVectorSetZPtr( vf, &fz );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 100.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetZPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetWPtr
    fw = 100.f;
    vv = XMVectorSetWPtr( vf, &fw );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 100.f )
    {
        printe ("%s: XMVectorSetWPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntXPtr
    ix = 1000;
    vv = XMVectorSetIntXPtr( vi, &ix );
    if ( XMVectorGetIntX( vv ) != 1000
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntXPtr failed!\n1000 20 30 40 ... %i %i %i %i\n",
                TestName, XMVectorGetIntX( vv ), XMVectorGetIntY( vv ), XMVectorGetIntZ( vv ), XMVectorGetIntW( vv ) );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntYPtr
    iy = 1000;
    vv = XMVectorSetIntYPtr( vi, &iy );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 1000
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntYPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntZPtr
    iz = 1000;
    vv = XMVectorSetIntZPtr( vi, &iz );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 1000
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntZPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntWPtr
    iw = 1000;
    vv = XMVectorSetIntWPtr( vi, &iw );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 1000 )
    {
        printe ("%s: XMVectorSetIntWPtr failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetByIndexPtr
    fx = 100.f;
    vv = XMVectorSetByIndexPtr( vf, &fx, 0 );
    if ( XMVectorGetX( vv ) != 100.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetByIndexPtr(0) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetByIndexPtr( vf, &fx, 1 );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 100.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetByIndexPtr(1) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetByIndexPtr( vf, &fx, 2 );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 100.f
         || XMVectorGetW( vv ) != 4.f )
    {
        printe ("%s: XMVectorSetByIndexPtr(2) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetByIndexPtr( vf, &fx, 3 );
    if ( XMVectorGetX( vv ) != 1.f
         || XMVectorGetY( vv ) != 2.f
         || XMVectorGetZ( vv ) != 3.f
         || XMVectorGetW( vv ) != 100.f )
    {
        printe ("%s: XMVectorSetByIndexPtr(3) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    // XMVectorSetIntByIndexPtr
    ix = 1000;
    vv = XMVectorSetIntByIndexPtr( vi, &ix, 0 );
    if ( XMVectorGetIntX( vv ) != 1000
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntByIndexPtr(0) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetIntByIndexPtr( vi, &ix, 1 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 1000
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntByIndexPtr(1) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetIntByIndexPtr( vi, &ix, 2 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 1000
         || XMVectorGetIntW( vv ) != 40 )
    {
        printe ("%s: XMVectorSetIntByIndexPtr(2) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    vv = XMVectorSetIntByIndexPtr( vi, &ix, 3 );
    if ( XMVectorGetIntX( vv ) != 10
         || XMVectorGetIntY( vv ) != 20
         || XMVectorGetIntZ( vv ) != 30
         || XMVectorGetIntW( vv ) != 1000 )
    {
        printe ("%s: XMVectorSetIntByIndexPtr(3) failed!\n", TestName );
        ret = MATH_FAIL;
    }

    return ret;
}

HRESULT Test322(LogProxy* pLog)
{
    HRESULT ret = S_OK;

// XMConvertToRadians
    float r = XMConvertToRadians(180.f);
    COMPARISON c = Compare(r, XM_PI);
    if (c > WITHINEPSILON)
    {
        printe("%s: XMConvertToRadians failed!\n", TestName);
        ret = MATH_FAIL;
    }

    for (float f = 0.f; f <= 360.f; f += 0.1f)
    {
        float chk = f * XM_PI / 180.f;
        r = XMConvertToRadians(f);
        c = Compare(r, chk);
        if (c > WITHIN4096)
        {
            printe("%s: XMConvertToRadians(%f) %f...%f failed!\n", TestName, f, r, chk);
            ret = MATH_FAIL;
        }
    }

// XMConvertToDegrees
    r = XMConvertToDegrees(XM_PI);
    c = Compare(r, 180.0f);
    if (c > WITHINEPSILON)
    {
        printe("%s: XMConvertToDegrees failed!\n", TestName);
        ret = MATH_FAIL;
    }

    for (float f = 0.f; f <= (XM_PI*4.f); f += 0.1f)
    {
        r = XMConvertToDegrees(f);
        float chk = f * 180.f / XM_PI;
        c = Compare(r, chk);
        if (c > WITHIN4096)
        {
            printe("%s: XMConvertToDegrees(%f) %f...%f failed!\n", TestName, f, r, chk);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test061(LogProxy* pLog)
{
//XMConvertFloatToHalf 
    HRESULT ret = S_OK;
    static const float fa[] = {0,1,2,3,65504,-65504,_INF, -_INF, 131008, -131008, _Q_NAN};
#if defined(_XENON) && !defined(_XM_NO_INTRINSICS_)
    static const HALF checka[] = {0,0x3c00,0x4000,0x4200,0x7BFF,0xFBFF,0x7fff,0xffff,0x7fff,0xFFFF,0x7FFF};
#else
    static const HALF checka[] = {0,0x3c00,0x4000,0x4200,0x7BFF,0xFBFF,0x7C00,0xFC00,0x7C00,0xFC00,0x7FFF};
#endif
    COMPARISON c;
    HALF r;

    for(int k = 0; k < countof(fa); k++) {
        r = XMConvertFloatToHalf(fa[k]);
        c = CompareHALF(r,checka[k]);
        if(c != EXACT) {
            printe("%s: %f = 0x%x ... 0x%x (%d)\n", TestName, fa[k],r,checka[k],c);
            ret = MATH_FAIL;
        }
    }

    static const uint32_t fb[] = {0x47000000, 0x477fe000, 0x38800000, 0x38ffe000,0xb8000000,0x37800000};


    //denormalized 16-bit floats are truncated to +/- 0 on Xenon.  Hence the difference
    //  between the expected results on platforms.
    #if defined(_XENON) && !defined(_XM_NO_INTRINSICS_)
        static const HALF checkb[] = {0x7800, 0x7bff, 0x0400, 0x07ff, 0x8000, 0x0};	
    //denormalized 16-bit floats are truncated to +/- 0.
    #else
        static const HALF checkb[] = {0x7800, 0x7bff, 0x0400, 0x07ff, 0x8200, 0x100};	
    #endif
    for(int k = 0; k < countof(fb); k++) {
        float b = *(const float*)&fb[k];
        r = XMConvertFloatToHalf(b);
        c = CompareHALF(r,checkb[k]);
        if(c != EXACT) {
            printe("%s: %f = 0x%x ... 0x%x (%d)\n", TestName, b,r,checkb[k],c);
            ret = MATH_FAIL;
        }
    }

    return ret;
}
HRESULT Test062(LogProxy* pLog)
{
//XMConvertFloatToHalfStream 
    const uint32_t dwDataSize = 5000;

    static const int ins[] =  {4, 4,  8, 20, 12};
    static const int outs[] = {2, 10, 2,  4,  6};
    const HALF *p;
    static const float fa[]    = {0,1,2,3,_Q_NAN};
    static const HALF checka[] = {0,0x3c00, 0x4000, 0x4200, 0x7fff};
    HRESULT ret = S_OK;
    int offset = 16;
    
    uint8_t *pbIn          = nullptr;
    uint8_t *pbOut         = nullptr;
    uint8_t *pbChk         = nullptr;
    uint8_t *pbInReal      = nullptr;
    uint8_t *pbOutReal     = nullptr;
    uint8_t *pbChkReal     = nullptr;
    int InAlignIndex    =0;
    int OutAlignIndex   = 0;
    BOOL OutWriteCombined = FALSE;

    //Chk is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbChkReal, (LPVOID*)&pbChk);
    if (!pbChk || !pbChkReal)
        goto Cleanup;

    for (OutWriteCombined=0; OutWriteCombined<2; OutWriteCombined++)
    {
        for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbInReal, (LPVOID*)&pbIn);
            if (!pbIn || !pbInReal)
                goto Cleanup;
            
            for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)	
            {
                AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbOutReal, (LPVOID*)&pbOut);
                if (!pbOutReal || !pbOut)
                    goto Cleanup;
    

                //For each input stride value (j)
                for(int j = 0; j < countof(ins); j++)
                {
                    uint32_t dwNumItems = (dwDataSize - offset) / std::max(ins[j], outs[j]) - 1;

                    int i;
                    for(i = 0; i < dwDataSize; i++) 
                    {
                        pbIn[i] = ~(uint8_t)(i & 0xff);
                        pbOut[i] = ~(uint8_t)(i & 0xff);
                        pbChk[i] = ~(uint8_t)(i & 0xff);
                    }

                    
                    for(i = 0; i < (int)dwNumItems; i++) 
                    {
                        int num = rand() % countof(checka);
                        *(float*)&( pbIn[offset + i*ins[j]])  = fa[num];
                        *(HALF*)&(pbChk[offset + i*outs[j]]) = checka[num];
                        
                    
                    }

                    p = XMConvertFloatToHalfStream((HALF*)&pbOut[offset], outs[j], (const float*)&pbIn[offset], ins[j], dwNumItems);
                    if(p != (const HALF*)&pbOut[offset]) {
                        printe("%s: returned %p, ... %p\n", TestName, p, &pbOut[offset]);
                        ret = MATH_FAIL;
                    }

                    for(i = 0; i < dwDataSize; i++) {
                        if(pbOut[i] != pbChk[i]) {
                            printe("%s: ins(%d), outs(%d), byte %d = %x ... %x\n",
                                TestName, ins[j], outs[j], i, pbOut[i], pbChk[i]);
                            ret = MATH_FAIL;
                        } 
                    }
                }
                FreeWithAlignment(pbOutReal, g_dwPhysicalAttribs);	        
                pbOut = pbOutReal = nullptr;
            }//end "for OutAlignIndex"
            FreeWithAlignment(pbInReal, g_dwPhysicalAttribs);
            pbIn = pbInReal = nullptr;
        }//end "for InAlignIndex"
    }//end "for OutWriteCombined"

Cleanup:
    FreeWithAlignment(pbOutReal, g_dwPhysicalAttribs);
    FreeWithAlignment(pbInReal, g_dwPhysicalAttribs);
    FreeWithAlignment(pbChkReal, g_dwPhysicalAttribs);


    return ret;
}

// Test063 - Removed because XMConvertVectorFloatToHalf is not in xboxmath or xnamath

HRESULT Test064(LogProxy* pLog)
{
//XMConvertHalfToFloat 
    HRESULT ret = S_OK;
#if defined(_XENON) && !defined(_XM_NO_INTRINSICS_)
    static const float fa[] = {0,1,2,3,65504, -65504, 65536, -65536, 131008, -131008};
#else
    static const float fa[] = {0,1,2,3,65504, -65504, _INF, -_INF, _Q_NAN, _Q_NAN};
#endif
    static const HALF checka[] = {0,0x3c00, 0x4000, 0x4200, 0x7BFF, 0xFBFF, 0x7C00, 0xFC00, 0x7fff, 0xffff};
    COMPARISON c;
    float r;

    for(int k = 0; k < countof(fa); k++) {
        r = XMConvertHalfToFloat(checka[k]);
        c = Compare(r,fa[k]);
        if(c != EXACT) {
            printe("%s: 0x%x = %f (0x%x)... %f (0x%x) (%d)\n", TestName, checka[k],r,*(const uint32_t*)&r,fa[k],*(const uint32_t*)&fa[k],c);
            ret = MATH_FAIL;
        } else {
            printi("%s: 0x%x = %f (0x%x)... %f (0x%x) (%d)\n", TestName, checka[k],r,*(const uint32_t*)&r,fa[k],*(const uint32_t*)&fa[k],c);
        }
    }

    static const uint32_t fb[] = {0x47000000, 0x477fe000, 0x38800000, 0x38ffe000,0x38800000,0x0};
    static const HALF checkb[] = {0x7800, 0x7bff, 0x0400, 0x07ff, 0x0400, 0x0};
    for(int k = 0; k < countof(fb); k++) {
        float b = *(const float*)&fb[k];
        r = XMConvertHalfToFloat(checkb[k]);
        c = Compare(r,b);
        if(c != EXACT) {
            printe("%s: 0x%x = %f (0x%x) ... %f (0x%x) (%d)\n", TestName, checkb[k],r,*(const uint32_t*)&r,b,fb[k],c);
            ret = MATH_FAIL;
        } else {
            printi("%s: 0x%x = %f (%x) ... %f (0x%x) (%d)\n", TestName, checkb[k],r,*(const uint32_t*)&r,b,fb[k],c);
        }
    }

    return ret;
}
HRESULT Test065(LogProxy* pLog)
{
    uint32_t dwDataSize = 5000;
//XMConvertHalfToFloatStream 
    static const int ins[] = {2, 2, 4,6,8,22}; 
    static const int outs[] = {4, 8, 16,8,4,12};
    float *p = nullptr;
    static const float fa[] = {0,1,2,3};
    static const HALF checka[] = {0,0x3c00, 0x4000, 0x4200};
    HRESULT ret = S_OK;
    int offset = 16;

    uint8_t *pbIn          = nullptr;
    uint8_t *pbOut         = nullptr;
    uint8_t *pbChk         = nullptr;
    uint8_t *pbInReal      = nullptr;
    uint8_t *pbOutReal     = nullptr;
    uint8_t *pbChkReal     = nullptr;
    int InAlignIndex    =0;
    int OutAlignIndex   = 0;
    BOOL OutWriteCombined = FALSE;

    //Chk is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbChkReal, (LPVOID*)&pbChk);
    if (!pbChk || !pbChkReal)
        goto Cleanup;

    for (OutWriteCombined=0; OutWriteCombined<2; OutWriteCombined++)
    {
        for (InAlignIndex = 0; InAlignIndex < countof(g_iStartAlignments); InAlignIndex++)
        {
            AllocWithAlignment(dwDataSize, g_iStartAlignments[InAlignIndex], FALSE, (LPVOID*)&pbInReal, (LPVOID*)&pbIn);
            if (!pbIn || !pbInReal)
                goto Cleanup;
            
            for (OutAlignIndex = 0; OutAlignIndex < countof(g_iStartAlignments); OutAlignIndex++)	
            {
                AllocWithAlignment(dwDataSize, g_iStartAlignments[OutAlignIndex], OutWriteCombined, (LPVOID*)&pbOutReal, (LPVOID*)&pbOut);
                if (!pbOutReal || !pbOut)
                    goto Cleanup;

                //For each input stride value (j)
                for(int j = 0; j < countof(ins); j++)
                {
                    uint32_t dwNumItems = (dwDataSize - offset) / std::max(ins[j], outs[j]) - 1;
                
                    //Wipe out everything.
                    int i;
                    for(i = 0; i < (int)dwDataSize; i++) {
                        pbIn[i] = ~(uint8_t)(i & 0xff);
                        pbOut[i] = ~(uint8_t)(i & 0xff);
                        pbChk[i] = ~(uint8_t)(i & 0xff);
                    }
                    

                    for(i = 0; i < (int)dwNumItems; i++) 
                    {
                        int num = rand() % countof(checka);		            
                        *(HALF*)&( pbIn[offset + i*ins[j]]) = checka[num];
                        *(float*)&(pbChk[offset + i*outs[j]]) = fa[num];
                    }

                    p = XMConvertHalfToFloatStream((float*)&pbOut[offset], outs[j], (const HALF*)&pbIn[offset], ins[j], dwNumItems);
                    if(p != (float*)&pbOut[offset]) {
                        printe("%s: returned %p, ... %p\n", TestName, p, &pbOut[offset]);
                        ret = MATH_FAIL;
                    }

                    for(i = 0; i < (int)dwNumItems; i++) {
                        if(pbOut[i] != pbChk[i]) {
                            printe("%s: ins(%d), outs(%d), byte %d = %x ... %x\n",
                                TestName, ins[j], outs[j], i, pbOut[i], pbChk[i]);
                            ret = MATH_FAIL;
                        } 
                    }
                }
                FreeWithAlignment(pbOutReal, g_dwPhysicalAttribs);	        
                pbOut = pbOutReal = nullptr;
            }//end "for OutAlignIndex"
            FreeWithAlignment(pbInReal, g_dwPhysicalAttribs);
            pbIn = pbInReal = nullptr;
        }//end "for InAlignIndex"
    }//end "for OutWriteCombined"

Cleanup:
    FreeWithAlignment(pbOutReal, g_dwPhysicalAttribs);
    FreeWithAlignment(pbInReal, g_dwPhysicalAttribs);
    FreeWithAlignment(pbChkReal, g_dwPhysicalAttribs);

    return ret;

}

// Test066 - Removed because XMConvertVectorHalfToFloat(v) is not in xboxmath or xnamath

HRESULT Test067(LogProxy* pLog)
{
//XMFresnelTerm 
    HRESULT ret = S_OK;
    XMVECTORF32 v1={}, v2={};
    XMVECTORF32 check={};
    for(int k = 0; k < 15; k++) {
        for(int i = 0; i < 4; i++) {
            v1.v = XMVectorSetByIndex(v1,GetRandomFloat(2.0f) - 1,i);
            v2.v = XMVectorSetByIndex(v2,GetRandomFloat(3.0f) + 1,i);
            float g,c,ri;
            c = XMVectorGetByIndex(v1,i);
            ri = XMVectorGetByIndex(v2,i);
            /*formula from d3dx source code*/
            g=sqrtf(c*c + ri*ri - 1);
            check.v = XMVectorSetByIndex(check,0.5f * ((g-c)*(g-c)) / ((g+c)*(g+c)) * (((c*(g+c)-1)*(c*(g+c)-1)) / ((c*(g-c)+1)*(c*(g-c)+1)) + 1),i);
            check.v = XMVectorSetByIndex(check,std::min(XMVectorGetByIndex(check,i), 1.f),i);
        }
        XMVECTOR r = XMFresnelTerm(v1,v2);
        COMPARISON c = CompareXMVECTOR(r,check,4);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(v1),XMVectorGetY(v1),XMVectorGetZ(v1),XMVectorGetW(v1),
                XMVectorGetX(v2),XMVectorGetY(v2),XMVectorGetZ(v2),XMVectorGetW(v2),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test068(LogProxy* pLog)
{
//XMLoadColor 
    static_assert( sizeof(XMCOLOR) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMCOLOR>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMCOLOR>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMCOLOR>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMCOLOR>::value, "Move Assign.");

    uint32_t u = 0;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR v = XMLoadColor((const XMCOLOR*)&u);
    if(!XMVector4Equal(v, XMVectorZero())) {
        printe("%s: load(zero) = %f %f %f %f ... 0,0,0,0\n",
            TestName,XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v));
        ret = MATH_FAIL;
    }
    u = 0xffffffff;
    v = XMLoadColor((const XMCOLOR*)&u);
    c = CompareXMVECTOR(v,g_XMOne,4);
    if (c > EXACT) {
        printe("%s: load(ffffffff) = %f %f %f %f ... 1,1,1,1 (%d)\n",
            TestName,XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),c);
        ret = MATH_FAIL;
    }

    XMVECTORF32 check = {{3.f/255.f, 1.f/255.f,8.f/255.f,5.f/255.f}};
    XMVECTOR chkadd = XMVectorReplicate(3.f/255.f);
    for(u = 0x05030108; (u&255) >= 8; u += 0x03030303) {
        v = XMLoadColor((const XMCOLOR*)&u);
        c = CompareXMVECTOR(v,check,4);
        if(c > WITHIN10EPSILON) {
            printe("%s: 0x%x = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, u, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check), c);
            ret= MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName,c);
        }
        check.v += chkadd;
    }
    return ret;
}
HRESULT Test069(LogProxy* pLog)
{
//XMLoadFloat2 / XMLoadInt2 / XMLoadSInt2 / XMLoadUInt2
    static_assert( sizeof(XMFLOAT2) == 8, "Unexpected structure size" );
    static_assert( sizeof(XMINT2) == 8, "Unexpected structure size" );
    static_assert( sizeof(XMUINT2) == 8, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_assignable<XMINT2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_assignable<XMUINT2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_copy_constructible<XMINT2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_copy_constructible<XMUINT2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMINT2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUINT2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT2>::value, "Move Assign.");
    static_assert(std::is_nothrow_move_assignable<XMINT2>::value, "Move Assign.");
    static_assert(std::is_nothrow_move_assignable<XMUINT2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    char c[64];
    int offset = 16;
    float f;
    int i, j; 
    HRESULT r = S_OK;
    const int floatcount = 2;

    for(j = 0; j < 16; j++) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }

        XMVECTOR v = XMLoadFloat2((const XMFLOAT2*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != i) {
                printe("%s: %d corrupted input data! %d: %f ... %f\n", TestName, j,i,f,(float)i);
                r=MATH_FAIL;
            }
            if(XMVectorGetByIndex(v,i) != i) {
                printe("%s: %d corrupted output float %d: %f ... %f\n", TestName, j,i,XMVectorGetByIndex(v,i),(float)i);
                r=MATH_FAIL;
            }
        }
    }

    const int intcount = 2;

    for(j = 0; j < 16; j++)
    {
        for(i = 0; i < sizeof(c); i++) { c[i] = (char)(~i & 0xff); }
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        for(i = 0; i < intcount; i++)
            WriteInt((uint32_t)i, (char*)&c[first + (i*4)]);

        XMVECTOR v = XMLoadInt2((const uint32_t*)&c[offset + j]);

        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        for(i = last; i < sizeof(c); i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++)
        {
            uint32_t x = ReadInt((char*)&(c[first+(i*4)]));
            if(x != (uint32_t)i)
            {
                printe("%s: %d corrupted input data! %d: %i ... %i\n", TestName, j,i,x,(uint32_t)i);
                r=MATH_FAIL;
            }

            if(XMVectorGetIntByIndex(v,i) != x)
            {
                printe("%s: %d corrupted output int %d: %i ... %i\n", TestName, j,i,XMVectorGetIntByIndex(v,i),x);
                r=MATH_FAIL;
            }
        }
    }

    int32_t sint[] = { 1, 1, 0, 0, -1, -1, 0x7FFFFFFF, int32_t(0xFFFFFFFF) };
    for( i=0; i < std::size(sint)/intcount; ++i )
    {
        XMVECTOR v = XMLoadSInt2( (const XMINT2*)&sint[i*intcount] );

        XMVECTORF32 check = { (float)sint[i*intcount], (float)sint[i*intcount+1], _Q_NAN, _Q_NAN };
        COMPARISON cmp = CompareXMVECTOR(v,check,2);
        if ( cmp > WITHIN10EPSILON )
        {
            printe("%s: SINT %d = %d %d = %f %f ... %f %f (%d)\n",
                   TestName, i, sint[i*intcount], sint[i*intcount+1],
                   XMVectorGetX(check), XMVectorGetY(check), XMVectorGetX(v), XMVectorGetY(v), cmp );
            r = MATH_FAIL;
        }
    }

    uint32_t uint[] = { 1, 1, 0, 0, 0x0340c0f0, 0x82e02304, 0x7FFFFFFF, 0xFFFFFFFF };
    for( i=0; i < std::size(uint)/intcount; ++i )
    {
        XMVECTOR v = XMLoadUInt2( (const XMUINT2*)&uint[i*intcount] );

        XMVECTORF32 check = { (float)uint[i*intcount], (float)uint[i*intcount+1], _Q_NAN, _Q_NAN };
        COMPARISON cmp = CompareXMVECTOR(v,check,2);
        if ( cmp > WITHIN10EPSILON )
        {
            printe("%s: uint32_t %d = %u %u = %f %f ... %f %f (%d)\n",
                   TestName, i, uint[i*intcount], uint[i*intcount+1], 
                   XMVectorGetX(check), XMVectorGetY(check), XMVectorGetX(v), XMVectorGetY(v), cmp );
            r = MATH_FAIL;
        }
    }

    return r;
}
HRESULT Test070(LogProxy* pLog)
{
//XMLoadFloat2A / XMLoadInt2A
    static_assert( sizeof(XMFLOAT2A) == 16, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT2A>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT2A>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT2A>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT2A>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XM_ALIGNED_DATA(16) char c[64];
    int offset = 16;
    float f;
    int i, j; 
    HRESULT r = S_OK;
    const int floatcount = 2;

    for(j = 0; j < 16; j+= 16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }


        XMVECTOR v = XMLoadFloat2A((const XMFLOAT2A*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != i) {
                printe("%s: %d corrupted input data! %d: %f ... %f\n", TestName, j,i,f,(float)i);
                r=MATH_FAIL;
            }
            if(XMVectorGetByIndex(v,i) != i) {
                printe("%s: %d corrupted output float %d: %f ... %f\n", TestName, j,i,XMVectorGetByIndex(v,i),(float)i);
                r=MATH_FAIL;
            }
        }
    }

    const int intcount = 2;

    for(j = 0; j < 16; j += 16)
    {
        for(i = 0; i < sizeof(c); i++) { c[i] = (char)(~i & 0xff); }
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        for(i = 0; i < intcount; i++)
            WriteInt((uint32_t)i, (char*)&c[first + (i*4)]);

        XMVECTOR v = XMLoadInt2A((const uint32_t*)&c[offset + j]);

        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        for(i = last; i < sizeof(c); i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++)
        {
            uint32_t x = ReadInt((char*)&(c[first+(i*4)]));
            if(x != (uint32_t)i)
            {
                printe("%s: %d corrupted input data! %d: %i ... %i\n", TestName, j,i,x,(uint32_t)i);
                r=MATH_FAIL;
            }

            if(XMVectorGetIntByIndex(v,i) != x)
            {
                printe("%s: %d corrupted output int %d: %i ... %i\n", TestName, j,i,XMVectorGetIntByIndex(v,i),x);
                r=MATH_FAIL;
            }
        }
    }

    return r;
}
HRESULT Test071(LogProxy* pLog)
{
//XMLoadFloat3 / XMLoadInt3 / XMLoadSInt3 / XMLoadUInt3
    static_assert( sizeof(XMFLOAT3) == 12, "Unexpected structure size" );
    static_assert( sizeof(XMINT3) == 12, "Unexpected structure size" );
    static_assert( sizeof(XMUINT3) == 12, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT3>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_assignable<XMINT3>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_assignable<XMUINT3>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT3>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_copy_constructible<XMINT3>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_copy_constructible<XMUINT3>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT3>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMINT3>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUINT3>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT3>::value, "Move Assign.");
    static_assert(std::is_nothrow_move_assignable<XMINT3>::value, "Move Assign.");
    static_assert(std::is_nothrow_move_assignable<XMUINT3>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    char c[64];
    int offset = 16;
    float f;
    int i, j; 
    HRESULT r = S_OK;
    const int floatcount = 3;

    for(j = 0; j < 16; j++) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }


        XMVECTOR v = XMLoadFloat3((const XMFLOAT3*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != i) {
                printe("%s: %d corrupted input data! %d: %f ... %f\n", TestName, j,i,f,(float)i);
                r=MATH_FAIL;
            }
            if(XMVectorGetByIndex(v,i) != i) {
                printe("%s: %d corrupted output float %d: %f ... %f\n", TestName, j,i,XMVectorGetByIndex(v,i),(float)i);
                r=MATH_FAIL;
            }
        }
    }

    const int intcount = 3;

    for(j = 0; j < 16; j++)
    {
        for(i = 0; i < sizeof(c); i++) { c[i] = (char)(~i & 0xff); }
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        for(i = 0; i < intcount; i++)
            WriteInt((uint32_t)i, (char*)&c[first + (i*4)]);

        XMVECTOR v = XMLoadInt3((const uint32_t*)&c[offset + j]);

        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        for(i = last; i < sizeof(c); i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++)
        {
            uint32_t x = ReadInt((char*)&(c[first+(i*4)]));
            if(x != (uint32_t)i)
            {
                printe("%s: %d corrupted input data! %d: %i ... %i\n", TestName, j,i,x,(uint32_t)i);
                r=MATH_FAIL;
            }

            if(XMVectorGetIntByIndex(v,i) != x)
            {
                printe("%s: %d corrupted output int %d: %i ... %i\n", TestName, j,i,XMVectorGetIntByIndex(v,i),x);
                r=MATH_FAIL;
            }
        }
    }

    int32_t sint[] = { 1, 1, 1, 0, 0, 0, -1, -1, -1, 0x7FFFFFFF, int32_t(0xFFFFFFFF), 0x1F1F1F1F };
    for( i=0; i < std::size(sint)/intcount; ++i )
    {
        XMVECTOR v = XMLoadSInt3( (const XMINT3*)&sint[i*intcount] );

        XMVECTORF32 check = { (float)sint[i*intcount], (float)sint[i*intcount+1], (float)sint[i*intcount+2], _Q_NAN };
        COMPARISON cmp = CompareXMVECTOR(v,check,3);
        if ( cmp > WITHIN10EPSILON )
        {
            printe("%s: SINT %d = %d %d %d = %f %f %f ... %f %f %f\n",
                   TestName, i, sint[i*intcount], sint[i*intcount+1], sint[i*intcount+2], 
                   XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), 
                   XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v) );
            r = MATH_FAIL;
        }
    }

    uint32_t uint[] = { 1, 1, 1, 0, 0, 0, 0x0340c0f0, 0x82e02304, 0x4839efcd, 0x7FFFFFFF, 0xFFFFFFFF, 0x1F1F1F1F  };
    for( i=0; i < std::size(uint)/intcount; ++i )
    {
        XMVECTOR v = XMLoadUInt3( (const XMUINT3*)&uint[i*intcount] );

        XMVECTORF32 check = { (float)uint[i*intcount], (float)uint[i*intcount+1], (float)uint[i*intcount+2], _Q_NAN };
        COMPARISON cmp = CompareXMVECTOR(v,check,3);
        if ( cmp > WITHIN10EPSILON )
        {
            printe("%s: uint32_t %d = %u %u %u = %f %f %f ... %f %f %f\n",
                   TestName, i, uint[i*intcount], uint[i*intcount+1], uint[i*intcount+2], 
                   XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), 
                   XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v) );
            r = MATH_FAIL;
        }
    }

    return r;
}
HRESULT Test072(LogProxy* pLog)
{
//XMLoadFloat3A / XMLoadInt3A
    static_assert( sizeof(XMFLOAT3A) == 16, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT3A>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT3A>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT3A>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT3A>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XM_ALIGNED_DATA(16) char c[64];
    int offset = 16;
    float f;
    int i, j; 
    XMVECTOR v;
    HRESULT r = S_OK;
    const int floatcount = 3;

    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }


        v = XMLoadFloat3A((const XMFLOAT3A*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != i) {
                printe("%s: %d corrupted input data! %d: %f ... %f\n", TestName, j,i,f,(float)i);
                r=MATH_FAIL;
            }
            if(XMVectorGetByIndex(v,i) != i) {
                printe("%s: %d corrupted output float %d: %f ... %f\n", TestName, j,i,XMVectorGetByIndex(v,i),(float)i);
                r=MATH_FAIL;
            }
        }
    }

    const int intcount = 3;

    for(j = 0; j < 16; j+=16)
    {
        for(i = 0; i < sizeof(c); i++) { c[i] = (char)(~i & 0xff); }
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        for(i = 0; i < intcount; i++)
            WriteInt((uint32_t)i, (char*)&c[first + (i*4)]);

        v = XMLoadInt3A((const uint32_t*)&c[offset + j]);

        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        for(i = last; i < sizeof(c); i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++)
        {
            uint32_t x = ReadInt((char*)&(c[first+(i*4)]));
            if(x != (uint32_t)i)
            {
                printe("%s: %d corrupted input data! %d: %i ... %i\n", TestName, j,i,x,(uint32_t)i);
                r=MATH_FAIL;
            }

            if(XMVectorGetIntByIndex(v,i) != x)
            {
                printe("%s: %d corrupted output int %d: %i ... %i\n", TestName, j,i,XMVectorGetIntByIndex(v,i),x);
                r=MATH_FAIL;
            }
        }
    }

    return r;
}
HRESULT Test073(LogProxy* pLog)
{
//XMLoadFloat3x3 
    static_assert( sizeof(XMFLOAT3X3) == 4*9, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT3X3>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT3X3>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT3X3>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT3X3>::value, "Move Assign.");

    XM_ALIGNED_DATA(16) char c[112];
    int floatcount = 9;
    int offset = 16;
    float f;
    int i, j; 
    XMMATRIX m;
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }

        m = XMLoadFloat3x3((const XMFLOAT3X3*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            float check = (float)i;
            float mf = XMVectorGetByIndex( m.r[i / 3], i % 3 );
            if(f != check) {
                printe("%s: %d corrupted source float %d: %f ... %f\n", TestName, j,i,f,check);
                r=MATH_FAIL;
            }
            if(mf != check) {
                printe("%s: %d improperly read float %d: %f ... %f\n", TestName, j,i,mf,check);
                r=MATH_FAIL;
            }
        }
        for(i = 0; i < 3; i++) {
            float mf = XMVectorGetW( m.r[i] );
            if(mf != 0.f) {
                printe("%s: corrupted the out-of-bounds float [%d][3]: %f ...0\n", TestName, i, mf);
            }
            mf = XMVectorGetByIndex(m.r[3], i);
            if(mf != 0.f) {
                printe("%s: corrupted the out-of-bounds float [3][%d]: %f ...0\n", TestName, i, mf);
            }
        }
        float mf = XMVectorGetW( m.r[3] );
        if(mf != 1.f) {
             printe("%s: corrupted the out-of-bounds float [3][3]: %f ...1\n", TestName, mf);
        }
    }
    return r;
}

HRESULT Test074(LogProxy* pLog)
{
//XMLoadFloat4 / XMLoadInt4 / XMLoadSInt4 / XMLoadUInt4
    static_assert( sizeof(XMFLOAT4) == 16, "Unexpected structure size" );
    static_assert( sizeof(XMINT4) == 16, "Unexpected structure size" );
    static_assert( sizeof(XMUINT4) == 16, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_assignable<XMINT4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_assignable<XMUINT4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_copy_constructible<XMINT4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_copy_constructible<XMUINT4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMINT4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUINT4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT4>::value, "Move Assign.");
    static_assert(std::is_nothrow_move_assignable<XMINT4>::value, "Move Assign.");
    static_assert(std::is_nothrow_move_assignable<XMUINT4>::value, "Move Assign.");

    char c[64];
    int offset = 16;
    float f;
    int i, j; 
    XMVECTOR v;
    HRESULT r = S_OK;
    const int floatcount = 4;

    for(j = 0; j < 16; j++) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }


        v = XMLoadFloat4((const XMFLOAT4*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != i) {
                printe("%s: %d corrupted input data! %d: %f ... %f\n", TestName, j,i,f,(float)i);
                r=MATH_FAIL;
            }
            if(XMVectorGetByIndex(v,i) != i) {
                printe("%s: %d corrupted output float %d: %f ... %f\n", TestName, j,i,XMVectorGetByIndex(v,i),(float)i);
                r=MATH_FAIL;
            }
        }
    }

    const int intcount = 4;

    for(j = 0; j < 16; j++)
    {
        for(i = 0; i < sizeof(c); i++) { c[i] = (char)(~i & 0xff); }
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        for(i = 0; i < intcount; i++)
            WriteInt((uint32_t)i, (char*)&c[first + (i*4)]);

        v = XMLoadInt4((const uint32_t*)&c[offset + j]);

        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        for(i = last; i < sizeof(c); i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++)
        {
            uint32_t x = ReadInt((char*)&(c[first+(i*4)]));
            if(x != (uint32_t)i)
            {
                printe("%s: %d corrupted input data! %d: %i ... %i\n", TestName, j,i,x,(uint32_t)i);
                r=MATH_FAIL;
            }

            if(XMVectorGetIntByIndex(v,i) != x)
            {
                printe("%s: %d corrupted output int %d: %i ... %i\n", TestName, j,i,XMVectorGetIntByIndex(v,i),x);
                r=MATH_FAIL;
            }
        }
    }

    int32_t sint[] = { 1, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -1, 0x7FFFFFFF, int32_t(0xFFFFFFFF), 0x1F1F1F1F, int32_t(0xCDCDCDCD) };
    for( i=0; i < std::size(sint)/intcount; ++i )
    {
        v = XMLoadSInt4( (const XMINT4*)&sint[i*intcount] );

        XMVECTORF32 check = { (float)sint[i*intcount], (float)sint[i*intcount+1], (float)sint[i*intcount+2], (float)sint[i*intcount+3] };
        COMPARISON cmp = CompareXMVECTOR(v,check,4);
        if ( cmp > WITHIN10EPSILON )
        {
            printe("%s: SINT %d = %d %d %d %d = %f %f %f %f ... %f %f %f %f (%d)\n",
                   TestName, i, sint[i*intcount], sint[i*intcount+1], sint[i*intcount+2], sint[i*intcount+3],
                   XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check),
                   XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), cmp );
            r = MATH_FAIL;
        }
    }

    uint32_t uint[] = { 1, 1, 1, 1, 0, 0, 0, 0, 0x0340c0f0, 0x82e02304, 0x4839efcd, 0x838df7cd, 0x7FFFFFFF, 0xFFFFFFFF, 0x1F1F1F1F, 0xCDCDCDCD };
    for( i=0; i < std::size(uint)/intcount; ++i )
    {
        v = XMLoadUInt4( (const XMUINT4*)&uint[i*intcount] );

        XMVECTORF32 check = { (float)uint[i*intcount], (float)uint[i*intcount+1], (float)uint[i*intcount+2], (float)uint[i*intcount+3] };
        COMPARISON cmp = CompareXMVECTOR(v,check,4);
        if ( cmp > WITHIN10EPSILON )
        {
            printe("%s: uint32_t %d = %u %u %u %u = %f %f %f %f ... %f %f %f %f (%d)\n",
                   TestName, i, uint[i*intcount], uint[i*intcount+1], uint[i*intcount+2], uint[i*intcount+3],
                   XMVectorGetX(check), XMVectorGetY(check), XMVectorGetZ(check), XMVectorGetW(check),
                   XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v), cmp );
            r = MATH_FAIL;
        }
    }

    return r;
}

HRESULT Test075(LogProxy* pLog)
{
//XMLoadFloat4A / XMLoadInt4A
    static_assert( sizeof(XMFLOAT4A) == 16, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT4A>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT4A>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT4A>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT4A>::value, "Move Assign.");

    XM_ALIGNED_DATA(16) char c[64];
    int offset = 16;
    float f;
    int i, j; 
    XMVECTOR v;
    HRESULT r = S_OK;
    const int floatcount = 4;

    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }


        v = XMLoadFloat4A((const XMFLOAT4A*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != i) {
                printe("%s: %d corrupted input data! %d: %f ... %f\n", TestName, j,i,f,(float)i);
                r=MATH_FAIL;
            }
            if(XMVectorGetByIndex(v,i) != i) {
                printe("%s: %d corrupted output float %d: %f ... %f\n", TestName, j,i,XMVectorGetByIndex(v,i),(float)i);
                r=MATH_FAIL;
            }
        }
    }

    const int intcount = 4;

    for(j = 0; j < 16; j+=16)
    {
        for(i = 0; i < sizeof(c); i++) { c[i] = (char)(~i & 0xff); }
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        for(i = 0; i < intcount; i++)
            WriteInt((uint32_t)i, (char*)&c[first + (i*4)]);

        v = XMLoadInt4A((const uint32_t*)&c[offset + j]);

        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        for(i = last; i < sizeof(c); i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++)
        {
            uint32_t x = ReadInt((char*)&(c[first+(i*4)]));
            if(x != (uint32_t)i)
            {
                printe("%s: %d corrupted input data! %d: %i ... %i\n", TestName, j,i,x,(uint32_t)i);
                r=MATH_FAIL;
            }

            if(XMVectorGetIntByIndex(v,i) != x)
            {
                printe("%s: %d corrupted output int %d: %i ... %i\n", TestName, j,i,XMVectorGetIntByIndex(v,i),x);
                r=MATH_FAIL;
            }
        }
    }

    return r;
}

HRESULT Test076(LogProxy* pLog)
{
//XMLoadFloat4x3 (row-major)
    static_assert( sizeof(XMFLOAT4X3) == 4*12, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT4X3>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT4X3>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT4X3>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT4X3>::value, "Move Assign.");

    XMMATRIX m;
    HRESULT ret = S_OK;

    {
        static const XMFLOAT4X3 s4x3 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        static const XMMATRIX check = { 1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 10, 11, 12, 1 };

        m = XMLoadFloat4x3(&s4x3);

        COMPARISON c = CompareXMMATRIX(m, check);
        if (c != EXACT)
        {
            printe("%s: (%d)\n", TestName, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    {
        char c[112];
        const int floatcount = 12;
        const int offset = 16;
        int i, j;

        for (j = 0; j < 16; j++) {
            for (i = 0; i < sizeof(c); i++) {
                c[i] = (char)(~i & 0xff);
            }
            int first = offset + j;
            int last = offset + j + 4 * floatcount;

            for (i = 0; i < floatcount; i++) {
                WriteFloat((float)i, (char*)&c[first + (i * 4)]);
            }

            m = XMLoadFloat4x3(reinterpret_cast<const XMFLOAT4X3*>(&c[offset + j]));

            for (i = 0; i < first; i++) {
                if (c[i] != (char)(~i & 0xff)) {
                    printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                    ret = MATH_FAIL;
                }
            }
            for (i = last; i < sizeof(c); i++) {
                if (c[i] != (char)(~i & 0xff)) {
                    printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                    ret = MATH_FAIL;
                }
            }
            for (i = 0; i < floatcount; i++) {
                float f = ReadFloat((char*)&(c[first + (i * 4)]));
                float check = (float)i;
                float mf = XMVectorGetByIndex(m.r[i / 3], i % 3);
                if (f != check) {
                    printe("%s: %d corrupted source float %d: %f ... %f\n", TestName, j, i, f, check);
                    ret = MATH_FAIL;
                }
                if (mf != check) {
                    printe("%s: %d improperly read float %d: %f ... %f\n", TestName, j, i, mf, check);
                    ret = MATH_FAIL;
                }
            }
        }
    }

    return ret;
}

HRESULT Test518(LogProxy* pLog)
{
    //XMLoadFloat3x4 (column-major) 

    static_assert(sizeof(XMFLOAT3X4) == 4 * 12, "Unexpected structure size");

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT3X4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT3X4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT3X4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT3X4>::value, "Move Assign.");

    XMMATRIX m;
    HRESULT ret = S_OK;

    {
        static const XMFLOAT3X4 s3x4 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        static const XMMATRIX check = { 1, 5, 9, 0, 2, 6, 10, 0, 3, 7, 11, 0, 4, 8, 12, 1 };

        m = XMLoadFloat3x4(&s3x4);

        COMPARISON c = CompareXMMATRIX(m, check);
        if (c != EXACT)
        {
            printe("%s: (%d)\n", TestName, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    {
        char c[112];
        const int floatcount = 12;
        const int offset = 16;
        int i, j;

        for (j = 0; j < 16; j++) {
            for (i = 0; i < sizeof(c); i++) {
                c[i] = (char)(~i & 0xff);
            }
            int first = offset + j;
            int last = offset + j + 4 * floatcount;

            for (i = 0; i < floatcount; i++) {
                WriteFloat((float)i, (char*)&c[first + (i * 4)]);
            }

            m = XMLoadFloat3x4(reinterpret_cast<const XMFLOAT3X4*>(&c[offset + j]));

            for (i = 0; i < first; i++) {
                if (c[i] != (char)(~i & 0xff)) {
                    printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                    ret = MATH_FAIL;
                }
            }
            for (i = last; i < sizeof(c); i++) {
                if (c[i] != (char)(~i & 0xff)) {
                    printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                    ret = MATH_FAIL;
                }
            }

            static const size_t transpose[12] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14 };

            for (i = 0; i < floatcount; i++) {
                float f = ReadFloat((char*)&(c[first + (i * 4)]));
                float check = (float)i;
                size_t index = transpose[i];
                float mf = XMVectorGetByIndex(m.r[index / 4], index % 4);
                if (f != check) {
                    printe("%s: %d corrupted source float %d: %f ... %f\n", TestName, j, i, f, check);
                    ret = MATH_FAIL;
                }
                if (mf != check) {
                    printe("%s: %d improperly read float %d: %f ... %f\n", TestName, j, i, mf, check);
                    ret = MATH_FAIL;
                }
            }
        }
    }

    return ret;
}

HRESULT Test077(LogProxy* pLog)
{
//XMLoadFloat4x4 
    static_assert( sizeof(XMFLOAT4X4) == 4*16, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT4X4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT4X4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT4X4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT4X4>::value, "Move Assign.");

    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!!!!!!!!!!!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);

    int offset = 0;
    int floatcount = 16;
    float f;
    intptr_t i, j;
    XMMATRIX m;
    HRESULT r = S_OK;

    for(j = pc64k - 64; j < pc64k + 16; j++) {
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }
        intptr_t first = offset+j;
        intptr_t last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }

        m = XMLoadFloat4x4((const XMFLOAT4X4*)&c[offset + j]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i), c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i), c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            float check = (float)i;
            float mf = XMVectorGetByIndex(m.r[i / 4], i % 4);
            if(f != check) {
                printe("%s: %p corrupted source float %p: %f ... %f\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),f,check);
                r=MATH_FAIL;
            }
            if(mf != check) {
                printe("%s: %p improperly read float %p: %f ... %f\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),mf,check);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test078(LogProxy* pLog)
{
// XMLoadFloat4x4A
    static_assert( sizeof(XMFLOAT4X4) == 4*16, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT4X4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT4X4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT4X4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT4X4>::value, "Move Assign.");

    XM_ALIGNED_DATA(16) char c[112];
    int floatcount = 16;
    int offset = 16;
    float f;
    int i, j; 
    XMMATRIX m;
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        for(i = 0; i < floatcount; i++) {
            WriteFloat((float)i, (char*)&c[first + (i*4)]);
        }

        m = XMLoadFloat4x4A((const XMFLOAT4X4A*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            float check = (float)i;
            float mf = XMVectorGetByIndex(m.r[i / 4], i % 4);
            if(f != check) {
                printe("%s: %d corrupted source float %d: %f ... %f\n", TestName, j,i,f,check);
                r=MATH_FAIL;
            }
            if(mf != check) {
                printe("%s: %d improperly read float %d: %f ... %f\n", TestName, j,i,mf,check);
                r=MATH_FAIL;
            }
        }
    }
    return r;
}

HRESULT Test079(LogProxy* pLog)
{
//XMLoadHalf2 
    static_assert( sizeof(XMHALF2) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMHALF2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMHALF2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMHALF2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMHALF2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    char c[112];
    int floatcount = 2;
    int offset = 16;
    int floatsize = 2;
    HALF f;
    int i, j; 
    XMVECTOR m;
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=1) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ floatsize * floatcount;

        for(i = 0; i < floatcount; i++) {
            *(HALF*)&c[first + (i*floatsize)] = 0x4200;
        }

        m = XMLoadHalf2((const XMHALF2*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = *(const HALF*)&(c[first+(i*floatsize)]);
            uint32_t check = 0x40400000;
            if(f != 0x4200) {
                printe("%s: %d corrupted source float %d: 0x%x ... 0x%x\n", TestName, j,i,f,0x4200);
                r=MATH_FAIL;
            }
            if(XMVectorGetIntByIndex(m,i) != check) {
                printe("%s: %d improperly read float %d: 0x%x ... 0x%x\n", TestName, j,i,XMVectorGetIntByIndex(m,i),check);
                r=MATH_FAIL;
            }
        }
        printi("%s: %d\n", TestName, j);
    }
    return r;
}

HRESULT Test080(LogProxy* pLog)
{
//XMLoadHalf4 
    static_assert( sizeof(XMHALF4) == 8, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMHALF4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMHALF4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMHALF4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMHALF4>::value, "Move Assign.");

    char c[112];
    int floatcount = 4;
    int offset = 16;
    int floatsize = 2;
    HALF f;
    int i, j; 
    XMVECTOR m;
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=1) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ floatsize * floatcount;

        for(i = 0; i < floatcount; i++) {
            *(HALF*)&c[first + (i*floatsize)] = (HALF)(0x4200 + i);
        }

        m = XMLoadHalf4((const XMHALF4*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = *(const HALF*)&(c[first+(i*floatsize)]);
            uint32_t check = 0x40400000 + (i << 13);
            if(f != 0x4200 + i) {
                printe("%s: %d corrupted source float %d: 0x%x ... 0x%x\n", TestName, j,i,f,0x4200+i);
                r=MATH_FAIL;
            }
            if(XMVectorGetIntByIndex(m,i) != check) {
                printe("%s: %d improperly read float %d: 0x%x ... 0x%x\n", TestName, j,i,XMVectorGetIntByIndex(m,i),check);
                r=MATH_FAIL;
            }
        }
        printi("%s: %d\n", TestName, j);
    }
    return r;
}

HRESULT Test081(LogProxy* pLog)
{
//XMLoadXDecN4 
    static_assert( sizeof(XMXDECN4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMXDECN4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMXDECN4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMXDECN4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMXDECN4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;

    static const uint32_t s[] = {0,0x1ff+(0x1ff<<10)+(0x1ff<<20)+(0x3U<<30),0x201+(0x201<<10)+(0x201<<20)+(0x3U<<30),0x0+(0x1ff<<10)+(0x201<<20)+(0x0U<<30)};
    static const XMVECTORF32 check[] = {{0,0,0,0},{1,1,1,1},{-1,-1,-1,1},{0,1,-1,0}};
    for(int k = 0; k < countof(s); k++) {
        v = XMLoadXDecN4((const XMXDECN4*)&s[k]);
        COMPARISON c = CompareXMVECTOR(v,check[k],4);
        if(c != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),c);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        uint32_t a = rand() & 0x3ff; if(a == 0x200) a = 0;
        uint32_t b = rand() & 0x3ff; if(b == 0x200) b = 0;
        uint32_t c = rand() & 0x3ff; if(c == 0x200) c = 0;
        uint32_t d = rand() & 0x3; 
        XMXDECN4 src;
        XMVECTORF32 chk= {{
            ((float)(a&0x1ff))/511.f-((a&0x200)?(512.f/511.f):0.f),
            ((float)(b&0x1ff))/511.f-((b&0x200)?(512.f/511.f):0.f),
            ((float)(c&0x1ff))/511.f-((c&0x200)?(512.f/511.f):0.f),
            ((float)(d))/3.f}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadXDecN4(&src);
        c = CompareXMVECTOR(v,chk,4);
        if(c > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src),XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test082(LogProxy* pLog)
{
//XMLoadShortN2 
    static_assert( sizeof(XMSHORTN2) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMSHORTN2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMSHORTN2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMSHORTN2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMSHORTN2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMSHORTN2 src;

    static const unsigned short s[4][4] = {{0,0,0,0},{0x8001,0x8001,0x8001,0x8001},{0x7fff,0x7fff,0x7fff,0x7fff},{0x8001,0x7fff,0x0000,0x8001}};
    static const XMVECTORF32 check[] = {{0,0,0,0},{-1,-1,-1,-1},{1,1,1,1},{-1,1,0,-1}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1];
        v = XMLoadShortN2(&src);
        COMPARISON c = CompareXMVECTOR(v,check[k],2);
        if(c != EXACT) {
            printe("%s: %x -> %f %f ... %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),c);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        short a = (rand() + rand()) & 0xffff; if(a == -32768) a = 0;
        short b = (rand() + rand()) & 0xffff; if(b == -32768) b = 0;
        XMVECTORF32 chk= {{a/32767.f, b/32767.f, _Q_NAN, _Q_NAN}};
        src.x=a; src.y=b;

        v = XMLoadShortN2(&src);
        COMPARISON c2 = CompareXMVECTOR(v,chk,2);
        if(c2 > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f ... %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),c2);
            ret = MATH_FAIL;
        }
    }

// Special case of -32768
    {
        src.x = src.y = -32768;
        XMVECTORF32 chk = { -1.f, -1.f, 0, 0 };
        v = XMLoadShortN2(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f ... %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test083(LogProxy* pLog)
{
//XMLoadShortN4 
    static_assert( sizeof(XMSHORTN2) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMSHORTN2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMSHORTN2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMSHORTN2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMSHORTN2>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMSHORTN4 src;

    static const unsigned short s[4][4] = {{0,0,0,0},{0x8001,0x8001,0x8001,0x8001},{0x7fff,0x7fff,0x7fff,0x7fff},{0x8001,0x7fff,0x0000,0x8001}};
    static const XMVECTORF32 check[] = {{0,0,0,0},{-1,-1,-1,-1},{1,1,1,1},{-1,1,0,-1}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; src.z=s[k][2]; src.w=s[k][3];
        v = XMLoadShortN4(&src);
        COMPARISON c = CompareXMVECTOR(v,check[k],4);
        if(c != EXACT) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),c);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        short a = (rand() + rand()) & 0xffff; if(a == -32768) a = 0;
        short b = (rand() + rand()) & 0xffff; if(b == -32768) b = 0;
        short c = (rand() + rand()) & 0xffff; if(c == -32768) c = 0;
        short d = (rand() + rand()) & 0xffff; if(d == -32768) d = 0;
        XMVECTORF32 chk= {{a/32767.f, b/32767.f, c/32767.f, d/32767.f}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadShortN4(&src);
        COMPARISON c2 = CompareXMVECTOR(v,chk,4);
        if(c2 > WITHIN10EPSILON) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src),XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),c2);
            ret = MATH_FAIL;
        }
    }

// Special case of -32768
    {
        src.x = src.y = src.z = src.w = -32768;
        XMVECTORF32 chk = { -1.f, -1.f, -1.f, -1.f };
        v = XMLoadShortN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src),XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test125(LogProxy* pLog)
{
//XMPlaneDot 
    XMVECTORF32 l={}, v={};
    COMPARISON c = EXACT, temp;
    HRESULT ret = S_OK;
    int i, j;
    for(i = 0; i < 10; i++) {
        for(j = 0; j < 4; j++) {
            l.v = XMVectorSetByIndex(l,((float)rand()) / 1000.f,j);
            v.v = XMVectorSetByIndex(v,((float)rand()) / 1000.f,j);
        }
        XMVECTOR r = XMPlaneDot(l, v);
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(l)*XMVectorGetX(v)+  XMVectorGetY(l)*XMVectorGetY(v)+  XMVectorGetZ(l)*XMVectorGetZ(v)+  XMVectorGetW(l)*XMVectorGetW(v));
        temp = CompareXMVECTOR(r, check,4);
        c = std::max(c,temp);
        if( temp > WITHIN4096) {
            printe ("%s: %f %f %f %f dot %f %f %f %f = %f ... %f (%d)\n",
                TestName, XMVectorGetX(l),XMVectorGetY(l),XMVectorGetZ(l),XMVectorGetW(l),
                XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(r),XMVectorGetX(check), temp);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, temp);
        }
    }
    return ret;
}
HRESULT Test126(LogProxy* pLog)
{
//XMPlaneDotCoord 
    XMVECTORF32 l={}, v={};
    COMPARISON c = EXACT, temp;
    HRESULT ret = S_OK;
    int i, j;
    for(i = 0; i < 10; i++) {
        for(j = 0; j < 4; j++) {
            l.v = XMVectorSetByIndex(l,((float)rand()) / 1000.f,j);
            v.v = XMVectorSetByIndex(v,((float)rand()) / 1000.f,j);
        }
        v.v = XMVectorSetW(v,_Q_NAN);
        XMVECTOR r = XMPlaneDotCoord(l, v);
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(l)*XMVectorGetX(v)+  XMVectorGetY(l)*XMVectorGetY(v)+ XMVectorGetZ(l)*XMVectorGetZ(v)+ XMVectorGetW(l));
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
HRESULT Test127(LogProxy* pLog)
{
//XMPlaneDotNormal 
    XMVECTORF32 l={}, v={};
    COMPARISON c = EXACT, temp;
    HRESULT ret = S_OK;
    int i, j;
    for(i = 0; i < 10; i++) {
        for(j = 0; j < 4; j++) {
            l.v = XMVectorSetByIndex(l,((float)rand()) / 1000.f,j);
            v.v = XMVectorSetByIndex(v,((float)rand()) / 1000.f,j);
        }
        l.v = XMVectorSetW(l,_Q_NAN);
        v.v = XMVectorSetW(v,_Q_NAN);
        XMVECTOR r = XMPlaneDotNormal(l, v);
        XMVECTOR check = XMVectorReplicate(XMVectorGetX(l)*XMVectorGetX(v)+ XMVectorGetY(l)*XMVectorGetY(v)+ XMVectorGetZ(l)*XMVectorGetZ(v));
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
HRESULT Test128(LogProxy* pLog)
{
//XMPlaneEqual 
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
        r = XMPlaneEqual(v1,v2);
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
        r = XMPlaneEqual(v1,v2);
        if(r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n",TestName,j,i,r,check);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test129(LogProxy* pLog)
{
//XMPlaneFromPointNormal 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 p={}, n={};
    XMVECTORF32 check={};

    for(int k = 0; k < 7; k++) {
        for(int i = 0; i < 4; i++) {
            p.v = XMVectorSetByIndex(p,((float)rand())/2000.f - 8.f,i);
            n.v = XMVectorSetByIndex(n,((float)rand())/2000.f - 8.f,i);
            check.v = XMVectorSetByIndex(check,XMVectorGetByIndex(n,i),i);
        }
        p.v = XMVectorSetW(p,_Q_NAN);
        n.v = XMVectorSetW(n,_Q_NAN);
        check.v = XMVectorSetW(check,-(XMVectorGetX(p)*XMVectorGetX(n)+XMVectorGetY(p)*XMVectorGetY(n)+XMVectorGetZ(p)*XMVectorGetZ(n)));

        XMVECTOR r = XMPlaneFromPointNormal(p,n);
        c = CompareXMVECTOR(r,check,4);
        if(c > WITHIN4096) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, XMVectorGetX(p),XMVectorGetY(p),XMVectorGetZ(p),XMVectorGetW(p),
                XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n),XMVectorGetW(n),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
        float dot = XMVectorGetX(p)*XMVectorGetX(r)+XMVectorGetY(p)*XMVectorGetY(r)+XMVectorGetZ(p)*XMVectorGetZ(r)+XMVectorGetW(r);
        c = Compare(dot,0);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f. Dot(%f ... 0) (%d)\n",
                TestName, XMVectorGetX(p),XMVectorGetY(p),XMVectorGetZ(p),XMVectorGetW(p),
                XMVectorGetX(n),XMVectorGetY(n),XMVectorGetZ(n),XMVectorGetW(n),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),dot,c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test130(LogProxy* pLog)
{
//XMPlaneFromPoints 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR p[3];
    XMVECTOR r,check;
    p[0] = XMVectorSet(1,0,0,_Q_NAN);
    p[1] = XMVectorSet(0,1,0,_Q_NAN);
    p[2] = XMVectorSet(0,0,0,_Q_NAN);
    check = XMVectorSet(0,0,1,0);
    r = XMPlaneFromPoints(p[0],p[1],p[2]);
    c = CompareXMVECTOR(r,check,4);
    if(c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
            TestName, XMVectorGetX(p[0]),XMVectorGetY(p[0]),XMVectorGetZ(p[0]),XMVectorGetW(p[0]),
            XMVectorGetX(p[1]),XMVectorGetY(p[1]),XMVectorGetZ(p[1]),XMVectorGetW(p[1]),
            XMVectorGetX(p[2]),XMVectorGetY(p[2]),XMVectorGetZ(p[2]),XMVectorGetW(p[2]),
            XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
            XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
        ret = MATH_FAIL;
    } else {
        printi("%s: %d\n", TestName, c);
    }
    p[0] = XMVectorSet(0,1,0,_Q_NAN);
    p[1] = XMVectorSet(0,0,1,_Q_NAN);
    p[2] = XMVectorSet(0,0,0,_Q_NAN);
    check = XMVectorSet(1,0,0,0);
    r = XMPlaneFromPoints(p[0],p[1],p[2]);
    c = CompareXMVECTOR(r,check,4);
    if(c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
            TestName, XMVectorGetX(p[0]),XMVectorGetY(p[0]),XMVectorGetZ(p[0]),XMVectorGetW(p[0]),
            XMVectorGetX(p[1]),XMVectorGetY(p[1]),XMVectorGetZ(p[1]),XMVectorGetW(p[1]),
            XMVectorGetX(p[2]),XMVectorGetY(p[2]),XMVectorGetZ(p[2]),XMVectorGetW(p[2]),
            XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
            XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
        ret = MATH_FAIL;
    } else {
        printi("%s: %d\n", TestName, c);
    }
    p[0] = XMVectorSet(0,0,1,_Q_NAN);
    p[1] = XMVectorSet(1,0,0,_Q_NAN);
    p[2] = XMVectorSet(0,0,0,_Q_NAN);
    check = XMVectorSet(0,1,0,0);
    r = XMPlaneFromPoints(p[0],p[1],p[2]);
    c = CompareXMVECTOR(r,check,4);
    if(c > WITHINBIGEPSILON) {
        printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f (%d)\n",
            TestName, XMVectorGetX(p[0]),XMVectorGetY(p[0]),XMVectorGetZ(p[0]),XMVectorGetW(p[0]),
            XMVectorGetX(p[1]),XMVectorGetY(p[1]),XMVectorGetZ(p[1]),XMVectorGetW(p[1]),
            XMVectorGetX(p[2]),XMVectorGetY(p[2]),XMVectorGetZ(p[2]),XMVectorGetW(p[2]),
            XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
            XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),c);
        ret = MATH_FAIL;
    } else {
        printi("%s: %d\n", TestName, c);
    }
    
    int i;
    for(i = 0; i < 3; i++) {
        for(int j = 0; j < 4; j++) {
            p[i] = XMVectorSetByIndex(p[i],((float)rand()) / 2000.f - 8.f,j);
        }
        p[i] = XMVectorSetW(p[i],_Q_NAN);
    }
    if((XMVectorGetX(p[0]-p[1]) * XMVectorGetY(p[0]-p[2])) == (XMVectorGetY(p[0]-p[1]) * XMVectorGetX(p[0]-p[2]))) {
        p[0] = XMVectorSetX(p[0],0);
        p[0] = XMVectorSetY(p[0],0);
        p[1] = XMVectorSetX(p[1],1);
        p[1] = XMVectorSetY(p[1],0);
        p[2] = XMVectorSetX(p[2],0);
        p[2] = XMVectorSetY(p[2],1);
    }
    r = XMPlaneFromPoints(p[0],p[1],p[2]);
    for(i = 0; i < 3; i++) {
        float dot = XMVectorGetX(p[i])*XMVectorGetX(r)+XMVectorGetY(p[i])*XMVectorGetY(r)+XMVectorGetZ(p[i])*XMVectorGetZ(r)+XMVectorGetW(r);
        c = Compare(dot,0);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f = %f %f %f %f ... %f %f %f %f. Dot%d(%f ... 0)(%d)\n",
                TestName,
                XMVectorGetX(p[0]),XMVectorGetY(p[0]),XMVectorGetZ(p[0]),XMVectorGetW(p[0]),
                XMVectorGetX(p[1]),XMVectorGetY(p[1]),XMVectorGetZ(p[1]),XMVectorGetW(p[1]),
                XMVectorGetX(p[2]),XMVectorGetY(p[2]),XMVectorGetZ(p[2]),XMVectorGetW(p[2]),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),
                XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check),i,dot,c);
            ret = MATH_FAIL;
        }
    }
    return ret;
}
HRESULT Test131(LogProxy* pLog)
{
//XMPlaneIntersectLine 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR l[2];
    XMVECTOR p = XMVectorZero();
    XMVECTOR r;
    for(int k = 0; k < 8; k++) {
        for(int i = 0; i < 2; i++) {
            l[i] = XMVectorSetX(l[i],((float)rand()) / 2000.f - 8.f);
            l[i] = XMVectorSetY(l[i],((float)rand()) / 2000.f - 8.f);
            l[i] = XMVectorSetZ(l[i],((float)rand()) / 2000.f - 8.f);
            l[i] = XMVectorSetW(l[i],_Q_NAN);
        }
        p = XMVectorSetX(p,((float)rand()) / 2000.f - 8.f);
        p = XMVectorSetY(p,((float)rand()) / 2000.f - 8.f);
        p = XMVectorSetZ(p,((float)rand()) / 2000.f - 8.f);
        p = XMVectorSetW(p,((float)rand()) / 2000.f - 8.f);

        if(fabs(XMVectorGetX(XMPlaneDot(p,l[0]-l[1]))) < .00001f) {
            FLOAT fxtemp = XMVectorGetX(p);
            p = XMVectorSetX(p,XMVectorGetY(p));
            p = XMVectorSetY(p,XMVectorGetZ(p));
            p = XMVectorSetZ(p,-fxtemp);
        }
        
        r = XMPlaneIntersectLine(p, l[0], l[1]);
        float dot = XMVectorGetX(XMPlaneDotCoord(p,r));
        c = Compare(dot,0);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f = %f %f %f %f. dotplane(%f, 0), %d\n",
                TestName, XMVectorGetX(p),XMVectorGetY(p),XMVectorGetZ(p),XMVectorGetW(p),
                XMVectorGetX(l[0]),XMVectorGetY(l[0]),XMVectorGetZ(l[0]),XMVectorGetW(l[0]),
                XMVectorGetX(l[1]),XMVectorGetY(l[1]),XMVectorGetZ(l[1]),XMVectorGetW(l[1]),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),dot,c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }

        float dist = XMVectorGetX(XMVector3LinePointDistance(l[0],l[1],r));
        c = Compare(dist,0);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f, %f %f %f %f, %f %f %f %f = %f %f %f %f. dist(%f, 0), %d\n",
                TestName, XMVectorGetX(p),XMVectorGetY(p),XMVectorGetZ(p),XMVectorGetW(p),
                XMVectorGetX(l[0]),XMVectorGetY(l[0]),XMVectorGetZ(l[0]),XMVectorGetW(l[0]),
                XMVectorGetX(l[1]),XMVectorGetY(l[1]),XMVectorGetZ(l[1]),XMVectorGetW(l[1]),
                XMVectorGetX(r),XMVectorGetY(r),XMVectorGetZ(r),XMVectorGetW(r),dist,c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test132(LogProxy* pLog)
{
//XMPlaneIntersectPlane 
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTOR l[2];
    XMVECTOR p[2];
    int i,j,k;
    for(k = 0; k < 8; k++) {
        for(i = 0; i < 2; i++) {
            for(j = 0; j < 4; j++) {
                p[i] = XMVectorSetByIndex(p[i],((float)rand()) / 2000.f - 8.f,j);
            }
        }

        XMPlaneIntersectPlane(&l[0],&l[1],p[0],p[1]);
        c = CompareXMVECTOR(l[0],l[1], 3);
        if(c <= WITHINEPSILON) {
            printe("%s: %f %f %f %f  %f %f %f %f = %f %f %f %f, %f %f %f %f. (%d)\nCan't be equal\n",
                TestName,
                XMVectorGetX(p[0]),XMVectorGetY(p[0]),XMVectorGetZ(p[0]),XMVectorGetW(p[0]),
                XMVectorGetX(p[1]),XMVectorGetY(p[1]),XMVectorGetZ(p[1]),XMVectorGetW(p[1]),
                XMVectorGetX(l[0]),XMVectorGetY(l[0]),XMVectorGetZ(l[0]),XMVectorGetW(l[0]),
                XMVectorGetX(l[1]),XMVectorGetY(l[1]),XMVectorGetZ(l[1]),XMVectorGetW(l[1]),c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, WAYOFF-c);
        }
        for(i = 0; i < 2; i++) {
            for(j = 0; j < 2; j++) {
                float dot = XMVectorGetX(XMPlaneDotCoord(p[i],l[j]));
//				float dot = XMVectorGetX(p[i])*XMVectorGetX(l[j])+XMVectorGetY(p[i])*XMVectorGetY(l[j])+XMVectorGetZ(p[i])*XMVectorGetZ(l[j])+XMVectorGetW(p[i]);
                c = Compare(dot,0);
                if(c > WITHINBIGEPSILON) {
                    printe("%s: %f %f %f %f  %f %f %f %f = %f %f %f %f, %f %f %f %f. i%d, j%d, dot (%f ... 0) (%d)\n",
                        TestName,
                        XMVectorGetX(p[0]),XMVectorGetY(p[0]),XMVectorGetZ(p[0]),XMVectorGetW(p[0]),
                        XMVectorGetX(p[1]),XMVectorGetY(p[1]),XMVectorGetZ(p[1]),XMVectorGetW(p[1]),
                        XMVectorGetX(l[0]),XMVectorGetY(l[0]),XMVectorGetZ(l[0]),XMVectorGetW(l[0]),
                        XMVectorGetX(l[1]),XMVectorGetY(l[1]),XMVectorGetZ(l[1]),XMVectorGetW(l[1]),i,j,dot,c);
                    ret = MATH_FAIL;
                } else {
                    printi("%s: %d\n", TestName, c);
                }
            }
        }

    }
    return ret;
}
HRESULT Test133(LogProxy* pLog)
{
//XMPlaneIsInfinite 
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
                    res = XMPlaneIsInfinite(col);
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
HRESULT Test134(LogProxy* pLog)
{
//XMPlaneIsNaN 
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
                    col = XMVectorSet(f[r],f[g],f[b],f[a]);
                    check = (r < 2 || g < 2 || b < 2 || a < 2) ? TRUE : FALSE;
                    res = XMPlaneIsNaN(col);
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

HRESULT Test135(LogProxy* pLog)
{
//XMPlaneNormalize 
    int i,k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1={};
    XMVECTORF32 check={};
    float rcplength;

    for(k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        if(k == 0) v1.v = XMVectorSetW(v1,_Q_NAN);
        rcplength = 1.f/sqrtf(XMVectorGetX(v1)*XMVectorGetX(v1) + XMVectorGetY(v1)*XMVectorGetY(v1) + XMVectorGetZ(v1)*XMVectorGetZ(v1));
        for(i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check,XMVectorGetByIndex(v1,i) * rcplength,i);
        }

        XMVECTOR r = XMPlaneNormalize(v1);
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
HRESULT Test136(LogProxy* pLog)
{
//XMPlaneNotEqual 
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
        r = XMPlaneNotEqual(v1,v2);
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
        r = XMPlaneNotEqual(v1,v2);
        if(r != check) {
            printe("%s: j(%d) i(%d): %x ... %x\n",TestName,j,i,r,check);
            ret = MATH_FAIL;
        }
    }
    return ret;	
}
HRESULT Test137(LogProxy* pLog)
{
//XMPlaneTransform 
    XMVECTORF32 v={};
    XMVECTORF32 check={};
    COMPARISON c;
    HRESULT ret = S_OK;
    for(int k = 0; k < 15; k++) {
        int i;
        float tmp[4][4];
        for(i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                tmp[i][j] = ((float)rand())/2000.f - 8.f;
            }
            v.v = XMVectorSetByIndex(v,((float)rand())/2000.f - 8.f,i);
        }
#pragma warning( suppress : 6385 )
        XMMATRIX m( &tmp[0][0] );
        for(i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check,0,i);
            for(int j = 0; j < 4; j++) {
                check.v = XMVectorSetByIndex(check,XMVectorGetByIndex(check,i) + XMVectorGetByIndex(v,j) * tmp[j][i],i);
            }
        }
        XMVECTOR r = XMPlaneTransform(v,m);
        c = CompareXMVECTOR(r,check,4);
        if(c > WITHINBIGEPSILON) {
            printe("%s: %f %f %f %f *   (%d)\n", TestName, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),c);
            printmatrixe(m); dqe(r); printe("..."); dqe(check);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}



HRESULT Test138(LogProxy* pLog)
{
//XMPlaneTransformStream 

    static const int ins[] = {16,20,32,24,};
    static const int outs[] = {16,32,44,36};
    
    const uint32_t dwDataSize = 5000;
    const uint32_t dwNumItems = 50;
    uint8_t *pbSandbox1 = nullptr;
    uint8_t *pbSandbox2 = nullptr;
    uint8_t *pbSandbox3 = nullptr;
    uint8_t *pbReal1= nullptr;
    uint8_t *pbReal2= nullptr;
    uint8_t *pbReal3= nullptr;
    int InStrideIndex       =0;
    int OutStrideIndex      =0;
    int InAlignIndex        =0;
    int OutAlignIndex       = 0;
    BOOL OutWriteCombined = FALSE;
    
    XMVECTOR v[dwNumItems];
    XMVECTOR check[dwNumItems] = {};
    int count;
    HRESULT ret = S_OK;
    int i,j,n;
    

    //Sandbox 2 is always there.
    AllocWithAlignment(dwDataSize, 16, FALSE, (LPVOID*)&pbReal2, (LPVOID*)&pbSandbox2);
    if (!pbReal2 || !pbSandbox2)
        goto Cleanup;

    for (OutWriteCombined=0; OutWriteCombined<2; OutWriteCombined++)
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
            
                for (InStrideIndex=0; InStrideIndex<countof(ins); InStrideIndex++)
                {
                    for (OutStrideIndex=0; OutStrideIndex<countof(outs); OutStrideIndex++)
                    {
                        //wc memory and misaligned data don't mix.
                        if (OutWriteCombined)
                            if (outs[OutStrideIndex]%16 || g_iStartAlignments[OutAlignIndex]%16)
                                continue;
                    
                        count = dwNumItems;
                        float tmp[4][4];
                        for(i = 0; i < 4; i++) {
                            for(int j = 0; j < 4; j++) 
                            {
                                tmp[i][j] = ((float)rand())/2000.f - 8.f;
                            }
                            for(n = 0; n < count; n++) 
                            {
                                v[n] = XMVectorSetByIndex(v[n],((float)rand())/2000.f - 8.f,i);
                            }
                        }
#pragma warning( suppress : 6385 )
                        XMMATRIX m( &tmp[0][0] );
                        for(n = 0; n < count; n++) 
                        {
                            for(i = 0; i < 4; i++) 
                            {
                                check[n] = XMVectorSetByIndex(check[n],0,i);
                                for(j = 0; j < 4; j++) 
                                {
                                    check[n] = XMVectorSetByIndex(check[n],XMVectorGetByIndex(check[n],i) + XMVectorGetByIndex(v[n],j) * tmp[j][i],i);
                                }
                            }
                        }
                        fillsandbox(pbSandbox1,dwDataSize,v,    16,ins [InStrideIndex], count);
                        fillsandbox(pbSandbox2,dwDataSize,check,16,outs[OutStrideIndex],count);
                        initsandbox(pbSandbox3,dwDataSize);
                        const XMFLOAT4*pointer =XMPlaneTransformStream((XMFLOAT4*)&pbSandbox3[0],outs[OutStrideIndex],(const XMFLOAT4*)&pbSandbox1[0],ins[InStrideIndex],count,m);

                        if(pointer != (const XMFLOAT4*)&pbSandbox3[0]) 
                        {
                            printe("%s: returned %p ... %p\n", TestName, pointer, &pbSandbox3[0]);
                            ret = MATH_FAIL;
                        }
                        if(!checksandbox(pLog, pbSandbox3,pbSandbox2,outs[OutStrideIndex],16,count,5000,4)) 
                        {
                            printe("%s: ins:%d, outs:%d, count:%d\n", TestName, ins[InStrideIndex],outs[OutStrideIndex],count);
                            ret = MATH_FAIL;
                        }
                    }
                }//end "for InStrideIndex"
                FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);	        
                pbReal3=pbSandbox3=nullptr;
            }//end "for OutAlignIndex"
            FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
            pbReal1=pbSandbox1=nullptr;
        }//end "for InAlignIndex"
    }//end "for OutWriteCombined"

Cleanup:
    FreeWithAlignment(pbReal1, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal2, g_dwPhysicalAttribs);
    FreeWithAlignment(pbReal3, g_dwPhysicalAttribs);

    return ret;
}

HRESULT Test165(LogProxy* pLog)
{
//XMScalarACos
    float r, check2;
    HRESULT ret = S_OK;
    COMPARISON c;

    static const float f[] = {-1, 0, 1};
    static const float check[] = {Pi, Pi/2.f, 0};
    for(int i = 0; i < sizeof(f) / sizeof(f[0]); i++) {
        r = XMScalarACos(f[i]);
        c = Compare(r, check[i]);
        if(c > WITHIN10EPSILON) {
            printe("%s: (%f) = %f, MUST BE %f.  (%d)\n", 
                TestName, f[i], r, check[i], c);
            ret = MATH_FAIL;
        }
    }

    // Range for arc-cosine is [-1,1]
    for(float v = -1; v <= 1; v += .01f) {
        r = XMScalarACos(v);
        check2 = acosf(v);
        c = Compare(r,check2);
        if(c > WITHINBIGEPSILON) {
            printe("%s: (%f) = %f ... %f.  (%d)\n", 
                TestName, v, r, check2, c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test166(LogProxy* pLog)
{
//XMScalarACosEst
    float r, check2;
    HRESULT ret = S_OK;
    COMPARISON c;

    static const float f[] = {-1, 0, 1};
    static const float check[] = {Pi, Pi/2.f, 0};
    for(int i = 0; i < sizeof(f) / sizeof(f[0]); i++) {
        r = XMScalarACosEst(f[i]);
        c = Compare(r, check[i]);
        if(c > WITHIN4096) {
            printe("%s: (%f) = %f, MUST BE %f.  (%d)\n", 
                TestName, f[i], r, check[i], c);
            ret = MATH_FAIL;
        }
    }

    // Range for arc-cosine is [-1,1]
    for(float v = -1; v <= 1; v += .01f) {
        r = XMScalarACosEst(v);
        check2 = acosf(v);
        c = Compare(r,check2);
        if(c > WITHINBIGEPSILON) {
            printe("%s: (%f) = %f ... %f.  (%d)\n", 
                TestName, v, r, check2, c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test167(LogProxy* pLog)
{
//XMScalarASin
    float r, check2;
    HRESULT ret = S_OK;
    COMPARISON c;

    static const float f[] = {-1, 0, 1};
    static const float check[] = {-Pi/2.f, 0, Pi/2.f};
    for(int i = 0; i < sizeof(f) / sizeof(f[0]); i++) {
        r = XMScalarASin(f[i]);
        c = Compare(r, check[i]);
        if(c > WITHIN10EPSILON) {
            printe("%s: (%f) = %f, MUST BE %f.  (%d)\n", 
                TestName, f[i], r, check[i], c);
            ret = MATH_FAIL;
        }
    }

    // Range for arc-sine is [-1,1]
    for(float v = -1; v <= 1; v += .01f) {
        r = XMScalarASin(v);
        check2 = asinf(v);
        c = Compare(r,check2);
        if(c > WITHINBIGEPSILON) {
            printe("%s: (%f) = %f ... %f.  (%d)\n", 
                TestName, v, r, check2, c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test168(LogProxy* pLog)
{
//XMScalarASinEst
    float r, check2;
    HRESULT ret = S_OK;
    COMPARISON c;

    static const float f[] = {-1, 0, 1};
    static const float check[] = {-Pi/2.f, 0, Pi/2.f};
    for(int i = 0; i < sizeof(f) / sizeof(f[0]); i++) {
        r = XMScalarASinEst(f[i]);
        c = Compare(r, check[i]);
        if(c > WITHINBIGEPSILON) {
            printe("%s: (%f) = %f, MUST BE %f.  (%d)\n", 
                TestName, f[i], r, check[i], c);
            ret = MATH_FAIL;
        }
    }

    // Range for arc-sine is [-1,1]
    for(float v = -1; v <= 1; v += .01f) {
        r = XMScalarASinEst(v);
        check2 = asinf(v);
        c = Compare(r,check2);
        if(c > WITHINBIGEPSILON) {
            printe("%s: (%f) = %f ... %f.  (%d)\n", 
                TestName, v, r, check2, c);
            ret = MATH_FAIL;
        } else {
            printi("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test169(LogProxy* pLog)
{
//XMScalarCos
    COMPARISON c;
    HRESULT ret = S_OK;
    for(float f = -Pi; f < Pi; f += .032f) {
        float r = XMScalarCos(f);
        float check = cosf(f);
        c = Compare(r,check);
        if(c > WITHIN10EPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    for(float f = -100; f < 100; f += 3.61f) {
        float r = XMScalarCos(f);
        float check = cosf(f);
        c = Compare(r,check);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    for( float v = UC_START; v <= UC_END; v += UC_STEP ) {
        float r = XMScalarCos(v);
        float check = cosf(v);
        c = Compare(r,check);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, v,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    return ret;
}
HRESULT Test170(LogProxy* pLog)
{
//XMScalarCosEst
    COMPARISON c;
    HRESULT ret = S_OK;
    for(float f = -Pi; f < Pi; f += .032f) {
        float r = XMScalarCosEst(f);
        float check = cosf(f);
        c = Compare(r,check);
        if(c > WITHIN4096) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    for(float f = -100; f < 100; f += 3.61f) {
        float r = XMScalarCosEst(f);
        float check = cosf(f);
        c = Compare(r,check);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    for( float v = UC_START; v <= UC_END; v += UC_STEP ) {
        float r = XMScalarCosEst(v);
        float check = cosf(v);
        c = Compare(r,check);
        if(c > WITHIN4096) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, v,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    return ret;
}

HRESULT Test171(LogProxy* pLog)
{
//XMScalarModAngle 
    HRESULT ret = S_OK;
    COMPARISON c;
    float r, check;
    float v;
    for(v = -Pi + .00001f; v < Pi; v+= .4f) {
        r = XMScalarModAngle(v - (Pi + Pi));
        check = v;
        c = Compare(r,check);
        if(c > WITHIN10EPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, v - Pi - Pi, r, check, c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
        r = XMScalarModAngle(v + (Pi + Pi));
        check = v;
        c = Compare(r,check);
        if(c > WITHIN10EPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, v+Pi+Pi, r, check, c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }
    for(v = -100; v < 100; v += 10) {
        r = XMScalarModAngle(v);
        check = v - ((floorf((v + Pi) / (Pi + Pi))) * (Pi + Pi));
        c = Compare(r,check);
        if(c > WITHIN4096) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, v, r, check, c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }
    return ret;
}

HRESULT Test172(LogProxy* pLog)
{
//XMScalarNearEqual 
    static const float f[] =     { 0,0,0,        5,5,5, -.23f,-.23f,-.27f };
    static const float f2[] =    { 0,.125,.125, -5,5.5,5.625,  .23f,-.25f,-.25f};
    static const float e[] =     { .125,.125,.124f, .5f,.5f,.5f, .025f,.025f,.025f};
    static const BOOL check[] =  { TRUE,TRUE,FALSE,FALSE,TRUE,FALSE,FALSE,TRUE,TRUE};
    HRESULT ret = S_OK;
    for(int i = 0; i < countof(f); i++) {
        BOOL r = XMScalarNearEqual(f[i],f2[i],e[i]);
        if (r != check[i]) {
            printe("%s: %f %f %f = %s ... %s\n",
                TestName, f[i],f2[i],e[i],r?"TRUE":"FALSE",check[i]?"TRUE":"FALSE");
            ret = MATH_FAIL;
        } else {
            printi("%s: 0\n", TestName);
        }
    }

    return ret;
}

HRESULT Test173(LogProxy* pLog)
{
//XMScalarSin
    COMPARISON c;
    HRESULT ret = S_OK;
    for(float f = -Pi; f < Pi; f += .031415926535897932384626f) {
        float r = XMScalarSin(f);
        float check = sinf(f);
        c = Compare(r,check);
        if(c > WITHIN10EPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    for(float f = -100; f < 100; f += 3.61f) {
        float r = XMScalarSin(f);
        float check = sinf(f);
        c = Compare(r,check);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    for( float f = UC_START; f <= UC_END; f += UC_STEP ) {
        float r = XMScalarSin(f);
        float check = sinf(f);
        c = Compare(r,check);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
            TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test174(LogProxy* pLog)
{
//XMScalarSinCos
    COMPARISON c;
    HRESULT ret = S_OK;
    for(float f = -Pi; f < Pi; f += .031415926535897932384626f) {
        float rc,rs;
        XMScalarSinCos(&rs, &rc, f);
        float checks = sinf(f);
        float checkc = cosf(f);
        c = Compare(rc,checkc);
        if(c > WITHIN10EPSILON) {
            printe ("%s: (c) %f = %f ... %f (%d)\n",
                TestName, f,rc,checkc,c);
            ret = MATH_FAIL;
        } 
        c = Compare(rs,checks);
        if(c > WITHIN10EPSILON) {
            printe ("%s: (s) %f = %f ... %f (%d)\n",
                TestName, f,rs,checks,c);
            ret = MATH_FAIL;
        }
        printi ("%s: %d\n", TestName, c);
    }

    for(float f = -100; f < 100; f += 3.61f) {
        float rc, rs;
        XMScalarSinCos(&rs, &rc, f);
        float checks = sinf(f);
        float checkc = cosf(f);
        c = Compare(rc,checkc);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: (c) %f = %f ... %f (%d)\n",
                TestName, f,rc,checkc,c);
            ret = MATH_FAIL;
        } 
        c = Compare(rs,checks);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: (s) %f = %f ... %f (%d)\n",
                TestName, f,rs,checks,c);
            ret = MATH_FAIL;
        }
        printi ("%s: %d\n", TestName, c);
    }

    for( float f = UC_START; f <= UC_END; f += UC_STEP ) {
        float rc, rs;
        XMScalarSinCos(&rs, &rc, f);
        float checks = sinf(f);
        float checkc = cosf(f);
        c = Compare(rc,checkc);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: (c) %f = %f ... %f (%d)\n",
            TestName, f,rc,checkc,c);
            ret = MATH_FAIL;
        } 
        c = Compare(rs,checks);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: (s) %f = %f ... %f (%d)\n",
                TestName, f,rs,checks,c);
                ret = MATH_FAIL;
        }
        printi ("%s: %d\n", TestName, c);
    }
    return ret;
}
HRESULT Test175(LogProxy* pLog)
{
//XMScalarSinCosEst
    COMPARISON c;
    HRESULT ret = S_OK;
    for(float f = -Pi; f < Pi; f += .031415926535897932384626f) {
        float rc,rs;
        XMScalarSinCosEst(&rs, &rc, f);
        float checks = sinf(f);
        float checkc = cosf(f);
        c = Compare(rc,checkc);
        if(c > WITHIN4096) {
            printe ("%s: (c) %f = %f ... %f (%d)\n",
                TestName, f,rc,checkc,c);
            ret = MATH_FAIL;
        } 
        c = Compare(rs,checks);
        if(c > WITHIN4096) {
            printe ("%s: (s) %f = %f ... %f (%d)\n",
                TestName, f,rs,checks,c);
            ret = MATH_FAIL;
        }
        printi ("%s: %d\n", TestName, c);
    }

    for(float f = -100; f < 100; f += 3.61f) {
        float rc, rs;
        XMScalarSinCosEst(&rs, &rc, f);
        float checks = sinf(f);
        float checkc = cosf(f);
        c = Compare(rc,checkc);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: (c) %f = %f ... %f (%d)\n",
                TestName, f,rc,checkc,c);
            ret = MATH_FAIL;
        } 
        c = Compare(rs,checks);
        if(c > WITHIN4096) {
            printe ("%s: (s) %f = %f ... %f (%d)\n",
                TestName, f,rs,checks,c);
            ret = MATH_FAIL;
        }
        printi ("%s: %d\n", TestName, c);
    }

    for( float f = UC_START; f <= UC_END; f += UC_STEP ) {
        float rc, rs;
        XMScalarSinCosEst(&rs, &rc, f);
        float checks = sinf(f);
        float checkc = cosf(f);
        c = Compare(rc,checkc);
        if(c > WITHIN4096) {
            printe ("%s: (c) %f = %f ... %f (%d)\n",
            TestName, f,rc,checkc,c);
            ret = MATH_FAIL;
        } 
        c = Compare(rs,checks);
        if(c > WITHIN4096) {
            printe ("%s: (s) %f = %f ... %f (%d)\n",
                TestName, f,rs,checks,c);
                ret = MATH_FAIL;
        }
        printi ("%s: %d\n", TestName, c);
    }
    return ret;
}
HRESULT Test176(LogProxy* pLog)
{
//XMScalarSinEst
    COMPARISON c;
    HRESULT ret = S_OK;
    for(float f = -Pi; f < Pi; f += .031415926535897932384626f) {
        float r = XMScalarSinEst(f);
        float check = sinf(f);
        c = Compare(r,check);
        if(c > WITHIN4096) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    for(float f = -100; f < 100; f += 3.61f) {
        float r = XMScalarSinEst(f);
        float check = sinf(f);
        c = Compare(r,check);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
                TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }

    for( float f = UC_START; f <= UC_END; f += UC_STEP ) {
        float r = XMScalarSinEst(f);
        float check = sinf(f);
        c = Compare(r,check);
        if(c > WITHINBIGEPSILON) {
            printe ("%s: %f = %f ... %f (%d)\n",
            TestName, f,r,check,c);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %d\n", TestName, c);
        }
    }
    return ret;
}
HRESULT Test177(LogProxy* pLog)
{
//XMStoreColor

//
// Using a pattern of 0.0f,0.0f,0.0f,0.0f, write it
// on uint32_t aligned boundries
//

    uint8_t b[64];
    HRESULT ret = S_OK;
    int offset;
    for(offset = 16; offset <= 32; offset += 4) {
        int i;
        for(i=0; i < sizeof(b); i++) {
            b[i] = (uint8_t)(~i & 0xff);
        }
        XMStoreColor(reinterpret_cast<XMCOLOR*>(&b[offset]),XMVectorZero());
        // Test the buffer for the out of bounds writing
        for(i = 0; i < sizeof(b); i++) {
            if (i == offset) {
                i += 3;
                continue;
            }
            if (b[i] != (uint8_t)(~i & 0xff)) {
                printe("%s: (%d) corrupted byte %d: %x ... %x\n",
                    TestName, offset, i, b[i], (uint8_t)(~i & 0xff));
                ret = MATH_FAIL;
            }
        }
        // Test for the control value (Zero)
        // Because all values are zero, there is no issue with endian
        for(i = offset; i < offset + 4; i++) {
            if (b[i] != 0) {
                printe("%s: (%d) corrupted color(%d): %x ... 0\n",
                    TestName, offset, i-offset, b[i]);
                ret = MATH_FAIL;
            }
        }
    }

    // Test the value conversion.
    // x = round up test
    // y = simple 2.0f test
    // z = round down test
    // w = control value (Must be highest since it's what's used to stop iterations)
    static const XMVECTORF32 vTestStart = {0.6f,2.0f,1.4f,3.0f};
    // Load the control value
    XMVECTOR v = vTestStart;
    // Endian correct expected answer
    uint32_t u = 0x03010201;
    // Step value
    XMVECTOR adder = XMVectorReplicate(3.f);
    // Due to floating point inaccuracy, perform a division before use
    XMVECTOR divisor = XMVectorReplicate(1.0f/255.0f);

    offset = 16;		// uint32_t aligned
    // Test W for exit of loop.
    for(; XMVectorGetW(v) <= 255.0f; v += adder, u += 0x03030303) {
        // Take the value, divide by 255 to yield the 0.0f to 1.0f range
        XMStoreColor((XMCOLOR*)&b[offset],XMVectorMultiply(v,divisor));
        // Pull the 32-bit value from memory (Endian is native)
        uint32_t r = reinterpret_cast<const uint32_t *>(&b[offset])[0];
        // Expected?
        if (r != u) {
            printe ("%s: (%d) %f %f %f %f = 0x%x ... 0x%x\n",
                TestName, offset, XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v), r,u);
            ret = MATH_FAIL;
        } else {
            printi ("%s: %x\n", TestName, r);
        }
        // Step by 4 (Retain uint32_t alignment)
        offset += 4;
        // Prevent buffer overrun
        if (offset == 32) {
            offset = 16;
        }
    }
    return ret;
}

HRESULT Test178(LogProxy* pLog)
{
//XMStoreFloat2 / XMStoreInt2 / XMStoreSInt2 / XMStoreUInt2

    char c[64];
    int offset = 16;
    int i, j; 
    XMVECTORF32 v = {{1,2,3,4}};
    static const XMVECTORF32 v1 = {{4,4,4,4}};
    HRESULT r = S_OK;

    const int floatcount = 2;

    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        XMStoreFloat2((XMFLOAT2*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            float f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != XMVectorGetByIndex(v,i)) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,XMVectorGetByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        v.v = XMVectorAdd(v.v, v1);
    }

    const int intcount = 2;

    uint32_t vi[4] = {1,2,3,4};         // Can't be const
    static const uint32_t vi1[4] = {4,4,4,4};
    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        v.v = XMVectorSetInt( vi[0], vi[1], vi[2], vi[3] );
        XMStoreInt2((uint32_t*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++) {
            uint32_t ii = ReadInt((char*)&(c[first+(i*4)]));
            if(ii != XMVectorGetIntByIndex(v,i)) {
                printe("%s: %d corrupted int %d: %d ... %d\n", TestName, j,i,ii,XMVectorGetIntByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        vi[0] += vi1[0];
        vi[1] += vi1[1];
        vi[2] += vi1[2];
        vi[3] += vi1[3];
    }

    static const XMVECTORF32 vsint[] = { { 1.f, 1.f, _Q_NAN, _Q_NAN }, { 0.f, 0.f, _Q_NAN, _Q_NAN }, { -1.f, -1.f, _Q_NAN, _Q_NAN }, { 2147483392.f, -1.f, _Q_NAN, _Q_NAN } };
    int32_t sint[] = { 1, 1, 0, 0, -1, -1, 0x7FFFFF00, -1 };
    for( i=0; i < std::size(vsint); ++i )
    {
        XMINT2 x;
        XMStoreSInt2( &x, vsint[i] );

        if ( sint[i*intcount] != x.x
             || sint[i*intcount+1] != x.y )
        {
            printe ("%s: SINT %d = %f %f = %d %d ... %d %d\n",
                    TestName, i, XMVectorGetX(vsint[i]), XMVectorGetY(vsint[i]), x.x, x.y, sint[i*intcount], sint[i*intcount+1] );
            r = MATH_FAIL;
        }
    }

    static const XMVECTORF32 vuint[] = { { 1.f, 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, { 54575344.f, 2195727104.f, 1211756544.f, 2207119360.f }, { 2147483392.f, 4294967040.f, 134217728.f, 522133280.f } };
    uint32_t uint[] = { 1, 1, 0, 0, 0x0340c0f0, 0x82e02300, 0x7FFFFF00, 0xFFFFFF00 };
    for( i=0; i < std::size(vuint); ++i )
    {
        XMUINT2 x;
        XMStoreUInt2( &x, vuint[i] );

        if ( uint[i*intcount] != x.x
             || uint[i*intcount+1] != x.y )
        {
            printe ("%s: SINT %d = %f %f = %u %u ... %u %u\n",
                    TestName, i, XMVectorGetX(vuint[i]), XMVectorGetY(vuint[i]), x.x, x.y, uint[i*intcount], uint[i*intcount+1] );
            r = MATH_FAIL;
        }
    }

    return r;
}
HRESULT Test179(LogProxy* pLog)
{
// XMStoreFloat2A / XMStoreInt2A

    XM_ALIGNED_DATA(16) char c[64];
    int offset = 16;
    int i, j; 
    XMVECTORF32 v = {{1,2,3,4}};
    XMVECTORF32 v1 = {{4,4,4,4}};
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int floatcount = 2;
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        XMStoreFloat2A((XMFLOAT2A*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            float f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != XMVectorGetByIndex(v,i)) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,XMVectorGetByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        v.v = XMVectorAdd(v.v, v1);
    }

    uint32_t vi[4] = {1,2,3,4};
    static const uint32_t vi1[4] = {4,4,4,4};
    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int intcount = 2;
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        v.v = XMVectorSetInt( vi[0], vi[1], vi[2], vi[3] );
        XMStoreInt2A((uint32_t*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++) {
            uint32_t ii = ReadInt((char*)&(c[first+(i*4)]));
            if(ii != XMVectorGetIntByIndex(v,i)) {
                printe("%s: %d corrupted int %d: %d ... %d\n", TestName, j,i,ii,XMVectorGetIntByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        vi[0] += vi1[0];
        vi[1] += vi1[1];
        vi[2] += vi1[2];
        vi[3] += vi1[3];
    }

    return r;
}
HRESULT Test180(LogProxy* pLog)
{
//XMStoreFloat3 / XMStoreInt3 / XMStoreSInt3 / XMStoreUInt3

    char c[64];
    int offset = 16;
    int i, j; 
    XMVECTORF32 v = {{1,2,3,4}};
    XMVECTORF32 v1 = {{4,4,4,4}};
    HRESULT r = S_OK;

    const int floatcount = 3;

    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        XMStoreFloat3((XMFLOAT3*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            float f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != XMVectorGetByIndex(v,i)) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,XMVectorGetByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        v.v = XMVectorAdd(v.v, v1);
    }

    const int intcount = 3;

    uint32_t vi[4] = {1,2,3,4};
    static const uint32_t vi1[4] = {4,4,4,4};

    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
    
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        v.v = XMVectorSetInt( vi[0], vi[1], vi[2], vi[3] );
        XMStoreInt3((uint32_t*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++) {
            uint32_t ii = ReadInt((char*)&(c[first+(i*4)]));
            if(ii != XMVectorGetIntByIndex(v,i)) {
                printe("%s: %d corrupted int %d: %d ... %d\n", TestName, j,i,ii,XMVectorGetIntByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        vi[0] += vi1[0];
        vi[1] += vi1[1];
        vi[2] += vi1[2];
        vi[3] += vi1[3];
    }

    static const XMVECTORF32 vsint[] = { { 1.f, 1.f, 1.f, _Q_NAN }, { 0.f, 0.f, 0.f, _Q_NAN }, { -1.f, -1.f, -1.f, _Q_NAN }, { 2147483392.f, -1.f, 134217728.f, _Q_NAN } };
    int32_t sint[] = { 1, 1, 1, 0, 0, 0, -1, -1, -1, 0x7FFFFF00, -1, 0x8000000 };
    for( i=0; i < std::size(vsint); ++i )
    {
        XMINT3 x;
        XMStoreSInt3( &x, vsint[i] );

        if ( sint[i*intcount] != x.x
             || sint[i*intcount+1] != x.y
             || sint[i*intcount+2] != x.z )
        {
            printe ("%s: SINT %d = %f %f %f = %d %d %d ... %d %d %d\n",
                    TestName, i, XMVectorGetX(vsint[i]), XMVectorGetY(vsint[i]), XMVectorGetZ(vsint[i]), 
                    x.x, x.y, x.z, sint[i*intcount], sint[i*intcount+1], sint[i*intcount+2] );
            r = MATH_FAIL;
        }
    }

    static const XMVECTORF32 vuint[] = { { 1.f, 1.f, 1.f, _Q_NAN }, { 0.f, 0.f, 0.f, _Q_NAN }, { 54575344.f, 2195727104.f, 1211756544.f, _Q_NAN }, { 2147483392.f, 4294967040.f, 134217728.f, _Q_NAN } };
    static uint32_t uint[] = { 1, 1, 1, 0, 0, 0, 0x0340c0f0, 0x82e02300, 0x4839f000, 0x7FFFFF00, 0xFFFFFF00, 0x8000000 };
    for( i=0; i < std::size(vuint); ++i )
    {
        XMUINT3 x;
        XMStoreUInt3( &x, vuint[i] );

        if ( uint[i*intcount] != x.x
             || uint[i*intcount+1] != x.y
             || uint[i*intcount+2] != x.z )
        {
            printe ("%s: uint32_t %d = %f %f %f = %u %u %u ... %u %u %u\n",
                    TestName, i, XMVectorGetX(vuint[i]), XMVectorGetY(vuint[i]), XMVectorGetZ(vuint[i]), 
                    x.x, x.y, x.z, uint[i*intcount], uint[i*intcount+1], uint[i*intcount+2] );
            r = MATH_FAIL;
        }
    }

    return r;
}

HRESULT Test181(LogProxy* pLog)
{
//XMStoreFloat3A / XMStoreInt3A

    XM_ALIGNED_DATA(16) char c[64];
    int offset = 16;
    float f;
    int i, j; 
    XMVECTORF32 v = {{1,2,3,4}};
    XMVECTORF32 v1 = {{4,4,4,4}};
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int floatcount = 3;
        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        XMStoreFloat3A((XMFLOAT3A*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != XMVectorGetByIndex(v,i)) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,XMVectorGetByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        v.v = XMVectorAdd(v.v, v1);
    }

    uint32_t vi[4] = {1,2,3,4};
    static const uint32_t vi1[4] = {4,4,4,4};
    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int intcount = 3;
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        v.v = XMVectorSetInt( vi[0], vi[1], vi[2], vi[3] );
        XMStoreInt3A((uint32_t*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++) {
            uint32_t ii = ReadInt((char*)&(c[first+(i*4)]));
            if(ii != XMVectorGetIntByIndex(v,i)) {
                printe("%s: %d corrupted int %d: %d ... %d\n", TestName, j,i,ii,XMVectorGetIntByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        vi[0] += vi1[0];
        vi[1] += vi1[1];
        vi[2] += vi1[2];
        vi[3] += vi1[3];
    }

    return r;
}

HRESULT Test182(LogProxy* pLog)
{
//XMStoreFloat3x3

    char c[112];
    int floatcount = 9;
    int offset = 16;
    float f;
    int i, j; 
    XMMATRIX m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    static const XMMATRIX m1(16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16);
    HRESULT r = S_OK;

    //XMStoreFloat3x3
    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        XMStoreFloat3x3((XMFLOAT3X3*)&c[offset + j], m);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            float mf = XMVectorGetByIndex( m.r[i / 3], i % 3 );
            if(f != mf) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,mf);
                r=MATH_FAIL;
            }
        }
        m.r[0] = XMVectorAdd(m.r[0], m1.r[0]);
        m.r[1] = XMVectorAdd(m.r[1], m1.r[1]);
        m.r[2] = XMVectorAdd(m.r[2], m1.r[2]);
        m.r[3] = XMVectorAdd(m.r[3], m1.r[3]);
    }

    return r;
}

HRESULT Test183(LogProxy* pLog)
{
//XMStoreFloat4 / XMStoreInt4 / XMStoreSInt4 / XMStoreUInt4

    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!!!!!!!!!!!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);

    int offset = 0;
    const int floatcount = 4;
    intptr_t i, j; 
    XMVECTORF32 v = {{1,2,3,4}};
    XMVECTORF32 v1 = {{4,4,4,4}};
    HRESULT r = S_OK;

    for(j = pc64k - 16; j <= pc64k + 16; j += 4) {
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+   16;

        XMStoreFloat4((XMFLOAT4*)&c[offset + j], v);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        int index;
        for(index = 0; index < floatcount; index++) {
            float f = ReadFloat((char*)&(c[first+(index*4)]));
            if(f != XMVectorGetByIndex(v,index)) {
                printe("%s: %p corrupted float %x: %f ... %f\n", TestName, reinterpret_cast<void*>(j), index,f,XMVectorGetByIndex(v,index));
                r=MATH_FAIL;
            }
        }
        v.v = XMVectorAdd(v.v, v1);
    }

    const int intcount = 4;

    static const XMVECTORF32 vsint[] = { { 1.f, 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, { -1.f, -1.f, -1.f, -1.f }, { 2147483392.f, -1.f, 134217728.f, 522133280.f } };
    int32_t sint[] = { 1, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -1, 0x7FFFFF00, -1, 0x8000000, 0x1F1F1F20 };
    for( i=0; i < std::size(vsint); ++i )
    {
        XMINT4 x;
        XMStoreSInt4( &x, vsint[i] );

        if ( sint[i*intcount] != x.x
             || sint[i*intcount+1] != x.y
             || sint[i*intcount+2] != x.z
             || sint[i*intcount+3] != x.w )
        {
            printe ("%s: SINT %p = %f %f %f %f = %d %d %d %d ... %d %d %d %d\n",
                    TestName, reinterpret_cast<void*>(i), XMVectorGetX(vsint[i]), XMVectorGetY(vsint[i]), XMVectorGetZ(vsint[i]), XMVectorGetW(vsint[i]), 
                    x.x, x.y, x.z, x.w, sint[i*intcount], sint[i*intcount+1], sint[i*intcount+2], sint[i*intcount+3] );
            r = MATH_FAIL;
        }
    }

    static const XMVECTORF32 vuint[] = { { 1.f, 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f, 0.f }, { 54575344.f, 2195727104.f, 1211756544.f, 2207119360.f }, { 2147483392.f, 4294967040.f, 134217728.f, 522133280.f } };
    uint32_t uint[] = { 1, 1, 1, 1, 0, 0, 0, 0, 0x0340c0f0, 0x82e02300, 0x4839f000, 0x838df800, 0x7FFFFF00, 0xFFFFFF00, 0x8000000, 0x1F1F1F20 };
    for( i=0; i < std::size(vuint); ++i )
    {
        XMUINT4 x;
        XMStoreUInt4( &x, vuint[i] );

        if ( uint[i*intcount] != x.x
             || uint[i*intcount+1] != x.y
             || uint[i*intcount+2] != x.z
             || uint[i*intcount+3] != x.w )
        {
            printe ("%s: uint32_t %p = %f %f %f %f = %u %u %u %u ... %u %u %u %u\n",
                    TestName, reinterpret_cast<void*>(i), XMVectorGetX(vuint[i]), XMVectorGetY(vuint[i]), XMVectorGetZ(vuint[i]), XMVectorGetW(vuint[i]), 
                    x.x, x.y, x.z, x.w, uint[i*intcount], uint[i*intcount+1], uint[i*intcount+2], uint[i*intcount+3] );
            r = MATH_FAIL;
        }
    }

    free(c);
    return r;
}

HRESULT Test184(LogProxy* pLog)
{
// XMStoreFloat4A / XMStoreInt4A

    XM_ALIGNED_DATA(16) char c[64];
    int offset = 16;
    float f;
    int i, j; 
    XMVECTORF32 v = {{1,2,3,4}};
    static const XMVECTORF32 v1 = {{4,4,4,4}};
    HRESULT r = S_OK;

    for(j = 0; j < 16; j += 16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+   16;

        XMStoreFloat4A((XMFLOAT4A*)&c[offset + j], v);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < 4; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            if(f != XMVectorGetByIndex(v,i)) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,XMVectorGetByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        v.v = XMVectorAdd(v.v, v1);
    }

    uint32_t vi[4] = {1,2,3,4};     // Can't be const
    static const uint32_t vi1[4] = {4,4,4,4};
    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int intcount = 4;
        int first = offset+j;
        int last  = offset+j+ 4 * intcount;

        v.v = XMVectorSetInt( vi[0], vi[1], vi[2], vi[3] );
        XMStoreInt4A((uint32_t*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < intcount; i++) {
            uint32_t ii = ReadInt((char*)&(c[first+(i*4)]));
            if(ii != XMVectorGetIntByIndex(v,i)) {
                printe("%s: %d corrupted int %d: %d ... %d\n", TestName, j,i,ii,XMVectorGetIntByIndex(v,i));
                r=MATH_FAIL;
            }
        }
        vi[0] += vi1[0];
        vi[1] += vi1[1];
        vi[2] += vi1[2];
        vi[3] += vi1[3];
    }

    return r;
}

HRESULT Test185(LogProxy* pLog)
{
//XMStoreFloat4x3 (row-major)

    char c[112];
    const int floatcount = 12;
    const int offset = 16;
    int i, j; 
    XMMATRIX m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    static const XMMATRIX m1(16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16);
    HRESULT r = S_OK;

    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        XMStoreFloat4x3(reinterpret_cast<XMFLOAT4X3*>(&c[offset + j]), m);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            float f = ReadFloat((char*)&(c[first+(i*4)]));
            float mf = XMVectorGetByIndex( m.r[i / 3], i % 3 );
            if(f != mf) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,mf);
                r=MATH_FAIL;
            }
        }
        m.r[0] = XMVectorAdd(m.r[0], m1.r[0]);
        m.r[1] = XMVectorAdd(m.r[1], m1.r[1]);
        m.r[2] = XMVectorAdd(m.r[2], m1.r[2]);
        m.r[3] = XMVectorAdd(m.r[3], m1.r[3]);
    }

    return r;
}

HRESULT Test520(LogProxy* pLog)
{
    //XMStoreFloat3x4 (column-major) 

    char c[112];
    const int floatcount = 12;
    const int offset = 16;
    int i, j;
    XMMATRIX m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    static const XMMATRIX m1(16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16);
    HRESULT r = S_OK;

    for (j = 0; j < 16; j += 4) {
        for (i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset + j;
        int last = offset + j + 4 * floatcount;

        XMStoreFloat3x4(reinterpret_cast<XMFLOAT3X4*>(&c[offset + j]), m);

        for (i = 0; i < first; i++) {
            if (c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for (i = last; i < sizeof(c); i++) {
            if (c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        static const size_t transpose[12] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14 };

        for (i = 0; i < floatcount; i++) {
            float f = ReadFloat((char*)&(c[first + (i * 4)]));
            size_t index = transpose[i];
            float mf = XMVectorGetByIndex(m.r[index / 4], index % 4);
            if (f != mf) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j, i, f, mf);
                r = MATH_FAIL;
            }
        }
        m.r[0] = XMVectorAdd(m.r[0], m1.r[0]);
        m.r[1] = XMVectorAdd(m.r[1], m1.r[1]);
        m.r[2] = XMVectorAdd(m.r[2], m1.r[2]);
        m.r[3] = XMVectorAdd(m.r[3], m1.r[3]);
    }

    return r;
}

HRESULT Test186(LogProxy* pLog)
{
//XMStoreFloat4x4
    char c[112];
    int floatcount = 16;
    int offset = 16;
    float f;
    int i, j; 
    XMMATRIX m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    static const XMMATRIX m1(16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16);
    HRESULT r = S_OK;

    //XMStoreFloat4x4
    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        XMStoreFloat4x4((XMFLOAT4X4*)&c[offset + j], m);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            float mf = XMVectorGetByIndex( m.r[i / 4], i % 4 );
            if(f != mf) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,mf);
                r=MATH_FAIL;
            }
        }
        m.r[0] = XMVectorAdd(m.r[0], m1.r[0]);
        m.r[1] = XMVectorAdd(m.r[1], m1.r[1]);
        m.r[2] = XMVectorAdd(m.r[2], m1.r[2]);
        m.r[3] = XMVectorAdd(m.r[3], m1.r[3]);
    }

    return r;
}

HRESULT Test187(LogProxy* pLog)
{
// XMStoreFloat4x4A

    XM_ALIGNED_DATA(16) char c[112];
    int floatcount = 16;
    int offset = 16;
    float f;
    int i, j; 
    XMMATRIX m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    static const XMMATRIX m1(16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16);
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4 * floatcount;

        XMStoreFloat4x4A((XMFLOAT4X4A*)&c[offset + j], m);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            float mf = XMVectorGetByIndex( m.r[i / 4], i % 4 );
            if(f != mf) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,mf);
                r=MATH_FAIL;
            }
        }
        m.r[0] = XMVectorAdd(m.r[0], m1.r[0]);
        m.r[1] = XMVectorAdd(m.r[1], m1.r[1]);
        m.r[2] = XMVectorAdd(m.r[2], m1.r[2]);
        m.r[3] = XMVectorAdd(m.r[3], m1.r[3]);
    }
    return r;
}

HRESULT Test188(LogProxy* pLog)
{
//XMStoreHalf2 
    const int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!!!!!!!!!!!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    const int floatsize = 2;

    int offset = 0;
    const int floatcount = 2;
    HALF f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const XMVECTORU32 v = {0x40400000+(0<<13),0x40400000+(1<<13),0x40400000+(2<<13),0x40400000+(3<<13)};
    static const HALF check[4] = {0x4200,0x4201,0x4202,0x4203};
    
    for(j = pc64k - 16; j <= pc64k + 16; j += 4) {
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+floatsize*floatcount;

        XMStoreHalf2((XMHALF2*)&c[offset + j], v);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        int index;
        for(index = 0; index < floatcount; index++) {
            f = *((const HALF*)&(c[first+(index*floatsize)]));
            if(f != check[index]) {
                printe("%s: %p corrupted half %x: %x ... %x\n", TestName, reinterpret_cast<void*>(j),index,f,check[index]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test189(LogProxy* pLog)
{
//XMStoreHalf4 
    const int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!!!!!!!!!!!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    int floatsize = 2;

    int offset = 0;
    int floatcount = 4;
    HALF f;
    int k;
    intptr_t i, j; 
    XMVECTORF32 v={};
    XMVECTORF32 check={};
    HRESULT r = S_OK;

    for(k = 0; k < 4; k++) {
        v.v = XMVectorSetIntByIndex(v,0x40400000 + (k << 13),k);
        check.v = XMVectorSetIntByIndex(check,0x4200 + k,k);
    }

    for(j = pc64k - 16; j <= pc64k + 16; j += 4) {
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+floatsize*floatcount;

        XMStoreHalf4((XMHALF4*)&c[offset + j], v);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        int index;
        for(index = 0; index < floatcount; index++) {
            f = *((const HALF*)&(c[first+(index*floatsize)]));
            if(f != XMVectorGetIntByIndex(check,index)) {
                printe("%s: %p corrupted half %x: %x ... %x\n", TestName, reinterpret_cast<void*>(j),index,f,XMVectorGetIntByIndex(check,index));
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test190(LogProxy* pLog)
{
//XMStoreXDecN4 
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!!!!!!!!!!!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    int floatsize = 4;

    int offset = 0;
    int floatcount = 1;
    XMXDECN4 f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint32_t check[] = {0,0x1ff+(0x1ff<<10)+(0x1ff<<20)+(0x3U<<30),0x201+(0x201<<10)+(0x201<<20)+(0x3U<<30),0x0+(0x1ff<<10)+(0x201<<20)+(0x0<<30)};
    static const XMVECTORF32 v[] = {{0,0,0,0},{1,1,1,1},{-1,-1,-1,1},{0,1,-1,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j += 4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+floatsize*floatcount;

        XMStoreXDecN4((XMXDECN4*)&c[offset + j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < floatcount; i++) {
            f = *((XMXDECN4*)&(c[first+(i * floatsize)]));
            if(f != check[n]) {
                printe("%s: %p corrupted float %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i), *((const uint32_t*)&f),check[n]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test191(LogProxy* pLog)
{
//XMStoreShortN2 
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    int datasize = 2;

    int offset = 0;
    int datacount = 2;
    uint16_t value;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint16_t check[4][4] = {{0,0,0,0},{0x8001,0x8001,0x8001,0x8001},{0x7fff,0x7fff,0x7fff,0x7fff},{0x8001,0x7fff,0x0000,0x8001}};
    static const XMVECTORF32 v[] = {{0,0,0,0},{-1,-1,-1,-1},{1,1,1,1},{-1,1,0,-1}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+datasize*datacount;

        XMStoreShortN2((XMSHORTN2*)&c[offset + j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < datacount; i++) {
            value = *((const uint16_t*)&(c[first+(i * datasize)]));
            if(value != check[n][i]) {
                printe("%s: %p corrupted short %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),*((const uint16_t*)&value),check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test192(LogProxy* pLog)
{
//XMStoreShortN4 
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    int datasize = 2;

    int offset = 0;
    int datacount = 4;
    uint16_t value;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint16_t check[4][4] = {{0,0,0,0},{0x8001,0x8001,0x8001,0x8001},{0x7fff,0x7fff,0x7fff,0x7fff},{0x8001,0x7fff,0x0000,0x8001}};
    static const XMVECTORF32 v[] = {{0,0,0,0},{-1,-1,-1,-1},{1,1,1,1},{-1,1,0,-1}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j += 4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+datasize*datacount;

        XMStoreShortN4((XMSHORTN4*)&c[offset + j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < datacount; i++) {
            value = *((const uint16_t*)&(c[first+(i * datasize)]));
            if(value != check[n][i]) {
                printe("%s: %p corrupted short %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),*((const uint16_t*)&value),check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test496(LogProxy* pLog)
{
//XMPlaneNearEqual
    XMVECTORF32 r1 = {1,2,3,4};
    XMVECTORF32 r2 = {1,2,2.9f,4};
    XMVECTORF32 e = {.02f, .02f, .02f, .02f};
    HRESULT r = S_OK;
    if(XMPlaneNearEqual(r1, r2, e)) {
        printe("%s: ({%f,%f,%f,%f}, {%f,%f,%f,%f},{%f}) = %d ... %d",
        TestName, XMVectorGetX(r1), XMVectorGetY(r1),XMVectorGetZ(r1),XMVectorGetW(r1),
        XMVectorGetX(r2),XMVectorGetY(r2),XMVectorGetZ(r2),XMVectorGetW(r2),XMVectorGetX(e), FALSE, TRUE);
        r = MATH_FAIL;
    }
    e.v *= 4;
    r1.v = XMPlaneNormalize(r1);
    r2.v = XMPlaneNormalize(r2);
    if(!XMPlaneNearEqual(r1,r2,e)) {
        printe("%s: ({%f,%f,%f,%f}, {%f,%f,%f,%f},{%f}) = %d ... %d",
        TestName,XMVectorGetX(r1),XMVectorGetY(r1),XMVectorGetZ(r1),XMVectorGetW(r1),
        XMVectorGetX(r2),XMVectorGetY(r2),XMVectorGetZ(r2),XMVectorGetW(r2),XMVectorGetX(e), TRUE, FALSE);
        r = MATH_FAIL;
    }
    return r;
}

HRESULT Test497(LogProxy* pLog)
{
//XMLoadFloat4x3A (row-major)
    static_assert( sizeof(XMFLOAT4X3A) == 4*12, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT4X3A>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT4X3A>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT4X3A>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT4X3A>::value, "Move Assign.");

    XMMATRIX m;
    HRESULT ret = S_OK;

    {
        static const XMFLOAT4X3A s4x3 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        static const XMMATRIX check = { 1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 10, 11, 12, 1 };

        m = XMLoadFloat4x3A(&s4x3);

        COMPARISON c = CompareXMMATRIX(m, check);
        if (c != EXACT)
        {
            printe("%s: (%d)\n", TestName, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    {
        XM_ALIGNED_DATA(16) char c[112];
        const int floatcount = 12;
        const int offset = 16;
        int i, j;

        for (j = 0; j < 16; j += 16) {
            for (i = 0; i < sizeof(c); i++) {
                c[i] = (char)(~i & 0xff);
            }
            int first = offset + j;
            int last = offset + j + 4 * floatcount;

            for (i = 0; i < floatcount; i++) {
                WriteFloat((float)i, (char*)&c[first + (i * 4)]);
            }

            m = XMLoadFloat4x3A(reinterpret_cast<const XMFLOAT4X3A*>(&c[offset + j]));

            for (i = 0; i < first; i++) {
                if (c[i] != (char)(~i & 0xff)) {
                    printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                    ret = MATH_FAIL;
                }
            }
            for (i = last; i < sizeof(c); i++) {
                if (c[i] != (char)(~i & 0xff)) {
                    printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                    ret = MATH_FAIL;
                }
            }
            for (i = 0; i < floatcount; i++) {
                float f = ReadFloat((char*)&(c[first + (i * 4)]));
                float check = (float)i;
                float mf = XMVectorGetByIndex(m.r[i / 3], i % 3);
                if (f != check) {
                    printe("%s: %d corrupted source float %d: %f ... %f\n", TestName, j, i, f, check);
                    ret = MATH_FAIL;
                }
                if (mf != check) {
                    printe("%s: %d improperly read float %d: %f ... %f\n", TestName, j, i, mf, check);
                    ret = MATH_FAIL;
                }
            }
        }
    }

    return ret;
}

HRESULT Test519(LogProxy* pLog)
{
    //XMLoadFloat3x4A (column-major) 

    static_assert(sizeof(XMFLOAT3X4A) == 4 * 12, "Unexpected structure size");

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT3X4A>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT3X4A>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT3X4A>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT3X4A>::value, "Move Assign.");

    XMMATRIX m;
    HRESULT ret = S_OK;

    {
        static const XMFLOAT3X4A s3x4 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        static const XMMATRIX check = { 1, 5, 9, 0, 2, 6, 10, 0, 3, 7, 11, 0, 4, 8, 12, 1 };

        m = XMLoadFloat3x4A(&s3x4);

        COMPARISON c = CompareXMMATRIX(m, check);
        if (c != EXACT)
        {
            printe("%s: (%d)\n", TestName, c);
            printmatrixe(m); printe("...\n");
            printmatrixe(check);
            ret = MATH_FAIL;
        }
    }

    {
        XM_ALIGNED_DATA(16) char c[112];
        const int floatcount = 12;
        const int offset = 16;
        int i, j;

        for (j = 0; j < 16; j += 16) {
            for (i = 0; i < sizeof(c); i++) {
                c[i] = (char)(~i & 0xff);
            }
            int first = offset + j;
            int last = offset + j + 4 * floatcount;

            for (i = 0; i < floatcount; i++) {
                WriteFloat((float)i, (char*)&c[first + (i * 4)]);
            }

            m = XMLoadFloat3x4A(reinterpret_cast<const XMFLOAT3X4A*>(&c[offset + j]));

            for (i = 0; i < first; i++) {
                if (c[i] != (char)(~i & 0xff)) {
                    printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                    ret = MATH_FAIL;
                }
            }
            for (i = last; i < sizeof(c); i++) {
                if (c[i] != (char)(~i & 0xff)) {
                    printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j, i, c[i], (unsigned char)(~i));
                    ret = MATH_FAIL;
                }
            }

            static const size_t transpose[12] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14 };

            for (i = 0; i < floatcount; i++) {
                float f = ReadFloat((char*)&(c[first + (i * 4)]));
                float check = (float)i;
                size_t index = transpose[i];
                float mf = XMVectorGetByIndex(m.r[index / 4], index % 4);
                if (f != check) {
                    printe("%s: %d corrupted source float %d: %f ... %f\n", TestName, j, i, f, check);
                    ret = MATH_FAIL;
                }
                if (mf != check) {
                    printe("%s: %d improperly read float %d: %f ... %f\n", TestName, j, i, mf, check);
                    ret = MATH_FAIL;
                }
            }
        }
    }

    return ret;
}

HRESULT Test498(LogProxy* pLog)
{
//XMStoreFloat4x3A (row-major)

    XM_ALIGNED_DATA(16) char c[112];
    int floatcount = 12;
    int offset = 16;
    float f;
    int i, j; 
    XMMATRIX m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    static const XMMATRIX m1(16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16);
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=16) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;

        XMStoreFloat4x3A(reinterpret_cast<XMFLOAT4X3A*>(&c[offset + j]), m);
/*
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r =  MATH_FAIL;
            }
            }*/
        for(i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first+(i*4)]));
            float mf = XMVectorGetByIndex( m.r[i / 3], i % 3 );
            if(f != mf) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j,i,f,mf);
                r=MATH_FAIL;
            }
        }
        m.r[0] = XMVectorAdd(m.r[0], m1.r[0]);
        m.r[1] = XMVectorAdd(m.r[1], m1.r[1]);
        m.r[2] = XMVectorAdd(m.r[2], m1.r[2]);
        m.r[3] = XMVectorAdd(m.r[3], m1.r[3]);
    }
    return r;
}

HRESULT Test521(LogProxy* pLog)
{
    //XMStoreFloat3x4A (column-major) 

    XM_ALIGNED_DATA(16) char c[112];
    int floatcount = 12;
    int offset = 16;
    float f;
    int i, j;
    XMMATRIX m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    static const XMMATRIX m1(16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16);
    HRESULT r = S_OK;

    for (j = 0; j < 16; j += 16) {
        for (i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset + j;

        XMStoreFloat3x4A(reinterpret_cast<XMFLOAT3X4A*>(&c[offset + j]), m);

        static const size_t transpose[12] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14 };

        for (i = 0; i < floatcount; i++) {
            f = ReadFloat((char*)&(c[first + (i * 4)]));
            size_t index = transpose[i];
            float mf = XMVectorGetByIndex(m.r[index / 4], index % 4);
            if (f != mf) {
                printe("%s: %d corrupted float %d: %f ... %f\n", TestName, j, i, f, mf);
                r = MATH_FAIL;
            }
        }
        m.r[0] = XMVectorAdd(m.r[0], m1.r[0]);
        m.r[1] = XMVectorAdd(m.r[1], m1.r[1]);
        m.r[2] = XMVectorAdd(m.r[2], m1.r[2]);
        m.r[3] = XMVectorAdd(m.r[3], m1.r[3]);
    }
    return r;
}

HRESULT Test501(LogProxy* pLog)
{
//XMPlaneNormalizeEst 
    int i,k;
    HRESULT ret = S_OK;
    COMPARISON c;
    XMVECTORF32 v1={};
    XMVECTORF32 check={};
    float rcplength;

    for(k = 0; k < 10; k++) {
        v1.v = GetRandomVector16();
        if(k == 0) v1.v = XMVectorSetW(v1,_Q_NAN);
        rcplength = 1.f/sqrtf(XMVectorGetX(v1)*XMVectorGetX(v1) + XMVectorGetY(v1)*XMVectorGetY(v1) + XMVectorGetZ(v1)*XMVectorGetZ(v1));
        for(i = 0; i < 4; i++) {
            check.v = XMVectorSetByIndex(check,XMVectorGetByIndex(v1,i) * rcplength,i);
        }

        XMVECTOR r = XMPlaneNormalizeEst(v1);
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
    return ret;
}

HRESULT Test509(LogProxy* pLog)
{
//XMLoadByte4 
    static_assert( sizeof(XMBYTE4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMBYTE4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMBYTE4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMBYTE4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMBYTE4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMBYTE4 src;

    static const unsigned char s[4][4] = {{0,0,0,0},{0xff,0xff,0xff,0xff},{0x01,0x01,0x01,0x01},{0xff,0x01,0x00,0xff}};
    static const XMVECTORF32 check[] = {{0,0,0,0},{-1,-1,-1,-1},{1,1,1,1},{-1,1,0,-1}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; src.z=s[k][2]; src.w=s[k][3];
        v = XMLoadByte4(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        char a = (rand() + rand()) & 0xff;
        char b = (rand() + rand()) & 0xff;
        char c = (rand() + rand()) & 0xff;
        char d = (rand() + rand()) & 0xff;
        XMVECTORF32 chk= {{float(a), float(b), float(c), float(d)}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadByte4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHINEPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src),XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

// Special-case of -128
    {
        src.x = src.y = src.z = src.w = -128;
        XMVECTORF32 chk = { -128.f, -128.f, -128.f, -128.f };
        v = XMLoadByte4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test510(LogProxy* pLog)
{
//XMLoadByteN4 
    static_assert( sizeof(XMBYTEN4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMBYTEN4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMBYTEN4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMBYTEN4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMBYTEN4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMBYTEN4 src;

    static const unsigned char s[4][4] = {{0,0,0,0},{0x81,0x81,0x81,0x81},{0x7f,0x7f,0x7f,0x7f},{0x81,0x7f,0x0,0x81}};
    static const XMVECTORF32 check[] = {{0,0,0,0},{-1,-1,-1,-1},{1,1,1,1},{-1,1,0,-1}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; src.z=s[k][2]; src.w=s[k][3];
        v = XMLoadByteN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        char a = (rand() + rand()) & 0xff; if(a == -128) a = 0;
        char b = (rand() + rand()) & 0xff; if(b == -128) b = 0;
        char c = (rand() + rand()) & 0xff; if(c == -128) c = 0;
        char d = (rand() + rand()) & 0xff; if(d == -128) d = 0;
        XMVECTORF32 chk= {{a/127.0f, b/127.0f, c/127.0f, d/127.0f}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadByteN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHINEPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

// Special-case of -128
    {
        src.x = src.y = src.z = src.w = -128;
        XMVECTORF32 chk = { -1.f, -1.f, -1.f, -1.f };
        v = XMLoadByteN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test511(LogProxy* pLog)
{
//XMLoadUByte4 
    static_assert( sizeof(XMUBYTE4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUBYTE4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUBYTE4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUBYTE4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUBYTE4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMUBYTE4 src;

    static const unsigned char s[4][4] = {{0,0,0,0},{0xff,0xff,0xff,0xff},{0x80,0x80,0x80,0x80},{0x80,0x00,0xff,0x80}};
    static const XMVECTORF32 check[] = {{0,0,0,0},{255,255,255,255},{128,128,128,128},{128,0,255,128}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; src.z=s[k][2]; src.w=s[k][3];
        v = XMLoadUByte4(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        unsigned char a = (rand() + rand()) & 0xff; 
        unsigned char b = (rand() + rand()) & 0xff; 
        unsigned char c = (rand() + rand()) & 0xff; 
        unsigned char d = (rand() + rand()) & 0xff; 
        XMVECTORF32 chk= {{float(a), float(b), float(c), float(d)}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadUByte4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHINEPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test512(LogProxy* pLog)
{
//XMLoadUByteN4 
    static_assert( sizeof(XMUBYTEN4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUBYTEN4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUBYTEN4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUBYTEN4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUBYTEN4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMUBYTEN4 src;

    static const unsigned char s[4][4] = {{0,0,0,0},{0xff,0xff,0xff,0xff},{0xff,0,0xff,0},{0,0xff,0,0xff}};
    static const XMVECTORF32 check[] = {{0,0,0,0},{1,1,1,1},{1,0,1,0},{0,1,0,1}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; src.z=s[k][2]; src.w=s[k][3];
        v = XMLoadUByteN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        unsigned char a = (rand() + rand()) & 0xff; 
        unsigned char b = (rand() + rand()) & 0xff; 
        unsigned char c = (rand() + rand()) & 0xff; 
        unsigned char d = (rand() + rand()) & 0xff; 
        XMVECTORF32 chk= {{a/255.0f, b/255.0f, c/255.0f, d/255.0f}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadUByteN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

#pragma warning(push)
#pragma warning(disable : 4996)
// C4996: ignore deprecation warning

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

HRESULT Test513(LogProxy* pLog)
{
//XMLoadDec4 
    static_assert( sizeof(XMDEC4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMDEC4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMDEC4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMDEC4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMDEC4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;

    static const uint32_t s[] = {0,0x1ff+(0x1ff<<10)+(0x1ff<<20)+(0x01U<<30),0x201+(0x201<<10)+(0x201<<20)+(0x3U<<30),0x0+(0x1ff<<10)+(0x201<<20)+(0x0U<<30)};
    static const XMVECTORF32 check[] = {{0,0,0,0},{511,511,511,1},{-511,-511,-511,-1},{0,511,-511,0}};
    for(int k = 0; k < countof(s); k++) {
        v = XMLoadDec4((const XMDEC4*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        uint32_t a = rand() & 0x3ff; if(a == 0x200) a = 0;
        uint32_t b = rand() & 0x3ff; if(b == 0x200) b = 0;
        uint32_t c = rand() & 0x3ff; if(c == 0x200) c = 0;
        uint32_t d = rand() & 0x3;   if(d == 0x2)   d = 0;
        XMDEC4 src;
        XMVECTORF32 chk= {{
            ((float)(a&0x1ff))-((a&0x200)?(512.f):0.f),
            ((float)(b&0x1ff))-((b&0x200)?(512.f):0.f),
            ((float)(c&0x1ff))-((c&0x200)?(512.f):0.f),
            ((float)(d&0x1) -((d&0x2)?2.f:0.f))
            }};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadDec4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test514(LogProxy* pLog)
{
//XMLoadDecN4 
    static_assert( sizeof(XMDECN4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMDECN4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMDECN4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMDECN4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMDECN4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;

    static const uint32_t s[] = {0,0x1ff+(0x1ff<<10)+(0x1ff<<20)+(0x01U<<30),0x201+(0x201<<10)+(0x201<<20)+(0x3U<<30),0x0+(0x1ff<<10)+(0x201<<20)+(0x0U<<30)};
    static const XMVECTORF32 check[] = {{0,0,0,0},{1,1,1,1},{-1,-1,-1,-1},{0,1,-1,0}};
    for(int k = 0; k < countof(s); k++) {
        v = XMLoadDecN4((const XMDECN4*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        uint32_t a = rand() & 0x3ff; if(a == 0x200) a = 0;
        uint32_t b = rand() & 0x3ff; if(b == 0x200) b = 0;
        uint32_t c = rand() & 0x3ff; if(c == 0x200) c = 0;
        uint32_t d = rand() & 0x3;   if(d == 0x2)   d = 0;
        XMDECN4 src;
        XMVECTORF32 chk= {{
            ((float)(a&0x1ff))/511.f-((a&0x200)?(512.f/511.f):0.f),
            ((float)(b&0x1ff))/511.f-((b&0x200)?(512.f/511.f):0.f),
            ((float)(c&0x1ff))/511.f-((c&0x200)?(512.f/511.f):0.f),
            ((float)(d&0x1) -((d&0x2)?2.f:0.f))
            }};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadDecN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#pragma warning(pop)

HRESULT Test515(LogProxy* pLog)
{
//XMLoadUDec4 
    static_assert( sizeof(XMUDEC4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUDEC4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUDEC4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUDEC4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUDEC4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;

    static const uint32_t s[] = {0,0x1ff+(0x1ff<<10)+(0x1ff<<20)+(0x01U<<30),0x201+(0x201<<10)+(0x201<<20)+(0x3U<<30),0x0+(0x1ff<<10)+(0x201<<20)+(0x0U<<30)};
    static const XMVECTORF32 check[] = {{0,0,0,0},{511,511,511,1},{513,513,513,3},{0,511,513,0}};
    for(int k = 0; k < countof(s); k++) {
        v = XMLoadUDec4((const XMUDEC4*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        uint32_t a = rand() & 0x3ff; 
        uint32_t b = rand() & 0x3ff; 
        uint32_t c = rand() & 0x3ff; 
        uint32_t d = rand() & 0x3;   
        XMUDEC4 src;
        XMVECTORF32 chk= {{(float)a, (float)b, (float)c, (float)d}};

        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadUDec4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test516(LogProxy* pLog)
{
//XMLoadUDecN4
    static_assert( sizeof(XMUDECN4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUDECN4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUDECN4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUDECN4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUDECN4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;

    {
        static const uint32_t s[] = {0,
                     0x3ff+(0x3ff<<10)+(0x3ff<<20)+(0x3U<<30),
                     0x3ff+(0x0<<10)+(0x3ff<<20)+(0x0U<<30),
                     0x0+(0x3ff<<10)+(0x0<<20)+(0x3U<<30)};
        static const XMVECTORF32 check[] = {{0,0,0,0},{1,1,1,1},{1,0,1,0},{0,1,0,1}};
        for(int k = 0; k < countof(s); k++) {
            v = XMLoadUDecN4((const XMUDECN4*)&s[k]);
            COMPARISON cc = CompareXMVECTOR(v,check[k],4);
            if(cc != EXACT) {
                printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                    XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
                ret = MATH_FAIL;
            }
        }

        for(int k = 0; k < 15; k++) {
            uint32_t a = rand() & 0x3ff;
            uint32_t b = rand() & 0x3ff;
            uint32_t c = rand() & 0x3ff;
            uint32_t d = rand() & 0x3;
            XMUDECN4 src;
            XMVECTORF32 chk= {{
                (float)a/1023.f,
                (float)b/1023.f,
                (float)c/1023.f,
                (float)d/3.f
                }};
            src.x=a; src.y=b; src.z=c; src.w=d;

            v = XMLoadUDecN4(&src);
            COMPARISON cc = CompareXMVECTOR(v,chk,4);
            if(cc > WITHIN10EPSILON) {
                printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                    XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
                ret = MATH_FAIL;
            }
        }
    }

//XMLoadUDecN4_XR
    {
        static const uint32_t s[] = {0,0x18060180,0xA7F9FE7F,0x77EDFB7E,0xFFEFFBFE,0x2FE9FDFF};
        static const XMVECTORF32 check[] = { {-0.7529f, -0.7529f, -0.7529f, 0.f},
                                             {0.0f, 0.0f, 0.0f, 0.0f},
                                             {0.5f, 0.5f, 0.5f, 0.666667f},
                                             {1.0f, 1.0f, 1.0f, 0.333334f},
                                             {1.250980f, 1.250980f, 1.250980f, 1.0f},
                                             {0.249020f, 0.5f, 0.749020f, 0.0f} };
        for(int k = 0; k < countof(s); k++) {
            v = XMLoadUDecN4_XR((const XMUDECN4*)&s[k]);
            COMPARISON cc = CompareXMVECTOR(v,check[k],4);
            if(cc > WITHIN4096) {
                printe("%s (XR): %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                    XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
                ret = MATH_FAIL;
            }
        }

        for(int k = 0; k < 15; k++) {
            uint32_t a = rand() & 0x3ff;
            uint32_t b = rand() & 0x3ff;
            uint32_t c = rand() & 0x3ff;
            uint32_t d = rand() & 0x3;
            XMUDECN4 src;
            XMVECTORF32 chk= {{
                (float)( int32_t(a) - 0x180)/510.f,
                (float)( int32_t(b) - 0x180)/510.f,
                (float)( int32_t(c) - 0x180)/510.f,
                (float)d/3.f
                }};
            src.x=a; src.y=b; src.z=c; src.w=d;

            v = XMLoadUDecN4_XR(&src);
            COMPARISON cc = CompareXMVECTOR(v,chk,4);
            if(cc > WITHIN4096) {
                printe("%s (XR): %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                    TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                    XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
                ret = MATH_FAIL;
            }
        }
    }

    return ret;
}

#pragma warning(push)
#pragma warning(disable : 4996)
// C4996: ignore deprecation warning

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

HRESULT Test517(LogProxy* pLog)
{
//XMLoadXDec4 
    static_assert( sizeof(XMXDEC4) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMXDEC4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMXDEC4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMXDEC4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMXDEC4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;

    static const uint32_t s[] = {0,0x1ff+(0x1ff<<10)+(0x1ff<<20)+(0x01U<<30),0x201+(0x201<<10)+(0x201<<20)+(0x3U<<30),0x0+(0x1ff<<10)+(0x201<<20)+(0x0U<<30)};
    static const XMVECTORF32 check[] = {{0,0,0,0},{511,511,511,1},{-511,-511,-511,3},{0,511,-511,0}};
    for(int k = 0; k < countof(s); k++) {
        v = XMLoadXDec4((const XMXDEC4*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        uint32_t a = rand() & 0x3ff; if(a == 0x200) a = 0;
        uint32_t b = rand() & 0x3ff; if(b == 0x200) b = 0;
        uint32_t c = rand() & 0x3ff; if(c == 0x200) c = 0;
        uint32_t d = rand() & 0x3;
        XMXDEC4 src;
        XMVECTORF32 chk= {{
            ((float)(a&0x1ff))-((a&0x200)?(512.f):0.f),
            ((float)(b&0x1ff))-((b&0x200)?(512.f):0.f),
            ((float)(c&0x1ff))-((c&0x200)?(512.f):0.f),
            (float)(d)
            }};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadXDec4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#pragma warning(pop)

HRESULT Test524(LogProxy* pLog)
{
//XMLoadShort4 
    static_assert( sizeof(XMSHORT4) == 8, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMSHORT4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMSHORT4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMSHORT4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMSHORT4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMSHORT4 src;

    static const unsigned short s[4][4] = {{0,0,0,0},{0x8001,0x8001,0x8001,0x8001},{0x7fff,0x7fff,0x7fff,0x7fff},{0x8001,0x7fff,0x0000,0x8001}};
    static const XMVECTORF32 check[] = {{{0,0,0,0}},{{-32767,-32767,-32767,-32767}},{{32767,32767,32767,32767}},{{-32767,32767,0,-32767}}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; src.z=s[k][2]; src.w=s[k][3];
        v = XMLoadShort4(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        short a = (rand() + rand()) & 0xffff;
        short b = (rand() + rand()) & 0xffff;
        short c = (rand() + rand()) & 0xffff;
        short d = (rand() + rand()) & 0xffff;
        XMVECTORF32 chk= {{(float)a, (float)b, (float)c, (float)d}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadShort4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

// Special-case of -32768
    {
        src.x = src.y = src.z = src.w = -32768;
        XMVECTORF32 chk = { -32768.f, -32768.f, -32768.f, -32768.f };
        v = XMLoadShort4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test525(LogProxy* pLog)
{
//XMLoadUShort4 
    static_assert( sizeof(XMUSHORT4) == 8, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUSHORT4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUSHORT4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUSHORT4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUSHORT4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMUSHORT4 src;

    static const unsigned short s[4][4] = {{0,0,0,0},{0x8000,0x8000,0x8000,0x8000},{0xffff,0xffff,0xffff,0xffff},{0x8001,0xffff,0x0000,0x8001}};
    static const XMVECTORF32 check[] = {{{0,0,0,0}},{{32768,32768,32768,32768}},{{65535,65535,65535,65535}},{{32769,65535,0,32769}}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; src.z=s[k][2]; src.w=s[k][3];
        v = XMLoadUShort4(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        unsigned short a = (rand() + rand()) & 0xffff;
        unsigned short b = (rand() + rand()) & 0xffff;
        unsigned short c = (rand() + rand()) & 0xffff;
        unsigned short d = (rand() + rand()) & 0xffff;
        XMVECTORF32 chk= {{(float)a, (float)b, (float)c, (float)d}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadUShort4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test526(LogProxy* pLog)
{
//XMLoadUShortN4 
    static_assert( sizeof(XMUSHORTN4) == 8, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUSHORTN4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUSHORTN4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUSHORTN4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUSHORTN4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMUSHORTN4 src;

    static const unsigned short s[4][4] = {{0,0,0,0},{0xffff,0xffff,0xffff,0xffff},{0,0,0xffff,0xffff},{0xffff,0xffff,0,0}};
    static const XMVECTORF32 check[] = {{{0,0,0,0}},{{1,1,1,1}},{{0,0,1,1}},{{1,1,0,0}}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; src.z=s[k][2]; src.w=s[k][3];
        v = XMLoadUShortN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        unsigned short a = (rand() + rand()) & 0xffff;
        unsigned short b = (rand() + rand()) & 0xffff;
        unsigned short c = (rand() + rand()) & 0xffff;
        unsigned short d = (rand() + rand()) & 0xffff;
        XMVECTORF32 chk= {{(float)a / 65535.0f, (float)b / 65535.0f, (float)c / 65535.0f, (float)d / 65535.0f}};
        src.x=a; src.y=b; src.z=c; src.w=d;

        v = XMLoadUShortN4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %llx -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint64_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test535(LogProxy* pLog)
{
//XMLoadShort2 
    static_assert( sizeof(XMSHORT2) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMSHORT2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMSHORT2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMSHORT2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMSHORT2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMSHORT2 src;

    static const unsigned short s[4][4] = {{0,0,0,0},{0x8001,0x8001,0,0},{0x7fff,0x7fff,0,0},{0x8001,0x7fff,0,0}};
    static const XMVECTORF32 check[] = {{{0,0,0,0}},{{-32767,-32767,0,0}},{{32767,32767,0,0}},{{-32767,32767,0,0}}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; 
        v = XMLoadShort2(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],2);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f ... %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        short a = (rand() + rand()) & 0xffff;
        short b = (rand() + rand()) & 0xffff;
        XMVECTORF32 chk= {{(float)a, (float)b, _Q_NAN, _Q_NAN}};
        src.x=a; src.y=b; 

        v = XMLoadShort2(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f ... %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

// Special-case of -32768
    {
        src.x = src.y = -32768;
        XMVECTORF32 chk = { -32768.f, -32768.f, 0, 0 };
        v = XMLoadShort2(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f ... %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test536(LogProxy* pLog)
{
//XMLoadUShort2 
    static_assert( sizeof(XMUSHORT2) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUSHORT2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUSHORT2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUSHORT2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUSHORT2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMUSHORT2 src;

    static const unsigned short s[4][4] = {{0,0,0,0},{0x8001,0x8001,0,0},{0xffff,0xffff,0,0},{0x8001,0xffff,0,0}};
    static const XMVECTORF32 check[] = {{{0,0,0,0}},{{32769,32769,0,0}},{{65535,65535,0,0}},{{32769,65535,0,0}}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; 
        v = XMLoadUShort2(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],2);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        unsigned short a = (rand() + rand()) & 0xffff;
        unsigned short b = (rand() + rand()) & 0xffff;
        XMVECTORF32 chk= {{(float)a, (float)b, _Q_NAN, _Q_NAN}};
        src.x=a; src.y=b; 

        v = XMLoadUShort2(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test537(LogProxy* pLog)
{
//XMLoadUShortN2 
    static_assert( sizeof(XMUSHORTN2) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUSHORTN2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUSHORTN2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUSHORTN2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUSHORTN2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XMVECTOR v;
    HRESULT ret = S_OK;
    XMUSHORTN2 src;

    static const unsigned short s[4][4] = {{0,0,0,0},{0x0,0x0,0,0},{0xffff,0xffff,0,0},{0xffff,0xffff,0,0}};
    static const XMVECTORF32 check[] = {{{0,0,0,0}},{{0,0,0,0}},{{1,1,0,0}},{{1,1,0,0}}};
    for(int k = 0; k < countof(s); k++) {
        src.x=s[k][0]; src.y=s[k][1]; 
        v = XMLoadUShortN2(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],2);
        if(cc != EXACT) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++) {
        unsigned short a = (rand() + rand()) & 0xffff;
        unsigned short b = (rand() + rand()) & 0xffff;
        XMVECTORF32 chk= {{(float)a / 65535.0f, (float)b / 65535.0f, _Q_NAN, _Q_NAN}};
        src.x=a; src.y=b; 

        v = XMLoadUShortN2(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }
    return ret;
}

HRESULT Test538(LogProxy* pLog)
{
//XMStoreUShortN2 

    // TODO - Check for zeroing of partial loads

    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);
    int unitsize = 2; //size in byte

    int unitcount = 2;
    uint16_t f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const unsigned short check[4][4] = {{0,0,0,0},{0xffff,0xffff,0xffff,0xffff},{0,0xffff,0,0xffff},{0xffff,0,0,0xffff}};
    static const XMVECTORF32 v[] = {{{0,0,0,0}},{{1,1,1,1}},{{0,1,0,1}},{{1,0,0,1}}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + unitsize * unitcount;

        XMStoreUShortN2((XMUSHORTN2*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < unitcount; i++) {
            f = *((const uint16_t*)&(c[first+(i * unitsize)]));
            if(f != check[n][i]) {
                printe("%s: %p corrupted short %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),f,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test539(LogProxy* pLog)
{
//XMStoreUShort2 
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);
    int unitsize = 2; //size in byte
    int unitcount = 2;

    uint16_t f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const unsigned short check[4][4] = {{0,0,0,0},{0xffff,0xffff,0xffff,0xffff},{0,0xffff,0,0xffff},{0xffff,0,0,0xffff}};
    static const XMVECTORF32 v[] = {{0,0,0,0},{0xffff,0xffff,0xffff,0xffff},{0,0xffff,0,0xffff},{0xffff,0,0,0xffff}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + unitsize * unitcount;

        XMStoreUShort2((XMUSHORT2*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < unitcount; i++) {
            f = *((const uint16_t*)&(c[first+(i * unitsize)]));
            if(f != check[n][i]) {
                printe("%s: %p corrupted  %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),f,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test540(LogProxy* pLog)
{
//XMStoreShort2 
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);
    int unitsize = 2; //size in byte
    int unitcount = 2;

    uint16_t f;
    intptr_t i, j; 
    HRESULT r = S_OK;

#if defined(_XENON) && !defined(_XM_NO_INTRINSICS_)
    static const unsigned short check[5][4] = {{0,0,0,0},{0x8001,0xffff,0,0},{0xffff,0xffff,0,0},{0x8001,0xffff,0,0},{0x8000,0,0,0}};
#else
    static const unsigned short check[5][4] = {{0,0,0,0},{0x8001,0xffff,0,0},{0xffff,0xffff,0,0},{0x8001,0xffff,0,0},{0x8001,0,0,0}};
#endif
    static const XMVECTORF32 v[] = {{0,0,0,0},{-0x7fff,-1,0,0},{-1,-1,0,0},{-0x7fff,-1,0,0},{-32768.f,0,0,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + unitsize * unitcount;

        XMStoreShort2((XMSHORT2*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < unitcount; i++) {
            f = *((const uint16_t*)&(c[first+(i * unitsize)]));
            if(f != check[n][i]) {
                printe("%s: %p corrupted short %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),f,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test549(LogProxy* pLog)
{
//XMStoreShort4 
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);
    int unitsize = 2; //size in byte
    int unitcount = 4;

    uint16_t f;
    intptr_t i, j; 
    HRESULT r = S_OK;

#if defined(_XENON) && !defined(_XM_NO_INTRINSICS_)
    static const unsigned short check[5][4] = {{0,0,0,0},{0x8001,0xffff,0x8001,0xffff,},{0xffff,0xffff,0xffff,0xffff},{0x8001,0xffff,0x8001,0xffff},{0x8000,0,0,0}};
#else
    static const unsigned short check[5][4] = {{0,0,0,0},{0x8001,0xffff,0x8001,0xffff,},{0xffff,0xffff,0xffff,0xffff},{0x8001,0xffff,0x8001,0xffff},{0x8001,0,0,0}};
#endif
    static const XMVECTORF32 v[] = {{0,0,0,0},{-0x7fff,-1,-0x7fff,-1,},{-1,-1,-1,-1},{-0x7fff,-1,-0x7fff,-1},{-32768.f,0,0,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + unitsize * unitcount;

        XMStoreShort4((XMSHORT4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < unitcount; i++) {
            f = *((const uint16_t*)&(c[first+(i * unitsize)]));
            if(f != check[n][i]) {
                printe("%s: %p corrupted short %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),f,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test550(LogProxy* pLog)
{
//XMStoreUShortN4 
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);
    int unitsize = 2; //size in byte

    int unitcount = 4;
    uint16_t f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const unsigned short check[4][4] = {{0,0,0,0},{0xffff,0xffff,0xffff,0xffff},{0,0xffff,0,0xffff},{0xffff,0,0,0xffff}};
    static const XMVECTORF32 v[] = {{{0,0,0,0}},{{1,1,1,1}},{{0,1,0,1}},{{1,0,0,1}}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + unitsize * unitcount;

        XMStoreUShortN4((XMUSHORTN4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < unitcount; i++) {
            f = *((const uint16_t*)&(c[first+(i * unitsize)]));
            if(f != check[n][i]) {
                printe("%s: %p corrupted short %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),f,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test551(LogProxy* pLog)
{
//XMStoreUShort4 
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);
    int unitsize = 2; //size in byte
    int unitcount = 4;

    uint16_t f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const unsigned short check[4][4] = {{0,0,0,0},{0xffff,0xffff,0xffff,0xffff},{0,0xffff,0,0xffff},{0xffff,0,0,0xffff}};
    static const XMVECTORF32 v[] = {{0,0,0,0},{0xffff,0xffff,0xffff,0xffff},{0,0xffff,0,0xffff},{0xffff,0,0,0xffff}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + unitsize * unitcount;

        XMStoreUShort4((XMUSHORT4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < unitcount; i++) {
            f = *((const uint16_t*)&(c[first+(i * unitsize)]));
            if(f != check[n][i]) {
                printe("%s: %p corrupted short %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),f,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

#pragma warning(push)
#pragma warning(disable : 4996)
// C4996: ignore deprecation warning

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

HRESULT Test558(LogProxy* pLog)
{
//XMStoreXDec4 
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint32_t check[4] = {0, 
                      0x1ff + (0x1ff<<10) + (0x1ff<<20) + (0x3U<<30), 
                      0x201 + (0x201<<10) + (0x201<<20) + (0x0U<<30), 
                      0x1ff + (0x201<<10) + (0x1ff<<20) + (0x3U<<30)};
    static const XMVECTORF32 v[] = {{0,0,0,0},{511,511,511,3},{-511,-511,-511,0},{511,-511,511,3}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreXDec4((XMXDEC4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        f = *((const uint32_t *)&(c[first]));
        if(f != check[n]) {
            printe("%s: %p corrupted in: %x ... %x\n", TestName, reinterpret_cast<void*>(j),f,check[n]);
            r=MATH_FAIL;
        }
    }
    free(c);
    return r;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#pragma warning(pop)

HRESULT Test559(LogProxy* pLog)
{
//XMStoreUDecN4 
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    {
        static const uint32_t check[4] = {0, 
                          0x3ff + (0x3ff<<10) + (0x3ff<<20) + (0x3U<<30), 
                          0x3ff + (0<<10)     + (0x3ff<<20) + (0x0U<<30), 
                          0     + (0x3ff<<10) + (0<<20)     + (0x3U<<30)};
        static const XMVECTORF32 v[] = {{0,0,0,0},{1,1,1,1},{1,0,1,0},{0,1,0,1}};

        int n = 0;
        for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
            n = (n + 1) % (countof(check));
            for(i = 0; i < csize; i++) {
                c[i] = (char)(~i & 0xff);
            }

            intptr_t first = j;
            intptr_t last  = j + 4;

            XMStoreUDecN4((XMUDECN4*)&c[j], v[n]);
            for(i = 0; i < first; i++) {
                if(c[i] != (char)(~i & 0xff)) {
                    printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                    r = MATH_FAIL;
                }
            }
            for(i = last; i < csize; i++) {
                if(c[i] != (char)(~i & 0xff)) {
                    printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                    r = MATH_FAIL;
                }
            }
            f = *((const uint32_t *)&(c[first]));
            if(f != check[n]) {
                printe("%s: %p corrupted int: %x ... %x\n", TestName, reinterpret_cast<void*>(j),f,check[n]);
                r=MATH_FAIL;
            }
        }
    }

//XMStoreUDecN4_XR
    {
        static const uint32_t check[] = {0,0x18060180,0xA7F9FE7F,0x77EDFB7E,0xffdff7fd,0x2FE9FDFF};
        static const XMVECTORF32 v[] = { {-0.7529f, -0.7529f, -0.7529f, 0.f},
                                             {0.0f, 0.0f, 0.0f, 0.0f},
                                             {0.5f, 0.5f, 0.5f, 0.666667f},
                                             {1.0f, 1.0f, 1.0f, 0.333334f},
                                             {1.250980f, 1.250980f, 1.250980f, 1.0f},
                                             {0.249020f, 0.5f, 0.749020f, 0.0f} };

        int n = 0;
        for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
            n = (n + 1) % (countof(check));
            for(i = 0; i < csize; i++) {
                c[i] = (char)(~i & 0xff);
            }

            intptr_t first = j;
            intptr_t last  = j + 4;

            XMStoreUDecN4_XR((XMUDECN4*)&c[j], v[n]);
            for(i = 0; i < first; i++) {
                if(c[i] != (char)(~i & 0xff)) {
                    printe("%s (XR): %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                    r = MATH_FAIL;
                }
            }
            for(i = last; i < csize; i++) {
                if(c[i] != (char)(~i & 0xff)) {
                    printe("%s (XR): %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                    r = MATH_FAIL;
                }
            }
            f = *((const uint32_t *)&(c[first]));
            if(f != check[n]) {
                printe("%s (XR): %p corrupted int: %x ... %x\n", TestName, reinterpret_cast<void*>(j),f,check[n]);
                r=MATH_FAIL;
            }
        }
    }

    free(c);
    return r;
}

HRESULT Test560(LogProxy* pLog)
{
//XMStoreUDec4 
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint32_t check[4] = {0, 
                      0x3ff + (0x3ff<<10) + (0x3ff<<20) + (0x3U<<30), 
                      0x3ff + (0<<10)     + (0x3ff<<20) + (0x0U<<30), 
                      0     + (0x3ff<<10) + (0<<20)     + (0x3U<<30)};
    static const XMVECTORF32 v[] = {{0,0,0,0},{1023,1023,1023,3},{1023,0,1023,0},{0,1023,0,3}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreUDec4((XMUDEC4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        f = *((const uint32_t *)&(c[first]));
        if(f != check[n]) {
            printe("%s: %p corrupted int: %x ... %x\n", TestName, reinterpret_cast<void*>(j),f,check[n]);
            r=MATH_FAIL;
        }
    }
    free(c);
    return r;
}

#pragma warning(push)
#pragma warning(disable : 4996)
// C4996: ignore deprecation warning

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

HRESULT Test561(LogProxy* pLog)
{
//XMStoreDecN4 
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint32_t check[4] = {0, 
                      0x1ff + (0x1ff<<10) + (0x1ff<<20) + (0x1U<<30), 
                      0x201 + (0x201<<10) + (0x201<<20) + (0x3U<<30), 
                      0x201 + (0x1ff<<10) + (0x201<<20) + (0x1U<<30)};
    static const XMVECTORF32 v[] = {{0,0,0,0},{1,1,1,1},{-1,-1,-1,-1},{-1,1,-1,1}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreDecN4((XMDECN4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        f = *((const uint32_t *)&(c[first]));
        if(f != check[n]) {
            printe("%s: %p corrupted int: %x ... %x\n", TestName, reinterpret_cast<void*>(j),f,check[n]);
            r=MATH_FAIL;
        }
    }
    free(c);
    return r;
}

HRESULT Test562(LogProxy* pLog)
{
//XMStoreDec4 
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint32_t check[4] = {0, 
                      0x1ff + (0x1ff<<10) + (0x1ff<<20) + (0x1U<<30), 
                      0x201 + (0x201<<10) + (0x201<<20) + (0x3U<<30), 
                      0x201 + (0x1ff<<10) + (0x201<<20) + (0x1U<<30)};
    static const XMVECTORF32 v[] = {{0,0,0,0},{511,511,511,1},{-511,-511,-511,-1},{-511,511,-511,1}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreDec4((XMDEC4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        f = *((const uint32_t *)&(c[first]));
        if(f != check[n]) {
            printe("%s: %p corrupted int: %x ... %x\n", TestName, reinterpret_cast<void*>(j),f,check[n]);
            r=MATH_FAIL;
        }
    }
    free(c);
    return r;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#pragma warning(pop)

HRESULT Test563(LogProxy* pLog)
{
//XMStoreUByteN4
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    // The real XMUBYTEN4 structure is a C++ class
    // that can't be initialized with a static data
    // declaration. The structure below is a
    // copy of the real structure but formatted
    // in a way to allow static data declaration.

    struct FAKEXMUBYTEN4 {
        union {
            uint8_t b[4];
            uint32_t v;
        };
    };

    static const FAKEXMUBYTEN4 check[4] = {
        {0x00,0x00,0x00,0x00}, 
        {0xff,0xff,0xff,0xff},
        {0x00,0xff,0x00,0xff}, 
        {0xff,0x00,0xff,0x00}
    };
    static const XMVECTORF32 v[] = {{0,0,0,0},{1,1,1,1},{0,1,0,1},{1,0,1,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }
        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreUByteN4((XMUBYTEN4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        f = *((const uint32_t *)&(c[first]));
        if(f != check[n].v) {
            printe("%s: %p corrupted int %d: %x ... %x\n", TestName, reinterpret_cast<void*>(j),n,f,check[n].v);
            r=MATH_FAIL;
        }
    }
    free(c);
    return r;
}

HRESULT Test564(LogProxy* pLog)
{
//XMStoreUByte4
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    struct FAKEXMUBYTE4 {
        union {
            uint8_t b[4];
            uint32_t v;
        };
    };

    static const FAKEXMUBYTE4 check[4] = {
        {0x00,0x00,0x00,0x00}, 
        {0xff,0xff,0xff,0xff}, 
        {0x00,0xff,0x00,0xff},
        {0xff,0x00,0xff,0x00}
    };
    static const XMVECTORF32 v[] = {{0,0,0,0},{255,255,255,255},{0,255,0,255},{255,0,255,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }
        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreUByte4((XMUBYTE4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        f = *((const uint32_t *)&(c[first]));
        if(f != check[n].v) {
            printe("%s: %p corrupted %d : %x ... %x\n", TestName, reinterpret_cast<void*>(j),n,f,check[n].v);
            r=MATH_FAIL;
        }
    }
    free(c);
    return r;
}

HRESULT Test565(LogProxy* pLog)
{
//XMStoreByteN4
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    struct FAKEXMBYTEN4 {
        union {
            char c[4];
            uint32_t v;
        };
    };

    static const FAKEXMBYTEN4 check[4] = {
        {0x00,0x00,0x00,0x00}, 
        {0x7f,0x7f,0x7f,0x7f}, 
        {-0x7F,0x00,0x7f,-0x7F}, 
        {0x7f,0x00,0x00,-0x7F}
    };
    static const XMVECTORF32 v[] = {{0,0,0,0},{1,1,1,1},{-1,0,1,-1},{1,0,0,-1}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }
        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreByteN4((XMBYTEN4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        f = *((const uint32_t *)&(c[first]));
        if(f != check[n].v) {
            printe("%s: %p corrupted int %d: %x ... %x\n", TestName, reinterpret_cast<void*>(j),n,f,check[n].v);
            r=MATH_FAIL;
        }
    }
    free(c);
    return r;
}

HRESULT Test566(LogProxy* pLog)
{
//XMStoreByte4
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);


    uint32_t  f;
    intptr_t i, j; 
    HRESULT r = S_OK;

    struct FAKEXMBYTE4 {
        union {
            char b[4];
            uint32_t v;
        };
    };

    static const FAKEXMBYTE4 check[] = {
        {0x00,0x00,0x00,0x00}, 
        {0x7f,0x7f,0x7f,0x7f}, 
        {-0x7f,0x00,0x7f,-0x7f}, 
        {0x7f,0x00,0x00,-0x7f},
#if defined(_XENON) && !defined(_XM_NO_INTRINSICS_)
        {-128,0,0,0},
#else
        {-127,0,0,0},
#endif
    };
    static const XMVECTORF32 v[] = {{0,0,0,0},{127,127,127,127},{-127,0,127,-127},{127,0,0,-127},{-128.f,0,0,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4) {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++) {
            c[i] = (char)(~i & 0xff);
        }
        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreByte4((XMBYTE4*)&c[j], v[n]);
        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        f = *((const uint32_t *)&(c[first]));
        if(f != check[n].v) {
            printe("%s: %p corrupted %d: %x ... %x\n", TestName, reinterpret_cast<void*>(j),n,f,check[n].v);
            r=MATH_FAIL;
        }
    }
    free(c);
    return r;
}

HRESULT Test567(LogProxy* pLog)
{
//XMConvertVectorUIntToFloat and XMConvertVectorIntToFloat 
    HRESULT r = S_OK;
    XMVECTORF32 result;
    XMVECTOR check;
    COMPARISON cc;
    int n;

    //XMConvertVectorUIntToFloat
    static const XMVECTORU32 uv[] =     {{0,0,0,0},{127,127,127,127},{65535,0,0,65535},{1024,1,1024,1},{0xFFFFFFFF,0x80000000,0xFFFFFF80,0xFFFFFF00}};
    static const XMVECTORF32 ucheck[] = {{0,0,0,0},{127,127,127,127},{65535,0,0,65535},{1024,1,1024,1},{65536.0f*65536.0f,65536.0f*32768.0f,65536.0f*65536.0f,(65536.0f*65536.0f)-256.0f}};
    static const XMVECTORF32 vDivisor2 = {1.0f/2.0f,1.0f/2.0f,1.0f/2.0f,1.0f/2.0f};
    static const XMVECTORF32 vDivisor16= {1.0f/16.0f,1.0f/16.0f,1.0f/16.0f,1.0f/16.0f};
    static const XMVECTORF32 vDivisor31= {1.0f/(65536.0f*32768.0f),1.0f/(65536.0f*32768.0f),1.0f/(65536.0f*32768.0f),1.0f/(65536.0f*32768.0f)};

    for(n = 0; n < countof(uv); n++) {
        result.v = XMConvertVectorUIntToFloat(uv[n],0);
        check = ucheck[n];
        cc = CompareXMVECTOR(result,check,4);
        if(cc != EXACT) {
            printe("%s: v.X:      %u v.Y:      %u      v.Z: %u      v.W: %u\n", TestName, uv[n].u[0],uv[n].u[1],uv[n].u[2],uv[n].u[3]);
            printe("(0) result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           result.f[0],result.f[1],result.f[2],result.f[3]);
            printe("    ucheck.X: %f ucheck.Y: %f ucheck.Z: %f ucheck.W: %f\n",           XMVectorGetX(check), XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
            r=MATH_FAIL;
        }

        result.v = XMConvertVectorUIntToFloat(uv[n],1);
        check = XMVectorMultiply(ucheck[n],vDivisor2);
        cc = CompareXMVECTOR(result,check,4);
        if(cc != EXACT) {
            printe("%s: v.X:      %u v.Y:      %u      v.Z: %u      v.W: %u\n", TestName, uv[n].u[0],uv[n].u[1],uv[n].u[2],uv[n].u[3]);
            printe("(1) result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           result.f[0],result.f[1],result.f[2],result.f[3]);
            printe("    ucheck.X: %f ucheck.Y: %f ucheck.Z: %f ucheck.W: %f\n",           XMVectorGetX(check), XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
            r=MATH_FAIL;
        }

        result.v = XMConvertVectorUIntToFloat(uv[n],4);
        check = XMVectorMultiply(ucheck[n],vDivisor16);
        cc = CompareXMVECTOR(result,check,4);
        if(cc != EXACT) {
            printe("%s: v.X:      %u v.Y:      %u      v.Z: %u      v.W: %u\n", TestName, uv[n].u[0],uv[n].u[1],uv[n].u[2],uv[n].u[3]);
            printe("(4) result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           result.f[0],result.f[1],result.f[2],result.f[3]);
            printe("    ucheck.X: %f ucheck.Y: %f ucheck.Z: %f ucheck.W: %f\n",           XMVectorGetX(check), XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
            r=MATH_FAIL;
        }

        result.v = XMConvertVectorUIntToFloat(uv[n],31);
        check = XMVectorMultiply(ucheck[n],vDivisor31);
        cc = CompareXMVECTOR(result,check,4);
        if(cc != EXACT) {
            printe("%s: v.X:      %u v.Y:      %u      v.Z: %u      v.W: %u\n", TestName, uv[n].u[0],uv[n].u[1],uv[n].u[2],uv[n].u[3]);
            printe("(31)result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           result.f[0],result.f[1],result.f[2],result.f[3]);
            printe("    ucheck.X: %f ucheck.Y: %f ucheck.Z: %f ucheck.W: %f\n",           XMVectorGetX(check), XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
            r=MATH_FAIL;
        }
    }
    
    //XMConvertVectorIntToFloat

    static const XMVECTORI32 iv[] = {{0,0,0,0},{127,-127,127,-127},{-65535,0,0,65535},{1024,-1,-1024,1},{0x7FFFFFFF,int32_t(0x80000000),-0x7FFFFF80,0x7FFFFF80}};
    static const XMVECTORF32 icheck[] = {{0,0,0,0},{127,-127,127,-127},{-65535,0,0,65535},{1024,-1,-1024,1},{65536.0f*32768.0f,-65536.0f*32768.0f,-((65536.0f*32768.0f)-128.0f),(65536.0f*32768.0f)-128.0f}};
    for(n = 0; n < countof(iv); n++) {
        result.v = XMConvertVectorIntToFloat(iv[n],0);
        check = icheck[n];
        cc = CompareXMVECTOR(result,check,4);
        if(cc != EXACT) {
            printe("%s: v.X:      %d v.Y:      %d      v.Z: %d      v.W: %d\n", TestName, iv[n].i[0],iv[n].i[1],iv[n].i[2],iv[n].i[3]);
            printe("(0) result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           result.f[0],result.f[1],result.f[2],result.f[3]);
            printe("    icheck.X: %f icheck.Y: %f icheck.Z: %f icheck.W: %f\n",           XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
            r=MATH_FAIL;
        }
        result.v = XMConvertVectorIntToFloat(iv[n],1);
        check = XMVectorMultiply(icheck[n],vDivisor2);
        cc = CompareXMVECTOR(result,check,4);
        if(cc != EXACT) {
            printe("%s: v.X:      %d v.Y:      %d      v.Z: %d      v.W: %d\n", TestName, iv[n].i[0],iv[n].i[1],iv[n].i[2],iv[n].i[3]);
            printe("(1) result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           result.f[0],result.f[1],result.f[2],result.f[3]);
            printe("    icheck.X: %f icheck.Y: %f icheck.Z: %f icheck.W: %f\n",           XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
            r=MATH_FAIL;
        }
        result.v = XMConvertVectorIntToFloat(iv[n],4);
        check = XMVectorMultiply(icheck[n],vDivisor16);
        cc = CompareXMVECTOR(result,check,4);
        if(cc != EXACT) {
            printe("%s: v.X:      %d v.Y:      %d      v.Z: %d      v.W: %d\n", TestName, iv[n].i[0],iv[n].i[1],iv[n].i[2],iv[n].i[3]);
            printe("(4) result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           result.f[0],result.f[1],result.f[2],result.f[3]);
            printe("    icheck.X: %f icheck.Y: %f icheck.Z: %f icheck.W: %f\n",           XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
            r=MATH_FAIL;
        }     
        result.v = XMConvertVectorIntToFloat(iv[n],31);
        check = XMVectorMultiply(icheck[n],vDivisor31);
        cc = CompareXMVECTOR(result,check,4);
        if(cc != EXACT) {
            printe("%s: v.X:      %d v.Y:      %d      v.Z: %d      v.W: %d\n", TestName, iv[n].i[0],iv[n].i[1],iv[n].i[2],iv[n].i[3]);
            printe("(31)result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           result.f[0],result.f[1],result.f[2],result.f[3]);
            printe("    icheck.X: %f icheck.Y: %f icheck.Z: %f icheck.W: %f\n",           XMVectorGetX(check),XMVectorGetY(check),XMVectorGetZ(check),XMVectorGetW(check));
            r=MATH_FAIL;
        }     
    }
    return r;
}

HRESULT Test568(LogProxy* pLog)
{
//XMConvertVectorFloatToUInt and XMConvertVectorFloatToInt
    HRESULT r = S_OK;
    XMVECTORI32 result;
    int n;

    //XMConvertVectorFloatToUInt
    static const XMVECTORF32 uv[] =         {{0,0.25f,0.5f,0.75f},                  {127.0f,-127.0f,1.0f,-1.0f},        {1024.0f, 2048.0f, -1024.0f, -2048.0f},     {65535.0f, 65535.0f, 0, 1.0f},              {65536.0f*32768.0f,65536.0f*32768.0f,65536.0f*65536.0f-256.0f,65536.0f*65536.0f}};
    static const uint32_t ucheck0[][4] =    {{0,0,0,0},                             {127,0,1,0},                        {1024, 2048, 0, 0},                         {65535, 65535, 0, 1},                       {0x80000000,0x80000000,0xFFFFFF00U,0xFFFFFFFFU}};
    static const uint32_t ucheck1[][4] =    {{0,0,1,1},                             {127<<1,0<<1,1<<1,0<<1},            {1024<<1, 2048<<1, 0<<1, 0<<1},             {65535<<1, 65535<<1, 0<<1, 1<<1},           {0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU}};
    static const uint32_t ucheck4[][4] =    {{0,4,8,12},                            {127<<4,0<<4,1<<4,0<<4},            {1024<<4, 2048<<4, 0<<4, 0<<4},             {65535<<4, 65535<<4, 0<<4, 1<<4},           {0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU}};
    static const uint32_t ucheck31[][4]=    {{0,0x20000000,0x40000000,0x60000000},  {0xFFFFFFFFU,0<<31,1U<<31,0<<31},   {0xFFFFFFFFU, 0xFFFFFFFFU, 0<<31, 0<<31},   {0xFFFFFFFFU,0xFFFFFFFFU, 0<<31, 1U<<31},   {0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU}};

    for(n = 0; n < countof(uv); n++) {   
        result.v = XMConvertVectorFloatToUInt(uv[n], 0);
        if( (unsigned)result.i[0]!= ucheck0[n][0] ||
            (unsigned)result.i[1]!= ucheck0[n][1] ||
            (unsigned)result.i[2]!= ucheck0[n][2] ||
            (unsigned)result.i[3]!= ucheck0[n][3] 
            ) {
            printe("%s: v.X:      %f v.Y:      %f v.Z:      %f v.W:      %f\n", TestName, uv[n].f[0],uv[n].f[1],uv[n].f[2],uv[n].f[3]);
            printe("(0) ucheck.X: %u ucheck.Y: %u ucheck.Z: %u ucheck.W: %u\n",           ucheck0[n][0],ucheck0[n][1],ucheck0[n][2],ucheck0[n][3]);
            printe("    result.X: %u result.Y: %u result.Z: %u result.W: %u\n",           result.i[0],result.i[1],result.i[2],result.i[3]);
            r=MATH_FAIL;
        }
        result.v = XMConvertVectorFloatToUInt(uv[n], 1);
        if ((unsigned)result.i[0]!= ucheck1[n][0] ||
            (unsigned)result.i[1]!= ucheck1[n][1] ||
            (unsigned)result.i[2]!= ucheck1[n][2] ||
            (unsigned)result.i[3]!= ucheck1[n][3] 
            ) {
            printe("%s: v.X:      %f v.Y:      %f v.Z:      %f v.W:      %f\n", TestName, uv[n].f[0],uv[n].f[1],uv[n].f[2],uv[n].f[3]);
            printe("(1) ucheck.X: %u ucheck.Y: %u ucheck.Z: %u ucheck.W: %u\n",           ucheck1[n][0],ucheck1[n][1],ucheck1[n][2],ucheck1[n][3]);
            printe("    result.X: %u result.Y: %u result.Z: %u result.W: %u\n",           result.i[0],result.i[1],result.i[2],result.i[3]);
            r=MATH_FAIL;
        }
        result.v = XMConvertVectorFloatToUInt(uv[n], 4);
        if( (unsigned)result.i[0]!= ucheck4[n][0] ||
            (unsigned)result.i[1]!= ucheck4[n][1] ||
            (unsigned)result.i[2]!= ucheck4[n][2] ||
            (unsigned)result.i[3]!= ucheck4[n][3] 
            ) {
            printe("%s: v.X:      %f v.Y:      %f v.Z:      %f v.W:      %f\n", TestName, uv[n].f[0],uv[n].f[1],uv[n].f[2],uv[n].f[3]);
            printe("(4) ucheck.X: %u ucheck.Y: %u ucheck.Z: %u ucheck.W: %u\n",           ucheck4[n][0],ucheck4[n][1],ucheck4[n][2],ucheck4[n][3]);
            printe("    result.X: %u result.Y: %u result.Z: %u result.W: %u\n",           result.i[0],result.i[1],result.i[2],result.i[3]);
            r=MATH_FAIL;
        }
        result.v = XMConvertVectorFloatToUInt(uv[n],31);
        if( (unsigned)result.i[0]!= ucheck31[n][0] ||
            (unsigned)result.i[1]!= ucheck31[n][1] ||
            (unsigned)result.i[2]!= ucheck31[n][2] ||
            (unsigned)result.i[3]!= ucheck31[n][3] 
            ) {
            printe("%s: v.X:      %f v.Y:      %f v.Z:      %f v.W:      %f\n", TestName, uv[n].f[0],uv[n].f[1],uv[n].f[2],uv[n].f[3]);
            printe("(31)ucheck.X: %u ucheck.Y: %u ucheck.Z: %u ucheck.W: %u\n",           ucheck31[n][0],ucheck31[n][1],ucheck31[n][2],ucheck31[n][3]);
            printe("    result.X: %u result.Y: %u result.Z: %u result.W: %u\n",           result.i[0],result.i[1],result.i[2],result.i[3]);
            r=MATH_FAIL;
        }
    }
    
    
    //XMConvertVectorFloatToInt
    static const XMVECTORF32 iv[]     = {{0,0.25f,0.5f,0.75f},                  {-1.0f,-0.25f,-0.5f,-0.75f},                                 {127.0f,-127.0f,1.0f,-1.0f},                                      {1024.0f, 2048.0f, -1024.0f, -2048.0f},                           {65535.0f, 65535.0f, 0, 1.0f},              {65536.0f*16384.0f,-65536.0f*16384.0f,65536.0f*32768-128.0f,-65536.0f*65536.0f}};
    static const int32_t icheck0[][4] = {{0,0,0,0},                             {-1,0,0,0},                                                  {127,-127,1,-1},                                                  {1024, 2048, -1024, -2048},                                       {65535, 65535, 0, 1},                       {0x40000000,-0x40000000,0x7FFFFF80,int32_t(0x80000000)}};
    static const int32_t icheck1[][4] = {{0,0,1,1},                             {-2,0,-1,-1},                                                {127<<1,-(127<<1),1<<1,-(1<<1)},                                  {1024<<1, 2048<<1, -(1024<<1), -(2048<<1)},                       {65535<<1, 65535<<1, 0<<1, 1<<1},           {0x7FFFFFFF,int32_t(0x80000000),0x7FFFFFFF,int32_t(0x80000000)}};
    static const int32_t icheck4[][4] = {{0,4,8,12},                            {-16,-4,-8,-12},                                             {127<<4,-(127<<4),1<<4,-(1<<4)},                                  {1024<<4, 2048<<4, -(1024<<4), -(2048<<4)},                       {65535<<4, 65535<<4, 0<<4, 1<<4},           {0x7FFFFFFF,int32_t(0x80000000),0x7FFFFFFF,int32_t(0x80000000)}};
    static const int32_t icheck31[][4] ={{0,0x20000000,0x40000000,0x60000000},  {int32_t(0x80000000),-0x20000000,-0x40000000,-0x60000000},   {0x7FFFFFFF,int32_t(0x80000000),0x7FFFFFFF,int32_t(0x80000000)},  {0x7FFFFFFF,0x7FFFFFFF,int32_t(0x80000000),int32_t(0x80000000)},  {0x7FFFFFFF,0x7FFFFFFF,0<<31,0x7FFFFFFF},   {0x7FFFFFFF,int32_t(0x80000000),0x7FFFFFFF,int32_t(0x80000000)}};
    
    for(n = 0; n < countof(iv); n++) {   
        result.v = XMConvertVectorFloatToInt(iv[n], 0);
        if( result.i[0]!= icheck0[n][0] ||
            result.i[1]!= icheck0[n][1] ||
            result.i[2]!= icheck0[n][2] ||
            result.i[3]!= icheck0[n][3] 
            ) {
            printe("%s: v.X:      %f v.Y:      %f v.Z:      %f v.W:      %f\n", TestName, iv[n].f[0],iv[n].f[1],iv[n].f[2],iv[n].f[3]);
            printe("(0) icheck.X: %d icheck.Y: %d icheck.Z: %d icheck.W: %d\n",           icheck0[n][0],icheck0[n][1],icheck0[n][2],icheck0[n][3]);
            printe("    result.X: %d result.Y: %d result.Z: %d result.W: %d\n",           result.i[0],result.i[1],result.i[2],result.i[3]);
            r=MATH_FAIL;
        }
        result.v = XMConvertVectorFloatToInt(iv[n], 1);
        if( result.i[0]!= icheck1[n][0] ||
            result.i[1]!= icheck1[n][1] ||
            result.i[2]!= icheck1[n][2] ||
            result.i[3]!= icheck1[n][3] 
            ) {
            printe("%s: v.X:      %f v.Y:      %f v.Z:      %f v.W:      %f\n", TestName, iv[n].f[0],iv[n].f[1],iv[n].f[2],iv[n].f[3]);
            printe("(1) icheck.X: %d icheck.Y: %d icheck.Z: %d icheck.W: %d\n",           icheck1[n][0],icheck1[n][1],icheck1[n][2],icheck1[n][3]);
            printe("    result.X: %d result.Y: %d result.Z: %d result.W: %d\n",           result.i[0],result.i[1],result.i[2],result.i[3]);
            r=MATH_FAIL;
        }
        result.v = XMConvertVectorFloatToInt(iv[n], 4);
        if( result.i[0]!= icheck4[n][0] ||
            result.i[1]!= icheck4[n][1] ||
            result.i[2]!= icheck4[n][2] ||
            result.i[3]!= icheck4[n][3] 
            ) {
            printe("%s: v.X:      %f v.Y:      %f v.Z:      %f v.W:      %f\n", TestName, iv[n].f[0],iv[n].f[1],iv[n].f[2],iv[n].f[3]);
            printe("(4) icheck.X: %d icheck.Y: %d icheck.Z: %d icheck.W: %d\n",           icheck4[n][0],icheck4[n][1],icheck4[n][2],icheck4[n][3]);
            printe("    result.X: %d result.Y: %d result.Z: %d result.W: %d\n",           result.i[0],result.i[1],result.i[2],result.i[3]);
            r=MATH_FAIL;
        }
        result.v = XMConvertVectorFloatToInt(iv[n],31);
        if( result.i[0]!= icheck31[n][0] ||
            result.i[1]!= icheck31[n][1] ||
            result.i[2]!= icheck31[n][2] ||
            result.i[3]!= icheck31[n][3] 
            ) {
            printe("%s: v.X:      %f v.Y:      %f v.Z:      %f v.W:      %f\n", TestName, iv[n].f[0],iv[n].f[1],iv[n].f[2],iv[n].f[3]);
            printe("(31)icheck.X: %d icheck.Y: %d icheck.Z: %d icheck.W: %d\n",           icheck31[n][0],icheck31[n][1],icheck31[n][2],icheck31[n][3]);
            printe("    result.X: %d result.Y: %d result.Z: %d result.W: %d\n",           result.i[0],result.i[1],result.i[2],result.i[3]);
            r=MATH_FAIL;
        }
    }
    return r;
}
 
HRESULT Test569(LogProxy* pLog)
{
//XMVectorInsert
    HRESULT r = S_OK;
    XMVECTORF32 v[] = {{0,0,0,0},{65535.0f,-65535.0f,1.0f,-1.0f},{1024.0f, 2048.0f, -1024.0f, -2048.0f},{0.5f, 0.25f, -0.5f, -0.25f,}};

    int n = 0;
    for(n = 0; n < countof(v); n++) {
        XMVECTOR vd = v[n];
        XMVECTOR vs = v[(n+1)%countof(v)];

        // Function form
        {
            uint32_t select0[4] = { 11, 0, 11, 0 };
            uint32_t select1[4] = { 22, 1024, 22, 1024 };
            uint32_t select2[4] = { 33, 65535, 33, 65535 };
            uint32_t select3[4] = { 44, 1, 44, 1 };
            for( uint32_t i =0; i < 4; ++i )
            {
                XMVECTOR result = XMVectorInsert(vd,vs,i,select0[i],select1[i],select2[i],select3[i]);
                XMVECTOR check =  XMVectorSelect((vd), XMVectorRotateLeft((vs), i),
                                                        XMVectorSelectControl((select0[i]) & 1, (select1[i]) & 1, (select2[i]) & 1, (select3[i]) & 1));
                COMPARISON cc = CompareXMVECTOR(result,check,4);
                if(cc > EXACT) {
                    printe("%s: %d  v.X:      %f v.Y:      %f      v.Z: %f      v.W: %f\n", TestName, i, XMVectorGetX(v[n]),   XMVectorGetY(v[n]),   XMVectorGetZ(v[n]),   XMVectorGetW(v[n]));
                    printe("        result.X: %f result.Y: %f result.Z: %f result.W: %f\n",              XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result), XMVectorGetW(result));
                    printe("        check.X:  %f check.Y:  %f check.Z:  %f check.W:  %f\n",              XMVectorGetX(check),  XMVectorGetY(check),  XMVectorGetZ(check),  XMVectorGetW(check));
                    r=MATH_FAIL;
                }
            }
        }

        // Template form
        {
            XMVECTOR result = XMVectorInsert<0,11,22,33,44>(vd,vs);
            XMVECTOR check =  XMVectorSelect((vd), XMVectorRotateLeft((vs), 0), XMVectorSelectControl((11) & 1, (22) & 1, (33) & 1, (44) & 1));
            COMPARISON cc = CompareXMVECTOR(result,check,4);
            if(cc > EXACT) {
                printe("%s: <0> v.X:      %f v.Y:      %f      v.Z: %f      v.W: %f\n", TestName, XMVectorGetX(v[n]),   XMVectorGetY(v[n]),   XMVectorGetZ(v[n]),   XMVectorGetW(v[n]));
                printe("        result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result), XMVectorGetW(result));
                printe("        check.X:  %f check.Y:  %f check.Z:  %f check.W:  %f\n",           XMVectorGetX(check),  XMVectorGetY(check),  XMVectorGetZ(check),  XMVectorGetW(check));
                r=MATH_FAIL;
            }
        }

        {
            XMVECTOR result = XMVectorInsert<1,0,1024,65535,1>(vd,vs);
            XMVECTOR check =  XMVectorSelect((vd), XMVectorRotateLeft((vs), 1),
                                                    XMVectorSelectControl((0) & 1, (1024) & 1, (65535) & 1, (1) & 1));
            COMPARISON cc = CompareXMVECTOR(result,check,4);
            if(cc > EXACT) {
                printe("%s: <1> v.X:      %f v.Y:      %f      v.Z: %f      v.W: %f\n", TestName, XMVectorGetX(v[n]),   XMVectorGetY(v[n]),   XMVectorGetZ(v[n]),   XMVectorGetW(v[n]));
                printe("        result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result), XMVectorGetW(result));
                printe("        check.X:  %f check.Y:  %f check.Z:  %f check.W:  %f\n",           XMVectorGetX(check),  XMVectorGetY(check),  XMVectorGetZ(check),  XMVectorGetW(check));
                r=MATH_FAIL;
            }
        }

        {
            XMVECTOR result = XMVectorInsert<2,11,22,33,44>(vd,vs);
            XMVECTOR check =  XMVectorSelect((vd), XMVectorRotateLeft((vs), 2),
                                                   XMVectorSelectControl((11) & 1, (22) & 1, (33) & 1, (44) & 1));
            COMPARISON cc = CompareXMVECTOR(result,check,4);
            if(cc > EXACT) {
                printe("%s: <2> v.X:      %f v.Y:      %f      v.Z: %f      v.W: %f\n", TestName, XMVectorGetX(v[n]),   XMVectorGetY(v[n]),   XMVectorGetZ(v[n]),   XMVectorGetW(v[n]));
                printe("        result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result), XMVectorGetW(result));
                printe("        check.X:  %f check.Y:  %f check.Z:  %f check.W:  %f\n",           XMVectorGetX(check),  XMVectorGetY(check),  XMVectorGetZ(check),  XMVectorGetW(check));
                r=MATH_FAIL;
            }
        }

        {
            XMVECTOR result = XMVectorInsert<3,0,1024,65535,1>(vd,vs);
            XMVECTOR check =  XMVectorSelect((vd), XMVectorRotateLeft((vs), 3),
                                                   XMVectorSelectControl((0) & 1, (1024) & 1, (65535) & 1, (1) & 1));
            COMPARISON cc = CompareXMVECTOR(result,check,4);
            if(cc > EXACT) {
                printe("%s: <3> v.X:      %f v.Y:      %f      v.Z: %f      v.W: %f\n", TestName, XMVectorGetX(v[n]),   XMVectorGetY(v[n]),   XMVectorGetZ(v[n]),   XMVectorGetW(v[n]));
                printe("        result.X: %f result.Y: %f result.Z: %f result.W: %f\n",           XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result), XMVectorGetW(result));
                printe("        check.X:  %f check.Y:  %f check.Z:  %f check.W:  %f\n",           XMVectorGetX(check),  XMVectorGetY(check),  XMVectorGetZ(check),  XMVectorGetW(check));
                r=MATH_FAIL;
            }
        }

    }
    return r;
}

HRESULT Test576(LogProxy* pLog)
{
//XMLoadFloat / XMLoadInt

    // TODO - Check for zeroing of partial loads

    char c[64];
    int offset = 16;
    int i, j; 
    HRESULT r = S_OK;

    for(j = 0; j < 16; j+=4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }
        int first = offset+j;
        int last  = offset+j+ 4;

        float x = j*4;

        WriteFloat(x, (char*)&c[first]);

        XMVECTOR v = XMLoadFloat((const FLOAT*)&c[offset + j]);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        float f = ReadFloat((char*)&(c[first]));
        if(f != x) {
            printe("%s: %d corrupted input data! %f ... %f\n", TestName, j,f,x);
            r=MATH_FAIL;
        }
        if(XMVectorGetX(v) != x) {
            printe("%s: %d corrupted output float %f ... %f\n", TestName, j, XMVectorGetX(v),x);
            r=MATH_FAIL;
        }
    }

    for(j = 0; j < 16; j+=4)
    {
        for(i = 0; i < sizeof(c); i++) { c[i] = (char)(~i & 0xff); }
        int first = offset+j;
        int last  = offset+j+ 4;

        uint32_t xx=j*4;

        WriteInt(xx, (char*)&c[first]);

        XMVECTOR v = XMLoadInt((const uint32_t*)&c[offset + j]);

        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }

        for(i = last; i < sizeof(c); i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %d corrupted input byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        uint32_t x = ReadInt((char*)&(c[first]));
        if(x != xx)
        {
            printe("%s: %d corrupted input data! %i ... %i\n", TestName, j,x,xx);
            r=MATH_FAIL;
        }

        if(XMVectorGetIntX(v) != xx)
        {
            printe("%s: %d corrupted output int %i ... %i\n", TestName, j,XMVectorGetIntX(v),x);
            r=MATH_FAIL;
        }
    }

    return r;
}

HRESULT Test577(LogProxy* pLog)
{
//XMStoreFloat / XMStoreInt
    char c[64];
    int offset = 16;
    int i, j; 
    XMVECTORF32 v = {{1,2,3,4}};
    XMVECTORF32 v1 = {{4,4,4,4}};
    HRESULT r = S_OK;

    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4;

        XMStoreFloat((FLOAT*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        float f = ReadFloat((char*)&(c[first]));
        if(f != XMVectorGetX(v)) {
            printe("%s: %d corrupted float %f ... %f\n", TestName, j,f,XMVectorGetX(v));
            r=MATH_FAIL;
        }
        v.v = XMVectorAdd(v.v, v1);
    }

    uint32_t vi[4] = {1,2,3,4};       // Can't be const
    static const uint32_t vi1[4] = {4,4,4,4};
    for(j = 0; j < 16; j += 4) {
        for(i = 0; i < sizeof(c); i++) {
            c[i] = (char)(~i & 0xff);
        }

        int first = offset+j;
        int last  = offset+j+ 4;

        v.v = XMVectorSetInt( vi[0], vi[1], vi[2], vi[3] );
        XMStoreInt((uint32_t*)&c[offset + j], v);

        for(i = 0; i < first; i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < sizeof(c); i++) {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %d corrupted byte %d: %x ... %x\n", TestName, j,i,c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        uint32_t ii = ReadInt((char*)&(c[first]));
        if(ii != XMVectorGetIntX(v)) {
            printe("%s: %d corrupted int %d ... %d\n", TestName, j,ii,XMVectorGetIntX(v));
            r=MATH_FAIL;
        }
        vi[0] += vi1[0];
        vi[1] += vi1[1];
        vi[2] += vi1[2];
        vi[3] += vi1[3];
    }
    return r;
}

#pragma warning(disable:4127)

HRESULT Test582(LogProxy* pLog)
{
// XMLoadU565
    static_assert( sizeof(XMU565) == 2, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMU565>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMU565>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMU565>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMU565>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XMVECTOR v;
    HRESULT ret = S_OK;

    uint16_t s[] = {0,0x1f+(0x3f<<5)+(0x1f<<11),0x10+(0x20<<5)+(0x10<<11),0x0+(0x3f<<5)+(0x10<<11)};
    XMVECTORF32 check[] = {{{0,0,0, _Q_NAN}},{{31,63,31,_Q_NAN}},{{16,32,16, _Q_NAN}},{{0,63,16, _Q_NAN}}};
    for(int k = 0; k < countof(s); k++)
    {
        v = XMLoadU565((const XMU565*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],3);
        if(cc != EXACT) {
            printe("%s [%d]: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                   TestName, k, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                   XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++)
    {
        uint32_t a = rand() & 0x1f;
        uint32_t b = rand() & 0x3f;
        uint32_t c = rand() & 0x1f;
        XMU565 src;
        XMVECTORF32 chk= {{ ((float)(a&0x1f)),
                         ((float)(b&0x3f)),
                         ((float)(c&0x1f)),
                      }};

        src.x = uint16_t(a); src.y = uint16_t(b); src.z = uint16_t(c);

        v = XMLoadU565(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,3);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                   TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                   XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test583(LogProxy* pLog)
{
// XMStoreU565
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);

    uint16_t u;
    intptr_t i, j; 
    HRESULT r = S_OK;

    uint16_t check[4] = {0,0x1f+(0x3f<<5)+(0x1f<<11),0x10+(0x20<<5)+(0x10<<11),0x0+(0x3f<<5)+(0x10<<11)};
    const XMVECTORF32 v[] = {{{0,0,0, _Q_NAN}},{{31,63,31,_Q_NAN}},{{16,32,16, _Q_NAN}},{{0,63,16, _Q_NAN}}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4)
    {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 2;

        XMStoreU565((XMU565*)&c[j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        u = *((const uint16_t *)&(c[first]));
        if(u != check[n]) {
            printe("%s: %p corrupted short: %x ... %x\n", TestName, reinterpret_cast<void*>(j),u,check[n]);
            r=MATH_FAIL;
        }
    }

    free(c);
    return r;
}

HRESULT Test584(LogProxy* pLog)
{
// XMLoadUNibble4
    static_assert( sizeof(XMUNIBBLE4) == 2, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUNIBBLE4>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUNIBBLE4>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUNIBBLE4>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUNIBBLE4>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;

    uint16_t s[] = {0,0xffff,0x8888,0x08f0};
    XMVECTORF32 check[] = {{{0,0,0,0}},{{15,15,15,15}},{{8,8,8,8}},{{0,15,8,0}}};
    for(int k = 0; k < countof(s); k++)
    {
        v = XMLoadUNibble4((const XMUNIBBLE4*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s [%d]: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                   TestName, k, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                   XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++)
    {
        uint32_t a = rand() & 0xf;
        uint32_t b = rand() & 0xf;
        uint32_t c = rand() & 0xf;
        uint32_t d = rand() & 0xf;
        XMUNIBBLE4 src;
        XMVECTORF32 chk= {{ ((float)(a&0xf)),
                         ((float)(b&0xf)),
                         ((float)(c&0xf)),
                         ((float)(d&0xf))
                      }};

        src.x = uint16_t(a); src.y = uint16_t(b); src.z = uint16_t(c); src.w = uint16_t(d);

        v = XMLoadUNibble4(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                   TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                   XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test585(LogProxy* pLog)
{
// XMStoreUNibble4
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);

    uint16_t u;
    intptr_t i, j; 
    HRESULT r = S_OK;

    uint16_t check[4] = {0,0xffff,0x8888,0x08f0};
    const XMVECTORF32 v[] = {{{0,0,0,0}},{{15,15,15,15}},{{8,8,8,8}},{{0,15,8,0}}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4)
    {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 2;

        XMStoreUNibble4((XMUNIBBLE4*)&c[j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        u = *((const uint16_t *)&(c[first]));
        if(u != check[n]) {
            printe("%s: %p corrupted short: %x ... %x\n", TestName, reinterpret_cast<void*>(j),u,check[n]);
            r=MATH_FAIL;
        }
    }

    free(c);
    return r;
}

HRESULT Test586(LogProxy* pLog)
{
// XMLoadU555
    static_assert( sizeof(XMU555) == 2, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMU555>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMU555>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMU555>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMU555>::value, "Move Assign.");

    XMVECTOR v;
    HRESULT ret = S_OK;

    uint16_t s[] = {0,0x1f+(0x1f<<5)+(0x1f<<10)+0x8000,0x10+(0x10<<5)+(0x10<<10),0x0+(0x1f<<5)+(0x10<<10)};
    XMVECTORF32 check[] = {{{0,0,0,0}},{{31,31,31,1}},{{16,16,16,0}},{{0,31,16,0}}};
    for(int k = 0; k < countof(s); k++)
    {
        v = XMLoadU555((const XMU555*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],4);
        if(cc != EXACT) {
            printe("%s [%d]: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                   TestName, k, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                   XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),XMVectorGetW(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; k++)
    {
        uint32_t a = rand() & 0x1f;
        uint32_t b = rand() & 0x1f;
        uint32_t c = rand() & 0x1f;
        BOOL d = rand() & 0x1;
        XMU555 src;
        XMVECTORF32 chk= {{ ((float)(a&0x1f)),
                         ((float)(b&0x1f)),
                         ((float)(c&0x1f)),
                         (d) ? 1.0f : 0.f
                      }};

        src.x = uint16_t(a); src.y = uint16_t(b); src.z = uint16_t(c); src.w = uint16_t(d);

        v = XMLoadU555(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,4);
        if(cc > WITHIN10EPSILON) {
            printe("%s: %x -> %f %f %f %f ... %f %f %f %f (%d)\n",
                   TestName, *((const uint32_t*)&src), XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),XMVectorGetW(v),
                   XMVectorGetX(chk),XMVectorGetY(chk),XMVectorGetZ(chk),XMVectorGetW(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test587(LogProxy* pLog)
{
// XMStoreU555
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);

    uint16_t u;
    intptr_t i, j; 
    HRESULT r = S_OK;

    uint16_t check[4] = {0,0x1f+(0x1f<<5)+(0x1f<<10)+0x8000,0x10+(0x10<<5)+(0x10<<10),0x0+(0x1f<<5)+(0x10<<10)};
    const XMVECTORF32 v[] = {{{0,0,0,0}},{{31,31,31,1}},{{16,16,16,0}},{{0,31,16,0}}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4)
    {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 2;

        XMStoreU555((XMU555*)&c[j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        u = *((const uint16_t *)&(c[first]));
        if(u != check[n]) {
            printe("%s: %p corrupted short: %x ... %x\n", TestName, reinterpret_cast<void*>(j),u,check[n]);
            r=MATH_FAIL;
        }
    }

    free(c);
    return r;
}

HRESULT Test588(LogProxy* pLog)
{
// XMLoadFloat3PK
    static_assert( sizeof(XMFLOAT3PK) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT3PK>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT3PK>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT3PK>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT3PK>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XMVECTOR v;
    HRESULT ret = S_OK;

    uint32_t s[] = {0,0xffffffff,0x842003c0,0x212c25};
    XMVECTORF32 check[] = {{{0,0,0,0}},{{_Q_NAN,_Q_NAN,_Q_NAN,0}},{{1,2,3,0}},{{3.15625f,3.15625f,0,0}}};
    for(int k = 0; k < countof(s); k++)
    {
        v = XMLoadFloat3PK((const XMFLOAT3PK*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],3);
        if(cc != EXACT) {
            printe("%s [%d]: %x -> %f %f %f ... %f %f %f (%d)\n",
                   TestName, k, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),
                   XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test589(LogProxy* pLog)
{
// XMStoreFloat3PK
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);

    uint32_t u;
    intptr_t i, j; 
    HRESULT r = S_OK;

    uint32_t check[4] = {0,0xffffffff,0x842003c0,0x212c25};
    const XMVECTORF32 v[] = {{{0,0,0,0}},{{_Q_NAN,_Q_NAN,_Q_NAN,0}},{{1,2,3,0}},{{XM_PI,XM_PI,-XM_PI,0}}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4)
    {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreFloat3PK((XMFLOAT3PK*)&c[j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        u = *((const uint32_t *)&(c[first]));
        if(u != check[n]) {
            XMVECTOR T = XMLoadFloat3PK( (const XMFLOAT3PK*)&(c[first]) );
            printe("%s: %p corrupted int %d: %x (%f %f %f) ... %x\n", TestName, reinterpret_cast<void*>(j), n, u,
                   XMVectorGetX(T), XMVectorGetY(T), XMVectorGetZ(T), check[n]);
            r=MATH_FAIL;
        }

    }

    free(c);
    return r;
}

HRESULT Test590(LogProxy* pLog)
{
// XMLoadFloat3SE
    static_assert( sizeof(XMFLOAT3SE) == 4, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMFLOAT3SE>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMFLOAT3SE>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMFLOAT3SE>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMFLOAT3SE>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    XMVECTOR v;
    HRESULT ret = S_OK;

    uint32_t s[] = {0,
                    0x04020100,
                    0x7C020100,
                    0x84020100,
                    0xBFFBFDFE,
                    0xFFFFFFFF,
                    0xBC020100,
                    0xFC020100,
                    0x84CC6680,
                    0xa8000100,
                    0x8e020080,
                    0xd7e0f008};
    XMVECTORF32 check[] = { {0.0f, 0.0f, 0.0f, 1.f},
                            {1.f/65536.f, 1.f/65536.f, 1.f/65536.f, 1.f},
                            {0.5f, 0.5f, 0.5f, 1.0f},
                            {1.0f, 1.0f, 1.0f, 1.0f},
                            {255.0, 255.0, 255.0, 1.0f},
                            {65408.f, 65408.f, 65408.f, 1.0f},
                            {128.0, 128.0, 128.0, 1.0f},
                            {32768.f, 32768.f, 32768.f, 1.0f},
                            {0.5f, 0.199219f, 1.199219f, 1.0f},
                            {32.f, 0.f, 0.f, 1.f},
                            {1.f, 2.f, 3.f, 1.f},
                            {32.f, 480.f, 2016.f, 1.f} };

    static_assert( std::size(s) == std::size(check), "bad test" );

    for(int k = 0; k < countof(s); k++)
    {
        v = XMLoadFloat3SE((const XMFLOAT3SE*)&s[k]);
        COMPARISON cc = CompareXMVECTOR(v,check[k],3);
        if(cc > WITHIN4096) {
            printe("%s [%d]: %x -> %f %f %f ... %f %f %f (%d)\n",
                   TestName, k, s[k],XMVectorGetX(v),XMVectorGetY(v),XMVectorGetZ(v),
                   XMVectorGetX(check[k]),XMVectorGetY(check[k]),XMVectorGetZ(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test591(LogProxy* pLog)
{
// XMStoreFloat3SE
    int csize = 1024+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(c); 
    pc64k = 65536 - (pc64k & 65535);

    uint32_t u;
    intptr_t i, j; 
    HRESULT r = S_OK;

    uint32_t check[] = {0,
                        0x04020100,
                        0x7C020100,
                        0x84020100,
                        0xBFFBFDFE,
                        0xFFFFFFFF,
                        0xBC020100,
                        0xFC020100,
                        0x84CC6680,
                        0xa8000100,
                        0x8e020080,
                        0xd7e0f008};
    XMVECTORF32 v[] = { {0.0f, 0.0f, 0.0f, 1.f},
                        {1.f/65536.f, 1.f/65536.f, 1.f/65536.f, 1.f},
                        {0.5f, 0.5f, 0.5f, 1.0f},
                        {1.0f, 1.0f, 1.0f, 1.0f},
                        {255.0, 255.0, 255.0, 1.0f},
                        {65408.f, 65408.f, 65408.f, 1.0f},
                        {128.0, 128.0, 128.0, 1.0f},
                        {32768.f, 32768.f, 32768.f, 1.0f},
                        {0.5f, 0.199219f, 1.199219f, 1.0f},
                        {32.f, 0.f, 0.f, 1.f},
                        {1.f, 2.f, 3.f, 1.f},
                        {32.f, 480.f, 2016.f, 1.f} };

    static_assert( std::size(v) == std::size(check), "bad test" );

    int n = 0;
    for(j = pc64k - 64; j <= pc64k + 64; j+=4)
    {
        n = (n + 1) % (std::size(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = j;
        intptr_t last  = j + 4;

        XMStoreFloat3SE((XMFLOAT3SE*)&c[j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff)) {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        u = *((const uint32_t *)&(c[first]));
        if(u != check[n]) {
            XMVECTOR T = XMLoadFloat3SE( (const XMFLOAT3SE*)&(c[first]) );
            printe("%s: %p corrupted int %d : %x (%f %f %f) ... %x\n", TestName, reinterpret_cast<void*>(j), n,u,
                   XMVectorGetX(T), XMVectorGetY(T), XMVectorGetZ(T), check[n]);
            r=MATH_FAIL;
        }
    }

    free(c);
    return r;
}

HRESULT Test595(LogProxy* pLog)
{
// XMLoadByteN2
    static_assert( sizeof(XMBYTEN2) == 2, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMBYTEN2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMBYTEN2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMBYTEN2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMBYTEN2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    HRESULT ret = S_OK;

    static const char data[4][2] = { {0,0}, {127,127}, {-127,-127}, {64,-64} };
    static const XMVECTORF32 check[] = { {0,0,0,0},{1.f,1.f,0,0},{-1.f,-1.f,0,0},{0.5039370078f,-0.5039370078f,0,0}};

    for(int k = 0; k < 4; ++k)
    {
        XMBYTEN2 src;
        src.x=data[k][0]; src.y=data[k][1]; 
        XMVECTOR v = XMLoadByteN2(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],2);
        if(cc > WITHIN10EPSILON)
        {
            printe("%s: %d,%d -> %f %f ... %f %f (%d)\n",
                TestName, data[k][0], data[k][1], XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; ++k)
    {
        char a = (char)(rand() & 0xff);
        char b = (char)(rand() & 0xff);

        if ( a == -128 ) a = 0;
        if ( b == -128 ) b = 0;

        XMVECTORF32 chk= {(float)(a/127.f), (float)(b/127.f), _Q_NAN, _Q_NAN};

        XMBYTEN2 src;
        src.x=a; src.y=b; 
        XMVECTOR v = XMLoadByteN2(&src);

        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON)
        {
            printe("%s: %02X,%02X -> %f %f ... %f %f (%d)\n",
                TestName, a, b, XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

// Special-case of -128
    {
        XMBYTEN2 src;
        src.x = src.y = -128;
        XMVECTORF32 chk = { -1.f, -1.f, 0, 0 };
        XMVECTOR v = XMLoadByteN2(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON) {
            printe("%s: 80,80 -> %f %f ... %f %f (%d)\n",
                TestName, XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test596(LogProxy* pLog)
{
// XMLoadByte2
    static_assert( sizeof(XMBYTE2) == 2, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMBYTE2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMBYTE2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMBYTE2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMBYTE2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    HRESULT ret = S_OK;

    static const char data[4][2] = { {0,0}, {127,127}, {-127,-127}, {64,-64} };
    static const XMVECTORF32 check[] = { {0,0,0,0},{127.f,127.f,0,0},{-127.f,-127.f,0,0},{64.f,-64.f,0,0}};

    for(int k = 0; k < 4; ++k)
    {
        XMBYTE2 src;
        src.x=data[k][0]; src.y=data[k][1]; 
        XMVECTOR v = XMLoadByte2(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],2);
        if(cc != EXACT)
        {
            printe("%s: %d,%d -> %f %f ... %f %f (%d)\n",
                TestName, data[k][0], data[k][1], XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; ++k)
    {
        char a = (char)(rand() & 0xff);
        char b = (char)(rand() & 0xff);

        XMVECTORF32 chk= {(float)a, (float)b, _Q_NAN, _Q_NAN};

        XMBYTE2 src;
        src.x=a; src.y=b; 
        XMVECTOR v = XMLoadByte2(&src);

        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON)
        {
            printe("%s: %02X,%02X -> %f %f ... %f %f (%d)\n",
                TestName, a, b, XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

// Special-case of -128
    {
        XMBYTE2 src;
        src.x = src.y = -128;
        XMVECTORF32 chk = { -128.f, -128.f, _Q_NAN, _Q_NAN };
        XMVECTOR v = XMLoadByte2(&src);
        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON) {
            printe("%s: 80,80 -> %f %f ... %f %f (%d)\n",
                TestName, XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test597(LogProxy* pLog)
{
// XMLoadUByteN2
    static_assert( sizeof(XMUBYTEN2) == 2, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUBYTEN2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUBYTEN2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUBYTEN2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUBYTEN2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    HRESULT ret = S_OK;

    static const uint8_t data[4][2] = { {0,0}, {127,127}, {255,255}, {64,64} };
    static const XMVECTORF32 check[] = { {0,0,0,0},{0.4980392156f,0.4980392156f,0,0},{1.f,1.f,0,0},{0.2509803921f,0.2509803921f,0,0}};

    for(int k = 0; k < 4; ++k)
    {
        XMUBYTEN2 src;
        src.x=data[k][0]; src.y=data[k][1]; 
        XMVECTOR v = XMLoadUByteN2(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],2);
        if(cc > WITHIN10EPSILON)
        {
            printe("%s: %d,%d -> %f %f ... %f %f (%d)\n",
                TestName, data[k][0], data[k][1], XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; ++k)
    {
        uint8_t a = (uint8_t)(rand() & 0xff);
        uint8_t b = (uint8_t)(rand() & 0xff);

        XMVECTORF32 chk= {(float)(a/255.f), (float)(b/255.f), _Q_NAN, _Q_NAN};

        XMUBYTEN2 src;
        src.x=a; src.y=b; 
        XMVECTOR v = XMLoadUByteN2(&src);

        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON)
        {
            printe("%s: %02X,%02X -> %f %f ... %f %f (%d)\n",
                TestName, a, b, XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test598(LogProxy* pLog)
{
// XMLoadUByte2
    static_assert( sizeof(XMUBYTE2) == 2, "Unexpected structure size" );

    static_assert(std::is_nothrow_copy_assignable<XMUBYTE2>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<XMUBYTE2>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<XMUBYTE2>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<XMUBYTE2>::value, "Move Assign.");

    // TODO - Check for zeroing of partial loads

    HRESULT ret = S_OK;

    static const uint8_t data[4][2] = { {0,0}, {127,127}, {255,255}, {64,64} };
    static const XMVECTORF32 check[] = { {0,0,0,0},{127.f,127.f,0,0},{255.f,255.f,0,0},{64.f,64.f,0,0}};

    for(int k = 0; k < 4; ++k)
    {
        XMUBYTE2 src;
        src.x=data[k][0]; src.y=data[k][1]; 
        XMVECTOR v = XMLoadUByte2(&src);
        COMPARISON cc = CompareXMVECTOR(v,check[k],2);
        if(cc > WITHIN10EPSILON)
        {
            printe("%s: %d,%d -> %f %f ... %f %f (%d)\n",
                TestName, data[k][0], data[k][1], XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(check[k]),XMVectorGetY(check[k]),cc);
            ret = MATH_FAIL;
        }
    }

    for(int k = 0; k < 15; ++k)
    {
        uint8_t a = (uint8_t)(rand() & 0xff);
        uint8_t b = (uint8_t)(rand() & 0xff);

        XMVECTORF32 chk= {(float)a, (float)b, _Q_NAN, _Q_NAN};

        XMUBYTE2 src;
        src.x=a; src.y=b; 
        XMVECTOR v = XMLoadUByte2(&src);

        COMPARISON cc = CompareXMVECTOR(v,chk,2);
        if(cc > WITHIN10EPSILON)
        {
            printe("%s: %02X,%02X -> %f %f ... %f %f (%d)\n",
                TestName, a, b, XMVectorGetX(v),XMVectorGetY(v),
                XMVectorGetX(chk),XMVectorGetY(chk),cc);
            ret = MATH_FAIL;
        }
    }

    return ret;
}

HRESULT Test599(LogProxy* pLog)
{
// XMStoreByteN2
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    int datasize = 1;

    int offset = 0;
    int datacount = 2;
    char value;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const char check[4][2] = { {0,0}, {127,127}, {-127,-127}, {64,-64} };
    static const XMVECTORF32 v[] = { {0,0,0,0},{1.f,1.f,0,0},{-1.f,-1.f,0,0},{0.5039370078f,-0.5039370078f,0,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4)
    {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+datasize*datacount;

        XMStoreByteN2((XMBYTEN2*)&c[offset + j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < datacount; i++)
        {
            value = *((const char*)&(c[first+(i * datasize)]));
            if(value != check[n][i])
            {
                printe("%s: %p corrupted char %p: %d ... %d\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),value,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test600(LogProxy* pLog)
{
// XMStoreByte2
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    int datasize = 1;

    int offset = 0;
    int datacount = 2;
    char value;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const char check[5][2] = { {0,0}, {127,127}, {-127,-127}, {64,-64}, {-127,0} };
    static const XMVECTORF32 v[] = { {0,0,0,0},{127.f,127.f,0,0},{-127.f,-127.f,0,0},{64.f,-64.f,0,0},{-128.f,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4)
    {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+datasize*datacount;

        XMStoreByte2((XMBYTE2*)&c[offset + j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < datacount; i++)
        {
            value = *((const char*)&(c[first+(i * datasize)]));
            if(value != check[n][i])
            {
                printe("%s: %p corrupted char %p: %d ... %d\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),value,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test601(LogProxy* pLog)
{
// XMStoreUByteN2
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    int datasize = 1;

    int offset = 0;
    int datacount = 2;
    uint8_t value;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint8_t check[4][2] = { {0,0}, {127,127}, {255,255}, {64,64} };
    static const XMVECTORF32 v[] =  { {0,0,0,0},{0.4980392156f,0.4980392156f,0,0},{1.f,1.f,0,0},{0.2509803921f,0.2509803921f,0,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4)
    {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+datasize*datacount;

        XMStoreUByteN2((XMUBYTEN2*)&c[offset + j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < datacount; i++)
        {
            value = *((const uint8_t*)&(c[first+(i * datasize)]));
            if(value != check[n][i])
            {
                printe("%s: %p corrupted uchar %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),value,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}

HRESULT Test602(LogProxy* pLog)
{
// XMStoreUByte2
    int csize = 256+65536;
    char *c = (char*)malloc (csize);
    if(!c) { printe ("malloc failed!\n"); 	return MATH_FAIL; }
    intptr_t pc64k = (intptr_t)(&c); 
    pc64k = 65536 - (pc64k & 65535);
    int datasize = 1;

    int offset = 0;
    int datacount = 2;
    uint8_t value;
    intptr_t i, j; 
    HRESULT r = S_OK;

    static const uint8_t check[4][2] = { {0,0}, {127,127}, {255,255}, {64,64} };
    static const XMVECTORF32 v[] =  { {0,0,0,0},{127.f,127.f,0,0},{255.f,255.f,0,0},{64.f,64.f,0,0}};

    int n = 0;
    for(j = pc64k - 16; j <= pc64k + 16; j+=4)
    {
        n = (n + 1) % (countof(check));
        for(i = 0; i < csize; i++)
        {
            c[i] = (char)(~i & 0xff);
        }

        intptr_t first = offset+j;
        intptr_t last  = offset+j+datasize*datacount;

        XMStoreUByte2((XMUBYTE2*)&c[offset + j], v[n]);
        for(i = 0; i < first; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = last; i < csize; i++)
        {
            if(c[i] != (char)(~i & 0xff))
            {
                printe("%s: %p corrupted byte %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),c[i],(unsigned char)(~i));
                r = MATH_FAIL;
            }
        }
        for(i = 0; i < datacount; i++)
        {
            value = *((const uint8_t*)&(c[first+(i * datasize)]));
            if(value != check[n][i])
            {
                printe("%s: %p corrupted uchar %p: %x ... %x\n", TestName, reinterpret_cast<void*>(j), reinterpret_cast<void*>(i),value,check[n][i]);
                r=MATH_FAIL;
            }
        }
    }
    free(c);
    return r;
}
