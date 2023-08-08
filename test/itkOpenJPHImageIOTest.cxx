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

  // ATTENTION THIS IS THE PIXEL TYPE FOR
  // THE RESULTING IMAGE
  constexpr unsigned int Dimension = 2;
  using PixelType = short;
  using ImageType = itk::Image<PixelType, Dimension>;
  ImageType::Pointer image;

  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();

  itk::OpenJPHImageIOFactory::RegisterOneFactory();
  using IOType = itk::OpenJPHImageIO;
  IOType::Pointer jphIO = IOType::New();

  reader->SetImageIO(jphIO);
  reader->SetFileName(inputJ2CFileName);

  ITK_TEST_EXPECT_TRUE(jphIO->CanReadFile(inputJ2CFileName.c_str()));
  ITK_TEST_EXPECT_TRUE(jphIO->CanWriteFile(outputJ2CFileName.c_str()));

  // Populate the IO with file's metadata
  ITK_TRY_EXPECT_NO_EXCEPTION(reader->UpdateOutputInformation());

  ITK_EXERCISE_BASIC_OBJECT_METHODS(jphIO, OpenJPHImageIO, ImageIOBase);

  // Read the file without explicitly requesting OpenJPHIO
  ITK_TRY_EXPECT_NO_EXCEPTION(image = itk::ReadImage<ImageType>(inputJ2CFileName));
  image->Print(std::cout);
  itk::MetaDataDictionary & metaData = image->GetMetaDataDictionary(); // Get metadata from regularly read image

  ITK_TRY_EXPECT_NO_EXCEPTION(itk::WriteImage(image, outputFileName));

  // using WriterType = itk::ImageFileWriter<ImageType>;
  // WriterType::Pointer writer = WriterType::New();
  // if (argc > 3) // Explicitly use jphIO
  // {
  //   ITK_TEST_EXPECT_TRUE(jphIO->CanWriteFile(outputFileName.c_str()));

  //   ITK_TEST_EXPECT_TRUE(!jphIO->CanWriteFile((outputFileName + ".exe").c_str()));
  //   writer->SetImageIO(jphIO);
  // }
  // writer->SetInput(image);
  // writer->SetFileName(outputFileName);
  // ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished" << std::endl;
  return EXIT_SUCCESS;
}
