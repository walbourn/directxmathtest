//-------------------------------------------------------------------------------------
// hdrtest.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This test file ensures that the headers do not have external include dependancies (such as windows.h)

// Off by default warnings
#pragma warning(disable : 4514 4668) 
// C4514 'function' : unreferenced inline function has been removed 
// C4668 not defined as a preprocessor macro

#include <directxmath.h>
#include <directxcolors.h>
#include <directxpackedvector.h>
#include <directxcollision.h>
