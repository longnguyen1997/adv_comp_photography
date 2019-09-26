/* --------------------------------------------------------------------------
 * File:    align.cpp
 * Created: 2015-10-01
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "align.h"

using namespace std;

Image denoiseSeq(const vector<Image> &imSeq) {
  // // --------- HANDOUT  PS03 ------------------------------
  // Basic denoising by computing the average of a sequence of images
  return Image(1, 1, 1);
}

Image logSNR(const vector<Image> &imSeq, float scale) {
  // // --------- HANDOUT  PS03 ------------------------------
  // returns an image visualizing the per-pixel and
  // per-channel log of the signal-to-noise ratio scaled by scale.
  return Image(1, 1, 1);
}

vector<int> align(const Image &im1, const Image &im2, int maxOffset) {
  // // --------- HANDOUT  PS03 ------------------------------
  // returns the (x,y) offset that best aligns im2 to match im1.
  return vector<int>();
}

Image alignAndDenoise(const vector<Image> &imSeq, int maxOffset) {
  // // --------- HANDOUT  PS03 ------------------------------
  // Registers all images to the first one in a sequence and outputs
  // a denoised image even when the input sequence is not perfectly aligned.
  return Image(1, 1, 1);
}

Image split(const Image &sergeyImg) {
  // --------- HANDOUT  PS03 ------------------------------
  // 6.865 only:
  // split a Sergey images to turn it into one 3-channel image.
  return Image(1, 1, 1);
}

Image sergeyRGB(const Image &sergeyImg, int maxOffset) {
  // // --------- HANDOUT  PS03 ------------------------------
  // 6.865 only:
  // aligns the green and blue channels of your rgb channel of a sergey
  // image to the red channel. This should return an aligned RGB image
  return Image(1, 1, 1);
}

/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// This circularly shifts an image by xRoll in the x direction and
// yRoll in the y direction. xRoll and yRoll can be negative or postive
Image roll(const Image &im, int xRoll, int yRoll) {

  int xNew, yNew;
  Image imRoll(im.width(), im.height(), im.channels());

  // for each pixel in the original image find where its corresponding
  // location is in the rolled image
  for (int x = 0; x < im.width(); x++) {
    for (int y = 0; y < im.height(); y++) {

      // use modulo to figure out where the new location is in the
      // rolled image. Then take care of when this returns a negative
      // number
      xNew = (x + xRoll) % im.width();
      yNew = (y + yRoll) % im.height();
      xNew = (xNew < 0) * (imRoll.width() + xNew) + (xNew >= 0) * xNew;
      yNew = (yNew < 0) * (imRoll.height() + yNew) + (yNew >= 0) * yNew;

      // assign the rgb values for each pixel in the original image to
      // the location in the new image
      for (int z = 0; z < im.channels(); z++) {
        imRoll(xNew, yNew, z) = im(x, y, z);
      }
    }
  }

  // return the rolled image
  return imRoll;
}
