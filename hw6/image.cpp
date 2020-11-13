#include <boost/multi_array.hpp>
#include <iostream>
#include <jpeglib.h>
#include <stdexcept>
#include <string>
#define BOOST_DISABLE_ASSERTS

#include "hw6.hpp"
//#include "image.hpp"

class Image i
// public:
  boost::multi_array<unsigned char, 2> a;
  boost::multi_array<unsigned char, 2> b;
  std::string filename;
  public:
  Image(std::string filename) i
    this->filename = filename;
    ReadGrayscaleJPEG(filename, this->a);

//    for (unsigned int n = 0; n < a.num_elements (); n++)
  //     std::cout  << "a.data ()[" << n << "] = " << a.data ()[n] << std::endl;  }
  }
  boost::multi_array<unsigned char, 2> getFile(void) {return a;}
  
  void Convolution(boost::multi_array<unsigned char,2>& input,
                   boost::multi_array<unsigned char,2>& output,
                   boost::multi_array<float,2>& kernel) {
  
    if (kernel.shape()[0] != kernel.shape()[1] || kernel.shape()[0] % 2 == 0 || kernel.shape()[0] < 3) {
      throw std::runtime_error("misspecified kernel");
    }
    long unsigned int kernLen = kernel.shape()[0];
    //std::cout << kernLen << std::endl;
    long unsigned int addLen = (kernLen-1);
    boost::multi_array<unsigned char, 2> tmp(boost::extents[input.shape()[0]+addLen][input.shape()[1]+addLen]);
  //input.resize(boost::extents[input.shape()[0]+addLen][input.shape()[1]+addLen]);
    long unsigned int inputH = input.shape()[0];
    long unsigned int inputW = input.shape()[1];
    long unsigned int tmpH = tmp.shape()[0];
    long unsigned int tmpW = tmp.shape()[1];
  //unsigned int rowThresh = addLen/2;
    for (long unsigned int i=0;i<tmpH;i++) {
      for (long unsigned int j=0;j<tmpW;j++) {
        //tmp[i][j] = 3;
        if (i >= addLen/2 && i < tmpH-addLen/2 && j>= addLen/2 && j < tmpW-addLen/2) {
          //tmp[i][j] = 3
          tmp[i][j] = (unsigned char)input[i-addLen/2][j-addLen/2];
          std::cout << input[i-addLen/2][j-addLen/2] << std::endl; 
          }
        /*if ((i < addLen/2 || i >= tmpH-addLen/2) && j>= addLen/2 && j < tmpW-addLen/2) {
          if (i < addLen/2) {
            tmp[i][j] = input[0][j-addLen/2]; }
          else if (i >= tmpH-addLen/2) {
            tmp[i][j] = input[inputH-1][j-addLen/2]; }          
        }
        if (i >= addLen/2 && i < tmpH-addLen/2 && (j< addLen/2 || j >= tmpW-addLen/2)) {
          if (j < addLen/2) {
            tmp[i][j] = input[i-addLen/2][0]; }
          else if (j >= tmpW-addLen/2) {
            tmp[i][j] = input[i-addLen/2][inputW-1]; }
        }
        if  (i < addLen/2 && j < addLen/2) {
          tmp[i][j] = input[0][0]; } 
        if  (i < addLen/2 && j >= tmpW- addLen/2) { 
          tmp[i][j] = input[0][inputW-1]; } 
        if  (i >= tmpH-addLen/2 && j >= tmpW- addLen/2) { 
          tmp[i][j] = input[inputH-1][inputW-1]; }
        if (i >= tmpH-addLen/2 && j < addLen/2) {
          tmp[i][j] = input[inputH-1][0]; }*/  
     
      }
    }
    
    for (long unsigned int i=0;i<inputH;i++) {
      for (long unsigned int j=0;j<inputW;j++) {
        double sum=0;
        //center = tmp[i+addlen/2][j+addlen/2];
        for (long unsigned int u=i;u<=i+addLen;u++) {
          for (long unsigned int v=j;v<=j+addLen;v++) {
            //std::cout << tmp[u][v] << " " << kernel[u-i][v-j] << std::endl;
            sum += tmp[u][v]*kernel[u-i][v-j];
          }
        }
        double b = kernLen;
        output[i][j] = (unsigned char)(sum/(b*b));
      }
    } 
  
    /*for (unsigned  int i = 0; i < tmpH; i++) {
      for (unsigned  int j = 0; j < tmpW; j++) {
        std::cout  << "tmp[" << i << "][" << j << "] = " << tmp[i][j] << std::endl;
      }
    }*/
    
     
    for (unsigned  int i = 0; i < inputH; i++) {
      for (unsigned  int j = 0; j < inputW; j++) {
        std::cout  << "output[" << i << "][" << j << "] = " << output[i][j] << std::endl;
      }
    }
  
 }
 
 unsigned int BoxBlur(long unsigned int& kernSize) {
   boost::multi_array<float, 2> kernel(boost::extents[kernSize][kernSize]);
   for (long unsigned int i=0;i<kernSize;i++) {
     for (long unsigned int j=0;j<kernSize;j++) {
       kernel[i][j] = 1.0/(kernSize*kernSize);
      
     }
   }  
   long unsigned int outputH = this->a.shape()[0];
   long unsigned int outputW = this->a.shape()[1];
   boost::multi_array<unsigned char, 2> output2(boost::extents[outputH][outputW]);
   Convolution(this->a, output2,kernel);
   //for (unsigned  int i = 0; i < output2.shape()[0]; i++) {
    // for (unsigned  int j = 0; j < output2.shape()[1]; j++) {
      // std::cout  << "output2[" << i << "][" << j << "] = " << output2[i][j] << std::endl;
  // }
  // }
   return Sharpness(output2);
 }
 
 unsigned int Sharpness(boost::multi_array<unsigned char,2>& input) {
   boost::multi_array<float, 2> kernel(boost::extents[3][3]);
   kernel[0][0]=0;
   kernel[0][1]=1;
   kernel[0][2]=0;
   kernel[1][0]=1;
   kernel[1][1]=-4;
   kernel[1][2]=1;
   kernel[2][0]=0;
   kernel[2][1]=1;
   kernel[2][2]=0;
   
   long unsigned int outputH = input.shape()[0];
   long unsigned int outputW = input.shape()[1];
   boost::multi_array<unsigned char, 2> output(boost::extents[outputH][outputW]);
   Convolution(input, output,kernel);
   /*for (unsigned  int n = 0; n < output.num_elements (); n++){
         std::cout  << "a.data ()[" << n << "] = " << output.data ()[n] << std::endl;
   }*/
   
   unsigned int max = *std::max_element(output.origin(), output.origin() + output.num_elements());
   return max;
 }  
 
  void printArray(void) {
    //return a;
    for (unsigned  int n = 0; n < a.num_elements (); n++)
      std::cout  << "a.data ()[" << n << "] = " << a.data ()[n]*3 << std::endl;
  }
};

int main () {
  std::string file = "stanford.jpg";
  Image i(file);
  boost::multi_array<unsigned char, 2> input= i.getFile();
  long unsigned int size = 3;
  //std::cout << i.BoxBlur(size) << std::endl;
  //std::cout << i.Sharpness(input) << std::endl;
  
  boost::multi_array<float, 2> kernel(boost::extents[3][3]);
  //boost::multi_array<unsigned char, 2> input(boost::extents[3][3]);
  //boost::multi_array<unsigned char, 2> input(boost::extents[3][3]);
  boost::multi_array<unsigned char, 2> output(boost::extents[input.shape()[0]][input.shape()[1]]);
  
  /*input[0][0] = (unsigned char)2;
  input[0][1] = (unsigned char)2;
  input[0][2] = (unsigned char)2;
  input[1][0] = (unsigned char)2;
  input[1][1] = (unsigned char)2;
  input[1][2] = (unsigned char)2;
  input[2][0] = (unsigned char)2;
  input[2][1] = (unsigned char)2;
  input[2][2] = (unsigned char)2;*/
  /*n =0;
    for (unsigned  int i = 0; i < 3; i++) {
      for (unsigned int j=0;j<3;j++) {
        input[i][j] = n++;
  }
  }*/
  //std::cout << *std::max_element(input.origin(), input.origin() + input.num_elements()) << std::endl;
  for (unsigned  int i = 0; i < 3; i++) {
      for (unsigned int j=0;j<3;j++) {
        kernel[i][j] = 1.0;
  }
  }

  //std::cout << kernel.shape()[0] << std::endl;
  /*for (unsigned  int i = 0; i < 3; i++) {
        for (unsigned  int j = 0; j < 3; j++) {
                std::cout  << "input[" << i << "][" << j << "] = " << input[i][j] << std::endl;
  }
  }*/
  i.Convolution(input, output, kernel);
  
  //  std::cout  << "a.data ()[" << n << "] = " << a.data ()[n] << std::endl;
  //std::cout << u.shape()[0] << std::endl;
  return 0;
}
