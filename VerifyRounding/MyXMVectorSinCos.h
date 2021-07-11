#include <DirectXMath.h>

using namespace DirectX;

// from DirectXMath 3.16

inline XMVECTOR XM_CALLCONV MyXMVectorRound(FXMVECTOR V) noexcept
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTORF32 Result = { { {
            Internal::round_to_nearest(V.vector4_f32[0]),
            Internal::round_to_nearest(V.vector4_f32[1]),
            Internal::round_to_nearest(V.vector4_f32[2]),
            Internal::round_to_nearest(V.vector4_f32[3])
        } } };
    return Result.v;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
    return vrndnq_f32(V);
#else
    uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(V), g_XMNegativeZero);
    float32x4_t sMagic = vreinterpretq_f32_u32(vorrq_u32(g_XMNoFraction, sign));
    float32x4_t R1 = vaddq_f32(V, sMagic);
    R1 = vsubq_f32(R1, sMagic);
    float32x4_t R2 = vabsq_f32(V);
    uint32x4_t mask = vcleq_f32(R2, g_XMNoFraction);
    return vbslq_f32(mask, R1, V);
#endif
#elif defined(_XM_SSE4_INTRINSICS_)
    return _mm_round_ps(V, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
#elif defined(_XM_SSE_INTRINSICS_)
//    __m128 sign = _mm_and_ps(V, g_XMNegativeZero);
//    __m128 sMagic = _mm_or_ps(g_XMNoFraction, sign);
//    __m128 R1 = _mm_add_ps(V, sMagic);
//    R1 = _mm_sub_ps(R1, sMagic);
//    __m128 R2 = _mm_and_ps(V, g_XMAbsMask);
//    __m128 mask = _mm_cmple_ps(R2, g_XMNoFraction);
//    R2 = _mm_andnot_ps(mask, V);
//    R1 = _mm_and_ps(R1, mask);
//    XMVECTOR vResult = _mm_xor_ps(R1, R2);
//    return vResult;
	return _mm_cvtepi32_ps(_mm_cvtps_epi32(V));						// <--- modified by shinjich for testing
#endif
}

inline XMVECTOR XM_CALLCONV MyXMVectorModAngles(FXMVECTOR Angles) noexcept
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;
    XMVECTOR Result;

    // Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
    V = XMVectorMultiply(Angles, g_XMReciprocalTwoPi.v);
    V = XMVectorRound(V);
    Result = XMVectorNegativeMultiplySubtract(g_XMTwoPi.v, V, Angles);
    return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    // Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
    XMVECTOR vResult = vmulq_f32(Angles, g_XMReciprocalTwoPi);
    // Use the inline function due to complexity for rounding
    vResult = XMVectorRound(vResult);
    return vmlsq_f32(Angles, vResult, g_XMTwoPi);
#elif defined(_XM_SSE_INTRINSICS_)
    // Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
    XMVECTOR vResult = _mm_mul_ps(Angles, g_XMReciprocalTwoPi);
    // Use the inline function due to complexity for rounding
//    vResult = XMVectorRound(vResult);
    vResult = MyXMVectorRound(vResult);								// <--- modified by shinjich for testing
    return XM_FNMADD_PS(vResult, g_XMTwoPi, Angles);
#endif
}

_Use_decl_annotations_
inline void XM_CALLCONV MyXMVectorSinCos
(
    XMVECTOR* pSin,
    XMVECTOR* pCos,
    FXMVECTOR V
) noexcept
{
    assert(pSin != nullptr);
    assert(pCos != nullptr);

    // 11/10-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
    XMVECTORF32 Sin = { { {
            sinf(V.vector4_f32[0]),
            sinf(V.vector4_f32[1]),
            sinf(V.vector4_f32[2]),
            sinf(V.vector4_f32[3])
        } } };

    XMVECTORF32 Cos = { { {
            cosf(V.vector4_f32[0]),
            cosf(V.vector4_f32[1]),
            cosf(V.vector4_f32[2]),
            cosf(V.vector4_f32[3])
        } } };

    *pSin = Sin.v;
    *pCos = Cos.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    // Force the value within the bounds of pi
    XMVECTOR x = XMVectorModAngles(V);

    // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
    uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), g_XMNegativeZero);
    uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
    float32x4_t absx = vabsq_f32(x);
    float32x4_t  rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
    uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
    x = vbslq_f32(comp, x, rflx);
    float32x4_t fsign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

    float32x4_t x2 = vmulq_f32(x, x);

    // Compute polynomial approximation for sine
    const XMVECTOR SC1 = g_XMSinCoefficients1;
    const XMVECTOR SC0 = g_XMSinCoefficients0;
    XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
    XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

    vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
    Result = vmlaq_f32(vConstants, Result, x2);

    vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
    Result = vmlaq_f32(vConstants, Result, x2);

    vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
    Result = vmlaq_f32(vConstants, Result, x2);

    Result = vmlaq_f32(g_XMOne, Result, x2);
    *pSin = vmulq_f32(Result, x);

    // Compute polynomial approximation for cosine
    const XMVECTOR CC1 = g_XMCosCoefficients1;
    const XMVECTOR CC0 = g_XMCosCoefficients0;
    vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
    Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

    vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
    Result = vmlaq_f32(vConstants, Result, x2);

    vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
    Result = vmlaq_f32(vConstants, Result, x2);

    vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
    Result = vmlaq_f32(vConstants, Result, x2);

    Result = vmlaq_f32(g_XMOne, Result, x2);
    *pCos = vmulq_f32(Result, fsign);
#elif defined(_XM_SVML_INTRINSICS_)
    *pSin = _mm_sincos_ps(pCos, V);
#elif defined(_XM_SSE_INTRINSICS_)
    // Force the value within the bounds of pi
//    XMVECTOR x = XMVectorModAngles(V);
    XMVECTOR x = MyXMVectorModAngles(V);								// <--- modified by shinjich for testing

    // Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
    XMVECTOR sign = _mm_and_ps(x, g_XMNegativeZero);
    __m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
    __m128 absx = _mm_andnot_ps(sign, x);  // |x|
    __m128 rflx = _mm_sub_ps(c, x);
    __m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
    __m128 select0 = _mm_and_ps(comp, x);
    __m128 select1 = _mm_andnot_ps(comp, rflx);
    x = _mm_or_ps(select0, select1);
    select0 = _mm_and_ps(comp, g_XMOne);
    select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
    sign = _mm_or_ps(select0, select1);

    __m128 x2 = _mm_mul_ps(x, x);

    // Compute polynomial approximation of sine
    const XMVECTOR SC1 = g_XMSinCoefficients1;
    __m128 vConstantsB = XM_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
    const XMVECTOR SC0 = g_XMSinCoefficients0;
    __m128 vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
    __m128 Result = XM_FMADD_PS(vConstantsB, x2, vConstants);

    vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
    Result = XM_FMADD_PS(Result, x2, vConstants);

    vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
    Result = XM_FMADD_PS(Result, x2, vConstants);

    vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
    Result = XM_FMADD_PS(Result, x2, vConstants);

    Result = XM_FMADD_PS(Result, x2, g_XMOne);
    Result = _mm_mul_ps(Result, x);
    *pSin = Result;

    // Compute polynomial approximation of cosine
    const XMVECTOR CC1 = g_XMCosCoefficients1;
    vConstantsB = XM_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
    const XMVECTOR CC0 = g_XMCosCoefficients0;
    vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
    Result = XM_FMADD_PS(vConstantsB, x2, vConstants);

    vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
    Result = XM_FMADD_PS(Result, x2, vConstants);

    vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
    Result = XM_FMADD_PS(Result, x2, vConstants);

    vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
    Result = XM_FMADD_PS(Result, x2, vConstants);

    Result = XM_FMADD_PS(Result, x2, g_XMOne);
    Result = _mm_mul_ps(Result, sign);
    *pCos = Result;
#endif
}
