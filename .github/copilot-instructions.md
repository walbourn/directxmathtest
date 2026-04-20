# GitHub Copilot Instructions for DirectXMath Test Suite

These instructions define how GitHub Copilot should assist with the DirectXMath test suite located in this repository ([Test Suite](https://github.com/walbourn/directxmathtest)). The goal is to ensure consistent, high-quality test code generation aligned with the existing conventions. They supplement the [parent project instructions](https://github.com/microsoft/DirectXMath/blob/main/.github/copilot-instructions.md) with test-specific conventions.

## Context

- **Project**: Test suite for the DirectXMath SIMD C++ math library
- **Repository**: <https://github.com/walbourn/directxmathtest>
- **Language**: C++17 (also tested with C++11, C++14, and C++20)
- **Build System**: CMake / CTest (built as subdirectory of the main DirectXMath repo)
- **Documentation**: https://github.com/walbourn/directxmathtest/wiki
- **Parent Project**: https://github.com/microsoft/DirectXMath

## Repository Layout

The test suite is designed to be cloned inside the DirectXMath source tree at `Tests/`:

```txt
Tests/                     # This repository (walbourn/directxmathtest)
├── CMakeLists.txt         # Main test build configuration
├── CMakePresets.json      # Build presets (x64, x86, ARM64, AVX, AVX2, etc.)
├── math3/                 # Core math library tests (vectors, matrices, quaternions, collision)
├── ext/                   # SIMD extension tests (SSE3, SSE4, AVX, AVX2, F16C, FMA3, FMA4, BE)
├── shmath/                # Spherical Harmonics (SHMath) tests
├── xdsp/                  # Digital Signal Processing (XDSP) tests
├── headertest/            # Header self-containment / compilation tests
├── cpuid/                 # CPU feature detection utility (Windows x86/x64 only)
├── stacktest/             # MatrixStack tests
└── VerifyRounding/        # Rounding verification helpers
```

The test suite expects to be located one directory below the DirectXMath root so that `../Inc/DirectXMath.h` resolves correctly. The CMake build enforces this with a `FATAL_ERROR` check.

## Building and Running Tests

```powershell
# Configure and build (default: x64, C++17, SSE2)
cmake --preset x64
cmake --build --preset x64

# Run all tests
ctest --preset x64

# Run a specific test executable directly
./out/build/x64/bin/math3
./out/build/x64/bin/shmathtest
```

### Key CMake Options

| Option | Default | Description |
| --- | --- | --- |
| `BUILD_AVX_TEST` | `OFF` | Build `math3_avx` with `/arch:AVX` |
| `BUILD_AVX2_TEST` | `OFF` | Build `math3_avx2` with `/arch:AVX2` |
| `BUILD_F16C_TEST` | `OFF` | Build `math3_f16c` with F16C intrinsics |
| `BUILD_NO_INTRINSICS` | `OFF` | Force `_XM_NO_INTRINSICS_` (pure C++ fallback) |
| `BUILD_DISABLE_SVML` | `OFF` | Disable Intel SVML |
| `BUILD_CXX11` | `OFF` | Build with C++11 standard |
| `BUILD_CXX14` | `OFF` | Build with C++14 standard |
| `BUILD_CXX20` | `OFF` | Build with C++20 standard |
| `ENABLE_CODE_ANALYSIS` | `OFF` | Enable MSVC `/analyze` |
| `ENABLE_ASAN` | `OFF` | Enable Address Sanitizer |

### CTest Labels

Tests are labeled for selective execution with `ctest -L <label>`:

| Label | Tests |
| --- | --- |
| `Library` | `math3`, `headertest`, `stacktest` |
| `Library;SH` | `shmathtest` |
| `Library;XDSP` | `xdsptest` |
| `Library;MS` | `stacktest` (MatrixStack) |
| `Extensions` | `testavx`, `testavx2`, `testsse3`, `testsse4`, `testf16c`, `testfma3`, `testfma4`, `testbe` |

## Test Architecture

### math3 — Core Test Framework

The `math3/` directory is the primary test suite. Key files:

| File | Purpose |
| --- | --- |
| `math3.h` | Shared test header: platform abstractions, comparison utilities, helper macros |
| `math3.cpp` | Main entry point, test registration, command-line parsing |
| `math3tests.cpp` | Test function registration table and shared test utilities |
| `shared.cpp` | Comparison functions, sandbox memory helpers, random generators |
| `xmvec.cpp` | XMVECTOR operation tests |
| `xmvec234.cpp` | XMVector2/3/4 function tests |
| `xmmat.cpp` | XMMATRIX operation tests |
| `xmquat.cpp` | Quaternion operation tests |
| `xmcolor.cpp` | Color operation tests |
| `box.cpp` | BoundingBox tests |
| `obox.cpp` | BoundingOrientedBox tests |
| `sphere.cpp` | BoundingSphere tests |
| `frustum.cpp` | BoundingFrustum tests |
| `triangle.cpp` | TriangleTests tests |
| `constexpr.cpp` | Compile-time constexpr validation |

### Test Function Conventions

Each test function follows one of two patterns:

**math3 pattern** — Functions are named `TestNNN` (e.g., `Test001`, `Test278`) and registered in a global `APIFUNCT tests[]` table:

```cpp
HRESULT Test278(LogProxy* pLog)
{
    // Test XMVectorAbs
    HRESULT ret = S_OK;

    // ... test logic ...

    COMPARISON c = CompareXMVECTOR(result, expected, 4);
    if (c > WITHINEPSILON) {
        printe("%s: ...\n", TestName);
        ret = MATH_FAIL;
    }

    return ret;
}
```

**stacktest/collision pattern** — Functions are named `TestNN` (e.g., `Test01`, `TestB01`) and return `bool`:

```cpp
bool Test01()
{
    bool success = true;

    // ... test logic ...

    if (/* failure condition */) {
        printe("%s: error message\n", TestName);
        success = false;
    }

    return success;
}
```

### Test Return Values

- **math3**: Returns `HRESULT` — `S_OK` (`MATH_PASS`) on success, `MATH_FAIL` on failure
- **stacktest**: Returns `bool` — `true` for pass, `false` for fail
- **ext tests**: Use `printf` output and return `int` from `main()`

### Logging and Diagnostics

- `printi(fmt, ...)` — Informational output (suppressed in standalone builds)
- `printe(fmt, ...)` — Error output (always printed; maps to `printf` in standalone builds)
- `TestName` — Macro that resolves to the current test name string
- `PRINT(...)` — Printf wrapper (suppressed in gauntlet mode)

### Floating-Point Comparison Framework

The test suite uses an epsilon-based comparison system defined in `math3.h`. The `COMPARISON` enum ranks match quality:

```cpp
enum COMPARISON {
    EXACT,              // Bit-identical
    WITHINEPSILON,      // Within 1.192092896e-7f (FLT_EPSILON)
    WITHIN2EPSILON,     // Within 2 * FLT_EPSILON
    WITHIN10EPSILON,    // Within 10 * FLT_EPSILON
    WITHIN100EPSILON,   // Within 100 * FLT_EPSILON
    WITHIN4096,         // Within ~0.024%
    WITHINBIGEPSILON,   // Within 0.001
    WITHINBIGGEREPSILON,// Within 0.0025
    WITHINHUGEEPSILON,  // Within 0.01
    // ... up to WAYOFF
};
```

Comparison functions are provided for all DirectXMath types:

```cpp
COMPARISON Compare(float a, float b);
COMPARISON CompareXMVECTOR(XMVECTOR a, XMVECTOR b, int NumElements);
COMPARISON CompareXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b);
COMPARISON CompareXMFLOAT4X4(XMFLOAT4X4 a, XMFLOAT4X4 b);
COMPARISON CompareXMMATRIX(CXMMATRIX a, CXMMATRIX b);
// ... etc.
```

Tests compare the result against a threshold (e.g., `c > WITHINEPSILON`). Choose the tightest threshold that accounts for the expected precision loss of the operation being tested.

### Sandbox Memory Pattern

Tests that validate Load/Store operations use a "sandbox" pattern to detect out-of-bounds writes:

```cpp
uint8_t sandbox1[256], sandbox2[256];
initsandbox(sandbox1, sizeof(sandbox1));  // Fill with known pattern
// ... perform load/store operations into sandbox1 ...
checksandbox(pLog, sandbox1, sandbox2, stride, size, count, sizeof(sandbox1), numfloat);
```

### Random Test Data

Use the provided random helpers for generating test inputs:

```cpp
float GetRandomFloat(float fRange);
XMVECTOR GetRandomVector16(void);
float GetRandomFloat16(void);
XMMATRIX GetRandomMatrix4(void);
```

Use `XM_RAND()` (not `rand()`) to ensure consistent behavior across compilers (`XM_RAND_MAX` is `0x7fff`).

## Writing New Tests

### Adding a Test to math3

1. **Create the test function** in the appropriate source file (e.g., `xmvec.cpp` for vector operations):

    ```cpp
    HRESULT TestNNN(LogProxy* pLog)
    {
        HRESULT ret = S_OK;
        // Test implementation using Compare functions
        return ret;
    }
    ```

2. **Register the test** in the `tests[]` table in `math3tests.cpp`:

    ```cpp
    tests[NNN] = { TestNNN, "FunctionBeingTested" };
    ```

3. **Use static_assert** for compile-time property validation:

    ```cpp
    static_assert(std::is_nothrow_copy_assignable<BoundingBox>::value, "Copy Assign.");
    static_assert(std::is_nothrow_move_constructible<BoundingBox>::value, "Move Ctor.");
    ```

### Adding a New Extension Test

Extension tests in `ext/` are standalone executables. Each tests one SIMD instruction set:

1. Create `ext/testnew.cpp` with `#include` of the extension header
2. Call `XMVerifyCPUSupport()` for baseline and the extension's verify function
3. Add the executable and test in `CMakeLists.txt` with appropriate `ARCH_*` compile options:

    ```cmake
    add_executable(testnew ext/testnew.cpp)
    target_compile_options(testnew PRIVATE ${ARCH_NEW})
    target_include_directories(testnew PUBLIC ${DIRECTXMATH_PATH}/Extensions)
    add_test(NAME "new" COMMAND testnew)
    set_tests_properties(new PROPERTIES LABELS "Extensions")
    ```

### Adding a New Standalone Test

For self-contained tests like `stacktest/`:

1. Create the test source with a `main()` function
2. Use a test registration table pattern with `TestInfo` structs
3. Return `0` for all-pass, `1` for any failure
4. Add as a new executable in the root `CMakeLists.txt`

## Cross-Platform Considerations

- **Non-Windows builds** fetch SAL headers from the .NET runtime repository via `FetchContent`
- Use `_WIN32` guards for Windows-specific code (D3D11/D3D12 integration in shmath)
- Use `#ifndef _MSC_VER` to provide fallbacks for `memcpy_s`, `sprintf_s`, `fscanf_s`
- Extension tests (`ext/`) and `cpuid` are excluded when building for ARM or with `_XM_NO_INTRINSICS_`
- The `testfma4` test only runs on AMD CPUs (checked via registry on Windows)

## Warning Suppression

Tests intentionally suppress many warnings since test code exercises edge cases:

- **MSVC**: Warnings are suppressed in `math3.h` via `#pragma warning(disable: ...)` with comments explaining each
- **Clang**: Uses `#pragma clang diagnostic ignored` in `math3.h`
- **GCC**: Uses `#pragma GCC diagnostic ignored` for known benign warnings
- When adding new test code, prefer suppressing warnings locally with `#pragma` push/pop rather than globally

## Code Style

Follow the same conventions as the parent DirectXMath project:

- **File headers**: Use the standard copyright block with `https://go.microsoft.com/fwlink/?LinkID=615560`
- **Indentation**: 4 spaces, CRLF line endings, latin1 charset
- **Naming**: PascalCase for test functions and types, UPPERCASE for macros and constants
- **Includes**: Include `math3.h` first in math3 test files; include DirectXMath headers via angle brackets (`<DirectXMath.h>`)
- **Namespaces**: Use `using namespace DirectX;` and `using namespace DirectX::PackedVector;` at file scope in test files
- **Section separators**: Use `//-------------------------------------------------------------------------------------` between major sections

## References

- [DirectXMath Test Suite Wiki](https://github.com/walbourn/directxmathtest/wiki)
- [DirectXMath Source Repository](https://github.com/microsoft/DirectXMath)
- [DirectXMath Documentation](https://learn.microsoft.com/windows/win32/dxmath/directxmath-portal)
- [DirectXMath Wiki](https://github.com/microsoft/DirectXMath/wiki)
