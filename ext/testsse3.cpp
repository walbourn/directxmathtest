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

static const XMVECTORF32 c_V1 = { { { 0.1f, 0.2f, 0.3f, 0.4f } } };
static const XMVECTORF32 c_V2 = { { { 1.1f, 1.2f, 1.3f, 1.4f } } };
static const XMVECTORF32 c_V3 = { { { 2.1f, 2.2f, 2.3f, 2.4f } } };

int main()
{
    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");

        XMVECTOR V = XMVector2Dot( c_V1, c_V2 );
        printvector( L"Dot2", V );

        V = XMVector3Dot( c_V2, c_V3 );
        printvector( L"Dot3",V );

        V = XMVector4Dot( c_V1, c_V3 );
        printvector( L"Dot4",V );

        V = XMVectorSwizzle<0,0,2,2>( c_V1 );
        printvector( L"0022", V );

        V = XMVectorSwizzle<1,1,3,3>( c_V1 );
        printvector( L"1133", V );
    }

    if ( SSE3::XMVerifySSE3Support() )
    {
        printf("SSE3 supported\n");

        XMVECTOR V = SSE3::XMVector2Dot( c_V1, c_V2 );
        printvector( L"Dot2",V );

        V = SSE3::XMVector3Dot( c_V2, c_V3 );
        printvector( L"Dot3", V );

        V = SSE3::XMVector4Dot( c_V1, c_V3 );
        printvector( L"Dot4", V );

        V = SSE3::XMVectorSwizzle_0022( c_V1 );
        printvector( L"0022", V );

        V = SSE3::XMVectorSwizzle_1133( c_V1 );
        printvector( L"1133", V );
    }

    return 0;
}
