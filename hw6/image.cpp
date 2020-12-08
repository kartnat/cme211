#include <boost/multi_array.hpp>
#include <cmath>
#include <iostream>
#include <jpeglib.h>
#include <math.h>
#include <stdexcept>
#include <string>

#include "hw6.hpp"
#include "image.hpp"

#define BOOST_DISABLE_ASSERTS

void Convolution(boost::multi_array<unsigned char,2>& input,
                   boost::multi_array<unsigned char,2>& output,
                   boost::multi_array<float,2>& kernel) {
  
    //Throwing error if kernel is not square, has odd size, or size < 3
    if (kernel.shape()[0] != kernel.shape()[1] || kernel.shape()[0] % 2 == 0 || kernel.shape()[0] < 3) {
      throw std::runtime_error("misspecified kernel");
    }
    long unsigned int kernLen = kernel.shape()[0];
    long unsigned int addLen = (kernLen-1);
    boost::multi_array<unsigned char, 2> tmp(boost::extents[input.shape()[0]+addLen][input.shape()[1]+addLen]);
    long unsigned int inputH = input.shape()[0];
    long unsigned int inputW = input.shape()[1];
    long unsigned int tmpH = tmp.shape()[0];
    long unsigned int tmpW = tmp.shape()[1];
    // Populating larger array (tmp) that has all necessary entries to compute
    // the convolution 
//--design_1
//--Copying is inefficient
//--START
    for (long unsigned int i=0;i<tmpH;i++) {
      for (long unsigned int j=0;j<tmpW;j++) {
        //If entry lies in the original jpeg input, copy it
        if (i >= addLen/2 && i < tmpH-addLen/2 && j>= addLen/2 && j < tmpW-addLen/2) {
          tmp[i][j] = input[i-addLen/2][j-addLen/2];
          }
//--END
        //Out of bounds on left or right of image
        else if ((i < addLen/2 || i >= tmpH-addLen/2) && j>= addLen/2 && j < tmpW-addLen/2) {
          if (i < addLen/2) {
            tmp[i][j] = input[0][j-addLen/2]; }
          else if (i >= tmpH-addLen/2) {
            tmp[i][j] = input[inputH-1][j-addLen/2]; }          
        }
        //Out of bounds on top or bottom
        else if (i >= addLen/2 && i < tmpH-addLen/2 && (j< addLen/2 || j >= tmpW-addLen/2)) {
          if (j < addLen/2) {
            tmp[i][j] = input[i-addLen/2][0]; }
          else if (j >= tmpW-addLen/2) {
            tmp[i][j] = input[i-addLen/2][inputW-1]; }
        }
        //Extending corner entries, of which there are four
        else if  (i < addLen/2 && j < addLen/2) {
          tmp[i][j] = input[0][0]; } 
        else if  (i < addLen/2 && j >= tmpW- addLen/2) { 
          tmp[i][j] = input[0][inputW-1]; } 
        else if  (i >= tmpH-addLen/2 && j >= tmpW- addLen/2) { 
          tmp[i][j] = input[inputH-1][inputW-1]; }
        else if (i >= tmpH-addLen/2 && j < addLen/2) {
          tmp[i][j] = input[inputH-1][0]; }
     
      }
    }
    
    // Performing the convolution operation
    for (long unsigned int i=0;i<inputH;i++) {
      for (long unsigned int j=0;j<inputW;j++) {
        float sum=0;
        for (long unsigned int u=i;u<=i+addLen;u++) {
          for (long unsigned int v=j;v<=j+addLen;v++) {
            sum += (unsigned int)tmp[u][v]*kernel[u-i][v-j];
          }
        }
        //Handling overflow and underflow
        if (sum<0) {
          output[i][j] = (unsigned char)0; 
          }
        else if (sum > 255) {
          output[i][j] = (unsigned char)255; }
        else {
//--style_0
//--Careful with double casts
//--START
          output[i][j] = (unsigned char)((unsigned int)(sum));}
//--END
      }
    }  
}

  
  boost::multi_array<unsigned char, 2> a;
  // Class constructor, instantiates the jpeg array in place
  Image::Image(std::string filename) {
    this->filename = filename;
    ReadGrayscaleJPEG(filename, this->a);
  }

  boost::multi_array<unsigned char, 2> Image::getFile(void) {return a;}
   
  void Image::Save(std::string file) {
    if (file == "")
      file == "Stanford.jpg";
    WriteGrayscaleJPEG(file, this->a);
  }
 
  unsigned int Image::BoxBlur(unsigned int kernSize) {
    boost::multi_array<float, 2> kernel(boost::extents[kernSize][kernSize]);
    // Populating the kernel array 
    for (unsigned int i=0;i<kernSize;i++) {
      for (unsigned int j=0;j<kernSize;j++) {
        kernel[i][j] = (float)(1.0/(kernSize*kernSize));
     }
   }
   long unsigned int outputH = this->a.shape()[0];
   long unsigned int outputW = this->a.shape()[1];
   //Initializing output array to output the blurred images
   boost::multi_array<unsigned char, 2> output(boost::extents[outputH][outputW]);
   Convolution(this->a, output,kernel);
   //Re-assigning the blurred output to member attribute, in order to call
   //the save method with the blurred image
   this->a = output;
   return Sharpness(output);
 }
 
 unsigned int Image::Sharpness(boost::multi_array<unsigned char,2>& sharpInput) {
   //Initializing the Laplacian kernel
   boost::multi_array<float, 2> kernel(boost::extents[3][3]);
   kernel[0][0]=0;
   kernel[0][1]=(float)1;
   kernel[0][2]=0;
   kernel[1][0]=(float)1;
   kernel[1][1]=(float)-4;
   kernel[1][2]=(float)1;
   kernel[2][0]=0;
   kernel[2][1]=(float)1;
   kernel[2][2]=0;
   
   long unsigned int outputH = sharpInput.shape()[0];
   long unsigned int outputW = sharpInput.shape()[1];
   boost::multi_array<unsigned char, 2> sharpOutput(boost::extents[outputH][outputW]);
   
   //Performing the sharpness convolution on the blurred image
   Convolution(sharpInput, sharpOutput,kernel);
   
   //Finding the maximal element in the array
   unsigned int max = *std::max_element(sharpOutput.origin(), sharpOutput.origin() + sharpOutput.num_elements());
   return max;
 }  

