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
                chrominance(i, j, 0) = 0.0f;
                chrominance(i, j, 1) = 0.0f;
                chrominance(i, j, 2) = 0.0f;
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
    Image output(im.width(), im.height(), im.channels());
    // Quantizes the image to 2^bits levels
    const int unit = 256 / pow(2, bits);
    for (int c = 0; c < im.channels(); c++)
    {
        for (int i = 0; i < im.width(); ++i)
        {
            for (int j = 0; j < im.height(); j++)
            {
                // Scale the input pixel.
                const float scaled = round(255 * im(i, j, c));
                const float quantized = floor(scaled / unit) / pow(2, bits);
                output(i, j, c) = quantized;
            }
        }
    }
    return output;
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
    vector<Image> images;
    images.push_back(gamma_code(quantize(im, bits), gamma));
    images.push_back(quantize(gamma_code(im, gamma), bits));
    return images;
}

// Return two images in a C++ vector
std::vector<Image> spanish(const Image &im)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Remember to create the output images and the output vector
    // Push the images onto the vector
    // Do all the required processing
    // Return the vector, color image first
    std::vector<Image> images;
    Image color(im.width(), im.height(), im.channels());
    Image bw(im.width(), im.height(), im.channels());

    /*
    The first image has a constant luminance (Y)
    and its chrominance is the opposite of the
    input’s chrominance (-U and -V).

    In the first image, set the luminance to be 0.5.
    */
    const Image yuv = rgb2yuv(im);
    for (int c = 0; c < im.channels(); c++)
    {
        for (int x = 0; x < im.width(); x++)
        {
            for (int y = 0; y < im.height(); y++)
            {
                color(x, y, c) = (c == 0) ? 0.5f : -yuv(x, y, c);
            }
        }
    }

    /*
    The second image is a black-and-white version of the original,
    i.e. both U and V should be uniformly zero.
    */
    for (int c = 0; c < im.channels(); c++)
    {
        for (int x = 0; x < im.width(); x++)
        {
            for (int y = 0; y < im.height(); y++)
            {
                bw(x, y, c) = (c == 0) ? yuv(x, y, c) : 0.0f;
            }
        }
    }

    /*
    If image has dimensions w × h, make sure that the black
    dot is at the 0-indexed location floor(w/2), floor(h/2).
    */
    float center_x, center_y;
    center_x = floor(im.width() / 2);
    center_y = floor(im.height() / 2);

    color.create_rectangle(center_x, center_y, center_x, center_y, 0.0f, 0.0f, 0.0f);
    bw.create_rectangle(center_x, center_y, center_x, center_y, 0.0f, 0.0f, 0.0f);

    images.push_back(yuv2rgb(color));
    images.push_back(yuv2rgb(bw));

    return images;
}

float channel_average(const Image &im, const int channel)
{
    float sum = 0;
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {
            sum += im(x, y, channel);
        }
    }
    return sum / (im.width() * im.height());
}

std::vector<float> get_wb_factors(const Image &im)
{
    std::vector<float> weights;
    const float red_average = channel_average(im, 0);
    const float green_average = channel_average(im, 1);
    const float blue_average = channel_average(im, 2);
    // weights[0] is factor for R channel.
    weights.push_back(green_average / red_average);
    // weights[1] is factor for B channel.
    weights.push_back(green_average / blue_average);
    // Uncomment to verify white balance factors.
    // cout << weights[0] * red_average << endl;
    // cout << green_average << endl;
    // cout << weights[1]*blue_average << endl;
    return weights;
}

// White balances an image using the gray world assumption
Image grayworld(const Image &im)
{
    // --------- HANDOUT  PS01 ------------------------------
    // Implement automatic white balance by multiplying each channel
    // of the input by a factor such that the three channel of the output
    // image have the same mean value. The mean value of the green channel
    // is taken as reference.
    Image output(im.width(), im.height(), im.channels());
    const std::vector<float> wb_weights = get_wb_factors(im);
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {
            output(x, y, 0) = im(x, y, 0) * wb_weights[0];
            output(x, y, 1) = im(x, y, 1);
            output(x, y, 2) = im(x, y, 2) * wb_weights[1];
        }
    }
    return output;
}
