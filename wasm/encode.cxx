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
#include "itkVectorImage.h"
#include "itkSupportInputImageTypes.h"

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

    bool isReversible = false;
    pipeline.add_flag("-r,--reversible", isReversible, "Use reversible, i.e. lossless, encoding");

    float quantizationStep = 1.0f;
    pipeline.add_option("-q,--quantization-step", quantizationStep, "Quantization step size for lossy compression. Ignored if isReversible is true.")->check(CLI::Range(0.0f, 1.0f));

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
    // int decompositionLevel = 0;
    // pipeline.add_option("-d,--decomposition-level", decompositionLevel, "Decomposition level to decode. 0 is the highest resolution.");

    // bool informationOnly = false;
    // pipeline.add_flag("-i,--information-only", informationOnly, "Only read metadata for the image but do not decode the pixel data.");

    // using OutputImageType = itk::wasm::OutputImage<ImageType>;
    // OutputImageType outputImage;
    // pipeline.add_option("image", outputImage, "Output image")->type_name("OUTPUT_IMAGE")->required();

    ITK_WASM_PARSE(pipeline);

    // typename ImageType::Pointer image = ImageType::New();

    // const auto size = openjphDecoder->calculateSizeAtDecompositionLevel(decompositionLevel);

    // image->SetRegions({size.width, size.height});
    // image->Allocate();

    // const auto imageOffset = openjphDecoder->getImageOffset();
    // typename ImageType::PointType origin;
    // origin[0] = static_cast<double>( imageOffset.x );
    // origin[1] = static_cast<double>( imageOffset.y );
    // image->SetOrigin(origin);

    // itk::MetaDataDictionary & thisDic = image->GetMetaDataDictionary();
    // // Number of wavelet decompositions
    // itk::EncapsulateMetaData<int>(thisDic, "NumberOfDecompositions", openjphDecoder->getNumDecompositions());
    // itk::EncapsulateMetaData<bool>(thisDic, "IsReversible", openjphDecoder->getIsReversible());
    // itk::EncapsulateMetaData<int>(thisDic, "ProgressionOrder", openjphDecoder->getProgressionOrder());

    // std::vector<double> tileSize(2);
    // tileSize[0] = openjphDecoder->getTileSize().width;
    // tileSize[1] = openjphDecoder->getTileSize().height;
    // itk::EncapsulateMetaData<decltype(tileSize)>(thisDic, "TileSize" , tileSize);

    // std::vector<double> tileOffset(2);
    // tileOffset[0] = openjphDecoder->getTileOffset().x;
    // tileOffset[1] = openjphDecoder->getTileOffset().y;
    // itk::EncapsulateMetaData<decltype(tileOffset)>(thisDic, "TileOffset" , tileOffset);

    // std::vector<double> blockDimensions(2);
    // blockDimensions[0] = openjphDecoder->getBlockDimensions().width;
    // blockDimensions[1] = openjphDecoder->getBlockDimensions().height;
    // itk::EncapsulateMetaData<decltype(blockDimensions)>(thisDic, "BlockDimensions" , blockDimensions);

    // const int numberOfLayers = openjphDecoder->getNumLayers();
    // itk::EncapsulateMetaData<decltype(numberOfLayers)>(thisDic, "NumberOfLayers" , numberOfLayers);

    // const bool isUsingColorTransform = openjphDecoder->getIsUsingColorTransform();
    // itk::EncapsulateMetaData<decltype(isUsingColorTransform)>(thisDic, "UseColorTransform" , isUsingColorTransform);

    // if (!informationOnly)
    // {
    //   ITK_WASM_CATCH_EXCEPTION(pipeline, openjphDecoder->decodeSubResolution(decompositionLevel));

    //   const std::vector<uint8_t> & decodedBytes = openjphDecoder->getDecodedBytes();
    //   auto buffer = image->GetBufferPointer();
    //   std::memcpy(buffer, decodedBytes.data(), decodedBytes.size());
    // }

    // outputImage.Set(image);

    return EXIT_SUCCESS;
  }
};

int main( int argc, char * argv[] )
{
  itk::wasm::Pipeline pipeline("encode", "Encode an ITK Image into a High Throughput JPEG2000 codestream", argc, argv);

  return itk::wasm::SupportInputImageTypes<PipelineFunctor,
    uint8_t,
    int8_t,
    uint16_t,
    int16_t>::Dimensions<2U>("image", pipeline);

  // itk::wasm::InputBinaryStream inputCodestream;
  // auto codestreamOption = pipeline.add_option("codestream", inputCodestream, "Input HTJ2K codestream")->type_name("INPUT_BINARY_STREAM");

  // ITK_WASM_PRE_PARSE(pipeline);

  // codestreamOption->required();

  // constexpr unsigned int Dimension = 2;
  // OpenJPH::FrameInfo frameInfo{ 256, 256, 8, 1, false };

  // // not -h, --help
  // if (pipeline.get_argc() > 2)
  // {
  //   inputCodestream.Get().seekg(0, std::ios::end);
  //   const std::streampos streamSize = inputCodestream.Get().tellg();
  //   inputCodestream.Get().seekg(0, std::ios::beg);

  //   inputCodestream.Get().unsetf(std::ios::skipws);
  //   std::vector<uint8_t> & encodedBytes = openjphDecoder->getEncodedBytes();
  //   encodedBytes.reserve(streamSize);
  //   encodedBytes.insert(encodedBytes.begin(),
  //                       std::istream_iterator<uint8_t>(inputCodestream.Get()),
  //                       std::istream_iterator<uint8_t>());

  //   ITK_WASM_CATCH_EXCEPTION(pipeline, openjphDecoder->readHeader());

  //   const auto readFrameInfo = openjphDecoder->getFrameInfo();
  //   frameInfo.width = readFrameInfo.width;
  //   frameInfo.height = readFrameInfo.height;
  //   frameInfo.bitsPerSample = readFrameInfo.bitsPerSample;
  //   frameInfo.componentCount = readFrameInfo.componentCount;
  //   frameInfo.isSigned = readFrameInfo.isSigned;
  // }

  // if (frameInfo.bitsPerSample <= 8)
  // {
  //   if (frameInfo.isSigned)
  //   {
  //     if (frameInfo.componentCount == 1)
  //     {
  //       using ImageType = itk::Image<int8_t, Dimension>;
  //       using PipelineType = PipelineFunctor<ImageType>;
  //       return PipelineType()(pipeline);
  //     }
  //     else
  //     {
  //       using ImageType = itk::VectorImage<int8_t, Dimension>;
  //       using PipelineType = PipelineFunctor<ImageType>;
  //       return PipelineType()(pipeline);
  //     }
  //   }
  //   else
  //   {
  //     if (frameInfo.componentCount == 1)
  //     {
  //       using ImageType = itk::Image<uint8_t, Dimension>;
  //       using PipelineType = PipelineFunctor<ImageType>;
  //       return PipelineType()(pipeline);
  //     }
  //     else
  //     {
  //       using ImageType = itk::VectorImage<uint8_t, Dimension>;
  //       using PipelineType = PipelineFunctor<ImageType>;
  //       return PipelineType()(pipeline);
  //     }
  //   }
  // }
  // else if (frameInfo.bitsPerSample <= 16)
  // {
  //   if (frameInfo.isSigned)
  //   {
  //     if (frameInfo.componentCount == 1)
  //     {
  //       using ImageType = itk::Image<int16_t, Dimension>;
  //       using PipelineType = PipelineFunctor<ImageType>;
  //       return PipelineType()(pipeline);
  //     }
  //     else
  //     {
  //       using ImageType = itk::VectorImage<int16_t, Dimension>;
  //       using PipelineType = PipelineFunctor<ImageType>;
  //       return PipelineType()(pipeline);
  //     }
  //   }
  //   else
  //   {
  //     if (frameInfo.componentCount == 1)
  //     {
  //       using ImageType = itk::Image<uint16_t, Dimension>;
  //       using PipelineType = PipelineFunctor<ImageType>;
  //       return PipelineType()(pipeline);
  //     }
  //     else
  //     {
  //       using ImageType = itk::VectorImage<uint16_t, Dimension>;
  //       using PipelineType = PipelineFunctor<ImageType>;
  //       return PipelineType()(pipeline);
  //     }
  //   }
  // }

  // return EXIT_SUCCESS;
}
