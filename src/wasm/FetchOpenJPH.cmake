set(OJPH_BUILD_TESTS OFF CACHE BOOL "Build OpenJPH tests")
if(EMSCRIPTEN OR WASI OR CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*|aarch64.*|AARCH64)")
  set(OJPH_DISABLE_INTEL_SIMD ON CACHE BOOL "Disables the use of SIMD instruction as associated files")
  set(OJPH_ENABLE_INTEL_AVX512 OFF CACHE BOOL "Enables the use of AVX512 SIMD instructions and associated files")
endif()
set(OJPH_ENABLE_TIFF_SUPPORT OFF CACHE BOOL "Enables input and output support for TIFF files")

set(_itk_build_shared ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries?" FORCE)
include(FetchContent)
FetchContent_Declare(
  OpenJPH
  GIT_REPOSITORY https://github.com/aous72/OpenJPH.git
  # master 2023-08-07
  GIT_TAG        02c1b163b483a28f3e192ef935d258b0cbe8c751
)
FetchContent_MakeAvailable(OpenJPH)
set(BUILD_SHARED_LIBS ${_itk_build_shared} CACHE BOOL "Build shared libraries?" FORCE) # restore original flag
unset(_itk_build_shared)