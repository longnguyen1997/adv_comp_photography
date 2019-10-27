/* -----------------------------------------------------------------
 * File:    a5_main.cpp
 * Author:  Michael Gharbi <gharbi@mit.edu>
 * Created: 2015-09-30
 * -----------------------------------------------------------------
 *
 *
 *
 * ---------------------------------------------------------------*/

#include "Image.h"
#include "basicImageManipulation.h"
#include "morphing.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void testNearestNeighbor() {
    // Test nearest neighbor
    const Image im("./Input/BostonRainbow-crop-400.png");
    float fact = 3.5;
    Image scaledNN = scaleNN(im, fact);
    scaledNN.write("./Output/testNN.png");
}

void testBilinearInterpolation() {
    // Test bilinear interpolation
    cout << endl << "linear interpolation" << endl;
    cout << "--------------------" << endl;
    Image test(2, 2, 1);
    test(0, 0, 0) = 0.0f;
    test(0, 1, 0) = 0.25f;
    test(1, 0, 0) = 0.5f;
    test(1, 1, 0) = 1.0f;

    float x = 0.25f, y = 0.0;
    cout << "interpolate at (" << x << ", " << y
         << "): " << interpolateLin(test, x, y, 0, false) << ", should be 0.125"
         << endl;

    x = 0.0;
    y = 0.25f;
    cout << "interpolate at (" << x << ", " << y
         << "): " << interpolateLin(test, x, y, 0, false) << ", should be 0.0625"
         << endl;

    x = 0.5;
    y = 0.5f;
    cout << "interpolate at (" << x << ", " << y
         << "): " << interpolateLin(test, x, y, 0, false) << ", should be 0.4375"
         << endl;
}

void testBilinearRescaling() {
    // Test bilinear
    const Image im("./Input/BostonRainbow-crop-400.png");
    float fact = 3.5;
    Image scaled = scaleLin(im, fact);
    scaled.write("./Output/testLin.png");
}

void testBicubicRescaling() {
    const Image im("./Input/BostonRainbow-crop-400.png");
    Image scaled = scaleBicubic(im, 3.5, 0.33, 0.33);
    scaled.write("./Output/testBicubic.png");
}

void testLanczos() {
    const Image im("./Input/BostonRainbow-crop-400.png");
    Image scaled = scaleLanczos(im, 3.5, 3);
    scaled.write("./Output/testLanczos.png");
}

void testRotation() {
    const Image im("./Input/BostonRainbow-crop-400.png");

    float theta = M_PI / 8;

    Image rot = rotate(im, theta);
    rot.write("./Output/testRotate.png");
}

void testVectorOperations() {
    // Test vector lib
    Vec2f a(10.0f, 3.0f);
    Vec2f b(-4.1f, 2.7f);
    float f = 2.0f;
    cout << endl << "vector operations" << endl;
    cout << "-----------------" << endl;

    cout << "a(" << a.x << ", " << a.y << ")" << endl;
    cout << "b(" << b.x << ", " << b.y << ")" << endl;
    cout << "f=" << f << endl;

    // a+b
    Vec2f c = a + b;
    cout << "a+b: (" << c.x << ", " << c.y << ")" << endl;

    // a-b
    c = a - b;
    cout << "a-b: (" << c.x << ", " << c.y << ")" << endl;

    // a*f
    c = a * f;
    cout << "a*f: (" << c.x << ", " << c.y << ")" << endl;

    // a/f
    c = a / f;
    cout << "a/f: (" << c.x << ", " << c.y << ")" << endl;

    // <a,b>
    float s = dot(a, b);
    cout << "<a,b>=" << s << endl;

    // ||a||
    float l = length(a);
    cout << "||a||=" << l << endl;

    Vec2f p_a = perpendicular(a);
    cout << "<a, perpendicular(a)>=" << dot(a, p_a) << endl;
}

void testSegment() {
    // Test segment
    // are P,Q,e1 e1 correctly implemented?
    // are the u,v coordinates meaningful?
    // What should be u and v for P,Q ?
    // Come up with a few test cases !
}

void testWarpBy1() {
    // Test warpBy1 ----------------------------------
    Image fredo("./Input/fredo2.png");

    // define before and after segments
    Segment segBefore(Vec2f(81, 105), Vec2f(122, 112));
    Segment segAfter(Vec2f(131, 168), Vec2f(134, 127));

    // warp
    Image warp1 = warpBy1(fredo, segBefore, segAfter);
    warp1.write("./Output/fredo_upside_down.png");
    // ------------------------------------------------
}

void testWarp() {
    // Make your own test !
    Image bear("./Input/bear.png");

    // define before and after segments
    Segment segBefore(Vec2f(0, 0), Vec2f(10, 0));
    Segment segAfter(Vec2f(10, 10), Vec2f(30, 15));

    // warp
    Image warp1 = warpBy1(bear, segBefore, segAfter);
    warp1.write("./Output/bear.png");
}

void testMorph() {
    // Test morph
    Image fredo("./Input/fredo2.png");
    Image werewolf("./Input/werewolf.png");

    // ... use the UI to specify segments
    vector<Segment> segsBefore;
    segsBefore.push_back(Segment(Vec2f(85, 102), Vec2f(119, 108)));
    segsBefore.push_back(Segment(Vec2f(143, 117), Vec2f(169, 114)));
    vector<Segment> segsAfter;
    segsAfter.push_back(Segment(Vec2f(86, 93), Vec2f(114, 104)));
    segsAfter.push_back(Segment(Vec2f(137, 94), Vec2f(152, 79)));

    // This should monsterify Fredo a little.
    vector<Image> out = morph(fredo, werewolf, segsBefore, segsAfter, 5);

    // This is how you can write an image sequence
    for (int i = 0; i < (int)out.size(); ++i) {
        ostringstream fname;
        fname << "./Output/test_morph_";
        fname << setfill('0') << setw(2);
        fname << i;
        fname << ".png";
        out[i].write(fname.str());
    }
}

void classMorph() {
    // Test morph
    Image lpn("./lpn.png");
    Image pjwu("./pjwu.png");

    // ... use the UI to specify segments
    vector<Segment> segsBefore;
    segsBefore.push_back(Segment(Vec2f(152, 197), Vec2f(229, 152)));
    segsBefore.push_back(Segment(Vec2f(336, 173), Vec2f(351, 268)));
    segsBefore.push_back(Segment(Vec2f(205, 349), Vec2f(297, 343)));
    segsBefore.push_back(Segment(Vec2f(168, 379), Vec2f(232, 416)));
    segsBefore.push_back(Segment(Vec2f(343, 363), Vec2f(317, 395)));
    segsBefore.push_back(Segment(Vec2f(182, 236), Vec2f(226, 241)));
    segsBefore.push_back(Segment(Vec2f(281, 240), Vec2f(326, 236)));
    vector<Segment> segsAfter;
    segsAfter.push_back(Segment(Vec2f(146, 112), Vec2f(221, 62)));
    segsAfter.push_back(Segment(Vec2f(356, 68), Vec2f(398, 217)));
    segsAfter.push_back(Segment(Vec2f(193, 335), Vec2f(328, 333)));
    segsAfter.push_back(Segment(Vec2f(129, 351), Vec2f(241, 441)));
    segsAfter.push_back(Segment(Vec2f(390, 351), Vec2f(346, 399)));
    segsAfter.push_back(Segment(Vec2f(170, 180), Vec2f(224, 193)));
    segsAfter.push_back(Segment(Vec2f(300, 189), Vec2f(366, 180)));

    vector<Image> out = morph(lpn, pjwu, segsBefore, segsAfter, 30);

    // This is how you can write an image sequence
    for (int i = 0; i < (int)out.size(); ++i) {
        ostringstream fname;
        fname << "./class_morph/class_morph_";
        fname << setfill('0') << setw(2);
        fname << i;
        fname << ".png";
        out[i].write(fname.str());
    }
}

// This is a way for you to test your functions.
// We will only grade the contents of morphing.cpp and
// basicImageManipulation.cpp
int main() {
    // testNearestNeighbor();
    // testBilinearInterpolation();
    // testBilinearRescaling();
    // testBicubicRescaling();
    // testLanczos();
    // testRotation();
    // testVectorOperations();
    // testSegment();
    // testWarpBy1();
    // testWarp();
    // testMorph();
    classMorph();
}
