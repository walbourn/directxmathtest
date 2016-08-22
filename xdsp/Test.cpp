// Test for XDSP header

#include <stdio.h>
#include <float.h>
#include <memory>
#include <malloc.h>

#include "XDSP.h"

//--------------------------------------------------------------------------------------
typedef DirectX::XMVECTOR XVECTOR;
typedef DirectX::XMVECTORF32 XVEC;

#define TESTEPSILON  0.00001f

inline bool Compare(float a, float b)
{
    if (_isnan(a) && _isnan(b)) return true;
    if (_isnan(a) || _isnan(b)) return false;
    if (!_finite(a) && !_finite(b)) {
        if (_copysign(1.0f, a) == _copysign(1.0f, b)) return true;
        else return false;
    }
    if (!_finite(a) || !_finite(b)) return false;

    if (a == b) return true;
    float f = fabs(b-a);
    if (f <= TESTEPSILON) return true;
    return false;
}

inline bool Compare( const XVEC& a, const XVEC& b )
{
    return ( Compare( a.f[0], b.f[0] ) && Compare( a.f[1], b.f[1] ) && Compare( a.f[2], b.f[2] ) && Compare( a.f[3], b.f[3] ) );
}

inline void print( const XVEC& v )
{
    printf("%f %f %f %f\n", v.f[0], v.f[1], v.f[2], v.f[3] );
}

inline bool Compare( const XVEC* a, const XVEC* b, size_t count ) 
{
    for( size_t i = 0; i < count; ++i )
    {
        if ( !Compare( a[i], b[i] ) )
            return false;
    }

    return true;
}

void print( const XVEC* a, size_t count )
{
    for( size_t i=0; i < count; ++i )
    {
        print( a[i] );
    }
}

struct aligned_deleter { void operator()(void* p) { _aligned_free(p); } };

typedef std::unique_ptr<XVEC, aligned_deleter> ScopedAlignedArrayXVEC;

//--------------------------------------------------------------------------------------
bool FFT4()
{
    using namespace XDSP;

    XVEC rinput[] = { { 1.f, 2.f, 3.f, 4.f }, };
    XVEC iinput[] = { { 5.f, 6.f, 7.f, 8.f }, };

    FFT4( (XMVECTOR*)rinput, (XMVECTOR*)iinput, 1 );

    static const XVEC ranswer[] = { { 10.f, -4.f, -2.f, 0.f } };
    static const XVEC ianswer[] = { { 26.f, 0.f, -2.f, -4.f } };

    if ( !Compare( rinput, ranswer, 1 ) )
    {
        printf("ERROR: Failed comparison FFT4 real\n" );
        print( rinput, 1 );
        return false;
    }

    if ( !Compare( iinput, ianswer, 1 ) )
    {
        printf("ERROR: Failed comparison FFT4 imaginary\n" );
        print( iinput, 1 );
        return false;
    }

    printf("FFT4 passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool FFT8()
{
    using namespace XDSP;

    XVEC rinput[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, };
    XVEC iinput[] = { { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f } };

    FFT8( (XMVECTOR*)rinput, (XMVECTOR*)iinput, 1 );

    static const XVEC ranswer[] = { { 36.f, -13.656855f, -8.f, -5.656854f }, { -4.f, -2.343146f, 0.f, 5.656854f } };
    static const XVEC ianswer[] = { { 100.f, 5.656854f, 0.f, -2.343146f }, { -4.f, -5.656854f, -8.f, -13.656855f } };
    if ( !Compare( rinput, ranswer, 2 ) )
    {
        printf("ERROR: Failed comparison FFT8 real\n" );
        print( rinput, 2 );
        return false;
    }

    if ( !Compare( iinput, ianswer, 2 ) )
    {
        printf("ERROR: Failed comparison FFT8 imaginary\n" );
        print( iinput, 2 );
        return false;
    }

    printf("FFT8 passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool FFT16()
{
    using namespace XDSP;

    XVEC rinput[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f } };
    XVEC iinput[] = { { 1.1f, 2.1f, 3.1f, 4.1f }, { 5.1f, 6.1f, 7.1f, 8.1f }, { 9.1f, 10.1f, 11.1f, 12.1f }, { 13.1f, 14.1f, 15.1f, 16.1f }  };

    FFT16( (XMVECTOR*)rinput, (XMVECTOR*)iinput, 1 );

    static const XVEC ranswer[] = { { 136.f, -16.f, -8.f, 0.f }, { -48.218716f, -13.345429f, -6.408703f, 3.972846f }, { -27.313709f, -11.313709f, -4.686292f, 11.313707f }, { -19.972847f, -9.591298f, -2.654571f, 32.218716f } };
    static const XVEC ianswer[] = { { 137.600006f, 0.f, -8.f, -16.f }, { 32.218716f, -2.654571f, -9.591298f, -19.972847f }, { 11.313708f, -4.686292f, -11.313708f, -27.313709f }, { 3.972846f, -6.408703f, -13.345429f, -48.218716f } };
    if ( !Compare( rinput, ranswer, 4 ) )
    {
        printf("ERROR: Failed comparison FFT16 real\n" );
        print( rinput, 4 );
        return false;
    }

    if ( !Compare( iinput, ianswer, 4 ) )
    {
        printf("ERROR: Failed comparison FFT16 imaginary\n" );
        print( iinput, 4 );
        return false;
    }

    printf("FFT16 passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool FFT()
{
    using namespace XDSP;

    XVEC rinput[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f },
                      { 1.1f, 2.1f, 3.1f, 4.1f }, { 5.1f, 6.1f, 7.1f, 8.1f }, { 9.1f, 10.1f, 11.1f, 12.1f }, { 13.1f, 14.1f, 15.1f, 16.1f } };
    XVEC iinput[] = { { 1.2f, 2.2f, 3.2f, 4.2f }, { 5.2f, 6.2f, 7.2f, 8.2f }, { 9.2f, 10.2f, 11.2f, 12.2f }, { 13.2f, 14.2f, 15.2f, 16.2f }, 
                      { 1.3f, 2.3f, 3.3f, 4.3f }, { 5.3f, 6.3f, 7.3f, 8.3f }, { 9.3f, 10.3f, 11.3f, 12.3f }, { 13.3f, 14.3f, 15.3f, 16.3f } };

    ScopedAlignedArrayXVEC unity( (XVEC*)_aligned_malloc( sizeof(XVEC) * 32, 16 ) );
    FFTInitializeUnityTable( (XMVECTOR*)unity.get(), 32 );

    FFT( (XMVECTOR*)rinput, (XMVECTOR*)iinput, (XMVECTOR*)unity.get(), 32, 1 );

    static const XVEC ranswer[] = { { 273.599976f, -54.627419f, -32.f, -22.627415f }, { -15.999985f, -9.372581f, 0.f, 22.627415f },
                                    { -1.115318f, -0.287085f, -0.182067f, -0.130334f }, { -0.090151f, -0.046550f, 0.021848f, 0.229655f },
                                    { -96.437424f, -39.945694f, -26.690855f, -19.182598f }, { -12.817410f, -5.309145f, 7.945688f, 64.437424f }, 
                                    { -0.429656f, -0.221848f, -0.153450f, -0.109849f }, { -0.069667f, -0.017935f, 0.087085f, 0.915322f } };
    static const XVEC ianswer[] = { { 280.f, 22.627415f, -0.000008f, -9.372585f }, { -16.f, -22.627415f, -31.999992f, -54.627415f },
                                    { 0.915318f, 0.087087f, -0.017932f, -0.069664f }, { -0.109847f, -0.153451f, -0.221851f, -0.429657f } ,
                                    { 64.437431f, 7.945694f, -5.309141f, -12.817406f }, { -19.182600f, -26.690861f, -39.945694f, -96.437431f },
                                    { 0.229655f, 0.021850f, -0.046548f, -0.090149f }, { -0.130333f, -0.182068f, -0.287089f, -1.115320f } };

    if ( !Compare( rinput, ranswer, 8 ) )
    {
        printf("ERROR: Failed comparison FFT (32) real\n" );
        print( rinput, 8 );
        return false;
    }

    if ( !Compare( iinput, ianswer, 8 ) )
    {
        printf("ERROR: Failed comparison FFT (32) imaginary\n" );
        print( iinput, 8 );
        return false;
    }

    printf("FFT (32) passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool FFTUnswizzle()
{
    using namespace XDSP;

#if 1
    //static const XVEC input[] = { { 0.f, 1.f, 2.f, 3.f },{ 4.f, 5.f, 6.f, 7.f },{ 8.f , 9.f, 10.f, 11.f },{ 12.f , 13.f, 14.f, 15.f } };
    static const XVEC input[] = { { 0.f, 8.f, 4.f, 12.f },{ 2.f, 10.f, 6.f, 14.f },{ 1.f, 9.f, 5.f, 13.f },{ 3.f, 11.f, 7.f, 15.f } };

    ScopedAlignedArrayXVEC output((XVEC*)_aligned_malloc(sizeof(XVEC) * 4, 16));

    FFTUnswizzle((XMVECTOR*)output.get(), (XMVECTOR*)input, 4);

    static const XVEC answer[] = { { 0.f, 1.f, 2.f, 3.f },{ 4.f, 5.f, 6.f, 7.f },{ 8.f , 9.f, 10.f, 11.f },{ 12.f , 13.f, 14.f, 15.f } };
    if (!Compare(output.get(), answer, 4))
    {
        printf("ERROR: Failed comparison FFTUnswizzle\n");
        print(output.get(), 4);
        return false;
    }

    
#else
    static const XVEC input[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f },
                                  { 1.1f, 2.1f, 3.1f, 4.1f }, { 5.1f, 6.1f, 7.1f, 8.1f }, { 9.1f, 10.1f, 11.1f, 12.1f }, { 13.1f, 14.1f, 15.1f, 16.1f } };

    ScopedAlignedArrayXVEC output( (XVEC*)_aligned_malloc( sizeof(XVEC) * 8, 16 ) );

    FFTUnswizzle( (XMVECTOR*)output.get(), (XMVECTOR*)input, 5 );
        
    static const XVEC answer[] = { { 1.f, 9.f, 1.1f, 9.1f }, { 2.f, 10.f, 2.1f, 10.1f }, { 3.f, 11.f, 3.1f, 11.1f }, { 4.f, 12.f, 4.1f, 12.1f },
                                   { 5.f, 13.f, 5.1f, 13.1f }, { 6.f, 14.f, 6.1f, 14.1f }, { 7.f, 15.f, 7.1f, 15.1f }, { 8.f, 16.f, 8.1f, 16.1f } };
    if ( !Compare( output.get(), answer, 8 ) )
    {
        printf("ERROR: Failed comparison FFTUnswizzle\n" );
        print( output.get(), 8 );
        return false;
    }
#endif

    printf("FFTUnswizzle passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool FFTPolar()
{
    using namespace XDSP;

    XVEC rinput[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f },
                      { 1.1f, 2.1f, 3.1f, 4.1f }, { 5.1f, 6.1f, 7.1f, 8.1f }, { 9.1f, 10.1f, 11.1f, 12.1f }, { 13.1f, 14.1f, 15.1f, 16.1f } };
    XVEC iinput[] = { { 1.2f, 2.2f, 3.2f, 4.2f }, { 5.2f, 6.2f, 7.2f, 8.2f }, { 9.2f, 10.2f, 11.2f, 12.2f }, { 13.2f, 14.2f, 15.2f, 16.2f }, 
                      { 1.3f, 2.3f, 3.3f, 4.3f }, { 5.3f, 6.3f, 7.3f, 8.3f }, { 9.3f, 10.3f, 11.3f, 12.3f }, { 13.3f, 14.3f, 15.3f, 16.3f } };

    ScopedAlignedArrayXVEC output( (XVEC*)_aligned_malloc( sizeof(XVEC) * 8, 16 ) );

    FFTPolar( (XMVECTOR*)output.get(), (XMVECTOR*)rinput, (XMVECTOR*)iinput, 32 );

    static const XVEC answer[] = { { 0.097628f, 0.185826f, 0.274146f, 0.362500f }, { 0.450867f, 0.539241f, 0.627620f, 0.716000f },
                                   { 0.804383f, 0.892766f, 0.981150f, 1.069535f }, { 1.157921f, 1.246307f, 1.334693f, 1.423080f },
                                   { 0.106434f, 0.194655f, 0.282981f, 0.371336f }, { 0.459704f, 0.548079f, 0.636457f, 0.724838f },
                                   { 0.813221f, 0.901604f, 0.989989f, 1.078374f }, { 1.166760f, 1.255146f, 1.343532f, 1.431919f } };

    if ( !Compare( output.get(), answer, 8 ) )
    {
        printf("ERROR: Failed comparison FFTPolar\n" );
        print( output.get(), 8 );
        return false;
    }

    printf("FFTPolar passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool Deinterleave()
{
    using namespace XDSP;

    static const XVEC input[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f },
                                  { 1.1f, 2.1f, 3.1f, 4.1f }, { 5.1f, 6.1f, 7.1f, 8.1f }, { 9.1f, 10.1f, 11.1f, 12.1f }, { 13.1f, 14.1f, 15.1f, 16.1f } };

    ScopedAlignedArrayXVEC output( (XVEC*)_aligned_malloc( sizeof(XVEC) * 8, 16 ) );

    Deinterleave( (XMVECTOR*)output.get(), (XMVECTOR*)input, 2, 16 );
        
    static const XVEC answer[] = { { 1.f, 3.f, 5.f, 7.f }, { 9.f, 11.f, 13.f, 15.f }, { 1.1f, 3.1f, 5.1f, 7.1f }, { 9.1f, 11.1f, 13.1f, 15.1f },
                                   { 2.f, 4.f, 6.f, 8.f }, { 10.f, 12.f, 14.f, 16.f }, { 2.1f, 4.1f, 6.1f, 8.1f }, { 10.1f, 12.1f, 14.1f, 16.1f } };
    if ( !Compare( output.get(), answer, 8 ) )
    {
        printf("ERROR: Failed comparison Deinterleave\n" );
        print( output.get(), 8 );
        return false;
    }

    printf("Deinterleave passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool Interleave()
{
    using namespace XDSP;

    static const XVEC input[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f },
                                  { 1.1f, 2.1f, 3.1f, 4.1f }, { 5.1f, 6.1f, 7.1f, 8.1f }, { 9.1f, 10.1f, 11.1f, 12.1f }, { 13.1f, 14.1f, 15.1f, 16.1f } };

    ScopedAlignedArrayXVEC output( (XVEC*)_aligned_malloc( sizeof(XVEC) * 8, 16 ) );

    Interleave( (XMVECTOR*)output.get(), (XMVECTOR*)input, 2, 16 );
        
    static const XVEC answer[] = { { 1.f, 1.1f, 2.f, 2.1f }, { 3.f, 3.1f, 4.f, 4.1f }, { 5.f, 5.1f, 6.f, 6.1f }, { 7.f, 7.1f, 8.f, 8.1f },
                                   { 9.f, 9.1f, 10.f, 10.1f }, { 11.f, 11.1f, 12.f, 12.1f }, { 13.f, 13.1f, 14.f, 14.1f }, { 15.f, 15.1f, 16.f, 16.1f } } ;
    if ( !Compare( output.get(), answer, 8 ) )
    {
        printf("ERROR: Failed comparison Interleave\n" );
        print( output.get(), 8 );
        return false;
    }

    printf("Interleave passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool FFTInterleaved()
{
    using namespace XDSP;

    XVEC rinput[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f },
                      { 1.1f, 2.1f, 3.1f, 4.1f }, { 5.1f, 6.1f, 7.1f, 8.1f }, { 9.1f, 10.1f, 11.1f, 12.1f }, { 13.1f, 14.1f, 15.1f, 16.1f },
                      { 1.2f, 2.2f, 3.2f, 4.2f }, { 5.2f, 6.2f, 7.2f, 8.2f }, { 9.2f, 10.2f, 11.2f, 12.2f }, { 13.2f, 14.2f, 15.2f, 16.2f }, 
                      { 1.3f, 2.3f, 3.3f, 4.3f }, { 5.3f, 6.3f, 7.3f, 8.3f }, { 9.3f, 10.3f, 11.3f, 12.3f }, { 13.3f, 14.3f, 15.3f, 16.3f } };
    XVEC iinput[] = { { 1.4f, 2.4f, 3.4f, 4.4f }, { 5.4f, 6.4f, 7.4f, 8.4f }, { 9.4f, 10.4f, 11.4f, 12.4f }, { 13.4f, 14.4f, 15.4f, 16.4f },
                      { 1.5f, 2.5f, 3.5f, 4.5f }, { 5.5f, 6.5f, 7.5f, 8.5f }, { 9.5f, 10.5f, 11.5f, 12.5f }, { 13.5f, 14.5f, 15.5f, 16.5f },
                      { 1.6f, 2.6f, 3.6f, 4.6f }, { 5.6f, 6.6f, 7.6f, 8.6f }, { 9.6f, 10.6f, 11.6f, 12.6f }, { 13.6f, 14.6f, 15.6f, 16.6f }, 
                      { 1.7f, 2.7f, 3.7f, 4.7f }, { 5.7f, 6.7f, 7.7f, 8.7f }, { 9.7f, 10.7f, 11.7f, 12.7f }, { 13.7f, 14.7f, 15.7f, 16.7f } };

    ScopedAlignedArrayXVEC unity( (XVEC*)_aligned_malloc( sizeof(XVEC) * 32, 16 ) );
    FFTInitializeUnityTable( (XMVECTOR*)unity.get(), 32 );

    FFTInterleaved( (XMVECTOR*)rinput, (XMVECTOR*)iinput, (XMVECTOR*)unity.get(), 2, 5 );

    static const XVEC ranswer[] = {
        { 260.799988f, -0.200000f, -0.199998f, -0.200001f },
        { -32.000000f, -0.199999f, -0.200001f, -0.200000f },
        { -31.999992f, -0.200000f, -0.200000f, -0.200000f },
        { -32.000000f, -0.200000f, -0.200001f, -0.200000f },
        { -31.999992f, -0.200000f, -0.200001f, -0.200000f },
        { -32.000000f, -0.200000f, -0.200000f, -0.200000f },
        { -31.999992f, -0.200000f, -0.200001f, -0.200000f },
        { -32.000000f, -0.200001f, -0.199998f, -0.200000f },
        { 292.799988f, -0.200001f, -0.199998f, -0.200001f },
        { -32.000004f, -0.200001f, -0.200000f, -0.200000f },
        { -31.999996f, -0.200001f, -0.200001f, -0.199999f },
        { -32.000004f, -0.200000f, -0.200000f, -0.199999f },
        { -32.000015f, -0.199999f, -0.200000f, -0.200000f },
        { -32.000004f, -0.199999f, -0.200001f, -0.200001f },
        { -31.999996f, -0.200000f, -0.200000f, -0.200001f },
        { -32.000004f, -0.200001f, -0.199998f, -0.200001f } };
    static const XVEC ianswer[] = { 
        { 0.000000f, 2.030633f, 1.005470f, 0.659311f },
        { 77.254837f, 0.374173f, 0.299321f, 0.243701f },
        { 31.999996f, 0.164136f, 0.133636f, 0.106902f },
        { 13.254833f, 0.060670f, 0.039782f, 0.019699f },
        { 0.f, -0.019698f, -0.039782f, -0.060670f },
        { -13.254833f, -0.106902f, -0.133636f, -0.164136f },
        { -31.999996f, -0.243701f, -0.299321f, -0.374173f },
        { -77.254837f, -0.659311f, -1.005470f, -2.030633f} ,
        { 0.f, 2.030634f, 1.005471f, 0.659309f },
        { 77.254837f, 0.374173f, 0.299321f, 0.243699f },
        { 32.000015f, 0.164134f, 0.133636f, 0.106902f },
        { 13.254833f, 0.060668f, 0.039783f, 0.019699f },
        { 0.f, -0.019699f, -0.039783f, -0.060668f },
        { -13.254833f, -0.106902f, -0.133636f, -0.164134f },
        { -32.000015f, -0.243699f, -0.299321f, -0.374173f },
        { -77.254837f, -0.659309f, -1.005471f, -2.030634f } };

    if ( !Compare( rinput, ranswer, 16 ) )
    {
        printf("ERROR: Failed comparison FFTInterleaved real\n" );
        print( rinput, 16 );
        return false;
    }

    if ( !Compare( iinput, ianswer, 16 ) )
    {
        printf("ERROR: Failed comparison FFTInterleaved imaginary\n" );
        print( iinput, 16 );
        return false;
    }

    printf("FFTInterleaved passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
bool IFFTDeinterleaved()
{
    using namespace XDSP;

    XVEC rinput[] = { { 1.f, 2.f, 3.f, 4.f }, { 5.f, 6.f, 7.f, 8.f }, { 9.f, 10.f, 11.f, 12.f }, { 13.f, 14.f, 15.f, 16.f },
                      { 1.1f, 2.1f, 3.1f, 4.1f }, { 5.1f, 6.1f, 7.1f, 8.1f }, { 9.1f, 10.1f, 11.1f, 12.1f }, { 13.1f, 14.1f, 15.1f, 16.1f },
                      { 1.2f, 2.2f, 3.2f, 4.2f }, { 5.2f, 6.2f, 7.2f, 8.2f }, { 9.2f, 10.2f, 11.2f, 12.2f }, { 13.2f, 14.2f, 15.2f, 16.2f }, 
                      { 1.3f, 2.3f, 3.3f, 4.3f }, { 5.3f, 6.3f, 7.3f, 8.3f }, { 9.3f, 10.3f, 11.3f, 12.3f }, { 13.3f, 14.3f, 15.3f, 16.3f } };
    XVEC iinput[] = { { 1.4f, 2.4f, 3.4f, 4.4f }, { 5.4f, 6.4f, 7.4f, 8.4f }, { 9.4f, 10.4f, 11.4f, 12.4f }, { 13.4f, 14.4f, 15.4f, 16.4f },
                      { 1.5f, 2.5f, 3.5f, 4.5f }, { 5.5f, 6.5f, 7.5f, 8.5f }, { 9.5f, 10.5f, 11.5f, 12.5f }, { 13.5f, 14.5f, 15.5f, 16.5f },
                      { 1.6f, 2.6f, 3.6f, 4.6f }, { 5.6f, 6.6f, 7.6f, 8.6f }, { 9.6f, 10.6f, 11.6f, 12.6f }, { 13.6f, 14.6f, 15.6f, 16.6f }, 
                      { 1.7f, 2.7f, 3.7f, 4.7f }, { 5.7f, 6.7f, 7.7f, 8.7f }, { 9.7f, 10.7f, 11.7f, 12.7f }, { 13.7f, 14.7f, 15.7f, 16.7f } };

       
    ScopedAlignedArrayXVEC unity( (XVEC*)_aligned_malloc( sizeof(XVEC) * 32, 16 ) );
    FFTInitializeUnityTable( (XMVECTOR*)unity.get(), 32 );

    IFFTDeinterleaved( (XMVECTOR*)rinput, (XMVECTOR*)iinput, (XMVECTOR*)unity.get(), 2, 5 );

    static const XVEC ranswer[] = {
        { 8.549999f, 8.750000f, 0.028604f, 0.028604f },
        { 2.013670f, 2.013670f, 0.007177f, 0.007177f },
        { 0.707107f, 0.707107f, 0.002721f, 0.002721f },
        { 0.248303f, 0.248303f, 0.000683f, 0.000683f },
        { 0.000000f, 0.000000f, -0.000560f, -0.000560f },
        { -0.165911f, -0.165910f, -0.001455f, -0.001455f },
        { -0.292893f, -0.292893f, -0.002177f, -0.002177f },
        { -0.400544f, -0.400544f, -0.002817f, -0.002817f }, 
        { -0.500000f, -0.500000f, -0.003433f, -0.003433f },
        { -0.599456f, -0.599456f, -0.004073f, -0.004073f },
        { -0.707107f, -0.707107f, -0.004795f, -0.004795f },
        { -0.834090f, -0.834090f, -0.005690f, -0.005690f },
        { -1.000000f, -1.000000f, -0.006933f, -0.006933f },
        { -1.248303f, -1.248303f, -0.008971f, -0.008971f },
        { -1.707107f, -1.707107f, -0.013427f, -0.013427f },
        { -3.013670f, -3.013670f, -0.034854f, -0.034854f } };
    static const XVEC ianswer[] = {
        { 1.4f, 2.4f, 3.4f, 4.4f },
        { 5.4f, 6.4f, 7.4f, 8.4f },
        { 9.4f, 10.4f, 11.4f, 12.4f },
        { 13.4f, 14.4f, 15.4f, 16.4f }, 
        { 1.5f, 2.5f, 3.5f, 4.5f }, 
        { 5.5f, 6.5f, 7.5f, 8.5f },
        { 9.5f, 10.5f, 11.5f, 12.5f },
        { 13.5f, 14.5f, 15.5f, 16.5f },
        { 1.6f, 2.6f, 3.6f, 4.6f },
        { 5.6f, 6.6f, 7.6f, 8.6f },
        { 9.6f, 10.6f, 11.6f, 12.6f },
        { 13.6f, 14.6f, 15.6f, 16.6f },
        { 1.7f, 2.7f, 3.7f, 4.7f },
        { 5.7f, 6.7f, 7.7f, 8.7f },
        { 9.7f, 10.7f, 11.7f, 12.7f },
        { 13.7f, 14.7f, 15.7f, 16.700001f } };

    if ( !Compare( rinput, ranswer, 16 ) )
    {
        printf("ERROR: Failed comparison IFFTDeinterleaved real\n" );
        print( rinput, 16 );
        return false;
    }

    if ( !Compare( iinput, ianswer, 16 ) )
    {
        printf("ERROR: Failed comparison IFFTDeinterleaved imaginary\n" );
        print( iinput, 16 );
        return false;
    }

    printf("IFFTDeinterleaved passed\n");
    return true;
}

//--------------------------------------------------------------------------------------
int main()
{
    if ( !FFT4() )
        return -1;

    if ( !FFT8() )
        return -1;

    if ( !FFT16() )
        return -1;

    if ( !FFT() )
        return -1;

    if ( !FFTUnswizzle() )
        return -1;

    if ( !FFTPolar() )
        return -1;

    if ( !Deinterleave() )
        return -1;

    if ( !Interleave() )
        return -1;

    if ( !FFTInterleaved() )
        return -1;

    if ( !IFFTDeinterleaved() )
        return -1;

    return 0;
}

