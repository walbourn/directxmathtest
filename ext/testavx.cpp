#include <windows.h>
#include <stdio.h>

#include "DirectXMathAVX.h"

using namespace DirectX;

void printvector( const WCHAR *str, FXMVECTOR V )
{
    printf("%S %f %f %f %f\n", str, V.m128_f32[0], V.m128_f32[1], V.m128_f32[2], V.m128_f32[3] );
}

static const XMVECTORF32 A = { 1.f, 2.f, 3.f, 4.f };

void main()
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
}