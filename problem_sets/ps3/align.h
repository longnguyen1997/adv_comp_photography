/* --------------------------------------------------------------------------
 * File:    align.h
 * Created: 2015-10-01
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#ifndef __ALIGN__H
#define __ALIGN__H

#include "Image.h"
#include "basicImageManipulation.h"
#include <cmath>
#include <iostream>

using namespace std;

Image denoiseSeq(const vector<Image> &imgs);
Image logSNR(const vector<Image> &imSeq, float scale = 1.0 / 20.0);
vector<int> align(const Image &im1, const Image &im2, int maxOffset = 20);
Image alignAndDenoise(const vector<Image> &imSeq, int maxOffset = 20);
Image split(const Image &sergeyImg);
Image sergeyRGB(const Image &sergeyImg, int maxOffset = 20);
Image roll(const Image &im, int xRoll, int yRoll);

#endif
