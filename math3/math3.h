//-------------------------------------------------------------------------------------
// math3.h - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#pragma once

#ifdef _MSC_VER
// Off by default warnings
#pragma warning(disable : 4619 4616 4061 4365 4514 4555 4571 4625 4626 4640 4668 4710 4711 4774 4820 5026 5027 5039 5045 5264)
// C4619/4616 #pragma warning warnings
// C4061 enumerator 'X' in switch of enum 'X' is not explicitly handled by a case label
// C4365 signed/unsigned mismatch
// C4514 'function' : unreferenced inline function has been removed
// C4555 expression has no effect
// C4571 behavior change
// C4625 copy constructor was implicitly defined as deleted
// C4626 assignment operator was implicitly defined as deleted
// C4640 construction of local static object is not thread-safe
// C4668 not defined as a preprocessor macro
// C4710 function not inlined
// C4711 selected for automatic inline expansion
// C4774 format string expected in argument 3 is not a string literal
// C4820 padding added after data member
// C5026 move constructor was implicitly defined as deleted
// C5027 move assignment operator was implicitly defined as deleted
// C5039 pointer or reference to potentially throwing function passed to extern C function under - EHc
// C5045 Spectre mitigation warning
// C5264 'const' variable is not used

#pragma warning(disable: 26812)
// 26812: Prefer 'enum class' over 'enum' (Enum.3).

#if !defined(_M_FP_FAST)
#pragma warning(disable : 4738)
// C4738 storing 32-bit float result in memory, possible loss of performance
#endif
#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wswitch-default"
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wc++98-compat-local-type-template-args"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wfloat-equal"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wundefined-reinterpret-cast"
#pragma clang diagnostic ignored "-Wunused-macros"
#endif

#ifdef __INTEL_COMPILER
#pragma warning(disable : 2557)
// warning #2557: comparison between signed and unsigned operands
#endif

#ifndef _MATH3_H_INCLUDED_
#define _MATH3_H_INCLUDED_

#if defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64) || defined(__i386__) || defined(__x86_64__) || defined(__arm__) || defined(__aarch64__)

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4005)
#endif
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef _WIN32
#include <Windows.h>
#else
#include <cstdint>
#include <functional>
#include <utility>

typedef long HRESULT;
typedef void* PVOID;
typedef void* LPVOID;
typedef int BOOL;
typedef float FLOAT;

#define S_OK ((HRESULT)0L)
#define E_FAIL ((HRESULT)0x80004005L)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define __cdecl
#define __stdcall

#define TRUE 1u
#define FALSE 0u

#define _wcsnicmp wcsncasecmp
#endif

#define print printf
#define DATAPATH ""
static const int XB = 0;
static const int PC = 1;

using APITEST_FUNC = HRESULT(*)(const char* TestName);
#define LogProxy const char
#else
#define NOD3D
#include <xtl.h>

#define print DebugPrint
#define DATAPATH "d:\\"
static const int XB = 1;
static const int PC = 0;

#endif

#ifndef _WIN32
#define memcpy_s(d,ds,s,c) memcpy(d,s,c)
#define sprintf_s(a,b) sprintf(a,b);
#endif

#if (__cplusplus >= 201703L) || defined(_MSC_VER)
#define STD_SIZE(arr) std::size(arr)
#else
#define STD_SIZE(arr) sizeof(arr)/sizeof(arr[0])
#endif

#ifndef _MSC_VER
#include <cstdio>

#define fscanf_s(a,b,...) fscanf(a,b,__VA_ARGS__)
#define fscanf_str(a,b,c,d) fscanf(a,b,c)
#else
#define fscanf_str(a,b,c,d) fscanf_s(a,b,c,d)
#endif

//make random behaviour identical for all the compilers
#define XM_RAND_MAX (0x7fff)
#define XM_RAND() (rand()%(XM_RAND_MAX+1))

#ifdef BUILD_FOR_HARNESS
#include "h2.h"

extern char* g_File;
extern int g_Line;
extern char* g_FunctionName;
extern int g_Status;
extern LogProxy* g_Log;

using APITEST_FUNC = HRESULT(*)(LogProxy* log);
extern unsigned seed;
extern unsigned testMode; // bvt, regression, or stress
extern LONG doabort;

char* GetFunctionName(char* func);
void DoLog(const char* pc, ...);

class Math3Test : public ITestObject
{
public:
    APITEST_FUNC testFunct;
    char* testName;

public:
    virtual HRESULT SetParametersFromFile(char* fileName)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT SetParametersFromString(char* xmlString)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT SetParameter(char* name, char* value)
    {
        if (_stricmp(name, "TESTMODE") == 0)
        {
            if (_stricmp(value, "BVT") == 0)
            {
                testMode = TESTMODE_BVT;
            }
            else if (_stricmp(value, "DAILY") == 0)
            {
                testMode = TESTMODE_DAILY_REGRESSION;
            }
            else if (_stricmp(value, "STRESS") == 0)
            {
                testMode = TESTMODE_STRESS;
            }
        }
        return S_OK;
    }
    virtual HRESULT GetParameter(char* name, char** value)
    {
        return E_NOTIMPL;
    }

public:
    virtual HRESULT StartTest(LogProxy* log)
    {
        InterlockedSet(&doabort, 0);

        log->SetOwner("jgould");
        log->SetName("Math3");
        log->SetArea("XTL");
        log->SetSubArea("Math");

        log->SetComponent(testName);
        log->StartVariation(testName);
        HRESULT hr = testFunct(log);

        if (!g_File)
            g_File = "math3.h";
        if (!g_FunctionName)
            g_FunctionName = "StartTest";

        if (TESTMODE_STRESS != testMode)
        {
            log->Debug(FILEINFO, ALL_DEBUG_ZONES, "%s %s", testName, SUCCEEDED(hr) ? "passed" : "failed");
        }

        if (hr == S_OK)
        {
            log->Pass(FILEINFO, "%s passed", testName);
        }
        else
        {
            log->Failure(FILEINFO, hr, "%s failed", testName);
        }

        log->EndVariation();
        return hr;
    }
    virtual HRESULT Abort(void)
    {
        InterlockedSet(&doabort, 1);
        return S_OK;
    }
};

#define TestName ""
#define printi g_File = __FILE__; g_Line = __LINE__; g_FunctionName = GetFunctionName(__FUNCTION__);if(!g_FunctionName)g_FunctionName=__FUNCTION__; g_Status = 0; g_Log = pLog; DoLog
#define printe g_File = __FILE__; g_Line = __LINE__; g_FunctionName = GetFunctionName(__FUNCTION__);if(!g_FunctionName)g_FunctionName=__FUNCTION__; g_Status = 1; g_Log = pLog; DoLog
#define dqi(q) printi("%s: %f,%f,%f,%f\n",#q,XMVectorGetX(q),XMVectorGetY(q),XMVectorGetZ(q),XMVectorGetW(q))
#define dqe(q) printe("%s: %f,%f,%f,%f\n",#q,XMVectorGetX(q),XMVectorGetY(q),XMVectorGetZ(q),XMVectorGetW(q))
#define dqestruct(q) printe("%s: %f,%f,%f,%f\n",#q,q.x,q.y,q.z,q.w)


#else
using APITEST_FUNC = HRESULT(*)(const char* TestName);
#define LogProxy const char
extern "C" void __cdecl DebugPrint(const char*, ...);

#define pLog TestName
#define printi blahblah
#define printe print
#define dqi(q) printi("%s(%ld): %f,%f,%f,%f\n",#q,long(__LINE__),XMVectorGetX(q),XMVectorGetY(q),XMVectorGetZ(q),XMVectorGetW(q))
#define dqe(q) printe("%s(%ld): %f,%f,%f,%f\n",#q,long(__LINE__),XMVectorGetX(q),XMVectorGetY(q),XMVectorGetZ(q),XMVectorGetW(q))
#define dqestruct(q) printe("%s(%ld): %f,%f,%f,%f\n",#q,long(__LINE__),q.x,q.y,q.z,q.w)
#endif //def BUILD_FOR_HARNESS


struct APIFUNCT
{
    APITEST_FUNC funct;
    const char* name;
};

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <cstring>
#include <iterator>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wformat-pedantic"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

using XMVECTORI = DirectX::XMVECTORU32;

#ifdef __GNUC__
#if defined(__i386__) || defined(__x86_64__)
#include <x86intrin.h>
#endif
#else
#include <intrin.h>
#endif

#include <cmath>

//extern "C" { extern HRESULT __stdcall MapDrive(char cDriveLetter, char* pszPartition); }

#define MAXTESTS 1000

#define Pi 3.1415926535897932384626f

const float UC_START = -2.f * Pi;
const float UC_END = 2.f * Pi;
const float UC_STEP = (2.0f * Pi) / 360.f;

enum TESTRETVAL { MATH_PASS = S_OK, MATH_FAIL = -1, FATAL = -2, NODATA = 3, NOTEST = 4 };

//comparisons that are equal return 0 (EXACT)
//comparisons that are within TESTEPSILON return 1 (WITHINEPSILON)
//comparisons that are within TEST2EPSILON return 2 (WITHIN2EPSILON)
//...
//comparisons that are within 100%/4096 return 5 (WITHIN4096)
//...
//comparisons that are within 25% return 10 (CLOSE)
//comparisons that are beyond that, or involve a mismatched NAN or INF return WAYOFF.
enum COMPARISON {
    EXACT, WITHINEPSILON, WITHIN2EPSILON,
    WITHIN10EPSILON, WITHIN100EPSILON, WITHIN4096,
    WITHINBIGEPSILON, WITHINBIGGEREPSILON, WITHINHUGEEPSILON, WITHIN1_256,
    WITHIN1_64, WITHIN1_16, WITHIN1_8, CLOSE, WAYOFF
};

#define TESTEPSILON  1.192092896e-7f
#define TEST10EPSILON  1.192092896e-6f
#define TEST100EPSILON  1.192092896e-6f
#define TEST2EPSILON .00000023841859f
#define TESTBIGEPSILON .001f
#define TESTBIGGEREPSILON .0025f
#define TESTHUGEEPSILON .01f

COMPARISON Compare(float a, float b);
using TESTFUNC = TESTRETVAL(*)(const char*);

extern /*__declspec(thread)*/ char bRunTest[MAXTESTS];
extern /*__declspec(thread)*/ APIFUNCT tests[MAXTESTS];
//extern const char *tests[MAXTESTS];
void AssignTests(void);


COMPARISON Compare(float a, float b);
COMPARISON CompareBYTE(uint8_t a, uint8_t b);
COMPARISON CompareXMVECTOR(DirectX::XMVECTOR a, DirectX::XMVECTOR b, int NumElements);
COMPARISON CompareXMCOLOR(DirectX::PackedVector::XMCOLOR    a, DirectX::PackedVector::XMCOLOR    b);
COMPARISON CompareXMFLOAT2(DirectX::XMFLOAT2   a, DirectX::XMFLOAT2   b);
COMPARISON CompareXMFLOAT3(DirectX::XMFLOAT3   a, DirectX::XMFLOAT3   b);
COMPARISON CompareXMFLOAT3X3(DirectX::XMFLOAT3X3 a, DirectX::XMFLOAT3X3 b);
COMPARISON CompareXMFLOAT4(DirectX::XMFLOAT4   a, DirectX::XMFLOAT4   b);
COMPARISON CompareXMFLOAT4X3(DirectX::XMFLOAT4X3 a, DirectX::XMFLOAT4X3 b);
COMPARISON CompareXMFLOAT4X3(DirectX::XMFLOAT4X3 a, DirectX::XMFLOAT4X3 b);
COMPARISON CompareXMFLOAT4X4(DirectX::XMFLOAT4X4 a, DirectX::XMFLOAT4X4 b);
COMPARISON CompareXMMATRIX(DirectX::CXMMATRIX   a, DirectX::CXMMATRIX   b);
COMPARISON CompareXMHALF2(DirectX::PackedVector::XMHALF2    a, DirectX::PackedVector::XMHALF2    b);
COMPARISON CompareXMHALF4(DirectX::PackedVector::XMHALF4    a, DirectX::PackedVector::XMHALF4    b);
COMPARISON CompareXMXDECN4(DirectX::PackedVector::XMXDECN4   a, DirectX::PackedVector::XMXDECN4   b);
COMPARISON CompareXMSHORTN2(DirectX::PackedVector::XMSHORTN2  a, DirectX::PackedVector::XMSHORTN2  b);
COMPARISON CompareXMSHORTN4(DirectX::PackedVector::XMSHORTN4  a, DirectX::PackedVector::XMSHORTN4  b);
COMPARISON CompareHALF(DirectX::PackedVector::HALF       a, DirectX::PackedVector::HALF       b);

using QWORD = uint64_t;

#define countof(x) static_cast<int>(((sizeof(x)) / (sizeof(x[0]))))
#define sizebytes(x) static_cast<int>(sizeof(x))

void fillsandbox(_Out_writes_(sandboxsize) uint8_t* sandbox, _In_ size_t sandboxsize, _In_reads_bytes_(datacount* datasize) const void* data, _In_ size_t datasize, _In_ size_t datastride, _In_ size_t datacount);
bool checksandbox(LogProxy* pLog, _In_reads_(sandboxsize) const uint8_t* sandbox1, _In_reads_(sandboxsize) const uint8_t* sandbox2, _In_ int stride, _In_ int size, _In_ size_t count, _In_ size_t sandboxsize, _In_ int numfloat, _In_ COMPARISON worst = WITHINBIGEPSILON);
void initsandbox(_Out_writes_(sandboxsize) uint8_t* sandbox, _In_ size_t sandboxsize);

extern float GetRandomFloat(float fRange);
extern DirectX::XMVECTOR GetRandomVector16(void);
extern float GetRandomFloat16(void);
extern DirectX::XMMATRIX GetRandomMatrix4(void);

// In shared.cpp

extern void AllocWithAlignment(uint32_t dwSize, uint32_t dwAlignment, BOOL bWriteCombined,
    LPVOID* ppvFreeThisOne, LPVOID* ppvUseThisOne);
extern void FreeWithAlignment(PVOID pAddress, uint32_t dwAllocAttributes);
extern void WriteFloat(float fInput, _Out_writes_(4) char* pOutput);
extern float ReadFloat(const char* pInput);
extern void WriteInt(uint32_t uInput, _Out_writes_(4) char* pOutput);
extern uint32_t ReadInt(const char* pInput);
extern DirectX::XMVECTOR ScalarQuatExp(DirectX::XMVECTOR q);
extern DirectX::XMVECTOR ScalarQuatLn(DirectX::XMVECTOR q);
extern DirectX::XMVECTOR ScalarQuatSlerp(DirectX::XMVECTOR q1, DirectX::XMVECTOR q2, float t);

inline void blahblah(const char*, ...) {}


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-reinterpret-cast"
#endif

const int c_Q_NANint = 0x7fffffff;
const float c_Q_NAN = *reinterpret_cast<const float*>(&c_Q_NANint);

const int c_INFint = 0x7f800000;
const float c_INF = *reinterpret_cast<const float*>(&c_INFint);

const int c_NANint = 0x7f835234; //random nan
const float c_NAN = *reinterpret_cast<const float*>(&c_NANint);

#ifdef __clang__
#pragma clang diagnostic pop
#endif

extern const int  g_iStartAlignments[4];
extern const uint32_t g_dwPhysicalAttribs;

inline void printmatrixe(const DirectX::XMMATRIX& ma)
{
    float m[4][4];
    memcpy_s(&m, sizeof(float) * 16, &ma, sizeof(DirectX::XMMATRIX));
    printe("%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n",
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3]);
}

inline void printmatrixi(const DirectX::XMMATRIX& ma)
{
    float m[4][4];
    memcpy_s(&m, sizeof(float) * 16, &ma, sizeof(DirectX::XMMATRIX));
    printi("%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n",
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3]);
}

struct dw10 {
    uint32_t w : 2;
    uint32_t z : 10;
    uint32_t y : 10;
    uint32_t x : 10;
};

// Workaround known issue for VS 2019 RTM that doesn't respect the float_control context for inline with new NaN optimizations
// https://developercommunity.visualstudio.com/content/problem/516492/vs-2019-simd-optimization-bug.html
#if !defined(_XM_NO_INTRINSICS_) && (defined(_MSC_VER) && (_MSC_VER >= 1920) && (_MSC_VER < 1923)) && !defined(__clang__)
#define ISNAN_TEST_BEGIN __pragma(float_control(push)) __pragma(float_control(precise, on))
#define ISNAN_TEST_END __pragma(float_control(pop))
#elif !defined(_XM_NO_INTRINSICS_) && defined(__clang__)
#define ISNAN_TEST_BEGIN _Pragma("float_control(push)") _Pragma("float_control(precise, on)")
#define ISNAN_TEST_END _Pragma("float_control(pop)")
#elif defined(_M_ARM64) && defined(_MSC_VER)
#define ISNAN_TEST_BEGIN __pragma(float_control(push)) __pragma(float_control(precise, on))
#define ISNAN_TEST_END __pragma(float_control(pop))
#else
#define ISNAN_TEST_BEGIN
#define ISNAN_TEST_END
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4244 4456 4738 6001 6031 6220 6221 6226 6246 6340 26451)
// C4244: Off by default noise
// C4456/4738/6001/6031/6220/6221/6226/6246/6340/26451: Ignore for tests
#endif
#endif //def _MATH3_H_INCLUDED_
