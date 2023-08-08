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
#include "itkOpenJPHImageIO.h"
#include "itkSpatialOrientationAdapter.h"
#include "itkIOCommon.h"
#include "itksys/SystemTools.hxx"
#include "itkMath.h"
#include "itkIntTypes.h"
#include "itkByteSwapper.h"
#include "itkMetaDataObject.h"

#include <fstream>
#include <algorithm>
#include <iterator>
#include <ctime>

#include "ojphInformation.h"
#include "ojphHTJ2KEncoder.h"
#include "ojphHTJ2KDecoder.h"

namespace itk
{

OpenJPHImageIO::OpenJPHImageIO()
{
  this->m_Encoder = std::make_unique<OpenJPH::HTJ2KEncoder>();
  this->m_Decoder = std::make_unique<OpenJPH::HTJ2KDecoder>();

  this->SetNumberOfDimensions(2);
  this->AddSupportedWriteExtension(".j2c");
  this->AddSupportedReadExtension(".j2c");
}


OpenJPHImageIO::~OpenJPHImageIO()
{
}


void
OpenJPHImageIO::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "OpenJPH Version: " << GetOpenJPHVersion() << std::endl;
  os << indent << "SIMD Level: " << GetSIMDLevel() << std::endl;
}

bool
OpenJPHImageIO::CanReadFile(const char * filename)
{
  try
  {
    bool canRead = this->HasSupportedReadExtension(filename);

    std::ifstream infile;
    this->OpenFileForReading(infile, filename);
    infile.close();

    return canRead;
  }
  catch (...) // file cannot be opened, access denied etc.
  {
    return false;
  }
}

void
OpenJPHImageIO::ReadImageInformation()
{
  if (this->m_FileName.empty())
  {
    itkExceptionMacro("FileName has not been set.");
  }

  std::vector<uint8_t> & encodedBytes = this->m_Decoder->getEncodedBytes();
  this->ReadFile(this->m_FileName, encodedBytes);

  this->m_Decoder->readHeader();

  const auto size = this->m_Decoder->calculateSizeAtDecompositionLevel(this->m_DecompositionLevel);
  this->SetDimensions(0, size.width);
  this->SetDimensions(1, size.height);

  const auto frameInfo = this->m_Decoder->getFrameInfo();
  if (frameInfo.bitsPerSample <= 8)
  {
    if (frameInfo.isSigned)
    {
      this->SetComponentType(IOComponentEnum::CHAR);
    }
    else
    {
      this->SetComponentType(IOComponentEnum::UCHAR);
    }
  }
  else if (frameInfo.bitsPerSample <= 16)
  {
    if (frameInfo.isSigned)
    {
      this->SetComponentType(IOComponentEnum::SHORT);
    }
    else
    {
      this->SetComponentType(IOComponentEnum::USHORT);
    }
  }
  else
  {
    itkExceptionMacro("OpenJPHImageIO only supports 8 and 16 bit images.");
  }

  if (frameInfo.componentCount == 1)
  {
    this->SetNumberOfComponents(1);
    this->SetPixelType(IOPixelEnum::SCALAR);
  }
  else
  {
    this->SetNumberOfComponents(frameInfo.componentCount);
    this->SetPixelType(IOPixelEnum::VARIABLELENGTHVECTOR);
  }

  const auto imageOffset = this->m_Decoder->getImageOffset();
  this->SetOrigin(0, imageOffset.x);
  this->SetOrigin(1, imageOffset.y);

  MetaDataDictionary & thisDic = this->GetMetaDataDictionary();
  // Number of wavelet decompositions
  EncapsulateMetaData<int>(thisDic, "NumberOfDecompositions", this->m_Decoder->getNumDecompositions());
  EncapsulateMetaData<bool>(thisDic, "IsReversible", this->m_Decoder->getIsReversible());
  EncapsulateMetaData<bool>(thisDic, "ProgressionOrder", this->m_Decoder->getProgressionOrder());

  std::vector<uint32_t> tileSize(2);
  tileSize[0] = this->m_Decoder->getTileSize().width;
  tileSize[1] = this->m_Decoder->getTileSize().height;
  EncapsulateMetaData<decltype(tileSize)>(thisDic, "TileSize" , tileSize);

  std::vector<uint32_t> tileOffset(2);
  tileOffset[0] = this->m_Decoder->getTileOffset().x;
  tileOffset[1] = this->m_Decoder->getTileOffset().y;
  EncapsulateMetaData<decltype(tileOffset)>(thisDic, "TileOffset" , tileOffset);

  std::vector<uint32_t> blockDimensions(2);
  blockDimensions[0] = this->m_Decoder->getBlockDimensions().width;
  blockDimensions[1] = this->m_Decoder->getBlockDimensions().height;
  EncapsulateMetaData<decltype(blockDimensions)>(thisDic, "BlockDimensions" , blockDimensions);

  const auto numberOfLayers = this->m_Decoder->getNumLayers();
  EncapsulateMetaData<decltype(numberOfLayers)>(thisDic, "NumberOfLayers" , numberOfLayers);

  const auto isUsingColorTransform = this-m_Decoder->getIsUsingColorTransform();
  EncapsulateMetaData<decltype(isUsingColorTransform)>(thisDic, "IsUsingColorTransform" , isUsingColorTransform);
}


void
OpenJPHImageIO::Read(void * buffer)
{
  std::vector<uint8_t> & encodedBytes = this->m_Decoder->getEncodedBytes();
  this->ReadFile(this->m_FileName, encodedBytes);

  this->m_Decoder->decode();

  const std::vector<uint8_t> & decodedBytes = this->m_Decoder->getDecodedBytes();

  std::memcpy(buffer, decodedBytes.data(), decodedBytes.size());
}


bool
OpenJPHImageIO::CanWriteFile(const char * name)
{
  const std::string filename = name;

  if (filename.empty())
  {
    return false;
  }

  return this->HasSupportedWriteExtension(name, true);
}


void
OpenJPHImageIO::WriteImageInformation()
{
  if (this->m_FileName.empty())
  {
    itkExceptionMacro("FileName has not been set.");
  }

  std::ofstream outFile;
  this->OpenFileForWriting(outFile, m_FileName);

  // this->WriteISQHeader(&outFile);

  outFile.close();
}


void
OpenJPHImageIO::Write(const void * buffer)
{
  this->WriteImageInformation();

  // std::ofstream outFile;
  // this->OpenFileForWriting(outFile, m_FileName, false);
  // outFile.seekp(this->m_HeaderSize, std::ios::beg);

  // const auto numberOfBytes = static_cast<SizeValueType>(this->GetImageSizeInBytes());
  // const auto numberOfComponents = static_cast<SizeValueType>(this->GetImageSizeInComponents());

  // if (this->GetComponentType() != IOComponentEnum::SHORT)
  // {
  //   itkExceptionMacro("OpenJPHImageIO only supports writing short files.");
  // }

  // if (ByteSwapper<short>::SystemIsBigEndian())
  // {
  //   char * tempmemory = new char[numberOfBytes];
  //   memcpy(tempmemory, buffer, numberOfBytes);
  //   {
  //     ByteSwapper<short>::SwapRangeFromSystemToBigEndian(reinterpret_cast<short *>(tempmemory), numberOfComponents);
  //   }

  //   // Write the actual pixel data
  //   outFile.write(static_cast<const char *>(tempmemory), numberOfBytes);
  //   delete[] tempmemory;
  // }
  // else
  // {
  //   outFile.write(static_cast<const char *>(buffer), numberOfBytes);
  // }

  // outFile.close();
}

std::string
OpenJPHImageIO
::GetOpenJPHVersion()
{
  return OpenJPH::getVersion();
}

int
OpenJPHImageIO
::GetSIMDLevel()
{
  return OpenJPH::getSIMDLevel();
}

void
OpenJPHImageIO
::ReadFile(const std::string & fileName, std::vector<uint8_t> & buffer)
{
  std::ifstream istrm(fileName, std::ios::in | std::ios::binary);
  // Stop eating new lines in binary mode!
  istrm.unsetf(std::ios::skipws);

  std::streampos fileSize;
  istrm.seekg(0, std::ios::end);
  fileSize = istrm.tellg();
  istrm.seekg(0, std::ios::beg);

  // reserve capacity
  buffer.reserve(fileSize);

  // read the data:
  buffer.insert(buffer.begin(),
                std::istream_iterator<uint8_t>(istrm),
                std::istream_iterator<uint8_t>());
}


void
OpenJPHImageIO
::WriteFile(const std::string & fileName, const std::vector<uint8_t> & buffer)
{
  std::ofstream ostrm(fileName, std::ios::out | std::ofstream::binary);
  std::copy(buffer.begin(), buffer.end(), std::ostreambuf_iterator<char>(ostrm));
}

} // end namespace itk
