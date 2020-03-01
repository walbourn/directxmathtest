# DirectXMath Test Suite
#
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.
#
# http://go.microsoft.com/fwlink/?LinkID=615560

cmake_minimum_required (VERSION 3.8)
project (DirectXMath-CMake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")

include_directories(../Inc ../Extensions ../SHMath ../XDSP)

#Optimization flags
set(ARCH_AVX   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx> $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>)
set(ARCH_AVX2  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx2 -mfma -mf16c> $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>)
set(ARCH_F16C  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mf16c>)
set(ARCH_FMA   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mfma>)
set(ARCH_FMA4  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mfma4>)
set(ARCH_SSE2  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse2> $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2>)
set(ARCH_SSE3  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse3>)
set(ARCH_SSSE3 $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mssse3>)

#compile options
if(MSVC)
    add_compile_options("/W3" "/Wall" "/EHsc" "/fp:fast" "/permissive-" "/Zc:__cplusplus" "/wd4514")
else()
    add_compile_options(
        "-Wall" "-Wpedantic" "-Wextra"
        "-Wno-float-equal" "-Wno-double-promotion" "-Wno-missing-prototypes"
        "-Wno-undef" "-Wno-unknown-pragmas"
    )
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    add_compile_options(
        "-Wno-reserved-id-macro" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
        "-Wno-gnu-anonymous-struct" "-Wno-global-constructors" "-Wno-missing-variable-declarations"
        "-Wno-nested-anon-types"
    )
endif()

#CPUID
add_executable(cpuid cpuid/cpuid.cpp)
target_compile_options(cpuid PRIVATE ${ARCH_AVX})

#MATH3
set(MATH3_SRCS
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
    math3/xmvec234.cpp
)

add_executable(math3_sse2 ${MATH3_SRCS})
target_compile_options(math3_sse2 PRIVATE ${ARCH_SSE2})

add_executable(math3_avx ${MATH3_SRCS})
target_compile_options(math3_avx PRIVATE ${ARCH_AVX})

add_executable(math3_avx2 ${MATH3_SRCS})
target_compile_options(math3_avx2 PRIVATE ${ARCH_AVX2})

#SHMath
add_executable(shmathtest
    shmath/test.cpp
    ../SHMath/DirectXSH.cpp
    ../SHMath/DirectXSH.h
)
if(WIN32)
    target_sources(shmathtest PRIVATE
        shmath/DDSTextureLoader.cpp
        shmath/DDSTextureLoader.h
        shmath/DDSTextureLoader12.cpp
        shmath/DDSTextureLoader12.h
        ../SHMath/DirectXSHD3D11.cpp
        ../SHMath/DirectXSHD3D12.cpp
    )
    target_compile_definitions(shmathtest PRIVATE USE_DIRECT3D12=1 )
    target_link_libraries(shmathtest d3d11.lib dxgi.lib d3d12.lib dxguid.lib )
endif()

#XDSP
add_executable(xdsptest xdsp/Test.cpp)
target_compile_options(xdsptest PRIVATE ${ARCH_AVX2})

#EXT
add_executable(testavx ext/testavx.cpp)
target_compile_options(testavx PRIVATE ${ARCH_AVX})

add_executable(testavx2 ext/testavx2.cpp)
target_compile_options(testavx2 PRIVATE ${ARCH_AVX2})

add_executable(testbe ext/testbe.cpp)
target_compile_options(testbe PRIVATE ${ARCH_SSSE3})

add_executable(testf16c ext/testf16c.cpp)
target_compile_options(testf16c PRIVATE ${ARCH_F16C})

add_executable(testfma3 ext/testfma3.cpp)
target_compile_options(testfma3 PRIVATE ${ARCH_FMA})

add_executable(testfma4 ext/testfma4.cpp)
target_compile_options(testfma4 PRIVATE ${ARCH_FMA4})

add_executable(testsse3 ext/testsse3.cpp)
target_compile_options(testsse3 PRIVATE ${ARCH_SSE3})

add_executable(testsse4 ext/testsse4.cpp)
target_compile_options(testsse4 PRIVATE ${ARCH_SSE4})

#source groups
source_group(cpuid REGULAR_EXPRESSION cpuid/*.*)
source_group(Ext REGULAR_EXPRESSION ext/*.*)
source_group(Math3 REGULAR_EXPRESSION math3/*.*)
source_group(shmathtest REGULAR_EXPRESSION shmath/*.*)
source_group(XDSPTest REGULAR_EXPRESSION xdsp/*.*)
