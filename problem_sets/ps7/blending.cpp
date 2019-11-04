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
    Matrix HI = H.inverse();
    BoundingBox bound = computeTransformedBBox(out.width(), out.height(), H);
    bound.x1 = max(0, bound.x1);
    bound.x2 = min(out.width(), bound.x2);
    bound.y1 = max(0, bound.y1);
    bound.y2 = min(out.height(), bound.y2);
    for (int x = bound.x1; x < bound.x2; ++x) {
        for (int y = bound.y1; y < bound.y2; ++y) {
            Vec3f XPrimeYPrimeW = HI * Vec3f(x, y, 1.0);
            XPrimeYPrimeW /= (float)XPrimeYPrimeW[2];
            int X = (int)round(XPrimeYPrimeW[0]);
            int Y = (int)round(XPrimeYPrimeW[1]);
            if (X >= 0 and Y >= 0 and X < source.width() and Y < source.height()) {
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
}

Image stitchLinearBlending(const Image &im1, const Image &im2, const Image &we1,
                           const Image &we2, const Matrix &H) {
    // --------- HANDOUT  PS07 ------------------------------
    // stitch using image weights.
    // note there is no weight normalization.
    BoundingBox bbox1 = computeTransformedBBox(im1.width(), im1.height(), H);
    BoundingBox bbox2 = BoundingBox(0, im2.width() - 1, 0, im2.height() - 1);
    BoundingBox bbox = bboxUnion(bbox1, bbox2);
    Matrix T = makeTranslation(bbox);
    Image stitchedLinear(bbox.x2 - bbox.x1, bbox.y2 - bbox.y1, im1.channels());
    applyhomographyBlend(im2, we2, stitchedLinear, T, true);
    applyhomographyBlend(im1, we1, stitchedLinear, T * H, true);
    return stitchedLinear;
}

Image stitchLinearBlendingNormalized(const Image &im1, const Image &im2, const Image &we1,
                                     const Image &we2, const Matrix &H) {
    // --------- HANDOUT  PS07 ------------------------------
    // stitch using image weights.
    // note there is no weight normalization.
    BoundingBox bbox1 = computeTransformedBBox(im1.width(), im1.height(), H);
    BoundingBox bbox2 = BoundingBox(0, im2.width() - 1, 0, im2.height() - 1);
    BoundingBox bbox = bboxUnion(bbox1, bbox2);
    Matrix T = makeTranslation(bbox);
    Image stitchedLinear(bbox.x2 - bbox.x1, bbox.y2 - bbox.y1, im1.channels());
    Image weightSums(bbox.x2 - bbox.x1, bbox.y2 - bbox.y1, im1.channels());
    Image norm1(im1.width(), im1.height(), im1.channels());
    Image norm2(im2.width(), im2.height(), im2.channels());
    norm1.set_color(1.0, 1.0, 1.0);
    norm2.set_color(1.0, 1.0, 1.0);
    applyhomographyBlend(norm2, we2, weightSums, T, true);
    applyhomographyBlend(norm1, we1, weightSums, T * H, true);
    applyhomographyBlend(im2, we2, stitchedLinear, T, true);
    applyhomographyBlend(im1, we1, stitchedLinear, T * H, true);
    for (int c = 0; c < stitchedLinear.channels(); ++c) {
        for (int y = 0; y < stitchedLinear.height(); ++y) {
            for (int x = 0; x < stitchedLinear.width(); ++x) {
                if (weightSums(x, y) != 0) stitchedLinear(x, y, c) /= weightSums(x, y);
            }
        }
    }
    return stitchedLinear;
}

Image stitchAbrupt(const Image &im1,
                   const Image &im2,
                   const Image &we1,
                   const Image &we2,
                   int outWidth,
                   int outHeight,
                   Matrix H) {
    // --------- HANDOUT  PS07 ------------------------------
    BoundingBox bbox1 = computeTransformedBBox(we1.width(), we1.height(), H);
    BoundingBox bbox2 = BoundingBox(0, we2.width() - 1, 0, we2.height() - 1);
    BoundingBox bbox = bboxUnion(bbox1, bbox2);
    Matrix T = makeTranslation(bbox);
    Image w1Prime(outWidth, outHeight, 1);
    Image w2Prime(outWidth, outHeight, 1);
    Image out(outWidth, outHeight, 3);
    Image outReference(outWidth, outHeight, 3);
    applyHomographyFast(we1, T * H, w1Prime, true);
    applyHomographyFast(we2, T, w2Prime, true);
    applyHomographyFast(im1, T * H, out, true);
    applyHomographyFast(im2, T, outReference, true);
    for (int c = 0; c < out.channels(); c++) {
        for (int y = 0; y < outHeight; ++y) {
            for (int x = 0; x < outWidth; ++x) {
                // Change out (reference to 1) iff
                // outReference (reference to 2) has
                // greater weight.
                if (w1Prime(x, y) < w2Prime(x, y)) {
                    out(x, y, c) = outReference(x, y, c);
                }
            }
        }
    }
    return out;
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
    Image we1 = blendingweight(im1.width(), im1.height());
    Image we2 = blendingweight(im2.width(), im2.height());
    if (blend == BlendType::BLEND_NONE) {
        BoundingBox bbox1 = computeTransformedBBox(im1.width(), im1.height(), H);
        BoundingBox bbox2 = BoundingBox(0, im2.width() - 1, 0, im2.height() - 1);
        BoundingBox bbox = bboxUnion(bbox1, bbox2);
        Matrix T = makeTranslation(bbox);
        Image stitch(bbox.x2 - bbox.x1, bbox.y2 - bbox.y1, im1.channels());
        applyHomographyFast(im2, T, stitch, true);
        applyHomographyFast(im1, T * H, stitch, true);
        return stitch;
    } else if (blend == BlendType::BLEND_LINEAR) {
        return stitchLinearBlendingNormalized(im1, im2, we1, we2, H);
    } else if (blend == BlendType::BLEND_2LAYER) {
        vector<Image> decomp1 = scaledecomp(im1);
        vector<Image> decomp2 = scaledecomp(im2);
        Image low1 = decomp1[0];
        Image low2 = decomp2[0];
        Image high1 = decomp1[1];
        Image high2 = decomp2[1];
        Image lowOut = stitchBlending(low1, low2, H, BlendType::BLEND_LINEAR);
        Image highOut = stitchAbrupt(
                            high1,
                            high2,
                            we1,
                            we2,
                            lowOut.width(),
                            lowOut.height(),
                            H);
        return highOut + lowOut;
    } else {
        assert(false == true); // Should never get here.
    }
}

// auto stitch
Image autostitch(const Image &im1, const Image &im2, BlendType blend,
                 float blurDescriptor, float radiusDescriptor) {
    // --------- HANDOUT  PS07 ------------------------------
    Matrix H = RANSAC(findCorrespondences(
                          computeFeatures(im1, HarrisCorners(im1), blurDescriptor, radiusDescriptor),
                          computeFeatures(im2, HarrisCorners(im2), blurDescriptor, radiusDescriptor)
                      ));
    return stitchBlending(im1, im2, H, blend);
}

/************************************************************************
 * Tricks: mini planets.
 ************************************************************************/

Image pano2planet(const Image &pano, int newImSize, bool clamp) {
    // // --------- HANDOUT  PS07 ------------------------------

    Image planet(newImSize, newImSize, pano.channels());
    float C = newImSize / 2.0f;
    float angleMult = pano.width() / (2 * M_PI);
    float radiusMult = pano.height() / C;
    for (int y = 0; y < newImSize; ++y) {
        for (int x = 0; x < newImSize; ++x) {
            // The left and right sides of the input panorama should be mapped to
            // an angle of 0, along the right horizontal axis in the new image with
            // increasing (counter-clockwise) angle in the output corresponding to
            // sweeping from left to right of the input panorama.
            int xDistCenter = x - C;
            int yDistCenter = y - C;
            float r = sqrt(pow(xDistCenter, 2.0) + pow(yDistCenter, 2.0));
            float theta = -atan2(yDistCenter, xDistCenter);
            for (int c = 0; c < planet.channels(); ++c) {
                float X = angleMult * theta;
                float Y = pano.height() - radiusMult * r;
                // Use interpolateLin to copy
                // pixels from panorama to planet image.
                if (theta > 0) {
                    planet(x, y, c) = interpolateLin(pano, X, Y, c, true);
                } else {
                    // Sweep around.
                    planet(x, y, c) = interpolateLin(pano, X + pano.width(), Y, c, true);
                }
            }
        }
    }
    return planet;
}

/************************************************************************
 * 6.865: Stitch N images into a panorama
 ************************************************************************/

// Pset07-865. Compute sequence of N-1 homographies going from Im_i to Im_{i+1}
// Implement me!
vector<Matrix> sequenceHs(const vector<Image> &ims, float blurDescriptor,
                          float radiusDescriptor) {
    // // --------- HANDOUT  PS07 ------------------------------
    vector<Matrix> seqH;
    for (int i = 0; i < ims.size() - 1; ++i) {
        vector<Feature> F1 = computeFeatures(
                                 ims[i],
                                 HarrisCorners(ims[i]),
                                 blurDescriptor,
                                 radiusDescriptor);
        vector<Feature> F2 = computeFeatures(
                                 ims[i + 1],
                                 HarrisCorners(ims[i + 1]),
                                 blurDescriptor,
                                 radiusDescriptor);
        // Find the homography between this and the next image.
        seqH.push_back(RANSAC(findCorrespondences(F1, F2)));
    }
    return seqH;
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
