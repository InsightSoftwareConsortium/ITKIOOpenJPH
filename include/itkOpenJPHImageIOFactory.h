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
#ifndef itkOpenJPHImageIOFactory_h
#define itkOpenJPHImageIOFactory_h
#include "IOOpenJPHExport.h"

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace itk
{
/** \class OpenJPHImageIOFactory
 * \brief Create instances of OpenJPHImageIO objects using an object factory.
 * \ingroup ITKIOOpenJPH
 */
class IOOpenJPH_EXPORT OpenJPHImageIOFactory : public ObjectFactoryBase
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(OpenJPHImageIOFactory);

  /** Standard class typedefs. */
  using Self = OpenJPHImageIOFactory;
  using Superclass = ObjectFactoryBase;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Class methods used to interface with the registered factories. */
  const char *
  GetITKSourceVersion() const override;

  const char *
  GetDescription() const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpenJPHImageIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void
  RegisterOneFactory()
  {
    OpenJPHImageIOFactory::Pointer openJphFactory = OpenJPHImageIOFactory::New();

    ObjectFactoryBase::RegisterFactoryInternal(openJphFactory);
  }

protected:
  OpenJPHImageIOFactory();
  ~OpenJPHImageIOFactory() override = default;
};
} // end namespace itk

#endif
