//-------------------------------------------------------------------------------------
// box.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

static const float EPSILON = 0.00001f;

inline bool IsEqual( const BoundingBox& b1, const BoundingBox& b2 )
{
    return ( ( fabs( b1.Center.x - b2.Center.x ) < EPSILON )
             && ( fabs( b1.Center.y - b2.Center.y ) < EPSILON )
             && ( fabs( b1.Center.z - b2.Center.z ) < EPSILON )
             && ( fabs( b1.Extents.x - b2.Extents.x ) < EPSILON )
             && ( fabs( b1.Extents.y - b2.Extents.y ) < EPSILON )
             && ( fabs( b1.Extents.z - b2.Extents.z ) < EPSILON ) );
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
// BoundingBox
HRESULT TestB01(LogProxy* pLog)
{
    bool success = true;

    static_assert(std::is_nothrow_copy_assignable<BoundingBox>::value, "Copy Assign.");
    static_assert(std::is_nothrow_copy_constructible<BoundingBox>::value, "Copy Ctor.");
    static_assert(std::is_nothrow_move_constructible<BoundingBox>::value, "Move Ctor.");
    static_assert(std::is_nothrow_move_assignable<BoundingBox>::value, "Move Assign.");

    // Default constructor
    BoundingBox bb;
    if ( fabs( bb.Center.x ) > EPSILON
         || fabs( bb.Center.y ) > EPSILON
         || fabs( bb.Center.z ) > EPSILON
         || fabs( bb.Extents.x - 1.0f ) > EPSILON
         || fabs( bb.Extents.y - 1.0f ) > EPSILON
         || fabs( bb.Extents.z - 1.0f ) > EPSILON )
    {
        printe("%s: Default constructor didn't create unit axis-aligned box\n",TestName);
        printbb( bb );
        success = false;
    }

    // Center + Extents constructor
    BoundingBox bbp( XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f) );
    if ( fabs( bbp.Center.x - 1.f) > EPSILON
         || fabs( bbp.Center.y - 1.f) > EPSILON
         || fabs( bbp.Center.z - 1.f) > EPSILON
         || fabs( bbp.Extents.x - 2.0f ) > EPSILON
         || fabs( bbp.Extents.y - 2.0f ) > EPSILON
         || fabs( bbp.Extents.z - 2.0f ) > EPSILON )
    {
        printe("%s: Constructor didn't create correct box\n",TestName);
        printbb( bbp );
        success = false;
    }

    // Copy constructor
    BoundingBox bbc(bbp);
    if ( !IsEqual(bbc,bbp) )
    {
        printe("%s: Copy constructor failed\n",TestName);
        printbb( bbc );
        printbb( bbp );
        success = false;
    }

    // Test assignment operator
    BoundingBox bbi;

    bbi = bbp;
    if ( !IsEqual(bbi,bbp) )
    {
        printe("%s: Assignment operator failed\n",TestName);
        printbb( bbi );
        printbb( bbp );
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingBox::Transform
HRESULT TestB02(LogProxy* pLog)
{
    bool success = true;

    BoundingBox ubox, vbox;

    XMMATRIX Mid = XMMatrixIdentity();
    XMVECTOR Rid = XMQuaternionIdentity();
    XMVECTOR Tid = XMVectorZero();

    // Unit box
    const BoundingBox unit;

    unit.Transform( ubox, Mid );
    if ( !IsEqual(unit,ubox) )
    {
        printe("%s: Unit box tranform(1) failed\n",TestName);
        printbb(unit);
        printbb(ubox);
        success = false;
    }

    unit.Transform( vbox, 1.0f, Rid, Tid );
    if ( !IsEqual(unit,vbox) )
    {
        printe("%s: Unit box tranform(2) failed\n",TestName);
        printbb(unit);
        printbb(vbox);
        success = false;
    }

    // Small box (w/ scaling)
    const BoundingBox _small( XMFLOAT3(0.1f, 0.0f, 0.0f), XMFLOAT3(0.5f,0.5f,0.5f) );

    XMMATRIX mat = XMMatrixTransformation( g_XMZero, // Scaling origin
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

    _small.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 1.3f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 1.0f) > EPSILON
         || fabs( ubox.Extents.x - 1.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.5f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON )
    {
        printe("%s: Small transform(1) failed\n",TestName);
        printbb(_small);
        printbb(ubox);
        success = false;
    }

    _small.Transform( vbox, 3.0f, Rid, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(2) failed\n",TestName);
        printbb(_small);
        printbb(ubox);
        printbb(vbox);
        success = false;
    }

    // Small box (w/ non-uniform scaling)
    _small.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 1.1f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 1.0f) > EPSILON
         || fabs( ubox.Extents.x - 0.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.0f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON )
    {
        printe("%s: Small transform(3) failed\n",TestName);
        printbb(_small);
        printbb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Small transform(4) failed\n",TestName);
        printbb(_small);
        printbb(ubox);
        printbb(vbox);
        success = false;
    }

    // Big box (w/ scaling)
    const BoundingBox big( XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3( 10.f, 10.f, 10.f ) );

    big.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 4.f) > EPSILON
         || fabs( ubox.Center.y - 7.f) > EPSILON
         || fabs( ubox.Center.z - 10.f) > EPSILON
         || fabs( ubox.Extents.x - 30.f) > EPSILON
         || fabs( ubox.Extents.y - 30.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON )
    {
        printe("%s: Big transform(1) failed\n",TestName);
        printbb(big);
        printbb(ubox);
        success = false;
    }

    big.Transform( vbox, 3.0f, Rid, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(2) failed\n",TestName);
        printbb(big);
        printbb(ubox);
        printbb(vbox);
        success = false;
    }

    // Big box (w/ non-uniform scaling)
    big.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 2.f) > EPSILON
         || fabs( ubox.Center.y - 5.f) > EPSILON
         || fabs( ubox.Center.z - 10.f) > EPSILON
         || fabs( ubox.Extents.x - 10.f) > EPSILON
         || fabs( ubox.Extents.y - 20.f) > EPSILON
         || fabs( ubox.Extents.z - 30.f) > EPSILON )
    {
        printe("%s: Big transform(3) failed\n",TestName);
        printbb(big);
        printbb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Big transform(4) failed\n",TestName);
        printbb(big);
        printbb(ubox);
        printbb(vbox);
        success = false;
    }

    // Small box (w/ rotation & scaling)
    XMVECTOR rot = XMQuaternionRotationRollPitchYaw( 0.f, XM_PIDIV2, 0.f );

    mat = XMMatrixTransformation( g_XMZero, // Scaling origin
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

    _small.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 1.f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 0.7f) > EPSILON
         || fabs( ubox.Extents.x - 1.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.5f) > EPSILON
         || fabs( ubox.Extents.z - 1.5f) > EPSILON )
    {
        printe("%s: Small transform(5) failed\n",TestName);
        printbb(_small);
        printbb(ubox);
        success = false;
    }

    _small.Transform( vbox, 3.0f, rot, g_XMOne );

    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Small transform(6) failed\n",TestName);
        printbb(_small);
        printbb(ubox);
        printbb(vbox);
        success = false;
    }

    // Small box (w/ rotation & non-uniform scaling)
    _small.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 1.f) > EPSILON
         || fabs( ubox.Center.y - 1.f) > EPSILON
         || fabs( ubox.Center.z - 0.9f) > EPSILON
         || fabs( ubox.Extents.x - 1.5f) > EPSILON
         || fabs( ubox.Extents.y - 1.0f) > EPSILON
         || fabs( ubox.Extents.z - 0.5f) > EPSILON )
    {
        printe("%s: Small transform(7) failed\n",TestName);
        printbb(_small);
        printbb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Small transform(8) failed\n",TestName);
        printbb(_small);
        printbb(ubox);
        printbb(vbox);
        success = false;
    }

    // Big box (w/ rotation & scaling)
    big.Transform( ubox, mat );
    if ( fabs( ubox.Center.x - 10.f) > EPSILON
         || fabs( ubox.Center.y - 7.f) > EPSILON
         || fabs( ubox.Center.z + 1.999998f) > EPSILON
         || fabs( ubox.Extents.x - 30.f) > EPSILON
         || fabs( ubox.Extents.y - 30.f) > EPSILON
         || fabs( ubox.Extents.z - 29.999998f) > EPSILON )
    {
        printe("%s: Big transform(5) failed\n",TestName);
        printbb(big);
        printbb(ubox);
        success = false;
    }

    big.Transform( vbox, 3.0f, rot, g_XMOne );
    if ( !IsEqual(ubox,vbox) )
    {
        printe("%s: Big transform(6) failed\n",TestName);
        printbb(big);
        printbb(ubox);
        printbb(vbox);
        success = false;
    }

    // Big box (w/ rotation & non-uniform scaling)
    big.Transform( ubox, matNonUniform );
    if ( fabs( ubox.Center.x - 10.f) > EPSILON
         || fabs( ubox.Center.y - 5.f) > EPSILON
         || fabs( ubox.Center.z + 0.f) > EPSILON
         || fabs( ubox.Extents.x - 30.f) > EPSILON
         || fabs( ubox.Extents.y - 20.f) > EPSILON
         || fabs( ubox.Extents.z - 10.f) > EPSILON )
    {
        printe("%s: Big transform(7) failed\n",TestName);
        printbb(big);
        printbb(ubox);
        success = false;
    }

    if ( IsEqual(ubox,vbox) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Big transform(8) failed\n",TestName);
        printbb(big);
        printbb(ubox);
        printbb(vbox);
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingBox::GetCorners
HRESULT TestB03(LogProxy* pLog)
{
    bool success = true;
          
    static_assert( BoundingBox::CORNER_COUNT == 8, "TestB03 expects there are 8 corners to a box" );

    {
        BoundingBox box;
        BoundingBox::CreateFromPoints( box, XMVectorSet( 1.0f, 2.0f, 3.0f, 0 ), XMVectorSet( 4.0f, 5.0f, 6.0f, 0 ) );

        XMFLOAT3 corners[BoundingBox::CORNER_COUNT];
        box.GetCorners( corners );

        static const float expected[][3] = { { 1.f, 2.f, 6.f }, { 4.f, 2.f, 6.f }, { 4.f, 5.f, 6.f }, { 1.f, 5.f, 6.f },
                                             { 1.f, 2.f, 3.f }, { 4.f, 2.f, 3.f }, { 4.f, 5.f, 3.f }, { 1.f, 5.f, 3.f } };

        for( size_t i = 0; i < BoundingBox::CORNER_COUNT; ++i )
        {
            if ( fabs( corners[i].x - expected[i][0] ) > EPSILON
                 || fabs( corners[i].y - expected[i][1] ) > EPSILON
                 || fabs( corners[i].z - expected[i][2] ) > EPSILON )
            {
                printe( "%s: GetCorners failed", TestName );
                printe( "%f %f %f ... %f %f %f \n", corners[i].x, corners[i].y, corners[i].z, expected[i][0], expected[i][1], expected[i][2] );
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingBox::Contains
HRESULT TestB04(LogProxy* pLog)
{
    const BoundingBox unit;
    ContainmentType c;

    bool success = true;

    // Box-Point
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

        static_assert( sizeof(pnts_in) == sizeof(pnts_out), "TestB04 box-point tests" );

        for( uint32_t i=0; i < (sizeof(pnts_in)/sizeof(XMVECTORF32)); ++i)
        {
            if ( (c=unit.Contains( pnts_in[i].v )) != CONTAINS )
            {
                printe("%s: Point-Box test failed (ins %d)\n",TestName, i);
                printbb(unit);
                printxmv( pnts_in[i].v );
                printct( c );
                success = false;
            }

            if ( (c=unit.Contains( pnts_out[i].v )) != DISJOINT )
            {
                printe("%s: Point-Box test failed (outs %d)\n",TestName, i);
                printbb(unit);
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
                printe( "%s: Failed Point-Box test1 %zu\n",TestName, i );
                printbb( unit );
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
                printe( "%s: Failed Point-Box test2 %zu\n",TestName, i );
                printbb( unit );
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
                printe("%s: Triangle-Box test failed (CONTAINS %d)\n",TestName, i);
                printct( c );
                printbb(unit);
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
                printe("%s: Triangle-Box test failed (INTERSECTS %d)\n",TestName, i);
                printct( c );
                printbb(unit);
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
                printe("%s: Triangle-Box test failed (DISJOINT %d)\n",TestName, i);
                printct( c );
                printbb(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    {
        const BoundingBox box( XMFLOAT3( 2.f, 3.f, 4.f ), XMFLOAT3( 4.f, 5.f, 6.f ) );

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
                printe("%s: Triangle-Box test2 failed (CONTAINS %d)\n",TestName, i);
                printct( c );
                printbb(box);
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
                printe("%s: Triangle-Box test2 failed (INTERSECTS %d)\n",TestName, i);
                printct( c );
                printbb(box);
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
                printe("%s: Triangle-Box test2 failed (DISJOINT %d)\n",TestName, i);
                printct( c );
                printbb(box);
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
            printe("%s: Sphere-Box unit test failed\n",TestName);
            printbb(unit);
            printsh(sunit);
            printct(c);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( (c=unit.Contains(_small)) != CONTAINS )
        {
            printe("%s: Sphere-Box small test failed\n",TestName );
            printbb(unit);
            printsh(_small);
            printct( c );
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( (c=unit.Contains(big)) != INTERSECTS )
        {
            printe("%s: Sphere-Box big test failed\n",TestName );
            printbb(unit);
            printsh(big);
            printct( c );
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( (c=unit.Contains(_far)) != DISJOINT )
        {
            printe("%s: Sphere-Box far test failed\n",TestName );
            printbb(unit);
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
                printe( "%s: Failed sphere-Box test1 %zu\n",TestName, i );
                printbb( unit );
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
                printe( "%s: Failed sphere-Box test2 %zu\n",TestName, i );
                printbb( unit );
                printsh( sph );
                printct( ct );
                success = false;
            }
        }
    }

    // Box-Box
    {
        if ( (c=unit.Contains(unit)) != CONTAINS )
        {
            printe("%s: Box-Box unit test failed\n",TestName);
            printbb(unit);
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
            const BoundingBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed box-box axis-based test1 %zu\n",TestName, i );
                printbb( unit );
                printbb( box );
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
            const BoundingBox box( center, XMFLOAT3( 1.f, 1.f, 1.f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-box axis-based test2 %zu\n",TestName, i );
                printbb( unit );
                printbb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingBox box1( XMFLOAT3( 0.1f, 0.1f, 0.1f ), XMFLOAT3( 1.f, 1.f, 1.f ) );
            const BoundingBox box( XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT3( 0.5f, 0.5f, 0.5f ) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed box-box axis-based test3\n",TestName );
                printbb( box1 );
                printbb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingBox box( XMFLOAT3(0,0,0), XMFLOAT3( 2.f, 2.f, 2.f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-box axis-based test4\n",TestName );
                printbb( unit );
                printbb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingBox box1( XMFLOAT3(0,0,0), XMFLOAT3( 2.f, 2.f, 2.f ) );
            const BoundingBox box( XMFLOAT3(0,0,0), XMFLOAT3( 0.5f, 0.5f, 0.5f ) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed box-box axis-based test5\n",TestName );
                printbb( box1 );
                printbb( box );
                printct( ct );
                success = false;
            }
        }

        static const float test3[][3] = { { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f }, { 1.f, -1.f, 1.f }, { 1.f, -1.f, -1.f }, 
                                          { -1.f, 1.f, 1.f }, { -1.f, 1.f, -1.f }, { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f } };
        for( size_t i=0; i < sizeof(test3)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test3[i][0], test3[i][1], test3[i][2]);
            const BoundingBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-box axis-based test6 %zu\n",TestName, i );
                printbb( unit );
                printbb( box );
                printct( ct );
                success = false;
            }
        }
    }        

    // Box-OrientedBox
    {
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
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed box-oobox axis-based test1 %zu\n",TestName, i );
                printbb( unit );
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
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-obox axis-based test2 %zu\n",TestName, i );
                printbb( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingBox box1( XMFLOAT3( 0.1f, 0.1f, 0.1f ), XMFLOAT3( 1.f, 1.f, 1.f ) );
            const BoundingOrientedBox box ( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-oobox axis-based test3\n",TestName );
                printbb( box1 );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingOrientedBox box ( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-oobox axis-based test4\n",TestName );
                printbb( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingBox box1( XMFLOAT3( 0.1f, 0.1f, 0.1f ), XMFLOAT3( 1.f, 1.f, 1.f ) );
            const BoundingOrientedBox box ( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-oobox axis-based test5\n",TestName );
                printbb( box1 );
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
            const BoundingOrientedBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-oobox axis-based test6 %zu\n",TestName, i );
                printbb( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingBox box1( XMFLOAT3( 2.f, 3.f, 4.f ), XMFLOAT3( 4.f, 5.f, 6.f ) );
            const BoundingOrientedBox box ( XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed box-oobox axis-based test7\n",TestName );
                printbb( box1 );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingBox box1( XMFLOAT3( 2.f, 3.f, 4.f ), XMFLOAT3( 4.f, 5.f, 6.f ) );
            const BoundingOrientedBox box ( XMFLOAT3(12.f, 3.f, 4.f), XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed box-oobox axis-based test8\n",TestName );
                printbb( box1 );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingBox box1( XMFLOAT3( 2.f, 3.f, 4.f ), XMFLOAT3( 4.9f, 5.9f, 6.9f ) );
            const BoundingOrientedBox box ( XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = box1.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed box-oobox axis-based test9\n",TestName );
                printbb( box1 );
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
                printe("%s: Failed box-frustum test1\n", TestName);
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
                printe( "%s: Failed box-frustum test2 %zu\n",TestName, i );
                printbb( unit );
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
                printe( "%s: Failed box-frustum test3 %zu\n",TestName, i );
                printbb( unit );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }
    }

    {
        const BoundingBox box(XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT3(4.f, 5.f, 6.f));

        {
            BoundingFrustum fr(XMFLOAT3(2.f, 3.f, 4.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 1.f, -1.f, 1.f, -1.f, 0.1f, 1.f);
            ContainmentType ct = box.Contains(fr);
            if (ct != CONTAINS)
            {
                printe("%s: Failed box-frustum test4\n", TestName);
                printbb(box);
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
                printe( "%s: Failed box-frustum test5 %zu\n",TestName, i );
                printbb( box );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }

        static const float test2[][3] = { { 0, 0, 12.f }, { -12.0f, 12.0f, 12.0f }, { 0.0f, 12.0f, 12.0f }, {  12.0f, 12.0f, 12.0f },
                                          { -12.0f, 0.0f, 12.0f }, { 12.0f, 0.0f, 12.0f }, { -12.0f,-12.0f, 12.0f }, {  0.0f,-12.0f, 12.0f }, {  12.0f,-12.0f, 12.0f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr( center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );
            ContainmentType ct = box.Contains( fr );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed box-frustum test6 %zu\n",TestName, i );
                printbb( box );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingBox::Intersects
HRESULT TestB05(LogProxy* pLog)
{
    bool success = true;

    const BoundingBox unit;

    // Box-Sphere
    {
        if ( !unit.Intersects(unit) )
        {
            printe("%s: Box-Sphere unit test failed\n",TestName);
            printbb(unit);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( !unit.Intersects(_small) )
        {
            printe("%s: Box-Sphere small test failed\n",TestName);
            printbb(unit);
            printsh(_small);
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( !unit.Intersects(big) )
        {
            printe("%s: Box-Sphere big test failed\n",TestName);
            printbb(unit);
            printsh(big);
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( unit.Intersects(_far) )
        {
            printe("%s: Box-sphere far test failed\n",TestName);
            printbb(unit);
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
                printe( "%s: Failed Box-Sphere axis-based test1A\n",TestName );
                printbb( unit );
                printsh( t );
                success = false;
            }

            // Just missing should be disjoint
            const BoundingSphere t2( XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f - EPSILON );
            if ( unit.Intersects( t2 ) )
            {
                printe( "%s: Failed Box-Sphere axis-based test1B\n",TestName );
                printbb( unit );
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
                printe( "%s: Failed Box-Sphere axis-based test2\n",TestName );
                printbb( unit );
                printsh( t );
                success = false;
            }
        }
    }

    // Box-Box
    {
        if ( !unit.Intersects(unit) )
        {
            printe("%s: Box-Box unit test failed\n",TestName);
            printbb(unit);
            success = false;
        }

        const BoundingBox _small( XMFLOAT3(0.1f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f) );
        if ( !_small.Intersects(unit) )
        {
            printe("%s: Box-Box small test failed\n",TestName);
            printbb(_small);
            printbb(unit);
            success = false;
        }

        const BoundingBox big( XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(10.f, 10.f, 10.f) );
        if ( !big.Intersects(unit) )
        {
            printe("%s: Box-Box big test failed\n",TestName);
            printbb(big);
            printbb(unit);
            success = false;
        }

        const BoundingBox _far( XMFLOAT3(10.f, -5.f, 4.f), XMFLOAT3(2.f, 2.f, 2.f) );
        if ( _far.Intersects(unit) )
        {
            printe("%s: Box-Box far test failed\n",TestName);
            printbb(_far);
            printbb(unit);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const BoundingBox t( XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3( 1.f, 1.f, 1.f ) );

            // Just-touching should intersect
            if ( !t.Intersects( unit ) )
            {
                printe( "%s: Failed Box-Box axis-based test1A\n",TestName );
                printbb( t );
                printbb(unit);
                success = false;
            }

            // Just missing should be disjoint
            const BoundingBox t2( XMFLOAT3(test[i][0], test[i][1], test[i][2]), XMFLOAT3( 1.f - EPSILON, 1.f - EPSILON, 1.f - EPSILON ) );
            if ( t2.Intersects( unit ) )
            {
                printe( "%s: Failed Box-Box axis-based test1B\n",TestName );
                printbb( t2 );
                printbb(unit);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const BoundingBox t( XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), XMFLOAT3(1.f, 1.f, 1.f) );

            if ( !t.Intersects( unit ) )
            {
                printe( "%s: Failed Box-Box axis-based test2\n",TestName );
                printbb( t );
                printbb(unit);
                success = false;
            }
        }
    }

    // Box-Triangle
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

        static_assert( sizeof(tri_in) == sizeof(tri_out), "TestB05 Box-tri tests" );

        for( uint32_t i=0; i < (sizeof(tri_in)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_in[i].v;
            XMVECTOR t1 = tri_in[i+1].v;
            XMVECTOR t2 = tri_in[i+2].v;
            if ( !unit.Intersects(t0, t1, t2) )
            {
                printe("%s: Triangle-Box test failed (ins %d)\n",TestName, i);
                printbb(unit);
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
                printe("%s: Triangle-Box test failed (outs %d)\n",TestName, i);
                printbb(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    // BoundingBox::Intersects( const BoundingOrientedBox& box ) covered by obox.cpp
    // BoundingBox::Intersects( const BoundingFrustum& fr ) covered by frustum.cpp

    // Box-Plane
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

        static_assert( (sizeof(planes)/sizeof(XMVECTORF32)) == (sizeof(result)/sizeof(PlaneIntersectionType)), "TestB05 Box-Plane tests" );

        for( uint32_t i=0; i < (sizeof(planes)/sizeof(XMVECTORF32)); ++i )
        {
            PlaneIntersectionType p = unit.Intersects( planes[i] );

            if ( p != result[i] )
            {
                printe("%s: Plane-Box test failed ([%d] result %d, expected %d)\n",TestName, i, p, result[i] );
                printbb(unit);
                printxmv( planes[i] );
                success = false;
            }
        }
    }

    // Box-Ray
    {
        float dist;

        const XMVECTORF32 rayA[2] = { { 0.1f, 0.1f, 0.1f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if ( !unit.Intersects( rayA[0], rayA[1], dist ) || (fabs(dist + 1.1f) > EPSILON) )
        {
            printe("%s: Box-Ray test A failed (dist=%f)\n",TestName, dist);
            printbb(unit);
            printxmv( rayA[0] );
            printxmv( rayA[1] );
            success = false;
        }

        const XMVECTORF32 rayB[2] = { { 10.f, 10.f, 10.f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if ( unit.Intersects( rayB[0], rayB[1], dist ) ) // should miss box
        {
            printe("%s: Box-Ray test B failed (dist=%f)\n",TestName, dist);
            printbb(unit);
            printxmv( rayB[0] );
            printxmv( rayB[1] );
            success = false;
        }

        const XMVECTORF32 rayC[2] = { { 10.f, 10.f, 10.f, 0.f }, { -0.577350f, -0.577350f, -0.577350f, 0.f } };
        if ( !unit.Intersects( rayC[0], rayC[1], dist ) || (fabs(dist - 15.588465f) > EPSILON) )
        {
            printe("%s: Box-Ray test C failed (dist=%f)\n",TestName, dist);
            printbb(unit);
            printxmv( rayC[0] );
            printxmv( rayC[1] );
            success = false;
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingBox::ContainedBy
HRESULT TestB06(LogProxy* pLog)
{
    bool success = true;

    const BoundingBox unit;

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
                printe( "%s: Failed box-6planes test1 %zu\n",TestName,i );
                printbb( unit );
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
                printe( "%s: Failed box-6planes test2 %zu\n",TestName,i );
                printbb( unit );
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
// BoundingBox::CreateMerged
HRESULT TestB07(LogProxy* pLog)
{
    bool success = true;

    BoundingBox original;
    BoundingBox::CreateFromPoints( original, XMVectorSet( 1.0f, 2.0f, 3.0f, 0 ), XMVectorSet( 4.0f, 5.0f, 6.0f, 0 ) );

    BoundingBox additional;
    BoundingBox::CreateFromPoints( additional, XMVectorSet( 7.0f, 8.0f, 9.0f, 0 ), XMVectorSet( 10.0f, 11.0f, 12.0f, 0 ) );

    BoundingBox check;
    BoundingBox::CreateFromPoints( check, XMVectorSet( 1.0f, 2.0f, 3.0f, 0 ), XMVectorSet( 10.0f, 11.0f, 12.0f, 0 ) );

    BoundingBox box;
    BoundingBox::CreateMerged( box, original, additional );

    if ( !IsEqual( box, check ) )
    {
        printe("%s: CreateFromSphere failed\n",TestName);
        printbb(box);
        printbb(check);
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingBox::CreateFromSphere
HRESULT TestB08(LogProxy* pLog)
{
    bool success = true;

    const BoundingSphere unit;

    BoundingBox box;
    BoundingBox::CreateFromSphere( box, unit );

    BoundingBox check;
    BoundingBox::CreateFromPoints( check, XMVectorSet( -1.f, -1.f, -1.f, 0 ), XMVectorSet( 1.f, 1.f, 1.f, 0 ) );

    if ( !IsEqual( box, check ) )
    {
        printe("%s: CreateFromSphere failed\n",TestName);
        printbb(box);
        printbb(check);
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// BoundingBox::CreateFromPoints
HRESULT TestB09(LogProxy* pLog)
{
    bool success = true;

    BoundingBox box;

    // CreateFromPoints Min/Max
    {
        XMVECTORF32 v[2] = { { 1.f, 2.f, 3.f, 0 }, { 4.f, 5.f, 6.f, 0  } };
        BoundingBox::CreateFromPoints( box, v[0], v[1] );

        BoundingBox check( XMFLOAT3(2.5f, 3.5f, 4.5f), XMFLOAT3(1.5f, 1.5f, 1.5f) );

        if ( !IsEqual( box, check ) )
        {
            printe("%s: CreateFromPoints (min, max) failed\n",TestName);
            printbb(box);
            printbb(check);
            success = false;
        }

        BoundingBox::CreateFromPoints( box, v[1], v[0] );

        if ( !IsEqual( box, check ) )
        {
            printe("%s: CreateFromPoints (max, min) failed\n",TestName);
            printbb(box);
            printbb(check);
            success = false;
        }
    }

    // CreateFromPoints array
    {
        const float points[] = { 1.1f, 2.2f, 3.3f, 2.1f, 3.2f, 1.3f, 3.1f, 1.2f, 2.3f };

        BoundingBox::CreateFromPoints( box, 3, reinterpret_cast<const XMFLOAT3*>( points ), sizeof(XMFLOAT3) );

        BoundingBox check;
        BoundingBox::CreateFromPoints( check, XMVectorSet( 1.1f, 1.2f, 1.3f, 0 ), XMVectorSet( 3.1f, 3.2f, 3.3f, 0 ) );

        if ( !IsEqual( box, check ) )
        {
            printe("%s: CreateFromPoints (array) failed\n",TestName);
            printbb(box);
            printbb(check);
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

        BoundingBox::CreateFromPoints( box, count, points, sizeof(XMFLOAT3) );

        // Expand a bit to ensure Contains works for all input points on all platforms
        box.Extents.x += EPSILON;
        box.Extents.y += EPSILON;
        box.Extents.z += EPSILON;

        for( uint32_t i=0; i < count; ++i )
        {
            XMVECTOR p = XMLoadFloat3( &points[i] );
            if ( box.Contains( p ) == DISJOINT )
            {
                printe("%s: Box-Point verification test failed (%d)\n",TestName, i);
                printxmv( p );
                printbb(box);
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}
