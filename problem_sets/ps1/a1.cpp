/* -----------------------------------------------------------------
 * File:    a1.cpp
 * Created: 2015-09-15
 * Updated: 2019-08-10
 * -----------------------------------------------------------------
 *
 * Assignment 01
 *
 * ---------------------------------------------------------------*/

#include "a1.h"
#include <cmath>
using namespace std;

// Create a surprise image
Image create_special()
{
    // // --------- HANDOUT  PS01 ------------------------------
    // create the image outlined in the handout (MIT logo)
    Image img(290, 150, 3);
    img.set_color(1.0, 1.0, 1.0);
    img.create_rectangle(0, 0, 31, 149, 0.64, 0.12, 0.2);
    img.create_rectangle(52, 0, 83, 102, 0.64, 0.12, 0.2);
    img.create_rectangle(103, 0, 134, 149, 0.64, 0.12, 0.2);
    img.create_rectangle(155, 0, 186, 30, 0.64, 0.12, 0.2);
    img.create_rectangle(155, 48, 186, 149, 0.55, 0.55, 0.55);
    img.create_rectangle(207, 0, 289, 30, 0.64, 0.12, 0.2);
    img.create_rectangle(207, 48, 238, 149, 0.64, 0.12, 0.2);
    return img;
}

// Change the brightness of the image
// const Image & means a reference to im will get passed to the function,
// but the compiler won't let you modify it within the function.
// So you will return a new image
Image brightness(const Image &im, float factor)
{
    // --------- HANDOUT  PS01 ------------------------------
    Image output(im.width(), im.height(), im.channels());
    // Modify image brightness
    const int num_chans = output.channels();
    for (int c = 0; c < num_chans; c++)
    {
        for (int x = 0; x < output.width(); x++)
        {
            for (int y = 0; y < output.height(); y++)
            {
                output(x, y, c) = min(im(x, y, c) * factor, 1.0f);
            }
        }
    }
    return output;
}

Image contrast(const Image &im, float factor, float midpoint)
{
    // --------- HANDOUT  PS01 ------------------------------
    Image output(im.width(), im.height(), im.channels());
    // Modify image contrast
    const int num_chans = output.channels();
    for (int c = 0; c < num_chans; c++)
    {
        for (int x = 0; x < output.width(); x++)
        {
            for (int y = 0; y < output.height(); y++)
            {
                output(x, y, c) = min(1.0f,
                                      factor * (im(x, y, c) - midpoint) + midpoint);
            }
        }
    }
    return output;
}

Image color2gray(const Image &im, const std::vector<float> &weights)
{
    // --------- HANDOUT  PS01 ------------------------------
    Image output(im.width(), im.height(), 1);
    // Convert to grayscale
    for (int x = 0; x < im.width(); x++)
    {
        for(int y = 0; y < im.height(); y++)
        {
            // Sanity check for grayscale inputs.
            if (im.channels() == 1)
            {
                output(x, y, 0) = im(x, y, 0) * weights[0];
            }
            else
            {
                output(x, y, 0) = (im(x, y, 0) * weights[0] +
                                   im(x, y, 1) * weights[1] +
                                   im(x, y, 2) * weights[2]);
            }
        }
    }
    return output;
}

// For this function, we want two outputs, a single channel luminance image
// and a three channel chrominance image. Return them in a vector with
// luminance first
std::vector<Image> lumiChromi(const Image &im)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Create the luminance image
    // Create the chrominance image
    // Create the output vector as (luminance, chrominance)
    vector<Image> images;
    const Image luminance = color2gray(im);

    Image chrominance(im.width(), im.height(), im.channels());
    for (int i = 0; i < im.width(); ++i)
    {
        for (int j = 0; j < im.height(); ++j)
        {
            if (luminance(i, j) == 0)
            {
                chrominance(i, j, 0) = 1.0f;
                chrominance(i, j, 1) = 1.0f;
                chrominance(i, j, 2) = 1.0f;
            }
            else
            {
                chrominance(i, j, 0) = im(i, j, 0) / luminance(i, j);
                chrominance(i, j, 1) = im(i, j, 1) / luminance(i, j);
                chrominance(i, j, 2) = im(i, j, 2) / luminance(i, j);
            }
        }
    }
    images.push_back(luminance);
    images.push_back(chrominance);
    return images;
}

// Modify brightness then contrast
Image brightnessContrastLumi(const Image &im, float brightF, float contrastF,
                             float midpoint)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Modify brightness, then contrast of luminance image
    const vector<Image> lumi_chromi = lumiChromi(im);
    const Image lumi_adjusted = contrast(brightness(lumi_chromi[0], brightF),
                                         contrastF, midpoint);
    Image output(im.width(), im.height(), im.channels());
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {
            output(x, y, 0) = lumi_adjusted(x, y) * lumi_chromi[1](x, y, 0);
            output(x, y, 1) = lumi_adjusted(x, y) * lumi_chromi[1](x, y, 1);
            output(x, y, 2) = lumi_adjusted(x, y) * lumi_chromi[1](x, y, 2);
        }
    }
    return output;
}

Image rgb2yuv(const Image &im)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Create output image of appropriate size
    // Change colorspace
    Image output(im.width(), im.height(), im.channels());
    for (int i = 0; i < im.width(); ++i)
    {
        for (int j = 0; j < im.height(); ++j)
        {
            const float r = im(i, j, 0);
            const float g = im(i, j, 1);
            const float b = im(i, j, 2);
            output(i, j, 0) = 0.299f * r + 0.587f * g + 0.114f * b;
            output(i, j, 1) = (-0.147f) * r + (-0.289f) * g + 0.436f * b;
            output(i, j, 2) = 0.615f * r + (-0.515f) * g + (-0.100f) * b;
        }
    }
    return output;
}

Image yuv2rgb(const Image &im)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Create output image of appropriate size
    // Change colorspace
    Image output(im.width(), im.height(), im.channels());
    for (int i = 0; i < im.width(); ++i)
    {
        for (int j = 0; j < im.height(); ++j)
        {
            const float y = im(i, j, 0);
            const float u = im(i, j, 1);
            const float v = im(i, j, 2);
            output(i, j, 0) = 1.0f * y + 0.0f * u + 1.14f * v;
            output(i, j, 1) = 1.0f * y + (-0.395f) * u + (-0.581f) * v;
            output(i, j, 2) = 1.0f * y + 2.032f * u + 0.0f * v;
        }
    }
    return output;
}

Image saturate(const Image &im, float factor)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Create output image of appropriate size
    // Saturate image
    Image yuv = rgb2yuv(im);
    for (int i = 0; i < im.width(); ++i)
    {
        for (int j = 0; j < im.height(); ++j)
        {
            yuv(i, j, 1) *= factor;
            yuv(i, j, 2) *= factor;
        }
    }
    return yuv2rgb(yuv);
}

// Gamma codes the image
Image gamma_code(const Image &im, float gamma)
{
    // // --------- HANDOUT  PS01 ------------------------------
    Image output(im.width(), im.height(), im.channels());
    // Gamma encodes the image
    const vector<Image> lumi_chromi = lumiChromi(im);
    Image luminance = lumi_chromi[0];
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {
            luminance(x, y, 0) = pow(luminance(x, y, 0), 1 / gamma);
        }
    }
    // Multiply back to get the gamma-encoded image.
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {
            output(x, y, 0) = luminance(x, y) * lumi_chromi[1](x, y, 0);
            output(x, y, 1) = luminance(x, y) * lumi_chromi[1](x, y, 1);
            output(x, y, 2) = luminance(x, y) * lumi_chromi[1](x, y, 2);
        }
    }
    return output;
}

// Quantizes the image to 2^bits levels and scales back to 0~1
Image quantize(const Image &im, int bits)
{
    // // --------- HANDOUT  PS01 ------------------------------
    // Image output(im.width(), im.height(), im.channels());
    // Quantizes the image to 2^bits levels
    // return output;
    return Image(1, 1, 1); // Change this
}

// Compare between first quantize then gamma_encode and first gamma_encode
// then quantize
std::vector<Image> gamma_test(const Image &im, int bits, float gamma)
{
    // // --------- HANDOUT  PS01 ------------------------------
    // // im1 = quantize then gamma_encode the image
    // // im2 = gamma_encode then quantize the image
    // // Remember to create the output images and the output vector
    // // Push the images onto the vector
    // // Do all the required processing
    // // Return the vector, color image first
    return std::vector<Image>(); // Change this
}

// Return two images in a C++ vector
std::vector<Image> spanish(const Image &im)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Remember to create the output images and the output vector
    // Push the images onto the vector
    // Do all the required processing
    // Return the vector, color image first
    return std::vector<Image>(); // Change this
}

// White balances an image using the gray world assumption
Image grayworld(const Image &im)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Implement automatic white balance by multiplying each channel
    // of the input by a factor such that the three channel of the output
    // image have the same mean value. The mean value of the green channel
    // is taken as reference.
    return Image(1, 1, 1); // Change this
}
