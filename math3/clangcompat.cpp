// This tests the option for supporting clang by disbling non-portable overloads of XMVECTOR

#pragma warning(disable : 4514 4668) // Disable "off by default" warnings when bulding with /Wall

#define _XM_NO_XMVECTOR_OVERLOADS_

#include <directxmath.h>
#include <directxcolors.h>
#include <directxpackedvector.h>
#include <directxcollision.h>
