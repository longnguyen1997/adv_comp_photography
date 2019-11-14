#include "a8.h"
#include <timing.h>

#include "reference_implementations.h"

int main(int argc, char **argv) {
  Image<uint8_t> input = load_image("Input/rgb.png");
  Image<uint8_t> lumi = load_image("Input/grey.png");
  Image<uint8_t> out0 = Image<uint8_t>(lumi.width(), lumi.height());

  // Part 1
  {
    Image<uint8_t> out1 = SmoothGradNormalized();
    Image<uint8_t> out2 = WavyRGB();
    Image<uint8_t> out3 = Luminance(input);
    Image<uint8_t> out4 = Sobel(lumi);

    if (out1.defined()) {
      save_image(out1, "Output/Smooth.png");
    } else {
      save_image(out0, "Output/Smooth.png");
    }
    if (out2.defined()) {
      save_image(out2, "Output/WavyRGB.png");
    } else {
      save_image(out0, "Output/WavyRGB.png");
    }
    if (out3.defined()) {
      save_image(out3, "Output/Luminance.png");
    } else {
      save_image(out0, "Output/Luminance.png");
    }
    if (out4.defined()) {
      save_image(out4, "Output/Sobel.png");
    } else {
      save_image(out0, "Output/Sobel.png");
    }

    // Test image
    Image<uint8_t> tiny(8, 8);
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        tiny(i, j) = 0;
      }
    }
    tiny(3, 3) = 255;
    tiny(4, 3) = 255;
    tiny(4, 4) = 255;

    save_image(tiny, "Output/original.png");

    // Example cpp-equivalent schedule
    Image<uint8_t> tiny_schedule_1 = boxSchedule1(tiny);
    save_image(tiny_schedule_1, "Output/boxblur_schedule1.png");

    // Your turn !
    Image<uint8_t> tiny_schedule_5 = boxSchedule5(tiny);
    save_image(tiny_schedule_5, "Output/boxblur_schedule5.png");
    Image<uint8_t> tiny_schedule_6 = boxSchedule6(tiny);
    save_image(tiny_schedule_6, "Output/boxblur_schedule6.png");
    Image<uint8_t> tiny_schedule_7 = boxSchedule7(tiny);
    save_image(tiny_schedule_7, "Output/boxblur_schedule7.png");
    return 0;
  }

  // Part 2
  {
    Image<uint8_t> out;

    int w = lumi.width();
    int h = lumi.height();

    Image<uint8_t> out_ref = Gaussian_cpp(lumi);
    save_image(out_ref, "Output/Gaussian_cpp_reference.png");
    Func r = Gaussian(lumi);
    if (r.defined()) {
      out = r.realize(w, h);
      cerr << "r ";
      profile(r, w, h);
    }
    if (out.defined()) {
      save_image(out, "Output/Gaussian.png");
    } else {
      save_image(out0, "Output/Gaussian.png");
    }
  }

  return EXIT_SUCCESS;
}
