#include "a0.h"

using namespace std;

// This is a way for you to test your functions.
// We will only grade the contents of a0.cpp and Image.cpp
int main() {
  // --------- HANDOUT  PS00 ------------------------------
  cout << endl;

  cout << "GREETING" << endl;
  cout << "Congratulations, you have compiled the starter code!" << endl;
  printf("\n");
  
  cout << "2.b: HELLO WORLD" << endl;
  helloworld(5.9, 6.8);
  
  printf("\n");

  cout << "SIMPLEIMAGE TESTS" << endl;

  int w = 10;
  int h = 9;
  cout << "Center rectangle spanning test (" << w << " x " << h << ")..." << endl;
  
  SimpleImage img = SimpleImage(w, h);
  img.make_rectangle();
  img.print_to_terminal();

  w = 5;
  h = 3;
  cout << "Center rectangle spanning test (" << w << " x " << h << ")..." << endl;
  
  img = SimpleImage(w, h);
  img.make_rectangle();
  img.print_to_terminal();

  w = 4;
  h = 4;
  cout << "Center rectangle spanning test (" << w << " x " << h << ")..." << endl;
  
  img = SimpleImage(w, h);
  img.make_rectangle();
  img.print_to_terminal();

  w = 3;
  h = 7;
  cout << "Center rectangle spanning test (" << w << " x " << h << ")..." << endl;
  
  img = SimpleImage(w, h);
  img.make_rectangle();
  img.print_to_terminal();

  w = 100;
  h = 31;
  cout << "Center rectangle spanning test (" << w << " x " << h << ")..." << endl;
  
  img = SimpleImage(w, h);
  img.make_rectangle();
  img.print_to_terminal();

  // Example Test
  // Read an Image
  // Image im = readAnImage("./Input/Cambridge1.png");

  // Write an Image
  // cout << "Image size: " << im.width() << ", "
  //      << im.height() << ", " << im.channels() << endl;
  // im.write("./Output/SameAsInput.png");
}
