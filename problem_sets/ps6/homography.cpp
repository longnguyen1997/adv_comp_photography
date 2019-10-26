#include "homography.h"
#include "matrix.h"

using namespace std;

void applyHomography(const Image &source, const Matrix &H, Image &out,
                     bool bilinear) {
  // --------- HANDOUT  PS06 ------------------------------
  // Transform image source using the homography H, and composite in onto out.
  // if bilinear == true, using bilinear interpolation. Use nearest neighbor
  // otherwise.
}

Matrix computeHomography(const CorrespondencePair correspondences[4]) {
  // --------- HANDOUT  PS06 ------------------------------
  // Compute a homography from 4 point correspondences.
  return Matrix::Zero(3, 3);
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
