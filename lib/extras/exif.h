// Copyright (c) the JPEG XL Project Authors.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef LIB_EXTRAS_EXIF_H_
#define LIB_EXTRAS_EXIF_H_

#include <stdint.h>

#include <vector>

namespace jxl {

// Sets the Exif orientation to the identity, to avoid repeated orientation
void ResetExifOrientation(std::vector<uint8_t>& exif);

}  // namespace jxl

#endif  // LIB_EXTRAS_EXIF_H_
