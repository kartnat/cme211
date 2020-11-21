#include <string>

#ifndef IMAGE_HPP
#define IMAGE_HPP

/*Convolution function that takes in the input containing the jpeg
image data, output for the input array post-convolution, and kernel
array.  The output array is modified in place.*/
void Convolution(boost::multi_array<unsigned char,2>& input,
                 boost::multi_array<unsigned char,2>& output,
                 boost::multi_array<float,2>& kernel);

/*Image class for performing the operations outlined in the assignment.
the main methods are as follows:

--getFile: Returns the current state of the image array to main
--Save: Takes as argument the name of the save file and outputs the 
current state of the jpeg image to said file.  If no file name is provided
the original image file is overwritten.
--BoxBlur: Takes as argument the size of the kernel array and smooths
the input using the Convolution() function.  The values of the kernel 
are scaled so that they sum to one.  It returns the sharpness of the image
(via a call to the sharpness method).
--Sharpness: Computes the sharpness of the "sharpInput" image, and returns 
an unsigned int representing the sharpness of the image.*/
class Image {
  boost::multi_array<unsigned char, 2> a;
  std::string filename;
  std::string file;
  unsigned int kernSize;
  boost::multi_array<unsigned char,2> sharpInput;
 public:
  Image(std::string filename);
  boost::multi_array<unsigned char, 2> getFile(void);
  void Save(std::string file);
  unsigned int BoxBlur(unsigned int kernsize);
  unsigned int Sharpness(boost::multi_array<unsigned char,2>& sharpInput);
};

#endif /* IMAGE_HPP */
