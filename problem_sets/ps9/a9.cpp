#include "a9.h"
#include "timing.h"
#include <algorithm>

using namespace Halide;

Func Gaussian_horizontal(Image<uint8_t> input, float sigma, float truncate) {
    // --------- HANDOUT  PS09 ------------------------------
    // Perform 1D horizontal Gaussian blur. The Gaussians have variance
    // sigma and the kernels are truncated at truncate*sigma on both sides.
    // We give a reference implementation in reference_implementation.h.
    //
    // Make sure to generate the kernel in Halide.
    // Use the sum() sugar (see tutorials) for kernel normalization
    //
    // INPUT: single channel luminance image
    // OUTPUT: Func that computes a horizontal Gaussian blur

    Func GX("Gaussian_horizontal");

    Var x("x"), y("y");

    // We give you the Halide starter code for computing the kernel
    // You will need to schedule it yourself
    int radius = sigma * truncate;
    int fwidth = 2 * radius + 1;

    // compute the Gaussian kernel
    Func GKernelUnNorm("GKernelUnnorm");
    Func GKernelSum("GKernelSum");
    Func GKernel("GKernel");
    RDom rx(0, fwidth);
    GKernelUnNorm(x) =
        exp(-((x - radius) * (x - radius)) / (2.0f * sigma * sigma));
    GKernelSum(x) = sum(GKernelUnNorm(rx));
    GKernel(x) = GKernelUnNorm(x) / GKernelSum(0);

    // Clamp the image (boundary conditions)
    Func clamped;
    clamped(x, y) = cast<float>(input(
                                    clamp(x, 0, input.width() - 1),
                                    clamp(y, 0, input.height() - 1))
                               );
    // Blur in x
    int kernelWidth = fwidth;
    Func X;
    X(x, y) = sum(cast<uint8_t>(clamped(x + rx - radius, y) * GKernel(rx)));
    GX(x, y) = X(x, y);
    // Schedule your pipeline
    GKernelUnNorm.compute_root();
    GKernelSum   .compute_root();
    GKernel      .compute_root();
    X.compute_at(GX, y).parallel(y).vectorize(x, 16);
    // Debug to html
    // Buffer<uint8_t> b(input.height(), input.width());
    // GX.compile_to_lowered_stmt("Output/Gaussian_hori.html", {b}, HTML);
    // Return the output Func (cast it to uint8_t)
    return GX;
}

Func Gaussian(Image<uint8_t> input, float sigma, float truncate) {
    // --------- HANDOUT  PS09 ------------------------------
    // Perform 2D separable Gaussian blur. The Gaussians have variance
    // sigma and the kernels are truncated at truncate*sigma on both sides.
    // We give a reference implementation in reference_implementation.h.
    //
    // Make sure to generate the kernel in Halide.
    // Use the sum() sugar (see tutorials) for kernel normalization
    //
    // INPUT: single channel luminance image
    // OUTPUT: Func that computes Gaussian blur

    Func GB("Gaussian");

    Var x("x"), y("y");
    Var xi("xi"), yi("yi");
    Var xo("xo"), yo("yo");

    // We give you the Halide starter code for computing the kernel
    // You will need to schedule it yourself
    int radius = sigma * truncate;
    int fwidth = 2 * radius + 1;

    // Compute the kernel
    Func GKernelUnNorm("GKernelUnnorm");
    Func GKernelSum("GKernelSum");
    Func GKernel("GKernel");
    RDom rx(0, fwidth);
    GKernelUnNorm(x) =
        exp(-((x - radius) * (x - radius)) / (2.0f * sigma * sigma));
    GKernelSum(x) = sum(GKernelUnNorm(rx));
    GKernel(x) = GKernelUnNorm(x) / GKernelSum(0);
    // Schedule the kernel
    GKernelUnNorm.compute_root();
    GKernelSum   .compute_root();
    GKernel      .compute_root();
    // Clamp the image (boundary conditions)
    Func clamped;
    clamped(x, y) = cast<float>(input(
                                    clamp(x, 0, input.width() - 1),
                                    clamp(y, 0, input.height() - 1))
                               );
    // Blur in x
    int kernelWidth = fwidth;
    Func X;
    X(x, y) = sum(clamped(x + rx - radius, y) * GKernel(rx));
    // Blur in y
    Func Y;
    Y(x, y) = sum(X(x, y + rx - radius) * GKernel(rx));
    GB(x, y) = cast<uint8_t>(Y(x, y));
    // Schedule your pipeline
    GB.tile(x, y, xo, yo, xi, yi, 256, 32).parallel(yo).vectorize(xi, 32);
    Y.compute_at(GB, xo);
    X.compute_at(GB, yo).vectorize(x, 16);
    // Debug to html
    // Buffer<uint8_t> b(input.height(), input.width());
    // GB.compile_to_lowered_stmt("Output/Gaussian.html", {b}, HTML);
    // Return the output Func (cast it to uint8_t)
    return GB;
}

Func UnsharpMask(Image<uint8_t> input, int schedule_index,
                 vector<int> schedule_parameters, float sigma, float truncate,
                 float gamma, float strength) {
    // --------- HANDOUT  PS09 ------------------------------
    // Implement Unsharp Mask as described in the handout pdf. By fusing
    // different stages of the processing pipeline and properly scheduling
    // your operations, you should be able to get a drastic speed-up over
    // the reference implementation given in 'reference_implementation.cpp'.
    //
    // Make sure you handle boundary conditions appropriately. You may use
    // the gaussian above for your blur but make sure to make a new schedule
    // for the Gaussian for better performance.
    //
    // Make sure you debug your intermediate Func (by realizing them over
    // some domain for example).
    //
    // Start by building a correct implementation of the algorithm and then
    // walk your way up to a fast implementation by trying various schedules.
    //
    // Return the output Func, properly scheduled. Check the reference
    // implementation to see how the input parameters are used.
    //
    // You can use schedule_index to switch between different schedules and
    // compare them. This will come in handy when you implement autotuner
    // (6.865). You also pass schedule parameters as a vector of ints.
    //
    // INPUT: single channel luminance image
    // OUTPUT: Halide Func that computes the UnsharpMask

    Var x, y, xi, yi, xo, yo;
    Func clamped, Gamma, ClampedGamma, X, Y, HiPass, Out;
    Func GKernelUnNorm, GKernelSum, GKernel;

    // Clamp the image (boundary conditions)
    clamped(x, y) = cast<float>(input(
                                    clamp(x, 0, input.width() - 1),
                                    clamp(y, 0, input.height() - 1)
                                )
                               );

    // Apply contrast-adjusted gamma correction to the image
    float factor = (1.f - 0.5f) / (1.f - pow(0.5f, 1.f / gamma));
    Gamma(x, y) = (pow(clamped(x, y) / 255.f, 1.f / gamma) - 1.f) * factor + 1.f;
    // Image<float> G = Gamma.realize(input.width(), input.height());
    // save_image(G, "Output/UnsharpMask_halide_gamma.png");

    // Apply Gaussian Blur to the adjusted image with
    // the specified sigma and truncate
    Image<float> GammaIm = Gamma.realize(input.width(), input.height());
    ClampedGamma(x, y) = cast<float>(GammaIm(
                                         clamp(x, 0, input.width() - 1),
                                         clamp(y, 0, input.height() - 1)
                                     )
                                    );
    int radius = sigma * truncate;
    int fwidth = 2 * radius + 1;
    RDom rx(0, fwidth);
    GKernelUnNorm(x) =
        exp(-((x - radius) * (x - radius)) / (2.0f * sigma * sigma));
    GKernelSum(x) = sum(GKernelUnNorm(rx));
    GKernel(x) = GKernelUnNorm(x) / GKernelSum(0);
    X(x, y) = sum(ClampedGamma(x + rx - radius, y) * GKernel(rx.x));
    // Image<float> unsharpBlurX = X.realize(input.width(), input.height());
    // save_image(unsharpBlurX, "Output/UnsharpMask_halide_blurX.png");
    Y(x, y) = sum(X(x, y + rx - radius) * GKernel(rx.x));
    // Image<float> unsharpBlurY = Y.realize(input.width(), input.height());
    // save_image(unsharpBlurY, "Output/UnsharpMask_halide_blurY.png");

    // // Construct the high-pass image with sigma and truncate
    HiPass(x, y) = Gamma(x, y) - Y(x, y);
    // Image<float> unsharpHipass = HiPass.realize(input.width(), input.height());
    // save_image(unsharpHipass, "Output/UnsharpMask_halide_hipass.png");

    // // Recombine the high-pass image times sharpening strength
    // // with the gamma-adjusted image from the very beginning.
    Out(x, y) = cast<uint8_t>(255.f * clamp((HiPass(x, y) * strength + Gamma(x, y)), 0.f, 1.f));

    // SCHEDULING
    GKernelUnNorm.compute_root();
    GKernelSum   .compute_root();
    GKernel      .compute_root();

    Out.tile(x, y, xo, yo, xi, yi, 256, 32)
    .parallel(yo)
    .vectorize(xi, 32);

    X.compute_root()
    .parallel(y)
    .vectorize(x, 32);

    clamped.compute_root()
    .parallel(y)
    .vectorize(x, 32);

    return Out;
}

// ----------------------------------------------------------
// This applies a compute_root() schedule to all the Func's
// that are consumed by the calling Func. DO NOT EDIT IT.
void apply_auto_schedule(Func F) {
    map<string, Internal::Function> flist =
        Internal::find_transitive_calls(F.function());
    flist.insert(std::make_pair(F.name(), F.function()));
    map<string, Internal::Function>::iterator fit;
    for (fit = flist.begin(); fit != flist.end(); fit++) {
        Func f(fit->second);
        f.compute_root();
        // cout << "Warning: applying default schedule to " << f.name() << endl;
    }
    cout << endl;
}
// ----------------------------------------------------------
void autoschedule_unsharp_mask(const Image<uint8_t> &input) {
    // --------- HANDOUT  PS09 ------------------------------
    // An example autotuner, that does not do much yet... Build your own !
    //

    cout << "** Autotuning begins **" << endl
         << "=======================" << endl;
    int w = input.width();
    int h = input.height();

    vector<float> timings;
    vector<int> schedule_idxs;
    vector<vector<int>> schedule_params;
    for (int idx = 0; idx < 2; ++idx) {
        vector<int> params;

        Func f = UnsharpMask(input, idx, params, 3.f);

        cout << endl << "---------------------------------------" << endl;
        cout << "* Autoschedule | " << idx << endl;

        cout << "  Params: ";
        for (int i = 0; i < (int)params.size(); ++i) {
            cout << params[i] << " ";
        }
        cout << endl;

        float time = profile(f, w, h);
        timings.push_back(time);
        schedule_idxs.push_back(idx);
        schedule_params.push_back(params);

        cout << endl << "---------------------------------------" << endl;
    }

    auto mini = std::min_element(timings.begin(), timings.end());
    int min_idx = mini - timings.begin();
    cout << endl
         << "==========================" << endl
         << "** Autotuning complete **" << endl
         << "   - best schedule: " << min_idx << endl
         << "   - time: " << timings[min_idx] << " ms" << endl
         << "   - params: ";
    for (int i = 0; i < (int)schedule_params[min_idx].size(); ++i) {
        cout << schedule_params[min_idx][i] << " ";
    }
    cout << endl;
}
