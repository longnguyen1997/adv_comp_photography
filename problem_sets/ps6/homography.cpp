#include "homography.h"
#include "matrix.h"

using namespace std;

void applyHomography(const Image &source, const Matrix &H, Image &out,
                     bool bilinear) {
    // --------- HANDOUT  PS06 ------------------------------
    // Transform image source using the homography H, and composite in onto out.
    // if bilinear == true, using bilinear interpolation. Use nearest neighbor
    // otherwise.

    for (int x = 0; x < out.width(); ++x) {
        for (int y = 0; y < out.height(); ++y) {
            Vec3f XPrimeYPrimeW = H.inverse() * Vec3f(x, y, 1.0);
            XPrimeYPrimeW /= XPrimeYPrimeW[2];
            float X = XPrimeYPrimeW[0];
            float Y = XPrimeYPrimeW[1];
            if (X < 0 || Y < 0 || X >= source.width() || Y >= source.height()) {
                continue;
            }
            for (int c = 0; c < out.channels(); ++c) {
                out(x, y, c) = bilinear ? interpolateLin(source, X, Y, c, true) : source(X, Y, c);
            }
        }
    }
}

Matrix generateEquations(const CorrespondencePair pair) {
    Matrix equations(2, 8);
    float x = pair.point1[0];
    float y = pair.point1[1];
    float xPrime = pair.point2[0];
    float yPrime = pair.point2[1];
    equations << y, x, 1, 0, 0, 0, -y *yPrime, -x *yPrime,
              0, 0, 0, y, x, 1, -y *xPrime, -x *xPrime;
    return equations;
}

Matrix computeHomography(const CorrespondencePair correspondences[4]) {
    // --------- HANDOUT  PS06 ------------------------------
    // Compute a homography from 4 point correspondences.
    Matrix A = Matrix::Zero(8, 8);
    for (int c = 0; c < 4; ++c) {
        CorrespondencePair pair = correspondences[c];
        Matrix equationPair = generateEquations(pair);
        cout<<equationPair<<endl;
        A.block<2, 8>(c * 2, 0) = equationPair;
    }
    Matrix B = Matrix::Zero(8,1);
    return A;
}

BoundingBox computeTransformedBBox(int imwidth, int imheight, Matrix H) {
    // --------- HANDOUT  PS06 ------------------------------
    // Predict the bounding boxes that encompasses all the transformed
    // coordinates for pixels frow and Image with size (imwidth, imheight)
    return BoundingBox(0, 0, 0, 0);
}

BoundingBox bboxUnion(BoundingBox B1, BoundingBox B2) {
    // --------- HANDOUT  PS06 ------------------------------
    // Compute the bounding box that tightly bounds the union of B1 an B2.
    return BoundingBox(0, 0, 0, 0);
}

Matrix makeTranslation(BoundingBox B) {
    // --------- HANDOUT  PS06 ------------------------------
    // Compute a translation matrix (as a homography matrix) that translates the
    // top-left corner of B to (0,0).
    return Matrix::Zero(3, 3);
}

Image stitch(const Image &im1, const Image &im2,
             const CorrespondencePair correspondences[4]) {
    // --------- HANDOUT  PS06 ------------------------------
    // Transform im1 to align with im2 according to the set of correspondences.
    // make sure the union of the bounding boxes for im2 and transformed_im1 is
    // translated properly (use makeTranslation)
    return im1;
}

// debug-useful
Image drawBoundingBox(const Image &im, BoundingBox bbox) {
    // --------- HANDOUT  PS06 ------------------------------
    /*
      ________________________________________
     / Draw me a bounding box!                \
     |                                        |
     | "I jumped to my                        |
     | feet, completely thunderstruck. I      |
     | blinked my eyes hard. I looked         |
     | carefully all around me. And I saw a   |
     | most extraordinary small person, who   |
     | stood there examining me with great    |
     | seriousness."                          |
     \              Antoine de Saint-Exupery  /
      ----------------------------------------
             \   ^__^
              \  (oo)\_______
                 (__)\       )\/\
                     ||----w |
                     ||     ||
    */
    return im;
}

void applyHomographyFast(const Image &source, const Matrix &H, Image &out,
                         bool bilinear) {
    // --------- HANDOUT  PS06 ------------------------------
    // Same as apply but change only the pixels of out that are within the
    // predicted bounding box (when H maps source to its new position).
}
