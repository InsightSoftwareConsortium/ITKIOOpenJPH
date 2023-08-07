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
#ifndef itkOpenJPHImageIO_h
#define itkOpenJPHImageIO_h
#include "IOOpenJPHExport.h"


#include <fstream>
#include "itkImageIOBase.h"

namespace itk
{
/** \class OpenJPHImageIO
 *
 * \brief Read and write High-throughput JPEG2000 (HTJ2K) images.
 *
 * HTJ2K is a a multiscale image format supporting lossless and lossy compression.
 * This itk::ImageIOBase is based on the OpenJPH accelerated codec implementation.
 *
 * See:
 *
 * - https://openjph.org/
 * - https://htj2k.com/
 *
 * A short description of the HTJ2K format can be found at:
 *
 * - https://ds.jpeg.org/whitepapers/jpeg-htj2k-whitepaper.pdf
 *
 * A detailed description of the HTJ2K format can be found at:
 *
 * - https://htj2k.com/wp-content/uploads/white-paper.pdf
 *
 * \ingroup IOFilters
 * \ingroup IOOpenJPH
 */
class IOOpenJPH_EXPORT OpenJPHImageIO : public ImageIOBase
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(OpenJPHImageIO);

  /** Standard class typedefs. */
  using Self = OpenJPHImageIO;
  using Superclass = ImageIOBase;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpenJPHImageIO, ImageIOBase);

  /** The different types of ImageIO's can support data of varying
   * dimensionality. For example, some file formats are strictly 2D
   * while others can support 2D, 3D, or even n-D. This method returns
   * true/false as to whether the ImageIO can support the dimension
   * indicated. */
  bool
  SupportsDimension(unsigned long dimension) override
  {
    if (dimension == 2)
    {
      return true;
    }
    return false;
  }

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  bool
  CanReadFile(const char *) override;

  /** Set the spacing and dimension information for the set filename. */
  void
  ReadImageInformation() override;

  /** Reads the data from disk into the memory buffer provided. */
  void
  Read(void * buffer) override;

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. */
  bool
  CanWriteFile(const char *) override;

  /** Set the spacing and dimension information for the set filename. */
  void
  WriteImageInformation() override;

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  void
  Write(const void * buffer) override;


  bool
  CanStreamRead() override
  {
    return false;
  }

  bool
  CanStreamWrite() override
  {
    return false;
  }

protected:
  OpenJPHImageIO();
  ~OpenJPHImageIO() override;

  void
  PrintSelf(std::ostream & os, Indent indent) const override;

private:
};
} // end namespace itk

#endif // itkOpenJPHImageIO_h
