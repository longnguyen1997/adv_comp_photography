#include "a9.h"

Image<uint8_t> Gaussian_horizontal_cpp(Image<uint8_t> input, float sigma,
                                       float truncate) {
  int radius = sigma * truncate;
  int fwidth = 2 * radius + 1;

  // Precompute the kernel
  Image<float> kernel1D(fwidth);
  float s = 0.0f;
  for (int x = 0; x < fwidth; ++x) {
    kernel1D(x) = exp(-((x - radius) * (x - radius)) / (2.0f * sigma * sigma));
    s += kernel1D(x);
  }
  // normalize it
  for (int x = 0; x < fwidth; ++x) {
    kernel1D(x) /= s;
  }

  int height = input.height();
  int width = input.width();
  Image<uint8_t> output(width, height);

  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      output(x, y) = 0;
    }

  // Blur horizontal
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      for (int i = 0; i < fwidth; ++i) {
        if (x + i < radius || x + i >= width) {
          continue;
        }
        output(x, y) += static_cast<uint8_t>(
            static_cast<float>(input(x + i - radius, y)) * kernel1D(i));
      }
    }

  return output;
}

Image<uint8_t> Gaussian_cpp(Image<uint8_t> input, float sigma, float truncate) {
  int radius = sigma * truncate;
  int fwidth = 2 * radius + 1;

  // Precompute the kernel
  Image<float> kernel1D(fwidth);
  float s = 0.0f;
  for (int x = 0; x < fwidth; ++x) {
    kernel1D(x) = exp(-((x - radius) * (x - radius)) / (2.0f * sigma * sigma));
    s += kernel1D(x);
  }
  // normalize it
  for (int x = 0; x < fwidth; ++x) {
    kernel1D(x) /= s;
  }

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
            static_cast<float>(input(x + i - radius, y)) * kernel1D(i);
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
        accum += blur_x(x, y + i - radius) * kernel1D(i);
      }
      output(x, y) = static_cast<uint8_t>(accum);
    }

  return output;
}

float clip(float x, float lo, float hi) {
  return std::max(lo, std::min(x, hi));
}

Image<uint8_t> UnsharpMask_cpp(Image<uint8_t> input, float sigma,
                               float truncate, float gamma, float strength) {

  int height = input.height();
  int width = input.width();

  // ----- Gamma correct the image ------------------------------------
  Image<float> gamma_im(width, height);

  float factor = (1.f - 0.5f) / (1.f - pow(0.5, 1.f / gamma));

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float in_float = static_cast<float>(input(x, y)) / 255.f;
      gamma_im(x, y) = (pow(in_float, 1.f / gamma) - 1.f) * factor + 1.f;
    }
  }
  int radius = sigma * truncate;
  int fwidth = 2 * radius + 1;

  // ----- Get blurred image ------------------------------------------
  Image<float> kernel1D(fwidth);
  float s = 0.0f;
  for (int x = 0; x < fwidth; ++x) {
    kernel1D(x) = exp(-((x - radius) * (x - radius)) / (2.0f * sigma * sigma));
    s += kernel1D(x);
  }
  // normalize it
  for (int x = 0; x < fwidth; ++x) {
    kernel1D(x) /= s;
  }

  Image<float> blur_x(width, height);
  Image<float> blur_y(width, height);

  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      blur_x(x, y) = 0.f;
      blur_y(x, y) = 0.f;
    }

  // Blur horizontal
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      for (int i = 0; i < fwidth; ++i) {
        if (x + i < radius || x + i >= width) {
          continue;
        }
        blur_x(x, y) += gamma_im(x + i - radius, y) * kernel1D(i);
      }
    }
  }

  // Blur vertical
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      for (int i = 0; i < fwidth; ++i) {
        if (y + i < radius || y + i >= height) {
          continue;
        }
        blur_y(x, y) += blur_x(x, y + i - radius) * kernel1D(i);
      }
    }
  }
  // ----- Construct hi-pass image ------------------------------------
  Image<float> hi_pass(width, height);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      hi_pass(x, y) = gamma_im(x, y) - blur_y(x, y);
    }
  }
  // ----- Reconstruct sharpened image --------------------------------
  Image<uint8_t> output(width, height);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float output_float = hi_pass(x, y) * strength + gamma_im(x, y);
      output(x, y) = static_cast<uint8_t>(clip(output_float, 0.f, 1.f) * 255.f);
    }
  }

  return output;
}
