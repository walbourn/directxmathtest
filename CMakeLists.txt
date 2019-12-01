# DirectXMath Test Suite
#
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.
#
# http://go.microsoft.com/fwlink/?LinkID=615560

cmake_minimum_required (VERSION 3.11)

project (DirectXMath LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")

# Use max Warning Level 
string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

# Not using typeid or dynamic_cast, so disable RTTI to save binary size
string(REPLACE "/GR " "/GR- " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(REPLACE "/GR " "/GR- " CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REPLACE "/GR " "/GR- " CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

add_executable(cpuid cpuid/cpuid.cpp)

add_executable(math3
    math3/box.cpp
    math3/constexpr.cpp
    math3/frustum.cpp
    math3/math3.cpp
    math3/math3tests.cpp
    math3/obox.cpp
    math3/shared.cpp
    math3/sphere.cpp
    math3/triangle.cpp
    math3/xmcolor.cpp
    math3/xmmat.cpp
    math3/xmquat.cpp
    math3/xmvec.cpp
    math3/xmvec234.cpp)

add_executable(shmathtest
    shmath/test.cpp
    shmath/DDSTextureLoader.cpp
    shmath/DDSTextureLoader.h
    shmath/DDSTextureLoader12.cpp
    shmath/DDSTextureLoader12.h
    ../SHMath/DirectXSH.cpp
    ../SHMath/DirectXSH.h
    ../SHMath/DirectXSHD3D11.cpp
    ../SHMath/DirectXSHD3D12.cpp)

add_executable(testavx ext/testavx.cpp)
add_executable(testavx2 ext/testavx2.cpp)
add_executable(testbe ext/testbe.cpp)
add_executable(testf16c ext/testf16c.cpp)
add_executable(testfma3 ext/testfma3.cpp)
add_executable(testfma4 ext/testfma4.cpp)
add_executable(testsse3 ext/testsse3.cpp)
add_executable(testsse4 ext/testsse4.cpp)

add_executable(xdsptest xdsp/Test.cpp)

source_group(cpuid REGULAR_EXPRESSION cpuid/*.*)
source_group(Ext REGULAR_EXPRESSION ext/*.*)
source_group(Math3 REGULAR_EXPRESSION math3/*.*)
source_group(shmathtest REGULAR_EXPRESSION shmath/*.*)
source_group(XDSPTest REGULAR_EXPRESSION xdsp/*.*)

target_include_directories( math3 PUBLIC ../inc )
target_include_directories( shmathtest PUBLIC ../inc ../SHMath )
target_include_directories( testavx PUBLIC ../inc ../Extensions )
target_include_directories( testavx2 PUBLIC ../inc ../Extensions )
target_include_directories( testbe PUBLIC ../inc ../Extensions )
target_include_directories( testf16c PUBLIC ../inc ../Extensions )
target_include_directories( testfma3 PUBLIC ../inc ../Extensions )
target_include_directories( testfma4 PUBLIC ../inc ../Extensions )
target_include_directories( testsse3 PUBLIC ../inc ../Extensions )
target_include_directories( testsse4 PUBLIC ../inc ../Extensions )
target_include_directories( xdsptest PUBLIC ../inc ../XDSP )

target_compile_options( math3 PRIVATE /fp:fast )
target_compile_options( shmathtest PRIVATE /fp:fast )
target_compile_options( testavx PRIVATE /fp:fast )
target_compile_options( testavx2 PRIVATE /fp:fast )
target_compile_options( testbe PRIVATE /fp:fast )
target_compile_options( testf16c PRIVATE /fp:fast )
target_compile_options( testfma3 PRIVATE /fp:fast )
target_compile_options( testfma4 PRIVATE /fp:fast )
target_compile_options( testsse3 PRIVATE /fp:fast )
target_compile_options( testsse4 PRIVATE /fp:fast )
target_compile_options( xdsptest PRIVATE /fp:fast )

target_compile_definitions( shmathtest PRIVATE USE_DIRECT3D12=1 )

target_link_libraries( shmathtest d3d11.lib dxgi.lib d3d12.lib dxguid.lib )

if (${CMAKE_SIZEOF_VOID_P} EQUAL "4")
    target_compile_options( math3 PRIVATE /arch:SSE2 )
    target_compile_options( shmathtest PRIVATE /arch:SSE2 )
    target_compile_options( testsse3 PRIVATE /arch:SSE2 )
    target_compile_options( testsse4 PRIVATE /arch:SSE2 )
    target_compile_options( xdsptest PRIVATE /arch:SSE2 )
endif()

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    set(WarningsEXE "-Wall" "-Wpedantic" "-Wextra"
        "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
        "-Wno-gnu-anonymous-struct" "-Wno-nested-anon-types"
        "-Wno-float-equal" "-Wno-double-promotion"
        "-Wno-missing-variable-declarations" "-Wno-missing-prototypes"
        "-Wno-global-constructors"
        "-Wno-reserved-id-macro" "-Wno-undef" )

    target_compile_options( cpuid PRIVATE ${WarningsEXE} )
    target_compile_options( math3 PRIVATE ${WarningsEXE} )
    target_compile_options( shmathtest PRIVATE ${WarningsEXE} )
    target_compile_options( testavx PRIVATE ${WarningsEXE} /arch:AVX )
    target_compile_options( testavx2 PRIVATE ${WarningsEXE} /arch:AVX2 )
    target_compile_options( testbe PRIVATE ${WarningsEXE} -mssse3 )
    target_compile_options( testf16c PRIVATE ${WarningsEXE} -mf16c )
    target_compile_options( testfma3 PRIVATE ${WarningsEXE} /arch:AVX2 )
    target_compile_options( testfma4 PRIVATE ${WarningsEXE} /arch:AVX -mfma4 )
    target_compile_options( testsse3 PRIVATE ${WarningsEXE} -msse3 )
    target_compile_options( testsse4 PRIVATE ${WarningsEXE} )
    target_compile_options( xdsptest PRIVATE ${WarningsEXE} )
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    target_compile_options( cpuid PRIVATE /permissive- /Zc:__cplusplus )
    target_compile_options( math3 PRIVATE /permissive- /Zc:__cplusplus )
    target_compile_options( shmathtest PRIVATE /permissive- /Zc:__cplusplus )
    target_compile_options( testavx PRIVATE /permissive- /Zc:__cplusplus /arch:AVX )
    target_compile_options( testavx2 PRIVATE /permissive- /Zc:__cplusplus /arch:AVX2 )
    target_compile_options( testbe PRIVATE /permissive- /Zc:__cplusplus )
    target_compile_options( testf16c PRIVATE /permissive- /Zc:__cplusplus /arch:AVX )
    target_compile_options( testfma3 PRIVATE /permissive- /Zc:__cplusplus /arch:AVX2 )
    target_compile_options( testfma4 PRIVATE /permissive- /Zc:__cplusplus /arch:AVX )
    target_compile_options( testsse3 PRIVATE /permissive- /Zc:__cplusplus )
    target_compile_options( testsse4 PRIVATE /permissive- /Zc:__cplusplus )
    target_compile_options( xdsptest PRIVATE /permissive- /Zc:__cplusplus )
endif()
