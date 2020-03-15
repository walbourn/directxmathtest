# DirectXMath Test Suite
#
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.
#
# http://go.microsoft.com/fwlink/?LinkID=615560

cmake_minimum_required (VERSION 3.12)

project (directxmath LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/CMake")

include_directories(../Inc)

set(ARCH_AVX   $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx>)
set(ARCH_AVX2  $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx2 -mfma -mf16c>)
set(ARCH_F16C  $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mf16c>)
set(ARCH_FMA   $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mfma>)
set(ARCH_FMA4  $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mfma4>)
set(ARCH_SSE2  $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse2>)
set(ARCH_SSE3  $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse3>)
set(ARCH_SSSE3 $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mssse3>)

if(MSVC)
    # Use max Warning Level 
    string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

    # Not using typeid or dynamic_cast, so disable RTTI to save binary size
    string(REPLACE "/GR " "/GR- " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REPLACE "/GR " "/GR- " CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/GR " "/GR- " CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

    # General settings for all MSVC-like targets
    add_compile_options(/fp:fast)
endif()

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    add_compile_options("-Wall" "-Wpedantic" "-Wextra"
       "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
       "-Wno-gnu-anonymous-struct" "-Wno-nested-anon-types"
       "-Wno-float-equal" "-Wno-double-promotion"
       "-Wno-missing-variable-declarations" "-Wno-missing-prototypes"
       "-Wno-global-constructors"
       "-Wno-reserved-id-macro" "-Wno-undef")
elseif ( CMAKE_CXX_COMPILER_ID MATCHES "GCC" )
    add_compile_options(
        "-Wno-reserved-id-macro" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
        "-Wno-gnu-anonymous-struct" "-Wno-ignored-attributes" "-Wno-global-constructors" "-Wno-missing-variable-declarations"
        "-Wno-nested-anon-types")
elseif ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    add_compile_options(/permissive- /Zc:__cplusplus /wd4514 /wd4710 /wd4820 /wd4668 /wd5039 /wd5045)
endif()

add_executable(cpuid cpuid/cpuid.cpp)
target_compile_options(cpuid PRIVATE ${ARCH_AVX})

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
    ../SHMath/DirectXSH.cpp
    ../SHMath/DirectXSH.h)

if (WIN32 AND NOT MINGW)
    target_sources(shmathtest PRIVATE
        shmath/DDSTextureLoader.cpp
        shmath/DDSTextureLoader.h
        ../SHMath/DirectXSHD3D11.cpp)
    target_compile_definitions(shmathtest PRIVATE USE_DIRECT3D11=1)
    target_link_libraries(shmathtest d3d11.lib dxgi.lib dxguid.lib)

    target_sources(shmathtest PRIVATE
        shmath/DDSTextureLoader12.cpp
        shmath/DDSTextureLoader12.h
        ../SHMath/DirectXSHD3D12.cpp)
    target_compile_definitions(shmathtest PRIVATE USE_DIRECT3D12=1)
    target_link_libraries(shmathtest d3d12.lib)
endif()

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

add_executable(xdsptest xdsp/Test.cpp)

source_group(cpuid REGULAR_EXPRESSION cpuid/*.*)
source_group(Ext REGULAR_EXPRESSION ext/*.*)
source_group(Math3 REGULAR_EXPRESSION math3/*.*)
source_group(shmathtest REGULAR_EXPRESSION shmath/*.*)
source_group(XDSPTest REGULAR_EXPRESSION xdsp/*.*)

target_include_directories( shmathtest PUBLIC ../SHMath )
target_include_directories( testavx PUBLIC ../Extensions )
target_include_directories( testavx2 PUBLIC ../Extensions )
target_include_directories( testbe PUBLIC ../Extensions )
target_include_directories( testf16c PUBLIC ../Extensions )
target_include_directories( testfma3 PUBLIC ../Extensions )
target_include_directories( testfma4 PUBLIC ../Extensions )
target_include_directories( testsse3 PUBLIC ../Extensions )
target_include_directories( testsse4 PUBLIC ../Extensions )
target_include_directories( xdsptest PUBLIC ../XDSP )
