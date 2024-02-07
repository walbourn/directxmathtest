# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

project(directxmath
  DESCRIPTION "DirectXMath Test Suite"
  HOMEPAGE_URL "https://github.com/walbourn/directxmathtest/wiki"
  LANGUAGES CXX)

option(BUILD_FOR_ONECORE   "Build for OneCore (Windows 10/Windows 11)" OFF)
option(BUILD_AVX_TEST      "Build math3 test for /arch:AVX" OFF)
option(BUILD_AVX2_TEST     "Build math3 test for /arch:AVX2" OFF)
option(BUILD_F16C_TEST     "Build math3 test for /arch:AVX + F16C" OFF)
option(BUILD_DISABLE_SVML  "Disable use of SVML (VS 2019)" OFF)
option(BUILD_NO_INTRINSICS "Disable use of compiler intrinsics" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(DIRECTXMATH_PATH ${CMAKE_CURRENT_LIST_DIR}/../)

if(NOT EXISTS "${DIRECTXMATH_PATH}/Inc/DirectXMath.h")
  message(FATAL_ERROR "DirectXMath test suite should be located 'below' DirectXMath")
endif()

include_directories(${DIRECTXMATH_PATH}/Inc)

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
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Ww][Ii][Nn]32$")
    set(DXMATH_ARCHITECTURE x86)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Xx]64$")
    set(DXMATH_ARCHITECTURE x64)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]$")
    set(DXMATH_ARCHITECTURE arm)
elseif(CMAKE_VS_PLATFORM_NAME_DEFAULT MATCHES "^[Aa][Rr][Mm]64$")
    set(DXMATH_ARCHITECTURE arm64)
elseif(NOT (DEFINED DXMATH_ARCHITECTURE))
    set(DXMATH_ARCHITECTURE "x64")
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

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
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
            add_compile_options("-Wno-unsafe-buffer-usage")
        endif()
elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    add_compile_options(
        "-Wno-reserved-id-macro" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
        "-Wno-gnu-anonymous-struct" "-Wno-ignored-attributes" "-Wno-global-constructors" "-Wno-missing-variable-declarations"
        "-Wno-nested-anon-types")
elseif( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    add_compile_options(/fp:fast /permissive- /Zc:__cplusplus /Zc:inline /wd4061 /wd4365 /wd4514 /wd4710 /wd4820 /wd4668 /wd5039 /wd5045)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.34)
      add_compile_options(/wd5264)
    endif()
endif()

# Always use retail static CRT for this tool
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)

enable_testing()

add_subdirectory(headertest)

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
set_tests_properties(stack PROPERTIES LABELS "Library")

add_executable(shmathtest
    shmath/test.cpp
    ${DIRECTXMATH_PATH}/SHMath/DirectXSH.cpp
    ${DIRECTXMATH_PATH}/SHMath/DirectXSH.h)
target_compile_options(shmathtest PRIVATE ${ARCH_SSE2})
target_include_directories( shmathtest PUBLIC ${DIRECTXMATH_PATH}/SHMath )
source_group(shmathtest REGULAR_EXPRESSION shmath/*.*)
add_test(NAME "shmath" COMMAND shmathtest WORKING_DIRECTORY $<TARGET_FILE_DIR:shmathtest>)
set_tests_properties(shmath PROPERTIES LABELS "Library")

add_custom_command(
  TARGET shmathtest POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shmath/media
    $<TARGET_FILE_DIR:shmathtest>/media
    )

add_executable(xdsptest xdsp/Test.cpp)
target_compile_options(xdsptest PRIVATE ${ARCH_SSE2})
target_include_directories( xdsptest PUBLIC ${DIRECTXMATH_PATH}/XDSP )
source_group(XDSPTest REGULAR_EXPRESSION xdsp/*.*)
add_test(NAME "xdsp" COMMAND xdsptest)
set_tests_properties(xdsp PROPERTIES LABELS "Library")

set(DXMATH_TESTS headertest math3 stacktest shmathtest xdsptest)

if(WIN32 AND NOT (DXMATH_ARCHITECTURE MATCHES "^arm"))
    add_executable(cpuid cpuid/cpuid.cpp)
    target_compile_options(cpuid PRIVATE ${ARCH_SSE2} $<$<CXX_COMPILER_ID:Clang,GNU>:-mxsave>)
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
        if (CPU_VENDOR_ID STREQUAL "AuthenticAMD")
          add_test(NAME "fma4" COMMAND testfma4)
          set_tests_properties(fma4 PROPERTIES LABELS "Extensions")
          list(APPEND DXMATH_TESTS testfma4)
        endif()
    endif()
endif()

foreach(proj IN LISTS DXMATH_TESTS)
    message(STATUS "Test enabled: ${proj}")
    target_compile_definitions(${proj} PRIVATE ${DXMATH_DEFS})
    if(WIN32)
        target_compile_definitions(${proj} PRIVATE _UNICODE UNICODE)
    endif()
endforeach()

if(WIN32 AND NOT MINGW)
    if(BUILD_AVX2_TEST)
       target_compile_options(math3 PRIVATE ${ARCH_AVX2})
    elseif(BUILD_F16C_TEST)
       target_compile_options(math3 PRIVATE ${ARCH_F16C})
       target_compile_definitions(math3 PRIVATE _XM_F16C_INTRINSICS_)
    elseif(BUILD_AVX_TEST)
       target_compile_options(math3 PRIVATE ${ARCH_AVX})
    else()
       target_compile_options(math3 PRIVATE ${ARCH_SSE2})
    endif()

    target_sources(shmathtest PRIVATE
        shmath/DDSTextureLoader11.cpp
        shmath/DDSTextureLoader11.h
        ${DIRECTXMATH_PATH}/SHMath/DirectXSHD3D11.cpp)
    target_compile_definitions(shmathtest PRIVATE USE_DIRECT3D11=1)
    target_link_libraries(shmathtest d3d11.lib dxgi.lib dxguid.lib)

    target_sources(shmathtest PRIVATE
        shmath/DDSTextureLoader12.cpp
        shmath/DDSTextureLoader12.h
        ${DIRECTXMATH_PATH}/SHMath/DirectXSHD3D12.cpp)
    target_compile_definitions(shmathtest PRIVATE USE_DIRECT3D12=1)
    target_link_libraries(shmathtest d3d12.lib)
else()
    target_compile_options(math3 PRIVATE ${ARCH_SSE2})
endif()

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
        URL https://raw.githubusercontent.com/dotnet/runtime/v8.0.1/src/coreclr/pal/inc/rt/sal.h
        URL_HASH SHA512=0f5a80b97564217db2ba3e4624cc9eb308e19cc9911dae21d983c4ab37003f4756473297ba81b386c498514cedc1ef5a3553d7002edc09aeb6a1335df973095f
        DOWNLOAD_NO_EXTRACT ON
    )
    FetchContent_MakeAvailable(salheader)

    foreach(proj IN LISTS DXMATH_TESTS)
        target_include_directories(${proj} PUBLIC ${salheader_SOURCE_DIR})
    endforeach()
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT math3)
