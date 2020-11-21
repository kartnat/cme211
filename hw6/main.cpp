#include <iostream> 
#include <string>

#include "hw6.hpp"
#include "image.hpp"

int main() {
  // Loading the original image and outputting the sharpness
  std::string file = "stanford.jpg";
  Image i(file);
  boost::multi_array<unsigned char, 2> input= i.getFile();
  std::cout << "Original Image: " << i.Sharpness(input) << std::endl;
  
  //Creating new instances of the image class with the same
  //stanford.jpg file, blurring the image, saving it, and 
  //printing the sharpness to console.
  
  Image i1(file);
  unsigned int size = 3;
  std::cout << "BoxBlur( 3): " << i1.BoxBlur(size) << std::endl;
  std::string saveFile = "BoxBlur03.jpg";
  i1.Save(saveFile);
            
  Image i2(file);
  size = 7;
  std::cout << "BoxBlur( 7): " << i2.BoxBlur(size) << std::endl;
  saveFile = "BoxBlur03.jpg";
  i2.Save(saveFile);
          
  Image i3(file);
  size = 11;
  std::cout << "BoxBlur(11): " << i3.BoxBlur(size) << std::endl;
  saveFile = "BoxBlur03.jpg";
  i3.Save(saveFile);
          
  Image i4(file);
  size = 15;
  std::cout << "BoxBlur(15): " << i4.BoxBlur(size) << std::endl;
  saveFile = "BoxBlur15.jpg";
  i4.Save(saveFile);
          
  Image i5(file);
  size = 19;
  std::cout << "BoxBlur(19): " <<i5.BoxBlur(size) << std::endl;
  saveFile = "BoxBlur19.jpg";
  i5.Save(saveFile);
          
  Image i6(file);
  size = 23;
  std::cout << "BoxBlur(23): " <<i6.BoxBlur(size) << std::endl;
  saveFile = "BoxBlur03.jpg";
  i6.Save(saveFile);
          
  Image i7(file);
  size = 27;
  std::cout << "BoxBlur(27): " <<i7.BoxBlur(size) << std::endl;
  saveFile = "BoxBlur27.jpg";
  i7.Save(saveFile);
          
  return 0;
}
