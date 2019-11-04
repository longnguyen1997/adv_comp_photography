#include "homography.h"
#include "matrix.h"

using namespace std;

void applyHomography(const Image &source, const Matrix &H, Image &out,
                     bool bilinear) {
    // --------- HANDOUT  PS06 ------------------------------
    // Transform image source using the homography H, and composite in onto out.
    // if bilinear == true, using bilinear interpolation. Use nearest neighbor
    // otherwise.
    Matrix HInverse = H.inverse();
    for (int y = 0; y < out.height(); ++y) {
        for (int x = 0; x < out.width(); ++x) {
            if (x <= source.width() - 1 and y <= source.height() - 1) {
                Vec3f XPrimeYPrimeW = HInverse * Vec3f(x, y, 1.0);
                XPrimeYPrimeW /= (float)XPrimeYPrimeW[2];
                float X = (int)round(XPrimeYPrimeW[0]);
                float Y = (int)round(XPrimeYPrimeW[1]);
                for (int c = 0; c < out.channels(); ++c) {
                    out(x, y, c) = bilinear ? interpolateLin(source, X, Y, c, true) : source(X, Y, c);
                }
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
    equations << x, y, 1.0, 0.0, 0.0, 0.0, -(xPrime * x), -(xPrime * y),
              0.0, 0.0, 0.0, x, y, 1.0, -(x * yPrime), -(y * yPrime);
    return equations;
}

Matrix computeHomography(const CorrespondencePair correspondences[4]) {
    // --------- HANDOUT  PS06 ------------------------------
    // Compute a homography from 4 point correspondences.
    Matrix A = Matrix::Zero(8, 8);
    Matrix B = Matrix::Zero(8, 1);
    for (int c = 0; c < 4; ++c) {
        CorrespondencePair pair = correspondences[c];
        Matrix equationPair = generateEquations(pair);
        A.block<2, 8>(c * 2, 0) = equationPair;
        float xPrime = pair.point2[0];
        float yPrime = pair.point2[1];
        B(c * 2, 0) = xPrime;
        B(c * 2 + 1, 0) = yPrime;
    }
    Matrix solution = A.inverse() * B;
    Matrix H(3, 3);
    for (int i = 0; i < 8; ++i) H(i / 3, i % 3) = solution(i, 0);
    H(2, 2) = 1;
    return H;
}

BoundingBox computeTransformedBBox(int imwidth, int imheight, Matrix H) {
    // --------- HANDOUT  PS06 ------------------------------
    // Predict the bounding boxes that encompasses all the transformed
    // coordinates for pixels frow and Image with size (imwidth, imheight)
    Vec3f A, B, C, D;
    A << 0, 0, 1;
    B << imwidth, 0, 1;
    C << 0, imheight, 1;
    D << imwidth, imheight, 1;
    A = H * A;
    B = H * B;
    C = H * C;
    D = H * D;
    A /= A[2];
    B /= B[2];
    C /= C[2];
    D /= D[2];
    int x1, x2, y1, y2;
    x1 = min(A[0], min(B[0], min(C[0], D[0])));
    y1 = min(A[1], min(B[1], min(C[1], D[1])));
    x2 = max(A[0], max(B[0], max(C[0], D[0])));
    y2 = max(A[1], max(B[1], max(C[1], D[1])));
    return BoundingBox(x1, x2, y1, y2);
}

BoundingBox bboxUnion(BoundingBox B1, BoundingBox B2) {
    // --------- HANDOUT  PS06 ------------------------------
    // Compute the bounding box that tightly bounds the union of B1 an B2.
    int a, b, c, d;
    a = min(B1.x1, B2.x1);
    b = max(B1.x2, B2.x2);
    c = min(B1.y1, B2.y1);
    d = max(B1.y2, B2.y2);
    assert(a <= b);
    assert(c <= d);
    return BoundingBox(a, b, c, d);
}

Matrix makeTranslation(BoundingBox B) {
    // --------- HANDOUT  PS06 ------------------------------
    // Compute a translation matrix (as a homography matrix) that translates the
    // top-left corner of B to (0,0).
    Matrix T(3, 3);
    T << 1, 0, -B.x1,
    0, 1, -B.y1,
    0, 0, 1;
    return T;
}

Image stitch(const Image &im1, const Image &im2,
             const CorrespondencePair correspondences[4]) {
    // --------- HANDOUT  PS06 ------------------------------
    // Transform im1 to align with im2 according to the set of correspondences.
    // make sure the union of the bounding boxes for im2 and transformed_im1 is
    // translated properly (use makeTranslation)

    // You first need to compute the homography between
    // the two images.
    Matrix H = computeHomography(correspondences);
    // Then compute the resulting bounding
    // box and translation matrix.
    BoundingBox newBox = bboxUnion(
                             computeTransformedBBox(im1.width(), im1.height(), H),
                             BoundingBox(0, im2.width() - 1, 0, im2.height() - 1)
                         );
    Matrix T = makeTranslation(newBox);
    // Create a new black image
    // of the size of the bounding box.
    Image canvas(newBox.x2 - newBox.x1,
                 newBox.y2 - newBox.y1,
                 max(im1.channels(), im2.channels()));
    // Then use a combination
    // of your translation and homography matrix to composite
    // both images into the output.
    applyHomographyFast(im1, T * H, canvas, true);
    applyHomographyFast(im2, T, canvas, true);
    return canvas;
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
    Matrix HInverse = H.inverse();
    BoundingBox bound = computeTransformedBBox(out.width(), out.height(), H);

    bound.x1 = max(0, bound.x1);
    bound.x2 = min(out.width(), bound.x2);
    bound.y1 = max(0, bound.y1);
    bound.y2 = min(out.height(), bound.y2);

    for (int x = bound.x1; x < bound.x2; ++x) {
        for (int y = bound.y1; y < bound.y2; ++y) {
            Vec3f XPrimeYPrimeW = HInverse * Vec3f(x, y, 1.0);
            XPrimeYPrimeW /= XPrimeYPrimeW[2];
            int X = (int)round(XPrimeYPrimeW[0]);
            int Y = (int)round(XPrimeYPrimeW[1]);
            if (X < 0 || Y < 0 || X >= source.width() || Y >= source.height()) {
                continue;
            }
            for (int c = 0; c < out.channels(); ++c) {
                out(x, y, c) = bilinear ? interpolateLin(source, X, Y, c, true) : source(X, Y, c);
            }

        }
    }
}
