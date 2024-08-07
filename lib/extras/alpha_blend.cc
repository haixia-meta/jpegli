// Copyright (c) the JPEG XL Project Authors.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "lib/extras/alpha_blend.h"

#include "lib/extras/packed_image.h"

namespace jxl {
namespace extras {

namespace {

void AlphaBlend(PackedFrame* frame, const float background[3]) {
  if (!frame) return;
  const PackedImage& im = frame->color;
  JxlPixelFormat format = im.format;
  if (format.num_channels != 2 && format.num_channels != 4) {
    return;
  }
  --format.num_channels;
  JXL_ASSIGN_OR_DIE(PackedImage blended,
                    PackedImage::Create(im.xsize, im.ysize, format));
  // TODO(szabadka) SIMDify this and make it work for float16.
  for (size_t y = 0; y < im.ysize; ++y) {
    for (size_t x = 0; x < im.xsize; ++x) {
      if (format.num_channels == 2) {
        float g = im.GetPixelValue(y, x, 0);
        float a = im.GetPixelValue(y, x, 1);
        float out = g * a + background[0] * (1 - a);
        blended.SetPixelValue(y, x, 0, out);
      } else {
        float r = im.GetPixelValue(y, x, 0);
        float g = im.GetPixelValue(y, x, 1);
        float b = im.GetPixelValue(y, x, 2);
        float a = im.GetPixelValue(y, x, 3);
        float out_r = r * a + background[0] * (1 - a);
        float out_g = g * a + background[1] * (1 - a);
        float out_b = b * a + background[2] * (1 - a);
        blended.SetPixelValue(y, x, 0, out_r);
        blended.SetPixelValue(y, x, 1, out_g);
        blended.SetPixelValue(y, x, 2, out_b);
      }
    }
  }
  frame->color = blended.Copy();
}

}  // namespace

void AlphaBlend(PackedPixelFile* ppf, const float background[3]) {
  if (!ppf || ppf->info.alpha_bits == 0) {
    return;
  }
  ppf->info.alpha_bits = 0;
  AlphaBlend(ppf->preview_frame.get(), background);
  for (auto& frame : ppf->frames) {
    AlphaBlend(&frame, background);
  }
}

}  // namespace extras
}  // namespace jxl
