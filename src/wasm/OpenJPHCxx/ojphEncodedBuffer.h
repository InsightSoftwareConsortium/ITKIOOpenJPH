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

#include <vector>

namespace OpenJPH {

/**
 * EncodedBuffer implements the ojph::outfile_base using a
 * std::vector<uint8_t>.  This allows OpenJPEG to write
 * directly to std::vector<> and avoid a memory copy that
 * would otherwise be needed when using ojph::mem_outfile.
 */
class EncodedBuffer : public ojph::outfile_base
{
public:
  /**  A constructor */
  OJPH_EXPORT
  EncodedBuffer() {}
  /**  A destructor */
  OJPH_EXPORT
  ~EncodedBuffer() {}

  /**  Call this function to open a memory file.
 *
   *  This function creates a memory buffer to be used for storing
   *  the generated j2k codestream.
   *
   *  @param initial_size is the initial memory buffer size.
   *         The default value is 2^16.
   */
  OJPH_EXPORT
  void open(size_t initial_size = 65536) {
      buffer_.resize(0);
      buffer_.reserve(initial_size);
  }

  /**  Call this function to write data to the memory file.
 *
   *  This function adds new data to the memory file.  The memory buffer
   *  of the file grows as needed.
   *
   *  @param ptr is the address of the new data.
   *  @param size the number of bytes in the new data.
   */
  OJPH_EXPORT
  virtual size_t write(const void *ptr, size_t size) {
      auto bytes = reinterpret_cast<uint8_t const*>(ptr);
      buffer_.insert(buffer_.end(), bytes, bytes + size);
      return size;
  }

  /** Call this function to know the file size (i.e., number of bytes used
   *  to store the file).
   *
   *  @return the file size.
   */
  OJPH_EXPORT
  virtual ojph::si64 tell() { return buffer_.size(); }

  /** Call this function to close the file and deallocate memory
 *
   *  The object can be used again after calling close
   */
  OJPH_EXPORT
  virtual void close() {}

  /** Call this function to access memory file data.
 *
   *  It is not recommended to store the returned value because buffer
   *  storage address can change between write calls.
   *
   *  @return a constant pointer to the data.
   */
  OJPH_EXPORT
  const ojph::ui8* get_data() { return buffer_.data(); }

  /** Call this function to access memory file data (for const objects)
 *
   *  This is similar to the above function, except that it can be used
   *  with constant objects.
   *
   *  @return a constant pointer to the data.
   */
  OJPH_EXPORT
  const ojph::ui8* get_data() const { return buffer_.data(); }
  
  /**
   * Returns the underlying buffer
   */
  OJPH_EXPORT
  const std::vector<uint8_t>& getBuffer() const {return buffer_;}

private:
  std::vector<uint8_t> buffer_;
};

} // namespace OpenJPH