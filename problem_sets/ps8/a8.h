#ifndef _A8_H_
#define _A8_H_

#include <Halide.h>
#include <cmath>
#include <image_io.h>
#include <iostream>

using std::string;
using std::map;
using std::cout;
using std::cerr;
using std::endl;

using Halide::Tools::Image;
using Halide::Func;
using Halide::Tools::save_image;
using Halide::Tools::load_image;

void apply_compute_root(Func F);

Image<uint8_t> SmoothGradNormalized(void);
Image<uint8_t> WavyRGB(void);
Image<uint8_t> Luminance(Image<uint8_t> input);
Image<uint8_t> Sobel(Image<uint8_t> input);

Image<uint8_t> boxSchedule1(Image<uint8_t> input);
Image<uint8_t> boxSchedule5(Image<uint8_t> input);
Image<uint8_t> boxSchedule6(Image<uint8_t> input);
Image<uint8_t> boxSchedule7(Image<uint8_t> input);

Func Gaussian(Image<uint8_t> input);
Image<uint8_t> Gaussian_cpp(Image<uint8_t> input);
#endif // _A8_H_
