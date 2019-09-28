/* --------------------------------------------------------------------------
 * File:    demosaic.cpp
 * Created: 2015-10-01
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "demosaic.h"
#include <cmath>

using namespace std;

Image basicGreen(const Image &raw, int offset)
{
    // --------- HANDOUT  PS03 ------------------------------
    // Takes as input a raw image and returns a single-channel
    // 2D image corresponding to the green channel using simple interpolation
    const int w = raw.width();
    const int h = raw.height();
    Image green(w, h, 1);
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            if (x == 0 || y == 0 || x == w - 1 || y == h - 1
                    || (x + (y % 2)) % 2 == offset)
            {
                green(x, y) = raw(x, y);
            }
            else
            {
                const float average = (raw(x, y - 1) +
                                       raw(x, y - 1) +
                                       raw(x - 1, y) +
                                       raw(x + 1, y)) / 4.0;
                green(x, y) = average;
            }
        }
    }
    return green;
}

Image basicRorB(const Image &raw, int offsetX, int offsetY)
{
    // --------- HANDOUT  PS03 ------------------------------
    //  Takes as input a raw image and returns a single-channel
    //  2D image corresponding to the red or blue channel using simple
    //  interpolation
    const int w = raw.width();
    const int h = raw.height();
    Image channel(w, h, 1);
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            if (x == 0 || y == 0 || x == w - 1 || y == h - 1)
            {
                channel(x, y) = raw(x, y);
            }
            else
            {
                if (x % 2 != offsetX && y % 2 != offsetY)
                {
                    // Average diagonal neighbors.
                    channel(x, y) = (raw(x - 1, y - 1) +
                                     raw(x + 1, y - 1) +
                                     raw(x - 1, y + 1) +
                                     raw(x + 1, y + 1)) / 4.0;
                }
                else if (x % 2 == offsetX && y % 2 != offsetY)
                {
                    // Average the two available values.
                    channel(x, y) = (raw(x, y - 1) + raw(x, y + 1)) / 2;
                }
                else if (x % 2 != offsetX && y % 2 == offsetY)
                {
                    // Average the two available values.
                    channel(x, y) = (raw(x - 1, y) + raw(x + 1, y)) / 2;
                }
                else
                {
                    // Match.
                    channel(x, y) = raw(x, y);
                }
            }
        }
    }
    return channel;
}

Image basicDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                    int offsetRedY, int offsetBlueX, int offsetBlueY)
{
    // --------- HANDOUT  PS03 ------------------------------
    // takes as input a raw image and returns an rgb image
    // using simple interpolation to demosaic each of the channels
    vector<Image> rgb;
    rgb.push_back(basicRorB(raw, offsetRedX, offsetRedY));
    rgb.push_back(basicGreen(raw, offsetGreen));
    rgb.push_back(basicRorB(raw, offsetBlueX, offsetBlueY));
    Image demosaiced(raw.width(), raw.width(), 3);
    for (int x = 0; x < demosaiced.width(); ++x)
    {
        for (int y = 0; y < demosaiced.height(); ++y)
        {
            for (int c = 0; c < 3; ++c)
            {
                demosaiced(x, y, c) = rgb[c](x, y);
            }
        }
    }
    return demosaiced;
}

Image edgeBasedGreen(const Image &raw, int offset)
{
    // --------- HANDOUT  PS03 ------------------------------
    // Takes a raw image and outputs a single-channel
    // image corresponding to the green channel taking into account edges
    const int w = raw.width();
    const int h = raw.height();
    Image green(w, h, 1);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (x == 0 || y == 0 || x == w - 1 || y == h - 1
                    || (x + y % 2) % 2 == offset)
            {
                green(x, y) = raw(x, y);
            }
            else
            {
                float leftRight = abs(raw(x - 1, y) - raw(x + 1, y));
                float upDown = abs(raw(x, y - 1) - raw(x, y + 1));
                float result = 0;
                // Interpolate along axis of least difference.
                if (leftRight < upDown)
                {
                    result = (raw(x - 1, y) + raw(x + 1, y)) / 2.0;
                }
                else
                {
                    result = (raw(x, y - 1) + raw(x, y + 1)) / 2.0;
                }
                green(x, y) = result;
            }
        }
    }
    return green;
}

Image edgeBasedGreenDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                             int offsetRedY, int offsetBlueX, int offsetBlueY)
{
    // --------- HANDOUT  PS03 ------------------------------
    // Takes as input a raw image and returns an rgb image
    // using edge-based green demosaicing for the green channel and
    // simple interpolation to demosaic the red and blue channels
    Image demosaiced(raw.width(), raw.height(), 3);
    const vector<Image> rgb
    {
        basicRorB(raw, offsetRedX, offsetRedY),
        edgeBasedGreen(raw, offsetGreen),
        basicRorB(raw, offsetBlueX, offsetBlueY)
    };
    for (int x = 0; x < raw.width(); ++x)
    {
        for (int y = 0; y < raw.height(); ++y)
        {
            demosaiced(x, y, 0) = rgb[0](x, y);
            demosaiced(x, y, 1) = rgb[1](x, y);
            demosaiced(x, y, 2) = rgb[2](x, y);
        }
    }
    return demosaiced;
}

float sub(const Image &raw, Image &green, int x, int y)
{
    return raw(x, y) - green(x, y);
}

Image greenBasedRorB(const Image &raw, Image &green, int offsetX, int offsetY)
{
    // --------- HANDOUT  PS03 ------------------------------
    // Takes as input a raw image and returns a single-channel
    // 2D image corresponding to the red or blue channel using green based
    // interpolation
    const int w = raw.width();
    const int h = raw.height();
    Image channel(w, h, 1);
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            if (x == 0 || y == 0 || x == w - 1 || y == h - 1)
            {
                channel(x, y) = raw(x, y);
            }
            else
            {
                if (x % 2 != offsetX && y % 2 != offsetY)
                {
                    // Average diagonal neighbors.
                    channel(x, y) = (sub(raw, green, x - 1, y - 1) +
                                     sub(raw, green, x + 1, y - 1) +
                                     sub(raw, green, x - 1, y + 1) +
                                     sub(raw, green, x + 1, y + 1)) / 4.0 + green(x, y);
                }
                else if (x % 2 == offsetX && y % 2 != offsetY)
                {
                    // Average the two available values.
                    channel(x, y) = (sub(raw, green, x, y - 1) + sub(raw, green, x, y + 1)) / 2 + green(x, y);
                }
                else if (x % 2 != offsetX && y % 2 == offsetY)
                {
                    // Average the two available values.
                    channel(x, y) = (sub(raw, green, x - 1, y) + sub(raw, green, x + 1, y)) / 2 + green(x, y);
                }
                else
                {
                    // Match.
                    channel(x, y) = raw(x, y);
                }
            }
        }
    }
    return channel;
}

Image improvedDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                       int offsetRedY, int offsetBlueX, int offsetBlueY)
{
    // // --------- HANDOUT  PS03 ------------------------------
    // Takes as input a raw image and returns an rgb image
    // using edge-based green demosaicing for the green channel and
    // simple green based demosaicing of the red and blue channels
    Image demosaiced(raw.width(), raw.height(), 3);
    Image green = edgeBasedGreen(raw, offsetGreen);
    Image red = greenBasedRorB(raw, green, offsetRedX, offsetRedY);
    Image blue = greenBasedRorB(raw, green, offsetBlueX, offsetBlueY);
    for (int x = 0; x < raw.width(); ++x)
    {
        for (int y = 0; y < raw.height(); ++y)
        {
            demosaiced(x, y, 0) = red(x, y);
            demosaiced(x, y, 1) = green(x, y);
            demosaiced(x, y, 2) = blue(x, y);
        }
    }
    return demosaiced;
}
