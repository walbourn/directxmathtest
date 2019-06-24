//-------------------------------------------------------------------------------------
// shared.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

#pragma prefast(disable : 25000, "FXMVECTOR is 16 bytes")

using namespace DirectX;

#ifndef BUILD_FOR_HARNESS
#else
  char * GetFunctionName(char* func) {
      uint32_t i = (uint32_t)((func[4]-'0')*100+(func[5]-'0')*10+(func[6]-'0'));
      return i < 1000 ? tests[i].name : func;
  }
  void DoLog(const char*pc, ...) {
//	  DebugBreak();
      char buf[512];
      va_list v;
      va_start(v, pc);
      _vsnprintf(buf, 511, pc, v);
      buf[511] = '\0';
      va_end(v);
      if(g_Status == 0) 
      {
//		  g_Log->Debug(g_File, g_Line, g_FunctionName, 0xF0000000, buf);
      }
      else if (g_Status == 1) 
      {
          g_Log->Debug(g_File, g_Line, g_FunctionName, 0xF0000000, buf);
          g_Log->Failure(g_File, g_Line, g_FunctionName, UNASSIGNED_ERROR_CODE, buf);
      }
  }

  char* g_File;
  int g_Line;
  char* g_FunctionName;
  int g_Status;
  LogProxy* g_Log;

#endif

bool sandcheck(LogProxy* pLog, const XMVECTOR* v1, const XMVECTOR*v2, int numfloat, COMPARISON worst) 
{
    COMPARISON c = CompareXMVECTOR(*v1,*v2,numfloat);
    if(c > worst) {
        switch(numfloat) {
            case 1:
                printe("%s: . %f ... %f (%d)\n", TestName,
                    XMVectorGetX(*v1), XMVectorGetX(*v2),c);
                return false;
            case 2:
                printe("%s: . %f %f ... %f %f (%d)\n", TestName,
                    XMVectorGetX(*v1),XMVectorGetY(*v1),XMVectorGetX(*v2),XMVectorGetY(*v2),c);
                return false;
            case 3:
                printe("%s: . %f %f %f ... %f %f %f (%d)\n", TestName,
                    XMVectorGetX(*v1),XMVectorGetY(*v1),XMVectorGetZ(*v1),XMVectorGetX(*v2),XMVectorGetY(*v2),XMVectorGetZ(*v2),c);
                return false;
            case 4:
                printe("%s: . %f %f %f %f ... %f %f %f %f (%d)\n", TestName,
                    XMVectorGetX(*v1),XMVectorGetY(*v1),XMVectorGetZ(*v1),XMVectorGetW(*v1), XMVectorGetX(*v2),XMVectorGetY(*v2),XMVectorGetZ(*v2),XMVectorGetW(*v2),c);
                return false;
        }
    }
    return true;
}

_Use_decl_annotations_
void initsandbox(uint8_t*sandbox, int sandboxsize) 
{
    for(int i =0; i < sandboxsize; i++)
    {
        sandbox[i] = ~(uint8_t)(i & 0xff);
    }
}

_Use_decl_annotations_
void fillsandbox(uint8_t*sandbox, int sandboxsize, void*data, int datasize, int datastride, int datacount) 
{
    initsandbox(sandbox,sandboxsize);
    for(int i = 0; i < datacount; i++) {
        for(int j = 0; j < datastride && j < datasize; j++) {
            int dest = i*datastride+j;
            if ( dest >= sandboxsize ) break;
            sandbox[dest]=((uint8_t*)data)[i*datasize+j];
        }
    }
}

_Use_decl_annotations_
bool checksandbox(LogProxy* pLog, const uint8_t*sandbox1, const uint8_t*sandbox2,int stride, int size, int count, int sandboxsize, int numfloat, COMPARISON worst) {
    bool ret = true;
    XMVECTOR v1, v2;
    int i,j;
    for(i = 0; i < count; i++) {
        
        //Just copy 16 bytes into the XMVECTORS.  Note - what if the stride is < 16, wil we run off the end?
        for(j = 0; j < 16; j++) 
        {
            ((uint8_t*)&v1)[j]=sandbox1[i*stride+j];
            ((uint8_t*)&v2)[j]=sandbox2[i*stride+j];
        }
        if(!sandcheck(pLog, &v1,&v2,numfloat, worst)) 
        {		
            ret = false;
        }
        
        //Check the bytes that were skipped over due to stride
        for(j = i*stride+size; (j < sandboxsize) && (j < i*stride+stride); j++)
        {
            if(sandbox1[j] != sandbox2[j]) {
                printe("corrupted byte %d: %x ... %x\n", j, sandbox1[j], sandbox2[j]);
                ret = false;
            }
        }
    }
    
    //Check from the end of the data to the end of the sandbox
    for(int j = i*stride; j < sandboxsize; j++) {
        if(sandbox1[j] != sandbox2[j]) {
            printe("corrupted byte %d: %x ... %x\n", j, sandbox1[j], sandbox2[j]);
            ret = false;
        }
    }
    return ret;
}

/**********************************

    Allocates the number of bytes and returns a pointer
    with the exactly alignment you specify.
    
    Two pointers are returned, ppvUseThisOne points to
    the aligned memory while ppvFreeThisOne is passed
    to FreeWithAlignment() to dispose of the memory.
   
    The two pointers are set to nullptr in the event of
    an out of memory condition.

**********************************/

void AllocWithAlignment(
    uint32_t dwSize,            //Actual number of bytes to allocate.
    uint32_t dwAlignment,       //Actual number of bytes to return offset from a 32k page.
    BOOL /*bWriteCombined*/,     //TRUE if write combined, FALSE if not.
    LPVOID *ppvFreeThisOne,  //Free this pointer with XMemFree.
    LPVOID *ppvUseThisOne    //Use this pointer, it has the alignment (and of course attributes) you requested.
    )
{
    // On other platforms, use malloc
    void *pMemory = malloc(dwSize+dwAlignment+16);
    if (pMemory) {
        *ppvFreeThisOne = pMemory;
        uintptr_t uMem = reinterpret_cast<uintptr_t >(pMemory);
        // Is the memory already satisfying the alignment?
        uMem = uMem % dwAlignment;
        if (uMem) {
            // Nope, push the pointer to the next valid aligned address.
            pMemory = reinterpret_cast<char *>(pMemory)+(dwAlignment-uMem);
        }
        else if ( dwAlignment < 16 )
        {
            // Ensures we aren't also 16 byte aligned
            pMemory = reinterpret_cast<char *>(pMemory)+16-dwAlignment;
        }
        // Return the aligned memory pointer
        *ppvUseThisOne = pMemory;
    } else {
        *ppvFreeThisOne = nullptr;
        *ppvUseThisOne = nullptr;
    }
}
    
/**********************************

    Release memory allocated with
    AllocWithAlignment(). nullptr is
    an acceptable input parameter.

**********************************/

void FreeWithAlignment(PVOID pAddress,uint32_t /*dwAllocAttributes*/)
{
    // On other platforms, the memory was allocated with malloc()

    // free() can crash on some implementations with a nullptr pointer.
    // Prevent the crash.
    if (pAddress) {
        free(pAddress);
    }
}

/**********************************

    Write a floating point value into
    byte aligned memory.

**********************************/

_Use_decl_annotations_
void WriteFloat(float fInput,char* pOutput) 
{
    // Ensure the data is float aligned
    union {
        char m_cArray[4];
        float m_fTemp;
    } Temp;
    // Store the float to aligned memory
    Temp.m_fTemp = fInput;
    // Do a "memcpy" to unaligned memory
    // Note: This is an unavoidable Load/Hit/Store
    pOutput[0] = Temp.m_cArray[0];
    pOutput[1] = Temp.m_cArray[1]; 
    pOutput[2] = Temp.m_cArray[2]; 
    pOutput[3] = Temp.m_cArray[3];
}

/**********************************

    Read a floating point value from
    byte aligned memory.

**********************************/

float ReadFloat(const char* pInput) 
{
    // Ensure the data is float aligned
    union {
        char m_cArray[4];
        float m_fTemp;
    } Temp;
    // Copy the data to an aligned buffer
    Temp.m_cArray[0] = pInput[0];
    Temp.m_cArray[1] = pInput[1]; 
    Temp.m_cArray[2] = pInput[2];
    Temp.m_cArray[3] = pInput[3];
    // Fetch the floating point number
    // from float aligned memory

    // Note: This is an unavoidable Load/Hit/Store
    return Temp.m_fTemp;
}

/**********************************

    Write a int value into byte aligned memory

**********************************/

_Use_decl_annotations_
void WriteInt(uint32_t uInput,char*pOutput)
{
    // Ensure the data is int aligned
    union {
        char m_cArray[4];
        uint32_t m_uTemp;
    } Temp;
    // Store the int to aligned memory
    Temp.m_uTemp = uInput;
    // Do a "memcpy" to unaligned memory
    // Note: This is an unavoidable Load/Hit/Store
    pOutput[0] = Temp.m_cArray[0];
    pOutput[1] = Temp.m_cArray[1]; 
    pOutput[2] = Temp.m_cArray[2]; 
    pOutput[3] = Temp.m_cArray[3];
}

/**********************************

    Read a int value into byte aligned memory

**********************************/

uint32_t ReadInt(const char *pInput)
{
    // Ensure the data is float aligned
    union {
        char m_cArray[4];
        uint32_t m_uTemp;
    } Temp;
    // Copy the data to an aligned buffer
    Temp.m_cArray[0] = pInput[0];
    Temp.m_cArray[1] = pInput[1]; 
    Temp.m_cArray[2] = pInput[2];
    Temp.m_cArray[3] = pInput[3];
    // Note: This is an unavoidable Load/Hit/Store
    return Temp.m_uTemp;
}

/**********************************

    Perform a quaternion exponential
    operation using only scalar math.

**********************************/

XMVECTOR ScalarQuatExp(XMVECTOR q) 
{
    // Get the x,y and z components, the w is don't care
    float x = XMVectorGetX(q);
    float y = XMVectorGetY(q);
    float z = XMVectorGetZ(q);
    // Get theta
    float theta = sqrtf(x*x+y*y+z*z);
    float c = cosf(theta);  // The w component is cos.
    if (Compare(theta,0) > WITHINEPSILON) {
        float s = sinf(theta);  // Used for the x,y and z
        theta = s/theta;        // Combine the term s and theta
        x = x*theta;            // x = x * s / theta
        y = y*theta;
        z = z*theta;
    }
    // Convert the scalars to a vector and exit
    XMVECTOR vResult = XMVectorSet(x,y,z,c);
    return vResult;
}

/**********************************

    Perform a natural logarithm for a
    quaternion using only scalar math.

**********************************/

XMVECTOR ScalarQuatLn(XMVECTOR q) 
{
    // Get the x,y and z components
    float x = XMVectorGetX(q);
    float y = XMVectorGetY(q);
    float z = XMVectorGetZ(q);
    // Grab w for the normalization
    float qw = XMVectorGetW(q);
    // Only w less than 1 is valid.
    if (qw <= 1.0f) {
        float acosqw = acosf(qw);
        float s = sinf(acosqw);
        if (Compare(s,0.0f) > WITHINEPSILON) {
            acosqw = acosqw / s;
            x = x*acosqw;
            y = y*acosqw;
            z = z*acosqw;
        }
    }
    XMVECTOR r = XMVectorSet(x,y,z,0.0f);
    return r;
}

/**********************************

    Perform a spherical linear interpolation
    between two quaternions with t being
    the mid mark. Use only scalar math.

    t is of the range 0.0f to 1.0f

**********************************/

XMVECTOR ScalarQuatSlerp(XMVECTOR q1, XMVECTOR q2, float t)
{
    // Extract the components
    float q1x = XMVectorGetX(q1);
    float q1y = XMVectorGetY(q1);
    float q1z = XMVectorGetZ(q1);
    float q1w = XMVectorGetW(q1);
    float q2x = XMVectorGetX(q2);
    float q2y = XMVectorGetY(q2);
    float q2z = XMVectorGetZ(q2);
    float q2w = XMVectorGetW(q2);
    // Find the dot product
    float dot = (q1x * q2x) +
        (q1y * q2y) +
        (q1z * q2z) +
        (q1w * q2w);
    // Determine the direction of the rotation.
    if (dot < 0.0f) { 
        dot = -dot;
        q2x = -q2x;
        q2y = -q2y;
        q2z = -q2z;
        q2w = -q2w;
    }
    float theta = acosf(dot);
    float sintheta = sinf(theta);
    float scale1 = (sinf(theta*(1.0f-t)) / sintheta);
    float scale2 = (sinf(theta*t) / sintheta);
    // Perform the slerp.
    q1x = (q1x*scale1) + (q2x*scale2);
    q1y = (q1y*scale1) + (q2y*scale2);
    q1z = (q1z*scale1) + (q2z*scale2);
    q1w = (q1w*scale1) + (q2w*scale2);
    // Convert the scalar answer to a vector
    XMVECTOR r = XMVectorSet(q1x,q1y,q1z,q1w);
    return r;
}

//generated with a macro. If you need to change it, use a macro. It's just easier that way.
HRESULT Test001(LogProxy* pLog);
HRESULT Test049(LogProxy* pLog);
HRESULT Test050(LogProxy* pLog);
HRESULT Test051(LogProxy* pLog);
HRESULT Test052(LogProxy* pLog);
HRESULT Test053(LogProxy* pLog);
HRESULT Test054(LogProxy* pLog);
HRESULT Test055(LogProxy* pLog);
HRESULT Test056(LogProxy* pLog);
HRESULT Test057(LogProxy* pLog);
HRESULT Test058(LogProxy* pLog);
HRESULT Test059(LogProxy* pLog);
HRESULT Test060(LogProxy* pLog);
HRESULT Test061(LogProxy* pLog);
HRESULT Test062(LogProxy* pLog);
HRESULT Test064(LogProxy* pLog);
HRESULT Test065(LogProxy* pLog);
HRESULT Test067(LogProxy* pLog);
HRESULT Test068(LogProxy* pLog);
HRESULT Test069(LogProxy* pLog);
HRESULT Test070(LogProxy* pLog);
HRESULT Test071(LogProxy* pLog);
HRESULT Test072(LogProxy* pLog);
HRESULT Test073(LogProxy* pLog);
HRESULT Test074(LogProxy* pLog);
HRESULT Test075(LogProxy* pLog);
HRESULT Test076(LogProxy* pLog);
HRESULT Test077(LogProxy* pLog);
HRESULT Test078(LogProxy* pLog);
HRESULT Test079(LogProxy* pLog);
HRESULT Test080(LogProxy* pLog);
HRESULT Test081(LogProxy* pLog);
HRESULT Test082(LogProxy* pLog);
HRESULT Test083(LogProxy* pLog);
HRESULT Test084(LogProxy* pLog);
HRESULT Test085(LogProxy* pLog);
HRESULT Test086(LogProxy* pLog);
HRESULT Test087(LogProxy* pLog);
HRESULT Test088(LogProxy* pLog);
HRESULT Test089(LogProxy* pLog);
HRESULT Test090(LogProxy* pLog);
HRESULT Test091(LogProxy* pLog);
HRESULT Test092(LogProxy* pLog);
HRESULT Test093(LogProxy* pLog);
HRESULT Test094(LogProxy* pLog);
HRESULT Test095(LogProxy* pLog);
HRESULT Test096(LogProxy* pLog);
HRESULT Test097(LogProxy* pLog);
HRESULT Test098(LogProxy* pLog);
HRESULT Test099(LogProxy* pLog);
HRESULT Test100(LogProxy* pLog);
HRESULT Test101(LogProxy* pLog);
HRESULT Test102(LogProxy* pLog);
HRESULT Test103(LogProxy* pLog);
HRESULT Test104(LogProxy* pLog);
HRESULT Test105(LogProxy* pLog);
HRESULT Test106(LogProxy* pLog);
HRESULT Test107(LogProxy* pLog);
HRESULT Test108(LogProxy* pLog);
HRESULT Test109(LogProxy* pLog);
HRESULT Test110(LogProxy* pLog);
HRESULT Test111(LogProxy* pLog);
HRESULT Test112(LogProxy* pLog);
HRESULT Test113(LogProxy* pLog);
HRESULT Test114(LogProxy* pLog);
HRESULT Test115(LogProxy* pLog);
HRESULT Test116(LogProxy* pLog);
HRESULT Test117(LogProxy* pLog);
HRESULT Test118(LogProxy* pLog);
HRESULT Test119(LogProxy* pLog);
HRESULT Test120(LogProxy* pLog);
HRESULT Test121(LogProxy* pLog);
HRESULT Test122(LogProxy* pLog);
HRESULT Test123(LogProxy* pLog);
HRESULT Test124(LogProxy* pLog);
HRESULT Test125(LogProxy* pLog);
HRESULT Test126(LogProxy* pLog);
HRESULT Test127(LogProxy* pLog);
HRESULT Test128(LogProxy* pLog);
HRESULT Test129(LogProxy* pLog);
HRESULT Test130(LogProxy* pLog);
HRESULT Test131(LogProxy* pLog);
HRESULT Test132(LogProxy* pLog);
HRESULT Test133(LogProxy* pLog);
HRESULT Test134(LogProxy* pLog);
HRESULT Test135(LogProxy* pLog);
HRESULT Test136(LogProxy* pLog);
HRESULT Test137(LogProxy* pLog);
HRESULT Test138(LogProxy* pLog);
HRESULT Test139(LogProxy* pLog);
HRESULT Test140(LogProxy* pLog);
HRESULT Test141(LogProxy* pLog);
HRESULT Test142(LogProxy* pLog);
HRESULT Test143(LogProxy* pLog);
HRESULT Test144(LogProxy* pLog);
HRESULT Test145(LogProxy* pLog);
HRESULT Test146(LogProxy* pLog);
HRESULT Test147(LogProxy* pLog);
HRESULT Test148(LogProxy* pLog);
HRESULT Test149(LogProxy* pLog);
HRESULT Test150(LogProxy* pLog);
HRESULT Test151(LogProxy* pLog);
HRESULT Test152(LogProxy* pLog);
HRESULT Test153(LogProxy* pLog);
HRESULT Test154(LogProxy* pLog);
HRESULT Test155(LogProxy* pLog);
HRESULT Test156(LogProxy* pLog);
HRESULT Test157(LogProxy* pLog);
HRESULT Test158(LogProxy* pLog);
HRESULT Test159(LogProxy* pLog);
HRESULT Test160(LogProxy* pLog);
HRESULT Test161(LogProxy* pLog);
HRESULT Test162(LogProxy* pLog);
HRESULT Test163(LogProxy* pLog);
HRESULT Test164(LogProxy* pLog);
HRESULT Test165(LogProxy* pLog);
HRESULT Test166(LogProxy* pLog);
HRESULT Test167(LogProxy* pLog);
HRESULT Test168(LogProxy* pLog);
HRESULT Test169(LogProxy* pLog);
HRESULT Test170(LogProxy* pLog);
HRESULT Test171(LogProxy* pLog);
HRESULT Test172(LogProxy* pLog);
HRESULT Test173(LogProxy* pLog);
HRESULT Test174(LogProxy* pLog);
HRESULT Test175(LogProxy* pLog);
HRESULT Test176(LogProxy* pLog);
HRESULT Test177(LogProxy* pLog);
HRESULT Test178(LogProxy* pLog);
HRESULT Test179(LogProxy* pLog);
HRESULT Test180(LogProxy* pLog);
HRESULT Test181(LogProxy* pLog);
HRESULT Test182(LogProxy* pLog);
HRESULT Test183(LogProxy* pLog);
HRESULT Test184(LogProxy* pLog);
HRESULT Test185(LogProxy* pLog);
HRESULT Test186(LogProxy* pLog);
HRESULT Test187(LogProxy* pLog);
HRESULT Test188(LogProxy* pLog);
HRESULT Test189(LogProxy* pLog);
HRESULT Test190(LogProxy* pLog);
HRESULT Test191(LogProxy* pLog);
HRESULT Test192(LogProxy* pLog);
HRESULT Test193(LogProxy* pLog);
HRESULT Test194(LogProxy* pLog);
HRESULT Test195(LogProxy* pLog);
HRESULT Test196(LogProxy* pLog);
HRESULT Test197(LogProxy* pLog);
HRESULT Test198(LogProxy* pLog);
HRESULT Test199(LogProxy* pLog);
HRESULT Test200(LogProxy* pLog);
HRESULT Test201(LogProxy* pLog);
HRESULT Test202(LogProxy* pLog);
HRESULT Test203(LogProxy* pLog);
HRESULT Test204(LogProxy* pLog);
HRESULT Test205(LogProxy* pLog);
HRESULT Test206(LogProxy* pLog);
HRESULT Test207(LogProxy* pLog);
HRESULT Test208(LogProxy* pLog);
HRESULT Test209(LogProxy* pLog);
HRESULT Test210(LogProxy* pLog);
HRESULT Test211(LogProxy* pLog);
HRESULT Test212(LogProxy* pLog);
HRESULT Test213(LogProxy* pLog);
HRESULT Test214(LogProxy* pLog);
HRESULT Test215(LogProxy* pLog);
HRESULT Test216(LogProxy* pLog);
HRESULT Test217(LogProxy* pLog);
HRESULT Test218(LogProxy* pLog);
HRESULT Test219(LogProxy* pLog);
HRESULT Test220(LogProxy* pLog);
HRESULT Test221(LogProxy* pLog);
HRESULT Test222(LogProxy* pLog);
HRESULT Test223(LogProxy* pLog);
HRESULT Test224(LogProxy* pLog);
HRESULT Test225(LogProxy* pLog);
HRESULT Test226(LogProxy* pLog);
HRESULT Test227(LogProxy* pLog);
HRESULT Test228(LogProxy* pLog);
HRESULT Test229(LogProxy* pLog);
HRESULT Test230(LogProxy* pLog);
HRESULT Test231(LogProxy* pLog);
HRESULT Test232(LogProxy* pLog);
HRESULT Test233(LogProxy* pLog);
HRESULT Test234(LogProxy* pLog);
HRESULT Test235(LogProxy* pLog);
HRESULT Test236(LogProxy* pLog);
HRESULT Test237(LogProxy* pLog);
HRESULT Test238(LogProxy* pLog);
HRESULT Test239(LogProxy* pLog);
HRESULT Test240(LogProxy* pLog);
HRESULT Test241(LogProxy* pLog);
HRESULT Test242(LogProxy* pLog);
HRESULT Test243(LogProxy* pLog);
HRESULT Test244(LogProxy* pLog);
HRESULT Test245(LogProxy* pLog);
HRESULT Test246(LogProxy* pLog);
HRESULT Test247(LogProxy* pLog);
HRESULT Test248(LogProxy* pLog);
HRESULT Test249(LogProxy* pLog);
HRESULT Test250(LogProxy* pLog);
HRESULT Test251(LogProxy* pLog);
HRESULT Test252(LogProxy* pLog);
HRESULT Test253(LogProxy* pLog);
HRESULT Test254(LogProxy* pLog);
HRESULT Test255(LogProxy* pLog);
HRESULT Test256(LogProxy* pLog);
HRESULT Test257(LogProxy* pLog);
HRESULT Test258(LogProxy* pLog);
HRESULT Test259(LogProxy* pLog);
HRESULT Test260(LogProxy* pLog);
HRESULT Test261(LogProxy* pLog);
HRESULT Test262(LogProxy* pLog);
HRESULT Test263(LogProxy* pLog);
HRESULT Test264(LogProxy* pLog);
HRESULT Test265(LogProxy* pLog);
HRESULT Test266(LogProxy* pLog);
HRESULT Test267(LogProxy* pLog);
HRESULT Test268(LogProxy* pLog);
HRESULT Test269(LogProxy* pLog);
HRESULT Test270(LogProxy* pLog);
HRESULT Test271(LogProxy* pLog);
HRESULT Test272(LogProxy* pLog);
HRESULT Test273(LogProxy* pLog);
HRESULT Test274(LogProxy* pLog);
HRESULT Test275(LogProxy* pLog);
HRESULT Test276(LogProxy* pLog);
HRESULT Test277(LogProxy* pLog);
HRESULT Test278(LogProxy* pLog);
HRESULT Test279(LogProxy* pLog);
HRESULT Test280(LogProxy* pLog);
HRESULT Test281(LogProxy* pLog);
HRESULT Test282(LogProxy* pLog);
HRESULT Test283(LogProxy* pLog);
HRESULT Test284(LogProxy* pLog);
HRESULT Test285(LogProxy* pLog);
HRESULT Test286(LogProxy* pLog);
HRESULT Test287(LogProxy* pLog);
HRESULT Test288(LogProxy* pLog);
HRESULT Test289(LogProxy* pLog);
HRESULT Test290(LogProxy* pLog);
HRESULT Test291(LogProxy* pLog);
HRESULT Test292(LogProxy* pLog);
HRESULT Test293(LogProxy* pLog);
HRESULT Test294(LogProxy* pLog);
HRESULT Test295(LogProxy* pLog);
HRESULT Test296(LogProxy* pLog);
HRESULT Test297(LogProxy* pLog);
HRESULT Test298(LogProxy* pLog);
HRESULT Test299(LogProxy* pLog);
HRESULT Test300(LogProxy* pLog);
HRESULT Test301(LogProxy* pLog);
HRESULT Test302(LogProxy* pLog);
HRESULT Test303(LogProxy* pLog);
HRESULT Test304(LogProxy* pLog);
HRESULT Test305(LogProxy* pLog);
HRESULT Test306(LogProxy* pLog);
HRESULT Test307(LogProxy* pLog);
HRESULT Test308(LogProxy* pLog);
HRESULT Test309(LogProxy* pLog);
HRESULT Test310(LogProxy* pLog);
HRESULT Test311(LogProxy* pLog);
HRESULT Test312(LogProxy* pLog);
HRESULT Test313(LogProxy* pLog);
HRESULT Test314(LogProxy* pLog);
HRESULT Test315(LogProxy* pLog);
HRESULT Test316(LogProxy* pLog);
HRESULT Test317(LogProxy* pLog);
HRESULT Test318(LogProxy* pLog);
HRESULT Test319(LogProxy* pLog);
HRESULT Test320(LogProxy* pLog);
HRESULT Test321(LogProxy* pLog);
HRESULT Test322(LogProxy* pLog);
HRESULT Test323(LogProxy* pLog);
HRESULT Test324(LogProxy* pLog);
HRESULT Test325(LogProxy* pLog);
HRESULT Test326(LogProxy* pLog);
HRESULT Test327(LogProxy* pLog);
HRESULT Test328(LogProxy* pLog);
HRESULT Test329(LogProxy* pLog);
HRESULT Test330(LogProxy* pLog);
HRESULT Test331(LogProxy* pLog);
HRESULT Test332(LogProxy* pLog);
HRESULT Test333(LogProxy* pLog);
HRESULT Test334(LogProxy* pLog);
HRESULT Test335(LogProxy* pLog);
HRESULT Test336(LogProxy* pLog);
HRESULT Test337(LogProxy* pLog);
HRESULT Test338(LogProxy* pLog);
HRESULT Test339(LogProxy* pLog);
HRESULT Test340(LogProxy* pLog);
HRESULT Test341(LogProxy* pLog);
HRESULT Test342(LogProxy* pLog);
HRESULT Test343(LogProxy* pLog);
HRESULT Test344(LogProxy* pLog);
HRESULT Test345(LogProxy* pLog);
HRESULT Test346(LogProxy* pLog);
HRESULT Test347(LogProxy* pLog);
HRESULT Test348(LogProxy* pLog);
HRESULT Test349(LogProxy* pLog);
HRESULT Test350(LogProxy* pLog);
HRESULT Test351(LogProxy* pLog);
HRESULT Test352(LogProxy* pLog);
HRESULT Test353(LogProxy* pLog);
HRESULT Test440(LogProxy* pLog);
HRESULT Test441(LogProxy* pLog);
HRESULT Test442(LogProxy* pLog);
HRESULT Test443(LogProxy* pLog);
HRESULT Test444(LogProxy* pLog);
HRESULT Test445(LogProxy* pLog);
HRESULT Test446(LogProxy* pLog);
HRESULT Test447(LogProxy* pLog);
HRESULT Test448(LogProxy* pLog);
HRESULT Test449(LogProxy* pLog);
HRESULT Test450(LogProxy* pLog);
HRESULT Test451(LogProxy* pLog);
HRESULT Test452(LogProxy* pLog);
HRESULT Test453(LogProxy* pLog);
HRESULT Test454(LogProxy* pLog);
HRESULT Test455(LogProxy* pLog);
HRESULT Test456(LogProxy* pLog);
HRESULT Test457(LogProxy* pLog);
HRESULT Test458(LogProxy* pLog);
HRESULT Test459(LogProxy* pLog);
HRESULT Test460(LogProxy* pLog);
HRESULT Test461(LogProxy* pLog);
HRESULT Test462(LogProxy* pLog);
HRESULT Test463(LogProxy* pLog);
HRESULT Test464(LogProxy* pLog);
HRESULT Test465(LogProxy* pLog);
HRESULT Test466(LogProxy* pLog);
HRESULT Test467(LogProxy* pLog);
HRESULT Test468(LogProxy* pLog);
HRESULT Test469(LogProxy* pLog);
HRESULT Test470(LogProxy* pLog);
HRESULT Test471(LogProxy* pLog);
HRESULT Test472(LogProxy* pLog);
HRESULT Test473(LogProxy* pLog);
HRESULT Test474(LogProxy* pLog);
HRESULT Test475(LogProxy* pLog);
HRESULT Test476(LogProxy* pLog);
HRESULT Test477(LogProxy* pLog);
HRESULT Test478(LogProxy* pLog);
HRESULT Test479(LogProxy* pLog);
HRESULT Test480(LogProxy* pLog);
HRESULT Test481(LogProxy* pLog);
HRESULT Test482(LogProxy* pLog);
HRESULT Test483(LogProxy* pLog);
HRESULT Test484(LogProxy* pLog);
HRESULT Test485(LogProxy* pLog);
HRESULT Test486(LogProxy* pLog);
HRESULT Test487(LogProxy* pLog);
HRESULT Test488(LogProxy* pLog);
HRESULT Test489(LogProxy* pLog);
HRESULT Test490(LogProxy* pLog);
HRESULT Test491(LogProxy* pLog);
HRESULT Test492(LogProxy* pLog);
HRESULT Test493(LogProxy* pLog);
HRESULT Test494(LogProxy* pLog);
HRESULT Test495(LogProxy* pLog);
HRESULT Test496(LogProxy* pLog);
HRESULT Test497(LogProxy* pLog);
HRESULT Test498(LogProxy* pLog);
HRESULT Test501(LogProxy* pLog);
HRESULT Test502(LogProxy* pLog);
HRESULT Test503(LogProxy* pLog);
HRESULT Test504(LogProxy* pLog);
HRESULT Test505(LogProxy* pLog);
HRESULT Test506(LogProxy* pLog);
HRESULT Test507(LogProxy* pLog);
HRESULT Test508(LogProxy* pLog);
HRESULT Test509(LogProxy* pLog);
HRESULT Test510(LogProxy* pLog);
HRESULT Test511(LogProxy* pLog);
HRESULT Test512(LogProxy* pLog);
HRESULT Test513(LogProxy* pLog);
HRESULT Test514(LogProxy* pLog);
HRESULT Test515(LogProxy* pLog);
HRESULT Test516(LogProxy* pLog);
HRESULT Test517(LogProxy* pLog);
HRESULT Test518(LogProxy* pLog);
HRESULT Test519(LogProxy* pLog);
HRESULT Test520(LogProxy* pLog);
HRESULT Test521(LogProxy* pLog);
HRESULT Test522(LogProxy* pLog);
HRESULT Test523(LogProxy* pLog);
HRESULT Test524(LogProxy* pLog);
HRESULT Test525(LogProxy* pLog);
HRESULT Test526(LogProxy* pLog);
HRESULT Test527(LogProxy* pLog);
HRESULT Test528(LogProxy* pLog);
HRESULT Test529(LogProxy* pLog);
HRESULT Test530(LogProxy* pLog);
HRESULT Test531(LogProxy* pLog);
HRESULT Test532(LogProxy* pLog);
HRESULT Test533(LogProxy* pLog);
HRESULT Test534(LogProxy* pLog);
HRESULT Test535(LogProxy* pLog);
HRESULT Test536(LogProxy* pLog);
HRESULT Test537(LogProxy* pLog);
HRESULT Test538(LogProxy* pLog);
HRESULT Test539(LogProxy* pLog);
HRESULT Test540(LogProxy* pLog);
HRESULT Test541(LogProxy* pLog);
HRESULT Test542(LogProxy* pLog);
HRESULT Test543(LogProxy* pLog);
HRESULT Test544(LogProxy* pLog);
HRESULT Test545(LogProxy* pLog);
HRESULT Test546(LogProxy* pLog);
HRESULT Test547(LogProxy* pLog);
HRESULT Test548(LogProxy* pLog);
HRESULT Test549(LogProxy* pLog);
HRESULT Test550(LogProxy* pLog);
HRESULT Test551(LogProxy* pLog);
HRESULT Test552(LogProxy* pLog);
HRESULT Test553(LogProxy* pLog);
HRESULT Test554(LogProxy* pLog);
HRESULT Test555(LogProxy* pLog);
HRESULT Test556(LogProxy* pLog);
HRESULT Test557(LogProxy* pLog);
HRESULT Test558(LogProxy* pLog);
HRESULT Test559(LogProxy* pLog);
HRESULT Test560(LogProxy* pLog);
HRESULT Test561(LogProxy* pLog);
HRESULT Test562(LogProxy* pLog);
HRESULT Test563(LogProxy* pLog);
HRESULT Test564(LogProxy* pLog);
HRESULT Test565(LogProxy* pLog);
HRESULT Test566(LogProxy* pLog);
HRESULT Test567(LogProxy* pLog);
HRESULT Test568(LogProxy* pLog);
HRESULT Test569(LogProxy* pLog);
HRESULT Test570(LogProxy* pLog);
HRESULT Test571(LogProxy* pLog);
HRESULT Test572(LogProxy* pLog);
HRESULT Test573(LogProxy* pLog);
HRESULT Test574(LogProxy* pLog);
HRESULT Test575(LogProxy* pLog);
HRESULT Test576(LogProxy* pLog);
HRESULT Test577(LogProxy* pLog);
HRESULT Test578(LogProxy* pLog);
HRESULT Test579(LogProxy* pLog);
HRESULT Test580(LogProxy* pLog);
HRESULT Test581(LogProxy* pLog);
HRESULT Test582(LogProxy* pLog);
HRESULT Test583(LogProxy* pLog);
HRESULT Test584(LogProxy* pLog);
HRESULT Test585(LogProxy* pLog);
HRESULT Test586(LogProxy* pLog);
HRESULT Test587(LogProxy* pLog);
HRESULT Test588(LogProxy* pLog);
HRESULT Test589(LogProxy* pLog);
HRESULT Test590(LogProxy* pLog);
HRESULT Test591(LogProxy* pLog);
HRESULT Test592(LogProxy* pLog);
HRESULT Test593(LogProxy* pLog);
HRESULT Test594(LogProxy* pLog);
HRESULT Test595(LogProxy* pLog);
HRESULT Test596(LogProxy* pLog);
HRESULT Test597(LogProxy* pLog);
HRESULT Test598(LogProxy* pLog);
HRESULT Test599(LogProxy* pLog);
HRESULT Test600(LogProxy* pLog);
HRESULT Test601(LogProxy* pLog);
HRESULT Test602(LogProxy* pLog);
HRESULT Test603(LogProxy* pLog);
HRESULT Test604(LogProxy* pLog);
HRESULT Test605(LogProxy* pLog);
HRESULT Test606(LogProxy* pLog);
HRESULT Test607(LogProxy* pLog);
HRESULT Test608(LogProxy* pLog);
HRESULT Test609(LogProxy* pLog);
HRESULT Test610(LogProxy* pLog);

HRESULT TestS01(LogProxy* pLog);
HRESULT TestS02(LogProxy* pLog);
HRESULT TestS03(LogProxy* pLog);
HRESULT TestS04(LogProxy* pLog);
HRESULT TestS05(LogProxy* pLog);
HRESULT TestS06(LogProxy* pLog);
HRESULT TestS07(LogProxy* pLog);
HRESULT TestS08(LogProxy* pLog);
HRESULT TestS09(LogProxy* pLog);

HRESULT TestB01(LogProxy* pLog);
HRESULT TestB02(LogProxy* pLog);
HRESULT TestB03(LogProxy* pLog);
HRESULT TestB04(LogProxy* pLog);
HRESULT TestB05(LogProxy* pLog);
HRESULT TestB06(LogProxy* pLog);
HRESULT TestB07(LogProxy* pLog);
HRESULT TestB08(LogProxy* pLog);
HRESULT TestB09(LogProxy* pLog);

HRESULT TestO01(LogProxy* pLog);
HRESULT TestO02(LogProxy* pLog);
HRESULT TestO03(LogProxy* pLog);
HRESULT TestO04(LogProxy* pLog);
HRESULT TestO05(LogProxy* pLog);
HRESULT TestO06(LogProxy* pLog);
HRESULT TestO07(LogProxy* pLog);
HRESULT TestO08(LogProxy* pLog);

HRESULT TestF01(LogProxy* pLog);
HRESULT TestF02(LogProxy* pLog);
HRESULT TestF03(LogProxy* pLog);
HRESULT TestF04(LogProxy* pLog);
HRESULT TestF05(LogProxy* pLog);
HRESULT TestF06(LogProxy* pLog);
HRESULT TestF07(LogProxy* pLog);
HRESULT TestF08(LogProxy* pLog);

HRESULT TestT01(LogProxy* pLog);
HRESULT TestT02(LogProxy* pLog);

void AssignTests(void)
{
    tests[  1].funct = Test001; tests[  1].name = "Init";
    tests[ 49].funct = Test049; tests[ 49].name = "XMColorAdjustContrast";
    tests[ 50].funct = Test050; tests[ 50].name = "XMColorAdjustSaturation";
    tests[ 51].funct = Test051; tests[ 51].name = "XMColorEqual";
    tests[ 52].funct = Test052; tests[ 52].name = "XMColorGreater";
    tests[ 53].funct = Test053; tests[ 53].name = "XMColorGreaterOrEqual";
    tests[ 54].funct = Test054; tests[ 54].name = "XMColorIsInfinite";
    tests[ 55].funct = Test055; tests[ 55].name = "XMColorIsNaN";
    tests[ 56].funct = Test056; tests[ 56].name = "XMColorLess";
    tests[ 57].funct = Test057; tests[ 57].name = "XMColorLessOrEqual";
    tests[ 58].funct = Test058; tests[ 58].name = "XMColorModulate";
    tests[ 59].funct = Test059; tests[ 59].name = "XMColorNegative";
    tests[ 60].funct = Test060; tests[ 60].name = "XMColorNotEqual";
    tests[ 61].funct = Test061; tests[ 61].name = "XMConvertFloatToHalf";
    tests[ 62].funct = Test062; tests[ 62].name = "XMConvertFloatToHalfStream";
    tests[ 64].funct = Test064; tests[ 64].name = "XMConvertHalfToFloat";
    tests[ 65].funct = Test065; tests[ 65].name = "XMConvertHalfToFloatStream";
    tests[ 67].funct = Test067; tests[ 67].name = "XMFresnelTerm";
    tests[ 68].funct = Test068; tests[ 68].name = "XMLoadColor";
    tests[ 69].funct = Test069; tests[ 69].name = "XMLoadFloat/(S|U)Int2";
    tests[ 70].funct = Test070; tests[ 70].name = "XMLoadFloat/Int2A";
    tests[ 71].funct = Test071; tests[ 71].name = "XMLoadFloat/(S|U)Int3";
    tests[ 72].funct = Test072; tests[ 72].name = "XMLoadFloat/Int3A";
    tests[ 73].funct = Test073; tests[ 73].name = "XMLoadFloat3x3";
    tests[ 74].funct = Test074; tests[ 74].name = "XMLoadFloat/(S|U)Int4";
    tests[ 75].funct = Test075; tests[ 75].name = "XMLoadFloat/Int4A";
    tests[ 76].funct = Test076; tests[ 76].name = "XMLoadFloat4x3";
    tests[ 77].funct = Test077; tests[ 77].name = "XMLoadFloat4x4";
    tests[ 78].funct = Test078; tests[ 78].name = "XMLoadFloat4x4A";
    tests[ 79].funct = Test079; tests[ 79].name = "XMLoadHalf2";
    tests[ 80].funct = Test080; tests[ 80].name = "XMLoadHalf4";
    tests[ 81].funct = Test081; tests[ 81].name = "XMLoadXDecN4";
    tests[ 82].funct = Test082; tests[ 82].name = "XMLoadShortN2";
    tests[ 83].funct = Test083; tests[ 83].name = "XMLoadShortN4";
    tests[ 84].funct = Test084; tests[ 84].name = "XMMatrixAffineTransformation";
    tests[ 85].funct = Test085; tests[ 85].name = "XMMatrixAffineTransformation2D";
    tests[ 86].funct = Test086; tests[ 86].name = "XMMatrixDeterminant";
    tests[ 87].funct = Test087; tests[ 87].name = "XMMatrixIdentity";
    tests[ 88].funct = Test088; tests[ 88].name = "XMMatrixInverse";
    tests[ 89].funct = Test089; tests[ 89].name = "XMMatrixIsIdentity";
    tests[ 90].funct = Test090; tests[ 90].name = "XMMatrixIsInfinite";
    tests[ 91].funct = Test091; tests[ 91].name = "XMMatrixIsNaN";
    tests[ 92].funct = Test092; tests[ 92].name = "XMMatrixLookAtLH";
    tests[ 93].funct = Test093; tests[ 93].name = "XMMatrixLookAtRH";
    tests[ 94].funct = Test094; tests[ 94].name = "XMMatrixLookToLH";
    tests[ 95].funct = Test095; tests[ 95].name = "XMMatrixLookToRH";
    tests[ 96].funct = Test096; tests[ 96].name = "XMMatrixMultiply";
    tests[ 97].funct = Test097; tests[ 97].name = "XMMatrixMultiplyTranspose";
    tests[ 98].funct = Test098; tests[ 98].name = "XMMatrixOrthographicLH";
    tests[ 99].funct = Test099; tests[ 99].name = "XMMatrixOrthographicOffCenterLH";
    tests[100].funct = Test100; tests[100].name = "XMMatrixOrthographicOffCenterRH";
    tests[101].funct = Test101; tests[101].name = "XMMatrixOrthographicRH";
    tests[102].funct = Test102; tests[102].name = "XMMatrixPerspectiveFovLH";
    tests[103].funct = Test103; tests[103].name = "XMMatrixPerspectiveFovRH";
    tests[104].funct = Test104; tests[104].name = "XMMatrixPerspectiveLH";
    tests[105].funct = Test105; tests[105].name = "XMMatrixPerspectiveOffCenterLH";
    tests[106].funct = Test106; tests[106].name = "XMMatrixPerspectiveOffCenterRH";
    tests[107].funct = Test107; tests[107].name = "XMMatrixPerspectiveRH";
    tests[108].funct = Test108; tests[108].name = "XMMatrixReflect";
    tests[109].funct = Test109; tests[109].name = "XMMatrixRotationAxis";
    tests[110].funct = Test110; tests[110].name = "XMMatrixRotationNormal";
    tests[111].funct = Test111; tests[111].name = "XMMatrixRotationQuaternion";
    tests[112].funct = Test112; tests[112].name = "XMMatrixRotationRollPitchYaw";
    tests[113].funct = Test113; tests[113].name = "XMMatrixRotationRollPitchYawFromVector";
    tests[114].funct = Test114; tests[114].name = "XMMatrixRotationX";
    tests[115].funct = Test115; tests[115].name = "XMMatrixRotationY";
    tests[116].funct = Test116; tests[116].name = "XMMatrixRotationZ";
    tests[117].funct = Test117; tests[117].name = "XMMatrixScaling";
    tests[118].funct = Test118; tests[118].name = "XMMatrixScalingFromVector";
    tests[119].funct = Test119; tests[119].name = "XMMatrixShadow";
    tests[120].funct = Test120; tests[120].name = "XMMatrixTransformation";
    tests[121].funct = Test121; tests[121].name = "XMMatrixTransformation2D";
    tests[122].funct = Test122; tests[122].name = "XMMatrixTranslation";
    tests[123].funct = Test123; tests[123].name = "XMMatrixTranslationFromVector";
    tests[124].funct = Test124; tests[124].name = "XMMatrixTranspose";
    tests[125].funct = Test125; tests[125].name = "XMPlaneDot";
    tests[126].funct = Test126; tests[126].name = "XMPlaneDotCoord";
    tests[127].funct = Test127; tests[127].name = "XMPlaneDotNormal";
    tests[128].funct = Test128; tests[128].name = "XMPlaneEqual";
    tests[129].funct = Test129; tests[129].name = "XMPlaneFromPointNormal";
    tests[130].funct = Test130; tests[130].name = "XMPlaneFromPoints";
    tests[131].funct = Test131; tests[131].name = "XMPlaneIntersectLine";
    tests[132].funct = Test132; tests[132].name = "XMPlaneIntersectPlane";
    tests[133].funct = Test133; tests[133].name = "XMPlaneIsInfinite";
    tests[134].funct = Test134; tests[134].name = "XMPlaneIsNaN";
    tests[135].funct = Test135; tests[135].name = "XMPlaneNormalize";
    tests[136].funct = Test136; tests[136].name = "XMPlaneNotEqual";
    tests[137].funct = Test137; tests[137].name = "XMPlaneTransform";
    tests[138].funct = Test138; tests[138].name = "XMPlaneTransformStream";
    tests[139].funct = Test139; tests[139].name = "XMQuaternionBaryCentric[V]";
    tests[140].funct = Test140; tests[140].name = "XMQuaternionConjugate";
    tests[141].funct = Test141; tests[141].name = "XMQuaternionDot";
    tests[142].funct = Test142; tests[142].name = "XMQuaternionEqual";
    tests[143].funct = Test143; tests[143].name = "XMQuaternionExp";
    tests[144].funct = Test144; tests[144].name = "XMQuaternionIdentity";
    tests[145].funct = Test145; tests[145].name = "XMQuaternionInverse";
    tests[146].funct = Test146; tests[146].name = "XMQuaternionIsIdentity";
    tests[147].funct = Test147; tests[147].name = "XMQuaternionIsInfinite";
    tests[148].funct = Test148; tests[148].name = "XMQuaternionIsNaN";
    tests[149].funct = Test149; tests[149].name = "XMQuaternionLength";
    tests[150].funct = Test150; tests[150].name = "XMQuaternionLengthSq";
    tests[151].funct = Test151; tests[151].name = "XMQuaternionLn";
    tests[152].funct = Test152; tests[152].name = "XMQuaternionMultiply";
    tests[153].funct = Test153; tests[153].name = "XMQuaternionNormalize[Est]";
    tests[154].funct = Test154; tests[154].name = "XMQuaternionNotEqual";
    tests[155].funct = Test155; tests[155].name = "XMQuaternionReciprocalLength";
    tests[156].funct = Test156; tests[156].name = "XMQuaternionRotationAxis";
    tests[157].funct = Test157; tests[157].name = "XMQuaternionRotationMatrix";
    tests[158].funct = Test158; tests[158].name = "XMQuaternionRotationNormal";
    tests[159].funct = Test159; tests[159].name = "XMQuaternionRotationRollPitchYaw";
    tests[160].funct = Test160; tests[160].name = "XMQuaternionRotationRollPitchYawFromVector";
    tests[161].funct = Test161; tests[161].name = "XMQuaternionSlerp[V]";
    tests[162].funct = Test162; tests[162].name = "XMQuaternionSquad[V]";
    tests[163].funct = Test163; tests[163].name = "XMQuaternionSquadSetup";
    tests[164].funct = Test164; tests[164].name = "XMQuaternionToAxisAngle";
    tests[165].funct = Test165; tests[165].name = "XMScalarACos";
    tests[166].funct = Test166; tests[166].name = "XMScalarACosEst";
    tests[167].funct = Test167; tests[167].name = "XMScalarASin";
    tests[168].funct = Test168; tests[168].name = "XMScalarASinEst";
    tests[169].funct = Test169; tests[169].name = "XMScalarCos";
    tests[170].funct = Test170; tests[170].name = "XMScalarCosEst";
    tests[171].funct = Test171; tests[171].name = "XMScalarModAngle";
    tests[172].funct = Test172; tests[172].name = "XMScalarNearEqual";
    tests[173].funct = Test173; tests[173].name = "XMScalarSin";
    tests[174].funct = Test174; tests[174].name = "XMScalarSinCos";
    tests[175].funct = Test175; tests[175].name = "XMScalarSinCosEst";
    tests[176].funct = Test176; tests[176].name = "XMScalarSinEst";
    tests[177].funct = Test177; tests[177].name = "XMStoreColor";
    tests[178].funct = Test178; tests[178].name = "XMStoreFloat/(S|U)Int2";
    tests[179].funct = Test179; tests[179].name = "XMStoreFloat/Int2A";
    tests[180].funct = Test180; tests[180].name = "XMStoreFloat/(S|U)Int3";
    tests[181].funct = Test181; tests[181].name = "XMStoreFloat/Int3A";
    tests[182].funct = Test182; tests[182].name = "XMStoreFloat3x3";
    tests[183].funct = Test183; tests[183].name = "XMStoreFloat/(S|U)Int4";
    tests[184].funct = Test184; tests[184].name = "XMStoreFloat/Int4A";
    tests[185].funct = Test185; tests[185].name = "XMStoreFloat4x3";
    tests[186].funct = Test186; tests[186].name = "XMStoreFloat4x4";
    tests[187].funct = Test187; tests[187].name = "XMStoreFloat4x4A";
    tests[188].funct = Test188; tests[188].name = "XMStoreHalf2";
    tests[189].funct = Test189; tests[189].name = "XMStoreHalf4";
    tests[190].funct = Test190; tests[190].name = "XMStoreXDecN4";
    tests[191].funct = Test191; tests[191].name = "XMStoreShortN2";
    tests[192].funct = Test192; tests[192].name = "XMStoreShortN4";
    tests[193].funct = Test193; tests[193].name = "XMVector2AngleBetweenNormals";
    tests[194].funct = Test194; tests[194].name = "XMVector2AngleBetweenVectors";
    tests[195].funct = Test195; tests[195].name = "XMVector2ClampLength[V]";
    tests[196].funct = Test196; tests[196].name = "XMVector2Cross";
    tests[197].funct = Test197; tests[197].name = "XMVector2Dot";
    tests[198].funct = Test198; tests[198].name = "XMVector2Equal";
    tests[199].funct = Test199; tests[199].name = "XMVector2Greater";
    tests[200].funct = Test200; tests[200].name = "XMVector2GreaterOrEqual";
    tests[201].funct = Test201; tests[201].name = "XMVector2IntersectLine";
    tests[202].funct = Test202; tests[202].name = "XMVector2IsInfinite";
    tests[203].funct = Test203; tests[203].name = "XMVector2IsNaN";
    tests[204].funct = Test204; tests[204].name = "XMVector2Length";
    tests[205].funct = Test205; tests[205].name = "XMVector2LengthSq";
    tests[206].funct = Test206; tests[206].name = "XMVector2Less";
    tests[207].funct = Test207; tests[207].name = "XMVector2LessOrEqual";
    tests[208].funct = Test208; tests[208].name = "XMVector2LinePointDistance";
    tests[209].funct = Test209; tests[209].name = "XMVector2NearEqual";
    tests[210].funct = Test210; tests[210].name = "XMVector2Normalize";
    tests[211].funct = Test211; tests[211].name = "XMVector2NotEqual";
    tests[212].funct = Test212; tests[212].name = "XMVector2Orthogonal";
    tests[213].funct = Test213; tests[213].name = "XMVector2ReciprocalLength";
    tests[214].funct = Test214; tests[214].name = "XMVector2Reflect";
    tests[215].funct = Test215; tests[215].name = "XMVector2Refract[V]";
    tests[216].funct = Test216; tests[216].name = "XMVector2Transform";
    tests[217].funct = Test217; tests[217].name = "XMVector2TransformCoord";
    tests[218].funct = Test218; tests[218].name = "XMVector2TransformCoordStream";
    tests[219].funct = Test219; tests[219].name = "XMVector2TransformNormal";
    tests[220].funct = Test220; tests[220].name = "XMVector2TransformNormalStream";
    tests[221].funct = Test221; tests[221].name = "XMVector2TransformStream";
    tests[222].funct = Test222; tests[222].name = "XMVector3AngleBetweenNormals";
    tests[223].funct = Test223; tests[223].name = "XMVector3AngleBetweenVectors";
    tests[224].funct = Test224; tests[224].name = "XMVector3ClampLength[V]";
    tests[225].funct = Test225; tests[225].name = "XMVector3ComponentsFromNormal";
    tests[226].funct = Test226; tests[226].name = "XMVector3Cross";
    tests[227].funct = Test227; tests[227].name = "XMVector3Dot";
    tests[228].funct = Test228; tests[228].name = "XMVector3Equal";
    tests[229].funct = Test229; tests[229].name = "XMVector3Greater";
    tests[230].funct = Test230; tests[230].name = "XMVector3GreaterOrEqual";
    tests[231].funct = Test231; tests[231].name = "XMVector3IsInfinite";
    tests[232].funct = Test232; tests[232].name = "XMVector3IsNaN";
    tests[233].funct = Test233; tests[233].name = "XMVector3Length";
    tests[234].funct = Test234; tests[234].name = "XMVector3LengthSq";
    tests[235].funct = Test235; tests[235].name = "XMVector3Less";
    tests[236].funct = Test236; tests[236].name = "XMVector3LessOrEqual";
    tests[237].funct = Test237; tests[237].name = "XMVector3LinePointDistance";
    tests[238].funct = Test238; tests[238].name = "XMVector3NearEqual";
    tests[239].funct = Test239; tests[239].name = "XMVector3Normalize";
    tests[240].funct = Test240; tests[240].name = "XMVector3NotEqual";
    tests[241].funct = Test241; tests[241].name = "XMVector3Orthogonal";
    tests[242].funct = Test242; tests[242].name = "XMVector3Project";
    tests[243].funct = Test243; tests[243].name = "XMVector3ProjectStream";
    tests[244].funct = Test244; tests[244].name = "XMVector3ReciprocalLength";
    tests[245].funct = Test245; tests[245].name = "XMVector3Reflect";
    tests[246].funct = Test246; tests[246].name = "XMVector3Refract[V]";
    tests[247].funct = Test247; tests[247].name = "XMVector3Transform";
    tests[248].funct = Test248; tests[248].name = "XMVector3TransformCoord";
    tests[249].funct = Test249; tests[249].name = "XMVector3TransformCoordStream";
    tests[250].funct = Test250; tests[250].name = "XMVector3TransformNormal";
    tests[251].funct = Test251; tests[251].name = "XMVector3TransformNormalStream";
    tests[252].funct = Test252; tests[252].name = "XMVector3TransformStream";
    tests[253].funct = Test253; tests[253].name = "XMVector3Unproject";
    tests[254].funct = Test254; tests[254].name = "XMVector3UnprojectStream";
    tests[255].funct = Test255; tests[255].name = "XMVector4AngleBetweenNormals";
    tests[256].funct = Test256; tests[256].name = "XMVector4AngleBetweenVectors";
    tests[257].funct = Test257; tests[257].name = "XMVector4ClampLength[V]";
    tests[258].funct = Test258; tests[258].name = "XMVector4Cross";
    tests[259].funct = Test259; tests[259].name = "XMVector4Dot";
    tests[260].funct = Test260; tests[260].name = "XMVector4Equal";
    tests[261].funct = Test261; tests[261].name = "XMVector4Greater";
    tests[262].funct = Test262; tests[262].name = "XMVector4GreaterOrEqual";
    tests[263].funct = Test263; tests[263].name = "XMVector4IsInfinite";
    tests[264].funct = Test264; tests[264].name = "XMVector4IsNaN";
    tests[265].funct = Test265; tests[265].name = "XMVector4Length";
    tests[266].funct = Test266; tests[266].name = "XMVector4LengthSq";
    tests[267].funct = Test267; tests[267].name = "XMVector4Less";
    tests[268].funct = Test268; tests[268].name = "XMVector4LessOrEqual";
    tests[269].funct = Test269; tests[269].name = "XMVector4NearEqual";
    tests[270].funct = Test270; tests[270].name = "XMVector4Normalize[Est]";
    tests[271].funct = Test271; tests[271].name = "XMVector4NotEqual";
    tests[272].funct = Test272; tests[272].name = "XMVector4Orthogonal";
    tests[273].funct = Test273; tests[273].name = "XMVector4ReciprocalLength";
    tests[274].funct = Test274; tests[274].name = "XMVector4Reflect";
    tests[275].funct = Test275; tests[275].name = "XMVector4Refract[V]";
    tests[276].funct = Test276; tests[276].name = "XMVector4Transform";
    tests[277].funct = Test277; tests[277].name = "XMVector4TransformStream";
    tests[278].funct = Test278; tests[278].name = "XMVectorAbs";
    tests[279].funct = Test279; tests[279].name = "XMVectorACos";
    tests[280].funct = Test280; tests[280].name = "XMVectorACosEst";
    tests[281].funct = Test281; tests[281].name = "XMVectorAdd";
    tests[282].funct = Test282; tests[282].name = "XMVectorAddAngles";
    tests[283].funct = Test283; tests[283].name = "XMVectorASin";
    tests[284].funct = Test284; tests[284].name = "XMVectorASinEst";
    tests[285].funct = Test285; tests[285].name = "XMVectorATan";
    tests[286].funct = Test286; tests[286].name = "XMVectorATan2";
    tests[287].funct = Test287; tests[287].name = "XMVectorATan2Est";
    tests[288].funct = Test288; tests[288].name = "XMVectorATanEst";
    tests[289].funct = Test289; tests[289].name = "XMVectorBaryCentric[V]";
    tests[290].funct = Test290; tests[290].name = "XMVectorCatmullRom";
    tests[291].funct = Test291; tests[291].name = "XMVectorCeiling";
    tests[292].funct = Test292; tests[292].name = "XMVectorClamp";
    tests[293].funct = Test293; tests[293].name = "XMVectorCos";
    tests[294].funct = Test294; tests[294].name = "XMVectorCosEst";
    tests[295].funct = Test295; tests[295].name = "XMVectorCosH";
    tests[296].funct = Test296; tests[296].name = "XMVectorSum";
    tests[297].funct = Test297; tests[297].name = "XMVectorEqual";
    tests[298].funct = Test298; tests[298].name = "XMVectorExp";
    // Test299
    tests[300].funct = Test300; tests[300].name = "XMVectorFloor";
    tests[301].funct = Test301; tests[301].name = "XMVectorGreater";
    tests[302].funct = Test302; tests[302].name = "XMVectorGreaterOrEqual";
    tests[303].funct = Test303; tests[303].name = "XMVectorHermite";
    tests[304].funct = Test304; tests[304].name = "XMVectorIsInfinite";
    tests[305].funct = Test305; tests[305].name = "XMVectorIsNaN";
    tests[306].funct = Test306; tests[306].name = "XMVectorLerp[V]";
    tests[307].funct = Test307; tests[307].name = "XMVectorLess";
    tests[308].funct = Test308; tests[308].name = "XMVectorLessOrEqual";
    tests[309].funct = Test309; tests[309].name = "XMVectorLog";
    // Test310
    tests[311].funct = Test311; tests[311].name = "XMVectorMaximize";
    tests[312].funct = Test312; tests[312].name = "XMVectorMinimize";
    tests[313].funct = Test313; tests[313].name = "XMVectorMod";
    tests[314].funct = Test314; tests[314].name = "XMVectorModAngles";
    tests[315].funct = Test315; tests[315].name = "XMVectorMultiply";
    tests[316].funct = Test316; tests[316].name = "XMVectorMultiplyAdd";
    tests[317].funct = Test317; tests[317].name = "XMVectorNearEqual";
    tests[318].funct = Test318; tests[318].name = "XMVectorNegate";
    tests[319].funct = Test319; tests[319].name = "XMVectorNegativeMultiplySubtract";
    tests[320].funct = Test320; tests[320].name = "XMVectorNotEqual";
    tests[321].funct = Test321; tests[321].name = "XMVectorPermute";
    tests[322].funct = Test322; tests[322].name = "XMConvertToRadians/Degrees";
    tests[323].funct = Test323; tests[323].name = "XMVectorReciprocal";
    tests[324].funct = Test324; tests[324].name = "XMVectorReciprocalEst";
    tests[325].funct = Test325; tests[325].name = "XMVectorReciprocalSqrt";
    tests[326].funct = Test326; tests[326].name = "XMVectorReciprocalSqrtEst";
    tests[327].funct = Test327; tests[327].name = "XMVectorReplicate[Int][Ptr]";
    tests[328].funct = Test328; tests[328].name = "XMVectorRound";
    tests[329].funct = Test329; tests[329].name = "XMVectorSaturate";
    tests[330].funct = Test330; tests[330].name = "XMVectorScale";
    tests[331].funct = Test331; tests[331].name = "XMVectorSelect";
    tests[332].funct = Test332; tests[332].name = "XMVectorSelectControl";
    tests[333].funct = Test333; tests[333].name = "XMVectorSet";
    tests[334].funct = Test334; tests[334].name = "XMVectorSin";
    tests[335].funct = Test335; tests[335].name = "XMVectorSinCos";
    tests[336].funct = Test336; tests[336].name = "XMVectorSinCosEst";
    tests[337].funct = Test337; tests[337].name = "XMVectorSinEst";
    tests[338].funct = Test338; tests[338].name = "XMVectorSinH";
    // Test339
    tests[340].funct = Test340; tests[340].name = "XMVectorSplatW";
    tests[341].funct = Test341; tests[341].name = "XMVectorSplatX";
    tests[342].funct = Test342; tests[342].name = "XMVectorSplatY";
    tests[343].funct = Test343; tests[343].name = "XMVectorSplatZ";
    tests[344].funct = Test344; tests[344].name = "XMVectorSqrt";
    tests[345].funct = Test345; tests[345].name = "XMVectorSqrtEst";
    tests[346].funct = Test346; tests[346].name = "XMVectorSubtract";
    tests[347].funct = Test347; tests[347].name = "XMVectorSubtractAngles";
    tests[348].funct = Test348; tests[348].name = "XMVectorTan";
    tests[349].funct = Test349; tests[349].name = "XMVectorTanEst";
    tests[350].funct = Test350; tests[350].name = "XMVectorTanH";
    // Test351
    tests[352].funct = Test352; tests[352].name = "XMVectorTruncate";
    tests[353].funct = Test353; tests[353].name = "XMVectorZero";
    tests[440].funct = Test440; tests[440].name = "XMVectorEqualInt";
    tests[441].funct = Test441; tests[441].name = "XMVectorNotEqualInt";
    tests[442].funct = Test442; tests[442].name = "XMVector2AngleBetweenNormalsEst";
    tests[443].funct = Test443; tests[443].name = "XMVector3AngleBetweenNormalsEst";
    tests[444].funct = Test444; tests[444].name = "XMVector4AngleBetweenNormalsEst";
    tests[445].funct = Test445; tests[445].name = "XMVector2ReciprocalLengthEst";
    tests[446].funct = Test446; tests[446].name = "XMVector2LengthEst";
    tests[447].funct = Test447; tests[447].name = "XMVector3ReciprocalLengthEst";
    tests[448].funct = Test448; tests[448].name = "XMVector3LengthEst";
    tests[449].funct = Test449; tests[449].name = "XMVector4ReciprocalLengthEst";
    tests[450].funct = Test450; tests[450].name = "XMVector4LengthEst";
    tests[451].funct = Test451; tests[451].name = "XMVectorSetInt";
    tests[452].funct = Test452; tests[452].name = "XMVectorTrueInt";
    tests[453].funct = Test453; tests[453].name = "XMVectorFalseInt";
    tests[454].funct = Test454; tests[454].name = "XMVectorEqualInt";
    tests[455].funct = Test455; tests[455].name = "XMVectorNotEqualInt";
    tests[456].funct = Test456; tests[456].name = "XMVectorAndInt";
    tests[457].funct = Test457; tests[457].name = "XMVectorAndCInt";
    tests[458].funct = Test458; tests[458].name = "XMVectorOrInt";
    tests[459].funct = Test459; tests[459].name = "XMVectorNorInt";
    tests[460].funct = Test460; tests[460].name = "XMVectorXorInt";
    tests[461].funct = Test461; tests[461].name = "XMVector2EqualInt";
    tests[462].funct = Test462; tests[462].name = "XMVector2NotEqualInt";
    tests[463].funct = Test463; tests[463].name = "XMVector3EqualInt";
    tests[464].funct = Test464; tests[464].name = "XMVector3NotEqualInt";
    tests[465].funct = Test465; tests[465].name = "XMVector4EqualInt";
    tests[466].funct = Test466; tests[466].name = "XMVector4NotEqualInt";
    tests[467].funct = Test467; tests[467].name = "XMVectorEqualR";
    tests[468].funct = Test468; tests[468].name = "XMVectorEqualIntR";
    tests[469].funct = Test469; tests[469].name = "XMVectorGreaterR";
    tests[470].funct = Test470; tests[470].name = "XMVectorGreaterOrEqualR";
    tests[471].funct = Test471; tests[471].name = "XMVectorInBounds";
    tests[472].funct = Test472; tests[472].name = "XMVectorInBoundsR";
    tests[473].funct = Test473; tests[473].name = "XMVector2EqualR";
    tests[474].funct = Test474; tests[474].name = "XMVector2EqualIntR";
    tests[475].funct = Test475; tests[475].name = "XMVector2GreaterR";
    tests[476].funct = Test476; tests[476].name = "XMVector2GreaterOrEqualR";
    tests[477].funct = Test477; tests[477].name = "XMVector2InBounds";
    // Test478
    tests[479].funct = Test479; tests[479].name = "XMVector3EqualR";
    tests[480].funct = Test480; tests[480].name = "XMVector3EqualIntR";
    tests[481].funct = Test481; tests[481].name = "XMVector3GreaterR";
    tests[482].funct = Test482; tests[482].name = "XMVector3GreaterOrEqualR";
    tests[483].funct = Test483; tests[483].name = "XMVector3InBounds";
    // Test484
    tests[485].funct = Test485; tests[485].name = "XMVector4EqualR";
    tests[486].funct = Test486; tests[486].name = "XMVector4EqualIntR";
    tests[487].funct = Test487; tests[487].name = "XMVector4GreaterR";
    tests[488].funct = Test488; tests[488].name = "XMVector4GreaterOrEqualR";
    tests[489].funct = Test489; tests[489].name = "XMVector4InBounds";
    // Test490
    tests[491].funct = Test491; tests[491].name = "XMVector3Rotate";
    tests[492].funct = Test492; tests[492].name = "XMVector3InverseRotate";
    tests[493].funct = Test493; tests[493].name = "XMVectorSwizzle";
    tests[494].funct = Test494; tests[494].name = "XMVectorRotateLeft";
    tests[495].funct = Test495; tests[495].name = "XMVectorRotateRight";
    tests[496].funct = Test496; tests[496].name = "XMPlaneNearEqual";

    tests[497].funct = Test497; tests[497].name = "XMLoadFloat4x3A";
    tests[498].funct = Test498; tests[498].name = "XMStoreFloat4x3A";

    tests[501].funct = Test501; tests[501].name = "XMPlaneNormalizeEst";
    tests[502].funct = Test502; tests[502].name = "XMVector3NormalizeEst";
    tests[503].funct = Test503; tests[503].name = "XMVector2NormalizeEst";
    tests[504].funct = Test504; tests[504].name = "XMVectorSplatQNaN";
    tests[505].funct = Test505; tests[505].name = "XMVectorSplatInfinity";
    tests[506].funct = Test506; tests[506].name = "XMVectorPow[Est]";
    tests[507].funct = Test507; tests[507].name = "XMVectorHermiteV";
    tests[508].funct = Test508; tests[508].name = "XMVectorCatmullRomV";
    tests[509].funct = Test509; tests[509].name = "XMLoadByte4";
    tests[510].funct = Test510; tests[510].name = "XMLoadByteN4";
    tests[511].funct = Test511; tests[511].name = "XMLoadUByte4";
    tests[512].funct = Test512; tests[512].name = "XMLoadUByteN4";
    tests[513].funct = Test513; tests[513].name = "XMLoadDec4";
    tests[514].funct = Test514; tests[514].name = "XMLoadDecN4";
    tests[515].funct = Test515; tests[515].name = "XMLoadUDec4";
    tests[516].funct = Test516; tests[516].name = "XMLoadUDecN4[_XR]";
    tests[517].funct = Test517; tests[517].name = "XMLoadXDec4";

    tests[518].funct = Test518; tests[518].name = "XMLoadFloat3x4";
    tests[519].funct = Test519; tests[519].name = "XMLoadFloat3x4A";
    tests[520].funct = Test520; tests[520].name = "XMStoreFloat3x4";
    tests[521].funct = Test521; tests[521].name = "XMStoreFloat3x4A";
    // Test522
    // Test523

    tests[524].funct = Test524; tests[524].name = "XMLoadShort4";
    tests[525].funct = Test525; tests[525].name = "XMLoadUShort4";
    tests[526].funct = Test526; tests[526].name = "XMLoadUShortN4";

    // Test527
    // Test528
    // Test529
    // Test530
    // Test531
    // Test532
    // Test533
    // Test534
  
    tests[535].funct = Test535; tests[535].name = "XMLoadShort2";   
    tests[536].funct = Test536; tests[536].name = "XMLoadUShort2";   
    tests[537].funct = Test537; tests[537].name = "XMLoadUShortN2";   
    tests[538].funct = Test538; tests[538].name = "XMStoreUShortN2";
    tests[539].funct = Test539; tests[539].name = "XMStoreUShort2";
    tests[540].funct = Test540; tests[540].name = "XMStoreShort2";

    // Test541
    // Test542
    // Test543
    // Test544
    // Test545
    // Test546
    // Test547
    // Test548

    tests[549].funct = Test549; tests[549].name = "XMStoreShort4";  
    tests[550].funct = Test550; tests[550].name = "XMStoreUShortN4";
    tests[551].funct = Test551; tests[551].name = "XMStoreUShort4"; 

    // Test552
    // Test553
    // Test554
    // Test555
    // Test556
    // Test557

    tests[558].funct = Test558; tests[558].name = "XMStoreXDec4";  
    tests[559].funct = Test559; tests[559].name = "XMStoreUDecN4[_XR]";  
    tests[560].funct = Test560; tests[560].name = "XMStoreUDec4";  
    tests[561].funct = Test561; tests[561].name = "XMStoreDecN4"; 
    tests[562].funct = Test562; tests[562].name = "XMStoreDec4"; 
    tests[563].funct = Test563; tests[563].name = "XMStoreUByteN4";
    tests[564].funct = Test564; tests[564].name = "XMStoreUByte4"; 
    tests[565].funct = Test565; tests[565].name = "XMStoreByteN4"; 
    tests[566].funct = Test566; tests[566].name = "XMStoreByte4"; 
    tests[567].funct = Test567; tests[567].name = "XMConvertVector[U]IntToFloat"; 
    tests[568].funct = Test568; tests[568].name = "XMConvertVectorFloatTo[U]Int"; 
    tests[569].funct = Test569; tests[569].name = "XMVectorInsert"; 
    tests[570].funct = Test570; tests[570].name = "XMVectorSplatOne";
    tests[571].funct = Test571; tests[571].name = "XMVectorSplatEpsilon";
    tests[572].funct = Test572; tests[572].name = "XMVectorSplatSignMask";
    tests[573].funct = Test573; tests[573].name = "XMVectorMergeXY";
    tests[574].funct = Test574; tests[574].name = "XMVectorMergeZW";
    tests[575].funct = Test575; tests[575].name = "XMMatrixSet";
    tests[576].funct = Test576; tests[576].name = "XMLoadFloat/Int";
    tests[577].funct = Test577; tests[577].name = "XMStoreFloat/Int";
    tests[578].funct = Test578; tests[578].name = "XMMatrixDecompose";
    tests[579].funct = Test579; tests[579].name = "XMVectorSetBinaryConstant";
    tests[580].funct = Test580; tests[580].name = "XMVectorSplatConstant[Int]";
    tests[581].funct = Test581; tests[581].name = "XMVectorShiftLeft";
    tests[582].funct = Test582; tests[582].name = "XMLoadU565";
    tests[583].funct = Test583; tests[583].name = "XMStoreU565";
    tests[584].funct = Test584; tests[584].name = "XMLoadUNibble4";
    tests[585].funct = Test585; tests[585].name = "XMStoreUNibble4";
    tests[586].funct = Test586; tests[586].name = "XMLoadU555";
    tests[587].funct = Test587; tests[587].name = "XMStoreU555";
    tests[588].funct = Test588; tests[588].name = "XMLoadFloat3PK";
    tests[589].funct = Test589; tests[589].name = "XMStoreFloat3PK";
    tests[590].funct = Test590; tests[590].name = "XMLoadFloat3SE";
    tests[591].funct = Test591; tests[591].name = "XMStoreFloat3SE";
    tests[592].funct = Test592; tests[592].name = "XMVectorDivide";

    // Test593
    // Test594

    tests[595].funct = Test595; tests[595].name = "XMLoadByteN2";
    tests[596].funct = Test596; tests[596].name = "XMLoadByte2";
    tests[597].funct = Test597; tests[597].name = "XMLoadUByteN2";
    tests[598].funct = Test598; tests[598].name = "XMLoadUByte2";
    tests[599].funct = Test599; tests[599].name = "XMStoreByteN2";
    tests[600].funct = Test600; tests[600].name = "XMStoreByte2";
    tests[601].funct = Test601; tests[601].name = "XMStoreUByteN2";
    tests[602].funct = Test602; tests[602].name = "XMStoreUByte2";
    tests[603].funct = Test603; tests[603].name = "XMColorRGBToHSL/HSLToRGB";
    tests[604].funct = Test604; tests[604].name = "XMColorRGBToHSV/HSVToRGB";
    tests[605].funct = Test605; tests[605].name = "XMColorRGBToYUV/YUVToRGB";
    tests[606].funct = Test606; tests[606].name = "XMColorRGBToYUV_HD/YUVToRGB_HD";
    tests[607].funct = Test607; tests[607].name = "XMColorRGBToXYZ/XYZToRGB";
    tests[608].funct = Test608; tests[608].name = "XMColorXYZToSRGB/SRGBToXYZ";
    tests[609].funct = Test609; tests[609].name = "XMMATRIX::operator";
    tests[610].funct = Test610; tests[610].name = "XMColorRGBToSRGB/SRGBToRGB";

    tests[700].funct = TestS01; tests[700].name = "BoundingSphere";
    tests[701].funct = TestS02; tests[701].name = "Sphere::Transform";
    tests[702].funct = TestS03; tests[702].name = "Sphere::Contains";
    tests[703].funct = TestS04; tests[703].name = "Sphere::Intersects";
    tests[704].funct = TestS05; tests[704].name = "Sphere::ContainedBy";
    tests[705].funct = TestS06; tests[705].name = "Sphere::CreateMerged";
    tests[706].funct = TestS07; tests[706].name = "Sphere::CreateFromBoundingBox";
    tests[707].funct = TestS08; tests[707].name = "Sphere::CreateFromPoints";
    tests[708].funct = TestS09; tests[708].name = "Sphere::CreateFromFrustum";
    
    tests[710].funct = TestB01; tests[710].name = "BoundingBox";
    tests[711].funct = TestB02; tests[711].name = "BBox::Transform";
    tests[712].funct = TestB03; tests[712].name = "BBox::GetCorners";
    tests[713].funct = TestB04; tests[713].name = "BBox::Contains";
    tests[714].funct = TestB05; tests[714].name = "BBox::Intersects";
    tests[715].funct = TestB06; tests[715].name = "BBox::ContainedBy";
    tests[716].funct = TestB07; tests[716].name = "BBox::CreateMerged";
    tests[717].funct = TestB08; tests[717].name = "BBox::CreateFromSphere";
    tests[718].funct = TestB09; tests[718].name = "BBox::CreateFromPoints";

    tests[719].funct = TestO01; tests[719].name = "BoundingOrientedBox";
    tests[720].funct = TestO02; tests[720].name = "OBox::Transform";
    tests[721].funct = TestO03; tests[721].name = "OBox::GetCorners";
    tests[722].funct = TestO04; tests[722].name = "OBox::Contains";
    tests[723].funct = TestO05; tests[723].name = "OBox::Intersects";
    tests[724].funct = TestO06; tests[724].name = "OBox::ContainedBy";
    tests[725].funct = TestO07; tests[725].name = "OBox::CreateFromBoundingBox";
    tests[726].funct = TestO08; tests[726].name = "OBox::CreateFromPoints";

    tests[727].funct = TestF01; tests[727].name = "BoundingFrustum";
    tests[728].funct = TestF02; tests[728].name = "Frustum::Transform";
    tests[729].funct = TestF03; tests[729].name = "Frustum::GetCorners";
    tests[730].funct = TestF04; tests[730].name = "Frustum::Contains";
    tests[731].funct = TestF05; tests[731].name = "Frustum::Intersects";
    tests[732].funct = TestF06; tests[732].name = "Frustum::ContainedBy";
    tests[733].funct = TestF07; tests[733].name = "Frustum::GetPlanes";
    tests[734].funct = TestF08; tests[734].name = "Frustum::CreateFromMatrix";

    tests[735].funct = TestT01; tests[735].name = "TriTests::Intersects";
    tests[736].funct = TestT02; tests[736].name = "TriTests::ContainedBy";
}
