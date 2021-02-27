//-------------------------------------------------------------------------------------
// SSE3 extensions tester for SIMD C++ Math library
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include "DirectXMathSSE3.h"

#include <tmmintrin.h>

using namespace DirectX;

void printvector(const wchar_t* str, FXMVECTOR V)
{
    XMFLOAT4 vec;
    XMStoreFloat4(&vec, V);
    printf("%ls %f %f %f %f\n", str, vec.x, vec.y, vec.z, vec.w);
}

static const XMVECTORF32 V1 = { { { 0.1f, 0.2f, 0.3f, 0.4f } } };
static const XMVECTORF32 V2 = { { { 1.1f, 1.2f, 1.3f, 1.4f } } };
static const XMVECTORF32 V3 = { { { 2.1f, 2.2f, 2.3f, 2.4f } } };

int main()
{
    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");

        XMVECTOR V = XMVector2Dot( V1, V2 );
        printvector( L"Dot2", V );

        V = XMVector3Dot( V2, V3 );
        printvector( L"Dot3",V );

        V = XMVector4Dot( V1, V3 );
        printvector( L"Dot4",V );

        V = XMVectorSwizzle<0,0,2,2>( V1 );
        printvector( L"0022", V );

        V = XMVectorSwizzle<1,1,3,3>( V1 );
        printvector( L"1133", V );
    }

    if ( SSE3::XMVerifySSE3Support() )
    {
        printf("SSE3 supported\n");

        XMVECTOR V = SSE3::XMVector2Dot( V1, V2 );
        printvector( L"Dot2",V );

        V = SSE3::XMVector3Dot( V2, V3 );
        printvector( L"Dot3", V );

        V = SSE3::XMVector4Dot( V1, V3 );
        printvector( L"Dot4", V );

        V = SSE3::XMVectorSwizzle_0022( V1 );
        printvector( L"0022", V );

        V = SSE3::XMVectorSwizzle_1133( V1 );
        printvector( L"1133", V );
    }

    return 0;
}
