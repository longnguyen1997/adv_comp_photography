/* --------------------------------------------------------------------------
 * File:    align.cpp
 * Created: 2015-10-01
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "align.h"
#include <numeric>

using namespace std;

Image denoiseSeq(const vector<Image> &imSeq)
{
    // // --------- HANDOUT  PS03 ------------------------------
    // Basic denoising by computing the average of a sequence of images
    if (imSeq.size() == 0) return Image(1, 1, 1);
    const int N = imSeq.size();
    Image output(imSeq[0].width(), imSeq[0].height(), imSeq[0].channels());
    for (int x = 0; x < output.width(); ++x)
    {
        for (int y = 0; y < output.height(); ++y)
        {
            for (int c = 0; c < output.channels(); ++c)
            {
                for (int n = 0; n < N; ++n) output(x, y, c) += imSeq[n](x, y, c);
            }
        }
    }
    return output / N;
}

Image logSNR(const vector<Image> &imSeq, float scale)
{
    // // --------- HANDOUT  PS03 ------------------------------
    // returns an image visualizing the per-pixel and
    // per-channel log of the signal-to-noise ratio scaled by scale.

    const int W = imSeq[0].width();
    const int H = imSeq[0].height();
    const int C = imSeq[0].channels();
    const int N = imSeq.size();
    const Image sumOfImages = accumulate(imSeq.begin(), imSeq.end(), Image(W, H, C));
    const Image muNIm = sumOfImages / N;
    Image SNR(W, H, C);
    for (int i = 0; i < W; ++i)
    {
        for (int j = 0; j < H; ++j)
        {
            for (int c = 0; c < C; ++c)
            {
                float sigmaI = 0;
                float expected = 0;
                float expectedSquared = 0;
                for (int n = 0; n < N; ++n)
                {
                    expected += imSeq[n](i, j, c);
                    expectedSquared += pow(imSeq[n](i, j, c), 2);
                }
                expected /= N;
                expectedSquared /= N;
                sigmaI = expectedSquared - pow(expected, 2);
                SNR(i, j, c) = scale * 10 * log10(expectedSquared / (sigmaI + 0.000001f));
            }
        }
    }
    return SNR;
}

float squaredDiff(const Image &i1, const Image &i2, const int maxOffset)
{
    const int w = i1.width();
    const int h = i1.height();
    const int c = i1.channels();
    float diff = 0;
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            // Ignore edge pixels within maxOffset distance.
            if (x < maxOffset || x >= w - maxOffset
                    ||
                    y < maxOffset || y >= h - maxOffset)
            {
                continue;
            }
            for (int z = 0; z < c; ++z)
            {
                diff += pow(i1(x, y, z) - i2(x, y, z), 2);
            }
        }
    }
    return diff;
}

vector<int> align(const Image &im1, const Image &im2, int maxOffset)
{
    // // --------- HANDOUT  PS03 ------------------------------
    // returns the (x,y) offset that best aligns im2 to match im1.
    vector<int> translation(2, 0);
    float minDiff = squaredDiff(im1, im2, maxOffset);
    for (int xRoll = -maxOffset; xRoll <= maxOffset; ++xRoll)
    {
        for (int yRoll = -maxOffset; yRoll <= maxOffset; ++yRoll)
        {
            // Try translating (x, y).
            const float diff = squaredDiff(
                                   im1, roll(im2, xRoll, yRoll), maxOffset
                               );
            if (diff < minDiff)
            {
                minDiff = diff;
                translation[0] = xRoll;
                translation[1] = yRoll;
            }
        }
    }
    return translation;
}

Image alignAndDenoise(const vector<Image> &imSeq, int maxOffset)
{
    // // --------- HANDOUT  PS03 ------------------------------
    // Registers all images to the first one in a sequence and outputs
    // a denoised image even when the input sequence is not perfectly aligned.
    vector<Image> translatedImages;
    translatedImages.push_back(imSeq[0]);
    for (int i = 1; i < (int)imSeq.size(); ++i)
    {
        const vector<int> translation = align(imSeq[0], imSeq[i], maxOffset);
        translatedImages.push_back(roll(imSeq[i], translation[0], translation[1]));
    }
    return denoiseSeq(translatedImages);
}

Image split(const Image &sergeyImg)
{
    // --------- HANDOUT  PS03 ------------------------------
    // 6.865 only:
    // split a Sergey images to turn it into one 3-channel image.
    const int w = sergeyImg.width();
    const int h = floor((float)sergeyImg.height() / 3.0);
    Image rgb(w, h, 3);
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            rgb(x, y, 0) = sergeyImg(x, y + 2 * h);
            rgb(x, y, 1) = sergeyImg(x, y + h);
            rgb(x, y, 2) = sergeyImg(x, y);
        }
    }
    return rgb;
}

Image sergeyRGB(const Image &sergeyImg, int maxOffset)
{
    // // --------- HANDOUT  PS03 ------------------------------
    // 6.865 only:
    // aligns the green and blue channels of your rgb channel of a sergey
    // image to the red channel. This should return an aligned RGB image
    Image splitSergey = split(sergeyImg);
    const int w = splitSergey.width();
    const int h = splitSergey.height();
    Image r(w, h);
    Image g(w, h);
    Image b(w, h);
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            r(x, y) = splitSergey(x, y, 0);
            g(x, y) = splitSergey(x, y, 1);
            b(x, y) = splitSergey(x, y, 2);
        }
    }
    const vector<int> translateGreen = align(r, g, maxOffset);
    const vector<int> translateBlue = align(r, b, maxOffset);
    g = roll(g, translateGreen[0], translateGreen[1]);
    b = roll(b, translateBlue[0], translateBlue[1]);
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            splitSergey(x, y, 0) = r(x, y);
            splitSergey(x, y, 1) = g(x, y);
            splitSergey(x, y, 2) = b(x, y);
        }
    }
    return splitSergey;
}

/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// This circularly shifts an image by xRoll in the x direction and
// yRoll in the y direction. xRoll and yRoll can be negative or postive
Image roll(const Image &im, int xRoll, int yRoll)
{

    int xNew, yNew;
    Image imRoll(im.width(), im.height(), im.channels());

    // for each pixel in the original image find where its corresponding
    // location is in the rolled image
    for (int x = 0; x < im.width(); x++)
    {
        for (int y = 0; y < im.height(); y++)
        {

            // use modulo to figure out where the new location is in the
            // rolled image. Then take care of when this returns a negative
            // number
            xNew = (x + xRoll) % im.width();
            yNew = (y + yRoll) % im.height();
            xNew = (xNew < 0) * (imRoll.width() + xNew) + (xNew >= 0) * xNew;
            yNew = (yNew < 0) * (imRoll.height() + yNew) + (yNew >= 0) * yNew;

            // assign the rgb values for each pixel in the original image to
            // the location in the new image
            for (int z = 0; z < im.channels(); z++)
            {
                imRoll(xNew, yNew, z) = im(x, y, z);
            }
        }
    }

    // return the rolled image
    return imRoll;
}
