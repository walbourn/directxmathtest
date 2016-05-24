#pragma warning(disable : 4289 4365 4514 4555 4640 4619 4668 4710 4711 4820)

#pragma warning(disable : 4777) // When we drop VS 2013, we can change "%Iu" to "%zu" and remove this.

#pragma warning(disable : 5029) // Can remove when __vector4i is marked deprecated

#ifndef _MATH3_H_INCLUDED_
#define _MATH3_H_INCLUDED_

#if defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64)
  #define VC_EXTRALEAN
  #include <windows.h>
  #define print printf
  #define DATAPATH ""
  static const int XB = 0;
  static const int PC = 1;
  #define BREAK { _asm { int 3 }; }
  typedef HRESULT (*APITEST_FUNC)(const char *TestName);
  #define LogProxy const char
#else
  #define NOD3D
  #include <xtl.h>

  #define print DebugPrint
  #define DATAPATH "d:\\"
  static const int XB = 1;
  static const int PC = 0;

  #define BREAK { DebugBreak(); }
#endif

#ifdef BUILD_FOR_HARNESS
    #include "h2.h"

    extern char* g_File;
    extern int g_Line;
    extern char* g_FunctionName;
    extern int g_Status;
    extern LogProxy* g_Log;

    typedef HRESULT (*APITEST_FUNC)(LogProxy *log);
    extern unsigned seed;
    extern unsigned testMode; // bvt, regression, or stress
    extern LONG doabort;
    
	char * GetFunctionName(char* func);
    void DoLog(const char*pc, ...);

	class Math3Test : public ITestObject
	{
	public:
		APITEST_FUNC testFunct;
		char *testName;

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
			if(_stricmp(name, "TESTMODE") == 0)
			{
				if(_stricmp(value, "BVT") == 0)
				{
					testMode = TESTMODE_BVT;
				}
				else if(_stricmp(value, "DAILY") == 0)
				{
					testMode = TESTMODE_DAILY_REGRESSION;
				}
				else if(_stricmp(value, "STRESS") == 0)
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

			if(!g_File) 
			    g_File = "math3.h";
			if(!g_FunctionName) 
			    g_FunctionName = "StartTest";

			if (TESTMODE_STRESS!=testMode)
			{
				log->Debug(FILEINFO, ALL_DEBUG_ZONES, "%s %s", testName, SUCCEEDED(hr) ? "passed" : "failed");
			}

			if(hr == S_OK)
			{
				log->Pass(FILEINFO,"%s passed", testName);
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
    #define dqi(q) printi("%s: %f,%f,%f,%f\n",#q,XMVectorGetX(q),XMVectorGetY(q),XMVectorGetZ(q),XMVectorGetW(q));
    #define dqe(q) printe("%s: %f,%f,%f,%f\n",#q,XMVectorGetX(q),XMVectorGetY(q),XMVectorGetZ(q),XMVectorGetW(q));
    #define dqestruct(q) printe("%s: %f,%f,%f,%f\n",#q,q.x,q.y,q.z,q.w);


#else
    typedef HRESULT (*APITEST_FUNC)(const char *TestName);
    #define LogProxy const char
	extern "C" void __cdecl DebugPrint(const char*,...);

    #define pLog TestName
    #define printi blahblah
    #define printe print
    #define dqi(q) printi("%s(%d): %f,%f,%f,%f\n",#q,__LINE__,XMVectorGetX(q),XMVectorGetY(q),XMVectorGetZ(q),XMVectorGetW(q));
    #define dqe(q) printe("%s(%d): %f,%f,%f,%f\n",#q,__LINE__,XMVectorGetX(q),XMVectorGetY(q),XMVectorGetZ(q),XMVectorGetW(q));
    #define dqestruct(q) printe("%s(%d): %f,%f,%f,%f\n",#q,__LINE__,q.x,q.y,q.z,q.w);
#endif //def BUILD_FOR_HARNESS


  struct APIFUNCT
  {
      APITEST_FUNC funct;
      char *name;
  };

#include <stdio.h>
#include <stdlib.h>

#include <tchar.h>

#include <directxmath.h>
#include <directxcolors.h>
#include <directxpackedvector.h>
#include <directxcollision.h>
#include <intrin.h>
typedef DirectX::XMVECTORU32 XMVECTORI;

#pragma warning(push)
#pragma warning(disable : 4987)
#include <intrin.h>
#pragma warning(pop)

//extern "C" { extern HRESULT __stdcall MapDrive(char cDriveLetter, char* pszPartition); }

#define MAXTESTS 1000

#define Pi 3.1415926535897932384626f

const float UC_START = -2.f*Pi;
const float UC_END = 2.f*Pi;
const float UC_STEP = (2.0f*Pi) / 360.f;

enum TESTRETVAL {MATH_PASS=S_OK, MATH_FAIL=-1, FATAL=-2, NODATA=3, NOTEST=4};

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
typedef TESTRETVAL (*TESTFUNC)(const char*);

extern /*__declspec(thread)*/ char bRunTest[MAXTESTS];
extern /*__declspec(thread)*/ APIFUNCT tests[MAXTESTS];
//extern const char *tests[MAXTESTS];
void AssignTests(void);


COMPARISON Compare(float a, float b);
COMPARISON CompareBYTE(uint8_t a, uint8_t b);
COMPARISON CompareXMVECTOR(DirectX::XMVECTOR a, DirectX::XMVECTOR b, int NumElements);
COMPARISON Compare__vector4  (DirectX::XMVECTOR   a, DirectX::XMVECTOR   b);
COMPARISON CompareXMCOLOR   (DirectX::PackedVector::XMCOLOR    a, DirectX::PackedVector::XMCOLOR    b);
COMPARISON CompareXMFLOAT2  (DirectX::XMFLOAT2   a, DirectX::XMFLOAT2   b);
COMPARISON CompareXMFLOAT3  (DirectX::XMFLOAT3   a, DirectX::XMFLOAT3   b);
COMPARISON CompareXMFLOAT3X3(DirectX::XMFLOAT3X3 a, DirectX::XMFLOAT3X3 b);
COMPARISON CompareXMFLOAT4  (DirectX::XMFLOAT4   a, DirectX::XMFLOAT4   b);
COMPARISON CompareXMFLOAT4X3(DirectX::XMFLOAT4X3 a, DirectX::XMFLOAT4X3 b);
COMPARISON CompareXMFLOAT4X3(DirectX::XMFLOAT4X3 a, DirectX::XMFLOAT4X3 b);
COMPARISON CompareXMFLOAT4X4(DirectX::XMFLOAT4X4 a, DirectX::XMFLOAT4X4 b);
COMPARISON CompareXMMATRIX  (DirectX::CXMMATRIX   a, DirectX::CXMMATRIX   b);
COMPARISON CompareXMHALF2   (DirectX::PackedVector::XMHALF2    a, DirectX::PackedVector::XMHALF2    b);
COMPARISON CompareXMHALF4   (DirectX::PackedVector::XMHALF4    a, DirectX::PackedVector::XMHALF4    b);
COMPARISON CompareXMXDECN4  (DirectX::PackedVector::XMXDECN4   a, DirectX::PackedVector::XMXDECN4   b);
COMPARISON CompareXMSHORTN2 (DirectX::PackedVector::XMSHORTN2  a, DirectX::PackedVector::XMSHORTN2  b);
COMPARISON CompareXMSHORTN4 (DirectX::PackedVector::XMSHORTN4  a, DirectX::PackedVector::XMSHORTN4  b);
COMPARISON CompareHALF      (DirectX::PackedVector::HALF       a, DirectX::PackedVector::HALF       b);

typedef unsigned __int64 QWORD;

#define countof(x) ((sizeof(x)) / (sizeof(x[0])))

void fillsandbox(_Out_writes_(sandboxsize) uint8_t*sandbox, _In_ int sandboxsize, _In_reads_bytes_(datacount*datasize) void*data, _In_ int datasize, _In_ int datastride, _In_ int datacount);
bool checksandbox(LogProxy*pLog,_In_reads_(sandboxsize) const uint8_t*sandbox1, _In_reads_(sandboxsize) const uint8_t*sandbox2, _In_ int stride, _In_ int size, _In_ int count, _In_ int sandboxsize, _In_ int numfloat, _In_ COMPARISON worst = WITHINBIGEPSILON);
void initsandbox(_Out_writes_(sandboxsize) uint8_t*sandbox, _In_ int sandboxsize);

extern float GetRandomFloat(float fRange);
extern DirectX::XMVECTOR GetRandomVector16(void);
extern float GetRandomFloat16(void);
extern DirectX::XMMATRIX GetRandomMatrix4(void);

// In shared.cpp

extern void AllocWithAlignment(uint32_t dwSize,uint32_t dwAlignment,BOOL bWriteCombined,
    LPVOID *ppvFreeThisOne,LPVOID *ppvUseThisOne);
extern void FreeWithAlignment(PVOID pAddress,uint32_t dwAllocAttributes);
extern void WriteFloat(float fInput,_Out_writes_(4) char *pOutput); 
extern float ReadFloat(const char *pInput);
extern void WriteInt(uint32_t uInput,_Out_writes_(4) char*pOutput);
extern uint32_t ReadInt(const char *pInput);
extern DirectX::XMVECTOR ScalarQuatExp(DirectX::XMVECTOR q);
extern DirectX::XMVECTOR ScalarQuatLn(DirectX::XMVECTOR q);
extern DirectX::XMVECTOR ScalarQuatSlerp(DirectX::XMVECTOR q1,DirectX::XMVECTOR q2,float t);

inline void blahblah(const char*,...){}


const int _Q_NANint = 0x7fffffff;
const float _Q_NAN = *(float*)&_Q_NANint;

const int _INFint = 0x7f800000;
const float _INF = *(float*)&_INFint;

const int _NANint = 0x7f835234; //random nan
const float _NAN = *(float*)&_NANint;

extern const int  g_iStartAlignments[3];// = {4, 16, 128}; 	
extern const uint32_t g_dwPhysicalAttribs;

inline void printmatrixe(const DirectX::XMMATRIX& ma)
{
    float m[4][4];
    memcpy_s( &m, sizeof(float) * 16, &ma, sizeof(DirectX::XMMATRIX) );
	printe ("%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n",
		    m[0][0],m[0][1],m[0][2],m[0][3],
		    m[1][0],m[1][1],m[1][2],m[1][3],
		    m[2][0],m[2][1],m[2][2],m[2][3],
		    m[3][0],m[3][1],m[3][2],m[3][3]);
}

inline void printmatrixi(const DirectX::XMMATRIX& ma)
{
    float m[4][4];
    memcpy_s( &m, sizeof(float) * 16, &ma, sizeof(DirectX::XMMATRIX) );
	printi ("%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n",
		    m[0][0],m[0][1],m[0][2],m[0][3],
		    m[1][0],m[1][1],m[1][2],m[1][3],
		    m[2][0],m[2][1],m[2][2],m[2][3],
		    m[3][0],m[3][1],m[3][2],m[3][3]);
}

typedef struct {
	uint32_t w : 2;
	uint32_t z : 10;
	uint32_t y : 10;
	uint32_t x : 10;
} dw10;

#pragma warning(disable : 4244 4456 6001 6031 6220 6221 6226 6246 6340)

#endif //def _MATH3_H_INCLUDED_
