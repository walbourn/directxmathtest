# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

project (headertest
DESCRIPTION "DirectXMath Test Suite"
HOMEPAGE_URL "https://github.com/walbourn/directxmathtest/wiki"
LANGUAGES CXX)

option(ENABLE_CODE_ANALYSIS "Use Static Code Analysis on build" OFF)

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
elseif(CMAKE_GENERATOR_PLATFORM MATCHES "^[Aa][Rr][Mm]64EC$")
    set(DXMATH_ARCHITECTURE arm64ec)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Ww][Ii][Nn]32$")
    set(DXMATH_ARCHITECTURE x86)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Xx]64$")
    set(DXMATH_ARCHITECTURE x64)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]$")
    set(DXMATH_ARCHITECTURE arm)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]64$")
    set(DXMATH_ARCHITECTURE arm64)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]64EC$")
    set(DXMATH_ARCHITECTURE arm64ec)
elseif(NOT (DEFINED DXMATH_ARCHITECTURE))
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "[Aa][Rr][Mm]64|aarch64|arm64")
        set(DXMATH_ARCHITECTURE arm64)
    else()
        set(DXMATH_ARCHITECTURE x64)
    endif()
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

include_directories(${DIRECTXMATH_PATH}/Inc)

target_compile_options(${PROJECT_NAME} PRIVATE ${ARCH_SSE2})

if(MSVC)
    # General settings for all MSVC-like targets
    target_compile_options(${PROJECT_NAME} PRIVATE /Wall /GR-)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|IntelLLVM")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wpedantic" "-Wextra"
        "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
        "-Wno-gnu-anonymous-struct"
        "-Wno-missing-variable-declarations" "-Wno-missing-prototypes"
        "-Wno-unknown-warning-option"
        "-Wno-reserved-id-macro" "-Wno-undef"
    )

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13.0)
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-reserved-identifier")
    endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /sdl /permissive- /JMC- /Zc:__cplusplus /Zc:inline /fp:fast /wd4514 /wd4668)

    if(ENABLE_CODE_ANALYSIS)
        target_compile_options(${PROJECT_NAME} PRIVATE /analyze /WX)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.24)
        target_compile_options(${PROJECT_NAME} PRIVATE /ZH:SHA_256)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.26)
        target_compile_options(${PROJECT_NAME} PRIVATE /Zc:preprocessor /wd5104 /wd5105)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.28)
        target_compile_options(${PROJECT_NAME} PRIVATE /Zc:lambda)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.31)
        target_compile_options(${PROJECT_NAME} PRIVATE /Zc:static_assert-)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.35)
        target_compile_options(${PROJECT_NAME} PRIVATE /Zc:templateScope)
    endif()
endif()
