// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <cassert>
#include <cstdio>
#include <cstdint>

#include <xmmintrin.h>

#ifdef _MSC_VER
#include <excpt.h>
#include <intrin.h>
#endif

#if defined(__clang__) || defined(__GNUC__)
#include <cpuid.h>
#include <x86intrin.h>
#endif

int main()
{
#ifdef __clang__
    printf("cpuid using clang\n");
#elif defined(__MINGW32__)
    printf("cpuid using MinGW\n");
#elif defined(__GNUC__)
    printf("cpuid using GCC\n");
#elif defined(_MSC_VER)
    printf("cpuid using Visual C++\n");
#else
#error Unknown compiler
#endif

   unsigned int x = _mm_getcsr();
   printf("%08X\n", x );

#ifdef _m_prefetchw
   bool prefetchw = false;
#endif
   bool osxsave = false;

   // See http://msdn.microsoft.com/en-us/library/hskdteyh.aspx
   int CPUInfo[4] = { -1 };
#if defined(__clang__) || defined(__GNUC__)
   __cpuid(0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#else
   __cpuid(CPUInfo, 0);
#endif

   char vendor[0x20] = {};
   *reinterpret_cast<int*>(vendor) = CPUInfo[1];
   *reinterpret_cast<int*>(vendor + 4) = CPUInfo[3];
   *reinterpret_cast<int*>(vendor + 8) = CPUInfo[2];

   printf("Vendor %s\n", vendor);

   bool checkextfeature = (CPUInfo[0] >= 7);

   if ( CPUInfo[0] > 0 )
   {
#if defined(__clang__) || defined(__GNUC__)
       __cpuid(1, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#else
       __cpuid(CPUInfo, 1);
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

       if ( CPUInfo[2] & 0x2000000 ) // bit 25
           printf("AES-NI\n");

       if ( CPUInfo[2] & 0x800000 ) // bit 23
           printf("POPCNT\n");

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

       if ( CPUInfo[2] & 0x2 ) // bit 1
          printf("CLMUL\n");

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

   if ( checkextfeature )
   {
#if defined(__clang__) || defined(__GNUC__)
       __cpuid_count(7, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#else
       __cpuidex(CPUInfo, 7, 0);
#endif

       // EBX
       if ( CPUInfo[1] & 0x80000000 ) // bit 31
         printf("AVX512-VL\n"); // Vector Length

       if ( CPUInfo[1] & 0x40000000 ) // bit 30
         printf("AVX512-BW\n"); // Byte and Word

       if ( CPUInfo[1] & 0x20000000 ) // bit 29
         printf("SHA\n"); // SHA1 and SHA-256

       if ( CPUInfo[1] & 0x10000000 ) // bit 28
         printf("AVX512-CD\n"); // Conflict Detection

       if ( CPUInfo[1] & 0x8000000 ) // bit 27
         printf("AVX512-ER\n"); // Exponential and Reciprocal

       if ( CPUInfo[1] & 0x4000000 ) // bit 26
         printf("AVX512-PF\n"); // PreFetch

       if ( CPUInfo[1] & 0x200000 ) // bit 21
         printf("AVX512-IFMA\n"); // Integer Fused Multiply-Add

       if ( CPUInfo[1] & 0x80000 ) // bit 19
         printf("ADX\n");

       if ( CPUInfo[1] & 0x40000 ) // bit 18
         printf("RDSEED\n");

       if ( CPUInfo[1] & 0x20000 ) // bit 17
         printf("AVX512-DQ\n"); // Double and Quadword

       if ( CPUInfo[1] & 0x10000 ) // bit 16
         printf("AVX512F\n"); // Foundation

       if ( CPUInfo[1] & 0x100 ) // bit 8
         printf("BMI2\n");

       if ( CPUInfo[1] & 0x20 ) // bit 5
         printf("AVX2\n");

       if ( CPUInfo[1] & 0x8 ) // bit 3
         printf("BMI\n");

       // ECX
       if ( CPUInfo[2] & 0x4000 ) // bit 14
          printf("AVX512-VPOPCNTDQ\n");

       if ( CPUInfo[2] & 0x1000 ) // bit 12
          printf("AVX512-BITALG\n");

       if ( CPUInfo[2] & 0x800 ) // bit 11
          printf("AVX512-VNNI\n"); // Vector Neural Network Instructions

       if ( CPUInfo[2] & 0x400 ) // bit 10
          printf("CLMUL (256-bit)\n");

       if ( CPUInfo[2] & 0x200 ) // bit 9
          printf("VAES\n");

       if ( CPUInfo[2] & 0x80 ) // bit 7
          printf("CET\n");

       if ( CPUInfo[2] & 0x40 ) // bit 6
          printf("AVX512-VBMI2\n"); // Vector Bit Manipulation 2

       if ( CPUInfo[2] & 0x2 ) // bit 1
          printf("AVX512-VBMI\n"); // Vector Bit Manipulation

       // EDX
       if ( CPUInfo[3] & 0x800000 ) // bit 23
           printf("AVX512-FP16\n");

       if ( CPUInfo[3] & 0x400000 ) // bit 22
           printf("BF16\n");

       if ( CPUInfo[3] & 0x100 ) // bit 8
           printf("AVX512-VP2INTERSECT\n");

       if ( CPUInfo[3] & 0x8 ) // bit 3
           printf("AVX512-4FMAPS\n");

       if ( CPUInfo[3] & 0x4 ) // bit 2
           printf("AVX512-4NNIW\n");
   }
   else
       printf("CPU doesn't support Structured Extended Feature Flags\n");

   // Extended features
#if defined(__clang__) || defined(__GNUC__)
   __cpuid(0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#else
   __cpuid(CPUInfo, 0x80000000);
#endif

   if (uint32_t(CPUInfo[0]) > 0x80000000)
   {
#if defined(__clang__) || defined(__GNUC__)
       __cpuid(0x80000001, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#else
       __cpuid(CPUInfo, 0x80000001);
#endif

       // ECX
       if ( CPUInfo[2] & 0x10000 ) // bit 16
           printf("FMA4\n");

       if ( CPUInfo[2] & 0x800 ) // bit 11
           printf("XOP\n");

       if ( CPUInfo[2] & 0x100 ) // bit 8
       {
           printf("PREFETCHW\n");
#ifdef _m_prefetchw
           prefetchw = true;
#endif
       }

       if ( CPUInfo[2] & 0x80 ) // bit 7
           printf("Misalign SSE\n");

       if ( CPUInfo[2] & 0x40 ) // bit 6
           printf("SSE4A\n");

       if ( CPUInfo[2] & 0x20 ) // bit 5
           printf("LZCNT/ABM\n");

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

#ifndef __MINGW32__
   if ( osxsave )
   {
        uint32_t xcr0;
        // Could use GetEnabledXStateFeatures on Windows 7 SP1+, but requires Windows.h
        xcr0 = static_cast<uint32_t>(_xgetbv(0));
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
#endif

   return 0;
}
