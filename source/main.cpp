#include <iostream>
#include "bitmap.h"

using namespace std;

int main()
{
  Bitmap bitmap(200, 100);
  bitmap.SetPixel(10, 10, 255, 255, 255);
  bitmap.Serialize(&cout);
}
