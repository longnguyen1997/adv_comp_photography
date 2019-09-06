#include "a0.h"

using namespace std;

// This is a way for you to test your functions.
// We will only grade the contents of a0.cpp and Image.cpp
int main() {
  // --------- HANDOUT  PS00 ------------------------------
  cout << "Congratulations, you have compiled the starter code!" << endl;
  printf("\n");
  
  helloworld(5.9, 6.8);
  
  printf("\n");

  SimpleImage img = SimpleImage(5, 5);
  // cout << "Size of image array: " << img.size() << endl;

  // Example Test
  // Read an Image
  // Image im("./Input/Cambridge1.png");

  // Write an Image
  // cout << "Image size: " << im.width() << ", "
  //      << im.height() << ", " << im.channels() << endl;
  // im.write("./Output/SameAsInput.png");
}
