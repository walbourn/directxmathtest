//-------------------------------------------------------------------------------------
// FMA3 extensions tester for SIMD C++ Math library
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include "DirectXMathFMA3.h"

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
static const XMMATRIX c_M1( 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f );
static const XMMATRIX c_M2( 1.1f, 2.1f, 3.1f, 4.1f, 5.1f, 6.1f, 7.1f, 8.1f, 9.1f, 10.1f, 11.1f, 12.1f, 13.1f, 14.1f, 15.1f, 161.f );

int main()
{

    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");

        XMVECTOR V = XMVectorMultiplyAdd( c_V1, c_V2, c_V3 );
        printvector( L"V1*V2+V3", V );

        V = XMVectorNegativeMultiplySubtract( c_V1, c_V2, c_V3 );
        printvector( L"V3-V1*V2", V );

        XMMATRIX M = XMMatrixMultiply( c_M1, c_M2 );
        printvector( L"\nM1*M2[0]", M.r[0] );
        printvector( L"M1*M2[1]", M.r[1] );
        printvector( L"M1*M2[2]", M.r[2] );
        printvector( L"M1*M2[3]", M.r[3] );

        M = XMMatrixMultiplyTranspose( c_M1, c_M2 );
        printvector( L"\nT(M1*M2[0])", M.r[0] );
        printvector( L"T(M1*M2[1])", M.r[1] );
        printvector( L"T(M1*M2[2])", M.r[2] );
        printvector( L"T(M1*M2[3])", M.r[3] );

        XMVECTOR VT2 = XMVector2Transform( c_V1, M );
        printvector( L"\nVT2\n", VT2 );
        VT2 = XMVector2TransformCoord( c_V1, M );
        printvector( L"coord", VT2 );
        VT2 = XMVector2TransformNormal( c_V1, M );
        printvector( L"norm", VT2 );

        XMVECTOR VT3 = XMVector3Transform( c_V2, M );
        printvector( L"\nVT3\n", VT3 );
        VT3 = XMVector3TransformCoord( c_V2, M );
        printvector( L"coord", VT3 );
        VT3 = XMVector3TransformNormal( c_V2, M );
        printvector( L"norm", VT3 );

        XMVECTOR VT4 = XMVector3Transform( c_V3, M );
        printvector( L"\nVT4\n", VT4 );
    }

    if ( FMA3::XMVerifyFMA3Support() )
    {
        printf("\nFMA3 supported\n");

        XMVECTOR V = FMA3::XMVectorMultiplyAdd( c_V1, c_V2, c_V3 );
        printvector( L"V1*V2+V3", V );

        V = FMA3::XMVectorNegativeMultiplySubtract( c_V1, c_V2, c_V3 );
        printvector( L"V3-V1*V2", V );

        XMMATRIX M = FMA3::XMMatrixMultiply( c_M1, c_M2 );
        printvector( L"\nM1*M2[0]", M.r[0] );
        printvector( L"M1*M2[1]", M.r[1] );
        printvector( L"M1*M2[2]", M.r[2] );
        printvector( L"M1*M2[3]", M.r[3] );

        M = FMA3::XMMatrixMultiplyTranspose( c_M1, c_M2 );
        printvector( L"\nT(M1*M2[0])", M.r[0] );
        printvector( L"T(M1*M2[1])", M.r[1] );
        printvector( L"T(M1*M2[2])", M.r[2] );
        printvector( L"T(M1*M2[3])", M.r[3] );

        XMVECTOR VT2 = FMA3::XMVector2Transform( c_V1, M );
        printvector( L"\nVT2\n", VT2 );
        VT2 = FMA3::XMVector2TransformCoord( c_V1, M );
        printvector( L"coord", VT2 );
        VT2 = FMA3::XMVector2TransformNormal( c_V1, M );
        printvector( L"norm", VT2 );

        XMVECTOR VT3 = FMA3::XMVector3Transform( c_V2, M );
        printvector( L"\nVT3\n", VT3 );
        VT3 = FMA3::XMVector3TransformCoord( c_V2, M );
        printvector( L"coord", VT3 );
        VT3 = FMA3::XMVector3TransformNormal( c_V2, M );
        printvector( L"norm", VT3 );

        XMVECTOR VT4 = FMA3::XMVector3Transform( c_V3, M );
        printvector( L"\nVT4\n", VT4 );
    }

    return 0;
}
