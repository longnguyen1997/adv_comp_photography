/* -----------------------------------------------------------------
 * File:    a0.cpp
 * Created: 2015-09-09
 * Modified: 2017-09-05
 * -----------------------------------------------------------------
 *
 * Implementation file for PS00
 *
 * We define and implement the functions we declared in a0.h
 *
 * ---------------------------------------------------------------*/

// This command imports all the content of a0.h, thus declaring our functions.
#include "a0.h"

// We use the namespace "std" so that we can write the command "cout" instead
// of the verbose version "std::cout".
using namespace std;

// Return the sum of two numbers
float getSum(float a, float b) {
  // --------- HANDOUT  PS00 ------------------------------
  // Create a variable c that is the sum of a and b
  float c = a + b;
  // Return it
  return c;
}

// Print the sum of two numbers to the screen
void helloworld(float a, float b) {
  // --------- HANDOUT  PS00 ------------------------------
  // Create a variable c that is the sum of a and b
  float c = getSum(a, b);
  // Use cout to display the values
  cout << "Hello World!" << endl;
  cout << "The value of a is " << a << "." << endl;
  cout << "The value of b is " << b << "." << endl;
  cout << "The sum of a and b is " << c << "." << endl;
}

// Create an image and return it
// You can test your function by calling my_image.write(filename) to write the
// output somewhere
Image readAnImage(const std::string &filename) {
  // --------- HANDOUT  PS00 ------------------------------
  // Use the constructor in Image.h
  return Image(1, 1, 1); // Change this
}

// Create an empty image of size width x height
SimpleImage::SimpleImage(int width, int height){
  // --------- HANDOUT  PS00 ------------------------------
  // Make sure keep around all necessary data
  width_ = width;
  height_ = height;
  // Initialize the image to empty (0.0f)
  for (int i = 0; i < width * height; i++) {
    img_.push_back(0.0f);
  }
  // ASSERT STATEMENTS AND SANITY CHECKS BELOW.
  /*
  assert(img_.size() == width * height, "Image array isn't the right size!");
  cout << "Size of image array is " << img_.size() << '.' << endl;
  for (int i = 0; i < width * height; i++) {
    cout << img_[i] << endl;
  }
  */
};

// Computes the average pixel value of the image
float SimpleImage::compute_average() const {
  // --------- HANDOUT  PS00 ------------------------------
  // compute the average
  // return the average
  return 0.0f; // Change this
}

bool point_overlaps_rectangle(int n, int h, int w, float left_boundary, float right_boundary,
                        float top_boundary, float bottom_boundary) {
  // Get the pixel's x-y coordinates from the flattened point index.
  const int x = n % w;
  const int y = n / w;
  /*
  Note the < condition for the bottom and right boundaries.
  By definition, inclusive means up to, but not including,
  the next index/integer.
  */
  if (y >= floor(top_boundary) && y < ceil(bottom_boundary)
    && x >= floor(left_boundary) && x < ceil(right_boundary)) {
    return true;
  }
  return false;
}

// Create a white (1.0f) rectangle in the center of the image, taking up
// 10% of the image in each direction
void SimpleImage::make_rectangle() {

  /*
  PSEUDOCODE
  generate boundary rectangle, excluding 0, width, and height
  for each pixel n in the 1D pixel array:
    x = n / width
    y = n % width
    if x and y are within the boundary rectangle:
      set pixel n to white 
  */

  // --------- HANDOUT  PS00 ------------------------------
  // Denote white as 1.0f
  const float white = 1.0f;
  // Let’s make the rectangle be 20% of the 
  // width (and height respectively); within 10%
  // (inclusive) of the image center in each direction.
  const float rectangle_radius = 0.1;
  const float center_x = width_ / 2.0;
  const float center_y = height_ / 2.0;
  const float left_boundary = -(rectangle_radius * width_) + center_x;
  const float right_boundary = (rectangle_radius * width_) + center_x;
  const float top_boundary = -(rectangle_radius * height_) + center_y;
  const float bottom_boundary = (rectangle_radius * height_) + center_y;
  // Make sure rectangle is centered in the image and includes the center
  for (int pixel = 0; pixel < img_.size(); pixel++) {
    if (point_overlaps_rectangle(pixel, height_, width_,
                                left_boundary, right_boundary, 
                                top_boundary, bottom_boundary)) {
      img_[pixel] = white;
    }
  }
}

// Print the image to the terminal.
// denote black (0.0f) as '-' and white (1.0) as '+'
void SimpleImage::print_to_terminal() const {
  // --------- HANDOUT  PS00 ------------------------------
  // print the image as ASCII characters
  for (int pixel = 0; pixel < img_.size(); pixel++) {
    if (pixel % width_ == 0 && pixel != 0) cout << endl;
    if (img_[pixel] == 0.0f) {
      cout << "-";
    } else {
      cout << "+";
    }
  }
  cout << endl;
}
