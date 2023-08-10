set(IOOpenJPH_SRCS
  itkOpenJPHImageIO.cxx
  itkOpenJPHImageIOFactory.cxx
  OpenJPHCxx/ojphInformation.cxx
  )

include(${CMAKE_CURRENT_SOURCE_DIR}/FetchOpenJPH.cmake)

itk_module_add_library(IOOpenJPH ${IOOpenJPH_SRCS})
target_link_libraries(IOOpenJPH PRIVATE openjph)
target_include_directories(IOOpenJPH PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/OpenJPHCxx)