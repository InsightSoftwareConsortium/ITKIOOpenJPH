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
#include "itkPipeline.h"
#include "itkInputImage.h"
#include "itkOutputBinaryStream.h"
#include "itkImage.h"
#include "itkVector.h"
#include "itkVectorImage.h"
#include "itkSupportInputImageTypes.h"
#include "itkDefaultConvertPixelTraits.h"
#include "itkWasmMapComponentType.h"

#include "ojphHTJ2KEncoder.h"

static auto openjphEncoder = std::make_unique<OpenJPH::HTJ2KEncoder>();

template<typename TImage>
class PipelineFunctor
{
public:
  int operator()(itk::wasm::Pipeline & pipeline)
  {
    using ImageType = TImage;

    constexpr unsigned int Dimension = ImageType::ImageDimension;

    using InputImageType = itk::wasm::InputImage<ImageType>;
    InputImageType inputImage;
    pipeline.add_option("image", inputImage, "Input image")->type_name("INPUT_IMAGE")->required();

    int decompositions = 5;
    pipeline.add_option("-d,--decompositions", decompositions, "Number of wavelet decompositions");

    bool notReversible = false;
    pipeline.add_flag("-r,--not-reversible", notReversible, "Lossy and not reversible, i.e. not lossless, encoding");

    float quantizationStep = 1.0f;
    pipeline.add_option("-q,--quantization-step", quantizationStep, "Quantization step size for lossy compression. Ignored unless not reversible is enabled.")->check(CLI::Range(0.0f, 1.0f));

    int progressionOrder = 2;
    pipeline.add_option("-p,--progression-order", progressionOrder, "Progression order. 0 = LRCP, 1 = RLCP, 2 = RPCL, 3 = PCRL, 4 = CPRL");

    std::vector<int> tileSize = {512, 512};
    pipeline.add_option("-t,--tile-size", tileSize, "Tile size")->type_size(Dimension);

    std::vector<int> tileOffset = {0, 0};
    pipeline.add_option("-o,--tile-offset", tileOffset, "Tile offset")->type_size(Dimension);

    std::vector<int> blockDimensions = {64, 64};
    pipeline.add_option("-b,--block-dimensions", blockDimensions, "Block dimensions")->type_size(Dimension);

    using OutputBinaryStreamType = itk::wasm::OutputBinaryStream;
    OutputBinaryStreamType outputBinaryStream;
    pipeline.add_option("output", outputBinaryStream, "Output binary HTJ2K codestream")->type_name("OUTPUT_BINARY_STREAM")->required();

    ITK_WASM_PARSE(pipeline);

    typename ImageType::ConstPointer image = inputImage.Get();

    OpenJPH::FrameInfo frameInfo;

    const auto size = image->GetLargestPossibleRegion().GetSize();
    frameInfo.width = size[0];
    frameInfo.height = size[1];

    frameInfo.componentCount = image->GetNumberOfComponentsPerPixel();

    using PixelType = typename ImageType::IOPixelType;
    using ConvertPixelTraits = itk::DefaultConvertPixelTraits<PixelType>;
    using ComponentType = typename ConvertPixelTraits::ComponentType;
    const auto componentType = itk::wasm::MapComponentType<ComponentType>::ComponentString;

    if (componentType == "uint8")
      {
      frameInfo.bitsPerSample = 8;
      frameInfo.isSigned = false;
      }
    else if (componentType == "int8")
      {
      frameInfo.bitsPerSample = 8;
      frameInfo.isSigned = true;
      }
    else if (componentType == "uint16")
      {
      frameInfo.bitsPerSample = 16;
      frameInfo.isSigned = false;
      }
    else if (componentType == "int16")
      {
      frameInfo.bitsPerSample = 16;
      frameInfo.isSigned = true;
      }

    openjphEncoder->setFrameInfo(frameInfo);
    openjphEncoder->setDecodedBytes(reinterpret_cast<const uint8_t *>(image->GetBufferPointer()));

    openjphEncoder->setDecompositions(decompositions);
    openjphEncoder->setQuality(!notReversible, quantizationStep);
    openjphEncoder->setProgressionOrder(progressionOrder);
    OpenJPH::Point offset;
    const auto origin = image->GetOrigin();
    offset.x = origin[0];
    offset.y = origin[1];
    openjphEncoder->setImageOffset(offset);
    OpenJPH::Size ojphTileSize;
    ojphTileSize.width = tileSize[0];
    ojphTileSize.height = tileSize[1];
    openjphEncoder->setTileSize(ojphTileSize);
    OpenJPH::Point ojphTileOffset;
    ojphTileOffset.x = tileOffset[0];
    ojphTileOffset.y = tileOffset[1];
    openjphEncoder->setTileOffset(ojphTileOffset);
    OpenJPH::Size ojphBlockDimensions;
    ojphBlockDimensions.width = blockDimensions[0];
    ojphBlockDimensions.height = blockDimensions[1];
    openjphEncoder->setBlockDimensions(ojphBlockDimensions);
    // Todo, expose these options
    openjphEncoder->setIsUsingColorTransform(false);

    ITK_WASM_CATCH_EXCEPTION(pipeline, openjphEncoder->encode());

    const std::vector<uint8_t> & encodedBytes = openjphEncoder->getEncodedBytes();
    std::ostream_iterator<char> oIt(outputBinaryStream.Get());
    std::copy(encodedBytes.begin(), encodedBytes.end(), oIt);

    return EXIT_SUCCESS;
  }
};

int main( int argc, char * argv[] )
{
  itk::wasm::Pipeline pipeline("encode", "Encode an ITK Image into a High Throughput JPEG2000 codestream", argc, argv);

  return itk::wasm::SupportInputImageTypes<PipelineFunctor,
    uint8_t,
    itk::Vector<uint8_t, 4>,
    int8_t,
    uint16_t,
    int16_t>::Dimensions<2U>("image", pipeline);
}
