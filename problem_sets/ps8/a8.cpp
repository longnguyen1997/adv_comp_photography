/**
 *
 * This file contains a number of empty function stubs with the problem
 * statements. You need to fill in all details. These problems
 * are designed as extensions to the tutorials in this Pset. Please run
 * each tutorial and go through the code and comments. You are welcome
 * to discuss details on Piazza if you do not understand the tutorials.
 *
 * The main function is in a8_main.cpp. You can modify the contents for
 * your debugging purposes but restore the original contents.
 *
 * Live long and propser.
 */

#include "a8.h"

using namespace Halide;

// This applies a compute_root() schedule to all the Func's that are consumed
// by the calling Func. DO NOT EDIT IT.
void apply_compute_root(Func F) {
    map<string, Internal::Function> flist =
        Internal::find_transitive_calls(F.function());
    flist.insert(std::make_pair(F.name(), F.function()));
    for (auto fit = flist.begin(); fit != flist.end(); fit++) {
        Func f(fit->second);
        f.compute_root();
        cout << "Warning: applying compute root schedule to " << f.name() << endl;
    }
    cout << endl;
}

// ---------------------------------- PART 1 -----------------------------------
// For all Halide Func's you write in Part 1, you do not need to worry about
// schedule. However, things will be faster if you just use:
// apply_compute_root(final_Result_Func);

Image<uint8_t> SmoothGradNormalized(void) {
    // // --------- HANDOUT  PS08 ------------------------------
    // Use Halide to compute a 512x512 smooth gradient where pixel (x,y) is
    // equal to x+y divided by 1024.
    //
    // HINT: tutorial 1 has a very similar function except that it does not
    // normalize by 1024
    //
    // OUPUT: a single channel output image of size 512x512 (uint8_t)
    //
    // SCHEDULE: use compute_root() on all the Func's you create (i.e.
    // use apply_compute_root(Func f).

    Func gradSmooth;
    Var x("x");
    Var y("y");
    Expr e = (256.0f * (x + y) / 1024.0f);
    e = cast<uint8_t>(e);
    gradSmooth(x, y) = e;
    apply_compute_root(gradSmooth);
    return gradSmooth.realize(512, 512);
}

Image<uint8_t> WavyRGB(void) {
    // // --------- HANDOUT  PS08 ------------------------------
    // Use a Halide Func to compute a wavy RGB image obtained
    // by the formula: output(x,y,c) = (1-c)*cos(x/30)*cos(y/30)
    // Schedule: use compute_root() on all the Func's you create.
    //
    // HINT: you need one more dimension than before
    //
    // OUTPUT 512x512x3 RGB image (uint8_t)
    //
    // SCHEDULE: use compute_root() on all the Func's you create (i.e.
    // use apply_compute_root(Func f).
    const float PI = M_PI;
    Func wavy;
    Var x("x");
    Var y("y");
    Var c("c");
    Expr e = cast<uint8_t>(
                 255.0f * (1 - c) * cos(x / 30.0f) * cos(y / 30.0f)
             );
    wavy(x, y, c) = e;
    apply_compute_root(wavy);
    return wavy.realize(512, 512, 3);
}

Image<uint8_t> Luminance(Image<uint8_t> input) {
    // // --------- HANDOUT  PS08 ------------------------------
    // Use Halide to compute a 1-channel luminance image. Use the formula
    // L = 0.3R+0.6G+0.1B
    //
    // INPUT: RGB image (uint8_t)
    //
    // OUTPUT: single channel luminance image the same size as the input image
    //  note the output is of type uint8_t also
    //
    // SCHEDULE: use compute_root() on all the Func's you create (i.e.
    // use apply_compute_root(Func f).
    Func lum;
    Var x("x");
    Var y("y");
    Expr r = 0.3f * input(x, y, 0);
    Expr g = 0.6f * input(x, y, 1);
    Expr b = 0.1f * input(x, y, 2);
    lum(x, y) = cast<uint8_t>(r + g + b);
    apply_compute_root(lum);
    return lum.realize(input.width(), input.height());
}

Image<uint8_t> Sobel(Image<uint8_t> input) {
    // // --------- HANDOUT  PS08 ------------------------------
    // Use Halide to apply a Sobel filter and return the gradient magnitude.
    // Use the following kernels to compute the horizontal and vertical
    // gradients
    //
    //            | -1 0 1 |             | -1 -2 -1 |
    // x kernel = | -2 0 2 |, y kernel = |  0  0  0 |
    //            | -1 0 1 |             |  1  2  1 |
    //
    // INPUT: single channel luminance image
    //
    // OUTPUT: single channel luminance image the same size as the input image
    // OUTPUT: single channel image where each pixel is the magnitude of Sobel x
    //         and y filters
    //
    // SCHEDULE: use compute_root() on all the Func's you create (i.e.
    // use apply_compute_root(Func f).
    Var x;
    Var y;
    Func clamped;
    clamped(x, y) = cast<float>(input(clamp(x, 0, input.width() - 1), clamp(y, 0, input.height() - 1)));
    Func X, Y;
    X(x, y) = -clamped(x - 1, y - 1) + clamped(x + 1, y - 1) - 2.0f * clamped(x - 1, y) + 2.0f * clamped(x + 1, y) - clamped(x - 1, y + 1) + clamped(x + 1, y + 1);
    Y(x, y) = clamped(x - 1, y + 1) + 2.0f * clamped(x, y + 1) + clamped(x + 1, y + 1) - clamped(x - 1, y - 1) - 2.0f * clamped(x, y - 1) - clamped(x + 1, y - 1);
    Func sobelFilter;
    sobelFilter(x, y) = cast<uint8_t>(sqrt(X(x, y) * X(x, y) + Y(x, y) * Y(x, y)));
    apply_compute_root(sobelFilter);
    return sobelFilter.realize(input.width(), input.height());
}

// Example code for the equivalent .cpp loop questions
Image<uint8_t> boxSchedule1(Image<uint8_t> input) {
    cout << "Started boxSchedule1" << endl; // DO NOT CHANGE

    // Ignore boundaries: use this width and height
    int w = input.width() - 2;
    int h = input.height() - 2;

    // Intermediary stages of the pipeline, same size as input
    Image<float> blur_x(input.width(), input.height());
    Image<uint8_t> blur_y(input.width(), input.height());

    // - Schedule to replicate -
    // blur_y.compute_root();
    // blur_x.compute_root();
    // -------------------------

    // Your equivalent loops should go there --------
    // Compute blur_x root
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cout << "blur_x"
                 << " " << x << " " << y << " " << endl;
            blur_x(x, y) = (static_cast<float>(input(x, y)) +
                            static_cast<float>(input(x + 1, y)) +
                            static_cast<float>(input(x + 2, y))) /
                           3.0f;
        }
    }
    // Compute blur_y root
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cout << "blur_y"
                 << " " << x << " " << y << " " << endl;
            blur_y(x, y) = static_cast<uint8_t>(
                               (blur_x(x, y) + blur_x(x, y + 1) + blur_x(x, y + 2)) / 3.0f);
        }
    }

    // ----------- till here ------------------------

    cout << "Completed boxSchedule1" << endl; // DO NOT CHANGE
    return blur_y;
}

Image<uint8_t> boxSchedule5(Image<uint8_t> input) {
    // // --------- HANDOUT  PS08 ------------------------------
    // Write the cpp nested loops corresponding to the 3x3 box schedule 5 in
    // tutorial 6 print the order of evaluation. Each time you perform a
    // computation of blur_x or blur_y, put a triplet with the name of the
    // Func ("blur_x" or "blur_y") and the output coordinates x and y. For
    // example, when you compute (50,75) print: blur_x 50 75 (with exactly one
    // space between each) in a new line.
    //
    // Do not print anything else using cout within the current function.

    cout << "Started boxSchedule5" << endl; // DO NOT CHANGE

    // Ignore boundaries: use this width and height
    // int w = input.width()-2;
    // int h = input.height()-2;

    // Intermediary stages of the pipeline, same size as input
    Image<float> blur_x(input.width(), input.height());
    Image<uint8_t> blur_y(input.width(), input.height());

    // - Schedule 5 to replicate -
    //   blur_y.compute_root()
    //   blur_x.compute_at(blur_y, x)
    // -------------------------

    // Your equivalent loops should go there --------
    // ----------- till here ------------------------

    cout << "Completed boxSchedule5" << endl; // DO NOT CHANGE
    return blur_y;
}

Image<uint8_t> boxSchedule6(Image<uint8_t> input) {
    // // --------- HANDOUT  PS08 ------------------------------
    // Write the cpp nested loops corresponding to the 3x3 box schedule 6 in
    // tutorial 6 print the order of evaluation. Each time you perform a
    // computation of blur_x or blur_y, put a triplet with the name of the
    // Func ("blur_x" or "blur_y") and the output coordinates x and y. For
    // example, when you compute (50,75) print:
    // blur_x 50 75 ( with exactly one space between each) in a new line.
    //
    // Do not print anything else using cout within the current function.

    cout << "Started boxSchedule6" << endl; // DO NOT CHANGE

    // Ignore boundaries: use this width and height
    // int w = input.width()-2;
    // int h = input.height()-2;

    // Intermediary stages of the pipeline, same size as input
    Image<float> blur_x(input.width(), input.height());
    Image<uint8_t> blur_y(input.width(), input.height());

    // - Schedule 6 to replicate -
    //   blur_y.tile(x, y, xo, yo, xi, yi, 2, 2)
    //   blur_x.compute_at(blur_y, yo)
    // -------------------------

    // Your equivalent loops should go there --------
    // ----------- till here ------------------------

    cout << "Completed boxSchedule6" << endl; // DO NOT CHANGE
    return blur_y;
}

Image<uint8_t> boxSchedule7(Image<uint8_t> input) {
    // // --------- HANDOUT  PS08 ------------------------------
    // Write the cpp nested loops corresponding to the 3x3 box schedule 7 in
    // tutorial 6 print the order of evaluation. Each time you perform a
    // computation of blur_x or blur_y, put a triplet with the name of the
    // Func ("blur_x" or "blur_y") and the output coordinates x and y. For
    // example, when you compute (50,75) print:
    // blur_x 50 75 ( with exactly one space between each) in a new line.
    //
    // Do not print anything else using cout within the current function.

    cout << "Started boxSchedule7" << endl; // DO NOT CHANGE

    // Ignore boundaries: use this width and height
    // int w = input.width()-2;
    // int h = input.height()-2;

    // Intermediary stages of the pipeline, same size as input
    Image<float> blur_x(input.width(), input.height());
    Image<uint8_t> blur_y(input.width(), input.height());

    // - Schedule 7 to replicate -
    //   blur_y.split(x, xo, xi, 2)
    //   blur_x.compute_at(blur_y, y)
    // -------------------------

    // Your equivalent loops should go there --------
    // ----------- till here ------------------------

    cout << "Completed boxSchedule7" << endl; // DO NOT CHANGE
    return blur_y;
}

// ---------------------------------- PART 2 -----------------------------------
// For all the problems in Part 2, yo are expected to write schedules. You will
// score points for faster schedules. You should also report the machine used
// for testing in the form accompanying this Pset. Each problem is supposed to
// return a Halide Func that computes the result in uint8_t. All intermediate
// operations should be performed in float for Gaussian and unsharp mask.
//
// First get the algorithm right and make sure it is generating the
// correct output. Use apply_compute_root() during this phase. Once you
// are confident of your result, remove apply_compute_root() and write
// your own schedule. You can set the macro APPLY_COMPUTE_ROOT to 0 or 1 to
// easily alternate between the default and updated schedules.
//
// Hint: your schedule should beat apply_compute_root()

Func Gaussian(Image<uint8_t> input) {
    // // --------- HANDOUT  PS08 ------------------------------
    // Perform 2D gaussian blur with binomial weights:
    // 1/16 * (1, 4, 6, 4, 1).
    //
    // INPUT: single channel luminance image
    //
    // OUTPUT: Func that computes Gaussian blur

    Func GaussianX("GaussianX");
    Func GaussianY("GaussianY");

    Var x("x"), y("y");
    Var xi("xi"), yi("yi");
    Var xo("xo"), yo("yo");

    /*
    NOTE: This first applies a blur on the horizontal
    dimension and then a blur on the vertical direction.
    */

    // Clamp the image (boundary conditions)
    Func clamped;
    clamped(x, y) = cast<float>(input(clamp(x, 0, input.width() - 1), clamp(y, 0, input.height() - 1)));

    int radius = 5 / 2;
    // Blur in x
    GaussianX(x, y) = (
                          clamped(x + 0 - radius, y) * 1.f +
                          clamped(x + 1 - radius, y) * 4.f +
                          clamped(x + 2 - radius, y) * 6.f +
                          clamped(x + 3 - radius, y) * 4.f +
                          clamped(x + 4 - radius, y) * 1.f
                      ) / 16.0f;
    // Blur in y
    GaussianY(x, y) = cast<uint8_t>((
                                        GaussianX(x, y + 0 - radius) * 1.f +
                                        GaussianX(x, y + 1 - radius) * 4.f +
                                        GaussianX(x, y + 2 - radius) * 6.f +
                                        GaussianX(x, y + 3 - radius) * 4.f +
                                        GaussianX(x, y + 4 - radius) * 1.f
                                    ) / 16.0f);
    // Schedule your pipeline

    /*
    RESULTS TABLE
    i7-7820HQ 2.9Ghz, 16GB 2133Mhz DDR3, 512GB NVME 1.7GB/s
    inline:
      - runtime: 70.7 ms
      - throughput: 57.9349 megapixels/sec
    root:
      - runtime: 22.8 ms
      - throughput: 179.649 megapixels/sec
    tiling:
      - runtime: 22.4 ms
      - throughput: 182.857 megapixels/sec
    tiling + parallel + vectorization:
      - runtime: 1.1 ms
      - throughput: 3723.64 megapixels/sec
    */

    // Debug to html

    // Return the output Func (cast it to uint8_t)

#define APPLY_COMPUTE_ROOT 0
#if APPLY_COMPUTE_ROOT
    GaussianX.compute_root();
    GaussianY.compute_root();
#else
    // Tiling only
    GaussianY.tile(x, y, xo, yo, xi, yi, 64, 32);
    GaussianX.compute_at(GaussianY, xo);
    // UNCOMMENT 373 TO USE ROOT SCHEDULE
    // GaussianX.compute_root();
    // Parallel + vectorizing
    GaussianY.parallel(yo);
    GaussianX.vectorize(x, 16);
    GaussianY.vectorize(xi, 16);
#endif
#undef APPLY_COMPUTE_ROOT

    // Debug HTML
    // Buffer<uint8_t> b(input.height(),input.width());
    // GaussianY.compile_to_lowered_stmt("Output/Gaussian.html", {b}, HTML);

    return GaussianY;
}
