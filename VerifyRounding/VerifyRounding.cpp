#include <stdio.h>
#include <cmath>
#include "MyXMVectorSinCos.h"

using namespace DirectX;

int main()
{
	for (float f = -3.0f * XM_PI; f <= 3.0f * XM_PI; f += 0.1f)
	{
		XMVECTOR v = XMVectorSet(f, f, f, f);
		XMVECTOR vSin, vCos;
		XMVECTOR vSinWoRound, vCosWoRound;
		XMVectorSinCos( &vSin, &vCos, v );
		MyXMVectorSinCos( &vSinWoRound, &vCosWoRound, v );

		printf_s(
			"sincos(%f)=\n"
			"Standard library  : %f, %f\n"
			"XMVectorSinCos    : %f, %f (diff=%f, %f)\n"
			"XMVec wo/ Rounding: %f, %f (diff=%f, %f)\n"
			, f
			, std::sin(f), std::cos(f)
			, XMVectorGetX(vSin), XMVectorGetX(vCos), std::sin(f) - XMVectorGetX(vSin), std::cos(f) - XMVectorGetX(vCos)
			, XMVectorGetX(vSinWoRound), XMVectorGetX(vCosWoRound), std::sin(f) - XMVectorGetX(vSinWoRound), std::cos(f) - XMVectorGetX(vCosWoRound)
		);
	}
}
