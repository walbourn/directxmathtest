# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

cmake_minimum_required (VERSION 3.13)

project(directxmath
  DESCRIPTION "DirectXMath Test Suite"
  HOMEPAGE_URL "https://go.microsoft.com/fwlink/?LinkID=615560"
  LANGUAGES CXX)

option(BUILD_FOR_ONECORE   "Build for OneCore (Windows 10/Windows 11)" OFF)
option(BUILD_AVX_TEST      "Build math3 test for /arch:AVX" OFF)
option(BUILD_AVX2_TEST     "Build math3 test for /arch:AVX2" OFF)
option(BUILD_DISABLE_SVML  "Disable use of SVML (VS 2019)" OFF)
option(BUILD_NO_INTRINSICS "Disable use of compiler intrinsics" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/CMake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/CMake")

include_directories(../Inc)

if (BUILD_NO_INTRINSICS)
    message("Building with compiler intrinsics disabled (_XM_NO_INTRINSICS_)")
    add_compile_definitions(_XM_NO_INTRINSICS_)
endif()

if (NOT CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
    if (${CMAKE_SIZEOF_VOID_P} EQUAL "4")
        set(ARCH_SSE2 $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse2>)
    else()
        set(ARCH_SSE2 $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse2>)
    endif()

    set(ARCH_AVX   $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx>)
    set(ARCH_AVX2  $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mavx2 -mfma -mf16c>)
    set(ARCH_F16C  $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mf16c>)
    set(ARCH_FMA   $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mfma>)
    set(ARCH_FMA4  $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mfma4>)
    set(ARCH_SSE3  $<$<CXX_COMPILER_ID:MSVC>:${ARCH_SSE2}> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse3>)
    set(ARCH_SSSE3 $<$<CXX_COMPILER_ID:MSVC>:${ARCH_SSE2}> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mssse3>)
endif()

if(BUILD_DISABLE_SVML)
    set(DXMATH_DEFS _XM_DISABLE_INTEL_SVML_)
endif()

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

    if (BUILD_FOR_ONECORE)
        message("Building for OneCore (Windows 10)")
        # Locate Visual Studio (needed for VC Runtime DLLs)
        set(VCInstallDir "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2019/Community/VC")
        if(NOT EXISTS ${VCInstallDir})
            set(VCInstallDir "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2019/Professional/VC")
        endif()
        if(NOT EXISTS ${VCInstallDir})
            set(VCInstallDir "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2019/Enterprise/VC")
        endif()
        if(NOT EXISTS ${VCInstallDir})
            set(VCInstallDir "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2019/Preview/VC")
        endif()
        if(EXISTS ${VCInstallDir})
            message("VCInstallDir = ${VCInstallDir}")
        else()
            message(FATAL_ERROR "ERROR: Failed to locate Visual Studio 2019 install")
        endif()

        # Find VC toolset/runtime versions
        file(STRINGS "${VCInstallDir}/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt" VCToolsVersion)
        message("VCToolsVersion = ${VCToolsVersion}")

        # Don't link with kernel32.lib, etc.
        set(CMAKE_CXX_STANDARD_LIBRARIES "")
        set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "")

        # Need to link with "onecore" versions of Visual C++ libraries ("msvc_x64_x64" environment uses desktop libpath)
        set(VC_OneCore_LibPath "${VCInstallDir}/Tools/MSVC/${VCToolsVersion}/lib/onecore/x64")
        if(NOT EXISTS ${VC_OneCore_LibPath}/msvcrt.lib)
            message(FATAL_ERROR "ERROR: Cannot locate msvcrt.lib for the Visual C++ toolset (${VCToolsVersion})")
        endif()

        set(DXMATH_CONFIG "_WIN32_WINNT=0x0A00")

    endif()
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
    add_compile_options(/permissive- /Zc:__cplusplus /wd4061 /wd4365 /wd4514 /wd4710 /wd4820 /wd4668 /wd5039 /wd5045)
endif()

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
target_compile_options(math3 PRIVATE ${ARCH_SSE2})
target_compile_definitions(math3 PRIVATE ${DXMATH_DEFS})
source_group(Math3 REGULAR_EXPRESSION math3/*.*)

add_executable(stacktest
    stacktest/main.cpp
    ../MatrixStack/DirectXMatrixStack.h)
target_compile_options(stacktest PRIVATE ${ARCH_SSE2})
target_compile_definitions(stacktest PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
target_include_directories( stacktest PUBLIC ../MatrixStack )
source_group(stacktest REGULAR_EXPRESSION stacktest/*.*)

add_executable(shmathtest
    shmath/test.cpp
    ../SHMath/DirectXSH.cpp
    ../SHMath/DirectXSH.h)
target_compile_options(shmathtest PRIVATE ${ARCH_SSE2})
target_compile_definitions(shmathtest PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
target_include_directories( shmathtest PUBLIC ../SHMath )
source_group(shmathtest REGULAR_EXPRESSION shmath/*.*)

add_executable(xdsptest xdsp/Test.cpp)
target_compile_definitions(xdsptest PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
target_include_directories( xdsptest PUBLIC ../XDSP )
source_group(XDSPTest REGULAR_EXPRESSION xdsp/*.*)

if (WIN32)
    target_compile_definitions(math3 PRIVATE _UNICODE UNICODE)

    add_executable(cpuid cpuid/cpuid.cpp)
    source_group(cpuid REGULAR_EXPRESSION cpuid/*.*)
endif()

if (WIN32 AND (NOT BUILD_NO_INTRINSICS))
    source_group(Ext REGULAR_EXPRESSION ext/*.*)

    add_executable(testavx ext/testavx.cpp)
    target_compile_options(testavx PRIVATE ${ARCH_AVX})
    target_compile_definitions(testavx PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
    target_include_directories( testavx PUBLIC ../Extensions )

    add_executable(testavx2 ext/testavx2.cpp)
    target_compile_options(testavx2 PRIVATE ${ARCH_AVX2})
    target_compile_definitions(testavx2 PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
    target_include_directories( testavx2 PUBLIC ../Extensions )

    add_executable(testbe ext/testbe.cpp)
    target_compile_options(testbe PRIVATE ${ARCH_SSSE3})
    target_compile_definitions(testbe PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
    target_include_directories( testbe PUBLIC ../Extensions )

    add_executable(testf16c ext/testf16c.cpp)
    target_compile_options(testf16c PRIVATE ${ARCH_F16C})
    target_compile_definitions(testf16c PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
    target_include_directories( testf16c PUBLIC ../Extensions )

    add_executable(testfma3 ext/testfma3.cpp)
    target_compile_options(testfma3 PRIVATE ${ARCH_FMA})
    target_compile_definitions(testfma3 PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
    target_include_directories( testfma3 PUBLIC ../Extensions )

    add_executable(testfma4 ext/testfma4.cpp)
    target_compile_options(testfma4 PRIVATE ${ARCH_FMA4})
    target_compile_definitions(testfma4 PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
    target_include_directories( testfma4 PUBLIC ../Extensions )

    add_executable(testsse3 ext/testsse3.cpp)
    target_compile_options(testsse3 PRIVATE ${ARCH_SSE3})
    target_compile_definitions(testsse3 PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
    target_include_directories( testsse3 PUBLIC ../Extensions )

    add_executable(testsse4 ext/testsse4.cpp)
    target_compile_options(testsse4 PRIVATE ${ARCH_SSE4})
    target_compile_definitions(testsse4 PRIVATE ${DXMATH_DEFS} _UNICODE UNICODE)
    target_include_directories( testsse4 PUBLIC ../Extensions )
endif()

if (WIN32 AND NOT MINGW)

    if (BUILD_AVX2_TEST)
       target_compile_options(math3 PRIVATE ${ARCH_AVX2})
    elseif (BUILD_AVX_TEST)
       target_compile_options(math3 PRIVATE ${ARCH_AVX})
    endif()

    target_sources(shmathtest PRIVATE
        shmath/DDSTextureLoader11.cpp
        shmath/DDSTextureLoader11.h
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

if (MSVC AND BUILD_FOR_ONECORE)
    target_link_directories(cpuid PUBLIC ${VC_OneCore_LibPath})
    target_link_directories(math3 PUBLIC ${VC_OneCore_LibPath})
    target_link_directories(stacktest PUBLIC ${VC_OneCore_LibP})
    target_link_directories(shmathtest PUBLIC ${VC_OneCore_LibPath})
    target_link_directories(xdsptest PUBLIC ${VC_OneCore_LibPath})

    target_link_libraries(cpuid onecore.lib)
    target_link_libraries(math3 onecore.lib)
    target_link_libraries(stacktest onecore.lib)
    target_link_libraries(shmathtest onecore.lib)
    target_link_libraries(xdsptest onecore.lib)

    if (NOT BUILD_NO_INTRINSICS)
        target_link_directories(testavx PUBLIC ${VC_OneCore_LibPath})
        target_link_directories(testavx2 PUBLIC ${VC_OneCore_LibPath})
        target_link_directories(testbe PUBLIC ${VC_OneCore_LibPath})
        target_link_directories(testf16c PUBLIC ${VC_OneCore_LibPath})
        target_link_directories(testfma3 PUBLIC ${VC_OneCore_LibPath})
        target_link_directories(testfma4 PUBLIC ${VC_OneCore_LibPath})
        target_link_directories(testsse3 PUBLIC ${VC_OneCore_LibPath})
        target_link_directories(testsse4 PUBLIC ${VC_OneCore_LibPath})

        target_link_libraries(testavx onecore.lib)
        target_link_libraries(testavx2 onecore.lib)
        target_link_libraries(testbe onecore.lib)
        target_link_libraries(testf16c onecore.lib)
        target_link_libraries(testfma3 onecore.lib)
        target_link_libraries(testfma4 onecore.lib)
        target_link_libraries(testsse3 onecore.lib)
        target_link_libraries(testsse4 onecore.lib)
    endif()
endif()

if (NOT WIN32)
    include(ExternalProject)
    ExternalProject_Add(
        sal
        PREFIX ${CMAKE_BINARY_DIR}/sal
        URL http://raw.githubusercontent.com/dotnet/corert/master/src/Native/inc/unix/sal.h
        URL_HASH SHA256=bb9ea7c1d05cdfc10a17fe80d87a7cd160c1063c7e5fa089cd6405c31c610a1b
        DOWNLOAD_NO_EXTRACT ON
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
    )
    ExternalProject_Get_Property(sal download_dir)
    target_include_directories( math3 PUBLIC ${download_dir} )
    target_include_directories( stacktest PUBLIC ${download_dir} )
    target_include_directories( shmathtest PUBLIC ${download_dir} )
    target_include_directories( xdsptest PUBLIC ${download_dir} )
    target_include_directories( xdsptest PUBLIC ${download_dir} )
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT math3)
