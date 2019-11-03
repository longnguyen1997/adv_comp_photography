#include "blending.h"
#include "matrix.h"
#include <ctime>

using namespace std;

Image blendingweight(int imwidth, int imheight) {
    // // --------- HANDOUT  PS07 ------------------------------
    Image weights(imwidth, imheight, 1);
    for (int x = 0; x < imwidth; ++x) {
        for (int y = 0; y < imheight; ++y) {
            float X = 1.0 - fabs(2.0 * (0.5f - (x / float(imwidth))));
            float Y = 1.0 - fabs(2.0 * (0.5f - (y / float(imheight))));
            weights(x, y) = X * Y;
        }
    }
    return weights;
}

//  ****************************************************************************
//  * blending related functions re-written from previous assignments
//  ****************************************************************************

// instead of writing source in out, *add* the source to out based on the weight
// so out(x,y) = out(x, y) + weight * image
void applyhomographyBlend(const Image &source, const Image &weight, Image &out,
                          const Matrix &H, bool bilinear) {
    // --------- HANDOUT  PS07 ------------------------------
    for (int x = 0; x < out.width(); ++x) {
        for (int y = 0; y < out.height(); ++y) {
            Vec3f XPrimeYPrimeW = H.inverse() * Vec3f(x, y, 1);
            XPrimeYPrimeW /= (float)XPrimeYPrimeW[2];
            float X = XPrimeYPrimeW[0];
            float Y = XPrimeYPrimeW[1];
            if (X < 0 || Y < 0 || X >= source.width() || Y >= source.height()) {
                continue;
            }
            for (int c = 0; c < out.channels(); ++c) {
                if (bilinear) {
                    out(x, y, c) +=  interpolateLin(source, X, Y, c, true) * interpolateLin(weight, X, Y, 0, true);
                } else {
                    out(x, y, c) += source(X, Y, c) * weight(X, Y, 0);
                }
            }
        }
    }
}

Image stitchLinearBlending(const Image &im1, const Image &im2, const Image &we1,
                           const Image &we2, const Matrix &H) {
    // --------- HANDOUT  PS07 ------------------------------
    // stitch using image weights.
    // note there is no weight normalization.
    BoundingBox bbox1 = computeTransformedBBox(im1.width(), im1.height(), H);
    BoundingBox bbox2 = BoundingBox(0, im2.width(), 0, im2.height());
    BoundingBox bbox = bboxUnion(bbox1, bbox2);
    Matrix T = makeTranslation(bbox);
    Image stitchedLinear(bbox.x2 - bbox.x1, bbox.y2 - bbox.y1, im1.channels());
    applyhomographyBlend(im2, we2, stitchedLinear, T, true);
    applyhomographyBlend(im1, we1, stitchedLinear, T * H, true);
    return stitchedLinear;
}

/*****************************************************************************
 * blending functions Pset 08
 *****************************************************************************/

// low freq and high freq (2-scale decomposition)
vector<Image> scaledecomp(const Image &im, float sigma) {
    vector<Image> ims;
    ims.push_back(gaussianBlur_separable(im, sigma));
    ims.push_back(im - ims[0]);
    return ims;
}

// stitch using different blending models
// blend can be 0 (none), 1 (linear) or 2 (2-layer)
Image stitchBlending(const Image &im1, const Image &im2, const Matrix &H,
                     BlendType blend) {
    // --------- HANDOUT  PS07 ------------------------------
    return Image(1, 1, 1);
}

// auto stitch
Image autostitch(const Image &im1, const Image &im2, BlendType blend,
                 float blurDescriptor, float radiusDescriptor) {
    // --------- HANDOUT  PS07 ------------------------------
    return Image(1, 1, 1);
}

/************************************************************************
 * Tricks: mini planets.
 ************************************************************************/

Image pano2planet(const Image &pano, int newImSize, bool clamp) {
    // // --------- HANDOUT  PS07 ------------------------------
    return Image(1, 1, 1);
}

/************************************************************************
 * 6.865: Stitch N images into a panorama
 ************************************************************************/

// Pset07-865. Compute sequence of N-1 homographies going from Im_i to Im_{i+1}
// Implement me!
vector<Matrix> sequenceHs(const vector<Image> &ims, float blurDescriptor,
                          float radiusDescriptor) {
    // // --------- HANDOUT  PS07 ------------------------------
    return vector<Matrix>();
}

// stack homographies:
//   transform a list of (N-1) homographies that go from I_i to I_i+1
//   to a list of N homographies going from I_i to I_refIndex.
vector<Matrix> stackHomographies(const vector<Matrix> &Hs, int refIndex) {
    // // --------- HANDOUT  PS07 ------------------------------
    return vector<Matrix>();
}

// Pset07-865: compute bbox around N images given one main reference.
BoundingBox bboxN(const vector<Matrix> &Hs, const vector<Image> &ims) {
    // // --------- HANDOUT  PS07 ------------------------------
    return BoundingBox(0, 0, 0, 0);
}

// Pset07-865.
// Implement me!
Image autostitchN(const vector<Image> &ims, int refIndex, float blurDescriptor,
                  float radiusDescriptor) {
    // // --------- HANDOUT  PS07 ------------------------------
    return Image(1, 1, 1);
}

/******************************************************************************
 * Helper functions
 *****************************************************************************/

// copy a single-channeled image to several channels
Image copychannels(const Image &im, int nChannels) {
    // image must have one channel
    assert(im.channels() == 1);
    Image oim(im.width(), im.height(), nChannels);

    for (int i = 0; i < im.width(); i++) {
        for (int j = 0; j < im.height(); j++) {
            for (int c = 0; c < nChannels; c++) {
                oim(i, j, c) = im(i, j);
            }
        }
    }
    return oim;
}
