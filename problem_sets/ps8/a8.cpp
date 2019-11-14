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

  return Image<uint8_t>(1, 1);
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
  return Image<uint8_t>(1, 1, 1);
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
  return Image<uint8_t>(1, 1);
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

  return Image<uint8_t>(1, 1);
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

// Clamp the image (boundary conditions)

// Blur in x
// Blur in y

// Schedule your pipeline

// Debug to html

// Return the output Func (cast it to uint_8t)

#define APPLY_COMPUTE_ROOT 0
#if APPLY_COMPUTE_ROOT
  GaussianX.compute_root();
  GaussianY.compute_root();
#else
// Your schedule
#endif
#undef APPLY_COMPUTE_ROOT

  // Debug HTML
  // Buffer<uint8_t> b(input.height(),input.width());
  // GaussianY.compile_to_lowered_stmt("Output/Gaussian.html", {b}, HTML);

  return GaussianY;
}
