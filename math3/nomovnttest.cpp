//-------------------------------------------------------------------------------------
// nomovnttest.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This test file validates the optional _XM_NO_MOVNT_ define

// Off by default warnings
#pragma warning(disable : 4514 4668 4820)
// C4514 'function' : unreferenced inline function has been removed 
// C4668 not defined as a preprocessor macro
// C4820 padding added after data member

#define _XM_NO_MOVNT_
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
