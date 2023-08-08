itk_module_test()

set(IOOpenJPHTests
  itkOpenJPHImageIOTest.cxx
  )

CreateTestDriver(IOOpenJPH "${IOOpenJPH-Test_LIBRARIES}" "${IOOpenJPHTests}")

itk_add_test(NAME itkOpenJPHImageIOConvertTest
  COMMAND IOOpenJPHTestDriver
  # --compare
  #     DATA{Baseline/CT1.mha}
  #     ${ITK_TEST_OUTPUT_DIR}/CT1.mha
  itkOpenJPHImageIOTest
    DATA{Input/CT1.j2c}
    ${ITK_TEST_OUTPUT_DIR}/CT1.nrrd
    ${ITK_TEST_OUTPUT_DIR}/CT1Written.j2c
    ${ITK_TEST_OUTPUT_DIR}/CT1RoundTrip.mha
)