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
#include "itkInputBinaryStream.h"
#include "itkOutputImage.h"
#include "itkImage.h"
#include "itkVector.h"
#include "itkVectorImage.h"

#include "ojphInformation.h"
#include "ojphHTJ2KDecoder.h"

static auto openjphDecoder = std::make_unique<OpenJPH::HTJ2KDecoder>();

template<typename TImage>
class PipelineFunctor
{
public:
  int operator()(itk::wasm::Pipeline & pipeline)
  {
    using ImageType = TImage;

    int decompositionLevel = 0;
    pipeline.add_option("-d,--decomposition-level", decompositionLevel, "Decomposition level to decode. 0 is the highest resolution.");

    bool informationOnly = false;
    pipeline.add_flag("-i,--information-only", informationOnly, "Only read metadata for the image but do not decode the pixel data.");

    using OutputImageType = itk::wasm::OutputImage<ImageType>;
    OutputImageType outputImage;
    pipeline.add_option("image", outputImage, "Output image")->type_name("OUTPUT_IMAGE")->required();

    ITK_WASM_PARSE(pipeline);

    typename ImageType::Pointer image = ImageType::New();

    const auto size = openjphDecoder->calculateSizeAtDecompositionLevel(decompositionLevel);

    image->SetRegions({size.width, size.height});
    image->Allocate();

    const auto imageOffset = openjphDecoder->getImageOffset();
    typename ImageType::PointType origin;
    origin[0] = static_cast<double>( imageOffset.x );
    origin[1] = static_cast<double>( imageOffset.y );
    image->SetOrigin(origin);

    itk::MetaDataDictionary & thisDic = image->GetMetaDataDictionary();
    // Number of wavelet decompositions
    itk::EncapsulateMetaData<int>(thisDic, "NumberOfDecompositions", openjphDecoder->getNumDecompositions());
    itk::EncapsulateMetaData<bool>(thisDic, "IsReversible", openjphDecoder->getIsReversible());
    itk::EncapsulateMetaData<int>(thisDic, "ProgressionOrder", openjphDecoder->getProgressionOrder());

    std::vector<double> tileSize(2);
    tileSize[0] = openjphDecoder->getTileSize().width;
    tileSize[1] = openjphDecoder->getTileSize().height;
    itk::EncapsulateMetaData<decltype(tileSize)>(thisDic, "TileSize" , tileSize);

    std::vector<double> tileOffset(2);
    tileOffset[0] = openjphDecoder->getTileOffset().x;
    tileOffset[1] = openjphDecoder->getTileOffset().y;
    itk::EncapsulateMetaData<decltype(tileOffset)>(thisDic, "TileOffset" , tileOffset);

    std::vector<double> blockDimensions(2);
    blockDimensions[0] = openjphDecoder->getBlockDimensions().width;
    blockDimensions[1] = openjphDecoder->getBlockDimensions().height;
    itk::EncapsulateMetaData<decltype(blockDimensions)>(thisDic, "BlockDimensions" , blockDimensions);

    const int numberOfLayers = openjphDecoder->getNumLayers();
    itk::EncapsulateMetaData<decltype(numberOfLayers)>(thisDic, "NumberOfLayers" , numberOfLayers);

    const bool isUsingColorTransform = openjphDecoder->getIsUsingColorTransform();
    itk::EncapsulateMetaData<decltype(isUsingColorTransform)>(thisDic, "UseColorTransform" , isUsingColorTransform);

    if (!informationOnly)
    {
      ITK_WASM_CATCH_EXCEPTION(pipeline, openjphDecoder->decodeSubResolution(decompositionLevel));

      const std::vector<uint8_t> & decodedBytes = openjphDecoder->getDecodedBytes();
      auto buffer = image->GetBufferPointer();
      std::memcpy(buffer, decodedBytes.data(), decodedBytes.size());
    }

    outputImage.Set(image);

    return EXIT_SUCCESS;
  }
};

int main( int argc, char * argv[] )
{
  itk::wasm::Pipeline pipeline("decode", "Decode a High Throughput JPEG2000 codestream and generate an ITK Image", argc, argv);

  itk::wasm::InputBinaryStream inputCodestream;
  auto codestreamOption = pipeline.add_option("codestream", inputCodestream, "Input HTJ2K codestream")->type_name("INPUT_BINARY_STREAM");

  ITK_WASM_PRE_PARSE(pipeline);

  codestreamOption->required();

  constexpr unsigned int Dimension = 2;
  OpenJPH::FrameInfo frameInfo{ 256, 256, 8, 1, false };

  // not -h, --help
  if (pipeline.get_argc() > 2)
  {
    inputCodestream.Get().seekg(0, std::ios::end);
    const std::streampos streamSize = inputCodestream.Get().tellg();
    inputCodestream.Get().seekg(0, std::ios::beg);

    inputCodestream.Get().unsetf(std::ios::skipws);
    std::vector<uint8_t> & encodedBytes = openjphDecoder->getEncodedBytes();
    encodedBytes.reserve(streamSize);
    encodedBytes.insert(encodedBytes.begin(),
                        std::istream_iterator<uint8_t>(inputCodestream.Get()),
                        std::istream_iterator<uint8_t>());

    ITK_WASM_CATCH_EXCEPTION(pipeline, openjphDecoder->readHeader());

    const auto readFrameInfo = openjphDecoder->getFrameInfo();
    frameInfo.width = readFrameInfo.width;
    frameInfo.height = readFrameInfo.height;
    frameInfo.bitsPerSample = readFrameInfo.bitsPerSample;
    frameInfo.componentCount = readFrameInfo.componentCount;
    frameInfo.isSigned = readFrameInfo.isSigned;
  }

  if (frameInfo.bitsPerSample <= 8)
  {
    if (frameInfo.isSigned)
    {
      if (frameInfo.componentCount == 1)
      {
        using ImageType = itk::Image<int8_t, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
      else
      {
        using ImageType = itk::VectorImage<int8_t, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
    }
    else
    {
      if (frameInfo.componentCount == 1)
      {
        using ImageType = itk::Image<uint8_t, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
      else if(frameInfo.componentCount == 4)
      {
        using ImageType = itk::Image<itk::Vector<uint8_t, 4>, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
      else
      {
        using ImageType = itk::VectorImage<uint8_t, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
    }
  }
  else if (frameInfo.bitsPerSample <= 16)
  {
    if (frameInfo.isSigned)
    {
      if (frameInfo.componentCount == 1)
      {
        using ImageType = itk::Image<int16_t, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
      else
      {
        using ImageType = itk::VectorImage<int16_t, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
    }
    else
    {
      if (frameInfo.componentCount == 1)
      {
        using ImageType = itk::Image<uint16_t, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
      else
      {
        using ImageType = itk::VectorImage<uint16_t, Dimension>;
        using PipelineType = PipelineFunctor<ImageType>;
        return PipelineType()(pipeline);
      }
    }
  }

  return EXIT_SUCCESS;
}
