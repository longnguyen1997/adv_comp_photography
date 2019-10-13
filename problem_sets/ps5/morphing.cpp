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
    return Vec2f(a.x + b.x, a.y + b.y);
}

Vec2f operator-(const Vec2f &a, const Vec2f &b) {
    // --------- HANDOUT  PS05 ------------------------------
    // Return a-b
    return Vec2f(a.x - b.x, a.y - b.y);
}

Vec2f operator*(const Vec2f &a, float f) {
    // --------- HANDOUT  PS05 ------------------------------
    // Return a*f
    return Vec2f(f * a.x, f * a.y);
}

Vec2f operator/(const Vec2f &a, float f) {
    // --------- HANDOUT  PS05 ------------------------------
    // Return a/f
    return Vec2f(a.x / f, a.y / f);
}

float dot(const Vec2f &a, const Vec2f &b) {
    // --------- HANDOUT  PS05 ------------------------------
    // Return the dot product of a and b.
    return a.x * b.x + a.y * b.y;
}

float length(const Vec2f &a) {
    // --------- HANDOUT  PS05 ------------------------------
    // Return the length of a.
    return sqrt(pow(a.x, 2) + pow(a.y, 2));
}

float dist(Vec2f &a, Vec2f &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

Vec2f perpendicular(const Vec2f &a) {
    // --------- HANDOUT  PS05 ------------------------------
    // Return a vector that is perpendicular to a.
    // Either direction is fine.
    return Vec2f(-a.y, a.x);
}

// The Segment constructor takes in 2 points P(x1,y1) and Q(x2,y2) corresponding
// to the ends of a segment and initialize the local reference frame e1,e2.
Segment::Segment(Vec2f P_, Vec2f Q_) : P(P_), Q(Q_) {
    // // --------- HANDOUT  PS05 ------------------------------
    // // The initializer list above ": P(P_), Q(Q_)" already copies P_
    // // and Q_, so you don't have to do it in the body of the constructor.
    // You should:
    // * Initialize the local frame e1,e2 (see header file)
    lPQ = length(Q - P);
    //  e1 = (Q-P)/(||Q-P||) in the paper
    //  e2 = perpendicular(Q-P)/(||Q-P||) in the paper
    e1 = (Q - P) / lPQ;
    e2 = perpendicular(Q - P) / lPQ;
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

    // See Equations 1 and 2 in the paper.
    float u = dot(X - P, Q - P) / pow(lPQ, 2);
    float v = dot(X - P, perpendicular(Q - P)) / lPQ;
    return Vec2f(u, v);
}

Vec2f Segment::UVtoX(Vec2f uv) const {
    // --------- HANDOUT  PS05 ------------------------------
    // compute the (x, y) position of a point given by the (u,v)
    // location relative to this segment.
    // * Be careful with the different normalization for u and v
    return e1 * (lPQ * uv.x) + (e2 * uv.y) + P;
}

float Segment::distance(Vec2f X) const {
    // --------- HANDOUT  PS05 ------------------------------
    // Implement distance from a point X(x,y) to the segment. Remember the 3
    // cases from class.
    Vec2f v = P;
    Vec2f w = Q;
    Vec2f p = X;
    // Return minimum distance between line segment vw and point p
    float l2 = pow(lPQ, 2);  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) return dist(p, v); // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    float t = max(0.0f, min(1.0f, dot(p - v, w - v) / l2));
    Vec2f projection = v + (w - v) * t;  // Projection falls on the segment
    return dist(p, projection);
}

Image warpBy1(const Image &im, const Segment &segBefore,
              const Segment &segAfter) {
    // --------- HANDOUT  PS05 ------------------------------
    // Warp an entire image according to a pair of segments.
    Image warped(im.width(), im.height(), im.channels());
    for (int y = 0; y < im.height(); ++y) {
        for (int x = 0; x < im.width(); ++x) {
            Vec2f uv = segAfter.XtoUV(Vec2f(x, y));
            Vec2f after = segBefore.UVtoX(uv);
            for (int c = 0; c < im.channels(); ++c) {
                warped(x, y, c) = interpolateLin(im, after.x, after.y, c, true);
            }
        }
    }
    return warped;
}

float Segment::weight(Vec2f X, float a, float b, float p) const {
    // --------- HANDOUT  PS05 ------------------------------
    // compute the weight of a segment to a point X(x,y) given the weight
    // parameters a,b, and p (see paper for details).

    // weight = ((length^p)/(a+dist))^b
    return pow(pow(length(X), p) / (a + distance(X)), b);
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
