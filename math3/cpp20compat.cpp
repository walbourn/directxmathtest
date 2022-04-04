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
#error This file should be built with C++20 mode enabled
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
