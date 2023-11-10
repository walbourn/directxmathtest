# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

project (headertest
DESCRIPTION "DirectXMath Test Suite"
HOMEPAGE_URL "https://github.com/walbourn/directxmathtest/wiki"
LANGUAGES CXX)

set(DIRECTXMATH_PATH ${CMAKE_CURRENT_LIST_DIR}/../../)

if(NOT EXISTS "${DIRECTXMATH_PATH}/Inc/DirectXMath.h")
  message(FATAL_ERROR "DirectXMath test suite should be located 'below' DirectXMath")
endif()

if(DEFINED VCPKG_TARGET_ARCHITECTURE)
    set(DXMATH_ARCHITECTURE ${VCPKG_TARGET_ARCHITECTURE})
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Ww][Ii][Nn]32$")
    set(DXMATH_ARCHITECTURE x86)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Xx]64$")
    set(DXMATH_ARCHITECTURE x64)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Aa][Rr][Mm]$")
    set(DXMATH_ARCHITECTURE arm)
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Aa][Rr][Mm]64$")
    set(DXMATH_ARCHITECTURE arm64)
elseif(NOT DXMATH_ARCHITECTURE)
    set(DXMATH_ARCHITECTURE "x64")
endif()

if(NOT (DXMATH_ARCHITECTURE MATCHES "^arm"))

    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH_SSE2 $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,/arch:SSE2,-msse2>)
    else()
        set(ARCH_SSE2 $<$<NOT:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>>:-msse2>)
    endif()

    set(ARCH_AVX   $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,/arch:AVX,-mavx>)
    set(ARCH_AVX2  $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,/arch:AVX2,-mavx2 -mfma -mf16c>)
    set(ARCH_F16C  $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,/arch:AVX,-mf16c>)
    set(ARCH_FMA   $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,/arch:AVX2,-mfma>)
    set(ARCH_FMA4  $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,/arch:AVX,-mfma4>)
    set(ARCH_SSE3  $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,${ARCH_SSE2},-msse3>)
    set(ARCH_SSSE3 $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,${ARCH_SSE2},-mssse3>)
    set(ARCH_SSE4  $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,${ARCH_SSE2},-msse4.1>)
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    colors.cpp
    collision.cpp
    core.cpp
    packed.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${DIRECTXMATH_PATH}/Inc)

target_compile_options(${PROJECT_NAME} PRIVATE ${ARCH_SSE2})
