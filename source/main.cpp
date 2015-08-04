#include <iostream>
#include "bitmap.h"

using namespace std;

int main()
{
  cout << "Hello, world!" << endl;
  cout << "BitmapFileHeader: " << sizeof(BitmapFileHeader)
       << "bytes" << endl;
  cout << "BitmapHeader: " << sizeof(BitmapHeader)
       << "bytes" << endl;
}
