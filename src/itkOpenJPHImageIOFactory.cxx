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
#include "itkOpenJPHImageIOFactory.h"
#include "itkOpenJPHImageIO.h"
#include "itkVersion.h"

namespace itk
{
OpenJPHImageIOFactory::OpenJPHImageIOFactory()
{
  this->RegisterOverride(
    "itkImageIOBase", "itkOpenJPHImageIO", "OpenJPH Image IO", true, CreateObjectFunction<OpenJPHImageIO>::New());
}

const char *
OpenJPHImageIOFactory::GetITKSourceVersion() const
{
  return ITK_SOURCE_VERSION;
}

const char *
OpenJPHImageIOFactory::GetDescription() const
{
  return "OpenJPH ImageIO Factory, allows the loading of HTJ2K images into insight";
}

// Undocumented API used to register during static initialization.
// DO NOT CALL DIRECTLY.

static bool OpenJPHImageIOFactoryHasBeenRegistered;

void IOOpenJPH_EXPORT
     OpenJPHImageIOFactoryRegister__Private()
{
  if (!OpenJPHImageIOFactoryHasBeenRegistered)
  {
    OpenJPHImageIOFactoryHasBeenRegistered = true;
    OpenJPHImageIOFactory::RegisterOneFactory();
  }
}

} // end namespace itk
