//-------------------------------------------------------------------------------------
// cpp17compat.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This file has /std:c++17 when building with VS 2017

#if __cplusplus != 201703
#if !defined(_MSVC_LANG) || _MSVC_LANG != 201703
#error This file should be built with C++17 mode enabled
#endif
#endif

// Off by default warnings
#pragma warning(disable : 4514 4668 4820)
// C4514 'function' : unreferenced inline function has been removed 
// C4668 not defined as a preprocessor macro
// C4820 padding added after data member

#include <directxmath.h>
#include <directxcolors.h>
#include <directxpackedvector.h>
#include <directxcollision.h>
