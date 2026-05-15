//-------------------------------------------------------------------------------------
// cpp20tests.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// https://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This file has /std:c++20 when building with VS 2022 or later

#if __cplusplus < 202002L
#if defined(_MSVC_LANG) && _MSVC_LANG >= 202002L
#error Add /Zc:__cplusplus to the build settings
#endif
#error This file should be built with C++20 mode enabled
#endif

#include "math3.h"

using namespace DirectX;

//-------------------------------------------------------------------------------------
// TestSpaceShip01 - XMFLOAT2 (std::partial_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip01(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMFLOAT2&>() <=> std::declval<const XMFLOAT2&>()), std::partial_ordering>);

    HRESULT ret = MATH_PASS;
    const XMFLOAT2 a(1.f, 2.f);
    const XMFLOAT2 b(1.f, 2.f);
    const XMFLOAT2 c(2.f, 1.f);

    if (!(a == b)) { printe("%s: XMFLOAT2 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMFLOAT2 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMFLOAT2 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMFLOAT2 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMFLOAT2 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMFLOAT2 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMFLOAT2 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMFLOAT2 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMFLOAT2 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMFLOAT2 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMFLOAT2 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMFLOAT2 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMFLOAT2 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMFLOAT2 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMFLOAT2 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip02- XMINT2 (std::strong_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip02(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMINT2&>() <=> std::declval<const XMINT2&>()), std::strong_ordering>);

    HRESULT ret = MATH_PASS;
    const XMINT2 a(1, 2);
    const XMINT2 b(1, 2);
    const XMINT2 c(2, 1);

    if (!(a == b)) { printe("%s: XMINT2 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMINT2 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMINT2 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMINT2 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMINT2 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMINT2 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMINT2 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMINT2 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMINT2 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMINT2 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMINT2 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMINT2 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMINT2 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMINT2 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMINT2 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip03 - XMUINT2 (std::strong_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip03(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMUINT2&>() <=> std::declval<const XMUINT2&>()), std::strong_ordering>);

    HRESULT ret = MATH_PASS;
    const XMUINT2 a(1u, 2u);
    const XMUINT2 b(1u, 2u);
    const XMUINT2 c(2u, 1u);

    if (!(a == b)) { printe("%s: XMUINT2 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMUINT2 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMUINT2 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMUINT2 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMUINT2 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMUINT2 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMUINT2 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMUINT2 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMUINT2 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMUINT2 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMUINT2 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMUINT2 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMUINT2 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMUINT2 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMUINT2 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip04 - XMINT3 (std::strong_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip04(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMINT3&>() <=> std::declval<const XMINT3&>()), std::strong_ordering>);

    HRESULT ret = MATH_PASS;
    const XMINT3 a(1, 2, 3);
    const XMINT3 b(1, 2, 3);
    const XMINT3 c(2, 1, 1);

    if (!(a == b)) { printe("%s: XMINT3 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMINT3 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMINT3 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMINT3 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMINT3 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMINT3 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMINT3 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMINT3 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMINT3 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMINT3 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMINT3 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMINT3 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMINT3 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMINT3 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMINT3 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip05 - XMUINT3 (std::strong_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip05(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMUINT3&>() <=> std::declval<const XMUINT3&>()), std::strong_ordering>);

    HRESULT ret = MATH_PASS;
    const XMUINT3 a(1u, 2u, 3u);
    const XMUINT3 b(1u, 2u, 3u);
    const XMUINT3 c(2u, 1u, 1u);

    if (!(a == b)) { printe("%s: XMUINT3 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMUINT3 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMUINT3 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMUINT3 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMUINT3 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMUINT3 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMUINT3 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMUINT3 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMUINT3 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMUINT3 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMUINT3 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMUINT3 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMUINT3 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMUINT3 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMUINT3 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip06 - XMFLOAT4 (std::partial_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip06(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMFLOAT4&>() <=> std::declval<const XMFLOAT4&>()), std::partial_ordering>);

    HRESULT ret = MATH_PASS;
    const XMFLOAT4 a(1.f, 2.f, 3.f, 4.f);
    const XMFLOAT4 b(1.f, 2.f, 3.f, 4.f);
    const XMFLOAT4 c(2.f, 1.f, 1.f, 1.f);

    if (!(a == b)) { printe("%s: XMFLOAT4 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMFLOAT4 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMFLOAT4 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMFLOAT4 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMFLOAT4 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMFLOAT4 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMFLOAT4 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMFLOAT4 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMFLOAT4 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMFLOAT4 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMFLOAT4 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMFLOAT4 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMFLOAT4 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMFLOAT4 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMFLOAT4 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip07- XMINT4 (std::strong_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip07(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMINT4&>() <=> std::declval<const XMINT4&>()), std::strong_ordering>);

    HRESULT ret = MATH_PASS;
    const XMINT4 a(1, 2, 3, 4);
    const XMINT4 b(1, 2, 3, 4);
    const XMINT4 c(2, 1, 1, 1);

    if (!(a == b)) { printe("%s: XMINT4 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMINT4 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMINT4 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMINT4 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMINT4 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMINT4 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMINT4 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMINT4 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMINT4 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMINT4 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMINT4 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMINT4 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMINT4 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMINT4 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMINT4 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip08 - XMUINT4 (std::strong_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip08(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMUINT4&>() <=> std::declval<const XMUINT4&>()), std::strong_ordering>);

    HRESULT ret = MATH_PASS;
    const XMUINT4 a(1u, 2u, 3u, 4u);
    const XMUINT4 b(1u, 2u, 3u, 4u);
    const XMUINT4 c(2u, 1u, 1u, 1u);

    if (!(a == b)) { printe("%s: XMUINT4 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMUINT4 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMUINT4 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMUINT4 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMUINT4 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMUINT4 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMUINT4 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMUINT4 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMUINT4 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMUINT4 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMUINT4 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMUINT4 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMUINT4 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMUINT4 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMUINT4 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip09 - XMFLOAT3X3 (std::partial_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip09(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMFLOAT3X3&>() <=> std::declval<const XMFLOAT3X3&>()), std::partial_ordering>);

    HRESULT ret = MATH_PASS;
    const XMFLOAT3X3 a(1.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 1.f);
    const XMFLOAT3X3 b(1.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 1.f);
    const XMFLOAT3X3 c(2.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 1.f);
    const XMFLOAT3X3 d(1.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 2.f);  // differs only at _33 (last element)

    if (!(a == b)) { printe("%s: XMFLOAT3X3 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMFLOAT3X3 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMFLOAT3X3 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMFLOAT3X3 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMFLOAT3X3 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMFLOAT3X3 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMFLOAT3X3 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMFLOAT3X3 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMFLOAT3X3 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMFLOAT3X3 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMFLOAT3X3 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMFLOAT3X3 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMFLOAT3X3 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMFLOAT3X3 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMFLOAT3X3 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    // Verify ordering on non-first element (_33 differs)
    if (!(a < d)) { printe("%s: XMFLOAT3X3 < last element failed\n", TestName); ret = MATH_FAIL; }
    if (d < a) { printe("%s: XMFLOAT3X3 < last element reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(d > a)) { printe("%s: XMFLOAT3X3 > last element failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> d)) { printe("%s: XMFLOAT3X3 <=> last element failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip10 - XMFLOAT4X3 (std::partial_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip10(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMFLOAT4X3&>() <=> std::declval<const XMFLOAT4X3&>()), std::partial_ordering>);

    HRESULT ret = MATH_PASS;
    const XMFLOAT4X3 a(1.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 1.f,
                        0.f, 0.f, 0.f);
    const XMFLOAT4X3 b(1.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 1.f,
                        0.f, 0.f, 0.f);
    const XMFLOAT4X3 c(2.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 1.f,
                        0.f, 0.f, 0.f);
    const XMFLOAT4X3 d(1.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 1.f,
                        0.f, 0.f, 2.f);  // differs only at _43 (last element)

    if (!(a == b)) { printe("%s: XMFLOAT4X3 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMFLOAT4X3 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMFLOAT4X3 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMFLOAT4X3 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMFLOAT4X3 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMFLOAT4X3 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMFLOAT4X3 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMFLOAT4X3 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMFLOAT4X3 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMFLOAT4X3 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMFLOAT4X3 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMFLOAT4X3 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMFLOAT4X3 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMFLOAT4X3 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMFLOAT4X3 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    // Verify ordering on non-first element (_43 differs)
    if (!(a < d)) { printe("%s: XMFLOAT4X3 < last element failed\n", TestName); ret = MATH_FAIL; }
    if (d < a) { printe("%s: XMFLOAT4X3 < last element reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(d > a)) { printe("%s: XMFLOAT4X3 > last element failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> d)) { printe("%s: XMFLOAT4X3 <=> last element failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip11 - XMFLOAT3X4 (std::partial_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip11(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMFLOAT3X4&>() <=> std::declval<const XMFLOAT3X4&>()), std::partial_ordering>);

    HRESULT ret = MATH_PASS;
    const XMFLOAT3X4 a(1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f);
    const XMFLOAT3X4 b(1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f);
    const XMFLOAT3X4 c(2.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f);
    const XMFLOAT3X4 d(1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 2.f);  // differs only at _34 (last element)

    if (!(a == b)) { printe("%s: XMFLOAT3X4 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMFLOAT3X4 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMFLOAT3X4 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMFLOAT3X4 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMFLOAT3X4 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMFLOAT3X4 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMFLOAT3X4 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMFLOAT3X4 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMFLOAT3X4 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMFLOAT3X4 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMFLOAT3X4 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMFLOAT3X4 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMFLOAT3X4 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMFLOAT3X4 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMFLOAT3X4 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    // Verify ordering on non-first element (_34 differs)
    if (!(a < d)) { printe("%s: XMFLOAT3X4 < last element failed\n", TestName); ret = MATH_FAIL; }
    if (d < a) { printe("%s: XMFLOAT3X4 < last element reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(d > a)) { printe("%s: XMFLOAT3X4 > last element failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> d)) { printe("%s: XMFLOAT3X4 <=> last element failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip13 - XMFLOAT3 (std::partial_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip13(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMFLOAT3&>() <=> std::declval<const XMFLOAT3&>()), std::partial_ordering>);

    HRESULT ret = MATH_PASS;
    const XMFLOAT3 a(1.f, 2.f, 3.f);
    const XMFLOAT3 b(1.f, 2.f, 3.f);
    const XMFLOAT3 c(2.f, 2.f, 3.f);  // differs at x (first element)
    const XMFLOAT3 d(1.f, 2.f, 4.f);  // differs at z (last element)

    if (!(a == b)) { printe("%s: XMFLOAT3 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMFLOAT3 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMFLOAT3 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMFLOAT3 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMFLOAT3 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMFLOAT3 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMFLOAT3 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMFLOAT3 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMFLOAT3 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMFLOAT3 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMFLOAT3 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMFLOAT3 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMFLOAT3 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMFLOAT3 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMFLOAT3 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    // Verify ordering on non-first element (z differs)
    if (!(a < d)) { printe("%s: XMFLOAT3 < last element failed\n", TestName); ret = MATH_FAIL; }
    if (d < a) { printe("%s: XMFLOAT3 < last element reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(d > a)) { printe("%s: XMFLOAT3 > last element failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> d)) { printe("%s: XMFLOAT3 <=> last element failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}

//-------------------------------------------------------------------------------------
// TestSpaceShip12 - XMFLOAT4X4 (std::partial_ordering)
//-------------------------------------------------------------------------------------
HRESULT TestSpaceShip12(LogProxy* pLog)
{
    static_assert(std::is_same_v<decltype(std::declval<const XMFLOAT4X4&>() <=> std::declval<const XMFLOAT4X4&>()), std::partial_ordering>);

    HRESULT ret = MATH_PASS;
    const XMFLOAT4X4 a(1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f,
                        0.f, 0.f, 0.f, 1.f);
    const XMFLOAT4X4 b(1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f,
                        0.f, 0.f, 0.f, 1.f);
    const XMFLOAT4X4 c(2.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f,
                        0.f, 0.f, 0.f, 1.f);
    const XMFLOAT4X4 d(1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f,
                        0.f, 0.f, 0.f, 2.f);  // differs only at _44 (last element)

    if (!(a == b)) { printe("%s: XMFLOAT4X4 == equal failed\n", TestName); ret = MATH_FAIL; }
    if (a != b) { printe("%s: XMFLOAT4X4 != equal failed\n", TestName); ret = MATH_FAIL; }
    if (a == c) { printe("%s: XMFLOAT4X4 == unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a != c)) { printe("%s: XMFLOAT4X4 != unequal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a < c)) { printe("%s: XMFLOAT4X4 < failed\n", TestName); ret = MATH_FAIL; }
    if (c < a) { printe("%s: XMFLOAT4X4 < reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(c > a)) { printe("%s: XMFLOAT4X4 > failed\n", TestName); ret = MATH_FAIL; }
    if (a > c) { printe("%s: XMFLOAT4X4 > reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= b)) { printe("%s: XMFLOAT4X4 <= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(a <= c)) { printe("%s: XMFLOAT4X4 <= less failed\n", TestName); ret = MATH_FAIL; }
    if (!(b >= a)) { printe("%s: XMFLOAT4X4 >= equal failed\n", TestName); ret = MATH_FAIL; }
    if (!(c >= a)) { printe("%s: XMFLOAT4X4 >= greater failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_eq(a <=> b)) { printe("%s: XMFLOAT4X4 <=> equal failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> c)) { printe("%s: XMFLOAT4X4 <=> less failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_gt(c <=> a)) { printe("%s: XMFLOAT4X4 <=> greater failed\n", TestName); ret = MATH_FAIL; }

    // Verify ordering on non-first element (_44 differs)
    if (!(a < d)) { printe("%s: XMFLOAT4X4 < last element failed\n", TestName); ret = MATH_FAIL; }
    if (d < a) { printe("%s: XMFLOAT4X4 < last element reverse failed\n", TestName); ret = MATH_FAIL; }
    if (!(d > a)) { printe("%s: XMFLOAT4X4 > last element failed\n", TestName); ret = MATH_FAIL; }
    if (!std::is_lt(a <=> d)) { printe("%s: XMFLOAT4X4 <=> last element failed\n", TestName); ret = MATH_FAIL; }

    return ret;
}
