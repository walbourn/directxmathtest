//-------------------------------------------------------------------------------------
// cpp17compat.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This file has /std:c++17 when building with VS 2017 or later

#if __cplusplus != 201703L
#if defined(_MSVC_LANG) && _MSVC_LANG == 201703L
#error Add /Zc:__cplusplus to the build settings
#endif
#error This file should be built with C++17 mode enabled
#endif

// Off by default warnings
#pragma warning(disable : 4514 4668 4820)
// C4514 'function' : unreferenced inline function has been removed 
// C4668 not defined as a preprocessor macro
// C4820 padding added after data member

#pragma warning(disable: 26812)
// 26812: Prefer 'enum class' over 'enum' (Enum.3).

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
