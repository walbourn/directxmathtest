// Copyright(c) Microsoft Corporation.All rights reserved.
// Licensed under the MIT License.

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <xmmintrin.h>

#ifdef _MSC_VER
#include <excpt.h>
#include <intrin.h>
#else
#include <cpuid.h>
#include <x86intrin.h>
#endif

int main()
{
#ifdef __clang__
    printf("cpuid using clang\n");
#elif __GNUC__
    printf("cpuid using GCC\n");
#else
    printf("cpuid using Visual C++\n");
#endif

   unsigned int x = _mm_getcsr();
   printf("%08X\n", x );

   bool prefetchw = false;
   bool osxsave = false;

   // See http://msdn.microsoft.com/en-us/library/hskdteyh.aspx
   int CPUInfo[4] = { -1 };
#ifdef _MSC_VER
    __cpuid(CPUInfo, 0); 
#else
   __cpuid(0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#endif

   if ( CPUInfo[0] > 0 )
   {
#ifdef _MSC_VER
    __cpuid(CPUInfo, 1);
#else
    __cpuid(1, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);      
#endif

       // EAX
       {
           int stepping = (CPUInfo[0] & 0xf);
           int basemodel = (CPUInfo[0] >> 4) & 0xf;
           int basefamily = (CPUInfo[0] >> 8) & 0xf;
           int xmodel = (CPUInfo[0] >> 16) & 0xf;
           int xfamily = (CPUInfo[0] >> 20) & 0xff;

           int family = basefamily + xfamily;
           int model = (xmodel << 4) | basemodel;

           printf("Family %02X, Model %02X, Stepping %u\n", family, model, stepping );
       }

       // ECX
       if ( CPUInfo[2] & 0x20000000 ) // bit 29
          printf("F16C\n");

       if ( CPUInfo[2] & 0x10000000 ) // bit 28
          printf("AVX\n");

       if ( CPUInfo[2] & 0x8000000 ) // bit 27
       {
          osxsave = true;
          printf("OSXSAVE\n");
       }

       if ( CPUInfo[2] & 0x400000 ) // bit 22
          printf("MOVBE\n");

       if ( CPUInfo[2] & 0x100000 ) // bit 20
          printf("SSE4.2\n");

       if ( CPUInfo[2] & 0x80000 ) // bit 19
          printf("SSE4.1\n");

       if ( CPUInfo[2] & 0x2000 ) // bit 13
          printf("CMPXCHANG16B\n");

       if ( CPUInfo[2] & 0x1000 ) // bit 12
          printf("FMA3\n");

       if ( CPUInfo[2] & 0x200 ) // bit 9
          printf("SSSE3\n");

       if ( CPUInfo[2] & 0x1 ) // bit 0
          printf("SSE3\n");

       // EDX
       if ( CPUInfo[3] & 0x4000000 ) // bit 26
           printf("SSE2\n");

       if ( CPUInfo[3] & 0x2000000 ) // bit 25
           printf("SSE\n");

       if ( CPUInfo[3] & 0x800000 ) // bit 23
           printf("MMX\n");
   }
   else
       printf("CPU doesn't support Feature Identifiers\n");

   if ( CPUInfo[0] >= 7 )
   {
#ifdef _MSC_VER
        __cpuidex(CPUInfo, 7, 0);
#else
       __cpuid_count(7, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#endif

       // EBX
       if ( CPUInfo[1] & 0x10000000000 ) // bit 28
         printf("AVX512CD\n"); // Conflict Detection

       if ( CPUInfo[1] & 0x8000000000 ) // bit 27
         printf("AVX512ER\n"); // Exponential and Reciprocal

       if ( CPUInfo[1] & 0x4000000000 ) // bit 26
         printf("AVX512PF\n"); // PreFetch

       if ( CPUInfo[1] & 0x2000000 ) // bit 19
         printf("ADX\n");

       if ( CPUInfo[1] & 0x1000000 ) // bit 18
         printf("RDSEED\n");

       if ( CPUInfo[1] & 0x10000 ) // bit 16
         printf("AVX512F\n"); // Foundation

       if ( CPUInfo[1] & 0x100 ) // bit 8
         printf("BMI2\n");

       if ( CPUInfo[1] & 0x20 ) // bit 5
         printf("AVX2\n");

       if ( CPUInfo[1] & 0x8 ) // bit 3
         printf("BMI\n");
   }
   else
       printf("CPU doesn't support Structured Extended Feature Flags\n");

   // Extended features
#ifdef _MSC_VER
    __cpuid(CPUInfo, 0x80000000);
#else
    __cpuid(0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#endif

   if (uint32_t(CPUInfo[0]) > 0x80000000)
   {
#ifdef _MSC_VER
    __cpuid(CPUInfo, 0x80000001);
#else
    __cpuid(0x80000001, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);    
#endif

       // ECX
       if ( CPUInfo[2] & 0x10000 ) // bit 16
           printf("FMA4\n");

       if ( CPUInfo[2] & 0x800 ) // bit 11
           printf("XOP\n");

       if ( CPUInfo[2] & 0x100 ) // bit 8
       {
           printf("PREFETCHW\n");
           prefetchw = true;
       }

       if ( CPUInfo[2] & 0x80 ) // bit 7
           printf("Misalign SSE\n");

       if ( CPUInfo[2] & 0x40 ) // bit 6
           printf("SSE4A\n");

       if ( CPUInfo[2] & 0x20 ) // bit 5
           printf("LZCNT\n");

       if ( CPUInfo[2] & 0x1 ) // bit 0
           printf("LAHF/SAHF\n");

       // EDX
       if ( uint32_t(CPUInfo[3]) & 0x80000000 ) // bit 31
           printf("3DNow!\n");

       if ( CPUInfo[3] & 0x40000000 ) // bit 30
           printf("3DNowExt!\n");

       if ( CPUInfo[3] & 0x20000000 ) // bit 29
           printf("x64\n");

       if ( CPUInfo[3] & 0x100000 ) // bit 20
           printf("NX\n");
   }
   else
       printf("CPU doesn't support Extended Feature Identifiers\n");

#ifdef _m_prefetchw
   if ( !prefetchw )
   {
       bool illegal = false;

       __try
       {
           static const unsigned int s_data = 0xabcd0123;

           _m_prefetchw(&s_data);
       }
       __except (EXCEPTION_EXECUTE_HANDLER)
       {
           illegal = true;
       }

       if (illegal)
       {
           printf("PREFETCHW is an invalid instruction on this processor\n");
       }
   }
#endif

   if ( osxsave )
   {
        uint32_t xcr0;
#if (_WIN32_WINNT >= 0x0601 /* Windows 7, assumes Service Pack 1 */)
        xcr0 = static_cast<uint32_t>(GetEnabledXStateFeatures());
#else
        xcr0 = static_cast<uint32_t>(_xgetbv(0));
#endif
        assert(xcr0 & 0x1); // x87
        if (xcr0 & 0x2)
        {
            printf("XMM\n");
        }
        if (xcr0 & 0x4)
        {
            printf("YMM\n");
        }
        if ((xcr0 & 0xE0) == 0xE0)
        {
            printf("ZMM\n");
        }
   }

   return 0;
}
