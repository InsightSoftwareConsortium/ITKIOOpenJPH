set(IOOpenJPH_SRCS
  itkOpenJPHImageIO.cxx
  itkOpenJPHImageIOFactory.cxx
  )

itk_module_add_library(IOOpenJPH ${IOOpenJPH_SRCS})