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

#include <ojph_arch.h>
#include <ojph_file.h>
#include <ojph_mem.h>
#include <ojph_params.h>
#include <ojph_codestream.h>

#include "ojphEncodedBuffer.h"
#include "ojphFrameInfo.h"
#include "ojphPoint.h"
#include "ojphSize.h"

namespace OpenJPH {

/// <summary>
/// JavaScript API for encoding images to HTJ2K bitstreams with OpenJPH
/// </summary>
class HTJ2KEncoder
{
public:
  /// <summary>
  /// Constructor for encoding a HTJ2K image from JavaScript.
  /// </summary>
  HTJ2KEncoder() : decompositions_(5),
                   lossless_(true),
                   quantizationStep_(-1.0),
                   progressionOrder_(2), // RPCL
                   imageOffset_(0, 0),
                   tileOffset_(0, 0),
                   tileSize_(512, 512),
                   blockDimensions_(64, 64)
  {
  }

  /// <summary>
  /// Set the frame information.  This method is not
  /// exported to JavaScript, it is intended to be called by C++ code
  //
  //  If setDecodedBytes is called, then the returned value is ignored.
  /// </summary>
  void setFrameInfo(const FrameInfo & frameInfo)
  {
    frameInfo_ = frameInfo;
    downSamples_.resize(frameInfo_.componentCount);
    for (int c = 0; c < frameInfo_.componentCount; ++c)
    {
      downSamples_[c].x = 1;
      downSamples_[c].y = 1;
    }
  }

  /// <summary>
  /// Returns the buffer to store the decoded bytes.  This method is not
  /// exported to JavaScript, it is intended to be called by C++ code
  //
  //  If setDecodedBytes is called, then the returned value is ignored.
  /// </summary>
  std::vector<uint8_t> &getDecodedBytes()
  {
    return decoded_;
  }

  /// <summary>
  /// Set the buffer to store the decoded bytes.  This method is not
  /// exported to JavaScript, it is intended to be called by C++ code
  /// </summary>
  void setDecodedBytes(const uint8_t * decodedBytes)
  {
    decodedBytes_ = decodedBytes;
  }

  /// <summary>
  /// Returns the buffer to store the encoded bytes.  This method is not
  /// exported to JavaScript, it is intended to be called by C++ code
  /// </summary>
  const std::vector<uint8_t> &getEncodedBytes() const
  {
    return encoded_.getBuffer();
  }

  /// <summary>
  /// Sets the number of wavelet decompositions and clears any precincts
  /// </summary>
  void setDecompositions(size_t decompositions)
  {
    decompositions_ = decompositions;
    precincts_.resize(0);
  }

  /// <summary>
  /// Sets the quality level for the image.  If lossless is false then
  /// quantizationStep controls the lossy quantization applied.  quantizationStep
  /// is ignored if lossless is true
  /// </summary>
  void setQuality(bool lossless, float quantizationStep)
  {
    lossless_ = lossless;
    quantizationStep_ = quantizationStep;
  }

  /// <summary>
  /// Sets the progression order
  /// 0 = LRCP
  /// 1 = RLCP
  /// 2 = RPCL
  /// 3 = PCRL
  /// 4 = CPRL
  /// </summary>
  void setProgressionOrder(size_t progressionOrder)
  {
    progressionOrder_ = progressionOrder;
  }

  /// <summary>
  /// Sets the down sampling for component
  /// </summary>
  void setDownSample(size_t component, Point downSample)
  {
    if(downSamples_.size() <= component) {
      downSamples_.resize(component + 1);
    }
    downSamples_[component] = downSample;
  }

  /// <summary>
  /// Sets the image offset
  /// </summary>
  void setImageOffset(Point imageOffset)
  {
    imageOffset_ = imageOffset;
  }

  /// <summary>
  /// Sets the tile size
  /// </summary>
  void setTileSize(Size tileSize)
  {
    tileSize_ = tileSize;
  }

  /// <summary>
  /// Sets the tile offset
  /// </summary>
  void setTileOffset(Point tileOffset)
  {
    tileOffset_ = tileOffset;
  }

  /// <summary>
  /// Sets the block dimensions
  /// </summary>
  void setBlockDimensions(Size blockDimensions)
  {
    blockDimensions_ = blockDimensions;
  }

  /// <summary>
  /// Sets the number of precincts
  /// </summary>
  void setNumPrecincts(size_t numLevels)
  {
    precincts_.resize(numLevels);
  }

  /// <summary>
  /// Sets the precinct for the specified level.  You must
  /// call setNumPrecincts with the number of levels first
  /// </summary>
  void setPrecinct(size_t level, Size precinct)
  {
    precincts_[level] = precinct;
  }

  /// <summary>
  /// Sets whether to add TLM at beginning of file
  /// </summary>
  void setTLMMarker(bool set_tlm_marker)
  {
    request_tlm_marker_ = set_tlm_marker;
  }

  /// <summary>
  /// Sets whether to add SOT markers at beginning of resolutions
  /// </summary>
  void setTilePartDivisionsAtResolutions(bool set_tilepart_divisions_at_resolutions)
  {
    set_tilepart_divisions_at_resolutions_ = set_tilepart_divisions_at_resolutions;
  }

  /// <summary>
  /// Sets whether to add SOT markers at beginning of components
  /// </summary>
  void setTilePartDivisionsAtComponents(bool set_tilepart_divisions_at_components)
  {
    set_tilepart_divisions_at_components_ = set_tilepart_divisions_at_components;
  }

  /// <summary>
  /// Executes an HTJ2K encode using the data in the source buffer.  The
  /// JavaScript code must copy the source image frame into the source
  /// buffer before calling this method.  See documentation on getSourceBytes()
  /// above
  /// </summary>
  void encode()
  {
    encoded_.open();

    // Setup image size parameters
    ojph::codestream codestream;
    ojph::param_siz siz = codestream.access_siz();
    siz.set_image_extent(ojph::point(frameInfo_.width, frameInfo_.height));
    int num_comps = frameInfo_.componentCount;
    downSamples_.resize(num_comps);
    siz.set_num_components(num_comps);
    for (int c = 0; c < num_comps; ++c)
      siz.set_component(c, ojph::point(downSamples_[c].x, downSamples_[c].y), frameInfo_.bitsPerSample, frameInfo_.isSigned);
    siz.set_image_offset(ojph::point(imageOffset_.x, imageOffset_.y));
    siz.set_tile_size(ojph::size(tileSize_.width, tileSize_.height));
    siz.set_tile_offset(ojph::point(tileOffset_.x, tileOffset_.y));

    // Setup encoding parameters
    ojph::param_cod cod = codestream.access_cod();
    cod.set_num_decomposition(decompositions_);
    cod.set_block_dims(blockDimensions_.width, blockDimensions_.height);
    std::vector<ojph::size> precincts;
    precincts.resize(precincts_.size());
    for (size_t i = 0; i < precincts_.size(); i++)
    {
      precincts[i].w = precincts_[i].width;
      precincts[i].h = precincts_[i].height;
    }
    cod.set_precinct_size(precincts_.size(), precincts.data());

    const char *progOrders[] = {"LRCP", "RLCP", "RPCL", "PCRL", "CPRL"};
    cod.set_progression_order(progOrders[progressionOrder_]);
    cod.set_color_transform(frameInfo_.isUsingColorTransform);
    cod.set_reversible(lossless_);
    if (!lossless_)
    {
      codestream.access_qcd().set_irrev_quant(quantizationStep_);
    }
    codestream.set_tilepart_divisions(set_tilepart_divisions_at_resolutions_, set_tilepart_divisions_at_components_);
    codestream.request_tlm_marker(request_tlm_marker_);
    codestream.set_planar(frameInfo_.isUsingColorTransform == false);
    codestream.write_headers(&encoded_);

    // Encode the image
    const size_t bytesPerPixel = frameInfo_.bitsPerSample / 8;
    ojph::ui32 next_comp;
    ojph::line_buf *cur_line = codestream.exchange(NULL, next_comp);
    const uint8_t * decodedData = decodedBytes_ ? decodedBytes_ : decoded_.data();
    siz = codestream.access_siz();
    int height = siz.get_image_extent().y - siz.get_image_offset().y;
    for (size_t y = 0; y < height; y++)
    {
      for (size_t c = 0; c < siz.get_num_components(); c++)
      {
        int *dp = cur_line->i32;
        if (frameInfo_.bitsPerSample <= 8)
        {
          uint8_t *pIn = (uint8_t *)(decodedData + (y * frameInfo_.width * bytesPerPixel * siz.get_num_components()) + c);
          for (size_t x = 0; x < frameInfo_.width; x++)
          {
            *dp++ = *pIn;
            pIn += siz.get_num_components();
          }
        }
        else
        {
          if (frameInfo_.isSigned)
          {
            int16_t *pIn = (int16_t *)(decodedData + (y * frameInfo_.width * bytesPerPixel));
            for (size_t x = 0; x < frameInfo_.width; x++)
            {
              *dp++ = *pIn++;
            }
          }
          else
          {
            uint16_t *pIn = (uint16_t *)(decodedData + (y * frameInfo_.width * bytesPerPixel));
            for (size_t x = 0; x < frameInfo_.width; x++)
            {
              *dp++ = *pIn++;
            }
          }
        }
        cur_line = codestream.exchange(cur_line, next_comp);
      }
    }

    // cleanup
    codestream.flush();
    codestream.close();
  }

private:
  std::vector<uint8_t> decoded_;
  const uint8_t * decodedBytes_{ nullptr };
  EncodedBuffer encoded_;
  FrameInfo frameInfo_;
  size_t decompositions_ = 5;
  bool lossless_ = true;
  bool request_tlm_marker_ = false;
  bool set_tilepart_divisions_at_components_ = false;
  bool set_tilepart_divisions_at_resolutions_ = false;
  float quantizationStep_ = -1.0f;
  size_t progressionOrder_ = 2; // RPCL

  std::vector<Point> downSamples_;
  Point imageOffset_;
  Size tileSize_;
  Point tileOffset_;
  Size blockDimensions_ = Size(64,64);
  std::vector<Size> precincts_;
};

} // end namespace OpenJPH
