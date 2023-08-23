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

// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

// MIT License

// Copyright, the respective contributors

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <exception>
#include <memory>
#include <limits.h>

#include <ojph_arch.h>
#include <ojph_file.h>
#include <ojph_mem.h>
#include <ojph_params.h>
#include <ojph_codestream.h>


#include "ojphFrameInfo.h"
#include "ojphPoint.h"
#include "ojphSize.h"

namespace OpenJPH {

/// <summary>
/// JavaScript API for decoding HTJ2K bistreams with OpenJPH
/// </summary>
class HTJ2KDecoder
{
public:
  /// <summary>
  /// Constructor for decoding a HTJ2K image from JavaScript.
  /// </summary>
  HTJ2KDecoder()
  {
  }

  /// <summary>
  /// Returns the buffer to store the encoded bytes.  This method is not exported
  /// to JavaScript, it is intended to be called by C++ code
  ///
  /// Ignored if the encoded buffer is set via setEncodedBuffer().
  /// </summary>
  std::vector<uint8_t> &getEncodedBytes()
  {
    return encoded_;
  }


  /// <summary>
  /// Set the buffer to that stores the encoded bytes. This method is not exported
  /// to JavaScript, it is intended to be called by C++ code
  /// </summary>
  void setEncodedBuffer(const uint8_t * data, size_t size)
  {
    mem_infile_data_ = data;
    mem_infile_size_ = size;
  }


  /// <summary>
  /// Returns the buffer to store the decoded bytes.  This method is not exported
  /// to JavaScript, it is intended to be called by C++ code
  /// </summary>
  const std::vector<uint8_t> &getDecodedBytes() const
  {
    return decoded_;
  }

  /// <summary>
  /// Reads the header from an encoded HTJ2K bitstream.  The caller must have
  /// copied the HTJ2K encoded bitstream into the encoded buffer before
  /// calling this method, see getEncodedBuffer() and getEncodedBytes() above.
  /// </summary>
  void readHeader()
  {
    ojph::codestream codestream;
    ojph::mem_infile mem_file;
    if (mem_infile_data_ != nullptr)
    {
      mem_file.open(mem_infile_data_, mem_infile_size_);
    } else
    {
      mem_file.open(encoded_.data(), encoded_.size());
    }
    readHeader_(codestream, mem_file);
  }

  /// <summary>
  /// Calculates the resolution for a given decomposition level based on the
  /// current values in FrameInfo (which is populated via readHeader() and
  /// decode()).  level = 0 = full res, level = _numDecompositions = lowest resolution
  /// </summary>
  Size calculateSizeAtDecompositionLevel(int decompositionLevel)
  {
    Size result(frameInfo_.width, frameInfo_.height);
    while (decompositionLevel > 0)
    {
      result.width = ojph_div_ceil(result.width, 2);
      result.height = ojph_div_ceil(result.height, 2);
      decompositionLevel--;
    }
    return result;
  }

  /// <summary>
  /// Decodes the encoded HTJ2K bitstream.  The caller must have copied the
  /// HTJ2K encoded bitstream into the encoded buffer before calling this
  /// method, see getEncodedBuffer() and getEncodedBytes() above.
  /// </summary>
  void decode()
  {
    ojph::codestream codestream;
    ojph::mem_infile mem_file;
    if (mem_infile_data_ != nullptr)
    {
      mem_file.open(mem_infile_data_, mem_infile_size_);
    } else
    {
      mem_file.open(encoded_.data(), encoded_.size());
    }
    readHeader_(codestream, mem_file);
    decode_(codestream, frameInfo_, 0);
  }

  /// <summary>
  /// Decodes the encoded HTJ2K bitstream to the requested decomposition level.
  /// The caller must have copied the HTJ2K encoded bitstream into the encoded
  /// buffer before calling this method, see getEncodedBuffer() and
  ///  getEncodedBytes() above.
  /// </summary>
  void decodeSubResolution(size_t decompositionLevel)
  {
    ojph::codestream codestream;
    ojph::mem_infile mem_file;
    if (mem_infile_data_ != nullptr)
    {
      mem_file.open(mem_infile_data_, mem_infile_size_);
    } else
    {
      mem_file.open(encoded_.data(), encoded_.size());
    }
    readHeader_(codestream, mem_file);
    decode_(codestream, frameInfo_, decompositionLevel);
  }

  /// <summary>
  /// returns the FrameInfo object for the decoded image.
  /// </summary>
  const FrameInfo &getFrameInfo() const
  {
    return frameInfo_;
  }

  /// <summary>
  /// returns the number of wavelet decompositions.
  /// </summary>
  const size_t getNumDecompositions() const
  {
    return numDecompositions_;
  }

  /// <summary>
  /// returns true if the image is lossless, false if lossy
  /// </summary>
  const bool getIsReversible() const
  {
    return isReversible_;
  }

  /// <summary>
  /// returns progression order.
  // 0 = LRCP
  // 1 = RLCP
  // 2 = RPCL
  // 3 = PCRL
  // 4 = CPRL
  /// </summary>
  const size_t getProgressionOrder() const
  {
    return progressionOrder_;
  }

  /// <summary>
  /// returns the down sampling used for component.
  /// </summary>
  Point getDownSample(size_t component) const
  {
    return downSamples_[component];
  }

  /// <summary>
  /// returns the image offset
  /// </summary>
  Point getImageOffset() const
  {
    return imageOffset_;
  }

  /// <summary>
  /// returns the tile size
  /// </summary>
  Size getTileSize() const
  {
    return tileSize_;
  }

  /// <summary>
  /// returns the tile offset
  /// </summary>
  Point getTileOffset() const
  {
    return tileOffset_;
  }

  /// <summary>
  /// returns the block dimensions
  /// </summary>
  Size getBlockDimensions() const
  {
    return blockDimensions_;
  }

  /// <summary>
  /// returns the precinct for the specified resolution decomposition level
  /// </summary>
  Size getPrecinct(size_t level) const
  {
    return precincts_[level];
  }

  /// <summary>
  /// returns the number of layers
  /// </summary>
  int32_t getNumLayers() const
  {
    return numLayers_;
  }

  /// <summary>
  /// returns whether or not a color transform is used
  /// </summary>
  bool getIsUsingColorTransform() const
  {
    return isUsingColorTransform_;
  }

private:
  void readHeader_(ojph::codestream &codestream, ojph::mem_infile &mem_file)
  {
    // NOTE - enabling resilience does not seem to have any effect at this point...
    codestream.enable_resilience();
    codestream.read_headers(&mem_file);
    ojph::param_siz siz = codestream.access_siz();
    frameInfo_.width = siz.get_image_extent().x - siz.get_image_offset().x;
    frameInfo_.height = siz.get_image_extent().y - siz.get_image_offset().y;
    frameInfo_.componentCount = siz.get_num_components();
    frameInfo_.bitsPerSample = siz.get_bit_depth(0);
    frameInfo_.isSigned = siz.is_signed(0);
    downSamples_.resize(frameInfo_.componentCount);
    for (size_t i = 0; i < frameInfo_.componentCount; i++)
    {
      downSamples_[i].x = siz.get_downsampling(i).x;
      downSamples_[i].y = siz.get_downsampling(i).y;
    }

    imageOffset_.x = siz.get_image_offset().x;
    imageOffset_.y = siz.get_image_offset().y;
    tileSize_.width = siz.get_tile_size().w;
    tileSize_.height = siz.get_tile_size().h;

    tileOffset_.x = siz.get_tile_offset().x;
    tileOffset_.y = siz.get_tile_offset().y;

    ojph::param_cod cod = codestream.access_cod();
    numDecompositions_ = cod.get_num_decompositions();
    isReversible_ = cod.is_reversible();
    progressionOrder_ = cod.get_progression_order();
    blockDimensions_.width = cod.get_block_dims().w;
    blockDimensions_.height = cod.get_block_dims().h;
    precincts_.resize(numDecompositions_);
    for (size_t i = 0; i < numDecompositions_; i++)
    {
      precincts_[i].width = cod.get_precinct_size(i).w;
      precincts_[i].height = cod.get_precinct_size(i).h;
    }
    numLayers_ = cod.get_num_layers();
    isUsingColorTransform_ = cod.is_using_color_transform();
  }

  void decode_(ojph::codestream &codestream, const FrameInfo &frameInfo, size_t decompositionLevel)
  {

    // calculate the resolution at the requested decomposition level and
    // allocate destination buffer
    Size sizeAtDecompositionLevel = calculateSizeAtDecompositionLevel(decompositionLevel);
    int resolutionLevel = numDecompositions_ - decompositionLevel;
    const size_t bytesPerPixel = (frameInfo_.bitsPerSample + 8 - 1) / 8;
    const size_t destinationSize = sizeAtDecompositionLevel.width * sizeAtDecompositionLevel.height * frameInfo.componentCount * bytesPerPixel;
    decoded_.resize(destinationSize);

    // set the level to read to and reconstruction level to the specified decompositionLevel
    codestream.restrict_input_resolution(decompositionLevel, decompositionLevel);

    // parse it
    if (frameInfo.componentCount == 1)
    {
      codestream.set_planar(true);
    }
    else
    {
      if (isUsingColorTransform_)
      {
        codestream.set_planar(false);
      }
      else
      {
        // for color images without a color transform,
        // calling set_planar(true) invokes an optimization
        // https://github.com/aous72/OpenJPH/issues/34
        codestream.set_planar(true);
      }
    }
    codestream.create();

    // Extract the data line by line...
    // NOTE: All values must be clamped https://github.com/aous72/OpenJPH/issues/35
    ojph::ui32 comp_num;
    for (int y = 0; y < sizeAtDecompositionLevel.height; y++)
    {
      size_t lineStart = y * sizeAtDecompositionLevel.width * frameInfo.componentCount * bytesPerPixel;
      if (frameInfo.componentCount == 1)
      {
        ojph::line_buf *line = codestream.pull(comp_num);
        if (frameInfo.bitsPerSample <= 8)
        {
          unsigned char *pOut = (unsigned char *)&decoded_[lineStart];
          for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
          {
            int val = line->i32[x];
            pOut[x] = std::max(0, std::min(val, UCHAR_MAX));
          }
        }
        else
        {
          if (frameInfo.isSigned)
          {
            short *pOut = (short *)&decoded_[lineStart];
            for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
            {
              int val = line->i32[x];
              pOut[x] = std::max(SHRT_MIN, std::min(val, SHRT_MAX));
            }
          }
          else
          {
            unsigned short *pOut = (unsigned short *)&decoded_[lineStart];
            for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
            {
              int val = line->i32[x];
              pOut[x] = std::max(0, std::min(val, USHRT_MAX));
            }
          }
        }
      }
      else
      {
        for (int c = 0; c < frameInfo.componentCount; c++)
        {
          ojph::line_buf *line = codestream.pull(comp_num);
          if (frameInfo.bitsPerSample <= 8)
          {
            uint8_t *pOut = &decoded_[lineStart] + c;
            for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
            {
              int val = line->i32[x];
              pOut[x * frameInfo.componentCount] = std::max(0, std::min(val, UCHAR_MAX));
            }
          }
          else
          {
            // This should work but has not been tested yet
            if (frameInfo.isSigned)
            {
              short *pOut = (short *)&decoded_[lineStart] + c;
              for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
              {
                int val = line->i32[x];
                pOut[x * frameInfo.componentCount] = std::max(SHRT_MIN, std::min(val, SHRT_MAX));
              }
            }
            else
            {
              unsigned short *pOut = (unsigned short *)&decoded_[lineStart] + c;
              for (size_t x = 0; x < sizeAtDecompositionLevel.width; x++)
              {
                int val = line->i32[x];
                pOut[x * frameInfo.componentCount] = std::max(0, std::min(val, USHRT_MAX));
              }
            }
          }
        }
      }
    }
  }

  std::vector<uint8_t> encoded_;
  std::vector<uint8_t> decoded_;
  FrameInfo frameInfo_;
  std::vector<Point> downSamples_;
  size_t numDecompositions_;
  bool isReversible_;
  size_t progressionOrder_;
  Point imageOffset_;
  Size tileSize_;
  Point tileOffset_;
  Size blockDimensions_;
  std::vector<Size> precincts_;
  int32_t numLayers_;
  bool isUsingColorTransform_;
  const uint8_t * mem_infile_data_{nullptr};
  size_t mem_infile_size_{0};
};

} // end namespace OpenJPH