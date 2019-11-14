#include "a8.h"

// Reference implementation for separable Gaussian
Image<uint8_t> Gaussian_cpp(Image<uint8_t> input) {

  // Compute the kernel
  std::vector<float> kernel(
      {1.f / 16.f, 4.f / 16.f, 6.f / 16.f, 4.f / 16.f, 1.f / 16.f});
  int fwidth = kernel.size();
  int radius = fwidth / 2;
  std::cout << radius << "  " << fwidth << endl;

  int height = input.height();
  int width = input.width();
  Image<float> blur_x(width, height);
  Image<uint8_t> output(width, height);

  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      blur_x(x, y) = 0;
      output(x, y) = 0;
    }

  // Blur horizontal
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      for (int i = 0; i < fwidth; ++i) {
        if (x + i < radius || x + i >= width) {
          continue;
        }
        blur_x(x, y) +=
            static_cast<float>(input(x + i - radius, y)) * kernel[i];
      }
    }

  // Blur vertical
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      float accum = 0.0f;
      for (int i = 0; i < fwidth; ++i) {
        if (y + i < radius || y + i >= height) {
          continue;
        }
        accum += blur_x(x, y + i - radius) * kernel[i];
      }
      output(x, y) = static_cast<uint8_t>(accum);
    }

  return output;
}
