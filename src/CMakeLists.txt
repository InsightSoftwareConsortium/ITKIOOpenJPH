set(IOOpenJPH_SRCS
  itkOpenJPHImageIO.cxx
  itkOpenJPHImageIOFactory.cxx
  OpenJPHCxx/ojphInformation.cxx
  )

set(OJPH_BUILD_TESTS OFF CACHE BOOL "Build OpenJPH tests")
if(EMSCRIPTEN OR WASI)
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

itk_module_add_library(IOOpenJPH ${IOOpenJPH_SRCS})
target_link_libraries(IOOpenJPH PRIVATE openjph)
target_include_directories(IOOpenJPH PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/OpenJPHCxx)