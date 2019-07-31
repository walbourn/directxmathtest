# DirectXMath Test Suite
#
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.
#
# http://go.microsoft.com/fwlink/?LinkID=615560

cmake_minimum_required (VERSION 3.8)
project (DirectXMath-CMake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")

set(WarningsEXE "-Wall" "-Wpedantic" "-Wextra"
    "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
    "-Wno-gnu-anonymous-struct" "-Wno-nested-anon-types"
    "-Wno-float-equal" "-Wno-double-promotion"
    "-Wno-missing-variable-declarations" "-Wno-missing-prototypes"
    "-Wno-reserved-id-macro" "-Wno-undef" )

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

add_executable(xdsptest
xdsp/Test.cpp)

source_group(Math3 REGULAR_EXPRESSION math3/*.*)
source_group(XDSPTest REGULAR_EXPRESSION xdsp/*.*)

target_include_directories( math3 PUBLIC ../inc )
target_include_directories( xdsptest PUBLIC ../inc ../XDSP )

target_compile_options( math3 PRIVATE /EHsc /fp:fast )
target_compile_options( xdsptest PRIVATE /EHsc /fp:fast )

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_compile_options( math3 PRIVATE ${WarningsEXE} )
    target_compile_options( xdsptest PRIVATE ${WarningsEXE} )
    if (${CMAKE_SIZEOF_VOID_P} EQUAL "4")
        target_compile_options( math3 PRIVATE /arch:SSE2 )
        target_compile_options( xdsptest PRIVATE /arch:SSE2 )
    endif()
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    target_compile_options( math3 PRIVATE /Wall /permissive- /Zc:__cplusplus )
    target_compile_options( mxdsptest PRIVATE /Wall /permissive- /Zc:__cplusplus )
endif()
