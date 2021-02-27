//-------------------------------------------------------------------------------------
// clangcompat.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This tests the option for supporting clang by disbling non-portable overloads of XMVECTOR

// Off by default warnings
#pragma warning(disable : 4514 4668 4820)
// C4514 'function' : unreferenced inline function has been removed 
// C4668 not defined as a preprocessor macro
// C4820 padding added after data member

#pragma warning(disable: 26812)
// 26812: Prefer 'enum class' over 'enum' (Enum.3).

#define _XM_NO_XMVECTOR_OVERLOADS_

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
