﻿# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

project(directxmath
  DESCRIPTION "DirectXMath Test Suite"
  HOMEPAGE_URL "https://github.com/walbourn/directxmathtest/wiki"
  LANGUAGES CXX)

option(BUILD_FOR_ONECORE    "Build for OneCore (Windows 10/Windows 11)" OFF)
option(BUILD_AVX_TEST       "Build math3 test for /arch:AVX" OFF)
option(BUILD_AVX2_TEST      "Build math3 test for /arch:AVX2" OFF)
option(BUILD_F16C_TEST      "Build math3 test for /arch:AVX + F16C" OFF)
option(BUILD_DISABLE_SVML   "Disable use of SVML (VS 2019)" OFF)
option(BUILD_NO_INTRINSICS  "Disable use of compiler intrinsics" OFF)
option(BUILD_CXX11          "Build for C++ 11" OFF)
option(BUILD_CXX14          "Build for C++ 14" OFF)
option(BUILD_CXX20          "Build for C++ 20" OFF)
option(ENABLE_CODE_ANALYSIS "Use Static Code Analysis on build" OFF)

if(BUILD_CXX20)
    message(STATUS "Building for C++20 Standard")
    set(CMAKE_CXX_STANDARD 20)
elseif(BUILD_CXX14)
    message(STATUS "Building for C++14 Standard")
    set(CMAKE_CXX_STANDARD 14)
elseif(BUILD_CXX11)
    message(STATUS "Building for C++11 Standard")
    set(CMAKE_CXX_STANDARD 11)
else()
    message(STATUS "Building for C++17 Standard (default)")
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH DIRECTXMATH_PATH)

if(NOT EXISTS "${DIRECTXMATH_PATH}/Inc/DirectXMath.h")
  message(FATAL_ERROR "DirectXMath test suite should be located 'below' DirectXMath")
endif()

if(TARGET DirectXMath)
    link_libraries(DirectXMath)
else()
    include_directories(${DIRECTXMATH_PATH}/Inc)
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

if(BUILD_NO_INTRINSICS)
    message(STATUS "Building with compiler intrinsics disabled (_XM_NO_INTRINSICS_)")
    set(DXMATH_DEFS _XM_NO_INTRINSICS_)
elseif(BUILD_DISABLE_SVML)
    set(DXMATH_DEFS _XM_DISABLE_INTEL_SVML_)
endif()

if(NOT (DXMATH_ARCHITECTURE MATCHES "^arm"))

    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH_SSE2 $<IF:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>,/arch:SSE2,-msse2>)
    else()
        set(ARCH_SSE2 $<$<NOT:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Intel>>>:-msse2>)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        list(APPEND ARCH_SSE2 -mfpmath=sse)
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

if(MSVC)
    # General settings for all MSVC-like targets
    add_compile_options(/Wall /GR-)

    if(BUILD_FOR_ONECORE)
        message(STATUS "Building for OneCore (Windows 10/Window 11)")

        # Locate Visual Studio
        if(NOT DEFINED VCInstallDir AND DEFINED ENV{VCINSTALLDIR})
            set(VCInstallDir $ENV{VCINSTALLDIR})
        endif()

        if(NOT DEFINED VCInstallDir)
            set(KNOWN_VS_EDITIONS "Community" "Professional" "Enterprise" "Preview" "BuildTools")
            foreach(vsedition IN LISTS KNOWN_VS_EDITIONS)
                set(VCInstallDir "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2019/${vsedition}/VC")
                if(EXISTS ${VCInstallDir})
                    break()
                endif()
            endforeach()

            if(NOT EXISTS ${VCInstallDir})
                foreach(vsedition IN LISTS KNOWN_VS_EDITIONS)
                    set(VCInstallDir "$ENV{ProgramFiles}/Microsoft Visual Studio/2022/${vsedition}/VC")
                    if(EXISTS ${VCInstallDir})
                        break()
                    endif()
                endforeach()
            endif()
        endif()

        if(EXISTS ${VCInstallDir})
            message(VERBOSE "VCInstallDir = ${VCInstallDir}")
        else()
            message(FATAL_ERROR "ERROR: Failed to locate Visual Studio 2019 or 2022 install")
        endif()

        # Find VC toolset/runtime versions
        file(STRINGS "${VCInstallDir}/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt" VCToolsVersion)
        message(VERBOSE "VCToolsVersion = ${VCToolsVersion}")

        # Don't link with kernel32.lib, etc.
        set(CMAKE_CXX_STANDARD_LIBRARIES "")
        set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "")

        # Need to link with "onecore" versions of Visual C++ libraries
        set(VC_OneCore_LibPath "${VCInstallDir}/Tools/MSVC/${VCToolsVersion}/lib/onecore/${DXMATH_ARCHITECTURE}")
        if(NOT EXISTS ${VC_OneCore_LibPath}/msvcrt.lib)
            message(FATAL_ERROR "ERROR: Cannot locate msvcrt.lib for the Visual C++ toolset (${VCToolsVersion})")
        endif()

        list(APPEND DXMATH_DEFS "_WIN32_WINNT=0x0A00")
    endif()
else()
    add_compile_definitions(PRIVATE $<IF:$<CONFIG:DEBUG>,_DEBUG,NDEBUG>)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|IntelLLVM")
    add_compile_options("-Wall" "-Wpedantic" "-Wextra"
       "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
       "-Wno-gnu-anonymous-struct" "-Wno-nested-anon-types"
       "-Wno-float-equal" "-Wno-double-promotion"
       "-Wno-missing-variable-declarations" "-Wno-missing-prototypes"
       "-Wno-global-constructors"
       "-Wno-unknown-warning-option"
       "-Wno-reserved-id-macro" "-Wno-undef")

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13.0)
        add_compile_options("-Wno-reserved-identifier")
    endif()
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16.0)
        add_compile_options("-Wno-unsafe-buffer-usage" "-Walloc-size-larger-than=4GB")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options("-Wno-ignored-attributes")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    add_compile_options(/fp:fast /Zc:__cplusplus /Zc:inline /Qdiag-disable:177)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/fp:fast /Zc:inline
        /wd4061 /wd4365 /wd4514 /wd4710 /wd4820 /wd4668 /wd5039 /wd5045)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.10)
        add_compile_options(/permissive-)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.14)
        add_compile_options(/Zc:__cplusplus)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.24)
        add_compile_options(/ZH:SHA_256)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.26)
        add_compile_options(/Zc:preprocessor /wd5104 /wd5105)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.34)
      add_compile_options(/wd5264)
    endif()

    if(ENABLE_CODE_ANALYSIS)
      message(STATUS "Building with Code Analysis (PREFIX)")
      add_compile_options(/analyze /WX)
    endif()
endif()

if(NOT BUILD_SHMATH)
    # Always use retail static CRT for this tool
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
endif()

enable_testing()

add_subdirectory(headertest)

set(SOURCES
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

add_executable(math3 ${SOURCES})
source_group(Math3 REGULAR_EXPRESSION math3/*.*)
add_test(NAME "math3" COMMAND math3)
set_tests_properties(math3 PROPERTIES LABELS "Library")

add_executable(stacktest
    stacktest/main.cpp
    ${DIRECTXMATH_PATH}/MatrixStack/DirectXMatrixStack.h)
target_compile_options(stacktest PRIVATE ${ARCH_SSE2})
target_include_directories( stacktest PUBLIC ${DIRECTXMATH_PATH}/MatrixStack )
source_group(stacktest REGULAR_EXPRESSION stacktest/*.*)
add_test(NAME "stack" COMMAND stacktest)
set_tests_properties(stack PROPERTIES LABELS "Library;MS")

set(SHMATH_SOURCES shmath/test.cpp)
if(NOT (TARGET DirectXSH))
    set(SHMATH_SOURCES ${SHMATH_SOURCES}
        ${DIRECTXMATH_PATH}/SHMath/DirectXSH.cpp
        ${DIRECTXMATH_PATH}/SHMath/DirectXSH.h)
endif()

add_executable(shmathtest ${SHMATH_SOURCES})
target_compile_options(shmathtest PRIVATE ${ARCH_SSE2})
source_group(shmathtest REGULAR_EXPRESSION shmath/*.*)
add_test(NAME "shmath" COMMAND shmathtest WORKING_DIRECTORY $<TARGET_FILE_DIR:shmathtest>)
set_tests_properties(shmath PROPERTIES LABELS "Library;SH")

if(TARGET DirectXSH)
    target_link_libraries(shmathtest PRIVATE DirectXSH)
else()
    target_include_directories( shmathtest PUBLIC ${DIRECTXMATH_PATH}/SHMath )
endif()

add_custom_command(
  TARGET shmathtest POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shmath/media
    $<TARGET_FILE_DIR:shmathtest>/media
    )

add_executable(xdsptest xdsp/Test.cpp)
target_compile_options(xdsptest PRIVATE ${ARCH_SSE2})
source_group(xdsptest REGULAR_EXPRESSION xdsp/*.*)
add_test(NAME "xdsp" COMMAND xdsptest)
set_tests_properties(xdsp PROPERTIES LABELS "Library;XDSP")

if(TARGET XDSP)
    target_link_libraries(xdsptest PRIVATE XDSP)
else()
    target_include_directories(xdsptest PUBLIC ${DIRECTXMATH_PATH}/XDSP )
endif()

set(DXMATH_TESTS headertest math3 stacktest shmathtest xdsptest)

if(WIN32 AND NOT (DXMATH_ARCHITECTURE MATCHES "^arm"))
    add_executable(cpuid cpuid/cpuid.cpp)
    target_compile_options(cpuid PRIVATE ${ARCH_SSE2} $<$<CXX_COMPILER_ID:Clang,IntelLLVM,GNU>:-mxsave>)
    source_group(cpuid REGULAR_EXPRESSION cpuid/*.*)

    list(APPEND DXMATH_TESTS cpuid)
endif()

if(NOT (BUILD_NO_INTRINSICS OR (DXMATH_ARCHITECTURE MATCHES "^arm")))
    source_group(Ext REGULAR_EXPRESSION ext/*.*)

    add_executable(testavx ext/testavx.cpp)
    target_compile_options(testavx PRIVATE ${ARCH_AVX})
    target_include_directories( testavx PUBLIC ${DIRECTXMATH_PATH}/Extensions )
    add_test(NAME "avx" COMMAND testavx)
    set_tests_properties(avx PROPERTIES LABELS "Extensions")

    add_executable(testavx2 ext/testavx2.cpp)
    target_compile_options(testavx2 PRIVATE ${ARCH_AVX2})
    target_include_directories( testavx2 PUBLIC ${DIRECTXMATH_PATH}/Extensions )
    add_test(NAME "avx2" COMMAND testavx2)
    set_tests_properties(avx2 PROPERTIES LABELS "Extensions")

    add_executable(testbe ext/testbe.cpp)
    target_compile_options(testbe PRIVATE ${ARCH_SSSE3})
    target_include_directories( testbe PUBLIC ${DIRECTXMATH_PATH}/Extensions )
    add_test(NAME "movbe" COMMAND testbe)
    set_tests_properties(movbe PROPERTIES LABELS "Extensions")

    add_executable(testf16c ext/testf16c.cpp)
    target_compile_options(testf16c PRIVATE ${ARCH_F16C})
    target_include_directories( testf16c PUBLIC ${DIRECTXMATH_PATH}/Extensions )
    add_test(NAME "f16c" COMMAND testf16c)
    set_tests_properties(f16c PROPERTIES LABELS "Extensions")

    add_executable(testfma3 ext/testfma3.cpp)
    target_compile_options(testfma3 PRIVATE ${ARCH_FMA})
    target_include_directories( testfma3 PUBLIC ${DIRECTXMATH_PATH}/Extensions )
    add_test(NAME "fma3" COMMAND testfma3)
    set_tests_properties(fma3 PROPERTIES LABELS "Extensions")

    add_executable(testsse3 ext/testsse3.cpp)
    target_compile_options(testsse3 PRIVATE ${ARCH_SSE3})
    target_include_directories( testsse3 PUBLIC ${DIRECTXMATH_PATH}/Extensions )
    add_test(NAME "sse3" COMMAND testsse3)
    set_tests_properties(sse3 PROPERTIES LABELS "Extensions")

    add_executable(testsse4 ext/testsse4.cpp)
    target_compile_options(testsse4 PRIVATE ${ARCH_SSE4})
    target_include_directories( testsse4 PUBLIC ${DIRECTXMATH_PATH}/Extensions )
    add_test(NAME "sse4" COMMAND testsse4)
    set_tests_properties(sse4 PROPERTIES LABELS "Extensions")

    list(APPEND DXMATH_TESTS testavx testavx2 testbe testf16c testfma3 testsse3 testsse4)

    if(WIN32 AND (NOT (CMAKE_CXX_COMPILER_ID MATCHES "^Intel")))
        add_executable(testfma4 ext/testfma4.cpp)
        target_compile_options(testfma4 PRIVATE ${ARCH_FMA4})
        target_include_directories( testfma4 PUBLIC ${DIRECTXMATH_PATH}/Extensions )

        get_filename_component(CPU_VENDOR_ID "[HKEY_LOCAL_MACHINE\\Hardware\\Description\\System\\CentralProcessor\\0;VendorIdentifier]" NAME CACHE)
        if(CPU_VENDOR_ID STREQUAL "AuthenticAMD")
          add_test(NAME "fma4" COMMAND testfma4)
          set_tests_properties(fma4 PROPERTIES LABELS "Extensions")
          list(APPEND DXMATH_TESTS testfma4)
        endif()
    endif()
endif()

target_compile_options(math3 PRIVATE ${ARCH_SSE2})

if(WIN32 AND NOT MINGW)
    if(BUILD_AVX2_TEST)
        list(APPEND DXMATH_TESTS math3_avx2)
        add_executable(math3_avx2 ${SOURCES})
        add_test(NAME "math3_avx2" COMMAND math3_avx2)
        set_tests_properties(math3_avx2 PROPERTIES LABELS "Library")
        target_compile_options(math3_avx2 PRIVATE ${ARCH_AVX2})
    endif()

    if(BUILD_F16C_TEST)
        list(APPEND DXMATH_TESTS math3_f16c)
        add_executable(math3_f16c ${SOURCES})
        add_test(NAME "math3_f16c" COMMAND math3_f16c)
        set_tests_properties(math3_f16c PROPERTIES LABELS "Library")
        target_compile_definitions(math3_f16c PRIVATE _XM_F16C_INTRINSICS_)
        target_compile_options(math3_f16c PRIVATE ${ARCH_F16C})
    endif()

    if(BUILD_AVX_TEST)
        list(APPEND DXMATH_TESTS math3_avx)
        add_executable(math3_avx ${SOURCES})
        add_test(NAME "math3_avx" COMMAND math3_avx)
        set_tests_properties(math3_avx PROPERTIES LABELS "Library")
        target_compile_options(math3_avx PRIVATE ${ARCH_AVX})
    endif()

    set(SHMATH_DX11_SOURCES
        shmath/DDSTextureLoader11.cpp
        shmath/DDSTextureLoader11.h)
    if ((NOT (TARGET DirectXSH)) OR (NOT BUILD_DX11))
        set(SHMATH_DX11_SOURCES ${SHMATH_DX11_SOURCES} ${DIRECTXMATH_PATH}/SHMath/DirectXSHD3D11.cpp)
    endif()
    target_sources(shmathtest PRIVATE ${SHMATH_DX11_SOURCES})
    target_compile_definitions(shmathtest PRIVATE USE_DIRECT3D11=1)
    target_link_libraries(shmathtest PRIVATE d3d11.lib dxgi.lib dxguid.lib)

    set(SHMATH_DX12_SOURCES
        shmath/DDSTextureLoader12.cpp
        shmath/DDSTextureLoader12.h)
    if ((NOT (TARGET DirectXSH)) OR (NOT BUILD_DX12))
        set(SHMATH_DX12_SOURCES ${SHMATH_DX12_SOURCES} ${DIRECTXMATH_PATH}/SHMath/DirectXSHD3D12.cpp)
    endif()
    target_sources(shmathtest PRIVATE ${SHMATH_DX12_SOURCES})
    target_compile_definitions(shmathtest PRIVATE USE_DIRECT3D12=1)
    target_link_libraries(shmathtest PRIVATE d3d12.lib)
endif()

message(STATUS "Enabled tests: ${DXMATH_TESTS}")

foreach(proj IN LISTS DXMATH_TESTS)
    target_compile_definitions(${proj} PRIVATE ${DXMATH_DEFS})
    if(WIN32)
        target_compile_definitions(${proj} PRIVATE _UNICODE UNICODE)
    endif()
endforeach()

if(MSVC AND BUILD_FOR_ONECORE)
    foreach(proj IN LISTS DXMATH_TESTS)
        target_link_directories(${proj} PUBLIC ${VC_OneCore_LibPath})
        target_link_libraries(${proj} onecore_apiset.lib)
        target_link_options(${proj} PRIVATE /SUBSYSTEM:CONSOLE,10.0 /NODEFAULTLIB:kernel32.lib /NODEFAULTLIB:onecore.lib)
    endforeach()
endif()

if(NOT WIN32)
    include(FetchContent)
    FetchContent_Declare(
        salheader
        URL https://raw.githubusercontent.com/dotnet/runtime/v9.0.2/src/coreclr/pal/inc/rt/sal.h
        URL_HASH SHA512=8085f67bfa4ce01ae89461cadf72454a9552fde3f08b2dcc3de36b9830e29ce7a6192800f8a5cb2a66af9637be0017e85719826a4cfdade508ae97f319e0ee8e
        DOWNLOAD_NO_EXTRACT ON
    )
    FetchContent_MakeAvailable(salheader)

    foreach(proj IN LISTS DXMATH_TESTS)
        target_include_directories(${proj} PUBLIC ${salheader_SOURCE_DIR})
    endforeach()
endif()


set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT math3)
