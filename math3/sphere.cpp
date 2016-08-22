//-------------------------------------------------------------------------------------
// sphere.cpp
//  
// Copyright (c) Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

static const float EPSILON = 0.001f;

inline bool IsEqual( const BoundingSphere& s1, const BoundingSphere& s2 )
{
    return ( ( fabs( s1.Center.x - s2.Center.x ) < EPSILON )
             && ( fabs( s1.Center.y - s2.Center.y ) < EPSILON )
             && ( fabs( s1.Center.z - s2.Center.z ) < EPSILON )
             && ( fabs( s1.Radius - s2.Radius ) < EPSILON ) );
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

#define printtri(v,v0,v1,v2) printe("%s: v0=%f,%f,%f  v1=%f,%f,%f  v2=%f,%f,%f\n", #v, v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z )

#define printct(t) switch(t) { case DISJOINT: printe("%s: DISJOINT\n", #t); break; \
                               case INTERSECTS: printe("%s: INTERSECTS\n", #t); break; \
                               case CONTAINS: printe("%s: CONTAINS\n", #t); break; }

#define printpt(t) switch(t) { case FRONT: printe("%s: FRONT\n", #t); break; \
                               case INTERSECTING: printe("%s: INTERSECTING\n", #t); break; \
                               case BACK: printe("%s: BACK\n", #t); break; }

//-------------------------------------------------------------------------------------
// BoundingSphere
HRESULT TestS01(LogProxy* pLog)
{
    bool success = true;

    // Default constructor
    BoundingSphere sh;
    if ( fabs( sh.Center.x ) > EPSILON
         || fabs( sh.Center.y ) > EPSILON
         || fabs( sh.Center.z ) > EPSILON
         || fabs( sh.Radius - 1.0f ) > EPSILON )
    {
        printe("%s: Default constructor didn't create unit sphere\n", TestName);
        printsh( sh );
        success = false;
    }

    // Vector + Radius constructor
    BoundingSphere shp( XMFLOAT3(1.f, 2.f, 3.f), 4.f );
    if ( fabs( shp.Center.x - 1.f) > EPSILON
         || fabs( shp.Center.y - 2.f) > EPSILON
         || fabs( shp.Center.z - 3.f) > EPSILON
         || fabs( shp.Radius - 4.0f ) > EPSILON )
    {
        printe("%s: Constructor didn't create correct sphere\n", TestName);
        printsh( shp );
        success = false;
    }

    BoundingSphere shp2( XMFLOAT3(1.1f, 2.2f, 3.3f), 4.4f );
    if ( fabs( shp2.Center.x - 1.1f) > EPSILON
         || fabs( shp2.Center.y - 2.2f) > EPSILON
         || fabs( shp2.Center.z - 3.3f) > EPSILON
         || fabs( shp2.Radius - 4.4f ) > EPSILON )
    {
        printe("%s: Constructor didn't create correct sphere (2)\n", TestName);
        printsh( shp2 );
        success = false;
    }

    // Copy constructor
    BoundingSphere shc(shp);
    if ( !IsEqual(shc,shp) )
    {
        printe("%s: Copy constructor failed\n",TestName);
        printsh( shc );
        printsh( shp );
        success = false;
    }
  
    // Test assignment operator
    BoundingSphere shi;

    shi = shp;
    if ( !IsEqual(shi,shp) )
    {
        printe("%s: Assignment operator failed\n",TestName);
        printsh( shi );
        printsh( shp );
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// Sphere::Transform
HRESULT TestS02(LogProxy* pLog)
{
    bool success = true;

    BoundingSphere shu, shv;

    XMMATRIX Mid = XMMatrixIdentity();
    XMVECTOR Rid = XMQuaternionIdentity();
    XMVECTOR Tid = XMVectorZero();

    // Unit sphere
    const BoundingSphere unit;

    unit.Transform( shu, Mid );
    if ( !IsEqual(unit,shu) )
    {
        printe("%s: Unit sphere tranform(1) failed\n",TestName);
        printsh(unit);
        printsh(shu);
        success = false;
    }

    unit.Transform( shv, 1.0f, Rid, Tid );
    if ( !IsEqual(unit,shv) )
    {
        printe("%s: Unit sphere tranform(2) failed\n",TestName);
        printsh(unit);
        printsh(shv);
        success = false;
    }

    // Small sphere (w/ scaling)
    const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );

    XMMATRIX mat = XMMatrixTransformation( g_XMZero, // Scaling origin
                                           Rid, // Scaling orientation
                                           XMVectorSet( 3.f, 3.f, 3.f, 0.f ), // Scaling
                                           g_XMZero, // Rotation origin
                                           Rid, // Rotation
                                           g_XMOne // Translation
                                           );

    XMMATRIX matNonUniform = XMMatrixTransformation( g_XMZero, // Scaling origin
                                                     Rid, // Scaling orientation
                                                     XMVectorSet( 1.f, 2.f, 3.f, 0.f ), // Scaling
                                                     g_XMZero, // Rotation origin
                                                     Rid, // Rotation
                                                     g_XMOne // Translation
                                                    );

    _small.Transform( shu, mat );
    if ( fabs( shu.Center.x - 1.3f) > EPSILON
         || fabs( shu.Center.y - 1.f) > EPSILON
         || fabs( shu.Center.z - 1.0f) > EPSILON
         || fabs( shu.Radius - 1.5f ) > EPSILON )
    {
        printe("%s: Small transform(1) failed\n",TestName);
        printsh(_small);
        printsh(shu);
        success = false;
    }

    _small.Transform( shv, 3.0f, Rid, g_XMOne );
    if ( !IsEqual(shu,shv) )
    {
        printe("%s: Small transform(2) failed\n",TestName);
        printsh(_small);
        printsh(shu);
        printsh(shv);
        success = false;
    }

    // Small sphere (w/ non-uniform scaling)
    _small.Transform( shu, matNonUniform );
    if ( fabs( shu.Center.x - 1.1f) > EPSILON
         || fabs( shu.Center.y - 1.f) > EPSILON
         || fabs( shu.Center.z - 1.f) > EPSILON
         || fabs( shu.Radius - 1.5f ) > EPSILON )
    {
        printe("%s: Small transform(3) failed\n",TestName);
        printsh(_small);
        printsh(shu);
        success = false;
    }

    if ( IsEqual(shu,shv) )
    {
        // should not be equal to the uniform scale version
        printe("%s: Small transform(4) failed\n",TestName);
        printsh(_small);
        printsh(shu);
        printsh(shv);
        success = false;
    }

    // Big sphere (w/ scaling)
    const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );

    big.Transform( shu, mat );
    if ( fabs( shu.Center.x - 4.f) > EPSILON
         || fabs( shu.Center.y - 7.f) > EPSILON
         || fabs( shu.Center.z - 10.f) > EPSILON
         || fabs( shu.Radius - 30.f ) > EPSILON )
    {
        printe("%s: Big transform(1) failed\n",TestName);
        printsh(big);
        printsh(shu);
        success = false;
    }

    big.Transform( shv, 3.0f, Rid, g_XMOne );
    if ( !IsEqual(shu,shv) )
    {
        printe("%s: Big transform(2) failed\n",TestName);
        printsh(big);
        printsh(shu);
        printsh(shv);
        success = false;
    }

    // Big sphere (w/ non-uniform scaling)
    big.Transform( shu, matNonUniform );
    if ( fabs( shu.Center.x - 2.f) > EPSILON
         || fabs( shu.Center.y - 5.f) > EPSILON
         || fabs( shu.Center.z - 10.f) > EPSILON
         || fabs( shu.Radius - 30.f ) > EPSILON )
    {
        printe("%s: Big transform(3) failed\n",TestName);
        printsh(big);
        printsh(shu);
        success = false;
    }

    if ( IsEqual(shu,shv) )
    {
        printe("%s: Big transform(4) failed\n",TestName);
        printsh(big);
        printsh(shu);
        printsh(shv);
        success = false;
    }

    // Small sphere (w/ rotation & scaling)
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
                                            XMVectorSet( 1.f, 2.f, 3.f, 0.f ), // Scaling
                                            g_XMZero, // Rotation origin
                                            rot, // Rotation
                                            g_XMOne // Translation
                                          );

    _small.Transform( shu, mat );
    if ( fabs( shu.Center.x - 1.f) > EPSILON
         || fabs( shu.Center.y - 1.f) > EPSILON
         || fabs( shu.Center.z - 0.7f) > EPSILON
         || fabs( shu.Radius - 1.5f ) > EPSILON )
    {
        printe("%s: Small transform(5) failed\n",TestName);
        printsh(_small);
        printsh(shu);
        success = false;
    }

    _small.Transform( shv, 3.0f, rot, g_XMOne );
    if ( !IsEqual(shu,shv) )
    {
        printe("%s: Small transform(6) failed\n",TestName);
        printsh(_small);
        printsh(shu);
        printsh(shv);
        success = false;
    }

    // Small sphere (w/ rotation & non-uniform scaling)
    _small.Transform( shu, matNonUniform );
    if ( fabs( shu.Center.x - 1.f) > EPSILON
         || fabs( shu.Center.y - 1.f) > EPSILON
         || fabs( shu.Center.z - 0.9f) > EPSILON
         || fabs( shu.Radius - 1.5f ) > EPSILON )
    {
        printe("%s: Small transform(7) failed\n",TestName);
        printsh(_small);
        printsh(shu);
        success = false;
    }

    if ( IsEqual(shu,shv) )
    {
        printe("%s: Small transform(8) failed\n",TestName);
        printsh(_small);
        printsh(shu);
        printsh(shv);
        success = false;
    }

    // Big sphere (w/ rotation & scaling)
    big.Transform( shu, mat );
    if ( fabs( shu.Center.x - 9.99999f) > EPSILON
         || fabs( shu.Center.y - 7.f) > EPSILON
         || fabs( shu.Center.z + 1.99999f) > EPSILON
         || fabs( shu.Radius - 30.f ) > EPSILON )
    {
        printe("%s: Big transform(5) failed\n",TestName);
        printsh(big);
        printsh(shu);
        success = false;
    }

    big.Transform( shv, 3.0f, rot, g_XMOne );
    if ( !IsEqual(shu,shv) )
    {
        printe("%s: Big transform(6) failed\n",TestName);
        printsh(big);
        printsh(shu);
        printsh(shv);
        success = false;
    }

    // Big sphere (w/ rotation & non-scaling)
    big.Transform( shu, matNonUniform );
    if ( fabs( shu.Center.x - 9.99999f) > EPSILON
         || fabs( shu.Center.y - 5.f) > EPSILON
         || fabs( shu.Center.z - 0.f) > EPSILON
         || fabs( shu.Radius - 30.f ) > EPSILON )
    {
        printe("%s: Big transform(7) failed\n",TestName);
        printsh(big);
        printsh(shu);
        success = false;
    }

    if ( IsEqual(shu,shv) )
    {
        printe("%s: Big transform(8) failed\n",TestName);
        printsh(big);
        printsh(shu);
        printsh(shv);
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// Sphere::Contains
HRESULT TestS03(LogProxy* pLog)
{
    bool success = true;

    const BoundingSphere unit;
    ContainmentType c;

    // Sphere-Point tests
    {
        const XMVECTORF32 pnts_in[6] =
        {
            { 0.f, 0.f, 0.f, _Q_NAN },
            { 1.f, 0.f, 0.f, _Q_NAN },
            { 0.f, 1.f, 0.f, _Q_NAN },
            { 0.f, 0.f, 1.f, _Q_NAN },
            { 0.5f, 0.5f, 0.5f, _Q_NAN },
            { -0.5f, -0.5f, -0.5f, _Q_NAN }
        };

        const XMVECTORF32 pnts_out[6] = 
        {
            { 1.f, 1.f, 1.f, _Q_NAN },
            { 1.1f, 0.f, 0.f, _Q_NAN },
            { 10.f, -10.f, -15.f, _Q_NAN },
            { 0, -1.1f, 0.f, _Q_NAN },
            { -20.f, -20.f, -20.f, _Q_NAN },
            { 1.f, 2.f, 3.f, _Q_NAN }
        };

        static_assert( sizeof(pnts_in) == sizeof(pnts_out), "TestS03 Sphere-point tests" );

        for( uint32_t i=0; i < (sizeof(pnts_in)/sizeof(XMVECTORF32)); ++i)
        {
            if ( (c=unit.Contains( pnts_in[i].v )) != CONTAINS )
            {
                printe("%s: Point-Sphere test failed (ins %d)\n",TestName, i);
                printsh(unit);
                printxmv( pnts_in[i].v );
                printct( c );
                success = false;
            }

            if ( (c=unit.Contains( pnts_out[i].v )) != DISJOINT )
            {
                printe("%s: Point-Sphere test failed (outs %d)\n",TestName, i);
                printsh(unit);
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
                printe( "%s: Failed Point-sphere test1 %Iu\n",TestName, i );
                printsh( unit );
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
                printe( "%s: Failed Point-sphere test2 %Iu\n",TestName, i );
                printsh( unit );
                printxmv( point );
                printct( ct );
                success = false;
            }
        }
    }

    // Triangle-sphere tests
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

        static_assert( (sizeof(tri_CONTAINS) == sizeof(tri_INTERSECTS)) && (sizeof(tri_CONTAINS) == sizeof(tri_DISJOINT)), "TestS03 Tri-sphere test" );

        for( uint32_t i=0; i < (sizeof(tri_CONTAINS)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_CONTAINS[i].v;
            XMVECTOR t1 = tri_CONTAINS[i+1].v;
            XMVECTOR t2 = tri_CONTAINS[i+2].v;
            c = unit.Contains(t0, t1, t2);
            if ( c != CONTAINS )
            {
                printe("%s: Triangle-Sphere test failed (CONTAINS %d)\n",TestName, i);
                printct( c );
                printsh(unit);
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
                printe("%s: Triangle-Sphere test failed (INTERSECTS %d)\n",TestName, i);
                printct( c );
                printsh(unit);
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
                printe("%s: Triangle-Sphere test failed (DISJOINT %d)\n",TestName, i);
                printct( c );
                printsh(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    // Sphere-Sphere tests
    {
        if ( (c=unit.Contains(unit)) != CONTAINS )
        {
            printe("%s: Sphere-Sphere unit test failed\n",TestName);
            printsh(unit);
            printct(c);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( (c=unit.Contains(_small)) != CONTAINS )
        {
            printe("%s: Sphere-Sphere small test failed\n",TestName );
            printsh(unit);
            printsh(_small);
            printct( c );
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( (c=unit.Contains(big)) != INTERSECTS )
        {
            printe("%s: Sphere-Sphere big test failed\n",TestName );
            printsh(unit);
            printsh(big);
            printct( c );
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( (c=unit.Contains(_far)) != DISJOINT )
        {
            printe("%s: Sphere-sphere far test failed\n",TestName );
            printsh(unit);
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
            XMVECTOR v = XMVector3Normalize( XMVectorSet( test[i][0], test[i][1], test[i][2], 0 ) );

            v = XMVectorMultiply( v, XMVectorReplicate( 2.5f ) );

            XMFLOAT3 center;
            XMStoreFloat3( &center, v );

            const BoundingSphere sph( center, 1.f );
            ContainmentType ct = unit.Contains( sph );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed sphere-sphere test1 %Iu\n",TestName, i );
                printsh( unit );
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
                printe( "%s: Failed sphere-sphere test2 %Iu\n",TestName, i );
                printsh( unit );
                printsh( sph );
                printct( ct );
                success = false;
            }
        }
    }

    // Sphere-Axis-aligned Box tests
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
            const BoundingBox box( center, XMFLOAT3( 0.5f, 0.5f, 0.5f ) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed sphere-box axis-based test1 %Iu\n",TestName, i );
                printsh( unit );
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
                printe( "%s: Failed sphere-box axis-based test2 %Iu\n",TestName, i );
                printsh( unit );
                printbb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingSphere sph( XMFLOAT3( 0.1f, 0.1f, 0.1f ), 1.f );
            const BoundingBox box( XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT3( 0.5f, 0.5f, 0.5f ) );
            ContainmentType ct = sph.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed sphere-box axis-based test3\n",TestName );
                printsh( sph );
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
                printe( "%s: Failed sphere-box axis-based test4\n",TestName );
                printsh( unit );
                printbb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingSphere sph( XMFLOAT3(0,0,0), 2.f );
            const BoundingBox box( XMFLOAT3(0,0,0), XMFLOAT3( 0.5f, 0.5f, 0.5f ) );
            ContainmentType ct = sph.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed sphere-box axis-based test5\n",TestName );
                printsh( sph );
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
                printe( "%s: Failed sphere-box axis-based test6 %Iu\n",TestName, i );
                printsh( unit );
                printbb( box );
                printct( ct );
                success = false;
            }
        }
    }        

    // Sphere-Oriented Box tests
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
                printe( "%s: Failed sphere-oobox axis-based test1 %Iu\n",TestName, i );
                printsh( unit );
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
                printe( "%s: Failed sphere-obox axis-based test2 %Iu\n",TestName, i );
                printsh( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingSphere sph( XMFLOAT3( 0.1f, 0.1f, 0.1f ), 1.f );
            const BoundingOrientedBox box ( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = sph.Contains( box );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed sphere-oobox axis-based test3\n",TestName );
                printsh( sph );
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
                printe( "%s: Failed sphere-oobox axis-based test4\n",TestName );
                printsh( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }

        {
            const BoundingSphere sph( XMFLOAT3(0,0,0), 2.f );
            const BoundingOrientedBox box ( XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = sph.Contains( box );
            if ( ct != CONTAINS )
            {
                printe( "%s: Failed sphere-oobox axis-based test5\n",TestName );
                printsh( sph );
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
            const BoundingOrientedBox box( center, XMFLOAT3( 0.4f, 0.4f, 0.4f ), XMFLOAT4(0.461940f, 0.191342f, 0.191342f, 0.844623f) );
            ContainmentType ct = unit.Contains( box );
            if ( ct != DISJOINT )
            {
                printe( "%s: Failed sphere-oobox axis-based test6 %Iu\n",TestName, i );
                printsh( unit );
                printobb( box );
                printct( ct );
                success = false;
            }
        }
    }

    // Sphere-Frustum tests
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
            ContainmentType ct = unit.Contains( fr );
            if ( ct != INTERSECTS )
            {
                printe( "%s: Failed sphere-frustum test1 %Iu\n",TestName, i );
                printsh( unit );
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
                printe( "%s: Failed sphere-frustum test2 %Iu\n",TestName, i );
                printsh( unit );
                printfr( fr );
                printct( ct );
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// Sphere::Intersects
HRESULT TestS04(LogProxy* pLog)
{
    bool success = true;

    const BoundingSphere unit;

    // Sphere-Sphere
    {
        if ( !unit.Intersects(unit) )
        {
            printe("%s: Sphere-Sphere unit test failed\n",TestName);
            printsh(unit);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( !unit.Intersects(_small) )
        {
            printe("%s: Sphere-Sphere small test failed\n",TestName);
            printsh(unit);
            printsh(_small);
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( !unit.Intersects(big) )
        {
            printe("%s: Sphere-Sphere big test failed\n",TestName);
            printsh(unit);
            printsh(big);
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( unit.Intersects(_far) )
        {
            printe("%s: Sphere-sphere far test failed\n",TestName);
            printsh(unit);
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
                printe( "%s: Failed Sphere-Sphere axis-based test1A\n",TestName );
                printsh( unit );
                printsh( t );
                success = false;
            }

            // Just missing should be disjoint
            const BoundingSphere t2( XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f - EPSILON );
            if ( unit.Intersects( t2 ) )
            {
                printe( "%s: Failed Sphere-Sphere axis-based test1B\n",TestName );
                printsh( unit );
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
                printe( "%s: Failed Sphere-Sphere axis-based test2\n",TestName );
                printsh( unit );
                printsh( t );
                success = false;
            }
        }
    }

    // Box-sphere
    {
        const BoundingBox ubox;

        if ( !unit.Intersects(ubox) )
        {
            printe("%s: Sphere-Box unit test failed\n",TestName);
            printsh(unit);
            printbb(ubox);
            success = false;
        }

        const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
        if ( !_small.Intersects(ubox) )
        {
            printe("%s: Sphere-Box small test failed\n",TestName);
            printsh(_small);
            printbb(ubox);
            success = false;
        }

        const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
        if ( !big.Intersects(ubox) )
        {
            printe("%s: Sphere-Box big test failed\n",TestName);
            printsh(big);
            printbb(ubox);
            success = false;
        }

        const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
        if ( _far.Intersects(ubox) )
        {
            printe("%s: Sphere-Box far test failed\n",TestName);
            printsh(_far);
            printbb(ubox);
            success = false;
        }

        static const float test[][3] = { { 2.f, 0, 0 }, { -2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 }, { 0, 0, 2.f }, { 0, 0, -2.f } };
        for( size_t i=0; i < sizeof(test)/(sizeof(float)*3); ++i )
        {
            const BoundingSphere t( XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f );

            // Just-touching should intersect
            if ( !t.Intersects( ubox ) )
            {
                printe( "%s: Failed Sphere-Box axis-based test1A\n",TestName );
                printsh( t );
                printbb(ubox);
                success = false;
            }

            // Just missing should be disjoint
            const BoundingSphere t2( XMFLOAT3(test[i][0], test[i][1], test[i][2]), 1.f - EPSILON );
            if ( t2.Intersects( ubox ) )
            {
                printe( "%s: Failed Sphere-Box axis-based test1B\n",TestName );
                printsh( t2 );
                printbb(ubox);
                success = false;
            }
        }

        static const float test2[][3] = { { .5f, 0, 0 }, { -.5f, 0, 0 }, { 0, .5f, 0 }, { 0, -.5f, 0 }, { 0, 0, .5f }, { 0, 0, -.5f } };
        for( size_t i=0; i < sizeof(test2)/(sizeof(float)*3); ++i )
        {
            const BoundingSphere t( XMFLOAT3(test2[i][0], test2[i][1], test2[i][2]), 1.f );

            if ( !t.Intersects( ubox ) )
            {
                printe( "%s: Failed Sphere-Box axis-based test2\n",TestName );
                printsh( t );
                printbb(ubox);
                success = false;
            }
        }
    }

    // BoundingSphere::Intersects( const BoundingOrientedBox& box ) is covered by obox.cpp
    // BoundingSphere::Intersects( const BoundingFrustum& fr ) is covered by frustum.cpp

    // Triangle-Sphere
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

        static_assert( sizeof(tri_in) == sizeof(tri_out), "TestS04 Tri-Sphere test" );

        for( uint32_t i=0; i < (sizeof(tri_in)/sizeof(XMVECTORF32)); i += 3 )
        {
            XMVECTOR t0 = tri_in[i].v;
            XMVECTOR t1 = tri_in[i+1].v;
            XMVECTOR t2 = tri_in[i+2].v;
            if ( !unit.Intersects(t0, t1, t2) )
            {
                printe("%s: Triangle-Sphere test failed (ins %d)\n",TestName, i);
                printsh(unit);
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
                printe("%s: Triangle-Sphere test failed (outs %d)\n",TestName, i);
                printsh(unit);
                printxmv( t0 );
                printxmv( t1 );
                printxmv( t2 );
                success = false;
            }
        }
    }

    // Plane-Sphere
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

        static_assert( (sizeof(planes)/sizeof(XMVECTORF32)) == (sizeof(result)/sizeof(PlaneIntersectionType)), "TestS04 plane-sphere test" );

        for( uint32_t i=0; i < (sizeof(planes)/sizeof(XMVECTORF32)); ++i )
        {
            PlaneIntersectionType p = unit.Intersects( planes[i] );

            if ( p != result[i] )
            {
                printe("%s: Plane-Sphere test failed ([%d] result %d, expected %d)\n",TestName, i, p, result[i] );
                printsh(unit);
                printxmv( planes[i] );
                success = false;
            }
        }
    }

    // Ray-Sphere
    {
        float dist;

        const XMVECTORF32 rayA[2] = { { 0.1f, 0.1f, 0.1f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if ( !unit.Intersects( rayA[0], rayA[1], dist ) || (fabs(dist - 0.889950f) > EPSILON) )
        {
            printe("%s: Sphere-Ray test A failed (dist=%f)\n",TestName, dist);
            printsh(unit);
            printxmv( rayA[0] );
            printxmv( rayA[1] );
            success = false;
        }

        const XMVECTORF32 rayB[2] = { { 10.f, 10.f, 10.f, 0.f }, { 0.f, 0.f, 1.f, 0.f } };
        if ( unit.Intersects( rayB[0], rayB[1], dist ) ) // Should miss box
        {
            printe("%s: Sphere-Ray test B failed (dist=%f)\n",TestName, dist);
            printsh(unit);
            printxmv( rayB[0] );
            printxmv( rayB[1] );
            success = false;
        }

        const XMVECTORF32 rayC[2] = { { 10.f, 10.f, 10.f, 0.f }, { -0.577350f, -0.577350f, -0.577350f, 0.f } };
        if ( !unit.Intersects( rayC[0], rayC[1], dist ) || (fabs(dist - 16.320623f) > EPSILON) )
        {
            printe("%s: Sphere-Ray test C failed (dist=%f)\n",TestName, dist);
            printsh(unit);
            printxmv( rayC[0] );
            printxmv( rayC[1] );
            success = false;
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// Sphere::ContainedBy
HRESULT TestS05(LogProxy* pLog)
{
    bool success = true;

    const BoundingSphere unit;

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
                printe( "%s: Failed sphere-6planes test1 %Iu\n",TestName,i );
                printsh( unit );
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
                printe( "%s: Failed sphere-6planes test2 %Iu\n",TestName,i );
                printsh( unit );
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
};


//-------------------------------------------------------------------------------------
// Sphere::CreateMerged
HRESULT TestS06(LogProxy* pLog)
{
    bool success = true;
    BoundingSphere sht;

    const BoundingSphere unit;
    BoundingSphere::CreateMerged( sht, unit, unit );
    if ( !IsEqual(sht,unit) )
    {
        printe("%s: unit test failed\n",TestName);
        printsh(unit);
        printsh(sht);
        success = false;
    }

    const BoundingSphere _small( XMFLOAT3(0.1f, 0.0f, 0.0f), 0.5f );
    BoundingSphere::CreateMerged( sht, unit, _small );
    if ( !IsEqual(unit,sht) )
    {
        printe("%s: small test failed\n",TestName);
        printsh(unit);
        printsh(_small);
        printsh(sht);
        success = false;
    }

    const BoundingSphere big( XMFLOAT3(1.f, 2.f, 3.f), 10.0f );
    BoundingSphere::CreateMerged( sht, unit, big );
    if ( !IsEqual(sht,big) )
    {
        printe("%s: big test failed\n",TestName);
        printsh(unit);
        printsh(big);
        printsh(sht);
        success = false;
    }

    const BoundingSphere _far( XMFLOAT3(10.f, -5.f, 4.f), 2.f );
    BoundingSphere::CreateMerged( sht, _far, big );

    const BoundingSphere result( XMFLOAT3(2.354666f, 0.946371f, 3.150518f), 11.722761f );
    if ( !IsEqual(sht,result) )
    {
        printe("%s: _far-big test failed\n",TestName);
        printsh(_far);
        printsh(big);
        printsh(result);
        printsh(sht);
        success = false;
    }

    {
        const BoundingSphere sph2( XMFLOAT3( 2.f, 0, 0 ), 1.f );

        BoundingSphere::CreateMerged( sht, unit, sph2 );

        const BoundingSphere result2( XMFLOAT3( 1.f, 0, 0), 2.f );
        if ( !IsEqual( sht, result2 ) )
        {
            printe( "%s: create merge test1 failed\n",TestName );
            printsh( unit );
            printsh( sph2 );
            printsh( sht );
            printsh( result2 );
            success = false;
        }
    }

    {
        const BoundingSphere sph1( XMFLOAT3( 0, 0, 0), 5.f );
        const BoundingSphere sph2( XMFLOAT3( 2.f, 0, 0 ), 1.f );

        BoundingSphere::CreateMerged( sht, sph1, sph2 );

        if ( !IsEqual( sht, sph1 ) )
        {
            printe( "%s: create merge test2 failed\n",TestName );
            printsh( sph1 );
            printsh( sph2 );
            printsh( sht );
            success = false;
        }
    }

    {
        const BoundingSphere sph2( XMFLOAT3( 2.f, 0, 0 ), 5.f );

        BoundingSphere::CreateMerged( sht, unit, sph2 );

        if ( !IsEqual( sht, sph2 ) )
        {
            printe( "%s: create merge test3 failed\n",TestName );
            printsh( unit );
            printsh( sph2 );
            printsh( sht );
            success = false;
        }
    }

    {
        const BoundingSphere sph2( XMFLOAT3( 0, 0, 0 ), 2.f );

        BoundingSphere::CreateMerged( sht, unit, sph2 );

        if ( !IsEqual( sht, sph2 ) )
        {
            printe( "%s: create merge test4 failed\n",TestName );
            printsh( unit );
            printsh( sph2 );
            printsh( sht );
            success = false;
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//------------------------------------------------------------------------------------
// Sphere::CreateFromBoundingBox
HRESULT TestS07(LogProxy* pLog)
{
    bool success = true;
    BoundingSphere sht;

    // Axis-aligned bounding box
    XMVECTOR testmin = XMVectorSet(1.f, 2.f, 3.f, 0);
    XMVECTOR testmax = XMVectorSet(4.f, 5.f, 6.f, 0);

    BoundingBox abox;
    BoundingBox::CreateFromPoints( abox, testmin, testmax );
    BoundingSphere::CreateFromBoundingBox( sht, abox );

    BoundingSphere result;
    XMVECTOR dist = XMVectorSubtract( testmax, testmin );
    XMStoreFloat3( &result.Center, XMVectorAdd( testmin, XMVectorDivide( dist, XMVectorReplicate( 2.f ) ) ) );
    result.Radius = XMVectorGetX( XMVector3Length( dist ) ) * 0.5f;

    if ( !IsEqual(sht,result) )
    {
        printe("%s: aa box test failed\n",TestName);
        printbb(abox);
        printsh(result);
        printsh(sht);
        success = false;
    }

    // Oriented bounding box
    BoundingOrientedBox obox( XMFLOAT3(1.f, 2.f, 3.f), XMFLOAT3(4.f, 5.f, 6.f), XMFLOAT4( 0.f, 0.f, 0.f, 1.f ) );
    BoundingSphere::CreateFromBoundingBox( sht, obox );

    const BoundingSphere result2( XMFLOAT3( 1.f, 2.f, 3.f), 8.774964f );
    if ( !IsEqual(sht,result2) )
    {
        printe("%s: oriented box test failed\n",TestName);
        printobb(obox);
        printsh(result2);
        printsh(sht);
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// Sphere::CreateFromPoints
HRESULT TestS08(LogProxy* pLog)
{
    bool success = true;

    {
        const float points[] = { 0, 0, 0, 1.f, 0, 0 };

        BoundingSphere sht;
        BoundingSphere::CreateFromPoints( sht, 2, reinterpret_cast<const XMFLOAT3*>( points ), sizeof(XMFLOAT3) );

        const BoundingSphere result( XMFLOAT3(0.5f, 0, 0), 0.5f );
        if ( !IsEqual(sht,result) )
        {
            printe("%s: creating from points test1 failed\n",TestName);
            printsh(sht);
            printsh(result);
            success = false;
        }
    }

    {
        const float points[] = { 0, 0, 0, 0.5f, 0, 1.0f };

        BoundingSphere sht;
        BoundingSphere::CreateFromPoints( sht, 2, reinterpret_cast<const XMFLOAT3*>( points ), sizeof(XMFLOAT3) );

        const BoundingSphere result( XMFLOAT3(0.25f, 0, 0.5f), 0.559017f );
        if ( !IsEqual(sht,result) )
        {
            printe("%s: creating from points test2 failed\n",TestName);
            printsh(sht);
            printsh(result);
            success = false;
        }
    }

    {
        const float points[] = { 0, 0, 0, 0.0f, 0.5f, 1.0f };

        BoundingSphere sht;
        BoundingSphere::CreateFromPoints( sht, 2, reinterpret_cast<const XMFLOAT3*>( points ), sizeof(XMFLOAT3) );

        const BoundingSphere result( XMFLOAT3(0, 0.25f, 0.5f), 0.559017f );
        if ( !IsEqual(sht,result) )
        {
            printe("%s: creating from points test3 failed\n",TestName);
            printsh(sht);
            printsh(result);
            success = false;
        }
    }

    {
        float irt2 = 1.f / sqrtf(2.f);
        float irt3 = 1.f / sqrtf(3.f);
        const float points[] = { 0, 1.f, 0,
                                 0, -irt2, -irt2,
                                 -irt3, -irt3, irt3,
                                 irt3, -irt3, irt3 };

        BoundingSphere sht;
        BoundingSphere::CreateFromPoints( sht, 4, reinterpret_cast<const XMFLOAT3*>( points ), sizeof(XMFLOAT3) );

        const BoundingSphere result( XMFLOAT3(-0.0621097758f, 0.01741325f, -0.187598482f), 1.16094756f );
        if ( !IsEqual(sht,result) )
        {
            printe("%s: creating from points test4 failed\n",TestName);
            printsh(sht);
            printsh(result);
            success = false;
        }

        // See that the bound above is tighter than a sphere created from a bounding box of the same points
        BoundingBox box;
        BoundingBox::CreateFromPoints( box, 4, reinterpret_cast<const XMFLOAT3*>( points ), sizeof(XMFLOAT3) );

        BoundingSphere shv;
        BoundingSphere::CreateFromBoundingBox( shv, box );

        if ( sht.Radius > shv.Radius )
        {
            printe("%s: creating from points test4 failed, expected tighter bounds\n",TestName);
            printsh(sht);
            printsh(shv);
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

        BoundingSphere sht;
        BoundingSphere::CreateFromPoints( sht, count, points, sizeof(XMFLOAT3) );

        // Expand a bit to ensure Contains works for all input points on all platforms
        sht.Radius += EPSILON;

        for( uint32_t i=0; i < count; ++i )
        {
            XMVECTOR p = XMLoadFloat3( &points[i] );
            ContainmentType ct = sht.Contains(p);
            if ( ct != CONTAINS )
            {
                printe("%s: Sphere-Point verification test failed - %f %f %f (%d)\n",TestName,
                       points[i].x, points[i].y, points[i].z, i);
                printsh(sht);
                printct(ct);
                success = false;
            }
        }
    }

    return ( success ) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// Sphere::CreateFromFrustum
HRESULT TestS09(LogProxy* pLog)
{
    bool success = true;

    const BoundingFrustum fr( XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f );

    BoundingSphere sht;

    BoundingSphere::CreateFromFrustum( sht, fr );

    const BoundingSphere result( XMFLOAT3(1.f, 1.f, 3.f), 5.656854f);
    if ( !IsEqual(sht,result) )
    {
        printe("%s: Frustum failed\n",TestName);
        printfr(fr);
        printsh(result);
        printsh(sht);
        success = false;
    }

    XMMATRIX matrix = XMMatrixPerspectiveFovRH( XM_PIDIV2 /* 90 degrees */, 1.f, 1.f, 100.f );
    BoundingFrustum fr2;
    BoundingFrustum::CreateFromMatrix( fr2, matrix );

    BoundingSphere::CreateFromFrustum( sht, fr2 );

    const BoundingSphere result2( XMFLOAT3(0, 0, -100.000694f ), 141.422333f );
    if ( !IsEqual(sht,result2) )
    {
        printe("%s: Frustum2 failed\n",TestName);
        printfr(fr2);
        printsh(result2);
        printsh(sht);
        success = false;
    }

    return ( success ) ? S_OK : MATH_FAIL;
}
