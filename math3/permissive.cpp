//-------------------------------------------------------------------------------------
// permissive.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This file has /permissive- when building with VS 2017 or later

// Off by default warnings
#pragma warning(disable : 4619 4616 4514 4668 4820 5264)
// C4619/4616 #pragma warning warnings
// C4514 'function' : unreferenced inline function has been removed
// C4668 not defined as a preprocessor macro
// C4820 padding added after data member
// C5264 'const' variable is not used

#pragma warning(disable: 26812)
// 26812: Prefer 'enum class' over 'enum' (Enum.3).

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
