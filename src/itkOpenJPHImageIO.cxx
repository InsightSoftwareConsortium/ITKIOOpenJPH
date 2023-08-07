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

#include <algorithm>
#include <ctime>

namespace itk
{

OpenJPHImageIO::OpenJPHImageIO()
{
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
}

bool
OpenJPHImageIO::CanReadFile(const char * filename)
{
  try
  {
    std::ifstream infile;
    this->OpenFileForReading(infile, filename);

    bool canRead = false;
    // if (infile.good())
    // {
    //   // header is a 512 byte block
    //   char buffer[512];
    //   infile.read(buffer, 512);
    //   if (!infile.bad())
    //   {
    //     int fileType = OpenJPHImageIO::CheckVersion(buffer);
    //     canRead = (fileType > 0);
    //   }
    // }

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

  std::ifstream infile;
  this->OpenFileForReading(infile, this->m_FileName);

  // if (fileType == 0)
  // {
  //   infile.close();
  //   itkExceptionMacro("Unrecognized header in: " << m_FileName);
  // }

  infile.close();

  // this->PopulateMetaDataDictionary();
}

// void
// OpenJPHImageIO::PopulateMetaDataDictionary()
// {
  // MetaDataDictionary & thisDic = this->GetMetaDataDictionary();
  // EncapsulateMetaData<std::string>(thisDic, "Version", std::string(this->m_Version));
  // EncapsulateMetaData<std::string>(thisDic, "PatientName", std::string(this->m_PatientName));
  // EncapsulateMetaData<int>(thisDic, "PatientIndex", this->m_PatientIndex);
  // EncapsulateMetaData<int>(thisDic, "ScannerID", this->m_ScannerID);
  // EncapsulateMetaData<std::string>(thisDic, "CreationDate", std::string(this->m_CreationDate));
  // EncapsulateMetaData<std::string>(thisDic, "ModificationDate", std::string(this->m_ModificationDate));
  // EncapsulateMetaData<double>(thisDic, "SliceThickness", this->m_SliceThickness);
  // EncapsulateMetaData<double>(thisDic, "SliceIncrement", this->m_SliceIncrement);
  // std::vector<double> dataRange(2);
  // dataRange[0] = this->m_DataRange[0];
  // dataRange[1] = this->m_DataRange[1];
  // EncapsulateMetaData<std::vector<double>>(thisDic, "DataRange", dataRange);
  // EncapsulateMetaData<double>(thisDic, "MuScaling", this->m_MuScaling);
  // EncapsulateMetaData<int>(thisDic, "NumberOfSamples", this->m_NumberOfSamples);
  // EncapsulateMetaData<int>(thisDic, "NumberOfProjections", this->m_NumberOfProjections);
  // EncapsulateMetaData<double>(thisDic, "ScanDistance", this->m_ScanDistance);
  // EncapsulateMetaData<double>(thisDic, "SampleTime", this->m_SampleTime);
  // EncapsulateMetaData<int>(thisDic, "ScannerType", this->m_ScannerType);
  // EncapsulateMetaData<int>(thisDic, "MeasurementIndex", this->m_MeasurementIndex);
  // EncapsulateMetaData<int>(thisDic, "Site", this->m_Site);
  // EncapsulateMetaData<int>(thisDic, "ReconstructionAlg", this->m_ReconstructionAlg);
  // EncapsulateMetaData<double>(thisDic, "ReferenceLine", this->m_ReferenceLine);
  // EncapsulateMetaData<double>(thisDic, "Energy", this->m_Energy);
  // EncapsulateMetaData<double>(thisDic, "Intensity", this->m_Intensity);
  // EncapsulateMetaData<int>(thisDic, "RescaleType", this->m_RescaleType);
  // EncapsulateMetaData<std::string>(thisDic, "RescaleUnits", std::string(this->m_RescaleUnits));
  // EncapsulateMetaData<std::string>(thisDic, "CalibrationData", std::string(this->m_CalibrationData));
  // EncapsulateMetaData<double>(thisDic, "RescaleSlope", this->m_RescaleSlope);
  // EncapsulateMetaData<double>(thisDic, "RescaleIntercept", this->m_RescaleIntercept);
  // EncapsulateMetaData<double>(thisDic, "MuWater", this->m_MuWater);
// }

// void
// OpenJPHImageIO::SetHeaderFromMetaDataDictionary()
// {
  // MetaDataDictionary & metaData = this->GetMetaDataDictionary();

  // std::string stringMeta;
  // if (ExposeMetaData<std::string>(metaData, "Version", stringMeta))
  // {
  //   strncpy(this->m_Version, stringMeta.c_str(), 18);
  // }
  // if (ExposeMetaData<std::string>(metaData, "PatientName", stringMeta))
  // {
  //   strncpy(this->m_PatientName, stringMeta.c_str(), 42);
  // }

  // ExposeMetaData<int>(metaData, "PatientIndex", this->m_PatientIndex);
  // ExposeMetaData<int>(metaData, "ScannerID", this->m_ScannerID);

  // if (ExposeMetaData<std::string>(metaData, "CreationDate", stringMeta))
  // {
  //   strncpy(this->m_CreationDate, stringMeta.c_str(), 32);
  // }
  // if (ExposeMetaData<std::string>(metaData, "ModificationDate", stringMeta))
  // {
  //   strncpy(this->m_ModificationDate, stringMeta.c_str(), 32);
  // }

  // ExposeMetaData<double>(metaData, "SliceThickness", this->m_SliceThickness);
  // ExposeMetaData<double>(metaData, "SliceIncrement", this->m_SliceIncrement);

  // std::vector<double> dataRange(2);
  // if (ExposeMetaData<std::vector<double>>(metaData, "DataRange", dataRange))
  // {
  //   this->m_DataRange[0] = dataRange[0];
  //   this->m_DataRange[1] = dataRange[1];
  // }

  // ExposeMetaData<double>(metaData, "MuScaling", this->m_MuScaling);
  // ExposeMetaData<int>(metaData, "NumberOfSamples", this->m_NumberOfSamples);
  // ExposeMetaData<int>(metaData, "NumberOfProjections", this->m_NumberOfProjections);
  // ExposeMetaData<double>(metaData, "ScanDistance", this->m_ScanDistance);
  // ExposeMetaData<double>(metaData, "SampleTime", this->m_SampleTime);
  // ExposeMetaData<int>(metaData, "ScannerType", this->m_ScannerType);
  // ExposeMetaData<int>(metaData, "MeasurementIndex", this->m_MeasurementIndex);
  // ExposeMetaData<int>(metaData, "Site", this->m_Site);
  // ExposeMetaData<int>(metaData, "ReconstructionAlg", this->m_ReconstructionAlg);
  // ExposeMetaData<double>(metaData, "ReferenceLine", this->m_ReferenceLine);
  // ExposeMetaData<double>(metaData, "Energy", this->m_Energy);
  // ExposeMetaData<double>(metaData, "Intensity", this->m_Intensity);
  // ExposeMetaData<double>(metaData, "Intensity", this->m_Intensity);

  // ExposeMetaData<int>(metaData, "RescaleType", this->m_RescaleType);
  // if (ExposeMetaData<std::string>(metaData, "RescaleUnits", stringMeta))
  // {
  //   strncpy(this->m_RescaleUnits, stringMeta.c_str(), 18);
  // }
  // if (ExposeMetaData<std::string>(metaData, "CalibrationData", stringMeta))
  // {
  //   strncpy(this->m_CalibrationData, stringMeta.c_str(), 66);
  // }

  // ExposeMetaData<double>(metaData, "RescaleSlope", this->m_RescaleSlope);
  // ExposeMetaData<double>(metaData, "RescaleIntercept", this->m_RescaleIntercept);
  // ExposeMetaData<double>(metaData, "MuWater", this->m_MuWater);
// }

void
OpenJPHImageIO::Read(void * buffer)
{
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

} // end namespace itk
