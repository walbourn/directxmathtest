#include <windows.h>
#include <stdio.h>

#include "DirectXMathSSE4.h"

using namespace DirectX;

void main()
{
    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");
    }

    if ( SSE4::XMVerifySSE4Support() )
    {
        printf("SSE4 supported\n");
    }
}