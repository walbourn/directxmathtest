//-------------------------------------------------------------------------------------
// constexpr.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This test file covers constexpr initalization

// Off by default warnings
#pragma warning(disable : 4514 4668 4710 4820)
// C4514 'function' : unreferenced inline function has been removed 
// C4668 not defined as a preprocessor macro
// C4710 function not inlined
// C4820 padding added after data member

#pragma warning(disable: 26812)
// 26812: Prefer 'enum class' over 'enum' (Enum.3).

#ifdef __clang__
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wc++98-compat-local-type-template-args"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wfloat-equal"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wunused-const-variable"
#endif

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace Test
{
    constexpr float valueRadins = XMConvertToRadians(1.f);
    constexpr float valueDegrees = XMConvertToDegrees(XM_PI);

    constexpr static XMFLOAT2 g_array2[] = { XMFLOAT2(1.f, 2.f), XMFLOAT2(3.f, 4.f) };
    constexpr static XMFLOAT2A g_array2a[] = { XMFLOAT2A(1.f, 2.f), XMFLOAT2A(3.f, 4.f) };

    constexpr static XMFLOAT3 g_array3[] = { XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(4.f, 4.f, 5.f) };
    constexpr static XMFLOAT3A g_array3a[] = { XMFLOAT3A(1.f, 2.f, 3.f), XMFLOAT3A(4.f, 4.f, 5.f) };

    constexpr static XMFLOAT4 g_array4[] = { XMFLOAT4(1.f, 2.f, 3.f, 4.f), XMFLOAT4(5.f, 6.f, 7.f, 8.f) };
    constexpr static XMFLOAT4A g_array4a[] = { XMFLOAT4A(1.f, 2.f, 3.f, 4.f), XMFLOAT4A(5.f, 6.f, 7.f, 8.f) };

    constexpr static XMINT2 g_array2i[] = { XMINT2(1, 2), XMINT2(3, 4) };
    constexpr static XMUINT2 g_array2u[] = { XMUINT2(1, 2), XMUINT2(3, 4) };

    constexpr static XMINT3 g_array3i[] = { XMINT3(1, 2, 3), XMINT3(4, 5, 6) };
    constexpr static XMUINT3 g_array3u[] = { XMUINT3(1, 2, 3), XMUINT3(4, 5, 6) };

    constexpr static XMINT4 g_array4i[] = { XMINT4(1, 2, 3, 4), XMINT4(5, 6, 7, 8) };
    constexpr static XMUINT4 g_array4u[] = { XMUINT4(1, 2, 3, 4), XMUINT4(5, 6, 7, 8) };

    constexpr static XMFLOAT3X3 g_array3x3[] = { XMFLOAT3X3(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f), XMFLOAT3X3(10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f) };

    constexpr static XMFLOAT4X3 g_array4x3[] = { XMFLOAT4X3(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f), XMFLOAT4X3(13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f, 21.f, 22.f, 23.f, 24.f) };
    constexpr static XMFLOAT4X3A g_array4x3a[] = { XMFLOAT4X3A(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f), XMFLOAT4X3A(13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f, 21.f, 22.f, 23.f, 24.f) };

    constexpr static XMFLOAT3X4 g_array3x4[] = { XMFLOAT3X4(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f), XMFLOAT3X4(13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f, 21.f, 22.f, 23.f, 24.f) };
    constexpr static XMFLOAT3X4A g_array3x4a[] = { XMFLOAT3X4A(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f), XMFLOAT3X4A(13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f, 21.f, 22.f, 23.f, 24.f) };

    constexpr static XMFLOAT4X4 g_array4x4[] = { XMFLOAT4X4(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f), XMFLOAT4X4(17.f, 18.f, 19.f, 20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f, 30.f, 31.f, 32.f) };
    constexpr static XMFLOAT4X4A g_array4x4a[] = { XMFLOAT4X4A(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f), XMFLOAT4X4A(17.f, 18.f, 19.f, 20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f, 30.f, 31.f, 32.f) };

    // PackedVector
    constexpr static XMCOLOR g_color[] = { 0xff0c0d00, 0x0f0c0b1a };

    constexpr static XMHALF2 g_half2[] = { XMHALF2(HALF(0xabce), HALF(0xabcd)), XMHALF2(HALF(0xabce), HALF(0xabcd)) };
    constexpr static XMHALF2 g_half2p[] = { XMHALF2(0xabceabcd) };

    constexpr static XMHALF4 g_half4[] = { XMHALF4(HALF(0xabce), HALF(0xabcd), HALF(0xabcd), HALF(0xabcd)), XMHALF4(HALF(0xabce), HALF(0xabcd), HALF(0xabcd), HALF(0xabcd)) };
    constexpr static XMHALF4 g_half4p[] = { XMHALF4(0x12345678abceabcd) };

    constexpr static XMSHORTN2 g_shortn2[] = { XMSHORTN2(int16_t(0x7bce), int16_t(0x7bcd)), XMSHORTN2(int16_t(0x7bce), int16_t(0x7bcd)) };
    constexpr static XMSHORTN2 g_shortn2p[] = { XMSHORTN2(0xabceabcd) };
    constexpr static XMUSHORTN2 g_shortun2[] = { XMUSHORTN2(uint16_t(0x7bce), uint16_t(0x7bcd)), XMUSHORTN2(uint16_t(0x7bce), uint16_t(0x7bcd)) };
    constexpr static XMUSHORTN2 g_shortun2p[] = { XMUSHORTN2(0xabceabcd) };

    constexpr static XMSHORT2 g_short2[] = { XMSHORT2(int16_t(0x7bce), int16_t(0x7bcd)), XMSHORT2(int16_t(0x7bce), int16_t(0x7bcd)) };
    constexpr static XMSHORT2 g_short2p[] = { XMSHORT2(0xabceabcd) };
    constexpr static XMUSHORT2 g_shortu2[] = { XMUSHORT2(uint16_t(0x7bce), uint16_t(0x7bcd)), XMUSHORT2(uint16_t(0x7bce), uint16_t(0x7bcd)) };
    constexpr static XMUSHORT2 g_shortu2p[] = { XMUSHORT2(0xabceabcd) };

    constexpr static XMSHORTN4 g_shortn4[] = { XMSHORTN4(int16_t(0x7bce), int16_t(0x7bcd), int16_t(0x7bce), int16_t(0x7bcd)), XMSHORTN4(int16_t(0x7bce), int16_t(0x7bcd), int16_t(0x7bce), int16_t(0x7bcd)) };
    constexpr static XMSHORTN4 g_shortn4p[] = { XMSHORTN4(0x12345678abceabcd) };
    constexpr static XMUSHORTN4 g_shortun4[] = { XMUSHORTN4(uint16_t(0x7bce), uint16_t(0x7bcd), uint16_t(0x7bce), uint16_t(0x7bcd)), XMUSHORTN4(uint16_t(0x7bce), uint16_t(0x7bcd), uint16_t(0x7bce), uint16_t(0x7bcd)) };
    constexpr static XMUSHORTN4 g_shortun4p[] = { XMUSHORTN4(0x12345678abceabcd) };

    constexpr static XMSHORT4 g_short[] = { XMSHORT4(int16_t(0x7bce), int16_t(0x7bcd), int16_t(0x7bce), int16_t(0x7bcd)), XMSHORT4(int16_t(0x7bce), int16_t(0x7bcd), int16_t(0x7bce), int16_t(0x7bcd)) };
    constexpr static XMSHORT4 g_short4p[] = { XMSHORT4(0x12345678abceabcd) };
    constexpr static XMUSHORT4 g_shortu4[] = { XMUSHORT4(uint16_t(0x7bce), uint16_t(0x7bcd), uint16_t(0x7bce), uint16_t(0x7bcd)), XMUSHORT4(uint16_t(0x7bce), uint16_t(0x7bcd), uint16_t(0x7bce), uint16_t(0x7bcd)) };
    constexpr static XMUSHORT4 g_shortu4p[] = { XMUSHORT4(0x12345678abceabcd) };

    constexpr static XMBYTEN2 g_byten2[] = { XMBYTEN2(int8_t(0x7b), int8_t(0x7b)), XMBYTEN2(int8_t(0x7b), int8_t(0x7b)) };
    constexpr static XMBYTEN2 g_byten2p[] = { XMBYTEN2(0xabcd) };
    constexpr static XMUBYTEN2 g_byteun2[] = { XMUBYTEN2(uint8_t(0x7c), uint8_t(0x7b)), XMUBYTEN2(uint8_t(0x7b), uint8_t(0x7b)) };
    constexpr static XMUBYTEN2 g_byteun2p[] = { XMUBYTEN2(0xabce) };

    constexpr static XMBYTE2 g_byte2[] = { XMBYTE2(int8_t(0x7b), int8_t(0x7b)), XMBYTE2(int8_t(0x7b), int8_t(0x7d)) };
    constexpr static XMBYTE2 g_byte2p[] = { XMBYTE2(0xabce) };
    constexpr static XMUBYTE2 g_byteu2[] = { XMUBYTE2(uint8_t(0x7b), uint8_t(0x7d)), XMUBYTE2(uint8_t(0x7b), uint8_t(0x7d)) };
    constexpr static XMUBYTE2 g_byteu2p[] = { XMUBYTE2(0xabcd) };

    constexpr static XMBYTEN4 g_byten4[] = { XMBYTEN4(int8_t(0x7b), int8_t(0x7b), int8_t(0x7b), int8_t(0x7b)), XMBYTEN4(int8_t(0x7b), int8_t(0x7b), int8_t(0x7b), int8_t(0x7b)) };
    constexpr static XMBYTEN4 g_byten4p[] = { XMBYTEN4(0xabceabcd) };
    constexpr static XMUBYTEN4 g_byteun4[] = { XMUBYTEN4(uint8_t(0x7b), uint8_t(0x7b), uint8_t(0x7b), uint8_t(0x7b)), XMUBYTEN4(uint8_t(0x7b), uint8_t(0x7b), uint8_t(0x7b), uint8_t(0x7b)) };
    constexpr static XMUBYTEN4 g_byteun4p[] = { XMUBYTEN4(0xabceabcd) };

    constexpr static XMBYTE4 g_byte[] = { XMBYTE4(int8_t(0x7b), int8_t(0x7b), int8_t(0x7b), int8_t(0x7b)), XMBYTE4(int8_t(0x7b), int8_t(0x7b), int8_t(0x7b), int8_t(0x7b)) };
    constexpr static XMBYTE4 g_byte4p[] = { XMBYTE4(0xabceabcd) };
    constexpr static XMUBYTE4 g_byteu4[] = { XMUBYTE4(uint8_t(0x7b), uint8_t(0x7b), uint8_t(0x7b), uint8_t(0x7b)), XMUBYTE4(uint8_t(0x7b), uint8_t(0x7b), uint8_t(0x7b), uint8_t(0x7b)) };
    constexpr static XMUBYTE4 g_byteu4p[] = { XMUBYTE4(0xabceabcd) };

    constexpr static XMU565 g_u656[] = { XMU565(uint8_t(12), uint8_t(6), uint8_t(15)) };
    constexpr static XMU565 g_u656p[] = { XMU565(0xabce) };
    constexpr static XMU555 g_u555[] = { XMU555(uint8_t(23), uint8_t(2), uint8_t(30), true) };
    constexpr static XMU555 g_u555p[] = { XMU555(0xabce) };
    constexpr static XMUNIBBLE4 g_u4444[] = { XMUNIBBLE4(uint8_t(3), uint8_t(11), uint8_t(14), uint8_t(5)) };
    constexpr static XMUNIBBLE4 g_u4444p[] = { XMUNIBBLE4(0xabce) };

    constexpr static XMFLOAT3PK g_3pk[] = { XMFLOAT3PK(0xabceabcd) };
    constexpr static XMFLOAT3SE g_3se[] = { XMFLOAT3SE(0xabceabcd) };

    constexpr static XMXDECN4 g_xn4[] = { XMXDECN4(0xabceabcd) };
    constexpr static XMUDECN4 g_un4[] = { XMUDECN4(0xabceabcd) };
    constexpr static XMUDEC4 g_u4[] = { XMUDEC4(0xabceabcd) };

#pragma warning(push)
#pragma warning(disable : 4996)

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

    constexpr static XMXDEC4 g_x4[] = { XMXDEC4(0xabceabcd) };
    constexpr static XMDECN4 g_n4[] = { XMDECN4(0xabceabcd) };
    constexpr static XMDEC4 g_4[] = { XMDEC4(0xabceabcd) };

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#pragma warning(pop)

    // Collision
    constexpr static BoundingSphere g_sphere[] = { BoundingSphere(XMFLOAT3(1.f, 2.f, 3.f), 4.f), BoundingSphere(XMFLOAT3(5.f, 6.f, 7.f), 8.f) };
    constexpr static BoundingBox g_box[] = { BoundingBox(XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(4.f, 4.f, 5.f)), BoundingBox(XMFLOAT3(6.f, 7.f, 8.f), XMFLOAT3(9.f, 10.f, 11.f)) };
    constexpr static BoundingOrientedBox g_obox[] = { BoundingOrientedBox(XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(4.f, 4.f, 5.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f)), BoundingOrientedBox(XMFLOAT3(6.f, 7.f, 8.f), XMFLOAT3(9.f, 10.f, 11.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f)) };
    constexpr static BoundingFrustum g_frustum[] = { BoundingFrustum(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f), BoundingFrustum(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 1.f, -1.f, 1.f, -1.f, 0.1f, 1.f) };
}
