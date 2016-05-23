#include <windows.h>
#include <stdio.h>

#include "DirectXMathFMA4.h"

using namespace DirectX;

void printvector( const WCHAR *str, FXMVECTOR V )
{
    printf("%S %f %f %f %f\n", str, V.m128_f32[0], V.m128_f32[1], V.m128_f32[2], V.m128_f32[3] );
}

static const XMVECTORF32 V1 = { 0.1f, 0.2f, 0.3f, 0.4f };
static const XMVECTORF32 V2 = { 1.1f, 1.2f, 1.3f, 1.4f };
static const XMVECTORF32 V3 = { 2.1f, 2.2f, 2.3f, 2.4f };
static const XMMATRIX M1( 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f );
static const XMMATRIX M2( 1.1f, 2.1f, 3.1f, 4.1f, 5.1f, 6.1f, 7.1f, 8.1f, 9.1f, 10.1f, 11.1f, 12.1f, 13.1f, 14.1f, 15.1f, 161.f );

void main()
{

    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");

        XMVECTOR V = XMVectorMultiplyAdd( V1, V2, V3 );
        printvector( L"V1*V2+V3", V );

        V = XMVectorNegativeMultiplySubtract( V1, V2, V3 );
        printvector( L"V3-V1*V2", V );

        XMMATRIX M = XMMatrixMultiply( M1, M2 );
        printvector( L"\nM1*M2[0]", M.r[0] );
        printvector( L"M1*M2[1]", M.r[1] );
        printvector( L"M1*M2[2]", M.r[2] );
        printvector( L"M1*M2[3]", M.r[3] );

        M = XMMatrixMultiplyTranspose( M1, M2 );
        printvector( L"\nT(M1*M2[0])", M.r[0] );
        printvector( L"T(M1*M2[1])", M.r[1] );
        printvector( L"T(M1*M2[2])", M.r[2] );
        printvector( L"T(M1*M2[3])", M.r[3] );

        XMVECTOR VT2 = XMVector2Transform( V1, M );
        printvector( L"\nVT2\n", VT2 );
        VT2 = XMVector2TransformCoord( V1, M );
        printvector( L"coord", VT2 );
        VT2 = XMVector2TransformNormal( V1, M );
        printvector( L"norm", VT2 );

        XMVECTOR VT3 = XMVector3Transform( V2, M );
        printvector( L"\nVT3\n", VT3 );
        VT3 = XMVector3TransformCoord( V2, M );
        printvector( L"coord", VT3 );
        VT3 = XMVector3TransformNormal( V2, M );
        printvector( L"norm", VT3 );

        XMVECTOR VT4 = XMVector3Transform( V3, M );
        printvector( L"\nVT4\n", VT4 );
    }

    if ( FMA4::XMVerifyFMA4Support() )
    {
        printf("\nFMA4 supported\n");

        XMVECTOR V = FMA4::XMVectorMultiplyAdd( V1, V2, V3 );
        printvector( L"V1*V2+V3", V );

        V = FMA4::XMVectorNegativeMultiplySubtract( V1, V2, V3 );
        printvector( L"V3-V1*V2", V );

        XMMATRIX M = FMA4::XMMatrixMultiply( M1, M2 );
        printvector( L"\nM1*M2[0]", M.r[0] );
        printvector( L"M1*M2[1]", M.r[1] );
        printvector( L"M1*M2[2]", M.r[2] );
        printvector( L"M1*M2[3]", M.r[3] );

        M = FMA4::XMMatrixMultiplyTranspose( M1, M2 );
        printvector( L"\nT(M1*M2[0])", M.r[0] );
        printvector( L"T(M1*M2[1])", M.r[1] );
        printvector( L"T(M1*M2[2])", M.r[2] );
        printvector( L"T(M1*M2[3])", M.r[3] );

        XMVECTOR VT2 = FMA4::XMVector2Transform( V1, M );
        printvector( L"\nVT2\n", VT2 );
        VT2 = FMA4::XMVector2TransformCoord( V1, M );
        printvector( L"coord", VT2 );
        VT2 = FMA4::XMVector2TransformNormal( V1, M );
        printvector( L"norm", VT2 );

        XMVECTOR VT3 = FMA4::XMVector3Transform( V2, M );
        printvector( L"\nVT3\n", VT3 );
        VT3 = FMA4::XMVector3TransformCoord( V2, M );
        printvector( L"coord", VT3 );
        VT3 = FMA4::XMVector3TransformNormal( V2, M );
        printvector( L"norm", VT3 );

        XMVECTOR VT4 = FMA4::XMVector3Transform( V3, M );
        printvector( L"\nVT4\n", VT4 );
    }
}