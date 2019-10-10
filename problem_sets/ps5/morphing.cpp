/* -----------------------------------------------------------------
 * File:    morphing.cpp
 * Created: 2015-09-25
 * -----------------------------------------------------------------
 *
 *
 *
 * ---------------------------------------------------------------*/

#include "morphing.h"
#include <cassert>

using namespace std;

Vec2f operator+(const Vec2f &a, const Vec2f &b) {
  // --------- HANDOUT  PS05 ------------------------------
  // Return the vector sum of a an b
  return Vec2f(0.0f, 0.0f); // change me
}

Vec2f operator-(const Vec2f &a, const Vec2f &b) {
  // --------- HANDOUT  PS05 ------------------------------
  // Return a-b
  return Vec2f(0.0f, 0.0f); // change me
}

Vec2f operator*(const Vec2f &a, float f) {
  // --------- HANDOUT  PS05 ------------------------------
  // Return a*f
  return Vec2f(0.0f, 0.0f); // change me
}

Vec2f operator/(const Vec2f &a, float f) {
  // --------- HANDOUT  PS05 ------------------------------
  // Return a/f
  return Vec2f(0.0f, 0.0f); // change me
}

float dot(const Vec2f &a, const Vec2f &b) {
  // --------- HANDOUT  PS05 ------------------------------
  // Return the dot product of a and b.
  return 0.0f; // change me
}

float length(const Vec2f &a) {
  // --------- HANDOUT  PS05 ------------------------------
  // Return the length of a.
  return 0.0f; // change me
}

Vec2f perpendicular(const Vec2f &a) {
  // --------- HANDOUT  PS05 ------------------------------
  // Return a vector that is perpendicular to a.
  // Either direction is fine.
  return a;
}

// The Segment constructor takes in 2 points P(x1,y1) and Q(x2,y2) corresponding
// to the ends of a segment and initialize the local reference frame e1,e2.
Segment::Segment(Vec2f P_, Vec2f Q_) : P(P_), Q(Q_) {
  // // --------- HANDOUT  PS05 ------------------------------
  // // The initializer list above ": P(P_), Q(Q_)" already copies P_
  // // and Q_, so you don't have to do it in the body of the constructor.
  // You should:
  // * Initialize the local frame e1,e2 (see header file)
}

Vec2f Segment::XtoUV(Vec2f X) const {
  // --------- HANDOUT  PS05 ------------------------------
  // Compute the u,v coordinates of a point X with
  // respect to the local frame of the segment.
  // e2 ^
  //    |
  // v  +  * X
  //    | /
  //    |/
  //    *--+------>-----*
  //    P  u     e1     Q
  //                    u=1
  //
  // * Be careful with the different normalization for u and v
  return Vec2f(0.0f, 0.0f); // changeme
}

Vec2f Segment::UVtoX(Vec2f uv) const {
  // --------- HANDOUT  PS05 ------------------------------
  // compute the (x, y) position of a point given by the (u,v)
  // location relative to this segment.
  // * Be careful with the different normalization for u and v
  return Vec2f(0.0f, 0.0f);
}

float Segment::distance(Vec2f X) const {
  // --------- HANDOUT  PS05 ------------------------------
  // Implement distance from a point X(x,y) to the segment. Remember the 3
  // cases from class.
  return 0.0f;
}

Image warpBy1(const Image &im, const Segment &segBefore,
              const Segment &segAfter) {
  // --------- HANDOUT  PS05 ------------------------------
  // Warp an entire image according to a pair of segments.
  return im;
}

float Segment::weight(Vec2f X, float a, float b, float p) const {
  // --------- HANDOUT  PS05 ------------------------------
  // compute the weight of a segment to a point X(x,y) given the weight
  // parameters a,b, and p (see paper for details).
  return 1.0f; // changeme
}

Image warp(const Image &im, const vector<Segment> &src_segs,
           const vector<Segment> &dst_segs, float a, float b, float p) {
  // --------- HANDOUT  PS05 ------------------------------
  // Warp an image according to a vector of before and after segments using
  // segment weighting
  return im;
}

vector<Image> morph(const Image &im_before, const Image &im_after,
                    const vector<Segment> &segs_before,
                    const vector<Segment> &segs_after, int N, float a, float b,
                    float p) {
  // --------- HANDOUT  PS05 ------------------------------
  // return a vector of N+2 images: the two inputs plus N images that morphs
  // between im_before and im_after for the corresponding segments. im_before
  // should be the first image, im_after the last.
  return vector<Image>();
}
