//-------------------------------------------------------------------------------------
// frustum.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

static const float EPSILON = 0.001f;

inline bool IsEqual(const BoundingFrustum& f1, const BoundingFrustum& f2)
{
    return ((fabs(f1.Origin.x - f2.Origin.x) < EPSILON)
        && (fabs(f1.Origin.y - f2.Origin.y) < EPSILON)
        && (fabs(f1.Origin.z - f2.Origin.z) < EPSILON)
        && (fabs(f1.Orientation.x - f2.Orientation.x) < EPSILON)
        && (fabs(f1.Orientation.y - f2.Orientation.y) < EPSILON)
        && (fabs(f1.Orientation.z - f2.Orientation.z) < EPSILON)
        && (fabs(f1.Orientation.w - f2.Orientation.w) < EPSILON)
        && (fabs(f1.RightSlope - f2.RightSlope) < EPSILON)
        && (fabs(f1.LeftSlope - f2.LeftSlope) < EPSILON)
        && (fabs(f1.TopSlope - f2.TopSlope) < EPSILON)
        && (fabs(f1.BottomSlope - f2.BottomSlope) < EPSILON)
        && (fabs(f1.Near - f2.Near) < EPSILON)
        && (fabs(f1.Far - f2.Far) < EPSILON)) ? true : false;
}

#define printxmv(v) printe("%s: %f,%f,%f,%f\n", #v, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v))

#define printsh(v) printe("%s: center=%f,%f,%f  radius=%f\n", #v, v.Center.x, v.Center.y, v.Center.z, v.Radius )

#define printbb(v) printe("%s: center=%f,%f,%f  extents=%f,%f,%f\n", #v, v.Center.x, v.Center.y, v.Center.z, v.Extents.x, v.Extents.y, v.Extents.z )

#define printobb(v) printe("%s: center=%f,%f,%f  extents=%f,%f,%f  orientation=%f,%f,%f,%f\n", #v, \
                          v.Center.x, v.Center.y, v.Center.z, v.Extents.x, v.Extents.y, v.Extents.z, \
                          v.Orientation.x, v.Orientation.y, v.Orientation.z, v.Orientation.w )

#define printfr(v) printe("%s: origin=%f,%f,%f\n\torientation=%f,%f,%f,%f\n\tright=%f  left=%f\n\ttop=%f  bottom=%f\n\tnear=%f  far=%f\n", #v, \
                          v.Origin.x, v.Origin.y, v.Origin.z, v.Orientation.x, v.Orientation.y, v.Orientation.z, v.Orientation.w, \
                          v.RightSlope, v.LeftSlope, v.TopSlope, v.BottomSlope, v.Near, v.Far )

#define printct(t) switch(t) { case DISJOINT: printe("%s: DISJOINT", #t); break; \
                               case INTERSECTS: printe("%s: INTERSECTS", #t); break; \
                               case CONTAINS: printe("%s: CONTAINS", #t); break; } printe("\n")


//-------------------------------------------------------------------------------------
// BoundingFrustum
HRESULT TestF01(LogProxy* pLog)
{
    bool success = true;

    static_assert(std::is_nothrow_copy_assignable<BoundingFrustum>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<BoundingFrustum>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<BoundingFrustum>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<BoundingFrustum>::value, "Move Assign.");

    // Default constructor
    BoundingFrustum fr;
    if (fabs(fr.Origin.x) > EPSILON
        || fabs(fr.Origin.y) > EPSILON
        || fabs(fr.Origin.z) > EPSILON
        || fabs(fr.Orientation.x) > EPSILON
        || fabs(fr.Orientation.y) > EPSILON
        || fabs(fr.Orientation.z) > EPSILON
        || fabs(fr.Orientation.w - 1.f) > EPSILON
        || fabs(fr.RightSlope - 1.0f) > EPSILON
        || fabs(fr.LeftSlope + 1.0f) > EPSILON
        || fabs(fr.TopSlope - 1.0f) > EPSILON
        || fabs(fr.BottomSlope + 1.0f) > EPSILON
        || fabs(fr.Near) > EPSILON
        || fabs(fr.Far - 1.0f) > EPSILON)
    {
        printe("%s: Default constructor didn't create unit frustum\n", TestName);
        printfr(fr);
        success = false;
    }

    // Full constructor
    const BoundingFrustum frp(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
    if (fabs(frp.Origin.x - 1.f) > EPSILON
        || fabs(frp.Origin.y - 1.f) > EPSILON
        || fabs(frp.Origin.z - 1.f) > EPSILON
        || fabs(frp.Orientation.x) > EPSILON
        || fabs(frp.Orientation.y) > EPSILON
        || fabs(frp.Orientation.z) > EPSILON
        || fabs(frp.Orientation.w - 1.f) > EPSILON
        || fabs(frp.RightSlope - 2.0f) > EPSILON
        || fabs(frp.LeftSlope + 2.0f) > EPSILON
        || fabs(frp.TopSlope - 2.0f) > EPSILON
        || fabs(frp.BottomSlope + 2.0f) > EPSILON
        || fabs(frp.Near - 1.f) > EPSILON
        || fabs(frp.Far - 2.0f) > EPSILON)
    {
        printe("%s: Constructor didn't create correct frustum\n", TestName);
        printfr(frp);
        success = false;
    }

    // Copy constructor
    BoundingFrustum frc(frp);
    if (!IsEqual(frc, frp))
    {
        printe("%s: Copy constructor failed\n", TestName);
        printfr(frc);
        printfr(frp);
        success = false;
    }

    // Test assignment operator
    BoundingFrustum fri;
    fri = frp;
    if (!IsEqual(fri, frp))
    {
        printe("%s: Assignment operator failed\n", TestName);
        printfr(fri);
        printfr(frp);
        success = false;
    }

    // Matrix constructor covered by CreateFromMatrix

    return (success) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingFrustum::Transform
HRESULT TestF02(LogProxy* pLog)
{
    bool success = true;

    BoundingFrustum ufr, vfr;

    XMMATRIX Mid = XMMatrixIdentity();
    XMVECTOR Rid = XMQuaternionIdentity();
    XMVECTOR Tid = XMVectorZero();

    XMMATRIX mat = XMMatrixTransformation(g_XMZero, // Scaling origin
        Rid, // Scaling orientation
        XMVectorSet(1.f, 1.f, 1.f, 0.f), // Scaling
        g_XMZero, // Rotation origin
        Rid, // Rotation
        g_XMOne // Translation
    );

    XMMATRIX matScale = XMMatrixTransformation(g_XMZero, // Scaling origin
        Rid, // Scaling orientation
        XMVectorSet(3.f, 3.f, 3.f, 0.f), // Scaling
        g_XMZero, // Rotation origin
        Rid, // Rotation
        g_XMOne // Translation
    );

    XMMATRIX matNonUniform = XMMatrixTransformation(g_XMZero, // Scaling origin
        Rid, // Scaling orientation
        XMVectorSet(1.f, 2.f, 3.f, 0.f), // Non-uniform scaling
        g_XMZero, // Rotation origin
        Rid, // Rotation
        g_XMOne // Translation
    );

    // Unit frustum 
    const BoundingFrustum unit;

    unit.Transform(ufr, Mid);
    if (!IsEqual(unit, ufr))
    {
        printe("%s: Unit frustum tranform(1) failed\n", TestName);
        printfr(unit);
        printfr(ufr);
        success = false;
    }

    unit.Transform(vfr, 1.0f, Rid, Tid);
    if (!IsEqual(unit, vfr))
    {
        printe("%s: Unit frustum tranform(2) failed\n", TestName);
        printfr(unit);
        printfr(vfr);
        success = false;
    }

    // Small box (no rotation, no orientation)
    const BoundingFrustum _small(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

    _small.Transform(ufr, mat);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 2.f) > EPSILON
        || fabs(ufr.Origin.z - 2.f) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 1.f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 1.f) > EPSILON
        || fabs(ufr.Far - 2.0f) > EPSILON)
    {
        printe("%s: Small transform(1) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        success = false;
    }

    _small.Transform(vfr, 1.0f, Rid, g_XMOne);

    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(2) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (no rotation, no orientation w/ scale)
    _small.Transform(ufr, matScale);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 4.f) > EPSILON
        || fabs(ufr.Origin.z - 4.f) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 1.f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 3.f) > EPSILON
        || fabs(ufr.Far - 6.0f) > EPSILON)
    {
        printe("%s: Small transform(3) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        success = false;
    }

    _small.Transform(vfr, 3.0f, Rid, g_XMOne);

    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(4) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (no rotation, no orientation w/ non-uniform scale)
    _small.Transform(ufr, matNonUniform);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 3.f) > EPSILON
        || fabs(ufr.Origin.z - 4.f) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 1.f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 3.f) > EPSILON
        || fabs(ufr.Far - 6.0f) > EPSILON)
    {
        printe("%s: Small transform(5) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        success = false;
    }

    if (IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(6) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (no rotation, orientation)
    const BoundingFrustum _small2(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

    _small2.Transform(ufr, mat);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 2.f) > EPSILON
        || fabs(ufr.Origin.z - 2.f) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.z - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.844623f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 1.f) > EPSILON
        || fabs(ufr.Far - 2.0f) > EPSILON)
    {
        printe("%s: Small transform(7) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        success = false;
    }

    _small2.Transform(vfr, 1.0f, Rid, g_XMOne);

    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(8) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (no rotation, orientation w/ scale)
    _small2.Transform(ufr, matScale);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 4.f) > EPSILON
        || fabs(ufr.Origin.z - 4.f) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.z - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.844623f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 3.f) > EPSILON
        || fabs(ufr.Far - 6.0f) > EPSILON)
    {
        printe("%s: Small transform(9) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        success = false;
    }

    _small2.Transform(vfr, 3.0f, Rid, g_XMOne);

    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(10) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (no rotation, orientation w/ non-uniform scale)
    _small2.Transform(ufr, matNonUniform);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 3.f) > EPSILON
        || fabs(ufr.Origin.z - 4.f) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.z - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.844623f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 3.f) > EPSILON
        || fabs(ufr.Far - 6.0f) > EPSILON)
    {
        printe("%s: Small transform(11) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        success = false;
    }

    if (IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(12) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no rotation, no orientation)
    const BoundingFrustum big(XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 25.f, -20.f, 22.f, -21.f, 16.f, 24.f);

    big.Transform(ufr, mat);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 3.f) > EPSILON
        || fabs(ufr.Origin.z - 4.f) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 1.f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 16.f) > EPSILON
        || fabs(ufr.Far - 24.f) > EPSILON)
    {
        printe("%s: Big transform(1) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    big.Transform(vfr, 1.0f, Rid, g_XMOne);
    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(2) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no rotation, no orientation w/ scale)
    big.Transform(ufr, matScale);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 7.f) > EPSILON
        || fabs(ufr.Origin.z - 10.f) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 1.f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 48.f) > EPSILON
        || fabs(ufr.Far - 72.f) > EPSILON)
    {
        printe("%s: Big transform(3) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    big.Transform(vfr, 3.0f, Rid, g_XMOne);
    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(4) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no rotation, no orientation w/ non-uniform scale)
    big.Transform(ufr, matNonUniform);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 5.f) > EPSILON
        || fabs(ufr.Origin.z - 10.f) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 1.f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 48.f) > EPSILON
        || fabs(ufr.Far - 72.f) > EPSILON)
    {
        printe("%s: Big transform(5) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    if (IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(6) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no rotation, orientation)
    const BoundingFrustum big2(XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f), 25.f, -20.f, 22.f, -21.f, 16.f, 24.f);

    big2.Transform(ufr, mat);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 3.f) > EPSILON
        || fabs(ufr.Origin.z - 4.f) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.z - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.844623f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 16.f) > EPSILON
        || fabs(ufr.Far - 24.f) > EPSILON)
    {
        printe("%s: Big transform(7) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    big2.Transform(vfr, 1.0f, Rid, g_XMOne);
    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(8) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no rotation, orientation w/ scale)
    big2.Transform(ufr, matScale);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 7.f) > EPSILON
        || fabs(ufr.Origin.z - 10.f) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.z - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.844623f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 48.f) > EPSILON
        || fabs(ufr.Far - 72.f) > EPSILON)
    {
        printe("%s: Big transform(9) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    big2.Transform(vfr, 3.0f, Rid, g_XMOne);
    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(10) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no rotation, orientation w/ non-uniform scale)
    big2.Transform(ufr, matNonUniform);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 5.f) > EPSILON
        || fabs(ufr.Origin.z - 10.f) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.z - 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.844623f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 48.f) > EPSILON
        || fabs(ufr.Far - 72.f) > EPSILON)
    {
        printe("%s: Big transform(11) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    if (IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(12) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (no orientation)
    XMVECTOR rot = XMQuaternionRotationRollPitchYaw(0.f, XM_PIDIV2, 0.f);

    mat = XMMatrixTransformation(g_XMZero, // Scaling origin
        Rid, // Scaling orientation
        XMVectorSet(1.f, 1.f, 1.f, 0.f), // Scaling
        g_XMZero, // Rotation origin
        rot, // Rotation
        g_XMOne // Translation
    );

    matScale = XMMatrixTransformation(g_XMZero, // Scaling origin
        Rid, // Scaling orientation
        XMVectorSet(3.f, 3.f, 3.f, 0.f), // Scaling
        g_XMZero, // Rotation origin
        rot, // Rotation
        g_XMOne // Translation
    );

    matNonUniform = XMMatrixTransformation(g_XMZero, // Scaling origin
        Rid, // Scaling orientation
        XMVectorSet(1.f, 2.f, 3.f, 0.f), // Non-uniform scaling
        g_XMZero, // Rotation origin
        rot, // Rotation
        g_XMOne // Translation
    );

    _small.Transform(ufr, mat);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 2.f) > EPSILON
        || fabs(ufr.Origin.z) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y - 0.707107f) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 0.707107f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 1.f) > EPSILON
        || fabs(ufr.Far - 2.0f) > EPSILON)
    {
        printe("%s: Small transform(13) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        success = false;
    }

    _small.Transform(vfr, 1.0f, rot, g_XMOne);

    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(14) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (no orientation w/ scale)
    _small.Transform(ufr, matScale);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 4.f) > EPSILON
        || fabs(ufr.Origin.z + 2.f) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y - 0.707107f) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 0.707107f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 3.f) > EPSILON
        || fabs(ufr.Far - 6.0f) > EPSILON)
    {
        printe("%s: Small transform(15) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        success = false;
    }

    _small.Transform(vfr, 3.0f, rot, g_XMOne);

    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(16) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (no orientation w/ non-uniform scale)
    _small.Transform(ufr, matNonUniform);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 3.f) > EPSILON
        || fabs(ufr.Origin.z) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y - 0.707107f) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 0.707107f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 3.f) > EPSILON
        || fabs(ufr.Far - 6.0f) > EPSILON)
    {
        printe("%s: Small transform(17) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        success = false;
    }

    if (IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(18) failed\n", TestName);
        printfr(_small);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (w/ orientation)
    _small2.Transform(ufr, mat);
    if (fabs(ufr.Origin.x - 2.f) > EPSILON
        || fabs(ufr.Origin.y - 2.f) > EPSILON
        || fabs(ufr.Origin.z) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.732538f) > EPSILON
        || fabs(ufr.Orientation.z + 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.461939f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 1.f) > EPSILON
        || fabs(ufr.Far - 2.0f) > EPSILON)
    {
        printe("%s: Small transform(19) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        success = false;
    }

    _small2.Transform(vfr, 1.0f, rot, g_XMOne);

    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(20) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (w/ orientation and scale)
    _small2.Transform(ufr, matScale);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 4.f) > EPSILON
        || fabs(ufr.Origin.z + 2.f) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.732538f) > EPSILON
        || fabs(ufr.Orientation.z + 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.461939f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 3.f) > EPSILON
        || fabs(ufr.Far - 6.f) > EPSILON)
    {
        printe("%s: Small transform(21) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        success = false;
    }

    _small2.Transform(vfr, 3.0f, rot, g_XMOne);

    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(22) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Small box (w/ orientation and non-uniform scale)
    _small2.Transform(ufr, matNonUniform);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 3.f) > EPSILON
        || fabs(ufr.Origin.z) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.732538f) > EPSILON
        || fabs(ufr.Orientation.z + 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.461939f) > EPSILON
        || fabs(ufr.RightSlope - 2.0f) > EPSILON
        || fabs(ufr.LeftSlope + 2.0f) > EPSILON
        || fabs(ufr.TopSlope - 2.0f) > EPSILON
        || fabs(ufr.BottomSlope + 2.0f) > EPSILON
        || fabs(ufr.Near - 3.f) > EPSILON
        || fabs(ufr.Far - 6.f) > EPSILON)
    {
        printe("%s: Small transform(23) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        success = false;
    }

    if (IsEqual(ufr, vfr))
    {
        printe("%s: Small transform(24) failed\n", TestName);
        printfr(_small2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no orientation)
    big.Transform(ufr, mat);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 3.f) > EPSILON
        || fabs(ufr.Origin.z) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y - 0.707107f) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 0.707107f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 16.f) > EPSILON
        || fabs(ufr.Far - 24.f) > EPSILON)
    {
        printe("%s: Big transform(13) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    big.Transform(vfr, 1.0f, rot, g_XMOne);
    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(14) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no orientation w/ scale)
    big.Transform(ufr, matScale);
    if (fabs(ufr.Origin.x - 10.f) > EPSILON
        || fabs(ufr.Origin.y - 7.f) > EPSILON
        || fabs(ufr.Origin.z + 2.f) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y - 0.707107f) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 0.707107f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 48.f) > EPSILON
        || fabs(ufr.Far - 72.f) > EPSILON)
    {
        printe("%s: Big transform(15) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    big.Transform(vfr, 3.0f, rot, g_XMOne);
    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(16) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (no orientation w/ non-uniform scale)
    big.Transform(ufr, matNonUniform);
    if (fabs(ufr.Origin.x - 10.f) > EPSILON
        || fabs(ufr.Origin.y - 5.f) > EPSILON
        || fabs(ufr.Origin.z) > EPSILON
        || fabs(ufr.Orientation.x) > EPSILON
        || fabs(ufr.Orientation.y - 0.707107f) > EPSILON
        || fabs(ufr.Orientation.z) > EPSILON
        || fabs(ufr.Orientation.w - 0.707107f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 48.f) > EPSILON
        || fabs(ufr.Far - 72.f) > EPSILON)
    {
        printe("%s: Big transform(17) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        success = false;
    }

    if (IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(18) failed\n", TestName);
        printfr(big);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (w/ orientation)
    big2.Transform(ufr, mat);
    if (fabs(ufr.Origin.x - 4.f) > EPSILON
        || fabs(ufr.Origin.y - 3.f) > EPSILON
        || fabs(ufr.Origin.z) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.732538f) > EPSILON
        || fabs(ufr.Orientation.z + 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.461939f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 16.f) > EPSILON
        || fabs(ufr.Far - 24.f) > EPSILON)
    {
        printe("%s: Big transform(19) failed\n", TestName);
        printfr(big2);
        printfr(ufr);
        success = false;
    }

    big2.Transform(vfr, 1.0f, rot, g_XMOne);
    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(20) failed\n", TestName);
        printfr(big2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (w/ orientation and scale)
    big2.Transform(ufr, matScale);
    if (fabs(ufr.Origin.x - 10.f) > EPSILON
        || fabs(ufr.Origin.y - 7.f) > EPSILON
        || fabs(ufr.Origin.z + 2.f) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.732538f) > EPSILON
        || fabs(ufr.Orientation.z + 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.461939f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 48.f) > EPSILON
        || fabs(ufr.Far - 72.f) > EPSILON)
    {
        printe("%s: Big transform(21) failed\n", TestName);
        printfr(big2);
        printfr(ufr);
        success = false;
    }

    big2.Transform(vfr, 3.0f, rot, g_XMOne);
    if (!IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(22) failed\n", TestName);
        printfr(big2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    // Big box (w/ orientation and non-uniform scale)
    big2.Transform(ufr, matNonUniform);
    if (fabs(ufr.Origin.x - 10.f) > EPSILON
        || fabs(ufr.Origin.y - 5.f) > EPSILON
        || fabs(ufr.Origin.z) > EPSILON
        || fabs(ufr.Orientation.x - 0.461940f) > EPSILON
        || fabs(ufr.Orientation.y - 0.732538f) > EPSILON
        || fabs(ufr.Orientation.z + 0.191342f) > EPSILON
        || fabs(ufr.Orientation.w - 0.461939f) > EPSILON
        || fabs(ufr.RightSlope - 25.0f) > EPSILON
        || fabs(ufr.LeftSlope + 20.0f) > EPSILON
        || fabs(ufr.TopSlope - 22.0f) > EPSILON
        || fabs(ufr.BottomSlope + 21.0f) > EPSILON
        || fabs(ufr.Near - 48.f) > EPSILON
        || fabs(ufr.Far - 72.f) > EPSILON)
    {
        printe("%s: Big transform(23) failed\n", TestName);
        printfr(big2);
        printfr(ufr);
        success = false;
    }

    if (IsEqual(ufr, vfr))
    {
        printe("%s: Big transform(24) failed\n", TestName);
        printfr(big2);
        printfr(ufr);
        printfr(vfr);
        success = false;
    }

    return (success) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingFrustum::GetCorners
HRESULT TestF03(LogProxy* pLog)
{
    bool success = true;

    static_assert(BoundingFrustum::CORNER_COUNT == 8, "TestF03 expects there are 8 corners to a frustum");

    {
        const BoundingFrustum fr(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

        XMFLOAT3 corners[BoundingFrustum::CORNER_COUNT];
        fr.GetCorners(corners);

        static const float expected[][3] = { { -1.f, 3.f, 2.f }, { 3.f, 3.f, 2.f }, { 3.f, -1.f, 2.f }, { -1.f, -1.f, 2.f },
                                             { -3.f, 5.f, 3.f }, { 5.f, 5.f, 3.f }, { 5.f, -3.f, 3.f }, { -3.f, -3.f, 3.f } };

        for (size_t i = 0; i < BoundingFrustum::CORNER_COUNT; ++i)
        {
            if (fabs(corners[i].x - expected[i][0]) > EPSILON
                || fabs(corners[i].y - expected[i][1]) > EPSILON
                || fabs(corners[i].z - expected[i][2]) > EPSILON)
            {
                printe("%s: GetCorners(1) failed", TestName);
                printe("%f %f %f ... %f %f %f \n", corners[i].x, corners[i].y, corners[i].z, expected[i][0], expected[i][1], expected[i][2]);
                success = false;
            }
        }
    }

    {
        XMMATRIX matrix = XMMatrixPerspectiveFovRH(XM_PIDIV2 /* 90 degrees */, 1.f, 1.f, 100.f);

        BoundingFrustum fr;
        BoundingFrustum::CreateFromMatrix(fr, matrix);

        XMFLOAT3 corners[BoundingFrustum::CORNER_COUNT];
        fr.GetCorners(corners);

        static const float expected[][3] = { { -1.f, 1.f, -1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f }, { -1.f, -1.f, -1.f },
                                             { -100.000694f, 100.000694f, -100.000694f }, { 100.000694f, 100.000694f, -100.000694f }, { 100.000694f, -100.000694f, -100.000694f }, { -100.000694f, -100.000694f, -100.000694f } };

        for (size_t i = 0; i < BoundingFrustum::CORNER_COUNT; ++i)
        {
            if (fabs(corners[i].x - expected[i][0]) > EPSILON
                || fabs(corners[i].y - expected[i][1]) > EPSILON
                || fabs(corners[i].z - expected[i][2]) > EPSILON)
            {
                printe("%s: GetCorners(2) failed", TestName);
                printe("%f %f %f ... %f %f %f \n", corners[i].x, corners[i].y, corners[i].z, expected[i][0], expected[i][1], expected[i][2]);
                success = false;
            }
        }
    }

    return (success) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingFrustum::Contains
HRESULT TestF04(LogProxy* pLog)
{
    bool success = true;

    ContainmentType c;
    const BoundingFrustum unit;
    const BoundingFrustum revfr(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(0.f, 1.f, 0.f, 0.f), 1.f, -1.f, 1.f, -1.f, 0.f, 1.f);

    // Frustum-Point tests
    {
        const XMVECTORF32 pnts_in[8] =
        {
            { 0.f, 0.f, 0.f, _Q_NAN },
            { 0.f, 0.f, 1.f, _Q_NAN },
            { 0.5f, 0.5f, 0.5f, _Q_NAN },
            { 1.f, 1.f, 1.f, _Q_NAN },
            { 0.5f, 0.f, 0.5f, _Q_NAN },
            { 0.f, 0.5f, 0.5f, _Q_NAN },
            { 0.5f, 0.f, 0.5f, _Q_NAN },
            { 0.f, 0.f, 0.9f, _Q_NAN },
        };

        const XMVECTORF32 pnts_out[8] =
        {
            { 1.f, 0.f, 0.f, _Q_NAN },
            { 0.f, 1.f, 0.f, _Q_NAN },
            { -0.5f, -0.5f, -0.5f, _Q_NAN },
            { 1.1f, 0.f, 0.f, _Q_NAN },
            { 10.f, -10.f, -15.f, _Q_NAN },
            { 0, -1.1f, 0.f, _Q_NAN },
            { -20.f, -20.f, -20.f, _Q_NAN },
            { 1.f, 2.f, 3.f, _Q_NAN }
        };

        static_assert(sizeof(pnts_in) == sizeof(pnts_out), "TestF04 Frustum-point test");

        for (uint32_t i = 0; i < (sizeof(pnts_in) / sizeof(XMVECTORF32)); ++i)
        {
            if ((c = unit.Contains(pnts_in[i].v)) != CONTAINS)
            {
                printe("%s: Point-Frustum test failed (ins %d)\n", TestName, i);
                printfr(unit);
                printxmv(pnts_in[i].v);
                printct(c);
                success = false;
            }

            if ((c = unit.Contains(pnts_out[i].v)) != DISJOINT)
            {
                printe("%s: Point-Frustum test failed (outs %d)\n", TestName, i);
                printfr(unit);
                printxmv(pnts_out[i].v);
                printct(c);
                success = false;
            }

            if (i != 0 && (c = revfr.Contains(pnts_in[i].v)) != DISJOINT)
            {
                printe("%s: Point-Frustum test failed (ins %d)\n", TestName, i);
                printfr(revfr);
                printxmv(pnts_in[i].v);
                printct(c);
                success = false;
            }
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f }, { -2.0f, 0.0f, 2.0f },
                                         {  0.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f },
                                         {  2.0f,-2.0f, 2.0f }, { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f },
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f },
                                         { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0}, { 0, 0, -.5f },
                                         { -0.5f, 0.5f, 0.0f }, {  0.0f, 0.5f, 0.0f }, {  0.5f, 0.5f, 0.0f },
                                         { -0.5f, 0.0f, 0.0f }, {  0.5f, 0.0f, 0.0f }, { -0.5f,-0.5f, 0.0f }, {  0.0f,-0.5f, 0.0f },
                                         {  0.5f,-0.5f, 0.0f }, { -0.5f, 0.5f,-0.5f }, {  0.0f, 0.5f,-0.5f }, {  0.5f, 0.5f,-0.5f },
                                         { -0.5f, 0.0f,-0.5f }, {  0.0f, 0.0f,-0.5f }, {  0.5f, 0.0f,-0.5f }, { -0.5f,-0.5f,-0.5f },
                                         {  0.0f,-0.5f,-0.5f }, {  0.5f,-0.5f,-0.5f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            XMVECTOR point = XMVectorSet(test[i][0], test[i][1], test[i][2], 0);
            ContainmentType ct = unit.Contains(point);
            if (ct != DISJOINT)
            {
                printe("%s: Failed Point-Frustum test1 %zu\n", TestName, i);
                printfr(unit);
                printxmv(point);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, .5f },
                                         { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f }, {  0.5f, 0.5f, 0.5f }, { -0.5f, 0.0f, 0.5f },
                                         {  0.0f, 0.0f, 0.5f }, {  0.5f, 0.0f, 0.5f }, { -0.4f,-0.4f, 0.5f }, {  0.0f,-0.5f, 0.5f },
                                         {  0.5f,-0.5f, 0.5f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            XMVECTOR point = XMVectorSet(test2[i][0], test2[i][1], test2[i][2], 0);
            ContainmentType ct = unit.Contains(point);
            if (ct != CONTAINS)
            {
                printe("%s: Failed Point-Frustum test2 %zu\n", TestName, i);
                printfr(unit);
                printxmv(point);
                printct(ct);
                success = false;
            }
        }
    }

    // Frustum-Triangle
    {
        const XMVECTORF32 tri_INTERSECTS[3] =
        {
            { 0.5f, 0.5f, 0.5f, 0.f },
            { 1.0f, 0.f, 1.f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_DISJOINT[3] =
        {
            { 10.f, 10.f, 10.f, 0.f },
            { 2.0f, 0.f, 2.f, 0.f },
            { 5.f, 5.f, 5.f, 0.f }
        };

        static_assert((sizeof(tri_INTERSECTS) == sizeof(tri_DISJOINT)), "TestF04 frustum-tri tests");

        for (uint32_t i = 0; i < (sizeof(tri_INTERSECTS) / sizeof(XMVECTORF32)); i += 3)
        {
            XMVECTOR t0 = tri_INTERSECTS[i].v;
            XMVECTOR t1 = tri_INTERSECTS[i + 1].v;
            XMVECTOR t2 = tri_INTERSECTS[i + 2].v;
            c = unit.Contains(t0, t1, t2);
            if (c != INTERSECTS)
            {
                printe("%s: Triangle-Frustum test failed (INTERSECTS %d)\n", TestName, i);
                printct(c);
                printfr(unit);
                printxmv(t0);
                printxmv(t1);
                printxmv(t2);
                success = false;
            }

            t0 = tri_DISJOINT[i].v;
            t1 = tri_DISJOINT[i + 1].v;
            t2 = tri_DISJOINT[i + 2].v;
            c = unit.Contains(t0, t1, t2);
            if (c != DISJOINT)
            {
                printe("%s: Triangle-Frustum test failed (DISJOINT %d)\n", TestName, i);
                printct(c);
                printfr(unit);
                printxmv(t0);
                printxmv(t1);
                printxmv(t2);
                success = false;
            }
        }
    }

    // Frustum-Sphere
    {
        BoundingSphere sunit;
        if ((c = unit.Contains(sunit)) != INTERSECTS)
        {
            printe("%s: Sphere-Frustum unit test failed\n", TestName);
            printfr(unit);
            printsh(sunit);
            printct(c);
            success = false;
        }

        if ((c = revfr.Contains(sunit)) != INTERSECTS)
        {
            printe("%s: Sphere-Frustum revfr test failed\n", TestName);
            printfr(revfr);
            printsh(sunit);
            printct(c);
            success = false;
        }

        const BoundingSphere _small(XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f);
        if ((c = unit.Contains(_small)) != INTERSECTS)
        {
            printe("%s: Sphere-Frustum small test failed\n", TestName);
            printfr(unit);
            printsh(_small);
            printct(c);
            success = false;
        }

        const BoundingSphere big(XMFLOAT3(1.f, 2.f, 3.f), 10.0f);
        if ((c = unit.Contains(big)) != INTERSECTS)
        {
            printe("%s: Sphere-Frustum big test failed\n", TestName);
            printfr(unit);
            printsh(big);
            printct(c);
            success = false;
        }

        const BoundingSphere _far(XMFLOAT3(10.f, -5.f, 4.f), 2.f);
        if ((c = unit.Contains(_far)) != DISJOINT)
        {
            printe("%s: Sphere-Frustum far test failed\n", TestName);
            printfr(unit);
            printsh(_far);
            printct(c);
            success = false;
        }

        static const float test[][3] = { { 1.f, 0, 0 }, { -1.f, 0, 0 }, { 0, 1.f, 0 }, { 0, -1.f, 0 }, { 0, 0, 1.f }, { 0, 0, -1.f },
                                         { -1.f, 1.f, 1.f }, { 0.0f, 1.f, 1.f }, {  1.f, 1.f, 1.f }, { -1.f, 0.0f, 1.f },
                                         {  0.0f, 0.0f, 1.f }, {  1.f, 0.0f, 1.f }, { -1.f,-1.f, 1.f }, {  0.0f,-1.f, 1.f },
                                         {  1.f,-1.f, 1.f }, { -1.f, 1.f, 0.0f }, {  0.0f, 1.f, 0.0f }, {  1.f, 1.f, 0.0f },
                                         { -1.f, 0.0f, 0.0f }, {  1.f, 0.0f, 0.0f }, { -1.f,-1.f, 0.0f }, {  0.0f,-1.f, 0.0f },
                                         {  1.f,-1.f, 0.0f }, { -1.f, 1.f,-1.f }, {  0.0f, 1.f,-1.f }, {  1.f, 1.f,-1.f },
                                         { -1.f, 0.0f,-1.f }, {  0.0f, 0.0f,-1.f }, {  1.f, 0.0f,-1.f }, { -1.f,-1.f,-1.f },
                                         {  0.0f,-1.f,-1.f }, {  1.f,-1.f,-1.f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0] * 2.1f, test[i][1] * 2.1f, test[i][2] * 2.1f);
            const BoundingSphere sph(center, 1.f);
            ContainmentType ct = unit.Contains(sph);
            if (ct != DISJOINT)
            {
                printe("%s: Failed sphere-Frustum test1 %zu\n", TestName, i);
                printfr(unit);
                printsh(sph);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f },
                                         { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f }, {  0.5f, 0.5f, 0.5f }, { -0.5f, 0.0f, 0.5f },
                                         {  0.0f, 0.0f, 0.5f }, {  0.5f, 0.0f, 0.5f }, { -0.5f,-0.5f, 0.5f }, {  0.0f,-0.5f, 0.5f },
                                         {  0.5f,-0.5f, 0.5f }, { -0.5f, 0.5f, 0.0f }, {  0.0f, 0.5f, 0.0f }, {  0.5f, 0.5f, 0.0f },
                                         { -0.5f, 0.0f, 0.0f }, {  0.5f, 0.0f, 0.0f }, { -0.5f,-0.5f, 0.0f }, {  0.0f,-0.5f, 0.0f },
                                         {  0.5f,-0.5f, 0.0f }, { -0.5f, 0.5f,-0.5f }, {  0.0f, 0.5f,-0.5f }, {  0.5f, 0.5f,-0.5f },
                                         { -0.5f, 0.0f,-0.5f }, {  0.0f, 0.0f,-0.5f }, {  0.5f, 0.0f,-0.5f }, { -0.5f,-0.5f,-0.5f },
                                         {  0.0f,-0.5f,-0.5f }, {  0.5f,-0.5f,-0.5f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            const BoundingSphere sph(center, 1.f);
            ContainmentType ct = unit.Contains(sph);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed sphere-Frustum test2A %zu\n", TestName, i);
                printfr(unit);
                printsh(sph);
                printct(ct);
                success = false;
            }

            ct = revfr.Contains(sph);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed sphere-Frustum test2B %zu\n", TestName, i);
                printfr(revfr);
                printsh(sph);
                printct(ct);
                success = false;
            }
        }
    }

    // Frustum-Box
    {
        BoundingBox bunit;
        if ((c = unit.Contains(bunit)) != INTERSECTS)
        {
            printe("%s: Box-Frustum unit test failed\n", TestName);
            printfr(unit);
            printbb(bunit);
            printct(c);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f }, { -2.0f, 0.0f, 2.0f },
                                         {  0.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f },
                                         {  2.0f,-2.0f, 2.0f }, { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f },
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            const BoundingBox box(center, XMFLOAT3(0.5f, 0.5f, 0.5f));
            ContainmentType ct = unit.Contains(box);
            if (ct != DISJOINT)
            {
                printe("%s: Failed box-Frustum axis-based test1A %zu\n", TestName, i);
                printfr(unit);
                printbb(box);
                printct(ct);
                success = false;
            }

            ct = revfr.Contains(box);
            if (ct != DISJOINT)
            {
                printe("%s: Failed box-Frustum axis-based test1B %zu\n", TestName, i);
                printfr(revfr);
                printbb(box);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f },
                                         { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f }, {  0.5f, 0.5f, 0.5f }, { -0.5f, 0.0f, 0.5f },
                                         {  0.0f, 0.0f, 0.5f }, {  0.5f, 0.0f, 0.5f }, { -0.5f,-0.5f, 0.5f }, {  0.0f,-0.5f, 0.5f },
                                         {  0.5f,-0.5f, 0.5f }, { -0.5f, 0.5f, 0.0f }, {  0.0f, 0.5f, 0.0f }, {  0.5f, 0.5f, 0.0f },
                                         { -0.5f, 0.0f, 0.0f }, {  0.5f, 0.0f, 0.0f }, { -0.5f,-0.5f, 0.0f }, {  0.0f,-0.5f, 0.0f },
                                         {  0.5f,-0.5f, 0.0f }, { -0.5f, 0.5f,-0.5f }, {  0.0f, 0.5f,-0.5f }, {  0.5f, 0.5f,-0.5f },
                                         { -0.5f, 0.0f,-0.5f }, {  0.0f, 0.0f,-0.5f }, {  0.5f, 0.0f,-0.5f }, { -0.5f,-0.5f,-0.5f },
                                         {  0.0f,-0.5f,-0.5f }, {  0.5f,-0.5f,-0.5f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            const BoundingBox box(center, XMFLOAT3(1.f, 1.f, 1.f));
            ContainmentType ct = unit.Contains(box);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed box-Frustum axis-based test2 %zu\n", TestName, i);
                printfr(unit);
                printbb(box);
                printct(ct);
                success = false;
            }
        }

        {
            const BoundingBox box(XMFLOAT3(0, 0, 0), XMFLOAT3(2.f, 2.f, 2.f));
            ContainmentType ct = unit.Contains(box);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed box-Frustum axis-based test3\n", TestName);
                printfr(unit);
                printbb(box);
                printct(ct);
                success = false;
            }
        }

        static const float test3[][3] = { { 1.f, 1.f, 1.f }, { 1.f, -1.f, 1.f }, { -1.f, 1.f, 1.f }, { -1.f, -1.f, 1.f } };
        for (size_t i = 0; i < sizeof(test3) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test3[i][0], test3[i][1], test3[i][2]);
            const BoundingBox box(center, XMFLOAT3(0.5f, 0.5f, 0.5f));
            ContainmentType ct = unit.Contains(box);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed box-Frustum axis-based test4 %zu\n", TestName, i);
                printfr(unit);
                printbb(box);
                printct(ct);
                success = false;
            }

            ct = revfr.Contains(box);
            if (ct != DISJOINT)
            {
                printe("%s: Failed box-Frustum axis-based test5 %zu\n", TestName, i);
                printfr(revfr);
                printbb(box);
                printct(ct);
                success = false;
            }
        }

        static const float test4[][3] = { { 1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f },  { -1.f, 1.f, -1.f }, { -1.f, -1.f, -1.f } };
        for (size_t i = 0; i < sizeof(test4) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test4[i][0], test4[i][1], test4[i][2]);
            const BoundingBox box(center, XMFLOAT3(0.5f, 0.5f, 0.5f));
            ContainmentType ct = unit.Contains(box);
            if (ct != DISJOINT)
            {
                printe("%s: Failed box-Frustum axis-based test6 %zu\n", TestName, i);
                printfr(unit);
                printbb(box);
                printct(ct);
                success = false;
            }

            ct = revfr.Contains(box);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed box-Frustum axis-based test7 %zu\n", TestName, i);
                printfr(revfr);
                printbb(box);
                printct(ct);
                success = false;
            }
        }
    }

    // Frustum-OrientedBox
    {
        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f }, { -2.0f, 0.0f, 2.0f },
                                         {  0.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f },
                                         {  2.0f,-2.0f, 2.0f }, { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f },
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            const BoundingOrientedBox box(center, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f));
            ContainmentType ct = unit.Contains(box);
            if (ct != DISJOINT)
            {
                printe("%s: Failed Frustum-oobox axis-based test1 %zu\n", TestName, i);
                printfr(unit);
                printobb(box);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f },
                                         { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f }, {  0.5f, 0.5f, 0.5f }, { -0.5f, 0.0f, 0.5f },
                                         {  0.0f, 0.0f, 0.5f }, {  0.5f, 0.0f, 0.5f }, { -0.5f,-0.5f, 0.5f }, {  0.0f,-0.5f, 0.5f },
                                         {  0.5f,-0.5f, 0.5f }, { -0.5f, 0.5f, 0.0f }, {  0.0f, 0.5f, 0.0f }, {  0.5f, 0.5f, 0.0f },
                                         { -0.5f, 0.0f, 0.0f }, {  0.5f, 0.0f, 0.0f }, { -0.5f,-0.5f, 0.0f }, {  0.0f,-0.5f, 0.0f },
                                         {  0.5f,-0.5f, 0.0f }, { -0.5f, 0.5f,-0.5f }, {  0.0f, 0.5f,-0.5f }, {  0.5f, 0.5f,-0.5f },
                                         { -0.5f, 0.0f,-0.5f }, {  0.0f, 0.0f,-0.5f }, {  0.5f, 0.0f,-0.5f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            const BoundingOrientedBox box(center, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f));
            ContainmentType ct = unit.Contains(box);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed Frustum-obox axis-based test2 %zu\n", TestName, i);
                printfr(unit);
                printobb(box);
                printct(ct);
                success = false;
            }
        }

        {
            const BoundingOrientedBox box(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f));
            ContainmentType ct = unit.Contains(box);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed Frustum-oobox axis-based test3\n", TestName);
                printfr(unit);
                printobb(box);
                printct(ct);
                success = false;
            }
        }

        static const float test3[][3] = { { 1.f, 1.f, 1.f }, { 1.f, -1.f, 1.f }, { -1.f, 1.f, 1.f }, { -1.f, -1.f, 1.f } };
        for (size_t i = 0; i < sizeof(test3) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test3[i][0], test3[i][1], test3[i][2]);
            const BoundingOrientedBox box(center, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f));
            ContainmentType ct = unit.Contains(box);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed Frustum-oobox axis-based test4 %zu\n", TestName, i);
                printfr(unit);
                printobb(box);
                printct(ct);
                success = false;
            }
        }

        static const float test4[][3] = { { 1.f, 1.f, -1.f }, { 1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f }, { -1.f, -1.f, -1.f } };
        for (size_t i = 0; i < sizeof(test4) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test4[i][0], test4[i][1], test4[i][2]);
            const BoundingOrientedBox box(center, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f));
            ContainmentType ct = unit.Contains(box);
            if (ct != DISJOINT)
            {
                printe("%s: Failed Frustum-oobox axis-based test5 %zu\n", TestName, i);
                printfr(unit);
                printobb(box);
                printct(ct);
                success = false;
            }
        }
    }

    // Frustum-Frustum
    {
        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f },
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = unit.Contains(fr);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed Frustum-frustum test1 %zu\n", TestName, i);
                printfr(unit);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = unit.Contains(fr);
            if (ct != DISJOINT)
            {
                printe("%s: Failed Frustum-frustum test2 %zu\n", TestName, i);
                printfr(unit);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }
    }

    {
        static const float test[][3] = { { 0, 0, -2.f },
                                         { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = revfr.Contains(fr);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed Frustum-frustum test3 %zu\n", TestName, i);
                printfr(revfr);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = revfr.Contains(fr);
            if (ct != DISJOINT)
            {
                printe("%s: Failed Frustum-frustum test4 %zu\n", TestName, i);
                printfr(revfr);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }
    }

    return (success) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingFrustum::Intersects
HRESULT TestF05(LogProxy* pLog)
{
    bool success = true;

    const BoundingFrustum unit;
    const BoundingFrustum revfr(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(0.f, 1.f, 0.f, 0.f), 1.f, -1.f, 1.f, -1.f, 0.f, 1.f);

    // OBox-Sphere
    {
        if (!unit.Intersects(unit))
        {
            printe("%s: Frustum-Sphere unit test failed\n", TestName);
            printfr(unit);
            success = false;
        }

        const BoundingSphere _small(XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f);
        if (!unit.Intersects(_small))
        {
            printe("%s: Frustum-Sphere small test failed\n", TestName);
            printfr(unit);
            printsh(_small);
            success = false;
        }

        const BoundingSphere big(XMFLOAT3(1.f, 2.f, 3.f), 10.0f);
        if (!unit.Intersects(big))
        {
            printe("%s: Frustum-Sphere big test failed\n", TestName);
            printfr(unit);
            printsh(big);
            success = false;
        }

        const BoundingSphere _far(XMFLOAT3(10.f, -5.f, 4.f), 2.f);
        if (unit.Intersects(_far))
        {
            printe("%s: Frustum-sphere far test failed\n", TestName);
            printfr(unit);
            printsh(_far);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const BoundingSphere t(XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f);

            const BoundingSphere t2(XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f - EPSILON);
            if (unit.Intersects(t2))
            {
                printe("%s: Failed Frustum-Sphere axis-based test1\n", TestName);
                printfr(unit);
                printsh(t2);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const BoundingSphere t(XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), 1.f);

            if (!unit.Intersects(t))
            {
                printe("%s: Failed Frustum-Sphere axis-based test2\n", TestName);
                printfr(unit);
                printsh(t);
                success = false;
            }
        }
    }

    {
        if (!revfr.Intersects(unit))
        {
            printe("%s: Frustum-Sphere revfr test failed\n", TestName);
            printfr(revfr);
            success = false;
        }

        const BoundingSphere _small(XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f);
        if (!revfr.Intersects(_small))
        {
            printe("%s: Frustum-Sphere small test2 failed\n", TestName);
            printfr(revfr);
            printsh(_small);
            success = false;
        }

        const BoundingSphere big(XMFLOAT3(1.f, 2.f, 3.f), 10.0f);
        if (!revfr.Intersects(big))
        {
            printe("%s: Frustum-Sphere big test2 failed\n", TestName);
            printfr(revfr);
            printsh(big);
            success = false;
        }

        const BoundingSphere _far(XMFLOAT3(10.f, -5.f, 4.f), 2.f);
        if (revfr.Intersects(_far))
        {
            printe("%s: Frustum-sphere far test2 failed\n", TestName);
            printfr(revfr);
            printsh(_far);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const BoundingSphere t(XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f);

            const BoundingSphere t2(XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f - EPSILON);
            if (revfr.Intersects(t2))
            {
                printe("%s: Failed Frustum-Sphere axis-based test3\n", TestName);
                printfr(revfr);
                printsh(t2);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const BoundingSphere t(XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), 1.f);

            if (!revfr.Intersects(t))
            {
                printe("%s: Failed Frustum-Sphere axis-based test4\n", TestName);
                printfr(revfr);
                printsh(t);
                success = false;
            }
        }
    }

    // BoundingFrustum::Intersects( const BoundingBox& box ) is covered by OOB case

    // Frustum-OBox
    {
        if (!unit.Intersects(unit))
        {
            printe("%s: Frustum-OBox unit test failed\n", TestName);
            printfr(unit);
            success = false;
        }

        const BoundingOrientedBox _small(XMFLOAT3(0.1f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
        if (!unit.Intersects(_small))
        {
            printe("%s: Frustum-OBox small test failed\n", TestName);
            printobb(_small);
            printfr(unit);
            success = false;
        }

        const BoundingOrientedBox big(XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(10.f, 10.f, 10.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
        if (!unit.Intersects(big))
        {
            printe("%s: Frustum-OBox big test failed\n", TestName);
            printobb(big);
            printfr(unit);
            success = false;
        }

        const BoundingOrientedBox _far(XMFLOAT3(10.f, -5.f, 4.f), XMFLOAT3(2.f, 2.f, 2.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
        if (unit.Intersects(_far))
        {
            printe("%s: Frustum-OBox far test failed\n", TestName);
            printobb(_far);
            printfr(unit);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const BoundingOrientedBox t(XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

            const BoundingOrientedBox t2(XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3(1.f - EPSILON, 1.f - EPSILON, 1.f - EPSILON), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
            if (t2.Intersects(unit))
            {
                printe("%s: Failed Frustum-OBox axis-based test1\n", TestName);
                printobb(t2);
                printfr(unit);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const BoundingOrientedBox t(XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

            if (!unit.Intersects(t))
            {
                printe("%s: Failed Frustum-OBox axis-based test2\n", TestName);
                printobb(t);
                printfr(unit);
                success = false;
            }
        }
    }

    {
        if (!revfr.Intersects(unit))
        {
            printe("%s: Frustum-OBox revfr test failed\n", TestName);
            printfr(revfr);
            success = false;
        }

        const BoundingOrientedBox _small(XMFLOAT3(0.1f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
        if (!revfr.Intersects(_small))
        {
            printe("%s: Frustum-OBox small test2 failed\n", TestName);
            printobb(_small);
            printfr(revfr);
            success = false;
        }

        const BoundingOrientedBox big(XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(10.f, 10.f, 10.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
        if (!revfr.Intersects(big))
        {
            printe("%s: Frustum-OBox big test2 failed\n", TestName);
            printobb(big);
            printfr(unit);
            success = false;
        }

        const BoundingOrientedBox _far(XMFLOAT3(10.f, -5.f, 4.f), XMFLOAT3(2.f, 2.f, 2.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
        if (revfr.Intersects(_far))
        {
            printe("%s: Frustum-OBox far test2 failed\n", TestName);
            printobb(_far);
            printfr(unit);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const BoundingOrientedBox t(XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

            const BoundingOrientedBox t2(XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3(1.f - EPSILON, 1.f - EPSILON, 1.f - EPSILON), XMFLOAT4(0.f, 0.f, 0.f, 1.f));
            if (revfr.Intersects(t2))
            {
                printe("%s: Failed Frustum-OBox axis-based test3\n", TestName);
                printobb(t2);
                printfr(revfr);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const BoundingOrientedBox t(XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f));

            if (!revfr.Intersects(t))
            {
                printe("%s: Failed Frustum-OBox axis-based test4\n", TestName);
                printobb(t);
                printfr(revfr);
                success = false;
            }
        }
    }

    // Frustum-Frustum
    {
        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f },
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = unit.Contains(fr);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed frustum-frustum test1 %zu\n", TestName, i);
                printfr(unit);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = unit.Contains(fr);
            if (ct != DISJOINT)
            {
                printe("%s: Failed frustum-frustum test2 %zu\n", TestName, i);
                printfr(unit);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }
    }

    {
        static const float test[][3] = { { 0, 0, -2.f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = revfr.Contains(fr);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed frustum-frustum test3 %zu\n", TestName, i);
                printfr(revfr);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = revfr.Contains(fr);
            if (ct != DISJOINT)
            {
                printe("%s: Failed frustum-frustum test4 %zu\n", TestName, i);
                printfr(revfr);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test3[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f },
                                         {  2.0f,-2.0f, 0.0f } };
        for (size_t i = 0; i < sizeof(test3) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test3[i][0], test3[i][1], test3[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);
            ContainmentType ct = revfr.Contains(fr);
            if (ct != DISJOINT)
            {
                printe("%s: Failed frustum-frustum test5 %zu\n", TestName, i);
                printfr(revfr);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }
    }

    // Frustum-Triangle
    {
        const XMVECTORF32 tri_in[3] =
        {
            { 0.5f, 0.5f, 0.5f, 0.f },
            { 1.0f, 0.f, 1.f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_out[3] =
        {
            { 10.f, 10.f, 10.f, 0.f },
            { 2.0f, 0.f, 2.f, 0.f },
            { 5.f, 5.f, 5.f, 0.f }
        };

        static_assert(sizeof(tri_in) == sizeof(tri_out), "TestF05 OBox-tri tests");

        for (uint32_t i = 0; i < (sizeof(tri_in) / sizeof(XMVECTORF32)); i += 3)
        {
            XMVECTOR t0 = tri_in[i].v;
            XMVECTOR t1 = tri_in[i + 1].v;
            XMVECTOR t2 = tri_in[i + 2].v;
            if (!unit.Intersects(t0, t1, t2))
            {
                printe("%s: Triangle-Frustum test failed (ins %d)\n", TestName, i);
                printfr(unit);
                printxmv(t0);
                printxmv(t1);
                printxmv(t2);
                success = false;
            }

            t0 = tri_out[i].v;
            t1 = tri_out[i + 1].v;
            t2 = tri_out[i + 2].v;
            if (unit.Intersects(t0, t1, t2))
            {
                printe("%s: Triangle-Frustum test failed (outs %d)\n", TestName, i);
                printfr(unit);
                printxmv(t0);
                printxmv(t1);
                printxmv(t2);
                success = false;
            }
        }
    }

    {
        const XMVECTORF32 tri_in[3] =
        {
            { 0.5f, 0.5f, 0.5f, 0.f },
            { 1.0f, 0.f, 1.f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_out[3] =
        {
            { 10.f, 10.f, 10.f, 0.f },
            { 2.0f, 0.f, 2.f, 0.f },
            { 5.f, 5.f, 5.f, 0.f }
        };

        static_assert(sizeof(tri_in) == sizeof(tri_out), "TestF05 OBox-tri tests");

        for (uint32_t i = 0; i < (sizeof(tri_in) / sizeof(XMVECTORF32)); i += 3)
        {
            XMVECTOR t0 = tri_in[i].v;
            XMVECTOR t1 = tri_in[i + 1].v;
            XMVECTOR t2 = tri_in[i + 2].v;
            if (!revfr.Intersects(t0, t1, t2))
            {
                printe("%s: Triangle-Frustum test2 failed (ins %d)\n", TestName, i);
                printfr(revfr);
                printxmv(t0);
                printxmv(t1);
                printxmv(t2);
                success = false;
            }

            t0 = tri_out[i].v;
            t1 = tri_out[i + 1].v;
            t2 = tri_out[i + 2].v;
            if (revfr.Intersects(t0, t1, t2))
            {
                printe("%s: Triangle-Frustum test2 failed (outs %d)\n", TestName, i);
                printfr(revfr);
                printxmv(t0);
                printxmv(t1);
                printxmv(t2);
                success = false;
            }
        }
    }

    // Frustum-Plane
    {
        const XMVECTORF32 planes[9] =
        {
            { 0.f, 1.f, 0.f, 2.f },
            { 0.f, 1.f, 0.f, -2.f },
            { 0.f, 1.f, 0.f, 0.f },
            { 0.577350f, 0.577350f, 0.577350f, 2.f },
            { 0.577350f, 0.577350f, 0.577350f, -2.f },
            { 0.577350f, 0.577350f, 0.577350f, 0.f },
            { -0.577350f, -0.577350f, -0.577350f, 2.f },
            { -0.577350f, -0.577350f, -0.577350f, -2.f },
            { -0.577350f, -0.577350f, -0.577350f, 0.f },
        };

        PlaneIntersectionType result[9] =
        {
            FRONT,
            BACK,
            INTERSECTING,
            FRONT,
            BACK,
            INTERSECTING,
            FRONT,
            BACK,
            INTERSECTING,
        };

        static_assert((sizeof(planes) / sizeof(XMVECTORF32)) == (sizeof(result) / sizeof(PlaneIntersectionType)), "TestF05 Frustum-Plane tests");

        for (uint32_t i = 0; i < (sizeof(planes) / sizeof(XMVECTORF32)); ++i)
        {
            PlaneIntersectionType p = unit.Intersects(planes[i]);

            if (p != result[i])
            {
                printe("%s: Plane-Frustum test failed ([%d] result %d, expected %d)\n", TestName, i, p, result[i]);
                printfr(unit);
                printxmv(planes[i]);
                success = false;
            }
        }
    }

    {
        const XMVECTORF32 planes[9] =
        {
            { 0.f, 1.f, 0.f, 2.f },
            { 0.f, 1.f, 0.f, -2.f },
            { 0.f, 1.f, 0.f, 0.f },
            { 0.577350f, 0.577350f, 0.577350f, 2.f },
            { 0.577350f, 0.577350f, 0.577350f, -2.f },
            { 0.577350f, 0.577350f, 0.577350f, 0.f },
            { -0.577350f, -0.577350f, -0.577350f, 2.f },
            { -0.577350f, -0.577350f, -0.577350f, -2.f },
            { -0.577350f, -0.577350f, -0.577350f, 0.f },
        };

        PlaneIntersectionType result[9] =
        {
            FRONT,
            BACK,
            INTERSECTING,
            FRONT,
            BACK,
            INTERSECTING,
            FRONT,
            BACK,
            INTERSECTING,
        };

        static_assert((sizeof(planes) / sizeof(XMVECTORF32)) == (sizeof(result) / sizeof(PlaneIntersectionType)), "TestF05 Frustum-Plane tests");

        for (uint32_t i = 0; i < (sizeof(planes) / sizeof(XMVECTORF32)); ++i)
        {
            PlaneIntersectionType p = revfr.Intersects(planes[i]);

            if (p != result[i])
            {
                printe("%s: Plane-Frustum test2 failed ([%d] result %d, expected %d)\n", TestName, i, p, result[i]);
                printfr(revfr);
                printxmv(planes[i]);
                success = false;
            }
        }
    }

    // Frustum-Ray
    {
        float dist;

        const XMVECTORF32 rayA[2] = { { 0.1f, 0.1f, 0.1f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if (!unit.Intersects(rayA[0], rayA[1], dist) || (fabs(dist) > EPSILON))
        {
            printe("%s: Frustum-Ray test A failed (dist=%f)\n", TestName, dist);
            printfr(unit);
            printxmv(rayA[0]);
            printxmv(rayA[1]);
            success = false;
        }

        const XMVECTORF32 rayB[2] = { { 10.f, 10.f, 10.f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if (unit.Intersects(rayB[0], rayB[1], dist)) // should miss frustum
        {
            printe("%s: Frustum-Ray test B failed (dist=%f)\n", TestName, dist);
            printfr(unit);
            printxmv(rayB[0]);
            printxmv(rayB[1]);
            success = false;
        }

        const XMVECTORF32 rayC[2] = { { 10.f, 10.f, 10.f, 0.f }, { -0.577350f, -0.577350f, -0.577350f, 0.f } };
        if (!unit.Intersects(rayC[0], rayC[1], dist) || (fabs(dist - 15.588465f) > EPSILON))
        {
            printe("%s: Frustum-Ray test C failed (dist=%f)\n", TestName, dist);
            printfr(unit);
            printxmv(rayC[0]);
            printxmv(rayC[1]);
            success = false;
        }

        const XMVECTORF32 rayD[2] = { { -0.1f, -0.1f, -0.1f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if (!unit.Intersects(rayD[0], rayD[1], dist) || (fabs(dist - 0.2f) > EPSILON))
        {
            printe("%s: Frustum-Ray test D failed (dist=%f)\n", TestName, dist);
            printfr(unit);
            printxmv(rayD[0]);
            printxmv(rayD[1]);
            success = false;
        }
    }

    {
        float dist;

        const XMVECTORF32 rayA[2] = { { 0.1f, 0.1f, 0.1f, 0.f }, { 0.f, 0.f, -1.f, 0.f } };
        if (!revfr.Intersects(rayA[0], rayA[1], dist) || (fabs(dist - 0.2f) > EPSILON))
        {
            printe("%s: Frustum-Ray test E failed (dist=%f)\n", TestName, dist);
            printfr(revfr);
            printxmv(rayA[0]);
            printxmv(rayA[1]);
            success = false;
        }

        const XMVECTORF32 rayB[2] = { { 10.f, 10.f, 10.f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if (revfr.Intersects(rayB[0], rayB[1], dist)) // should miss frustum
        {
            printe("%s: Frustum-Ray test F failed (dist=%f)\n", TestName, dist);
            printfr(revfr);
            printxmv(rayB[0]);
            printxmv(rayB[1]);
            success = false;
        }

        const XMVECTORF32 rayC[2] = { { 10.f, 10.f, 10.f, 0.f }, { -0.577350f, -0.577350f, -0.577350f, 0.f } };
        if (!revfr.Intersects(rayC[0], rayC[1], dist) || (fabs(dist - 17.320517f) > EPSILON))
        {
            printe("%s: Frustum-Ray test G failed (dist=%f)\n", TestName, dist);
            printfr(revfr);
            printxmv(rayC[0]);
            printxmv(rayC[1]);
            success = false;
        }

        const XMVECTORF32 rayD[2] = { { -0.1f, -0.1f, -0.1f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if (!revfr.Intersects(rayD[0], rayD[1], dist) || (fabs(dist) > EPSILON))
        {
            printe("%s: Frustum-Ray test H failed (dist=%f)\n", TestName, dist);
            printfr(revfr);
            printxmv(rayD[0]);
            printxmv(rayD[1]);
            success = false;
        }
    }

    return (success) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingFrustum::ContainedBy
HRESULT TestF06(LogProxy* pLog)
{
    bool success = true;

    const BoundingFrustum unit;

    {
        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f },
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes(&Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5);

            ContainmentType ct = unit.ContainedBy(Plane0, Plane1, Plane2, Plane3, Plane4, Plane5);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed frustum-6planes test1 %zu\n", TestName, i);
                printfr(unit);
                printxmv(Plane0);
                printxmv(Plane1);
                printxmv(Plane2);
                printxmv(Plane3);
                printxmv(Plane4);
                printxmv(Plane5);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes(&Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5);

            ContainmentType ct = unit.ContainedBy(Plane0, Plane1, Plane2, Plane3, Plane4, Plane5);
            if (ct != DISJOINT)
            {
                printe("%s: Failed frustum-6planes test2 %zu\n", TestName, i);
                printfr(unit);
                printxmv(Plane0);
                printxmv(Plane1);
                printxmv(Plane2);
                printxmv(Plane3);
                printxmv(Plane4);
                printxmv(Plane5);
                printct(ct);
                success = false;
            }
        }
    }

    return (success) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingFrustum::GetPlanes
HRESULT TestF07(LogProxy* pLog)
{
    bool success = true;

    {
        const BoundingFrustum fr;

        XMVECTOR Plane[6];
        fr.GetPlanes(&Plane[0], &Plane[1], &Plane[2], &Plane[3], &Plane[4], &Plane[5]);

        XMVECTORF32 Check[6] = { { 0, 0, -1.f, 0 },
                                 { 0, 0, 1.f, -1.f },
                                 { 0.707107f, 0, -0.707107f, 0 },
                                 { -0.707107f, 0, -0.707107f, 0 },
                                 { 0, 0.707107f, -0.707107f, 0 },
                                 { 0, -0.707107f, -0.707107f, 0 }
        };

        for (size_t i = 0; i < 6; ++i)
        {
            COMPARISON c = CompareXMVECTOR(Plane[i], Check[i], 4);
            if (c > WITHIN10EPSILON)
            {
                printe("%s: GetPlanes(1) failed plane %zu (%d)\n", TestName, i, c);
                printfr(fr);
                printxmv(Plane[i]);
                printxmv(Check[i]);
                success = false;
            }
        }
    }

    {
        const BoundingFrustum fr(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

        XMVECTOR Plane[6];
        fr.GetPlanes(&Plane[0], &Plane[1], &Plane[2], &Plane[3], &Plane[4], &Plane[5]);

        XMVECTORF32 Check[6] = { { 0, 0, -1.f, 2.f },
                                 { 0, 0, 1.f, -3.f },
                                 { 0.447214f, 0, -0.894427f, 0.447214f },
                                 { -0.447214f, 0, -0.894427f, 1.341641f },
                                 { 0, 0.447214f, -0.894427f, 0.447214f },
                                 { 0, -0.447214f, -0.894427f, 1.341641f }
        };

        for (size_t i = 0; i < 6; ++i)
        {
            COMPARISON c = CompareXMVECTOR(Plane[i], Check[i], 4);
            if (c > WITHIN10EPSILON)
            {
                printe("%s: GetPlanes(2) failed plane %zu (%d)\n", TestName, i, c);
                printfr(fr);
                printxmv(Plane[i]);
                printxmv(Check[i]);
                success = false;
            }
        }
    }

    return (success) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingFrustum::CreateFromMatrix
HRESULT TestF08(LogProxy* pLog)
{
    bool success = true;

    {
        XMMATRIX matrix = XMMatrixPerspectiveFovRH(XM_PIDIV2 /* 90 degrees */, 1.f, 1.f, 100.f);

        BoundingFrustum fr;
        BoundingFrustum::CreateFromMatrix(fr, matrix);

        if (fabs(fr.Origin.x) > EPSILON
            || fabs(fr.Origin.y) > EPSILON
            || fabs(fr.Origin.z) > EPSILON
            || fabs(fr.Orientation.x) > EPSILON
            || fabs(fr.Orientation.y) > EPSILON
            || fabs(fr.Orientation.z) > EPSILON
            || fabs(fr.Orientation.w - 1.f) > EPSILON
            || fabs(fr.RightSlope + 1.0f) > EPSILON
            || fabs(fr.LeftSlope - 1.0f) > EPSILON
            || fabs(fr.TopSlope + 1.0f) > EPSILON
            || fabs(fr.BottomSlope - 1.0f) > EPSILON
            || fabs(fr.Near + 1.f) > EPSILON
            || fabs(fr.Far + 100.000694f) > EPSILON)
        {
            printe("%s: CreateFromMatrix failed\n", TestName);
            printfr(fr);
            success = false;
        }
    }

    return (success) ? S_OK : MATH_FAIL;
}
