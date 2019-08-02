//-------------------------------------------------------------------------------------
// AVX (version 1) extensions tester for SIMD C++ Math library
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include <Windows.h>
#include <stdio.h>

#include "DirectXMathAVX.h"

using namespace DirectX;

void printvector( const wchar_t *str, FXMVECTOR V )
{
    XMFLOAT4 vec;
    XMStoreFloat4(&vec, V);
    printf("%ls %f %f %f %f\n", str, vec.x, vec.y, vec.z, vec.w );
}

static const XMVECTORF32 A = { { { 1.f, 2.f, 3.f, 4.f } } };

int main()
{
    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");

        XMVECTOR V = XMVectorSplatX( A );
        printvector( L"A.xxxx", V );

        V = XMVectorSplatY( A );
        printvector( L"A.yyyy", V );

        V = XMVectorSplatZ( A );
        printvector( L"A.zzzz", V );

        V = XMVectorSplatW( A );
        printvector( L"A.wwww", V );
    }

    if ( AVX::XMVerifyAVXSupport() )
    {
        printf("AVX supported\n");

        XMVECTOR V = AVX::XMVectorSplatX( A );
        printvector( L"A.xxxx", V );

        V = AVX::XMVectorSplatY( A );
        printvector( L"A.yyyy", V );

        V = AVX::XMVectorSplatZ( A );
        printvector( L"A.zzzz", V );

        V = AVX::XMVectorSplatW( A );
        printvector( L"A.wwww", V );
    }

    return 0;
}
