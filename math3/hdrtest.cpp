// This test file ensures that the headers do not have external include dependancies (such as windows.h)

#pragma warning(disable : 4514 4668) // Disable "off by default" warnings when bulding with /Wall

#pragma warning(disable : 5029) // Can remove when __vector4i is marked deprecated

#include <directxmath.h>
#include <directxcolors.h>
#include <directxpackedvector.h>
#include <directxcollision.h>
