itk_module_test()

set(IOOpenJPHTests
  itkMinimalStandardRandomVariateGeneratorTest.cxx
  itkMyFilterTest.cxx
  )

CreateTestDriver(IOOpenJPH "${IOOpenJPH-Test_LIBRARIES}" "${IOOpenJPHTests}")

itk_add_test(NAME itkMinimalStandardRandomVariateGeneratorTest
  COMMAND IOOpenJPHTestDriver itkMinimalStandardRandomVariateGeneratorTest
  )

itk_add_test(NAME itkMyFilterTest
  COMMAND IOOpenJPHTestDriver
    --compare
    DATA{Baseline/itkMyFilterTestOutput.mha}
    ${ITK_TEST_OUTPUT_DIR}/itkMyFilterTestOutput.mha
  itkMyFilterTest
    ${ITK_TEST_OUTPUT_DIR}/itkMyFilterTestOutput.mha
  )
