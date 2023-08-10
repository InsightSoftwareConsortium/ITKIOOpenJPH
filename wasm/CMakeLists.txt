cmake_minimum_required(VERSION 3.16)
project(itkwasm-htj2k LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(EMSCRIPTEN)
  set(io_components
   )
elseif(WASI)
  set(io_components
    ITKIOMeta
    )
else()
  set(io_components
    ITKImageIO
    )
endif()
find_package(ITK REQUIRED
  COMPONENTS
    ${io_components}
    WebAssemblyInterface
  )
include(${ITK_USE_FILE})

include(${CMAKE_CURRENT_SOURCE_DIR}/../src/FetchOpenJPH.cmake)

add_executable(decode decode.cxx)
target_link_libraries(decode PUBLIC ${ITK_LIBRARIES} openjph)
target_include_directories(decode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/OpenJPHCxx)
if(WASI OR EMSCRIPTEN)
  target_compile_definitions(decode PUBLIC -DOJPH_ENABLE_WASM_SIMD)
endif()

add_executable(encode encode.cxx)
target_link_libraries(encode PUBLIC ${ITK_LIBRARIES} openjph)
target_include_directories(encode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/OpenJPHCxx)
if(WASI OR EMSCRIPTEN)
  target_compile_definitions(encode PUBLIC -DOJPH_ENABLE_WASM_SIMD)
endif()

enable_testing()
add_test(NAME decode-test
  COMMAND decode
    ${CMAKE_CURRENT_SOURCE_DIR}/test/data/input/CT1.j2c
    ${CMAKE_CURRENT_BINARY_DIR}/CT1.mha
  )

add_test(NAME decode-decomposition-level-test
  COMMAND decode
    ${CMAKE_CURRENT_SOURCE_DIR}/test/data/input/CT1.j2c
    ${CMAKE_CURRENT_BINARY_DIR}/CT1-decomposition-level-4.mha
    --decomposition-level 4
  )

add_test(NAME decode-information-only-test
  COMMAND decode
    ${CMAKE_CURRENT_SOURCE_DIR}/test/data/input/CT1.j2c
    ${CMAKE_CURRENT_BINARY_DIR}/CT1-information-only.mha
    --information-only
  )

add_test(NAME encode-test
  COMMAND encode
    ${CMAKE_CURRENT_SOURCE_DIR}/test/data/input/CT1.mha
    ${CMAKE_CURRENT_BINARY_DIR}/CT1.j2c
  )