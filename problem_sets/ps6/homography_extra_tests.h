#include "homography.h"
#include "matrix.h"
#include <cassert>
#include <iostream>

using namespace std;

void testStitchScience() {
    // // --------- HANDOUT  PS06 ------------------------------
    Image science1 = Image("Input/science-1.png");
    Image science2 = Image("Input/science-2.png");
    CorrespondencePair corresp[4] = {
        CorrespondencePair( 13, 304.125, 1, 213, 297.125, 1),
        CorrespondencePair( 105, 308.125, 1, 304, 296.125, 1),
        CorrespondencePair( 84, 365.125, 1, 287, 355.125, 1),
        CorrespondencePair( 52, 368.125, 1, 254, 358.125, 1)
    };
    Image science = stitch(science1, science2, corresp);
    science.write("./Output/science-stitch.png");
}

void testStitchConvention() {
    // // --------- HANDOUT  PS06 ------------------------------

    Image conv1 = Image("Input/convention-1.png");
    Image conv2 = Image("Input/convention-2.png");
    CorrespondencePair corresp[4] = {
        CorrespondencePair( 280, 291.125, 1, 142, 296.125, 1),
        CorrespondencePair( 167, 297.125, 1, 25, 314.125, 1),
        CorrespondencePair( 169, 202.125, 1, 23, 212.125, 1),
        CorrespondencePair( 297, 129.125, 1, 149, 142.125, 1)
    };
    Image convention = stitch(conv1, conv2, corresp);
    convention.write("./Output/convention-stitch.png");
}

void testStitchBoston1() {
    // // --------- HANDOUT  PS06 ------------------------------
    Image bos1 = Image("Input/boston1-1.png");
    Image bos2 = Image("Input/boston1-2.png");
    CorrespondencePair corresp[4] = {
        CorrespondencePair( 260, 360.125, 1, 109, 360.125, 1),
        CorrespondencePair( 187, 298.125, 1, 28, 304.125, 1),
        CorrespondencePair( 184, 349.125, 1, 24, 359.125, 1),
        CorrespondencePair( 228, 175.125, 1, 73, 175.125, 1)
    };
    Image bos = stitch(bos1, bos2, corresp);
    bos.write("./Output/boston1-stitch.png");
}
