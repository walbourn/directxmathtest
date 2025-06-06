//-------------------------------------------------------------------------------------
// cpp20compat.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This file has /std:c++20 when building with VS 2022 or later

#if __cplusplus != 202002L
#if defined(_MSVC_LANG) && _MSVC_LANG == 202002L
#error Add /Zc:__cplusplus to the build settings
#endif
#error This file should be built with C++20 mode enabled
#endif

#ifdef _MSC_VER
// Off by default warnings
#pragma warning(disable : 4619 4616 4514 4668 4820 5264)
// C4619/4616 #pragma warning warnings
// C4514 'function' : unreferenced inline function has been removed
// C4668 not defined as a preprocessor macro
// C4820 padding added after data member
// C5264 'const' variable is not used

#pragma warning(disable: 26812)
// 26812: Prefer 'enum class' over 'enum' (Enum.3).
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
