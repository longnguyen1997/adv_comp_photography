// hdr.cpp
// Assignment 5

#include "hdr.h"
#include "filtering.h"
#include <algorithm>
#include <math.h>
#include <cassert>

using namespace std;

/**************************************************************
 //                       HDR MERGING                        //
 *************************************************************/

Image computeWeight(const Image &im, float epsilonMini, float epsilonMaxi)
{
    // --------- HANDOUT  PS04 ------------------------------
    // Generate a weight image that indicates which pixels are good to use in
    // HDR, i.e. weight=1 when the pixel value is in [epsilonMini, epsilonMaxi].
    // The weight is per pixel, per channel.
    Image out(im.width(), im.height(), im.channels());
    for (int c = 0; c < out.channels(); ++c)
    {
        for (int x = 0; x < out.width(); ++x)
        {
            for (int y = 0; y < out.height(); ++y)
            {
                out(x, y, c) = (im(x, y, c) >= epsilonMini && im(x, y, c) <= epsilonMaxi) ? 1.0f : 0.0f;
            }
        }
    }
    return out;
}

float computeFactor(const Image &im1, const Image &w1, const Image &im2,
                    const Image &w2)
{
    // --------- HANDOUT  PS04 ------------------------------
    // Compute the multiplication factor between a pair of images. This
    // gives us the relative exposure between im1 and im2. It is computed as
    // the median of im2/(im1+eps) for some small eps, taking into account
    // pixels that are valid in both images.
    Image epsilonIm1 = im1 + 0.0000000001;
    Image divide = im2 / epsilonIm1;
    vector<float> values;
    for (int c = 0; c < im1.channels(); ++c)
    {
        for (int x = 0; x < im1.width(); ++x)
        {
            for (int y = 0; y < im1.height(); ++y)
            {
                if (w1(x, y, c) == 1.0f && w2(x, y, c) == 1.0f)
                {
                    values.push_back(divide(x, y, c));
                }
            }
        }
    }
    sort(values.begin(), values.end());
    if(values.size() % 2 == 0)
        return (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
    else
        return values[values.size() / 2];
}

Image makeHDR(vector<Image> &imSeq, float epsilonMini, float epsilonMaxi)
{
    // --------- HANDOUT  PS04 ------------------------------
    // Merge images to make a single hdr image
    // For each image in the sequence, compute the weight map (special cases
    // for the first and last images).
    // Compute the exposure factor for each consecutive pair of image.
    // Write the valid pixel to your hdr output, taking care of rescaling them
    // properly using the factor.

    /*
    Some pixels might be underexposed or
    overexposed in all images
    Simple solution: don’t eliminate dark pixels in
    the brightest image or bright pixels in the
    darkest one.
    */
    vector<Image> weights;
    for (int i = 0; i < (int)imSeq.size(); ++i)
    {
        if (i == 0)
        {
            weights.push_back(computeWeight(imSeq[i], 0.0, epsilonMaxi));
        }
        else if (i == (int)imSeq.size() - 1)
        {
            weights.push_back(computeWeight(imSeq[i], epsilonMini, 1.0));
        }
        else
        {
            weights.push_back(computeWeight(imSeq[i], epsilonMini, epsilonMaxi));
        }
    }
    vector<float> factors{1.0f};
    for (int i = 1; i < (int)imSeq.size(); ++i)
    {
        factors.push_back(computeFactor(imSeq[i - 1], weights[i - 1], imSeq[i], weights[i]) * factors[i - 1]);
    }
    assert(factors.size() == weights.size());

    Image out(imSeq[0].width(), imSeq[0].height(), imSeq[0].channels());
    for (int c = 0; c < out.channels(); ++c)
    {
        for (int x = 0; x < out.width(); ++x)
        {
            for (int y = 0; y < out.height(); ++y)
            {
                float weightSum = 0.0f;
                float innerSum = 0.0f;
                for (int i = 0; i < (int)imSeq.size(); ++i)
                {
                    weightSum += weights[i](x, y, c);
                    innerSum += (weights[i](x, y, c) * imSeq[i](x, y, c) / factors[i]);
                }
                out(x, y, c) = innerSum / (weightSum == 0 ? 1 : weightSum);
            }
        }
    }
    return out;
}

/**************************************************************
 //                      TONE MAPPING                        //
 *************************************************************/

Image toneMap(const Image &im, float targetBase, float detailAmp, bool useBila,
              float sigmaRange)
{
    // --------- HANDOUT  PS04 ------------------------------
    // tone map an hdr image
    // - Split the image into its luminance-chrominance components.
    // - Work in the log10 domain for the luminance
    // -
    vector<Image> lumiAndChromi = lumiChromi(im);
    Image log10Luminance = log10Image(lumiAndChromi[0]);
}

/*********************************************************************
 *                       Tone mapping helpers                        *
 *********************************************************************/

// image --> log10Image
Image log10Image(const Image &im)
{
    // --------- HANDOUT  PS04 ------------------------------
    // Taking a linear image im, transform to log10 scale.
    // To avoid infinity issues, make any 0-valued pixel be equal the the
    // minimum non-zero value. See image_minnonzero(im).
    float minimum = image_minnonzero(im);
    Image out(im.width(), im.height(), im.channels());
    for (int i = 0; i < im.number_of_elements(); ++i)
    {
        out(i) = im(i) == 0 ? minimum : log10(im(i));
    }
    return out;
}

// Image --> 10^Image
Image exp10Image(const Image &im)
{
    // --------- HANDOUT  PS04 ------------------------------
    // take an image in log10 domain and transform it back to linear domain.
    // see pow(a, b)
    return im;
}

// min non-zero pixel value of image
float image_minnonzero(const Image &im)
{
    // --------- HANDOUT  PS04 ------------------------------
    // return the smallest value in the image that is non-zeros (across all
    // channels too)
    float minimum = 1.01f;
    for (int i = 0; i < im.number_of_elements(); ++i)
    {
        if (im(i) != 0.0f) minimum = min(minimum, im(i));
    }
    return minimum;
}
