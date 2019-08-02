//-------------------------------------------------------------------------------------
// SSE4.1 extensions tester for SIMD C++ Math library
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include <Windows.h>
#include <stdio.h>

#include "DirectXMathSSE4.h"

using namespace DirectX;

int main()
{
    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");
    }

    if ( SSE4::XMVerifySSE4Support() )
    {
        printf("SSE4 supported\n");
    }

    return 0;
}
