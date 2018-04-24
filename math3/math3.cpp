//-------------------------------------------------------------------------------------
// math3.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

#ifdef BUILD_FOR_LOGGER 
    #include "CLogManager.h" 
    #include "WTTlogger.h"
#endif

#pragma prefast(disable : 25000, "FXMVECTOR is 16 bytes")

/*!*****************************************************************************
    wprintf wrapper
*******************************************************************************/
#if !defined( RUN_RL_UNDER_GAUNTLET )
#define PRINT( ... )                                                    \
{                                                                       \
    print( __VA_ARGS__ );                                               \
}
#else
#define PRINT( ... )
#endif /* RUN_RL_UNDER_GAUNTLET */

APIFUNCT tests[MAXTESTS]={};
//const char *tests[MAXTESTS];
char bRunTest[MAXTESTS]={};
bool gbPrintAll = false;
//Command Line Options
enum eTestMode
{
    TestModeNormal = 0x0,
    TestModeStress = 0x1
};
eTestMode  gTestMode = TestModeNormal; //Normal Test Mode, 1 for stress
bool gbCmdLineOnly = false; //Only output command line usage
bool gbBreakOnFail = false; //__debugbreak on failure

using namespace DirectX;
using namespace DirectX::PackedVector;

HRESULT __stdcall Initialize(void)
{
    char filename[256]={};
    PRINT("***************\n*************\n");
    PRINT("howdy, from the %s\n", XB ? "Xenon" : "PC");

    PRINT("\tTesting DirectX Math (aka XNAMath 3) %03d\n", DIRECTX_MATH_VERSION);

#if defined(_XM_NO_INTRINSICS_)
    PRINT("\tNO_INTRINSICS\n");
#else

#if defined(_XM_SSE_INTRINSICS_)
    PRINT("\tSSE_INTRINSICS\n");
#ifdef _XM_AVX2_INTRINSICS_
    PRINT("\tAVX2_INTRINSICS\n");
#endif
#ifdef _XM_FMA3_INTRINSICS_
    PRINT("\tFMA3_INTRINSICS_\n");
#endif
#ifdef _XM_F16C_INTRINSICS_
    PRINT("\tF16C_INTRINSICS\n");
#endif
#ifdef _XM_AVX_INTRINSICS_
    PRINT("\tAVX_INTRINSICS\n");
#endif
#ifdef _XM_SSE4_INTRINSICS_
    PRINT("\tSSE4_INTRINSICS\n");
#endif
#ifdef _XM_SSE3_INTRINSICS_
    PRINT("\tSSE3_INTRINSICS\n");
#endif
#endif

#if defined(_XM_ARM_NEON_INTRINSICS_)
#ifdef _M_ARM64
    PRINT("\tARM_NEON_INTRINSICS (ARMv8)\n");
#else
    PRINT("\tARM_NEON_INTRINSICS\n");
#endif
#endif
#endif // !_XM_NO_INTRINSICS_

#if _XM_VECTORCALL_ 
    PRINT("\t__vectorcall\n");
#endif

#ifdef __AVX2__
    PRINT("\t/arch:AVX2\n");
#elif defined(__AVX__)
    PRINT("\t/arch:AVX\n");
#elif defined(_M_IX86)
#if _M_IX86_FP == 0
    PRINT("\t/arch:IA32\n");
#elif _M_IX86_FP == 1
    PRINT("\t/arch:SSE\n");
#elif _M_IX86_FP == 2
    PRINT("\t/arch:SSE2\n");
#endif
#endif

#if defined(_M_ARM)
#if (_M_ARM_FP >= 40) && (_M_ARM_FP < 50)
    PRINT("\t/arch:VFPv4\n");
#endif
#endif

    HRESULT status = S_OK;

    if ( !XMVerifyCPUSupport() )
    {
        print("XMVerifyCPUSupport reports a failure on this platform, skipping all test!\n");
        status = E_FAIL;
    }

    if( SUCCEEDED(status) ) {
        AssignTests();
    }

    //  MapDrive('C', "\\Device\\Harddisk0\\Partition1");

    sprintf_s (filename, DATAPATH "math2.dat");

    FILE* f=NULL;
    int i;
    if( fopen_s(&f, filename, "rt") )
    {
        PRINT("Warning: Couldn't open %s. f==0\nAssuming ALL tests\n", filename);
        for(i = 0; i < MAXTESTS; i++) {
            bRunTest[i] = TRUE;
        }
    } else {
        assert(f != 0);
        _Analysis_assume_(f != 0);

        char str[256];
        bool bKeepReading = true;
        while(bKeepReading) {
            int num[3]={0,0,0};
            int ret = fscanf_s(f, "%ld", &num[0]);
            if(ret == 0) {
                break;
            } else if (ret == EOF) {
                break;
            }
            if(num[0] < -4 || num[0] >= MAXTESTS) {
                PRINT("error parsing %s: unexpected number %d\n", filename, num[0]);
                break;
            }

            switch(num[0]) {
                case -1:
                    for(i = 0; i < MAXTESTS; i++) {
                        bRunTest[i] = TRUE;
                    }
                    PRINT("-1: Adding all tests\n");
                    break;
                case -2:
                    fscanf_s(f, "%d %d", &num[1], &num[2]);
                    for(i = num[1]; i <= num[2]; i++) {
                        bRunTest[i] = TRUE;
                    }
                    PRINT("-2: Adding range of tests %d - %d\n", num[1], num[2]);
                    break;
                case -3:
                    //DebugBreak();
                    fscanf_s(f, "%s", str, 256);
                    for(i = 0; i < MAXTESTS; i++) {
                        if(tests[i].name) {
                            if(strstr(tests[i].name, str)) {
                                bRunTest[i] = TRUE;
                            }
                        }
                    }
                    gbPrintAll = true;
                    PRINT("-3: Adding %s\n", str);
                    break;
                case -4:
                    //DebugBreak();
                    fscanf_s(f, "%s", str, 256);
                    for(i = 0; i < MAXTESTS; i++) {
                        if(tests[i].name) {
                            if(strstr(tests[i].name, str)) {
                                bRunTest[i] = FALSE;
                            }
                        }
                    }
                    gbPrintAll = true;
                    PRINT("-4: Omitting %s\n", str);
                   break;
                default:
                    bRunTest[num[0]] = TRUE;
                    break;
            }
        }

        fclose(f);
    }

    return status;
}

int RunTests()
{
    int i;
    int numpass = 0;
    int numfail = 0;
    int numfatal = 0;
    int total = 0;



        #ifdef BUILD_FOR_LOGGER
        HRESULT hr = S_OK;          
        LONG    hDevice = NULL;
        CWTTLogger *Logger = new CWTTLogger();

        //ILogManager * pLogManager = NULL;
        //Dimension dimension;
        //ZeroMemory( &dimension, sizeof( dimension ) );

        //LogManagerCreate( &pLogManager );

        //hr = pLogManager->Initialize
        //	( 
        //		NULL, 
        //		0,
        //		TESTLOGINFO_GRAPHICS,
        //		NULL,
        //		//L"XNAMath", // use one of the following: VS2008_x86fre VS2008_x86chk VS2008_amd64fre VS2008_amd64chk
        //		LOGGER_ERRORHANDLING_CONTINUE |
        //		LOGGER_RESULTFILECACHING_OFF, 
        //		LOGCREATE_DUPLICATE_DIFFERENTIATE, 
        //		LOGFILTER_LOG_ALLRESULT_ALLINFOLEVEL
        //);

        //hr = pLogManager->LoadLoggerFromCommandLine( NULL );

        //hr = pLogManager->Open();
        //hr = pLogManager->GetDimension( L"GDK\\htmlogfilename", &dimension, NULL );
        //hr = pLogManager->BeginTestGroup( L"XNAMathTests", 1, L"XNAMathTests", TESTTYPE_API_NONE );
    
        // for WTT
        hr = Logger->CreateLogDevice(NULL, &hDevice);
        hr = Logger->Trace(WTT_LVL_MSG, hDevice, L"Starting XNAMath Tests");

        #endif




    for(i = 0; i < MAXTESTS; i++) {
        if(bRunTest[i] && tests[i].funct)
        {
            total++;
            HRESULT r;

            #ifdef BUILD_FOR_LOGGER 
                //hr = pLogManager->BeginTestCase( (LPCWSTR)tests[i].name, i );
                hr = Logger->StartTest((LPWSTR)tests[i].name, hDevice);
                //hr = Logger->Trace(WTT_LVL_MSG,hDevice, (LPWSTR)tests[i].name);
            #endif

            #ifdef BUILD_FOR_HARNESS //the only thing I could find in h2.h...
                r = tests[i].funct(NULL); //shouldn't really be here.
            #else
                r = tests[i].funct(tests[i].name);
            #endif

            if(r == FATAL) {
                PRINT("fatal error in test %d\n", i);
                numfatal++;
                #ifdef BUILD_FOR_LOGGER 
                    //hr = pLogManager->EndTestCase( FAIL );
                    hr = Logger->EndTest((LPWSTR)tests[i].name, WTT_TESTCASE_RESULT_FAIL, NULL,hDevice);

                #endif
            } else if (r == MATH_FAIL) {
                numfail++;
                PRINT("   test %d (%s) FAILED\n", i, tests[i].name);
                #ifdef BUILD_FOR_LOGGER 
                    //hr = pLogManager->EndTestCase( FAIL );
                    hr = Logger->EndTest((LPWSTR)tests[i].name, WTT_TESTCASE_RESULT_FAIL, NULL,hDevice);
                #endif
            } else if (r == S_OK) {
                numpass++;
                PRINT("test %d (%s) passed\n", i, tests[i].name);
                #ifdef BUILD_FOR_LOGGER 
                    //hr = pLogManager->EndTestCase( PASS );
                    hr = Logger->EndTest((LPWSTR)tests[i].name, WTT_TESTCASE_RESULT_PASS, NULL,hDevice);
                #endif
            } else if (r == NODATA) {
                #ifdef BUILD_FOR_LOGGER 
                    //hr = pLogManager->EndTestCase( FAIL );
                    hr = Logger->EndTest((LPWSTR)tests[i].name, WTT_TESTCASE_RESULT_FAIL, NULL,hDevice);
                #endif
            } else if (r == NOTEST) {
//                if(1) {
                    PRINT("test %d (%s) NOT IMPLEMENTED\n", i, tests[i].name);
                    #ifdef BUILD_FOR_LOGGER 
                        //hr = pLogManager->EndTestCase( FAIL );
                        hr = Logger->EndTest((LPWSTR)tests[i].name, WTT_TESTCASE_RESULT_FAIL, NULL,hDevice);
                    #endif
//                }
            }

            //Check for failure and request to breakOnFail
#if !defined( RUN_RL_UNDER_GAUNTLET )
            if( gbBreakOnFail && (r == MATH_FAIL || r == FATAL) )
            {
                __debugbreak();
            }
#endif /* !RUN_RL_UNDER_GAUNTLET */
        }
    }
    PRINT("Number of tests: %d   Total implemented: %d\nNumber passed: %d\nNumber failed: %d\nNumber fatal: %d\n", total, numpass+numfail+numfatal,numpass,numfail,numfatal);
#ifdef BUILD_FOR_LOGGER
        
        /*hr = pLogManager->EndTestGroup();
        hr = pLogManager->Close();
        hr = pLogManager->Cleanup();
        hr = pLogManager->Release();*/
        hr = Logger->Trace(WTT_LVL_ROLLUP, NULL, 0, 0, 0, 0, 0);
        hr = Logger->CloseLogDevice(NULL, hDevice);
        delete Logger;
        
#endif /* BUILD_FOR_LOGGER */

    return ( numpass == total ) ? 0 : 1;
}

#pragma warning(disable : 4746)
volatile bool g_DoQuit = false;

HRESULT __stdcall Uninitialize(void)
{
    g_DoQuit = true;
    return S_OK;
}

XMVECTOR func1(XMVECTOR v)
{
    XMVECTOR a = XMVectorSin(v);
    return a;
}

void func2(XMVECTOR* v)
{
    *v = GetRandomVector16();
    XMVECTOR a = XMVectorAdd(*v,*v);
    float b,c,d,e,f,g,h,i,j,k,l;
    b = XMVectorGetX(XMVector2Dot(*v,*v));
    c = XMVectorGetZ(XMVector3Dot(*v,*v));
    d = XMVectorGetW(XMVector4Dot(*v,*v));
    e = XMVectorGetY(XMVector4Dot(*v,*v));
    f = b*c+d*e-b*d-c*e-b*e-c*d+b*c*d*e;
    g = b*f+d*e-f*d-c*e/b*e-c*d+b*c*d*f;
    h = b*c+g*f-b*g-c*e-b*e-c*d+b*c*f*e;
    i = b*c+f*h-g*d-h*e-b*e/c*d+b*f*d*g;
    j = b*f+d*g-i*d-c*h-b*e-c*i+f*c*g*e;
    k = f*c+g*e-b*j-c*e-h*e-i*f/j*g*d*h;
    l = b*f+d*e-b*d-k*e-b*h-f*d+g*j*h*k;
    v[0] = XMVectorSetZ(v[0],l+k+j+i+h+g+b+c+d+e+f);
    v[1] = a;
}

void func3(XMVECTOR v1, XMVECTOR v2)
{
    XMVECTOR a,b,c,d;
    a = v1*v2+v1*(v2+v2-v1)/XMVectorGetX(v1)+XMVectorGetY(v2)*(v1*v2+XMVector3Dot(v2,v2));
    b = a*v1+v2*a+XMVectorNegativeMultiplySubtract(v1,a,v2)/XMVectorSplatZ(v2);
    c = a*b*a*v1*v2+(a+(v2*(a+(v1*(b*v2-v1)))-v1)*v2);
    d = a*v1+b*v2+c*(v1-v2);
}

#pragma prefast( suppress : 25004 );
DWORD __stdcall SecondThread(void* blah)
{
    UNREFERENCED_PARAMETER(blah);

    XMVECTOR v[3];
    int i = 0;
    PRINT("\nstarted second thread\n\n");
    while(!g_DoQuit) {
        i++;
        if(i == 5000) {
            i = 0;
            //PRINT("\nsecondthread tick\n\n");
        }
        v[0] = GetRandomVector16();
        v[2] = func1(v[0]);
        func2(&v[1]);
        func3(v[2], v[1]);
    }
    PRINT("\nsecond thread terminating\n\n");
    return 0;
}


//print out the command line usage
void PrintCommandLineUsage()
{
    printf("\nDirectXMath version %03d unit tests \n", DIRECTX_MATH_VERSION);
    print("\nCommand Line Usage:\n");
    print("/[parameter] [value] e.g. /mode stress\n");
    print("Parameters:\n");
    print("/? - Get list of command line parameters\n");
    print("/BreakOnFail [true][false] - Calls debugbreak on a failure. The default is false\n");
    print("/Mode [normal][stress] - Sets the test mode. The default mode is normal\n");
    print("\n");
}


//Look for specific command line settings
void ParseCommandLine( _In_z_ TCHAR* szCmdLine )
{
    TCHAR* szArg = szCmdLine;
    TCHAR* szValue = NULL;

    //Command line valid?
    if( !szArg )
        return;

    //Get first argument with /
    szArg = _tcschr( szArg, TEXT('/') );

    while( szArg )
    {
        //Increment string to argument text
        szArg++;

        //Find value following argument
        szValue = _tcschr( szArg, TEXT(' ') );

        //Verify there is a value
        if( szArg[0] != TEXT('?') && ( !szValue || szValue[1] == TEXT('/') ) )
        {
            //Try incrementing to the next argument and continuing
            szArg = _tcschr( szArg, TEXT('/') );
            continue;
        }

        //Increment to the value
        if ( szValue )
            szValue++;

        //Test mode
        if( !_tcsnicmp( szArg, TEXT("mode"), _tcslen(TEXT("mode")) ) )
        {
            if ( szValue )
            {
                //Decipher the test mode
                if( !_tcsnicmp( szValue, TEXT("normal"), _tcslen(TEXT("normal")) ) )
                {
                    gTestMode = TestModeNormal;
                }
                else if( !_tcsnicmp( szValue, TEXT("stress"), _tcslen(TEXT("stress")) ) )
                {
                    gTestMode = TestModeStress;
                }
            }
        }
        else if( !_tcsnicmp( szArg, TEXT("breakonfail"), _tcslen(TEXT("breakonfail")) ) )
        {
            if ( szValue )
            {
                //break on fail setting
                if( !_tcsnicmp( szValue, TEXT("true"), _tcslen(TEXT("true")) ) )
                {
                    gbBreakOnFail = true;
                }
                else if( !_tcsnicmp( szValue, TEXT("false"), _tcslen(TEXT("false")) ) )
                {
                    gbBreakOnFail = false;
                }
            }
        }
        else if( !_tcsnicmp( szArg, TEXT("?"), _tcslen(TEXT("?")) ) )
        {
            //Usage
            PrintCommandLineUsage();
            gbCmdLineOnly = true;
        }

        //Get next argument
        szArg = _tcschr( szArg, TEXT('/') );
    }
}


int __cdecl main(void)
{
    //Get the command line to check for global test settings
    // usage of TCHAR is to maintain compatibility with 360
    // and GetCommandLine function.
    TCHAR* cmdLine = GetCommandLine();
    ParseCommandLine( cmdLine );

    int result = 0;

    //Check to see if only outputting cmdline usage
    if( gbCmdLineOnly ){
        goto Cleanup;
    }

    HRESULT status = Initialize();

    DWORD threadid;
#ifndef __FOR_SBOX
    // Second thread is just irritating to debug for sbox
    HANDLE h = CreateThread(NULL, 0, SecondThread, NULL, 0, &threadid);
    if ( h != NULL )
        CloseHandle(h);
#endif
    if( SUCCEEDED(status) ) {
        if( gTestMode == TestModeStress ){
            //Stress testing
            for(;;)
                RunTests();
        }
        else {
            //Normal test run
            result = RunTests();
        }
    }

//Kill all threads
Cleanup:
    
    Uninitialize();

#if defined( RUN_RL_UNDER_GAUNTLET )
	printf( "%s\n", ( !result ) ? "PASS" : "FAIL\nFor more details build without the defination : RUN_RL_UNDER_GAUNTLET" );
#endif /* RUN_RL_UNDER_GAUNTLET */
    return result;
}



COMPARISON Compare(float a, float b)
{
    if (_isnan(a) && _isnan(b)) return EXACT;
    if (_isnan(a) || _isnan(b)) return WAYOFF;
    if (!_finite(a) && !_finite(b)) {
        if (_copysign(1.0f, a) == _copysign(1.0f, b)) return EXACT;
        else return WAYOFF;
    }
    if (!_finite(a) || !_finite(b)) return WAYOFF;

    if (a == b) return EXACT;
    float f = fabs(b-a);
    if (f <= TESTEPSILON) return WITHINEPSILON;
    if (f <= TEST2EPSILON) return WITHIN2EPSILON;
    if (f <= TEST10EPSILON) return WITHIN10EPSILON;
    if (f <= TEST100EPSILON) return WITHIN100EPSILON;
    if (f <= fabs(a / 4096)) return WITHIN4096;
    if (f <= TESTBIGEPSILON) return WITHINBIGEPSILON;
    if (f <= TESTBIGGEREPSILON) return WITHINBIGGEREPSILON;
    if (f <= TESTHUGEEPSILON) return WITHINHUGEEPSILON;
    if (f <= fabs(a/256)) return WITHIN1_256;
    if (f <= fabs(a/64)) return WITHIN1_64;
    if (f <= fabs(a/16)) return WITHIN1_16;
    if (f <= fabs(a/8)) return WITHIN1_8;
    if (f <= fabs(a / 4)) return CLOSE;

    return WAYOFF;
}

COMPARISON CompareBYTE(uint8_t a, uint8_t b)
{
    if (a == b) return EXACT;
    int ab = abs((int)a - (int)b);
    if (ab <= 1) return WITHINEPSILON;
    if (ab <= 3) return WITHINBIGEPSILON;
    if (ab <= 16) return CLOSE;
    return WAYOFF;
}

#define ASSSERT(f) { if(f) {print("ASSERT: "__FILE__ "(%u): "#f "\n", unsigned(__LINE__)); DebugBreak(); } }

COMPARISON CompareXMVECTOR(XMVECTOR a, XMVECTOR b, int NumElements)
{

    ASSSERT (!((NumElements <= 4) && (NumElements >= 0)));
    int i;
    COMPARISON ret = EXACT;
    COMPARISON r;

    for(i = 0; i < NumElements; i++) {
        r = Compare(XMVectorGetByIndex(a,i),XMVectorGetByIndex(b,i));
        ret = (r > ret) ? r : ret;
    }

    return ret;
}

COMPARISON Compare__vector4  (XMVECTOR   a, XMVECTOR   b)
{
    COMPARISON x = Compare(XMVectorGetX(a),XMVectorGetX(b));
    COMPARISON y = Compare(XMVectorGetY(a),XMVectorGetY(b));
    COMPARISON z = Compare(XMVectorGetZ(a),XMVectorGetZ(b));
    COMPARISON w = Compare(XMVectorGetW(a),XMVectorGetW(b));
    return std::max(x,std::max(y,std::max(z,w)));
}

COMPARISON CompareXMCOLOR   (XMCOLOR    a, XMCOLOR    b)
{
    COMPARISON x = CompareBYTE(a.a,b.a);
    COMPARISON y = CompareBYTE(a.r,b.r);
    COMPARISON z = CompareBYTE(a.g,b.g);
    COMPARISON w = CompareBYTE(a.b,b.b);
    return std::max(x,std::max(y,std::max(z,w)));
}

COMPARISON CompareXMFLOAT2  (XMFLOAT2   a, XMFLOAT2   b)
{
    COMPARISON x = Compare(a.x,b.x);
    COMPARISON y = Compare(a.y,b.y);
    return std::max(x,y);
}
COMPARISON CompareXMFLOAT3  (XMFLOAT3   a, XMFLOAT3   b)
{
    COMPARISON x = Compare(a.x,b.x);
    COMPARISON y = Compare(a.y,b.y);
    COMPARISON z = Compare(a.z,b.z);
    return std::max(x,std::max(y,z));
}
COMPARISON CompareXMFLOAT3X3(const XMFLOAT3X3& a, const XMFLOAT3X3& b)
{
    int x,y;
    COMPARISON ret = EXACT, temp;
    for(x = 0; x < 3; x++) {
        for(y=0;y<3;y++) {
            temp = Compare(a.m[x][y], b.m[x][y]);
            ret = std::max(temp, ret);
        }
    }
    return ret;
}
COMPARISON CompareXMFLOAT4  (XMFLOAT4   a, XMFLOAT4   b)
{
    COMPARISON x = Compare(a.x,b.x);
    COMPARISON y = Compare(a.y,b.y);
    COMPARISON z = Compare(a.z,b.z);
    COMPARISON w = Compare(a.w,b.w);
    return std::max(x,std::max(y,std::max(z,w)));
}

COMPARISON CompareXMFLOAT4X3(const XMFLOAT4X3& a, const XMFLOAT4X3& b)
{
    int x,y;
    COMPARISON ret = EXACT, temp;
    for(x = 0; x < 4; x++) {
        for(y=0;y<3;y++) {
            temp = Compare(a.m[x][y], b.m[x][y]);
            ret = std::max(temp, ret);
        }
    }
    return ret;
}
COMPARISON CompareXMFLOAT4X4(const XMFLOAT4X4& a, const XMFLOAT4X4& b)
{
    int x,y;
    COMPARISON ret = EXACT, temp;
    for(x = 0; x < 4; x++) {
        for(y=0;y<4;y++) {
            temp = Compare(a.m[x][y], b.m[x][y]);
            ret = std::max(temp, ret);
        }
    }
    return ret;
}

COMPARISON CompareXMMATRIX  (CXMMATRIX   a, CXMMATRIX   b)
{
    int x,y;
    COMPARISON ret = EXACT, temp;
    for(x = 0; x < 4; x++)
    {
        for(y=0;y<4;y++) {
            temp = Compare(XMVectorGetByIndex(a.r[x],y), XMVectorGetByIndex(b.r[x],y));
            ret = std::max(temp, ret);
        }
    }
    return ret;
}

float halftofloat(HALF a)
{
    uint32_t sign = a & 0x8000;
    uint32_t exponent = (a & 0x7c00) >> 10;
    uint32_t mantissa = a & 0x3ff;

    uint32_t out = sign << 16;
    if(exponent == 0) {
        if(mantissa == 0)
            ;
        else {
            exponent += 0x70;
            while(!(mantissa & 0x400)) {
                exponent--;
                mantissa <<= 1;
            }
            mantissa &= 0x3ff;
            out |= (exponent << 23) | (mantissa << 13);
        }
    } else if (exponent == 0x1f) {
        out |= 0x7f800000 | (mantissa << 13);
    } else {
        exponent = exponent - 0xf + 0x7f;
        out |= (exponent << 23) | (mantissa << 13);
    }

    float ret = *(float*)&out;
//  print("halftofloat(%x) = 0x%x = %f\n", a, out, ret);
    return ret;
}

COMPARISON CompareHALF      (HALF       a,  HALF       b)
{
    float fa = halftofloat(a);
    float fb = halftofloat(b);
    return Compare(fa,fb);
}
COMPARISON CompareXMHALF2   (XMHALF2    a,  XMHALF2    b);
COMPARISON CompareXMHALF4   (XMHALF4    a,  XMHALF4    b);

COMPARISON CompareXMXDECN4  (XMXDECN4   a, XMXDECN4   b);
COMPARISON CompareXMSHORTN2 (XMSHORTN2  a, XMSHORTN2  b);
COMPARISON CompareXMSHORTN4 (XMSHORTN4  a, XMSHORTN4  b);



#ifdef BUILD_FOR_HARNESS

HRESULT __stdcall InitForHarness(void)
{
    AssignTests();
    return S_OK;
}

unsigned int testMode;
long doabort;

HRESULT __stdcall GetTestList(char**TestNames)
{
    unsigned i;
    unsigned buffLen = 1; // starts at 1 for the NULL char
    char *output;

    if(!tests) return E_INVALIDARG;
    *TestNames = NULL;
    
    if FAILED(Initialize())
        return E_FAIL;

    for(i=0; i<ARRAYSIZE(tests); i++) {
        if(tests[i].name && bRunTest[i]) 
        {
            buffLen += strlen(tests[i].name) + 2; // +2 for comma space
        }
    }

    output = new /*(std::nothrow)*/ char[buffLen];
    if(!output) return E_OUTOFMEMORY;
    *TestNames = output;

    bool firstone = true;
    for(i=0; i<ARRAYSIZE(tests); i++)
    {
        if(tests[i].name && bRunTest[i]) {
            if(firstone) {
                firstone = false;
                output += sprintf_s(output, "%s", tests[i].name);
            } else {
                output += sprintf_s(output, ", %s", tests[i].name);
            }
        }
    }

    return S_OK;
}
HRESULT __stdcall DeleteBuffer(char* buffer)
{
    delete[] buffer;
    return S_OK;
}
HRESULT __stdcall GetDocumentation(char** docs)
{
    if(!docs) return E_INVALIDARG;
    *docs = NULL;
    return S_OK;
}
HRESULT __stdcall CreateTest(char* testName, ITestObject** test)
{
    static counter = 0;
    counter++;

    if(!testName || !test) return E_INVALIDARG;
    *test = NULL;

    for(unsigned i=0; i<ARRAYSIZE(tests); i++)
    {
        if(tests[i].name) {
            if(_stricmp(testName, tests[i].name) == 0)
            {
                Math2Test *t = new (std::nothrow) Math2Test;
                if(!t) return E_OUTOFMEMORY;
                t->testFunct = tests[i].funct;
                t->testName = tests[i].name;
                *test = t;
                return S_OK;
            }
        }
    }
    PRINT("CreateTest: %d: %s\n", counter, testName);

    return E_XBOX_TEST_NOT_FOUND;
}
HRESULT __stdcall DestroyTest(ITestObject* test)
    {
    if(!test) return E_INVALIDARG;

    delete (Math2Test*)test;

    return S_OK;
    }

BOOL WINAPI DllMain(HINSTANCE hInstance, uint32_t dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(lpReserved);

    if(dwReason == DLL_PROCESS_ATTACH)
    {
        //XboxDriverInit();
    }

    return TRUE;
}
#endif

/**********************************

    Generate a random number from 0 to fRange - epsilon.
    Example, fRange = 1.0f would yield 0 to 0.9999999f
    fRange = 5.5f would yield 0 to 5.49999999f

    Note: This MUST be thread safe and it cannot use
    any testable functions

**********************************/

float GetRandomFloat(float fRange)
{
    // Grab the raw integer value and convert to a float
    float fx = static_cast<float>(rand());
    // The float is 0 to RAND_MAX inclusive
    fx = (fx / static_cast<float>(RAND_MAX)) * fRange;   // Normalize to 0-fRange
    return fx;
}

/**********************************

    Generate a randomly initialized vector
    with each element being in the range of 16.767f to -16.0f
    Note: This MUST be thread safe and it cannot use
    any testable functions

**********************************/

XMVECTOR GetRandomVector16(void)
{
    // Use scalar math, not vector math, to
    // generate the values. This will prevent
    // accidental false positives if the vector
    // operations being used are the ones that
    // are failing.

    // Grab the raw integer value and convert to a float
    float fx = static_cast<float>(rand());
    float fy = static_cast<float>(rand());
    float fz = static_cast<float>(rand());
    float fw = static_cast<float>(rand());
    // The float is 0 to RAND_MAX inclusive
    fx = fx / (static_cast<float>(RAND_MAX)/32.767f);   // Normalize to 0-32.767f
    fy = fy / (static_cast<float>(RAND_MAX)/32.767f);   
    fz = fz / (static_cast<float>(RAND_MAX)/32.767f);
    fw = fw / (static_cast<float>(RAND_MAX)/32.767f);
    fx = fx-16.0f;          // Convert 0-32.767f to -16f - 16.767f
    fy = fy-16.0f;
    fz = fz-16.0f;
    fw = fw-16.0f;
    // This data type ensures there is no code being generated.
    XMVECTORF32 vResult = {
        fx,
        fy,
        fz,
        fw
    };
    // Return the vector value
    return vResult;
}

/**********************************

    Generate a random floating point number
    in the range of 16.767f to -16.0f
    Note: This MUST be thread safe and it cannot use
    any testable functions

**********************************/

float GetRandomFloat16(void)
{
    // Grab the raw integer value and convert to a float
    float fx = static_cast<float>(rand());
    // The float is 0 to RAND_MAX inclusive
    fx = fx / (static_cast<float>(RAND_MAX)/32.767f);   // Normalize to 0-32.767f
    fx = fx-16.0f;          // Convert 0-32.767f to -16f - 16.767f
    return fx;
}

/**********************************

    Generate a randomly initialized matrix
    with each element being in the range of 4.19175f
    to -4.0f. No attempt is made to normalize the
    matrix or make it suitable for any specific operation.
    Note: This MUST be thread safe and it cannot use
    any testable functions.

**********************************/

XMMATRIX GetRandomMatrix4(void)
{
    int i = 0;
    float m[4][4];
    do {
        int j = 0;
        do {
            //range of -4 .. 4.192
            m[i][j] = (static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)/8.19175f)) - 4.0f;
        } while (++j<4);
    } while (++i<4);
    XMMATRIX r(&m[0][0]);
    return r;
}

/**********************************

    This stub is needed to get the SBox version
    to link. The function does nothing.

**********************************/

#ifdef __FOR_SBOX
extern "C" void XLaunchNewImageEx(void * pvA, void * pvB, void * pvC);
void XLaunchNewImageEx(void * /* pvA */, void * /* pvB */, void * /* pvC */)
{
}
#endif
