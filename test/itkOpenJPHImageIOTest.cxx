/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include <fstream>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkOpenJPHImageIO.h"
#include "itkOpenJPHImageIOFactory.h"
#include "itkTestingMacros.h"


#define SPECIFIC_IMAGEIO_MODULE_TEST

template <typename TPixel>
int
testPixelType(const std::string & inputJ2CFileName, const std::string & outputFileName, const std::string & outputJ2CFileName, const std::string & outputRoundTripFileName)
{
  constexpr unsigned int Dimension = 2;
  using PixelType = TPixel;
  using ImageType = itk::Image<PixelType, Dimension>;
  typename ImageType::Pointer image;

  using ReaderType = itk::ImageFileReader<ImageType>;
  typename ReaderType::Pointer reader = ReaderType::New();

  itk::OpenJPHImageIOFactory::RegisterOneFactory();

  // Read the file without explicitly requesting OpenJPHIO
  ITK_TRY_EXPECT_NO_EXCEPTION(image = itk::ReadImage<ImageType>(inputJ2CFileName));
  image->Print(std::cout);
  itk::MetaDataDictionary & metaData = image->GetMetaDataDictionary(); // Get metadata from regularly read image

  int numberOfDecompositions = 0;
  itk::ExposeMetaData<int>(metaData, "NumberOfDecompositions", numberOfDecompositions);
  ITK_TEST_EXPECT_EQUAL(numberOfDecompositions, 5);

  bool isReversible = false;
  itk::ExposeMetaData<bool>(metaData, "IsReversible", isReversible);
  ITK_TEST_EXPECT_EQUAL(isReversible, true);

  ITK_TRY_EXPECT_NO_EXCEPTION(itk::WriteImage(image, outputFileName, true));

  typename ImageType::Pointer writtenImage;
  ITK_TRY_EXPECT_NO_EXCEPTION(writtenImage = itk::ReadImage<ImageType>(outputFileName));

  // Read the file explicitly requesting OpenJPHIO
  typename itk::OpenJPHImageIO::Pointer jphIO = itk::OpenJPHImageIO::New();
  ITK_TEST_EXPECT_TRUE(jphIO->CanWriteFile(outputJ2CFileName.c_str()));


  using WriterType = itk::ImageFileWriter<ImageType>;
  typename WriterType::Pointer writer = WriterType::New();
  ITK_TEST_EXPECT_TRUE(!jphIO->CanWriteFile((outputJ2CFileName + ".exe").c_str()));
  writer->SetImageIO(jphIO);
  writer->SetInput(writtenImage);
  writer->SetFileName(outputJ2CFileName);
  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  typename ImageType::Pointer writtenImageBack;
  ITK_TRY_EXPECT_NO_EXCEPTION(writtenImageBack = itk::ReadImage<ImageType>(outputJ2CFileName));

  ITK_TRY_EXPECT_NO_EXCEPTION(itk::WriteImage(writtenImageBack, outputRoundTripFileName, true));

  std::cout << "Test finished" << std::endl;
  return EXIT_SUCCESS;
}

int
itkOpenJPHImageIOTest(int argc, char * argv[])
{
  if (argc < 5)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << itkNameOfTestExecutableMacro(argv) << " InputJ2C Output OutputJ2C OutputRoundTrip" << std::endl;
    return EXIT_FAILURE;
  }
  const std::string inputJ2CFileName = argv[1];
  const std::string outputFileName = argv[2];
  const std::string outputJ2CFileName = argv[3];
  const std::string outputRoundTripFileName = argv[4];

  using IOType = itk::OpenJPHImageIO;
  IOType::Pointer jphIO = IOType::New();

  ITK_TEST_EXPECT_TRUE(jphIO->CanReadFile(inputJ2CFileName.c_str()));
  ITK_TEST_EXPECT_TRUE(jphIO->CanWriteFile(outputJ2CFileName.c_str()));
  jphIO->SetFileName(inputJ2CFileName);

  ITK_TRY_EXPECT_NO_EXCEPTION(jphIO->ReadImageInformation());

  ITK_EXERCISE_BASIC_OBJECT_METHODS(jphIO, OpenJPHImageIO, ImageIOBase);

  auto componentType = jphIO->GetComponentType();

  switch (componentType)
  {
    case itk::ImageIOBase::IOComponentEnum::UCHAR:
      return testPixelType<unsigned char>(inputJ2CFileName, outputFileName, outputJ2CFileName, outputRoundTripFileName);
      break;
    case itk::ImageIOBase::IOComponentEnum::CHAR:
      return testPixelType<char>(inputJ2CFileName, outputFileName, outputJ2CFileName, outputRoundTripFileName);
      break;
    case itk::ImageIOBase::IOComponentEnum::USHORT:
      return testPixelType<unsigned short>(inputJ2CFileName, outputFileName, outputJ2CFileName, outputRoundTripFileName);
      break;
    case itk::ImageIOBase::IOComponentEnum::SHORT:
      return testPixelType<short>(inputJ2CFileName, outputFileName, outputJ2CFileName, outputRoundTripFileName);
      break;
    default:
      std::cerr << "Unsupported input image pixel component type: ";
      std::cerr << itk::ImageIOBase::GetComponentTypeAsString(componentType);
      std::cerr << std::endl;
      return EXIT_FAILURE;
  }
}
