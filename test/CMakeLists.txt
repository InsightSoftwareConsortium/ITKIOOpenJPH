itk_module_test()

set(IOOpenJPHTests
  itkOpenJPHImageIOTest.cxx
  )

CreateTestDriver(IOOpenJPH "${IOOpenJPH-Test_LIBRARIES}" "${IOOpenJPHTests}")

itk_add_test(NAME itkOpenJPHImageIOConvertTest
  COMMAND IOOpenJPHTestDriver
  --compare
      # DATA{Baseline/C0004255.mha}
      ${ITK_TEST_OUTPUT_DIR}/C0004255.mha
    itkOpenJPHImageIOTest
      # DATA{Input/C0004255.ISQ}
      ${ITK_TEST_OUTPUT_DIR}/C0004255.mha
  )