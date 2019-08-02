//-------------------------------------------------------------------------------------
// F16C/CVT16 extensions tester for SIMD C++ Math library
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include <Windows.h>

#include <stdio.h>

#include "DirectXMathF16C.h"

using namespace DirectX;

int main()
{
    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");
    }

    if ( F16C::XMVerifyF16CSupport() )
    {
        printf("F16C supported\n");
    }

    return 0;
}
