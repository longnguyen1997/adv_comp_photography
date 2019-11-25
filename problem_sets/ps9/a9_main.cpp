#include "a9.h"
#include "timing.h"

#include "reference_implementations.h"

int main(int argc, char **argv) {

  Image<uint8_t> input = load_image("Input/grey-small.png");
  int w = input.width(), h = input.height();

  save_image(input, "Output/Original.png");

  // // Gaussian_horizontal
  // cout << "Testing Gaussian_horizonal" << endl;
  // {
  //   Image<uint8_t> out_ref;
  //   Image<uint8_t> out;
  //   // ------- C++ baseline -------------------------------------
  //   // unsigned long s = millisecond_timer();
  //   // for (int i = 0; i < N_TIMES; i++) {
  //   //   out_ref = Gaussian_horizontal_cpp(input, 3.0f);
  //   // }
  //   // float total_time = float(millisecond_timer() - s);
  //   // float mpixels = float(w * h) / 1e6;
  //   // cout << endl
  //   //      << "Reference implementation" << endl
  //   //      << "------------------------" << endl
  //   //      << "  - runtime " << total_time / N_TIMES << " ms " << endl
  //   //      << "  - throughput " << (mpixels * N_TIMES) / (total_time / 1000)
  //   //      << " megapixels/sec" << endl;
  //   // save_image(out_ref, "Output/Gaussian_horizontal_cpp.png");
  //   // ------- Halide version -----------------------------------
  //   // Func gaussian_hori_halide = Gaussian_horizontal(input, 3.0f);
  //   // cout<< endl
  //   //     << "Halide implementation" << endl
  //   //     << "---------------------" << endl;
  //   // profile(gaussian_hori_halide, w, h);
  //   // out = gaussian_hori_halide.realize(w, h);
  //   // save_image(out, "Output/Gaussian_horizontal_halide.png");
  //   // ----------------------------------------------------------
  // }

  // // Gaussian
  // cout << "Testing Gaussian" << endl;
  // {
  //   Image<uint8_t> out_ref;
  //   Image<uint8_t> out;
  //   // ------- C++ baseline -------------------------------------
  //   // unsigned long s = millisecond_timer();
  //   // for (int i = 0; i < N_TIMES; i++) {
  //   //   out_ref = Gaussian_cpp(input, 3.0f);
  //   // }
  //   // float total_time = float(millisecond_timer() - s);
  //   // float mpixels = float(w * h) / 1e6;
  //   // cout << endl
  //   //      << "Reference implementation" << endl
  //   //      << "------------------------" << endl
  //   //      << "  - runtime " << total_time / N_TIMES << " ms " << endl
  //   //      << "  - throughput " << (mpixels * N_TIMES) / (total_time / 1000)
  //   //      << " megapixels/sec" << endl;
  //   // save_image(out_ref, "Output/Gaussian_cpp.png");
  //   // ------- Halide version -----------------------------------
  //   // Func gaussian_halide = Gaussian(input, 3.0f);
  //   // cout<< endl
  //   //     << "Halide implementation" << endl
  //   //     << "---------------------" << endl;
  //   // profile(gaussian_halide, w, h);
  //   // out = gaussian_halide.realize(w, h);
  //   // save_image(out, "Output/Gaussian_halide.png");
  //   // ----------------------------------------------------------
  // }

  // UnsharpMask
  cout << "Testing UnsharpMask" << endl;
  {
    Image<uint8_t> out_ref;
    Image<uint8_t> out;
    // ------- C++ baseline -------------------------------------
    unsigned long s = millisecond_timer();
    for (int i = 0; i < N_TIMES; i++) {
      out_ref = UnsharpMask_cpp(input, 3.0f);
    }
    float total_time = float(millisecond_timer() - s);
    float mpixels = float(w * h) / 1e6;
    cout << endl
         << "Reference implementation" << endl
         << "------------------------" << endl
         << "  - runtime " << total_time / N_TIMES << " ms " << endl
         << "  - throughput " << (mpixels * N_TIMES) / (total_time / 1000)
         << " megapixels/sec" << endl;
    save_image(out_ref, "Output/UnsharpMask_cpp.png");
    // ------- Halide version -----------------------------------
    vector<int> schedule_args({});
    Func unsharp_mask_halide = UnsharpMask(input, 0, schedule_args, 3.0f);
    cout<< endl
        << "Halide implementation" << endl
        << "---------------------" << endl;
    profile(unsharp_mask_halide, w, h);
    out = unsharp_mask_halide.realize(w, h);
    save_image(out, "Output/UnsharpMask_halide.png");
    // ------- 6.865 autoscheduled version ----------------------
    // autoschedule_unsharp_mask(input);
    // ----------------------------------------------------------
  }

  return EXIT_SUCCESS;
}
