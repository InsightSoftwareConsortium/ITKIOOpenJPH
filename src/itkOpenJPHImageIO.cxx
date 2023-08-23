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

  m_TileSize.SetSize(2);
  m_TileSize[0] = 512;
  m_TileSize[1] = 512;

  m_TileOffset.SetSize(2);
  m_TileOffset[0] = 0;
  m_TileOffset[1] = 0;

  m_BlockDimensions.SetSize(2);
  m_BlockDimensions[0] = 64;
  m_BlockDimensions[1] = 64;
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

  this->ReadHeader();
}

void
OpenJPHImageIO::ReadHeader()
{
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
  EncapsulateMetaData<bool>(thisDic, "NotReversible", !this->m_Decoder->getIsReversible());
  EncapsulateMetaData<int>(thisDic, "ProgressionOrder", this->m_Decoder->getProgressionOrder());

  std::vector<double> tileSize(2);
  tileSize[0] = this->m_Decoder->getTileSize().width;
  tileSize[1] = this->m_Decoder->getTileSize().height;
  EncapsulateMetaData<decltype(tileSize)>(thisDic, "TileSize" , tileSize);

  std::vector<double> tileOffset(2);
  tileOffset[0] = this->m_Decoder->getTileOffset().x;
  tileOffset[1] = this->m_Decoder->getTileOffset().y;
  EncapsulateMetaData<decltype(tileOffset)>(thisDic, "TileOffset" , tileOffset);

  std::vector<double> blockDimensions(2);
  blockDimensions[0] = this->m_Decoder->getBlockDimensions().width;
  blockDimensions[1] = this->m_Decoder->getBlockDimensions().height;
  EncapsulateMetaData<decltype(blockDimensions)>(thisDic, "BlockDimensions" , blockDimensions);

  const int numberOfLayers = this->m_Decoder->getNumLayers();
  EncapsulateMetaData<decltype(numberOfLayers)>(thisDic, "NumberOfLayers" , numberOfLayers);

  const bool isUsingColorTransform = this->m_Decoder->getIsUsingColorTransform();
  EncapsulateMetaData<decltype(isUsingColorTransform)>(thisDic, "UseColorTransform" , isUsingColorTransform);
}


void
OpenJPHImageIO::Read(void * buffer)
{
  std::vector<uint8_t> & encodedBytes = this->m_Decoder->getEncodedBytes();
  this->ReadFile(this->m_FileName, encodedBytes);

  this->m_Decoder->decodeSubResolution(this->m_DecompositionLevel);

  this->ReadHeader();

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

  this->SetFrameInfo();

  this->m_Encoder->setDecompositions(this->GetDecompositions());
  this->m_Encoder->setQuality(!this->GetNotReversible(), this->GetQuantizationStep());
  this->m_Encoder->setProgressionOrder(this->GetProgressionOrder());

  OpenJPH::Point offset;
  offset.x = this->GetOrigin(0);
  offset.y = this->GetOrigin(1);
  this->m_Encoder->setImageOffset(offset);

  OpenJPH::Size tileSize;
  tileSize.width = this->GetTileSize()[0];
  tileSize.height = this->GetTileSize()[1];
  this->m_Encoder->setTileSize(tileSize);

  OpenJPH::Point tileOffset;
  tileOffset.x = this->GetTileOffset()[0];
  tileOffset.y = this->GetTileOffset()[1];
  this->m_Encoder->setTileOffset(tileOffset);

  OpenJPH::Size blockDimensions;
  blockDimensions.width = this->GetBlockDimensions()[0];
  blockDimensions.height = this->GetBlockDimensions()[1];
  this->m_Encoder->setBlockDimensions(blockDimensions);

  this->m_Encoder->setIsUsingColorTransform(this->GetUseColorTransform());
}

void
OpenJPHImageIO::SetFrameInfo()
{
  uint8_t bitsPerSample;
  bool isSigned;
  const auto componentType = this->GetComponentType();
  switch (componentType)
  {
    case IOComponentEnum::CHAR:
      bitsPerSample = 8;
      isSigned = true;
      break;
    case IOComponentEnum::UCHAR:
      bitsPerSample = 8;
      isSigned = false;
      break;
    case IOComponentEnum::SHORT:
      bitsPerSample = 16;
      isSigned = true;
      break;
    case IOComponentEnum::USHORT:
      bitsPerSample = 16;
      isSigned = false;
      break;
    default:
      itkExceptionMacro("OpenJPHImageIO only supports 8 and 16 bit images.");
  }

  const auto width = static_cast<uint16_t>(this->GetDimensions(0));
  const auto height = static_cast<uint16_t>(this->GetDimensions(1));
  const auto numberOfComponents = static_cast<uint8_t>(this->GetNumberOfComponents());
  OpenJPH::FrameInfo frameInfo{
    width,
    height,
    bitsPerSample,
    numberOfComponents,
    isSigned,
  };
  this->m_Encoder->setFrameInfo(frameInfo);
}


void
OpenJPHImageIO::Write(const void * buffer)
{
  this->WriteImageInformation();
  this->SetFrameInfo();

  this->m_Encoder->setDecodedBytes(reinterpret_cast<const uint8_t *>(buffer));

  this->m_Encoder->encode();

  this->WriteFile(this->m_FileName, this->m_Encoder->getEncodedBytes());
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

  istrm.seekg(0, std::ios::end);
  const std::streampos fileSize = istrm.tellg();
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
