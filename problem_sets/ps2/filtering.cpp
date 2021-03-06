/* -----------------------------------------------------------------
 * File:    filtering.cpp
 * Created: 2015-09-22
 * -----------------------------------------------------------------
 *
 * Image convolution and filtering
 *
 * ---------------------------------------------------------------*/

#include "filtering.h"
#include <cassert>
#include <cmath>


using namespace std;


/* BEGIN EXTRA CREDIT */

Image edgeDetection(const Image &im, const bool clamp)
{
    Filter edgeDetKernel = Filter(
                               vector<float>
    {
        -1.0f, -1.0f, -1.0f,
            -1.0f, 8.0f, -1.0f,
            -1.0f, -1.0f, -1.0f
        }, 3, 3);
    return edgeDetKernel.convolve(im, false);
}


/* END EXTRA CREDIT */


Image boxBlur(const Image &im, int k, bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Convolve an image with a box filter of size k by k
    // It is safe to asssume k is odd.
    Image blurred(im.width(), im.height(), im.channels());
    for (int x = 0; x < im.width(); ++x)
    {
        for (int y = 0; y < im.height(); ++y)
        {
            for (int z = 0; z < im.channels(); ++z)
            {
                float pixel = 0.0f;
                for (int i = -k / 2; i <= k / 2; ++i)
                {
                    for (int j = -k / 2; j <= k / 2; ++j)
                    {
                        pixel += im.smartAccessor(x + i, y + j, z, clamp);
                    }
                }
                blurred(x, y, z) = pixel / (k * k);
            }
        }
    }
    return blurred;
}

Image Filter::convolve(const Image &im, bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Write a convolution function for the filter class

    /*
    Set output image to zero
    • For y, x in output image
    - for y’, x’ in kernel
    - out[y, x]+=input[y+y’, x+x’]*kernel[y’, x’]
    • (this assumes the kernel coordinates are centered)
    */

    Image convolved(im.width(), im.height(), im.channels());
    for (int x = 0; x < im.width(); ++x)
    {
        for (int y = 0; y < im.height(); ++y)
        {
            for (int z = 0; z < im.channels(); ++z)
            {
                float pixel_val = 0.0f;
                for (int i = 0; i < width; ++i)
                {
                    for (int j = 0; j < height; ++j)
                    {
                        pixel_val += (
                                         im.smartAccessor(
                                             x + i - width / 2,
                                             y + j - height / 2,
                                             z,
                                             clamp
                                         ) * operator()(width - i - 1, height - j - 1)
                                     );
                    }
                }
                convolved(x, y, z) = pixel_val;
            }
        }
    }
    return convolved;
}

Image boxBlur_filterClass(const Image &im, int k, bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Reimplement the box filter using the filter class.
    // check that your results match those in the previous function "boxBlur"
    vector<float> kernel;
    kernel.assign(k * k, 1.0 / (k * k));
    return Filter(kernel, k, k).convolve(im, clamp);
}

Image gradientMagnitude(const Image &im, bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Uses a Sobel kernel to compute the horizontal and vertical components
    // of the gradient of an image and returns the gradient magnitude.
    Image gradientMagnitude(im.width(), im.height(), im.channels());
    vector<float> sobelKernelX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
    vector<float> sobelKernelY{-1, -2, -1, 0, 0, 0, 1, 2, 1};
    const Image sobelXIm = Filter(sobelKernelX, 3, 3).convolve(im, clamp);
    const Image sobelYIm = Filter(sobelKernelY, 3, 3).convolve(im, clamp);
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {
            for (int c = 0; c < im.channels(); c++)
            {
                gradientMagnitude(x, y, c) = sqrt(
                                                 pow(sobelXIm(x, y, c), 2) +
                                                 pow(sobelYIm(x, y, c), 2)
                                             );
            }
        }
    }
    return gradientMagnitude;
}

float gauss(const float x, const float sigma)
{
    return exp(-pow(x, 2) / (2 * pow(sigma, 2)));
}

vector<float> gauss1DFilterValues(float sigma, float truncate)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Create a vector containing the normalized values in a 1D Gaussian filter
    // Truncate the gaussian at truncate*sigma.
    vector<float> weights(1 + 2 * ceil(sigma * truncate), 0.0f);
    float sum = 0.0f;
    const int midpoint = (1 + 2 * ceil(sigma * truncate)) / 2;
    weights[midpoint] = gauss(0, sigma);
    sum += gauss(0, sigma);
    for (int i = 1; i <= ceil(sigma * truncate); i++)
    {
        const float v = gauss(i, sigma);
        weights[midpoint + i] = v;
        weights[midpoint - i] = v;
        sum += (2 * v);
    }
    float sumNormalized = 0.0f;
    for (int i = 0; i < 1 + 2 * ceil(sigma * truncate); ++i)
    {
        weights[i] /= sum;
        sumNormalized += weights[i];
    }
    assert(abs(sumNormalized - 1.0f < 0.0005f));
    return weights;
}

Image gaussianBlur_horizontal(const Image &im, float sigma, float truncate, bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Gaussian blur across the rows of an image
    const vector<float> K = gauss1DFilterValues(sigma, truncate);
    return Filter(K, 1 + 2 * ceil(sigma * truncate), 1).convolve(im, clamp);
}

float gauss2D(const int x, const int y, const float sigma)
{
    return exp(-(pow(x, 2) + pow(y, 2)) / (2 * pow(sigma, 2)));
}

vector<float> gauss2DFilterValues(float sigma, float truncate)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Create a vector containing the normalized values in a 2D Gaussian
    // filter. Truncate the gaussian at truncate*sigma.
    const int k = 1 + 2 * ceil(sigma * truncate);
    const int numElements = k * k;
    float sum = 0.0f;
    vector<float> weights;
    for (int i = -k / 2; i <= k / 2; ++i)
    {
        for (int j = -k / 2; j <= k / 2; ++j)
        {
            const float g = gauss2D(i, j, sigma);
            weights.push_back(g);
            sum += g;
        }
    }
    float sumNormalized = 0.0f;
    for (int i = 0; i < weights.size(); i++)
    {
        weights[i] /= sum;
        sumNormalized += weights[i];
    }
    assert(abs(sumNormalized - 1.0f < 0.0005f));
    assert(weights.size() == numElements);
    return weights;
}

Image 1gaussianBlur_2D(const Image &im, float sigma, float truncate,
                      bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Blur an image with a full 2D rotationally symmetric Gaussian kernel
    const int k = 1 + 2 * ceil(sigma * truncate);
    return Filter(gauss2DFilterValues(sigma, truncate), k, k).convolve(im, clamp);
}

Image gaussianBlur_separable(const Image &im, float sigma, float truncate,
                             bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Use principles of seperabiltity to blur an image using 2 1D Gaussian
    // Filters
    const vector<float> gaussianWeights = gauss1DFilterValues(sigma, truncate);
    const int k = 1 + 2 * ceil(sigma * truncate);
    Filter gaussianX(gaussianWeights, k, 1);
    Filter gaussianY(gaussianWeights, 1, k);
    return gaussianY.convolve(
               gaussianX.convolve(im, clamp), clamp
           );
}

Image unsharpMask(const Image &im, float sigma, float truncate, float strength,
                  bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Sharpen an image
    Image lowpassIm = gaussianBlur_separable(im, sigma, truncate, clamp);
    Image highpassIm = im - lowpassIm;
    return im + strength * highpassIm;
}

Image bilateral(const Image &im, float sigmaRange, float sigmaDomain,
                float truncateDomain, bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // Denoise an image using the bilateral filter
    Image bilatFiltered = Image(im.width(), im.height(), im.channels());
    const vector<float> gXgY = gauss2DFilterValues(sigmaDomain, truncateDomain);
    const int k = sqrt(gXgY.size());
    const int m = k / 2;
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {
            for (int c = 0; c < im.channels(); c++)
            {
                float innerSum = 0;
                float Z = 0;
                for (int j = 0; j < k; j++)
                {
                    for (int i = 0; i < k; i++)
                    {
                        const int xPrime = x + i - m;
                        const int yPrime = y + j - m;
                        float sumOfColors = 0;
                        // Sum over all color channels.
                        for (int d = 0; d < im.channels(); d++)
                        {
                            sumOfColors += pow(
                                               im.smartAccessor(x, y, d, clamp) -
                                               im.smartAccessor(xPrime, yPrime, d, clamp),
                                               2);
                        }
                        // The three terms to multiply in the bilateral summation.
                        // i + j * k gives the (x - x', y - y').
                        float gDomain = gXgY.at(i + j * k);
                        float rangeGaussian = gauss(sqrt(sumOfColors), sigmaRange);
                        float iXY = im.smartAccessor(xPrime, yPrime, c, clamp);
                        innerSum += gDomain * rangeGaussian * iXY;
                        Z += gDomain * rangeGaussian;
                    }
                }
                bilatFiltered(x, y, c) = innerSum / Z;
            }
        }
    }
    return bilatFiltered;
}

Image bilaYUV(const Image &im, float sigmaRange, float sigmaY, float sigmaUV,
              float truncateDomain, bool clamp)
{
    // --------- HANDOUT  PS02 ------------------------------
    // 6.865 only
    // Bilaterial Filter an image seperatly for
    // the Y and UV components of an image
    Image bilatFiltered = Image(im.width(), im.height(), im.channels());
    Image yuv = rgb2yuv(im);
    const vector<float> YgXgY = gauss2DFilterValues(sigmaY, truncateDomain);
    const vector<float> UVgXgY = gauss2DFilterValues(sigmaUV, truncateDomain);

    const int kY = sqrt(YgXgY.size());
    const int kUV = sqrt(UVgXgY.size());
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {
            for (int c = 0; c < im.channels(); c++)
            {
                const int k = (c == 0) ? kY : kUV;
                const int m = k / 2;
                float innerSum = 0;
                float Z = 0;
                for (int j = 0; j < k; j++)
                {
                    for (int i = 0; i < k; i++)
                    {
                        const int xPrime = x + i - m;
                        const int yPrime = y + j - m;
                        float sumOfColors = 0;
                        // Sum over all color channels.
                        for (int d = 0; d < im.channels(); d++)
                        {
                            sumOfColors += pow(
                                               yuv.smartAccessor(x, y, d, clamp) -
                                               yuv.smartAccessor(xPrime, yPrime, d, clamp),
                                               2);
                        }
                        // The three terms to multiply in the bilateral summation.
                        // i + j * k gives the (x - x', y - y').
                        float gDomain = (c == 0) ? YgXgY.at(i + j * k) : UVgXgY.at(i + j * k);
                        float rangeGaussian = gauss(sqrt(sumOfColors), sigmaRange);
                        float iXY = yuv.smartAccessor(xPrime, yPrime, c, clamp);
                        innerSum += gDomain * rangeGaussian * iXY;
                        Z += gDomain * rangeGaussian;
                    }
                }
                bilatFiltered(x, y, c) = innerSum / Z;
            }
        }
    }
    return yuv2rgb(bilatFiltered);
}

/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// Create an image of 0's with a value of 1 in the middle. This function
// can be used to test that you have properly set the kernel values in your
// Filter object. Make sure to set k to be larger than the size of your kernel
Image impulseImg(int k)
{
    // initlize a kxkx1 image of all 0's
    Image impulse(k, k, 1);

    // set the center pixel to have intensity 1
    int center = floor(k / 2);
    impulse(center, center, 0) = 1.0f;

    return impulse;
}

// ------------- FILTER CLASS -----------------------
Filter::Filter(const vector<float> &fData, int fWidth, int fHeight)
    : kernel(fData), width(fWidth), height(fHeight)
{
    assert(fWidth * fHeight == (int)fData.size());
}

Filter::Filter(int fWidth, int fHeight)
    : kernel(std::vector<float>(fWidth * fHeight, 0)), width(fWidth),
      height(fHeight) {}

Filter::~Filter() {}

const float &Filter::operator()(int x, int y) const
{
    if (x < 0 || x >= width)
        throw OutOfBoundsException();
    if (y < 0 || y >= height)
        throw OutOfBoundsException();

    return kernel[x + y * width];
}

float &Filter::operator()(int x, int y)
{
    if (x < 0 || x >= width)
        throw OutOfBoundsException();
    if (y < 0 || y >= height)
        throw OutOfBoundsException();

    return kernel[x + y * width];
}
// --------- END FILTER CLASS -----------------------
