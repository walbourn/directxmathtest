#include <windows.h>
#include <stdio.h>

#include "DirectXMathBE.h"

using namespace DirectX;

void printvector( const WCHAR *str, FXMVECTOR V )
{
#ifdef _M_ARM
    printf("%S %f %f %f %f\n\t(%08X %08X %08X %08X)\n", str, V.n128_f32[0], V.n128_f32[1], V.n128_f32[2], V.n128_f32[3],
                                                          V.n128_u32[0], V.n128_u32[1], V.n128_u32[2], V.n128_u32[3] );
#else
    printf("%S %f %f %f %f\n\t(%08X %08X %08X %08X)\n", str, V.m128_f32[0], V.m128_f32[1], V.m128_f32[2], V.m128_f32[3],
                                                          V.m128_u32[0], V.m128_u32[1], V.m128_u32[2], V.m128_u32[3] );
#endif
}

static const XMVECTORF32 V1 = { 0.1f, 0.2f, 0.3f, 0.4f };
static const XMVECTORF32 V2 = { 1.1f, 1.2f, 1.3f, 1.4f };
static const XMVECTORF32 V3 = { 2.1f, 2.2f, 2.3f, 2.4f };

void main()
{
    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");

        XMVECTOR BE = XMVectorEndian( V1 );
        printvector( L"V1", V1 );
        printvector( L"BE", BE );
 
        XMVECTOR LE = XMVectorEndian( BE );
        printvector( L"LE", LE );
    }

#ifndef _M_ARM
    if ( SSSE3::XMVerifySSSE3Support() )
    {
        printf("SSSE3 supported\n");

        XMVECTOR BE = SSSE3::XMVectorEndian( V1 );
        printvector( L"V1", V1 );
        printvector( L"BE", BE );
 
        XMVECTOR LE = SSSE3::XMVectorEndian( BE );
        printvector( L"LE", LE );
    }
#endif
}