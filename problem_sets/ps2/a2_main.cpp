/* --------------------------------------------------------------------------
 * File:    a2_main.cpp
 * Created: 2015-09-23
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "Image.h"
#include "filtering.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// This is a way for you to test your functions.
// We will only grade the contents of filter.cpp and Image.cpp
int main()
{
    cout << "nothing done in a2_main.cpp, debug me !" << endl;

    // ------- Example tests, change them ! --------------
    Image im = impulseImg(10);
    cout << "smart accessor at (1,3,0): " << im.smartAccessor(1, 3, 0, true)
         << endl;

    Image blurred = boxBlur(im, 3, true);
    cout << "blurred impulse image" << endl;

    Image blurredCambridge = boxBlur(Image("Input/Cambridge2.png"), 9, true);
    blurredCambridge.write("Output/Cambridge2Blur.png");

    Image blurredCambridgeFilter = boxBlur_filterClass(Image("Input/Cambridge2.png"), 9, true);
    blurredCambridgeFilter.write("Output/Cambridge2BlurFilterClass.png");

    for (int x = 0; x < blurredCambridge.width(); x++) {
        for (int y = 0; y < blurredCambridge.height(); y++) {
            for (int c = 0; c < blurredCambridge.channels(); c++) {
                assert(abs(blurredCambridge(x, y, c) - blurredCambridgeFilter(x, y, c)) < 0.00005f);
            }
        }
    }

    Image edgesCambridge = edgeDetection(Image("Input/Cambridge2.png"), true);
    edgesCambridge.write("Output/Cambridge2Edges.png");

    gradientMagnitude(Image("Input/lounge_view.png"), true).write("Output/lounge_view_gradient.png");

    gaussianBlur_horizontal(Image("Input/lounge_view.png"), 1.0f, 3.0f, true).write("Output/lounge_view_gaussian_horizontal.png");

    // cout << "keep testing..." << endl;
    // // ---------------------------------------------------

    // // ---------------------------------------------------
    // // Test the filter class on an impulse image
    // Image dirac = impulseImg(31);

    // // Test kernel
    // vector<float> kernel{0, 0, 1, 0, 1, 0, 1, 0, 0}; // C++11 syntax
    // Filter testFilter(kernel, 3, 3);
    // Image testOutput = testFilter.convolve(dirac);
    // // The output should be an exact copy of the kernel in the center of the
    // // image
    // testOutput.write("./Output/testKernel.png");
    // // ---------------------------------------------------

    // // ---------------------------------------------------
    // // E.g. test the sobelKernel
    // // create Sobel Filter that extracts horizontal gradients
    // // [ -1 0 1 ]
    // // [ -2 0 2 ]
    // // [ -1 0 1 ]
    // vector<float> fDataX{-1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0};
    // Filter sobelX(fDataX, 3, 3);

    // // verify that your filter is correct by using it to filter an impulse image
    // Image impulse = impulseImg(11); // create an image containing an impulse
    // // convolve the impulse image with the Sobel kernel. We divide the output
    // // by 4 and add 0.5 to make the range of the image between 0 and 1
    // Image verifyKernel = sobelX.convolve(impulse) / 4 + 0.5;
    // verifyKernel.write("./Output/verifySobelKernel.png");

    // // filter an image using the sobel kernel
    // Image im2("./Input/lounge_view.png");
    // Image sobelFiltered = sobelX.convolve(im2);

    // // make the range of the output image from 0 to 1 for visualization
    // // since the Sobel filter changes the range of a (0,1) image to (-2,2)
    // Image sobelOut = sobelFiltered / 4 + 0.5;
    // sobelOut.write("./Output/sobelFiltered.png");
    // // ---------------------------------------------------

    // // --- Timer example ---------------------------------
    // clock_t start = clock();
    // float sigma = 2.0f;
    // Image blurHorizontal = gaussianBlur_2D(im2, sigma);
    // clock_t end = clock();
    // double duration = (end - start) * 1.0f / CLOCKS_PER_SEC;
    // cout << "2D gaussian took: " << duration << "s" << endl;
    // // ---------------------------------------------------
}
