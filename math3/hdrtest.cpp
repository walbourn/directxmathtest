// This test file ensures that the headers do not have external include dependancies (such as windows.h)

#pragma warning(disable : 4514 4668) // Disable "off by default" warnings when bulding with /Wall

#include <directxmath.h>
#include <directxcolors.h>
#include <directxpackedvector.h>
#include <directxcollision.h>
