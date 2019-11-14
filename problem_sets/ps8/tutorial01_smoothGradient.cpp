// Halide tutorial lesson 1.

// This lesson demonstrates basic usage of Halide as a JIT compiler for imaging.

// In this tutorial we will create a function that has no input and that outputs
// a greyscale image representing a smooth gradient where the intensity of a
// pixel at x,y is equal to x+y

// Halide relies on metaprogramming, which means that it is a language within a
// language, in our case within C++. Creating a Halide program involves creating
// C++ objects that represent the Halide program, then calling the Halide
// compiler from C++ to turn these objects into executable binaries. and then
// actually executing the binary. This explains some of the quirks of the
// syntax. For example we need to declare functions and variables before
// defining them or using them, in order to create the Halide representation.

#include <Halide.h>
#include <cmath>
#include <image_io.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace Halide;
using namespace Halide::Tools;

int main(int argc, char **argv) {
  // This program defines a single-stage imaging pipeline that outputs a
  // grayscale diagonal gradient.

  // A 'Func' object represents a pipeline stage. It's a pure function that
  // defines what value each pixel should have. You can think of it as a
  // computed image.

  // Unlike C++, Halide requires that you declare functions and variable
  // before defining or using them. First, we declare the existence of a Func
  // called gradient. We'll define it later
  Func gradient;

  // Var objects are names to use as variables in the definition of a Func.
  // They have no meaning by themselves. Again, we declare them first. (The
  // string in parenthesis is the name that will be used for this variable
  // when Halide prints debugging information)
  Var x("x");
  Var y("y");

  // Funcs are defined at any integer coordinate of their variables as
  // a Halide expression (called Expr) in terms of those variables and other
  // functions. Here, we'll define an Expr which has the value x + y. Vars
  // have appropriate operator overloading so that expressions like 'x + y'
  // implicitly become 'Expr' objects.
  Expr e = x + y;

  // Halide does type inference. Var objects represent 32-bit integers, so the
  // Expr object 'x + y' also represents a 32-bit integer.
  // This is why we cast this expressioninto a float.
  e = cast<float>(e);

  // Now we'll add a definition for the Func object. At pixel x, y, the image
  // will have the value of the Expr e. On the left hand side we have the Func
  // we're defining and some Vars representing the domain. On the right hand
  // side we have some Expr object that uses those same Vars. Note the use of
  // square brackets, just a syntax quirk
  gradient(x, y) = e;

  // The last three lines are the same as writing:
  //
  //   gradient(x, y) = cast(Float(32), x + y)
  //
  // which is the more common form, but we are showing the intermediate Expr
  // here for completeness.

  // That line of code defined the Func, but it didn't actually compute the
  // output image yet. At this stage it's just Funcs, Exprs, and Vars in
  // memory, representing the structure of our imaging pipeline. We're
  // meta-programming. This C++ program is constructing a Halide program in
  // memory (more or less an abstract syntax tree). The actual computation of
  // pixel data comes next.

  // Now we 'realize' the Func, which JIT (just in time) compiles some code
  // that implements the pipeline we've defined, and then runs it. We also
  // need to tell Halide the domain over which to evaluate the Func, which
  // determines the range of x and y above, and the resolution of the output
  // image. We'll make a 512 x 512 image. By default, realize computes from
  // 0,0 to the provided width and height In general, realize needs one size
  // for each dimension (each Var) of your Func
  Image<float> output = gradient.realize(512, 512);

  // realize provides us with some Halide internal datatype representing image
  // buffers. Let's check everything worked, and we got the output we were
  // expecting. Let's use regular C++ for this.

  for (int j = 0; j < output.height(); j++) {
    for (int i = 0; i < output.width(); i++) {
      // We can access a pixel of an Image object using similar
      // syntax to defining and using functions.
      if (output(i, j) != i + j) {
        cout << "Something went wrong at (" << i << "," << j << ") "
             << output(i, j) << endl;
        return EXIT_FAILURE;
      }
    }
  }

  // Let's write the image to disk module dividing by 1024 to normalize
  for (int j = 0; j < output.height(); j++) {
    for (int i = 0; i < output.width(); i++) {
      output(i, j) /= 1024.0f;
    }
  }

  save_image(output, "Output/tut1_result.png");

  // Everything worked! We defined a Func, then called 'realize' on
  // it to generate and run machine code that produced an Image.
  cout << "Success!\n" << endl;

  return EXIT_SUCCESS;
}
