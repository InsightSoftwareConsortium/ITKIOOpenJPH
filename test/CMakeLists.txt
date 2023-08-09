itk_module_test()

set(IOOpenJPHTests
  itkOpenJPHImageIOTest.cxx
  )

CreateTestDriver(IOOpenJPH "${IOOpenJPH-Test_LIBRARIES}" "${IOOpenJPHTests}")

itk_add_test(NAME itkOpenJPHImageIOConvertCT1Test
  COMMAND IOOpenJPHTestDriver
  --compare
      DATA{Baseline/CT1RoundTrip.mha}
      ${ITK_TEST_OUTPUT_DIR}/CT1RoundTrip.mha
  itkOpenJPHImageIOTest
    DATA{Input/CT1.j2c}
    ${ITK_TEST_OUTPUT_DIR}/CT1.mha
    ${ITK_TEST_OUTPUT_DIR}/CT1Written.j2c
    ${ITK_TEST_OUTPUT_DIR}/CT1RoundTrip.mha
)

itk_add_test(NAME itkOpenJPHImageIOConvertMR1Test
  COMMAND IOOpenJPHTestDriver
  --compare
      DATA{Baseline/MR1RoundTrip.mha}
      ${ITK_TEST_OUTPUT_DIR}/MR1RoundTrip.mha
  itkOpenJPHImageIOTest
    DATA{Input/MR1.j2c}
    ${ITK_TEST_OUTPUT_DIR}/MR1.mha
    ${ITK_TEST_OUTPUT_DIR}/MR1Written.j2c
    ${ITK_TEST_OUTPUT_DIR}/MR1RoundTrip.mha
)

itk_add_test(NAME itkOpenJPHImageIOConvertRG1Test
  COMMAND IOOpenJPHTestDriver
  --compare
      DATA{Baseline/RG1RoundTrip.mha}
      ${ITK_TEST_OUTPUT_DIR}/RG1RoundTrip.mha
  itkOpenJPHImageIOTest
    DATA{Input/RG1.j2c}
    ${ITK_TEST_OUTPUT_DIR}/RG1.mha
    ${ITK_TEST_OUTPUT_DIR}/RG1Written.j2c
    ${ITK_TEST_OUTPUT_DIR}/RG1RoundTrip.mha
)

itk_add_test(NAME itkOpenJPHImageIOConvertXA1Test
  COMMAND IOOpenJPHTestDriver
  --compare
      DATA{Baseline/XA1RoundTrip.mha}
      ${ITK_TEST_OUTPUT_DIR}/XA1RoundTrip.mha
  itkOpenJPHImageIOTest
    DATA{Input/XA1.j2c}
    ${ITK_TEST_OUTPUT_DIR}/XA1.mha
    ${ITK_TEST_OUTPUT_DIR}/XA1Written.j2c
    ${ITK_TEST_OUTPUT_DIR}/XA1RoundTrip.mha
)