#include "a1.h"
#include <iostream>
#include <cassert>

using namespace std;

void check_bounds(Image &image, int index)
{
    try
    {
        image(index);
    }
    catch (OutOfBoundsException &)
    {
        cout << "Illegal access to " << index << '.' << endl;
    }
}

void check_bounds(Image &image, int x, int y, int z)
{
    try
    {
        image(x, y, z);
    }
    catch (OutOfBoundsException &)
    {
        cout << "Illegal access." << endl;
    }
}

void check_bounds(Image &image, int x, int y)
{
    try
    {
        image(x, y);
    }
    catch (OutOfBoundsException &)
    {
        cout << "Illegal access." << endl;
    }
}

// This is a way for you to test your functions.
// We will only grade the contents of a1.cpp and Image.cpp
int main()
{

    // Example for tests.
    Image im("./Input/castle_small.png");

    // 1.a
    assert(im.number_of_elements() == im.height() * im.width() * im.channels());
    cout << "Passed 1.a." << endl;

    // 1.b
    for (int i = 0; i < im.number_of_elements(); i++)
    {
        im(i);
    }
    check_bounds(im, im.number_of_elements());
    check_bounds(im, -1);

    cout << "Passed 1.b." << endl;

    // 1.c
    im(im.width() - 1, im.height() - 1, im.channels() - 1);
    check_bounds(im, im.width(), im.height(), im.channels());
    check_bounds(im, -1, -1, 0);

    cout << "Passed 1.c." << endl;

    check_bounds(im, im.width(), im.height());
    im(im.width() - 1, im.height() - 1);

    cout << "Passed 1.d." << endl;

    Image im2(100, 100, 3);
    im2.set_color(168.0 / 255, 50.0 / 255, 80.0 / 255);
    im2.write("./Output/pink.png");

    cout << "Passed 2.a. See pink.png." << endl;

    im2.create_rectangle(1, 1, 49, 49, 122 / 255.0, 9 / 255.0, 37 / 255.0);
    im2.write("./Output/pink_and_rect.png");

    cout << "Passed 2.b. See pink_and_rect.png." << endl;

    const Image special_image = create_special();
    special_image.write("./Output/special_img.png");

    cout << "Passed 2.c/d. Verify that special_img.png is MIT." << endl;

    im.create_line(10, 10, 10, 150, 122 / 255.0, 9 / 255.0, 37 / 255.0);
    im.write("./Output/vertical_line.png");
    im2.create_line(10, 50, 80, 50, 122 / 255.0, 9 / 255.0, 37 / 255.0);
    im2.write("./Output/horizontal_line.png");

    cout << "Passed 2.e. Verify {horizontal, vertical}_line.png." << endl;

    Image boston_underexposed = Image("./Input/Boston_underexposed.png");
    Image boston_corrected_exposure = brightness(boston_underexposed, 2.0f);
    boston_corrected_exposure.write("./Output/boston_exposure.png");
    // std::vector<Image> LC = lumiChromi(im);
    // LC[0].write("./Output/castle_luminance.png");
    // LC[1].write("./Output/castle_chrominance.png");
}
