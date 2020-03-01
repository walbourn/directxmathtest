// ------------------------------------------------------------------------------------ -
// Big-endian swap extensions tester for SIMD C++ Math library
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include "DirectXMathBE.h"

using namespace DirectX;

void printvector(const wchar_t* str, FXMVECTOR V)
{
    XMFLOAT4 vec;
    XMStoreFloat4(&vec, V);

    auto ivec = reinterpret_cast<const XMUINT4*>(&vec);

    printf("%ls %f %f %f %f\n\t(%08X %08X %08X %08X)\n", str, vec.x, vec.y, vec.z, vec.w, ivec->x, ivec->y, ivec->z, ivec->w);
}

static const XMVECTORF32 V1 = { { { 0.1f, 0.2f, 0.3f, 0.4f } } };

int main()
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

#if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64)
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

    return 0;
}
