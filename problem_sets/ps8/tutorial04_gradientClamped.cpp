// Halide tutorial lesson 4.

// This lessons demonstrates clamping to deal with boundary pixels for stencil
// operations. When using neighboring pixel at the border of the image, we might
// use indices that are negative or beyond the image size and would return an
// error. We will simply clamp the coordinates.

// This tutorial is a minor modification of the previous one.
// We will compute The magnitude of the gradient of an image, but the output
// will be the same size as the input

#include <Halide.h>
#include <cmath>
#include <image_io.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace Halide;
using namespace Halide::Tools;

int main(int argc, char **argv) {

  // This program defines a multi-stage Halide imaging pipeline. One stage
  // computes the horixontal gradient of an image dI/dx. Another stage
  // computes dI/dy (for all three channels of RGB in both cases). The final
  // stage computes the magnitude of the corresponding vector

  // We will compute the gradient with finite differences: dI/dx=I(x+1)-I(x)

  // As usual, let's load an input
  Image<uint8_t> im = load_image("Input/rgb.png");

  // Next we declaure the Vars
  // We here give an extra argument to the Var constructor, an optional string
  // that can help for debugging by naming the variable in the Halide
  // representation. Otherwise, the names x, y, c are only known to the C++
  // side
  Var x("x"), y("y"), c("c");

  // Next we declare the three Funcs corresponding to the various stages of
  // the gradient. Similarly, we pass strings to name them.
  Func gx = Func("gx");
  Func gy = Func("gy");
  Func gradientMagnitude("gradientMagnitude");

  // convert the image to float
  Func input("Input");
  input(x, y, c) = cast<float>(im(x, y, c));

  ////////// NEW CODE ////////

  // In addition to these stages, we need a clamping Func that can deal with
  // x, y, locations outside the original image. For this we use the function
  // clamp(x, min, max) which returns x if it's in the interval, min if it's
  // below and max if it's above. In terms of images, it returns the pixel on
  // the dge nearest to x, y
  Func clamped("clamped");
  clamped(x, y, c) =
      input(clamp(x, 0, im.width() - 1), clamp(y, 0, im.height() - 1), c);

  ////////// MODIFIED CODE ////////

  // We can now safely define our horizontal gradient using finite difference
  // The value at a pixel is the input at that pixel minus its left neighbor.
  // Note how we now use the more direct definition of Funcs without declaring
  // intermediate Exprs
  gx(x, y, c) = clamped(x, y, c) - clamped(x - 1, y, c);

  // Similarly define the vertical gradient.
  gy(x, y, c) = clamped(x, y, c) - clamped(x, y - 1, c);

  // Finally define the gradient magnitude as the Euclidean norm of the
  // gradient vector. We use overloaded operators and functions such as **, +
  // and sqrt. Through he magic of metaprogramming, this creates the
  // appropriate algebraic tree in Halide representation. Most operators and
  // functions you expect are supported. Check the documentation for the full
  // list.
  gradientMagnitude(x, y, c) = cast<uint8_t>(
      sqrt(gx(x, y, c) * gx(x, y, c) + gy(x, y, c) * gy(x, y, c)));

  // As usual, all we have done so far is create a Halide internal
  // representation. No computation has happened yet. We now call realize() to
  // compile and execute.
  Image<uint8_t> output =
      gradientMagnitude.realize(im.width(), im.height(), im.channels());

  save_image(output, "Output/tut4_gradientMagnitudeClamped.png");

  cout << "Success!\n" << endl;

  return EXIT_SUCCESS;
}

// exercises
// compute a Sobel gradient
// don't try to be too smart, brute-force it.
// or you can be smart and meta-program it
