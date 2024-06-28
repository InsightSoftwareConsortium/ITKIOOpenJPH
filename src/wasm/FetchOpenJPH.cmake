set(OJPH_BUILD_TESTS OFF CACHE BOOL "Build OpenJPH tests")
if(EMSCRIPTEN OR WASI OR CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*|aarch64.*|AARCH64)")
  set(OJPH_DISABLE_INTEL_SIMD ON CACHE BOOL "Disables the use of SIMD instruction as associated files")
  set(OJPH_ENABLE_INTEL_AVX512 OFF CACHE BOOL "Enables the use of AVX512 SIMD instructions and associated files")
  set(OJPH_BUILD_EXECUTABLE OFF CACHE BOOL "Build reference command line executables")
endif()
set(OJPH_ENABLE_TIFF_SUPPORT OFF CACHE BOOL "Enables input and output support for TIFF files")

set(_itk_build_shared ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries?" FORCE)
include(FetchContent)
FetchContent_Declare(
  OpenJPH
  GIT_REPOSITORY https://github.com/aous72/OpenJPH.git
  # upstream master 2024-06-28
  GIT_TAG         4b964c157d9ae1debfc4049c4d42e7ea9f4cb0b5
)
FetchContent_MakeAvailable(OpenJPH)
set(BUILD_SHARED_LIBS ${_itk_build_shared} CACHE BOOL "Build shared libraries?" FORCE) # restore original flag
unset(_itk_build_shared)
