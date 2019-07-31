//-------------------------------------------------------------------------------------
// obox.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

static const float EPSILON = 0.0001f;

inline bool IsEqual( const BoundingOrientedBox& b1, const BoundingOrientedBox& b2 )
{
    return ( ( fabs( b1.Center.x - b2.Center.x ) < EPSILON )
             && ( fabs( b1.Center.y - b2.Center.y ) < EPSILON )
             && ( fabs( b1.Center.z - b2.Center.z ) < EPSILON )
             && ( fabs( b1.Extents.x - b2.Extents.x ) < EPSILON )
             && ( fabs( b1.Extents.y - b2.Extents.y ) < EPSILON )
             && ( fabs( b1.Extents.z - b2.Extents.z ) < EPSILON )
             && ( fabs( b1.Orientation.x - b2.Orientation.x ) < EPSILON )
             && ( fabs( b1.Orientation.y - b2.Orientation.y ) < EPSILON )
             && ( fabs( b1.Orientation.z - b2.Orientation.z ) < EPSILON )
             && ( fabs( b1.Orientation.w - b2.Orientation.w ) < EPSILON ) ) ? true : false;
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
// BoundingOrientedBox
HRESULT TestO01(LogProxy* pLog)
{
    bool success = true;

    static_assert(std::is_nothrow_copy_assignable<BoundingOrientedBox>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<BoundingOrientedBox>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<BoundingOrientedBox>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<BoundingOrientedBox>::value, "Move Assign.");

    // Default constructor
    BoundingOrientedBox bb;
    if ( fabs( bb.Center.x ) > EPSILON
         || fabs( bb.Center.y ) > EPSILON
         || fabs( bb.Center.z ) > EPSILON
         || fabs( bb.Extents.x - 1.0f ) > EPSILON
         || fabs( bb.Extents.y - 1.0f ) > EPSILON
         || fabs( bb.Extents.z - 1.0f ) > EPSILON )
    {
        printe("%s: Default constructor didn't create unit oriented box\n",TestName);
        printobb( bb );
        success = false;
    }

    // Center + Extents + Orientation constructor
    BoundingOrientedBox bbp( XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
    if ( fabs( bbp.Center.x - 1.f) > EPSILON
         || fabs( bbp.Center.y - 1.f) > EPSILON
         || fabs( bbp.Center.z - 1.f) > EPSILON
         || fabs( bbp.Extents.x - 2.0f ) > EPSILON
         || fabs( bbp.Extents.y - 2.0f ) > EPSILON
         || fabs( bbp.Extents.z - 2.0f ) > EPSILON
         || fabs( bbp.Orientation.x ) > EPSILON
         || fabs( bbp.Orientation.y ) > EPSILON
         || fabs( bbp.Orientation.z ) > EPSILON
         || fabs( bbp.Orientation.w - 1.f ) > EPSILON )
    {
        printe("%s: Constructor didn't create correct box\n",TestName);
        printobb( bbp );
        success = false;
    }

    XMFLOAT4 orient;
    XMStoreFloat4( &orient, XMQuaternionRotationRollPitchYaw( XM_PIDIV4, XM_PIDIV4, XM_PIDIV4 ) );
    BoundingOrientedBox bbu( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), orient );
    if ( fabs( bbu.Center.x ) > EPSILON
         || fabs( bbu.Center.y ) > EPSILON
         || fabs( bbu.Center.z ) > EPSILON
         || fabs( bbu.Extents.x - 1.0f ) > EPSILON
         || fabs( bbu.Extents.y - 1.0f ) > EPSILON
         || fabs( bbu.Extents.z - 1.0f ) > EPSILON
         || fabs( bbu.Orientation.x - 0.461940f ) > EPSILON
         || fabs( bbu.Orientation.y - 0.191342f ) > EPSILON
         || fabs( bbu.Orientation.z - 0.191342f) > EPSILON
         || fabs( bbu.Orientation.w - 0.844623f ) > EPSILON )
    {
        printe("%s: Constructor didn't create correct box (2)\n",TestName);
        printobb( bbu );
        success = false;
    }

    // Copy constructor
    BoundingOrientedBox bbc(bbp);
    if ( !IsEqual(bbc,bbp) )
    {
        printe("%s: Copy constructor failed\n",TestName);
        printobb( bbc );
        printobb( bbp );
        success = false;
    }

    // Test assignment operator
    BoundingOrientedBox bbi;
    bbi = bbp;
    if ( !IsEqual(bbi,bbp) )
    {
        printe("%s: Assignment operator failed\n",TestName);
        printobb( bbi );
        printobb( bbp );
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingOrientedBox::Transform
HRESULT TestO02(LogProxy* pLog)
{
    bool success = true;

    BoundingOrientedBox ubox, vbox;

    XMMATRIX Mid = XMMatrixIdentity();
    XMVECTOR Rid = XMQuaternionIdentity();
    XMVECTOR Tid = XMVectorZero();

    XMMATRIX mat = XMMatrixTransformation( g_XMZero, // Scaling origin
                                           Rid, // Scaling orientation
                                           XMVectorSet( 1.f, 1.f, 1.f, 0.f ), // Scaling
                                           g_XMZero, // Rotation origin
                                           Rid, // Rotation
                                           g_XMOne // Translation
                                           );

    XMMATRIX matScale = XMMatrixTransformation( g_XMZero, // Scaling origin
                                               Rid, // Scaling orientation
                                               XMVectorSet( 3.f, 3.f, 3.f, 0.f ), // Scaling
                                               g_XMZero, // Rotation origin
                                               Rid, // Rotation
                                               g_XMOne // Translation
                                               );

    XMMATRIX matNonUniform = XMMatrixTransformation( g_XMZero, // Scaling origin
                                                     Rid, // Scaling orientation
                                                     XMVectorSet( 1.f, 2.f, 3.f, 0.f ), // Non-uniform scaling
                                                     g_XMZero, // Rotation origin
                                                     Rid, // Rotation
                                                     g_XMOne // Translation
                                                    );

    // Unit box 
    const BoundingOrientedBox unit;

    unit.Transform( ubox, Mid );
    if ( !IsEqual(unit,ubox) )
    {
        printe("%s: Unit obox tranform(1) failed\n",TestName);
        printobb(unit);
        printobb(ubox);
        success = false;
    }

    unit.Transform( vbox, 1.0f, Rid, Tid );
    if ( !IsEqual(unit,vbox) )
    {
        printe("%s: Unit obox tranform(2) failed\n",TestName);
        printobb(unit);
        printobb(vbox);
        success = false;
    }

    // Small box (no rotation, no orientation)
    const BoundingOrientedBox _small( XMFLOAT3(0.1f, 0.0f, 0.0f), XMFLOAT3(0.5f,0.5f,0.5f), XMFLOAT4(0.f, 0.f, 0.f, 1.f) );

    _small.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 1.1f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 1.0f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 0.5f) > EPSILON
         || fabs( ubox.Extents.z - 0.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 1.f) > EPSILON )
    {
        printe("%s: Small transform(1) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    _small.Transform( vbox, 1.0f, Rid, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(2) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (no rotation, no orientation w/ scale)
    _small.Transform( ubox, matScale );
    if ( fabs( ubox.Center.x - 1.3f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 1.0f) > EPSILON
         || fabs( ubox.Extents.x - 1.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.5f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 1.f) > EPSILON )
    {
        printe("%s: Small transform(3) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    _small.Transform( vbox, 3.0f, Rid, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(4) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (no rotation, no orientation w/ non-uniform scale)
    _small.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 1.1f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 1.0f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.0f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 1.f) > EPSILON )
    {
        printe("%s: Small transform(5) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(6) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (no rotation, orientation)
    const BoundingOrientedBox _small2( XMFLOAT3(0.1f, 0.0f, 0.0f), XMFLOAT3(0.5f,0.5f,0.5f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );

    _small2.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 1.1f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 1.0f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 0.5f) > EPSILON
         || fabs( ubox.Extents.z - 0.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.z - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.844623f) > EPSILON )
    {
        printe("%s: Small transform(7) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    _small2.Transform( vbox, 1.0f, Rid, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(8) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (no rotation, orientation w/ scale)
    _small2.Transform( ubox, matScale );
    if ( fabs( ubox.Center.x - 1.3f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 1.0f) > EPSILON
         || fabs( ubox.Extents.x - 1.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.5f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.z - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.844623f) > EPSILON )
    {
        printe("%s: Small transform(9) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    _small2.Transform( vbox, 3.0f, Rid, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(10) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (no rotation, orientation w/ non-uniform scale)
    _small2.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 1.1f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 1.0f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.0f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.z - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.844623f) > EPSILON )
    {
        printe("%s: Small transform(11) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(12) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no rotation, no orientation)
    const BoundingOrientedBox big( XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3( 10.f, 10.f, 10.f ), XMFLOAT4(0.f, 0.f, 0.f, 1.f) );

    big.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 2.f) > EPSILON
         || fabs( ubox.Center.y - 3.f) > EPSILON
         || fabs( ubox.Center.z - 4.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 10.f) > EPSILON
         || fabs( ubox.Extents.z - 10.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 1.f) > EPSILON )
    {
        printe("%s: Big transform(1) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    big.Transform( vbox, 1.0f, Rid, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(2) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no rotation, no orientation w/ scale)
    big.Transform( ubox, matScale );
    if ( fabs( ubox.Center.x - 4.f) > EPSILON
         || fabs( ubox.Center.y - 7.f) > EPSILON
         || fabs( ubox.Center.z - 10.f) > EPSILON
         || fabs( ubox.Extents.x - 30.f) > EPSILON
         || fabs( ubox.Extents.y - 30.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 1.f) > EPSILON )
    {
        printe("%s: Big transform(3) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    big.Transform( vbox, 3.0f, Rid, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(4) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no rotation, no orientation w/ non-uniform scale)
    big.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 2.f) > EPSILON
         || fabs( ubox.Center.y - 5.f) > EPSILON
         || fabs( ubox.Center.z - 10.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 20.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 1.f) > EPSILON )
    {
        printe("%s: Big transform(5) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(6) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no rotation, orientation)
    const BoundingOrientedBox big2( XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3( 10.f, 10.f, 10.f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );

    big2.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 2.f) > EPSILON
         || fabs( ubox.Center.y - 3.f) > EPSILON
         || fabs( ubox.Center.z - 4.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 10.f) > EPSILON
         || fabs( ubox.Extents.z - 10.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.z - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.844623f) > EPSILON )
    {
        printe("%s: Big transform(7) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    big2.Transform( vbox, 1.0f, Rid, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(8) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no rotation, orientation w/ scale)
    big2.Transform( ubox, matScale );
    if ( fabs( ubox.Center.x - 4.f) > EPSILON
         || fabs( ubox.Center.y - 7.f) > EPSILON
         || fabs( ubox.Center.z - 10.f) > EPSILON
         || fabs( ubox.Extents.x - 30.f) > EPSILON
         || fabs( ubox.Extents.y - 30.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.z - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.844623f) > EPSILON )
    {
        printe("%s: Big transform(9) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    big2.Transform( vbox, 3.0f, Rid, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(10) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no rotation, orientation w/ non-uniform scale)
    big2.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 2.f) > EPSILON
         || fabs( ubox.Center.y - 5.f) > EPSILON
         || fabs( ubox.Center.z - 10.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 20.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.z - 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.844623f) > EPSILON )
    {
        printe("%s: Big transform(11) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(12) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    XMVECTOR rot = XMQuaternionRotationRollPitchYaw( 0.f, XM_PIDIV2, 0.f );

    mat = XMMatrixTransformation( g_XMZero, // Scaling origin
                                  Rid, // Scaling orientation
                                  XMVectorSet( 1.f, 1.f, 1.f, 0.f ), // Scaling
                                  g_XMZero, // Rotation origin
                                  rot, // Rotation
                                  g_XMOne // Translation
                                );

    matScale = XMMatrixTransformation( g_XMZero, // Scaling origin
                                       Rid, // Scaling orientation
                                       XMVectorSet( 3.f, 3.f, 3.f, 0.f ), // Scaling
                                       g_XMZero, // Rotation origin
                                       rot, // Rotation
                                       g_XMOne // Translation
                                     );

    matNonUniform = XMMatrixTransformation( g_XMZero, // Scaling origin
                                            Rid, // Scaling orientation
                                            XMVectorSet( 1.f, 2.f, 3.f, 0.f ), // Non-uniform scaling
                                            g_XMZero, // Rotation origin
                                            rot, // Rotation
                                            g_XMOne // Translation
                                          );
 
    // Small box (no orientation)
    _small.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 1.f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 0.9f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 0.5f) > EPSILON
         || fabs( ubox.Extents.z - 0.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.707107f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 0.707107f) > EPSILON )
    {
        printe("%s: Small transform(13) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    _small.Transform( vbox, 1.0f, rot, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(14) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (no orientation w/ scale)
    _small.Transform( ubox, matScale );
    if ( fabs( ubox.Center.x - 1.f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 0.7f) > EPSILON
         || fabs( ubox.Extents.x - 1.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.5f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.707107f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 0.707107f) > EPSILON )
    {
        printe("%s: Small transform(15) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    _small.Transform( vbox, 3.0f, rot, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(16) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (no orientation w/ non-uniform scale)
    _small.Transform(ubox, matNonUniform);
    if ( fabs( ubox.Center.x - 1.f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 0.9f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.0f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.707107f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 0.707107f) > EPSILON )
    {
        printe("%s: Small transform(17) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Small transform(18) failed\n",TestName);
        printobb(_small);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (w/ orientation)
    _small2.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 1.f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 0.9f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 0.5f) > EPSILON
         || fabs( ubox.Extents.z - 0.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.732538f) > EPSILON
         || fabs( ubox.Orientation.z + 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.461939f) > EPSILON )
    {
        printe("%s: Small transform(19) failed\n",TestName);
        printobb(_small2);
        printobb(ubox);
        success = false;
    }

    _small2.Transform( vbox, 1.0f, rot, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(20) failed\n",TestName);
        printobb(_small2);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (w/ orientation and scale)
    _small2.Transform( ubox, matScale );
    if ( fabs( ubox.Center.x - 1.f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 0.7f) > EPSILON
         || fabs( ubox.Extents.x - 1.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.5f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.732538f) > EPSILON
         || fabs( ubox.Orientation.z + 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.461939f) > EPSILON )
    {
        printe("%s: Small transform(21) failed\n",TestName);
        printobb(_small2);
        printobb(ubox);
        success = false;
    }

    _small2.Transform( vbox, 3.0f, rot, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(22) failed\n",TestName);
        printobb(_small2);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Small box (w/ orientation and non-uniform scale)
    _small2.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 1.f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 0.9f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.0f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.732538f) > EPSILON
         || fabs( ubox.Orientation.z + 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.461939f) > EPSILON )
    {
        printe("%s: Small transform(23) failed\n",TestName);
        printobb(_small2);
        printobb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Small transform(24) failed\n",TestName);
        printobb(_small2);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no orientation)
    big.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 4.f) > EPSILON
         || fabs( ubox.Center.y - 3.f) > EPSILON
         || fabs( ubox.Center.z - 0.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 10.f) > EPSILON
         || fabs( ubox.Extents.z - 10.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.707107f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 0.707107f) > EPSILON )
    {
        printe("%s: Big transform(13) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    big.Transform( vbox, 1.0f, rot, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(14) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no orientation w/ scale)
    big.Transform( ubox, matScale );
    if ( fabs( ubox.Center.x - 10.f) > EPSILON
         || fabs( ubox.Center.y - 7.f) > EPSILON
         || fabs( ubox.Center.z + 2.f) > EPSILON
         || fabs( ubox.Extents.x - 30.f) > EPSILON
         || fabs( ubox.Extents.y - 30.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.707107f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 0.707107f) > EPSILON )
    {
        printe("%s: Big transform(15) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    big.Transform( vbox, 3.0f, rot, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(16) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (no orientation w/ non-uniform scale)
    big.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 10.f) > EPSILON
         || fabs( ubox.Center.y - 5.f) > EPSILON
         || fabs( ubox.Center.z + 0.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 20.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.f) > EPSILON
         || fabs( ubox.Orientation.y - 0.707107f) > EPSILON
         || fabs( ubox.Orientation.z - 0.f) > EPSILON
         || fabs( ubox.Orientation.w - 0.707107f) > EPSILON )
    {
        printe("%s: Big transform(17) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Big transform(18) failed\n",TestName);
        printobb(big);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (w/ orientation)
    big2.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 4.f) > EPSILON
         || fabs( ubox.Center.y - 3.f) > EPSILON
         || fabs( ubox.Center.z - 0.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 10.f) > EPSILON
         || fabs( ubox.Extents.z - 10.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.732538f) > EPSILON
         || fabs( ubox.Orientation.z + 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.461939f) > EPSILON )
    {
        printe("%s: Big transform(19) failed\n",TestName);
        printobb(big2);
        printobb(ubox);
        success = false;
    }

    big2.Transform( vbox, 1.0f, rot, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(20) failed\n",TestName);
        printobb(big2);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (w/ orientation and scale)
    big2.Transform( ubox, matScale );
    if ( fabs( ubox.Center.x - 10.f) > EPSILON
         || fabs( ubox.Center.y - 7.f) > EPSILON
         || fabs( ubox.Center.z + 2.f) > EPSILON
         || fabs( ubox.Extents.x - 30.f) > EPSILON
         || fabs( ubox.Extents.y - 30.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.732538f) > EPSILON
         || fabs( ubox.Orientation.z + 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.461939f) > EPSILON )
    {
        printe("%s: Big transform(21) failed\n",TestName);
        printobb(big2);
        printobb(ubox);
        success = false;
    }

    big2.Transform( vbox, 3.0f, rot, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(22) failed\n",TestName);
        printobb(big2);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    // Big box (w/ orientation and non-uniform scale)
    big2.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 10.f) > EPSILON
         || fabs( ubox.Center.y - 5.f) > EPSILON
         || fabs( ubox.Center.z + 0.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 20.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON
         || fabs( ubox.Orientation.x - 0.461940f) > EPSILON
         || fabs( ubox.Orientation.y - 0.732538f) > EPSILON
         || fabs( ubox.Orientation.z + 0.191342f) > EPSILON
         || fabs( ubox.Orientation.w - 0.461939f) > EPSILON )
    {
        printe("%s: Big transform(23) failed\n",TestName);
        printobb(big2);
        printobb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Big transform(24) failed\n",TestName);
        printobb(big2);
        printobb(ubox);
        printobb(vbox);
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingOrientedBox::GetCorners
HRESULT TestO03(LogProxy* pLog)
{
    bool success = true;

    static_assert( BoundingOrientedBox::CORNER_COUNT == 8, "TestO03 expects there are 8 corners to a box" );

    {
        BoundingBox box1;
        BoundingBox::CreateFromPoints( box1, XMVectorSet( 1.0f, 2.0f, 3.0f, 0 ), XMVectorSet( 4.0f, 5.0f, 6.0f, 0 ) );
        
        BoundingOrientedBox box;
        BoundingOrientedBox::CreateFromBoundingBox( box, box1 );

        XMFLOAT3 corners[BoundingOrientedBox::CORNER_COUNT];
        box.GetCorners( corners );

        static const float expected[][3] = { { 1.f, 2.f, 6.f }, { 4.f, 2.f, 6.f }, { 4.f, 5.f, 6.f }, { 1.f, 5.f, 6.f },
                                             { 1.f, 2.f, 3.f }, { 4.f, 2.f, 3.f }, { 4.f, 5.f, 3.f }, { 1.f, 5.f, 3.f } };

        for( size_t i = 0; i < BoundingOrientedBox::CORNER_COUNT; ++i )
        {
            if ( fabs( corners[i].x - expected[i][0] ) > EPSILON
                 || fabs( corners[i].y - expected[i][1] ) > EPSILON
                 || fabs( corners[i].z - expected[i][2] ) > EPSILON )
            {
                printe( "%s: GetCorners1 failed: ", TestName );
                printe( "%f %f %f ... %f %f %f \n", corners[i].x, corners[i].y, corners[i].z, expected[i][0], expected[i][1], expected[i][2] );
                success = false;
            }
        }
    }

    {
        const BoundingOrientedBox box ( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );

        XMFLOAT3 corners[BoundingOrientedBox::CORNER_COUNT];
        box.GetCorners( corners );
            
        static const float expected[][3] = { {-0.207106f,-1.707107f,-0.207108f}, {1.500001f,-0.707106f,-0.500001f}, {1.207107f,0.292893f,1.207107f}, {-0.499999f,-0.707108f,1.500000f},
                                             {-1.207107f,-0.292893f,-1.207107f}, {0.499999f,0.707108f,-1.500000f}, {0.207106f,1.707107f,0.207108f}, {-1.500001f,0.707106f,0.500001f} };

        for( size_t i = 0; i < BoundingOrientedBox::CORNER_COUNT; ++i )
        {
            if ( fabs( corners[i].x - expected[i][0] ) > EPSILON
                 || fabs( corners[i].y - expected[i][1] ) > EPSILON
                 || fabs( corners[i].z - expected[i][2] ) > EPSILON )
            {
                printe( "%s: GetCorners2 failed: ", TestName );
                printe( "%f %f %f ... %f %f %f \n", corners[i].x, corners[i].y, corners[i].z, expected[i][0], expected[i][1], expected[i][2] );
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingOrientedBox::Contains
HRESULT TestO04(LogProxy* pLog)
{
    bool success = true;

    ContainmentType c;

    const BoundingOrientedBox unit;
    const BoundingOrientedBox rotbox( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );

    // OBox-Point
    {
        const XMVECTORF32 pnts_in[7] =
        {
            { 0.f, 0.f, 0.f, _Q_NAN },
            { 1.f, 0.f, 0.f, _Q_NAN },
            { 0.f, 1.f, 0.f, _Q_NAN },
            { 0.f, 0.f, 1.f, _Q_NAN },
            { 0.5f, 0.5f, 0.5f, _Q_NAN },
            { -0.5f, -0.5f, -0.5f, _Q_NAN },
            { 1.f, 1.f, 1.f, _Q_NAN },
        };

        const XMVECTORF32 pnts_out[7] = 
        {
            { 1.1f, 1.1f, 1.1f, _Q_NAN },
            { 1.1f, 0.f, 0.f, _Q_NAN },
            { 10.f, -10.f, -15.f, _Q_NAN },
            { 0, -1.1f, 0.f, _Q_NAN },
            { -20.f, -20.f, -20.f, _Q_NAN },
            { 1.f, 2.f, 3.f, _Q_NAN },
            { 10.f, 10.f, 10.f, _Q_NAN }
        };

        static_assert( sizeof(pnts_in) == sizeof(pnts_out), "TestO04 Box-Point test" );

        for( uint32_t i=0; i < (sizeof(pnts_in)/sizeof(XMVECTORF32)); ++i)
        {
            if ( (c=unit.Contains( pnts_in[i].v )) != CONTAINS )
            {
                printe("%s: Point-OBox test failed (ins %d)\n",TestName, i);
                printobb(unit);
                printxmv( pnts_in[i].v );
                printct( c );
                success = false;
            }

            if ( (c=unit.Contains( pnts_out[i].v )) != DISJOINT )
            {
                printe("%s: Point-OBox test failed (outs %d)\n",TestName, i);
                printobb(unit);
                printxmv( pnts_out[i].v );
                printct( c );
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
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            XMVECTOR point = XMVectorSet(test[i][0], test[i][1], test[i][2], 0);
            ContainmentType ct = unit.Contains( point );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed Point-OBox test1 %zu\n",TestName, i );
                printobb( unit );
                printxmv( point );
                printct( ct );
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
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            XMVECTOR point = XMVectorSet(test2[i][0], test2[i][1], test2[i][2], 0);
            ContainmentType ct = unit.Contains( point );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed Point-OBox test2 %zu\n",TestName, i );
                printobb( unit );
                printxmv( point );
                printct( ct );
                success = false;
            }
        }
    }

    {
        const XMVECTORF32 pnts_in[8] =
        {
            { 0.f, 0.f, 0.f, _Q_NAN },
            { 1.f, 0.f, 0.f, _Q_NAN },
            { 0.f, 1.f, 0.f, _Q_NAN },
            { 0.f, 0.f, 1.f, _Q_NAN },
            { 0.5f, 0.5f, 0.5f, _Q_NAN },
            { -0.5f, -0.5f, -0.5f, _Q_NAN },
            { 1.1f, 0.f, 0.f, _Q_NAN },
            { 0, -1.1f, 0.f, _Q_NAN },
        };

        const XMVECTORF32 pnts_out[8] = 
        {
            { 1.1f, 1.1f, 1.1f, _Q_NAN },
            { 10.f, -10.f, -15.f, _Q_NAN },
            { -20.f, -20.f, -20.f, _Q_NAN },
            { 1.f, 2.f, 3.f, _Q_NAN },
            { 10.f, 10.f, 10.f, _Q_NAN },
            { 1.f, 1.f, 1.f, _Q_NAN },
            { 1.f, 1.f, 1.f, _Q_NAN }, 
            { -1.f, -1.f, -1.f, _Q_NAN },
        };

        static_assert( sizeof(pnts_in) == sizeof(pnts_out), "TestO04 Point-Obox test2" );

        for( uint32_t i=0; i < (sizeof(pnts_in)/sizeof(XMVECTORF32)); ++i)
        {
            if ( (c=rotbox.Contains( pnts_in[i].v )) != CONTAINS )
            {
                printe("%s: Point-OBox test(2) failed (ins %d)\n",TestName, i);
                printobb(rotbox);
                printxmv( pnts_in[i].v );
                printct( c );
                success = false;
            }

            if ( (c=rotbox.Contains( pnts_out[i].v )) != DISJOINT )
            {
                printe("%s: Point-OBox test(2) failed (outs %d)\n",TestName, i);
                printobb(rotbox);
                printxmv( pnts_out[i].v );
                printct( c );
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
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            XMVECTOR point = XMVectorSet(test[i][0], test[i][1], test[i][2], 0);
            ContainmentType ct = rotbox.Contains( point );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed Point-OBox test3 %zu\n",TestName, i );
                printobb( rotbox );
                printxmv( point );
                printct( ct );
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
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            XMVECTOR point = XMVectorSet(test2[i][0], test2[i][1], test2[i][2], 0);
            ContainmentType ct = rotbox.Contains( point );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed Point-OBox test4 %zu\n",TestName, i );
                printobb( rotbox );
                printxmv( point );
                printct( ct );
                success = false;
            }
        }
    }

    // Box-Triangle
    {
        const XMVECTORF32 tri_CONTAINS[3] =
        { 
            { 0.2f, 0.2f, 0.2f, 0.f },
            { 0.5f, 0.f, 0.5f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_INTERSECTS[3] =
        { 
            { 0.5f, 0.5f, 0.5f, 0.f },
            { 2.0f, 0.f, 1.f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_DISJOINT[3] =
        { 
            { 10.f, 10.f, 10.f, 0.f },
            { 2.0f, 0.f, 2.f, 0.f },
            { 5.f, 5.f, 5.f, 0.f }
        };

        static_assert( (sizeof(tri_CONTAINS) == sizeof(tri_INTERSECTS)) && (sizeof(tri_CONTAINS) == sizeof(tri_DISJOINT)), "TestB04 box-tri tests" );

        for( uint32_t i=0; i < (sizeof(tri_CONTAINS)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_CONTAINS[i].v;
            XMVECTOR t1 = tri_CONTAINS[i+1].v;
            XMVECTOR t2 = tri_CONTAINS[i+2].v;
            c = unit.Contains(t0, t1, t2);
            if ( c != CONTAINS )
            {
                printe("%s: Triangle-OBox test failed (CONTAINS %d)\n",TestName, i);
                printct( c );
                printobb(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_INTERSECTS[i].v;
            t1 = tri_INTERSECTS[i+1].v;
            t2 = tri_INTERSECTS[i+2].v;
            c = unit.Contains(t0, t1, t2);
            if ( c != INTERSECTS )
            {
                printe("%s: Triangle-OBox test failed (INTERSECTS %d)\n",TestName, i);
                printct( c );
                printobb(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_DISJOINT[i].v;
            t1 = tri_DISJOINT[i+1].v;
            t2 = tri_DISJOINT[i+2].v;
            c = unit.Contains(t0, t1, t2);
            if ( c != DISJOINT )
            {
                printe("%s: Triangle-OBox test failed (DISJOINT %d)\n",TestName, i);
                printct( c );
                printobb(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    {
        const XMVECTORF32 tri_CONTAINS[3] =
        { 
            { 0.2f, 0.2f, 0.2f, 0.f },
            { 0.5f, 0.f, 0.5f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

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

        static_assert( (sizeof(tri_CONTAINS) == sizeof(tri_INTERSECTS)) && (sizeof(tri_CONTAINS) == sizeof(tri_DISJOINT)), "TestB04 box-tri test2" );

        for( uint32_t i=0; i < (sizeof(tri_CONTAINS)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_CONTAINS[i].v;
            XMVECTOR t1 = tri_CONTAINS[i+1].v;
            XMVECTOR t2 = tri_CONTAINS[i+2].v;
            c = rotbox.Contains(t0, t1, t2);
            if ( c != CONTAINS )
            {
                printe("%s: Triangle-OBox test2 failed (CONTAINS %d)\n",TestName, i);
                printct( c );
                printobb(rotbox);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_INTERSECTS[i].v;
            t1 = tri_INTERSECTS[i+1].v;
            t2 = tri_INTERSECTS[i+2].v;
            c = rotbox.Contains(t0, t1, t2);
            if ( c != INTERSECTS )
            {
                printe("%s: Triangle-OBox test2 failed (INTERSECTS %d)\n",TestName, i);
                printct( c );
                printobb(rotbox);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_DISJOINT[i].v;
            t1 = tri_DISJOINT[i+1].v;
            t2 = tri_DISJOINT[i+2].v;
            c = rotbox.Contains(t0, t1, t2);
            if ( c != DISJOINT )
            {
                printe("%s: Triangle-OBox test2 failed (DISJOINT %d)\n",TestName, i);
                printct( c );
                printobb(rotbox);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    {
        const BoundingOrientedBox box( XMFLOAT3( 2.f, 3.f, 4.f ), XMFLOAT3( 4.f, 5.f, 6.f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );

        const XMVECTORF32 tri_CONTAINS[3] =
        { 
            { 2.f, 3.f, 4.f, 0.f },
            { 5.f, 7.f, 9.f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_INTERSECTS[3] =
        { 
            { 2.f, 3.f, 4.f, 0.f },
            { 12.f, 8.f, 10.f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_DISJOINT[3] =
        { 
            { 15.f, 10.f, 10.f, 0.f },
            { 7.0f, 0.f, 2.f, 0.f },
            { 10.f, 5.f, 5.f, 0.f }
        };

        static_assert( (sizeof(tri_CONTAINS) == sizeof(tri_INTERSECTS)) && (sizeof(tri_CONTAINS) == sizeof(tri_DISJOINT)), "TestB04 box-tri tests" );

        for( uint32_t i=0; i < (sizeof(tri_CONTAINS)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_CONTAINS[i].v;
            XMVECTOR t1 = tri_CONTAINS[i+1].v;
            XMVECTOR t2 = tri_CONTAINS[i+2].v;

            c = box.Contains(t0, t1, t2);
            if ( c != CONTAINS )
            {
                printe("%s: Triangle-OBox test3 failed (CONTAINS %d)\n",TestName, i);
                printct( c );
                printobb(box);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_INTERSECTS[i].v;
            t1 = tri_INTERSECTS[i+1].v;
            t2 = tri_INTERSECTS[i+2].v;

            c = box.Contains(t0, t1, t2);
            if ( c != INTERSECTS )
            {
                printe("%s: Triangle-OBox test3 failed (INTERSECTS %d)\n",TestName, i);
                printct( c );
                printobb(box);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_DISJOINT[i].v;
            t1 = tri_DISJOINT[i+1].v;
            t2 = tri_DISJOINT[i+2].v;

            c = box.Contains(t0, t1, t2);
            if ( c != DISJOINT )
            {
                printe("%s: Triangle-OBox test3 failed (DISJOINT %d)\n",TestName, i);
                printct( c );
                printobb(box);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    {
        const BoundingOrientedBox box( XMFLOAT3( 2.f, 3.f, 4.f ), XMFLOAT3( 4.f, 5.f, 6.f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );

        const XMVECTORF32 tri_CONTAINS[3] =
        { 
            { 2.f, 3.f, 4.f, 0.f },
            { 5.f, 5.f, 9.f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_INTERSECTS[3] =
        { 
            { 2.f, 3.f, 4.f, 0.f },
            { 12.f, 8.f, 10.f, 0.f },
            { 0.f, 0.f, 0.f, 0.f }
        };

        const XMVECTORF32 tri_DISJOINT[3] =
        { 
            { 15.f, 10.f, 10.f, 0.f },
            { 10.1f, 0.f, 2.f, 0.f },//
            { 10.f, 5.f, 5.f, 0.f }
        };

        static_assert( (sizeof(tri_CONTAINS) == sizeof(tri_INTERSECTS)) && (sizeof(tri_CONTAINS) == sizeof(tri_DISJOINT)), "TestB04 box-tri tests" );

        for( uint32_t i=0; i < (sizeof(tri_CONTAINS)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_CONTAINS[i].v;
            XMVECTOR t1 = tri_CONTAINS[i+1].v;
            XMVECTOR t2 = tri_CONTAINS[i+2].v;
            c = box.Contains(t0, t1, t2);
            if ( c != CONTAINS )
            {
                printe("%s: Triangle-OBox test4 failed (CONTAINS %d)\n",TestName, i);
                printct( c );
                printobb(box);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_INTERSECTS[i].v;
            t1 = tri_INTERSECTS[i+1].v;
            t2 = tri_INTERSECTS[i+2].v;
            c = box.Contains(t0, t1, t2);
            if ( c != INTERSECTS )
            {
                printe("%s: Triangle-OBox test4 failed (INTERSECTS %d)\n",TestName, i);
                printct( c );
                printobb(box);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_DISJOINT[i].v;
            t1 = tri_DISJOINT[i+1].v;
            t2 = tri_DISJOINT[i+2].v;
            c = box.Contains(t0, t1, t2);
            if ( c != DISJOINT )
            {
                printe("%s: Triangle-OBox test4 failed (DISJOINT %d)\n",TestName, i);
                printct( c );
                printobb(box);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    // Box-Sphere
    {
        BoundingSphere sunit;
        if ( (c=unit.Contains(sunit)) != CONTAINS )
        {
            printe("%s: Sphere-OBox unit test failed\n",TestName);
            printobb(unit);
            printsh(sunit);
            printct(c);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( (c=unit.Contains(_small)) != CONTAINS )
        {
            printe("%s: Sphere-OBox small test failed\n",TestName );
            printobb(unit);
            printsh(_small);
            printct( c );
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( (c=unit.Contains(big)) != INTERSECTS )
        {
            printe("%s: Sphere-OBox big test failed\n",TestName );
            printobb(unit);
            printsh(big);
            printct( c );
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( (c=unit.Contains(_far)) != DISJOINT )
        {
            printe("%s: Sphere-OBox far test failed\n",TestName );
            printobb(unit);
            printsh(_far);
            printct( c );
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
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0] * 2.1f, test[i][1] * 2.1f, test[i][2] * 2.1f);
            const BoundingSphere sph( center, 1.f );
            ContainmentType ct = unit.Contains( sph );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed sphere-OBox test1 %zu\n",TestName, i );
                printobb( unit );
                printsh( sph );
                printct( ct );
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
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            const BoundingSphere sph( center, 1.f );
            ContainmentType ct = unit.Contains( sph );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed sphere-OBox test2 %zu\n",TestName, i );
                printobb( unit );
                printsh( sph );
                printct( ct );
                success = false;
            }
        }
    }

    {
        BoundingSphere sunit;
        if ( (c=rotbox.Contains(sunit)) != CONTAINS )
        {
            printe("%s: Sphere-OBox unit test2 failed\n",TestName);
            printobb(rotbox);
            printsh(sunit);
            printct(c);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( (c=rotbox.Contains(_small)) != CONTAINS )
        {
            printe("%s: Sphere-OBox small test2 failed\n",TestName );
            printobb(rotbox);
            printsh(_small);
            printct( c );
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( (c=rotbox.Contains(big)) != INTERSECTS )
        {
            printe("%s: Sphere-OBox big test2 failed\n",TestName );
            printobb(rotbox);
            printsh(big);
            printct( c );
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( (c=rotbox.Contains(_far)) != DISJOINT )
        {
            printe("%s: Sphere-OBox far test2 failed\n",TestName );
            printobb(rotbox);
            printsh(_far);
            printct( c );
            success = false;
        }

        static const float test[][3] = { { -1.f, 1.f, 1.f }, { 0.0f, 1.f, 1.f }, {  1.f, 1.f, 1.f }, { -1.f, 0.0f, 1.f }, 
                                         {  1.f, 0.0f, 1.f }, { -1.f,-1.f, 1.f }, {  0.0f,-1.f, 1.f }, 
                                         {  1.f,-1.f, 1.f }, { -1.f, 1.f, 0.0f }, {  1.f, 1.f, 0.0f }, 
                                         { -1.f,-1.f, 0.0f }, 
                                         {  1.f,-1.f, 0.0f }, { -1.f, 1.f,-1.f }, {  0.0f, 1.f,-1.f }, {  1.f, 1.f,-1.f },
                                         { -1.f, 0.0f,-1.f }, {  1.f, 0.0f,-1.f }, { -1.f,-1.f,-1.f }, 
                                         {  0.0f,-1.f,-1.f }, {  1.f,-1.f,-1.f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0] * 2.1f, test[i][1] * 2.1f, test[i][2] * 2.1f);
            const BoundingSphere sph( center, 1.f );
            ContainmentType ct = rotbox.Contains( sph );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed sphere-OBox test3 %zu\n",TestName, i );
                printobb( rotbox );
                printsh( sph );
                printct( ct );
                success = false;
            }
        }

        static const float test2[][3] = { { 1.f, 0, 0 }, { -1.f, 0, 0 }, { 0, 1.f, 0 }, { 0, -1.f, 0 }, { 0, 0, 1.f }, { 0, 0, -1.f }, {  0.0f, 0.0f, 1.f },
                                         {  0.0f, 1.f, 0.0f }, { -1.f, 0.0f, 0.0f }, {  1.f, 0.0f, 0.0f }, {  0.0f,-1.f, 0.0f }, {  0.0f, 0.0f,-1.f }, 
                                         { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f },
                                         { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f }, {  0.5f, 0.5f, 0.5f }, { -0.5f, 0.0f, 0.5f }, 
                                         {  0.0f, 0.0f, 0.5f }, {  0.5f, 0.0f, 0.5f }, { -0.5f,-0.5f, 0.5f }, {  0.0f,-0.5f, 0.5f }, 
                                         {  0.5f,-0.5f, 0.5f }, { -0.5f, 0.5f, 0.0f }, {  0.0f, 0.5f, 0.0f }, {  0.5f, 0.5f, 0.0f }, 
                                         { -0.5f, 0.0f, 0.0f }, {  0.5f, 0.0f, 0.0f }, { -0.5f,-0.5f, 0.0f }, {  0.0f,-0.5f, 0.0f }, 
                                         {  0.5f,-0.5f, 0.0f }, { -0.5f, 0.5f,-0.5f }, {  0.0f, 0.5f,-0.5f }, {  0.5f, 0.5f,-0.5f },
                                         { -0.5f, 0.0f,-0.5f }, {  0.0f, 0.0f,-0.5f }, {  0.5f, 0.0f,-0.5f }, { -0.5f,-0.5f,-0.5f }, 
                                         {  0.0f,-0.5f,-0.5f }, {  0.5f,-0.5f,-0.5f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            const BoundingSphere sph( center, 1.f );
            ContainmentType ct = rotbox.Contains( sph );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed sphere-OBox test4 %zu\n",TestName, i );
                printobb( rotbox );
                printsh( sph );
                printct( ct );
                success = false;
            }
        }
    }

    // BoundingOrientedBox::Contains( const BoundingBox& box ) is covered by OOB case

    // OBox-OBox
    {
        if ( (c=unit.Contains(unit)) != CONTAINS )
        {
            printe("%s: OBox-OBox unit test failed\n",TestName);
            printobb(unit);
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
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed OBox-OBox axis-based test1 %zu\n",TestName, i );
                printobb( unit );
                printobb( box );
                printct( ct );
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
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            const BoundingOrientedBox box( center, XMFLOAT3( 1.f, 1.f, 1.f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed OBox-OBox axis-based test2 %zu\n",TestName, i );
                printobb( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingOrientedBox box1( XMFLOAT3( 0.1f, 0.1f, 0.1f ), XMFLOAT3( 1.f, 1.f, 1.f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            const BoundingOrientedBox box( XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed OBox-OBox axis-based test3\n",TestName );
                printobb( box1 );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingOrientedBox box( XMFLOAT3(0,0,0), XMFLOAT3( 2.f, 2.f, 2.f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed OBox-OBox axis-based test4\n",TestName );
                printobb( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingOrientedBox box1( XMFLOAT3(0,0,0), XMFLOAT3( 2.f, 2.f, 2.f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            const BoundingOrientedBox box( XMFLOAT3(0,0,0), XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed OBox-OBox axis-based test5\n",TestName );
                printobb( box1 );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        static const float test3[][3] = { { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f }, { 1.f, -1.f, -1.f }, 
                                          { -1.f, 1.f, 1.f }, { -1.f, 1.f, -1.f }, { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f } };
        for( size_t i=0; i < sizeof(test3)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test3[i][0], test3[i][1], test3[i][2]);
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed OBox-OBox axis-based test6 %zu\n",TestName, i );
                printobb( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }
    }        

    {
        if ( (c=rotbox.Contains(rotbox)) != INTERSECTS )
        {
            printe("%s: OBox-OBox rotbox test failed\n",TestName);
            printobb(rotbox);
            printct(c);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f }, { -2.0f, 0.0f, 2.0f }, 
                                         {  0.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, 
                                         {  2.0f,-2.0f, 2.0f }, { -2.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f }, 
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, 
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f }, 
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = rotbox.Contains( box );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed OBox-OBox axis-based test7 %zu\n",TestName, i );
                printobb( rotbox );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        static const float testb[][3] = { { 0, 2.f, 0 }, { 0, -2.f, 0 }, {  0.0f, 2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f } };
        for( size_t i=0; i < sizeof(testb)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(testb[i][0], testb[i][1], testb[i][2]);
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = rotbox.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed OBox-OBox axis-based test7b %zu\n",TestName, i );
                printobb( rotbox );
                printobb( box );
                printct( ct );
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
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            const BoundingOrientedBox box( center, XMFLOAT3( 1.f, 1.f, 1.f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = rotbox.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed OBox-OBox axis-based test8 %zu\n",TestName, i );
                printobb( rotbox );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingOrientedBox box1( XMFLOAT3( 0.1f, 0.1f, 0.1f ), XMFLOAT3( 1.f, 1.f, 1.f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            const BoundingOrientedBox box( XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed OBox-OBox axis-based test9\n",TestName );
                printobb( box1 );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingOrientedBox box( XMFLOAT3(0,0,0), XMFLOAT3( 2.f, 2.f, 2.f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = rotbox.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed OBox-OBox axis-based test10\n",TestName );
                printobb( rotbox );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingOrientedBox box1( XMFLOAT3(0,0,0), XMFLOAT3( 2.f, 2.f, 2.f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            const BoundingOrientedBox box( XMFLOAT3(0,0,0), XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed OBox-OBoxx axis-based test11\n",TestName );
                printobb( box1 );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        static const float test3[][3] = { { 1.f, 1.f, 1.f }, { 0.9f, -0.9f, -0.9f }, { -0.9f, 0.9f, 0.9f }, { -1.f, -1.f, -1.f } };
        for( size_t i=0; i < sizeof(test3)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test3[i][0], test3[i][1], test3[i][2]);
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = rotbox.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed OBox-OBox axis-based test12 %zu\n",TestName, i );
                printobb( rotbox );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        static const float test4[][3] = { { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f }, { -1.f, 1.f, -1.f }, { -1.f, -1.f, 1.f } };
        for( size_t i=0; i < sizeof(test4)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test4[i][0], test4[i][1], test4[i][2]);
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = rotbox.Contains( box );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed OBox-OBox axis-based test13 %zu\n",TestName, i );
                printobb( rotbox );
                printobb( box );
                printct( ct );
                success = false;
            }
        }
    }        

    // Box-Frustum
    {
        {
            BoundingFrustum fr(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 1.f, -1.f, 1.f, -1.f, 0.1f, 1.f);
            ContainmentType ct = unit.Contains(fr);
            if (ct != CONTAINS)
            {
                printe("%s: Failed obox-frustum test1\n", TestName);
                printbb(unit);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f }, 
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f }, 
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f }, 
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = unit.Contains( fr );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed obox-frustum test2 %zu\n",TestName, i );
                printobb( unit );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = unit.Contains( fr );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed obox-frustum test3 %zu\n",TestName, i );
                printobb( unit );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }
    }

    {
        {
            BoundingFrustum fr(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 1.f, -1.f, 1.f, -1.f, 0.1f, 1.f);
            ContainmentType ct = unit.Contains(fr);
            if (ct != CONTAINS)
            {
                printe("%s: Failed obox-frustum test4\n", TestName);
                printbb(unit);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f }, 
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f }, 
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f }, 
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = rotbox.Contains( fr );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed obox-frustum test5 %zu\n",TestName, i );
                printobb( rotbox );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = rotbox.Contains( fr );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed obox-frustum test6 %zu\n",TestName, i );
                printobb( rotbox );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }
    }

    {
        const BoundingOrientedBox box(XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT3(4.f, 5.f, 6.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );

        {
            BoundingFrustum fr(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 1.f, -1.f, 1.f, -1.f, 0.1f, 1.f);
            ContainmentType ct = box.Contains(fr);
            if (ct != CONTAINS)
            {
                printe("%s: Failed obox-frustum test7\n", TestName);
                printobb(box);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f }, 
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f }, 
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = box.Contains( fr );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed obox-frustum test8 %zu\n",TestName, i );
                printobb( box );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 12.f }, { -12.0f, 12.0f, 12.0f }, { 0.0f, 12.0f, 12.0f }, { 12.0f, 12.0f, 12.0f },
                                          { -12.0f, 0.0f, 12.0f }, { 12.0f, 0.0f, 12.0f }, { -12.0f,-12.0f, 12.0f }, { 0.0f,-12.0f, 12.0f }, { 12.0f,-12.0f, 12.0f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = box.Contains( fr );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed obox-frustum test9 %zu\n",TestName, i );
                printobb( unit );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }
    }

    {
        const BoundingOrientedBox box(XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT3(4.f, 5.f, 6.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );

        {
            BoundingFrustum fr(XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 1.f, -1.f, 1.f, -1.f, 0.1f, 1.f);
            ContainmentType ct = box.Contains(fr);
            if (ct != CONTAINS)
            {
                printe("%s: Failed obox-frustum test10\n", TestName);
                printobb(box);
                printfr(fr);
                printct(ct);
                success = false;
            }
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f },
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f }, 
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f }, 
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = box.Contains( fr );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed obox-frustum test11 %zu\n",TestName, i );
                printobb( box );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 12.f }, { -12.0f, 12.0f, 12.0f }, { 0.0f, 12.0f, 12.0f }, { 12.0f, 12.0f, 12.0f },
                                          { -12.0f, 0.0f, 12.0f }, { 12.0f, 0.0f, 12.0f }, { -12.0f,-12.0f, 12.0f }, { 0.0f,-12.0f, 12.0f }, { 12.0f,-12.0f, 12.0f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = box.Contains( fr );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed obox-frustum test12 %zu\n",TestName, i );
                printobb( unit );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingOrientedBox::Intersects
HRESULT TestO05(LogProxy* pLog)
{
    bool success = true;

    const BoundingOrientedBox unit;
    const BoundingOrientedBox rotbox( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );

    // OBox-Sphere
    {
        if ( !unit.Intersects(unit) )
        {
            printe("%s: OBox-Sphere unit test failed\n",TestName);
            printobb(unit);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( !unit.Intersects(_small) )
        {
            printe("%s: OBox-Sphere small test failed\n",TestName);
            printobb(unit);
            printsh(_small);
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( !unit.Intersects(big) )
        {
            printe("%s: OBox-Sphere big test failed\n",TestName);
            printobb(unit);
            printsh(big);
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( unit.Intersects(_far) )
        {
            printe("%s: OBox-sphere far test failed\n",TestName);
            printobb(unit);
            printsh(_far);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const BoundingSphere t( XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f );

            // Just-touching should intersect
            if ( !unit.Intersects( t ) )
            {
                printe( "%s: Failed OBox-Sphere axis-based test1A\n",TestName );
                printobb( unit );
                printsh( t );
                success = false;
            }

            // Just missing should be disjoint
            const BoundingSphere t2( XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f - EPSILON );
            if ( unit.Intersects( t2 ) )
            {
                printe( "%s: Failed OBox-Sphere axis-based test1B\n",TestName );
                printobb( unit );
                printsh( t2 );
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const BoundingSphere t( XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), 1.f );

            if ( !unit.Intersects( t ) )
            {
                printe( "%s: Failed OBox-Sphere axis-based test2\n",TestName );
                printobb( unit );
                printsh( t );
                success = false;
            }
        }
    }

    {
        if ( !rotbox.Intersects(unit) )
        {
            printe("%s: OBox-Sphere rotbox test failed\n",TestName);
            printobb(rotbox);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( !rotbox.Intersects(_small) )
        {
            printe("%s: OBox-Sphere small test2 failed\n",TestName);
            printobb(rotbox);
            printsh(_small);
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( !rotbox.Intersects(big) )
        {
            printe("%s: OBox-Sphere big test2 failed\n",TestName);
            printobb(rotbox);
            printsh(big);
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( rotbox.Intersects(_far) )
        {
            printe("%s: OBox-sphere far test2 failed\n",TestName);
            printobb(rotbox);
            printsh(_far);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const BoundingSphere t( XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f );

            if ( !rotbox.Intersects( t ) )
            {
                printe( "%s: Failed OBox-Sphere axis-based test3\n",TestName );
                printobb( rotbox );
                printsh( t );
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const BoundingSphere t( XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), 1.f );

            if ( !rotbox.Intersects( t ) )
            {
                printe( "%s: Failed OBox-Sphere axis-based test4\n",TestName );
                printobb( rotbox );
                printsh( t );
                success = false;
            }
        }
    }

    // BoundingOrientedBox::Intersects( const BoundingBox& box ) is covered by OOB case

    // OBox-OBox
    {
        if ( !unit.Intersects(unit) )
        {
            printe("%s: OBox-OBox unit test failed\n",TestName);
            printobb(unit);
            success = false;
        }

        const BoundingOrientedBox _small( XMFLOAT3(0.1f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
        if ( !_small.Intersects(unit) )
        {
            printe("%s: OBox-OBox small test failed\n",TestName);
            printobb(_small);
            printobb(unit);
            success = false;
        }

        const BoundingOrientedBox big( XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(10.f, 10.f, 10.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
        if ( !big.Intersects(unit) )
        {
            printe("%s: OBox-OBox big test failed\n",TestName);
            printobb(big);
            printobb(unit);
            success = false;
        }

        const BoundingOrientedBox _far( XMFLOAT3(10.f, -5.f, 4.f), XMFLOAT3(2.f, 2.f, 2.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
        if ( _far.Intersects(unit) )
        {
            printe("%s: OBox-OBox far test failed\n",TestName);
            printobb(_far);
            printobb(unit);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const BoundingOrientedBox t( XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3( 1.f, 1.f, 1.f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );

            // Just-touching should intersect
            if ( !t.Intersects( unit ) )
            {
                printe( "%s: Failed OBox-OBox axis-based test1A\n",TestName );
                printobb( t );
                printobb(unit);
                success = false;
            }

            // Just missing should be disjoint
            const BoundingOrientedBox t2( XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3( 1.f - EPSILON, 1.f - EPSILON, 1.f - EPSILON ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
            if ( t2.Intersects( unit ) )
            {
                printe( "%s: Failed OBox-OBox axis-based test1B\n",TestName );
                printobb( t2 );
                printobb(unit);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const BoundingOrientedBox t( XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );

            if ( !t.Intersects( unit ) )
            {
                printe( "%s: Failed OBox-OBox axis-based test2\n",TestName );
                printobb( t );
                printobb(unit);
                success = false;
            }
        }
    }

    {
        if ( !rotbox.Intersects(unit) )
        {
            printe("%s: OBox-OBox rotbox test failed\n",TestName);
            printobb(rotbox);
            success = false;
        }

        const BoundingOrientedBox _small( XMFLOAT3(0.1f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
        if ( !_small.Intersects(rotbox) )
        {
            printe("%s: OBox-OBox small test2 failed\n",TestName);
            printobb(_small);
            printobb(rotbox);
            success = false;
        }

        const BoundingOrientedBox big( XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(10.f, 10.f, 10.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
        if ( !big.Intersects(rotbox) )
        {
            printe("%s: OBox-OBox big test2 failed\n",TestName);
            printobb(big);
            printobb(rotbox);
            success = false;
        }

        const BoundingOrientedBox _far( XMFLOAT3(10.f, -5.f, 4.f), XMFLOAT3(2.f, 2.f, 2.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
        if ( _far.Intersects(rotbox) )
        {
            printe("%s: OBox-OBox far test2 failed\n",TestName);
            printobb(_far);
            printobb(rotbox);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const BoundingOrientedBox t( XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3( 1.f, 1.f, 1.f ), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );

            if ( !t.Intersects( rotbox ) )
            {
                printe( "%s: Failed OBox-OBox axis-based test3\n",TestName );
                printobb( t );
                printobb(rotbox);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const BoundingOrientedBox t( XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );

            if ( !t.Intersects( rotbox ) )
            {
                printe( "%s: Failed OBox-OBox axis-based test4\n",TestName );
                printobb( t );
                printobb(rotbox);
                success = false;
            }
        }
    }

    // OBox-Triangle
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

        static_assert( sizeof(tri_in) == sizeof(tri_out), "TestO05 OBox-tri tests" );

        for( uint32_t i=0; i < (sizeof(tri_in)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_in[i].v;
            XMVECTOR t1 = tri_in[i+1].v;
            XMVECTOR t2 = tri_in[i+2].v;
            if ( !unit.Intersects(t0, t1, t2) )
            {
                printe("%s: Triangle-OBox test failed (ins %d)\n",TestName, i);
                printobb(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_out[i].v;
            t1 = tri_out[i+1].v;
            t2 = tri_out[i+2].v;
            if ( unit.Intersects(t0, t1, t2) )
            {
                printe("%s: Triangle-OBox test failed (outs %d)\n",TestName, i);
                printobb(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
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

        static_assert( sizeof(tri_in) == sizeof(tri_out), "TestO05 OBox-tri test2" );

        for( uint32_t i=0; i < (sizeof(tri_in)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_in[i].v;
            XMVECTOR t1 = tri_in[i+1].v;
            XMVECTOR t2 = tri_in[i+2].v;
            if ( !rotbox.Intersects(t0, t1, t2) )
            {
                printe("%s: Triangle-OBox test2 failed (ins %d)\n",TestName, i);
                printobb(rotbox);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }

            t0 = tri_out[i].v;
            t1 = tri_out[i+1].v;
            t2 = tri_out[i+2].v;
            if ( rotbox.Intersects(t0, t1, t2) )
            {
                printe("%s: Triangle-OBox test2 failed (outs %d)\n",TestName, i);
                printobb(rotbox);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    // BoundingOrientedBox::Intersects( const BoundingFrustum& fr ) is covered by frustum.cpp

    // OBox-Plane
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

        static_assert( (sizeof(planes)/sizeof(XMVECTORF32)) == (sizeof(result)/sizeof(PlaneIntersectionType)), "TestO05 OBox-Plane tests" );

        for( uint32_t i=0; i < (sizeof(planes)/sizeof(XMVECTORF32)); ++i )
        {
            PlaneIntersectionType p = unit.Intersects( planes[i] );

            if ( p != result[i] )
            {
                printe("%s: Plane-OBox test failed ([%d] result %d, expected %d)\n",TestName, i, p, result[i] );
                printobb(unit);
                printxmv( planes[i] );
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

        static_assert( (sizeof(planes)/sizeof(XMVECTORF32)) == (sizeof(result)/sizeof(PlaneIntersectionType)), "TestO05 OBox-Plane test2" );

        for( uint32_t i=0; i < (sizeof(planes)/sizeof(XMVECTORF32)); ++i )
        {
            PlaneIntersectionType p = rotbox.Intersects( planes[i] );

            if ( p != result[i] )
            {
                printe("%s: Plane-OBox test2 failed ([%d] result %d, expected %d)\n",TestName, i, p, result[i] );
                printobb(rotbox);
                printxmv( planes[i] );
                success = false;
            }
        }
    }

    // OBox-Ray
    {
        float dist;

        const XMVECTORF32 rayA[2] = { { 0.1f, 0.1f, 0.1f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if ( !unit.Intersects( rayA[0], rayA[1], dist ) || (fabs(dist + 1.1f) > EPSILON) )
        {
            printe("%s: OBox-Ray test A failed (dist=%f)\n",TestName, dist);
            printobb(unit);
            printxmv( rayA[0] );
            printxmv( rayA[1] );
            success = false;
        }

        const XMVECTORF32 rayB[2] = { { 10.f, 10.f, 10.f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if ( unit.Intersects( rayB[0], rayB[1], dist ) ) // should miss box
        {
            printe("%s: OBox-Ray test B failed (dist=%f)\n",TestName, dist);
            printobb(unit);
            printxmv( rayB[0] );
            printxmv( rayB[1] );
            success = false;
        }

        const XMVECTORF32 rayC[2] = { { 10.f, 10.f, 10.f, 0.f }, { -0.577350f, -0.577350f, -0.577350f, 0.f } };
        if ( !unit.Intersects( rayC[0], rayC[1], dist ) || (fabs(dist - 15.588465f) > EPSILON) )
        {
            printe("%s: OBox-Ray test C failed (dist=%f)\n",TestName, dist);
            printobb(unit);
            printxmv( rayC[0] );
            printxmv( rayC[1] );
            success = false;
        }
    }

    {
        float dist;

        const XMVECTORF32 rayA[2] = { { 0.1f, 0.1f, 0.1f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if ( !rotbox.Intersects( rayA[0], rayA[1], dist ) || (fabs(dist + 1.312994f) > EPSILON) )
        {
            printe("%s: OBox-Ray test D failed (dist=%f)\n",TestName, dist);
            printobb(rotbox);
            printxmv( rayA[0] );
            printxmv( rayA[1] );
            success = false;
        }

        const XMVECTORF32 rayB[2] = { { 10.f, 10.f, 10.f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if ( rotbox.Intersects( rayB[0], rayB[1], dist ) ) // should miss box
        {
            printe("%s: OBox-Ray test E failed (dist=%f)\n",TestName, dist);
            printobb(rotbox);
            printxmv( rayB[0] );
            printxmv( rayB[1] );
            success = false;
        }

        const XMVECTORF32 rayC[2] = { { 10.f, 10.f, 10.f, 0.f }, { -0.577350f, -0.577350f, -0.577350f, 0.f } };
        if ( !rotbox.Intersects( rayC[0], rayC[1], dist ) || (fabs(dist - 15.885636f) > EPSILON) )
        {
            printe("%s: OBox-Ray test F failed (dist=%f)\n",TestName, dist);
            printobb(rotbox);
            printxmv( rayC[0] );
            printxmv( rayC[1] );
            success = false;
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingOrientedBox::ContainedBy
HRESULT TestO06(LogProxy* pLog)
{
    bool success = true;

    const BoundingOrientedBox unit;
    const BoundingOrientedBox rotbox( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );

    {
        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f }, 
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f }, 
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f }, 
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes( &Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5 );

            ContainmentType ct = unit.ContainedBy( Plane0, Plane1, Plane2, Plane3, Plane4, Plane5 );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed obox-6planes test1 %zu\n",TestName,i );
                printobb( unit );
                printxmv( Plane0 );
                printxmv( Plane1 );
                printxmv( Plane2 );
                printxmv( Plane3 );
                printxmv( Plane4 );
                printxmv( Plane5 );
                printct( ct );
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes( &Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5 );

            ContainmentType ct = unit.ContainedBy( Plane0, Plane1, Plane2, Plane3, Plane4, Plane5 );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed obox-6planes test2 %zu\n",TestName,i );
                printobb( unit );
                printxmv( Plane0 );
                printxmv( Plane1 );
                printxmv( Plane2 );
                printxmv( Plane3 );
                printxmv( Plane4 );
                printxmv( Plane5 );
                printct( ct );
                success = false;
            }
        }
    }

    {
        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, -2.f },
                                         { -2.0f, 2.0f, 0.0f }, {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f }, 
                                         { -2.0f, 0.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, { -2.0f,-2.0f, 0.0f }, {  0.0f,-2.0f, 0.0f }, 
                                         {  2.0f,-2.0f, 0.0f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f }, 
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes( &Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5 );

            ContainmentType ct = rotbox.ContainedBy( Plane0, Plane1, Plane2, Plane3, Plane4, Plane5 );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed obox-6planes test3 %zu\n",TestName,i );
                printobb( rotbox );
                printxmv( Plane0 );
                printxmv( Plane1 );
                printxmv( Plane2 );
                printxmv( Plane3 );
                printxmv( Plane4 );
                printxmv( Plane5 );
                printct( ct );
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes( &Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5 );

            ContainmentType ct = rotbox.ContainedBy( Plane0, Plane1, Plane2, Plane3, Plane4, Plane5 );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed obox-6planes test4 %zu\n",TestName,i );
                printobb( rotbox );
                printxmv( Plane0 );
                printxmv( Plane1 );
                printxmv( Plane2 );
                printxmv( Plane3 );
                printxmv( Plane4 );
                printxmv( Plane5 );
                printct( ct );
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingOrientedBox::CreateFromBoundingBox
HRESULT TestO07(LogProxy* pLog)
{
    bool success = true;

    BoundingBox aabox( XMFLOAT3(2.5f, 3.5f, 4.5f), XMFLOAT3(1.5f, 1.5f, 1.5f) );

    BoundingOrientedBox box;
    BoundingOrientedBox::CreateFromBoundingBox( box, aabox );

    if ( fabs( box.Center.x - 2.5) > EPSILON
         || fabs( box.Center.y - 3.5f) > EPSILON
         || fabs( box.Center.z - 4.5f) > EPSILON
         || fabs( box.Extents.x - 1.5f) > EPSILON
         || fabs( box.Extents.y - 1.5f) > EPSILON
         || fabs( box.Extents.z - 1.5f) > EPSILON
         || fabs( box.Orientation.x - 0.f) > EPSILON
         || fabs( box.Orientation.y - 0.f) > EPSILON
         || fabs( box.Orientation.z - 0.f) > EPSILON
         || fabs( box.Orientation.w - 1.f) > EPSILON )
    {
        printe("%s: failed\n",TestName);
        printbb(aabox);
        printobb(box);
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingOrientedBox::CreateFromPoints
HRESULT TestO08(LogProxy* pLog)
{
    bool success = true;

    BoundingOrientedBox box;

    // CreateFromPoints array
    {
        const float points[] = { 1.1f, 2.2f, 3.3f, 2.1f, 3.2f, 1.3f, 3.1f, 1.2f, 2.3f };

        BoundingOrientedBox::CreateFromPoints( box, 3, reinterpret_cast<const XMFLOAT3*>( points ), sizeof(XMFLOAT3) );
        if ( fabs( box.Center.x - 2.350041f) > EPSILON
             || fabs( box.Center.y - 1.950174f) > EPSILON
             || fabs( box.Center.z - 2.299908f) > EPSILON
             || fabs( box.Extents.x - 1.224745f) > EPSILON
             || fabs( box.Extents.y - 0.000229f) > EPSILON
             || fabs( box.Extents.z - 1.060754f) > EPSILON
             || fabs( box.Orientation.x + 0.059968f) > EPSILON
             || fabs( box.Orientation.y + 0.704545f) > EPSILON
             || fabs( box.Orientation.z + 0.455689f) > EPSILON
             || fabs( box.Orientation.w - 0.540710f) > EPSILON )
        {
            printe("%s: CreateFromPoints (array) failed\n",TestName);
            printobb(box);
            success = false;
        }
    }

    {
        XMFLOAT3 points[32];

        const uint32_t count = sizeof(points)/sizeof(XMFLOAT3);

        for( uint32_t i=0; i < count; ++i )
        {
            XMStoreFloat3( &points[i], GetRandomVector16() );
        }

        BoundingOrientedBox::CreateFromPoints( box, count, points, sizeof(XMFLOAT3) );

        // Expand a bit to ensure Contains works for all input points on all platforms
        box.Extents.x += EPSILON;
        box.Extents.y += EPSILON;
        box.Extents.z += EPSILON;

        for( uint32_t i=0; i < count; ++i )
        {
            XMVECTOR p = XMLoadFloat3( &points[i] );
            if ( box.Contains( p ) == DISJOINT )
            {
                printe("%s: OBox-Point verification test failed (%d)\n",TestName, i);
                printxmv( p );
                printobb(box);
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}
