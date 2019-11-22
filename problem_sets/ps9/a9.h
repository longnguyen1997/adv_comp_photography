//  -------------------------------------------------------------------
//  File:    a9.h
//  Original:Michael Gharbi <gharbi@mit.edu>
//  Created: 2015-11-19
//
//  Current: Monde Duinkharjav <budmonde@mit.edu>
//  Updated: 2019-04-24
//  -------------------------------------------------------------------
//
//  General Convolution Gaussian and Unsharp Mask in Halide
//
//  -------------------------------------------------------------------

#ifndef _A9_H_
#define _A9_H_

#include <Halide.h>
#include <cmath>
#include <image_io.h>
#include <iostream>
#include <timing.h>
#include <vector>

using std::string;
using std::map;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

using Halide::Tools::Image;
using Halide::Func;
using Halide::Tools::save_image;
using Halide::Tools::load_image;

void apply_auto_schedule(Func F);

Func Gaussian_horizontal(Image<uint8_t> input, float sigma,
                         float truncate = 3.0f);
Func Gaussian(Image<uint8_t> input, float sigma, float truncate = 3.0f);
Func UnsharpMask(Image<uint8_t> input, int schedule_index,
                 vector<int> schedule_parameters, float sigma,
                 float truncate = 3.0f, float gamma = 2.2f,
                 float strength = 1.0f);

Image<uint8_t> Gaussian_horizontal_cpp(Image<uint8_t> input, float sigma,
                                       float truncate = 3.0f);
Image<uint8_t> Gaussian_cpp(Image<uint8_t> input, float sigma,
                            float truncate = 3.0f);
Image<uint8_t> UnsharpMask_cpp(Image<uint8_t> input, float sigma,
                               float truncate = 3.0f, float gamma = 2.2f,
                               float strength = 1.0f);

void autoschedule_unsharp_mask(const Image<uint8_t> &input);

#endif // _A9_H_
