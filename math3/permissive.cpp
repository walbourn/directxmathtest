//-------------------------------------------------------------------------------------
// permissive.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

// This file has /permissive- when building with VS 2017

// Off by default warnings
#pragma warning(disable : 4514 4668) 
// C4514 'function' : unreferenced inline function has been removed 
// C4668 not defined as a preprocessor macro

#include <directxmath.h>
#include <directxcolors.h>
#include <directxpackedvector.h>
#include <directxcollision.h>
