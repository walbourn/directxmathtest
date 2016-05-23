#include <windows.h>
#include <stdio.h>

#include "DirectXMathSSE3.h"

#include <tmmintrin.h>

using namespace DirectX;

void printvector( const WCHAR *str, FXMVECTOR V )
{
    printf("%S %f %f %f %f\n", str, V.m128_f32[0], V.m128_f32[1], V.m128_f32[2], V.m128_f32[3] );
}

static const XMVECTORF32 V1 = { 0.1f, 0.2f, 0.3f, 0.4f };
static const XMVECTORF32 V2 = { 1.1f, 1.2f, 1.3f, 1.4f };
static const XMVECTORF32 V3 = { 2.1f, 2.2f, 2.3f, 2.4f };

void main()
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
}