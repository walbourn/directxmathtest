#include <stdio.h>

#pragma warning(disable : 4619 4616 4061 4265 4365 4514 4625 4668 4710 4711 4820 5039 5045)
// C4619/4616 #pragma warning warnings
// C4061 enumerator 'X' in switch of enum 'X' is not explicitly handled by a case label
// C4265 class has virtual functions, but destructor is not virtual
// C4365 signed/unsigned mismatch
// C4514 unreferenced inline function has been removed
// C4625 copy constructor was implicitly defined as deleted
// C4668 not defined as a preprocessor macro
// C4710 function not inlined
// C4711 selected for automatic inline expansion
// C4820 padding added after data member
// C5039 pointer or reference to potentially throwing function passed to extern C function under - EHc
// C5045 Spectre mitigation warning

#include <d3d11_1.h>
#pragma comment(lib,"d3d11.lib")

#ifdef USE_DIRECT3D12
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#endif

#include "DirectXSH.h"

#include "DDSTextureLoader.h"

#ifdef USE_DIRECT3D12
#include "DDSTextureLoader12.h"
#endif

#include <wrl/client.h>

//#define GEN_RESULTS
//#define TIMING

#ifndef TIMING
#pragma warning(disable:4189)
#endif

using namespace DirectX;

using Microsoft::WRL::ComPtr;

namespace
{

const float EPSILON = 0.0001f;

const size_t THETA_ITERATIONS = 10;
const size_t PHI_ITERATIONS = 20;

const float g_zeroTolerances[XM_SH_MAXORDER] = {
    // !!! D3DX9 SHMultiply tests fail if this is a zero.  D3DX10 does not have this problem.
    0.000001f,
    0, 0, 0, 0, 0 
};


const float g_bandTolerances[XM_SH_MAXORDER][XM_SH_MAXORDER] = {
    { 0.002f, 0.002f, 0.4f,   0.03f, 0.07f, 0.07f },
    { 0.002f, 0.002f, 0.3f,   0.03f, 0.07f, 0.07f },
    { 0.002f, 0.002f, 0.22f,  0.03f, 0.07f, 0.07f },
    { 0.002f, 0.002f, 0.1f,   0.03f, 0.07f, 0.07f },
    { 0.002f, 0.002f, 0.032f, 0.03f, 0.07f, 0.07f },
    { 0.002f, 0.002f, 0.03f,  0.03f, 0.07f, 0.07f },
};

float shResultA[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
 float shResultB[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
float shResultC[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
float shResultD[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
float shTmp0[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
float shTmp1[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
float shTmp2[XM_SH_MAXORDER*XM_SH_MAXORDER+1];

float shInputA[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
float shInputB[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
float shExpected[XM_SH_MAXORDER*XM_SH_MAXORDER+1];

bool fail = false;

#define Fail() fail = true;

//---------------------------------------------------------------------------------
class Timer
{
public:
    Timer()
    {
        LARGE_INTEGER qwTicksPerSec = { 0 };
        QueryPerformanceFrequency( &qwTicksPerSec );
        m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
        m_llBaseTime = 0;
    }

    void Start()
    {
        LARGE_INTEGER qwTime;
        QueryPerformanceCounter( &qwTime );
        m_llBaseTime = qwTime.QuadPart;
    }

    DWORD Stop()
    {
        LARGE_INTEGER qwTime;
        QueryPerformanceCounter( &qwTime );
        return DWORD( qwTime.QuadPart - m_llBaseTime );
    }

    LONGLONG TicksPerSecond() const { return m_llQPFTicksPerSec; }

private:
    LONGLONG    m_llQPFTicksPerSec;
    LONGLONG    m_llBaseTime;
} g_timer;


//-------------------------------------------------------------------------------------
HRESULT LoadCubemap( _In_ ID3D11Device* device, _In_z_ const WCHAR* filename, _Outptr_ ID3D11Texture2D** texture )
{
    if ( !device || !filename || !texture )
        return E_INVALIDARG;

    ComPtr<ID3D11Resource> res;
    HRESULT hr = CreateDDSTextureFromFile( device, filename, &res, nullptr );
    if ( FAILED(hr) )
        return hr;

    D3D11_RESOURCE_DIMENSION dim;
    res->GetType( &dim );

    if ( dim != D3D11_RESOURCE_DIMENSION_TEXTURE2D )
        return E_FAIL;

    hr = res.Get()->QueryInterface( _uuidof( ID3D11Texture2D ), (void**)texture );
    if ( FAILED(hr) )
        return hr;
        
    D3D11_TEXTURE2D_DESC desc;
    (*texture)->GetDesc( &desc );

    if ( (desc.ArraySize != 6) || !(desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) )
    {
        return E_FAIL;
    }

    return S_OK;
}

//-------------------------------------------------------------------------------------
inline float Luminance(float r, float g, float b) {
    return r*0.2125f + g*0.7154f + b*0.0721f;
}

//-------------------------------------------------------------------------------------
class CDomainIterator
{
public:
    void Setup(float fmin, float fmax, size_t iterations)
    {
        min = fmin;
        max = fmax;

        delta = (max - min) / static_cast<float>(iterations-1);
        val0 = min;

        count0 = iterations;
    }

    float delta;
    float val, val0;
    float min, max;
    size_t count, count0;
};

//-------------------------------------------------------------------------------------
// return true if floating-point value is valid (ie, normal, denormal,
// zero, etc) see float.h.  False otherwise.
//-------------------------------------------------------------------------------------
bool Validatefloat(double d)
{
    switch( _fpclass(d) )
    {
    case _FPCLASS_NN:
    case _FPCLASS_ND:
    case _FPCLASS_NZ:
    case _FPCLASS_PZ:
    case _FPCLASS_PN:
    case _FPCLASS_PD:
        return true;

    default:
        return false;
    }
}

// returns string describing floating point class for d.
const char *GetfloatClass(double d)
{
    switch( _fpclass(d) )
    {
    case _FPCLASS_SNAN:
        return "signaling NaN";
    case _FPCLASS_QNAN:
        return "quiet NaN";
    case _FPCLASS_NINF:
        return "negative infinity";
    case _FPCLASS_NN:
        return "negative normal";
    case _FPCLASS_ND:
        return "negative denormal";
    case _FPCLASS_NZ:
        return "-0";
    case _FPCLASS_PZ:
        return "+0";
    case _FPCLASS_PD:
        return "positive denormal";
    case _FPCLASS_PN:
        return "positive normal";
    case _FPCLASS_PINF:
        return "positive infinity";

    default:
        return "no type listed float.h returned";
    }
}


//-------------------------------------------------------------------------------------
bool Vrfy(float val1, float val2, float fTolerance, const char *fmt, ...)
{
    char msg[1024];
    va_list args;
    va_start(args, fmt);
    _vsnprintf_s(msg, 1024, fmt, args);
    va_end(args);    

    if(fabsf(val1 - val2) > fTolerance) {
        float diff = fabsf(val1 - val2);
        printf("%s val1=%.5f val2=%.5f tol=%.5f, diff=%.5f\n", msg, val1, val2,fTolerance, diff);
        Fail();
        return false;
    }
    else
        return true;
}


//-------------------------------------------------------------------------------------
inline void InitResultData(_Out_writes_(6*6+1) float *shResult)
{
    for (size_t i = 0; i < XM_SH_MAXORDER*XM_SH_MAXORDER+1; ++i)
    {
        shResult[i] = (float)i;
    }
}

inline void CheckResultData(_In_ size_t order, _In_reads_(order*order) const float *shResult)
{
    size_t i = order*order;
    float v = (float)i;
    if (v != shResult[i])
    {
        printf( "ERROR: sh[%zu]: expected value %f != %f", i, v, shResult[i]);
        Fail();
    }
    for (i = 0; i < order*order; i++) {
        if (!Validatefloat(shResult[i])) {
            printf( "ERROR: sh[%zu]: %f w/invalid float class %s", i, shResult[i], GetfloatClass(shResult[i]));
            Fail();
        }
    }
}


//-------------------------------------------------------------------------------------
void VerifySHVectors(_In_ size_t order, _In_reads_(order*order) const float *v1, _In_reads_(order*order) const float *v2,
                     _In_reads_opt_(order) const float *bandTolerances, _In_z_ const char *szMsgLabel)
{
    if (!v1 || !v2 || !bandTolerances || !szMsgLabel) {
        Fail();
        return;
    }
    
    // first ensure our end of vector marker is not overwritten
    size_t i  = order*order;
    size_t m  = 0;
    size_t l  = 0;
    float v = (float)i;
    if (v != v1[i]) {
        printf("%s: sh[%zu]: expected value %f != %f", szMsgLabel, i, v, v1[i]);
        Fail();
    }

    // second ensure that the two results match within epsilon
    i = 0;
    for (l = 0; l < order; l++) {
        for (m = 0; m <= 2*l; m++) {
            char csDesc[1024];
            sprintf_s(csDesc, "%s: sh[%zu]: expected value %f != %f", szMsgLabel, i, v1[i], v2[i]);
            Vrfy(v1[i], v2[i], bandTolerances[l], csDesc);
            i++;
        }
    }
}

void VerifySHVectors(_In_ size_t order, _In_reads_(order*order) const float *v1, _In_reads_(order*order) const float *v2) {
    if (!v1 || !v2) {
        printf("ERROR: VerifySHVectors(%p, %p): input(s) null!",v1, v2);
        Fail();
        return;
    }

    // first ensure our end of vector marker is not overwritten
    size_t i = order*order;
    float v = (float)i;
    if (v != v1[i]) {
        printf("ERROR: sh[%zu]: expected value %f != %f", i, v, v1[i]);
        Fail();
    }

    // second ensure that the two results match within epsilon
    for (i = 0; i < order*order; i++) {
        char csDesc[1024];
        sprintf_s(csDesc,  "ERROR: sh[%zu]: expected value %f != %f", i, v1[i], v2[i] );
        Vrfy(v1[i], v2[i], EPSILON, csDesc );
    }
}

}; // anonymous namespace


//-------------------------------------------------------------------------------------
// EvalDirectionAndRotate
//-------------------------------------------------------------------------------------
void EvalDirectionAndRotate()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("EvalDirectionAndRotate (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);

        XMVECTOR z = XMVectorSet( 0, 0, 1, 0 );
        const float *b = XMSHEvalDirection(shResultB,order,z);
        if (b != shResultB)
        {
            printf("ERROR: ptr returned by %s (%p) != pOut (%p)", "XMSHEvalDirection", b, shResultB);
            Fail();
        }

        CDomainIterator phi, theta;
        theta.Setup(0, XM_PI, THETA_ITERATIONS);
        phi.Setup(0, XM_2PI, PHI_ITERATIONS);

        for (theta.val = theta.val0, theta.count = theta.count0; theta.count > 1; theta.val += theta.delta, theta.count--)
        {
            const float fST = sinf(theta.val);
            const float fCT = cosf(theta.val);
            for (phi.val = phi.val0, phi.count = phi.count0; phi.count > 1; phi.val += phi.delta, phi.count--)
            {
                XMMATRIX matY = XMMatrixRotationY( theta.val );
                XMMATRIX matZ = XMMatrixRotationZ( phi.val );
                XMMATRIX mat = XMMatrixMultiply( matY, matZ );

                XMVECTOR dir = XMVectorSet(cosf(phi.val)*fST,sinf(phi.val)*fST,fCT, 0);
            
                float *a = XMSHEvalDirection(shResultA,order,dir);
                float *c = XMSHRotate(shResultC, order, mat, shResultB);

                if (a != shResultA) {
                    printf("ERROR: ptr returned by %s (%p) != pOut (%p)", "XMSHEvalDirection", a, shResultA);
                    Fail();
                }
                if (c != shResultC) {
                    printf("ERROR: ptr returned by %s (%p) != pOut (%p)", "XMSHRotate", c, shResultC);
                    Fail();
                }

                VerifySHVectors(order, shResultA, shResultC);
            }
        }

        CheckResultData(order, shResultA);
        CheckResultData(order, shResultB);
        CheckResultData(order, shResultC);
    }
}


//-------------------------------------------------------------------------------------
// RotateZ
//-------------------------------------------------------------------------------------
void RotateZ()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("RotateZ (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);

        CDomainIterator phi, theta;
        theta.Setup(0, XM_PI, THETA_ITERATIONS);
        phi.Setup(0, XM_2PI, PHI_ITERATIONS);

        for (theta.val = theta.val0, theta.count = theta.count0; theta.count > 1; theta.val += theta.delta, theta.count--) {
            const float fST = sinf(theta.val);
            const float fCT = cosf(theta.val);
            for (phi.val = phi.val0, phi.count = phi.count0; phi.count > 1; phi.val += phi.delta, phi.count--) {
                XMVECTOR dir = XMVectorSet(cosf(phi.val)*fST,sinf(phi.val)*fST,fCT,0);
                const float *a = XMSHEvalDirection(shResultA,order,dir);

                XMVECTOR z = XMVectorSet(fST, 0, fCT, 0);
                const float *b = XMSHEvalDirection(shResultB,order,z);

                const float *c = XMSHRotateZ(shResultC, order, phi.val, shResultB);

                if (a != shResultA) {
                    printf("ERROR: ptr returned by %s (%p) != pOut (%p)", "XMSHEvalDirection", a, shResultA);
                    Fail();
                }
                if (b != shResultB) {
                    printf("ERROR: ptr returned by %s (%p) != pOut (%p)", "XMSHEvalDirection", b, shResultB);
                    Fail();
                }
                if (c != shResultC) {
                    printf("ERROR: ptr returned by %s (%p) != pOut (%p)", "XMSHRotateZ", c, shResultC);
                    Fail();
                }

                VerifySHVectors(order,shResultA, shResultC);
            }
        }

        CheckResultData(order, shResultA);
        CheckResultData(order, shResultB);
        CheckResultData(order, shResultC);
    }
}


//-------------------------------------------------------------------------------------
// Add
//-------------------------------------------------------------------------------------
void Add()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("Add (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);
        InitResultData(shExpected);

        const float *c = XMSHAdd(shResultC, order, shResultA, shResultB);
        if (c != shResultC) {
            printf("ERROR: ptr returned by %s (%p) != pOut (%p)", "XMSHAdd", c, shResultC);
            Fail();
        }

        for( size_t i=0; i < order*order; ++i )
            shExpected[i] = shResultA[i] + shResultB[i];

        VerifySHVectors(order, shResultC, shExpected );

        CheckResultData(order, shResultC);
    }
}


//-------------------------------------------------------------------------------------
// Scale
//-------------------------------------------------------------------------------------
void Scale()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("Scale (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shExpected);

        const float *c = XMSHScale(shResultB, order, shResultA, XM_PI);
        if (c != shResultB) {
            printf("ERROR: ptr returned by %s (%p) != pOut (%p)", "XMSHScale", c, shResultB);
            Fail();
        }

        for( size_t i=0; i < order*order; ++i )
            shExpected[i] = shResultA[i] * XM_PI;

        VerifySHVectors(order, shResultB, shExpected );

        CheckResultData(order, shResultB);
    }
}


//-------------------------------------------------------------------------------------
// Dot
//-------------------------------------------------------------------------------------
void Dot()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("Dot (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);

        float r = XMSHDot(order, shResultA, shResultB);

        float e = 0.f;
        for( size_t i=0; i < order*order; ++i )
            e += shResultA[i] * shResultB[i];

        char csDesc[1024];
        sprintf_s(csDesc, "ERROR: sh[%zu]: expected value %f != %f", order, e, r);

        Vrfy( r, e, EPSILON, csDesc );
    }
}


//-------------------------------------------------------------------------------------
// EvalDirectionalLight
//-------------------------------------------------------------------------------------
void EvalDirectionalLight()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("EvalDirectionalLight (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);

        // we'll use these to generate an independent result
        InitResultData(shTmp0);
        InitResultData(shTmp1);
        InitResultData(shTmp2);

        XMVECTOR z = XMVectorSet(0, 0, 1, 0);
        const float r = Luminance(1, 0, 0);
        const float g = -Luminance(0, 1, 0); // compulsory negative value
        const float b = Luminance(0, 0, 1);
        XMVECTOR clr = XMVectorSet(r, g, b, 0);

        XMVECTOR clr1 = XMVectorSet(1, 0, 0, 0);
        g_timer.Start();
        bool res = XMSHEvalDirectionalLight(order, z, clr1, shTmp0, nullptr, nullptr);
        DWORD dur = g_timer.Stop();
        if ( !res ) 
        {
            printf("ERROR: XMSHEvalDirectionalLight() failed!\n");
            Fail();
        }
        CheckResultData(order, shTmp0);
#ifdef TIMING
        printf("TIMING: XMSHEvalDirectionalLight order %zu took %d ticks\n", order, dur );
#endif

        // use theta and phi to a try a sphere of dirs
        CDomainIterator phi, theta;
        theta.Setup(0, XM_PI, THETA_ITERATIONS);
        phi.Setup(0, XM_2PI, PHI_ITERATIONS);

        for (theta.val = theta.val0, theta.count = theta.count0; theta.count > 1; theta.val += theta.delta, theta.count--) {
            const float fST = sinf(theta.val);
            const float fCT = cosf(theta.val);
            for (phi.val = phi.val0, phi.count = phi.count0; phi.count > 1; phi.val += phi.delta, phi.count--) {
                XMMATRIX matY = XMMatrixRotationY(theta.val);
                XMMATRIX matZ = XMMatrixRotationZ(phi.val);
                XMMATRIX mat = XMMatrixMultiply(matY, matZ);
                XMVECTOR dir = XMVectorSet(cosf(phi.val)*fST,sinf(phi.val)*fST,fCT, 0);

                // use rotate to get the final result idependently
                const float *t1 = XMSHRotate(shTmp1, order, mat, shTmp0);
                if (t1 != shTmp1) {
                    printf("ptr returned by %s (%p) != pOut (%p)", "XMSHRotate", t1, shTmp1);
                    Fail();
                }
                CheckResultData(order, shTmp1);

                if ( !XMSHEvalDirectionalLight(order, dir, clr, shResultA, shResultB, shResultC))
                {
                    printf("ERROR: XMSHEvalDirectionalLight() failed!\n");
                    Fail();
                }
            
                CheckResultData(order, shResultA);
                XMSHScale(shTmp2, order, shTmp1, r);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultA, shTmp2);

                CheckResultData(order, shResultB);
                XMSHScale(shTmp2, order, shTmp1, g);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultB, shTmp2);

                CheckResultData(order, shResultC);
                XMSHScale(shTmp2, order, shTmp1, b);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultC, shTmp2);
            }
        }
    }
}


//-------------------------------------------------------------------------------------
// EvalSphericalLight
//-------------------------------------------------------------------------------------
void EvalSphericalLight()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("EvalSphericalLight (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);

        // we'll use these to generate an independent result
        InitResultData(shTmp0);
        InitResultData(shTmp1);
        InitResultData(shTmp2);

        XMVECTOR z = XMVectorSet(0, 0, 1, 0);
        const float r = Luminance(1, 0, 0);
        const float g = -Luminance(0, 1, 0); // compulsory negative value
        const float b = Luminance(0, 0, 1);

        XMVECTOR clr = XMVectorSet(r, g, b, 0);

        // !!! Could vary the radius here
        const float radius = XM_PI/8.0f;

        // !!! Might could factor out scale here and divide the nergy of
        // this result by sqrt(scale) or cuberoot(scale) to ensure scale
        // is accomodated for in the result.  Depends on the falloff of
        // energy over distance.
        const float scale  = XM_PI;

        XMVECTOR tz = XMVectorScale( z, scale );

        XMVECTOR clr1 = XMVectorSet(1, 0, 0, 0);
        g_timer.Start();
        bool res = XMSHEvalSphericalLight(order, tz, radius, clr1, shTmp0, nullptr, nullptr);
        DWORD dur = g_timer.Stop();
        if ( !res )
        {
            printf("ERROR: XMSHEvalSphericalLight() failed!\n");
            Fail();
        }
        CheckResultData(order, shTmp0);
#ifdef TIMING
        printf("TIMING: XMSHEvalSphericalLight order %zu took %d ticks\n", order, dur );
#endif

        // use theta and phi to a try a sphere of dirs
        CDomainIterator phi, theta;
        theta.Setup(0, XM_PI, THETA_ITERATIONS);
        phi.Setup(0, XM_2PI, PHI_ITERATIONS);

        for (theta.val = theta.val0, theta.count = theta.count0; theta.count > 1; theta.val += theta.delta, theta.count--) {
            const float fST = sinf(theta.val);
            const float fCT = cosf(theta.val);
            for (phi.val = phi.val0, phi.count = phi.count0; phi.count > 1; phi.val += phi.delta, phi.count--) {
                XMMATRIX matY = XMMatrixRotationY(theta.val);
                XMMATRIX matZ = XMMatrixRotationZ(phi.val);
                XMMATRIX mat = XMMatrixMultiply(matY, matZ);
                XMVECTOR dir = XMVectorSet(cosf(phi.val)*fST,sinf(phi.val)*fST,fCT, 0);

                // use rotate to get the final result idependently
                const float *t1 = XMSHRotate(shTmp1, order, mat, shTmp0);
                if (t1 != shTmp1)
{
                    printf("ptr returned by %s (%p) != pOut (%p)", "XMSHRotate", t1, shTmp1);
                    Fail();
                }
                CheckResultData(order, shTmp1);

                XMVECTOR tdir = XMVectorScale( dir, scale );

                if ( !XMSHEvalSphericalLight(order, tdir, radius, clr, shResultA, shResultB, shResultC) )
                {
                    printf("ERROR: XMSHEvalSphericalLight() failed!\n");
                    Fail();
                }
            
                CheckResultData(order, shResultA);
                XMSHScale(shTmp2, order, shTmp1, r);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultA, shTmp2);

                CheckResultData(order, shResultB);
                XMSHScale(shTmp2, order, shTmp1, g);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultB, shTmp2);

                CheckResultData(order, shResultC);
                XMSHScale(shTmp2, order, shTmp1, b);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultC, shTmp2);
            }
        }
    }
}


//-------------------------------------------------------------------------------------
// EvalConeLight
//-------------------------------------------------------------------------------------
void EvalConeLight()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("EvalConeLight (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);

        // we'll use these to generate an independent result
        InitResultData(shTmp0);
        InitResultData(shTmp1);
        InitResultData(shTmp2);

        XMVECTOR z = XMVectorSet(0, 0, 1, 0);
        const float r = Luminance(1, 0, 0);
        const float g = -Luminance(0, 1, 0); // compulsory negative value
        const float b = Luminance(0, 0, 1);

        XMVECTOR clr = XMVectorSet(r, g, b, 0);

        // !!! Could vary the radius here
        const float radius = XM_PI/8.0f;

        XMVECTOR clr1 = XMVectorSet(1, 0, 0, 0);
        g_timer.Start();
        bool res = XMSHEvalConeLight(order, z, radius, clr1, shTmp0, nullptr, nullptr);
        DWORD dur = g_timer.Stop();
        if ( !res )
        {
            printf("ERROR: XMSHEvalConeLight() failed!\n");
            Fail();
        }
        CheckResultData(order, shTmp0);
#ifdef TIMING
        printf("TIMING: XMSHEvalConeLight order %zu took %d ticks\n", order, dur );
#endif

        // use theta and phi to a try a sphere of dirs
        CDomainIterator phi, theta;
        theta.Setup(0, XM_PI, THETA_ITERATIONS);
        phi.Setup(0, XM_2PI, PHI_ITERATIONS);

        for (theta.val = theta.val0, theta.count = theta.count0; theta.count > 1; theta.val += theta.delta, theta.count--) {
            const float fST = sinf(theta.val);
            const float fCT = cosf(theta.val);
            for (phi.val = phi.val0, phi.count = phi.count0; phi.count > 1; phi.val += phi.delta, phi.count--) {
                XMMATRIX matY = XMMatrixRotationY(theta.val);
                XMMATRIX matZ = XMMatrixRotationZ(phi.val);
                XMMATRIX mat = XMMatrixMultiply(matY, matZ);
                XMVECTOR dir = XMVectorSet(cosf(phi.val)*fST,sinf(phi.val)*fST,fCT, 0);

                // use rotate to get the final result idependently
                const float *t1 = XMSHRotate(shTmp1, order, mat, shTmp0);
                if (t1 != shTmp1) {
                    printf("ptr returned by %s (%p) != pOut (%p)", "XMSHRotate", t1, shTmp1);
                    Fail();
                }
                CheckResultData(order, shTmp1);

                if ( !XMSHEvalConeLight(order, dir, radius, clr, shResultA, shResultB, shResultC))
                {
                    printf("ERROR: XMSHEvalConeLight() failed!\n");
                    Fail();
                }
            
                CheckResultData(order, shResultA);
                XMSHScale(shTmp2, order, shTmp1, r);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultA, shTmp2);

                CheckResultData(order, shResultB);
                XMSHScale(shTmp2, order, shTmp1, g);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultB, shTmp2);

                CheckResultData(order, shResultC);
                XMSHScale(shTmp2, order, shTmp1, b);
                CheckResultData(order, shTmp2);
                VerifySHVectors(order,shResultC, shTmp2);
            }
        }
    }
}


//-------------------------------------------------------------------------------------
// EvalHemisphereLight
//-------------------------------------------------------------------------------------
void EvalHemisphereLight()
{
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("EvalHemisphereLight (%zu)\n", order );
        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);

        // we'll use these to generate an independent result
        InitResultData(shTmp0);
        InitResultData(shTmp1);
        InitResultData(shTmp2);

        XMVECTOR z = XMVectorSet(0, 0, 1, 0);
        XMVECTOR top = XMVectorSet(2.2f, 1.0f, 2.2f, 0);
        XMVECTOR bot = XMVectorSet(1.0f, 2.2f, 1.0f, 0);
    
        // hold the prerotated result
        float shR[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
        float shG[XM_SH_MAXORDER*XM_SH_MAXORDER+1];
        float shB[XM_SH_MAXORDER*XM_SH_MAXORDER+1];

        InitResultData(shR);
        InitResultData(shG);
        InitResultData(shB);

        g_timer.Start();
        bool res = XMSHEvalHemisphereLight(order, z, top, bot, shR, shG, shB );
        DWORD dur = g_timer.Stop();
        if ( !res )
        {
            printf("ERROR: XMSHEvalHemisphereLight() failed!\n");
            Fail();
        }
        CheckResultData(order, shTmp0);
#ifdef TIMING
        printf("TIMING: XMSHEvalHemisphereLight order %zu took %d ticks\n", order, dur );
#endif

        // use theta and phi to a try a sphere of dirs
        CDomainIterator phi, theta;
        theta.Setup(0, XM_PI, THETA_ITERATIONS);
        phi.Setup(0, XM_2PI, PHI_ITERATIONS);

        for (theta.val = theta.val0, theta.count = theta.count0; theta.count > 1; theta.val += theta.delta, theta.count--) {
            const float fST = sinf(theta.val);
            const float fCT = cosf(theta.val);
            for (phi.val = phi.val0, phi.count = phi.count0; phi.count > 1; phi.val += phi.delta, phi.count--) {
                XMMATRIX matY = XMMatrixRotationY(theta.val);
                XMMATRIX matZ = XMMatrixRotationZ(phi.val);
                XMMATRIX mat = XMMatrixMultiply(matY, matZ);
                XMVECTOR dir = XMVectorSet(cosf(phi.val)*fST,sinf(phi.val)*fST,fCT, 0);

                // use rotate to get the final result idependently
                const float *t0 = XMSHRotate(shTmp0, order, mat, shR);
                const float *t1 = XMSHRotate(shTmp1, order, mat, shG);
                const float *t2 = XMSHRotate(shTmp2, order, mat, shB);
                if (t0 != shTmp0) {
                    printf("ptr returned by %s (%p) != pOut (%p)", "XMSHRotate", t0, shTmp0);
                    Fail();
                }
                CheckResultData(order, shTmp0);
                if (t1 != shTmp1) {
                    printf("ptr returned by %s (%p) != pOut (%p)", "XMSHRotate", t1, shTmp1);
                    Fail();
                }
                CheckResultData(order, shTmp1);
                if (t2 != shTmp2) {
                    printf("ptr returned by %s (%p) != pOut (%p)", "XMSHRotate", t2, shTmp2);
                    Fail();
                }
                CheckResultData(order, shTmp2);

                if ( !XMSHEvalHemisphereLight(order, dir, top, bot, shResultA, shResultB, shResultC))
                {
                    printf("ERROR: XMSHEvalHemisphereLight() failed!\n");
                    Fail();
                }
            
                CheckResultData(order,shResultA);
                VerifySHVectors(order,shResultA, shTmp0);

                CheckResultData(order,shResultB);
                VerifySHVectors(order,shResultB, shTmp1);

                CheckResultData(order,shResultC);
                VerifySHVectors(order,shResultC, shTmp2);
            }
        }
    }
}


//-------------------------------------------------------------------------------------
// Multiply
//-------------------------------------------------------------------------------------
void Multiply()
{
    static float g_shFuncA[6*6] = {
        1.772185f,
        0.000000f,
        -0.000000f,
        -1.534604f,
        0.000000f,
        0.000000f,
        0.000005f,
        0.000000f,
        0.000001f,
        0.000000f,
        0.000000f,
        0.000000f,
        -0.000000f,
        -0.358906f,
        0.000000f,
        0.463330f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000001f,
        0.000000f,
        0.000000f,
        -0.000000f,
        -0.000001f,
        -0.000000f,
        0.000000f,
        0.000000f,
        -0.000000f,
        -0.000000f,
        -0.000000f,
        -0.177846f,
        0.000000f,
        0.192091f,
        -0.000000f,
        -0.257701f,
    };
    static float g_shFuncB[6*6] = {
        1.772178f,
        -1.534644f,
        0.000001f,
        -0.000001f,
        -0.000000f,
        0.000000f,
        0.000002f,
        -0.000000f,
        0.000002f,
        -0.463320f,
        -0.000000f,
        -0.358908f,
        0.000002f,
        -0.000001f,
        0.000000f,
        0.000001f,
        -0.000000f,
        -0.000001f,
        0.000001f,
        0.000003f,
        0.000013f,
        -0.000000f,
        -0.000000f,
        -0.000000f,
        -0.000003f,
        -0.257709f,
        0.000000f,
        -0.192091f,
        0.000000f,
        -0.177848f,
        0.000007f,
        -0.000001f,
        -0.000000f,
        0.000000f,
        -0.000001f,
        -0.000003f,
    };
    static float g_shFuncAB[6*6] = {
        0.886075f,
        -0.767353f,
        -0.000000f,
        -0.767345f,
        0.728228f,
        -0.000000f,
        -0.000002f,
        0.000000f,
        0.000001f,
        -0.231670f,
        0.000001f,
        -0.179451f,
        -0.000001f,
        -0.179452f,
        -0.000000f,
        0.231670f,
        -0.000005f,
        -0.000000f,
        0.252273f,
        0.000002f,
        -0.000001f,
        0.000001f,
        -0.000000f,
        0.000000f,
        -0.000001f,
        -0.128855f,
        0.000000f,
        -0.096046f,
        0.000001f,
        -0.088922f,
        -0.000001f,
        -0.088923f,
        0.000000f,
        0.096045f,
        -0.000000f,
        -0.128859f,
    };

    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("Multiply (%zu)\n", order );

        // add end of vector markers
        InitResultData(shResultA); InitResultData(shResultB); InitResultData(shResultC); InitResultData(shResultD);
        InitResultData(shInputA); InitResultData(shInputB); InitResultData(shExpected); 

        // and init with the test values
        for (size_t i = 0; i < order*order; i++)
        {
            shInputA[i]   = g_shFuncA[i];
            shInputB[i]   = g_shFuncB[i];
            shExpected[i] = g_shFuncAB[i];
        }
    
        // check return value and sanctity of inputs tests
        g_timer.Start();
        float* pOut = XMSHMultiply(shResultC, order, shResultA, shResultB);
        DWORD dur = g_timer.Stop();
        if (pOut != shResultC) {
            printf("ptr returned by XMSHMultiply (%p) != (%p)", pOut, shResultC);
            Fail();
        }
#ifdef TIMING
        printf("TIMING: XMSHMultiply order %zu took %d ticks\n", order, dur );
#endif

        CheckResultData(order,shResultA); CheckResultData(order,shResultB); CheckResultData(order,shResultC);
        VerifySHVectors(order,shResultA, shResultB);

        //
        // commutativity test
        pOut = XMSHMultiply(shResultD, order, shResultB, shResultA);
        if (pOut != shResultD) {
            printf("ptr returned by XMSHMultiply (%p) != (%p)", pOut, shResultD);
            Fail();
        }

        CheckResultData(order,shResultA); CheckResultData(order,shResultB); CheckResultData(order,shResultD);
        VerifySHVectors(order,shResultC, shResultD);

        //
        // Multiply SH functions A and B and see if they match our golden results
        pOut = XMSHMultiply(shResultC, order, shInputA, shInputB);
        if (pOut != shResultC) {
            printf("ptr returned by XMSHMultiply (%p) != (%p)", pOut, shResultC);
            Fail();
        }
        CheckResultData(order,shInputA); CheckResultData(order,shInputB); CheckResultData(order,shResultC);
        VerifySHVectors(order,shExpected, shResultC, g_bandTolerances[order-1], "XMSHMultiply C = A*B");

        //
        // Multiply SH functions B and A and see if they match the A*B results
        pOut = XMSHMultiply(shResultD, order, shInputB, shInputA);
        if (pOut != shResultD) {
            printf("ptr returned by XMSHMultiply (%p) != (%p)", pOut, shResultD);
            Fail();
        }
        CheckResultData(order,shInputA); CheckResultData(order,shInputB); CheckResultData(order,shResultD);
        VerifySHVectors(order,shResultD, shResultC, g_zeroTolerances, "XMSHMultiply C = B*A == A*B");

        //
        // Rotate and then multiply SH functions A and B and B and A and see if they match our golden results
        //
        CDomainIterator phi, theta;
        theta.Setup(0, XM_PI, THETA_ITERATIONS);
        phi.Setup(0, XM_2PI, PHI_ITERATIONS);

        for (theta.val = theta.val0, theta.count = theta.count0; theta.count > 1; theta.val += theta.delta, theta.count--) {
            for (phi.val = phi.val0, phi.count = phi.count0; phi.count > 1; phi.val += phi.delta, phi.count--) {
                XMMATRIX matY = XMMatrixRotationY(theta.val);
                XMMATRIX matZ = XMMatrixRotationZ(phi.val);
                XMMATRIX mat = XMMatrixMultiply(matY, matZ);

                // rotate the g_shFuncA,B functions and store in shInputA,B
                XMSHRotate(shInputA, order, mat, g_shFuncA);
                XMSHRotate(shInputB, order, mat, g_shFuncB);

                // rotate the g_shFuncAB function and store in shExpected 
                XMSHRotate(shExpected, order, mat, g_shFuncAB);

                //
                // Multiply rotated SH functions A and B and see if they match our golden results
                pOut = XMSHMultiply(shResultC, order, shInputA, shInputB);
                if (pOut != shResultC) {
                    printf("ptr returned by XMSHMultiply (%p) != (%p)", pOut, shResultC);
                    Fail();
                }
                CheckResultData(order,shInputA); CheckResultData(order,shInputB); CheckResultData(order,shResultC);
                char csDesc[1024];
                sprintf_s(csDesc, "XMSHMultiply C = A*B: Rotate(Y, Z) = (%f,%f)", theta.val, phi.val);
                VerifySHVectors(order,shExpected, shResultC, g_bandTolerances[order-1], csDesc);

                //
                // Multiply SH functions B and A and see if they match our golden results
                pOut = XMSHMultiply(shResultD, order, shInputB, shInputA);
                if (pOut != shResultD) {
                    printf("ptr returned by XMSHMultiply (%p) != (%p)", pOut, shResultD);
                    Fail();
                }
                CheckResultData(order,shInputA); CheckResultData(order,shInputB); CheckResultData(order,shResultD);
                sprintf_s(csDesc, "SHMultiply C = B*A == A*B: Rotate(Y, Z) = (%f,%f)", theta.val, phi.val);
                VerifySHVectors(order,shResultC, shResultD, g_zeroTolerances, csDesc);
            }
        }
    }
}


//-------------------------------------------------------------------------------------
// ProjectCubeMap
//-------------------------------------------------------------------------------------
#ifdef GEN_RESULTS
#include "D:\DXSDK\include\d3dx11.h"
#ifdef _M_AMD64
#pragma comment(lib,"d:\\DXSDK\\lib\\x64\\d3dx11.lib")
#else
#pragma comment(lib,"d:\\DXSDK\\lib\\x86\\d3dx11.lib")
#endif

void dump_coeffs( _In_ FILE* f, _In_ size_t order, _In_reads_(order*order) const float *v, _In_z_ const char* varname )
{
    fprintf( f, "static float g_%s[%zu*%zu] = {\n    ", varname, order, order );

    for( size_t i = 0; i < order*order; ++i )
    {
        fprintf( f, "%lff, ",  v[i] );
    }

    fprintf( f, "};\n" );
}
#else
#include "coeff.cpp"
#endif

void ProjectCubeMap()
{
    // Create Direct3D 11 Device
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    D3D_FEATURE_LEVEL lvl[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };

    HRESULT hr = D3D11CreateDevice( NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, lvl, 2, D3D11_SDK_VERSION, &device, NULL, &context );
    if ( FAILED(hr) )
    {
        printf("Failed creating a WARP device for test!\n (%x)", static_cast<unsigned int>(hr));
        Fail();
        return;
    }

    // Load cubemap 0
    ComPtr<ID3D11Texture2D> cubemap0;
    hr = LoadCubemap( device.Get(), L"media\\shxyfunc.dds", &cubemap0 );
    if ( FAILED(hr) )
    {
        printf("ERROR: Failed loading shxyfunc.dds cubemap (%08X)!\n", static_cast<unsigned int>(hr));
        Fail();
        return;
    }

    // Load cubemap 1
    ComPtr<ID3D11Texture2D> cubemap1;
    hr = LoadCubemap( device.Get(), L"media\\shxyfunc_mip.dds", &cubemap1 );
    if ( FAILED(hr) )
    {
        printf("ERROR: Failed loading shxyfunc_mip.dds cubemap (%08X)!\n", static_cast<unsigned int>(hr));
        Fail();
        return;
    }

    // Load lightprobes
    ComPtr<ID3D11Texture2D> lightProbes[5];
    static const WCHAR* lpnames[5] = { L"media\\galileo_cross.dds",
                                       L"media\\grace_cross.dds",
                                       L"media\\rnl_cross.dds",
                                       L"media\\stpeters_cross.dds",
                                       L"media\\uffizi_cross.dds" };
    static const char* varnames[5] = { "galileo", "grace", "rnl", "stpeters", "uffizi" };

    for( size_t j=0; j < _countof(lpnames); ++j )
    {
        hr = LoadCubemap( device.Get(), lpnames[j], &lightProbes[j] );
        if ( FAILED(hr) )
        {
            printf("ERROR: Failed loading %S cubemap! (%08X)\n", lpnames[j], static_cast<unsigned int>(hr));
            Fail();
            return;
        }
    }

#ifdef GEN_RESULTS
    // Use D3DX to create 'golden' results

    FILE* fp = nullptr;
    if ( _wfopen_s( &fp, L"coeff.cpp", L"w" ) == 0 )
    {
        // shxyfunc.dds
        fprintf( fp, "\n\n// media\\shxyfunc.dds\n" );
        g_timer.Start();
        hr = D3DX11SHProjectCubeMap( context.Get(), 6, cubemap0.Get(), shResultA, shResultB, shResultC );
        DWORD dur = g_timer.Stop();
        if ( FAILED(hr) )
        {
            printf("ERROR: Failed D3DX11SHProjectCubeMap on cubemap0 order 6 (%08X)\n", hr );
        }
#ifdef TIMING
        printf("TIMING: D3DX11SHProjectCubeMap took %d ticks\n", dur );
#endif

        dump_coeffs( fp, 6, shResultA, "shxyfuncR" );
        dump_coeffs( fp, 6, shResultB, "shxyfuncG" );
        dump_coeffs( fp, 6, shResultC, "shxyfuncB" );

        fprintf( fp, "\n\n// media\\shxyfunc_mip.dds\n" );
        hr = D3DX11SHProjectCubeMap( context.Get(), 6, cubemap1.Get(), shResultA, shResultB, shResultC );
        if ( FAILED(hr) )
        {
            printf("ERROR: Failed D3DX11SHProjectCubeMap on cubemap1 order 6 (%08X)\n", hr );
        }

        dump_coeffs( fp, 6, shResultA, "shxyfuncMipR" );
        dump_coeffs( fp, 6, shResultB, "shxyfuncMipG" );
        dump_coeffs( fp, 6, shResultC, "shxyfuncMipB" );

        // light probes
        for( size_t j=0; j < _countof(lpnames); ++j )
        {
            fprintf( fp, "\n\n// %S\n", lpnames[j] );
            hr = D3DX11SHProjectCubeMap( context.Get(), 6, lightProbes[j].Get(), shResultA, shResultB, shResultC );
            if ( FAILED(hr) )
            {
                printf("ERROR: Failed D3DX11SHProjectCubeMap on %S order 6 (%08X)\n", lpnames[j], hr );
            }

            char desc[32];
            sprintf_s( desc, "%sR", varnames[j] );
            dump_coeffs( fp, 6, shResultA, desc );

            sprintf_s( desc, "%sG", varnames[j] );
            dump_coeffs( fp, 6, shResultB, desc );

            sprintf_s( desc, "%sB", varnames[j] );
            dump_coeffs( fp, 6, shResultC, desc );
        }

        fclose(fp);
    }
#else

#ifdef TIMING
    {
        g_timer.Start();
        HRESULT hr = SHProjectCubeMap( context.Get(), 6, cubemap0.Get(), shResultA, shResultB, shResultC );
        DWORD dur = g_timer.Stop();
        if ( FAILED(hr) )
        {
            printf("SHProjectCubemap failed for shxyfunc.dds order 6!\n");
            Fail();
        }
        printf("TIMING: XMSHProjectCubeMap took %d ticks\n", dur );
    }
#endif
    
    // Test SH project cubemap function
    for( size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order )
    {
        printf("ProjectCubeMap (%zu)\n", order );

        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);
        InitResultData(shResultD);

        if ( FAILED(SHProjectCubeMap( context.Get(), order, cubemap0.Get(), shResultA, shResultB, shResultC )) )
        {
            printf("SHProjectCubemap failed for shxyfunc.dds!\n");
            Fail();
            continue;
        }
        CheckResultData(order,shResultA); CheckResultData(order,shResultB); CheckResultData(order,shResultC);
        VerifySHVectors(order,shResultA, g_shxyfuncR);
        VerifySHVectors(order,shResultB, g_shxyfuncG);
        VerifySHVectors(order,shResultC, g_shxyfuncB);

        if ( FAILED(SHProjectCubeMap( context.Get(), order, cubemap1.Get(), shResultD, nullptr, nullptr )) )
        {
            printf("SHProjectCubemap failed for shxyfunc_mip.dds!\n");
            Fail();
            continue;
        }
        CheckResultData(order,shResultA); CheckResultData(order,shResultB); CheckResultData(order,shResultC); CheckResultData(order,shResultD);
        VerifySHVectors(order,shResultD, shResultA);
        VerifySHVectors(order,shResultD, g_shxyfuncMipR);

        if ( FAILED(SHProjectCubeMap( context.Get(), order, cubemap1.Get(), shResultA, shResultD, nullptr )) )
        {
            printf("SHProjectCubemap failed for shxyfunc_mip.dds!\n");
            Fail();
            continue;
        }
        CheckResultData(order,shResultD);
        VerifySHVectors(order,shResultD,shResultB);
        VerifySHVectors(order,shResultA, g_shxyfuncMipR);
        VerifySHVectors(order,shResultD, g_shxyfuncMipG);

        if ( FAILED(SHProjectCubeMap( context.Get(), order, cubemap1.Get(), shResultA, shResultB, shResultD )) )
        {
            printf("SHProjectCubemap failed for shxyfunc_mip.dds!\n");
            Fail();
            continue;
        }
        CheckResultData(order,shResultD);
        VerifySHVectors(order,shResultD, shResultC);
        VerifySHVectors(order,shResultA, g_shxyfuncMipR);
        VerifySHVectors(order,shResultB, g_shxyfuncMipG);
        VerifySHVectors(order,shResultC, g_shxyfuncMipB);

        if ( FAILED(SHProjectCubeMap( context.Get(), order, cubemap1.Get(), nullptr, shResultB, shResultD )) )
        {
            printf("SHProjectCubemap failed for shxyfunc_mip.dds!\n");
            Fail();
            continue;
        }

        CheckResultData(order,shResultD);
        VerifySHVectors(order,shResultD, shResultC);
        VerifySHVectors(order,shResultB, g_shxyfuncMipG);
        VerifySHVectors(order,shResultD, g_shxyfuncMipB);

        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);
        InitResultData(shResultD);

        // Check five lightprobes
        for( size_t j=0; j < _countof(lpnames); ++j )
        {
            if ( FAILED(SHProjectCubeMap( context.Get(), order, lightProbes[j].Get(), shResultA, shResultB, shResultC )) )
            {
                printf("SHProjectCubemap failed for %S!\n", lpnames[j]);
                Fail();
                continue;
            }

            CheckResultData(order,shResultA);
            CheckResultData(order,shResultB);
            CheckResultData(order,shResultC);

            switch (j)
            {
            case 0:
                VerifySHVectors(order,shResultA,g_galileoR);
                VerifySHVectors(order,shResultB,g_galileoG);
                VerifySHVectors(order,shResultC,g_galileoB);
                break;
            case 1:
                VerifySHVectors(order,shResultA,g_graceR);
                VerifySHVectors(order,shResultB,g_graceG);
                VerifySHVectors(order,shResultC,g_graceB);
                break;
            case 2:
                VerifySHVectors(order,shResultA,g_rnlR);
                VerifySHVectors(order,shResultB,g_rnlG);
                VerifySHVectors(order,shResultC,g_rnlB);
                break;
            case 3:
                VerifySHVectors(order,shResultA,g_stpetersR);
                VerifySHVectors(order,shResultB,g_stpetersG);
                VerifySHVectors(order,shResultC,g_stpetersB);
                break;
            case 4:
                VerifySHVectors(order,shResultA,g_uffiziR);
                VerifySHVectors(order,shResultB,g_uffiziG);
                VerifySHVectors(order,shResultC,g_uffiziB);
                break;
            }
        }
    }
#endif
}


#ifdef USE_DIRECT3D12
void ProjectCubeMap12()
{
    // Create Direct3D 12 Device
    ComPtr<ID3D12Device> device;

    ComPtr<IDXGIFactory4> dxgiFactory;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(hr))
    {
        printf("Failed creating DXGI for test\n (%x)", static_cast<unsigned int>(hr));
        Fail();
        return;
    }

    ComPtr<IDXGIAdapter1> warpAdapter;
    hr = dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
    if (FAILED(hr))
    {
        printf("Failed getting a WARP adapter for test!\n (%x)", static_cast<unsigned int>(hr));
        Fail();
        return;
    }

    hr = D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    if (FAILED(hr))
    {
        printf("Failed creating a WARP device for test!\n (%x)", static_cast<unsigned int>(hr));
        Fail();
        return;
    }

    // Load cubemap 0
    ComPtr<ID3D12Resource> cubemap0;
    std::unique_ptr<uint8_t[]> ddsData;
    std::vector<D3D12_SUBRESOURCE_DATA> sub;
    bool isCubeMap;
    hr = LoadDDSTextureFromFile(device.Get(), L"media\\shxyfunc.dds", &cubemap0, ddsData, sub, 0, nullptr, &isCubeMap);
    if (FAILED(hr)
        || !isCubeMap
        || sub.size() != 6)
    {
        printf("ERROR: Failed loading shxyfunc.dds cubemap (%08X)!\n", static_cast<unsigned int>(hr));
        Fail();
        return;
    }

    // Load lightprobes
    ComPtr<ID3D12Resource> lightProbes[5];
    std::unique_ptr<uint8_t[]> ddsDataLP[5];
    std::vector<D3D12_SUBRESOURCE_DATA> subLP[5];
    static const WCHAR* lpnames[5] = { L"media\\galileo_cross.dds",
        L"media\\grace_cross.dds",
        L"media\\rnl_cross.dds",
        L"media\\stpeters_cross.dds",
        L"media\\uffizi_cross.dds" };
    static const char* varnames[5] = { "galileo", "grace", "rnl", "stpeters", "uffizi" };

    for (size_t j = 0; j < _countof(lpnames); ++j)
    {
        hr = LoadDDSTextureFromFile(device.Get(), lpnames[j], &lightProbes[j], ddsDataLP[j], subLP[j], 0, nullptr, &isCubeMap);
        if (FAILED(hr)
            || !isCubeMap
            || subLP[j].size() != 6)
        {
            printf("ERROR: Failed loading %S cubemap! (%08X)\n", lpnames[j], static_cast<unsigned int>(hr));
            Fail();
            return;
        }
    }

    // Test SH project cubemap function
    for (size_t order = XM_SH_MINORDER; order <= XM_SH_MAXORDER; ++order)
    {
        printf("ProjectCubeMap [DX12] (%zu)\n", order);

        InitResultData(shResultA);
        InitResultData(shResultB);
        InitResultData(shResultC);
        InitResultData(shResultD);

        auto desc = cubemap0->GetDesc();

        if (FAILED(SHProjectCubeMap(order, desc, sub.data(), shResultA, shResultB, shResultC)))
        {
            printf("SHProjectCubemap failed for shxyfunc.dds!\n");
            Fail();
            continue;
        }
        CheckResultData(order, shResultA); CheckResultData(order, shResultB); CheckResultData(order, shResultC);
        VerifySHVectors(order, shResultA, g_shxyfuncR);
        VerifySHVectors(order, shResultB, g_shxyfuncG);
        VerifySHVectors(order, shResultC, g_shxyfuncB);

        // Check five lightprobes
        for (size_t j = 0; j < _countof(lpnames); ++j)
        {
            auto descLP = lightProbes[j]->GetDesc();

            if (descLP.MipLevels != 1)
            {
                printf("Test cubemap should not have mipmap for %S!\n", lpnames[j]);
                Fail();
                continue;
            }

            if (FAILED(SHProjectCubeMap(order, descLP, subLP[j].data(), shResultA, shResultB, shResultC)))
            {
                printf("SHProjectCubemap failed for %S!\n", lpnames[j]);
                Fail();
                continue;
            }

            CheckResultData(order, shResultA);
            CheckResultData(order, shResultB);
            CheckResultData(order, shResultC);

            switch (j)
            {
            case 0:
                VerifySHVectors(order, shResultA, g_galileoR);
                VerifySHVectors(order, shResultB, g_galileoG);
                VerifySHVectors(order, shResultC, g_galileoB);
                break;
            case 1:
                VerifySHVectors(order, shResultA, g_graceR);
                VerifySHVectors(order, shResultB, g_graceG);
                VerifySHVectors(order, shResultC, g_graceB);
                break;
            case 2:
                VerifySHVectors(order, shResultA, g_rnlR);
                VerifySHVectors(order, shResultB, g_rnlG);
                VerifySHVectors(order, shResultC, g_rnlB);
                break;
            case 3:
                VerifySHVectors(order, shResultA, g_stpetersR);
                VerifySHVectors(order, shResultB, g_stpetersG);
                VerifySHVectors(order, shResultC, g_stpetersB);
                break;
            case 4:
                VerifySHVectors(order, shResultA, g_uffiziR);
                VerifySHVectors(order, shResultB, g_uffiziG);
                VerifySHVectors(order, shResultC, g_uffiziB);
                break;
            }
        }
    }
}
#endif

//-------------------------------------------------------------------------------------
// Entry Point
//-------------------------------------------------------------------------------------
int main()
{
    printf( "Spherical Harmonics Math Test (version %03d)\n\n", DIRECTX_SHMATH_VERSION );

    EvalDirectionAndRotate();
    RotateZ();
    Add();
    Scale();
    Dot();
    EvalDirectionalLight();
    EvalSphericalLight();
    EvalConeLight();
    EvalHemisphereLight();
    Multiply();
    ProjectCubeMap();

#ifdef USE_DIRECT3D12
    ProjectCubeMap12();
#endif

    if ( fail )
    {
        printf("***FAILED***\n");
        return 1;
    }

    return 0;
}