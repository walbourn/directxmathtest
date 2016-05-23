#include <windows.h>
#include <stdio.h>

#include "DirectXMathF16C.h"

using namespace DirectX;

void main()
{
    if ( XMVerifyCPUSupport() )
    {
        printf("CPU supported for SSE/SSE2\n");
    }

    if ( F16C::XMVerifyF16CSupport() )
    {
        printf("F16C supported\n");
    }
}