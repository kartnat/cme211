#include <boost/multi_array.hpp>
#include <cmath>
#include <iostream>
#include <jpeglib.h>
#include <math.h>
#include <stdexcept>
#include <string>
#define BOOST_DISABLE_ASSERTS

#include "hw6.hpp"
#include "image.hpp"

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
          tmp[i][j] = input[i-addLen/2][j-addLen/2];
          //std::cout << input[i-addLen/2][j-addLen/2] << std::endl; 
          }
        else if ((i < addLen/2 || i >= tmpH-addLen/2) && j>= addLen/2 && j < tmpW-addLen/2) {
          if (i < addLen/2) {
            tmp[i][j] = input[0][j-addLen/2]; }
          else if (i >= tmpH-addLen/2) {
            tmp[i][j] = input[inputH-1][j-addLen/2]; }          
        }
        else if (i >= addLen/2 && i < tmpH-addLen/2 && (j< addLen/2 || j >= tmpW-addLen/2)) {
          if (j < addLen/2) {
            tmp[i][j] = input[i-addLen/2][0]; }
          else if (j >= tmpW-addLen/2) {
            tmp[i][j] = input[i-addLen/2][inputW-1]; }
        }
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
    
    for (long unsigned int i=0;i<inputH;i++) {
      for (long unsigned int j=0;j<inputW;j++) {
        float sum=0;
        //center = tmp[i+addlen/2][j+addlen/2];
        for (long unsigned int u=i;u<=i+addLen;u++) {
          for (long unsigned int v=j;v<=j+addLen;v++) {
            //std::cout << tmp[u][v] << " " << kernel[u-i][v-j] << std::endl;
            sum += (unsigned int)tmp[u][v]*kernel[u-i][v-j];
          }
        }
        //float b=(float)(kernLen*kernLen);
        if (sum<0) {
          //output[i][j] = (unsigned char)(std::abs (round(sum)));
          output[i][j] = (unsigned char)0; 
          }
        else if (sum > 255) {
          output[i][j] = (unsigned char)255; }
        else {
          output[i][j] = (unsigned char)((unsigned int)(sum));}
      }
    } 
  
    /*for (unsigned  int i = 0; i < 7; i++) {
      for (unsigned  int j = 0; j < 7; j++) {
        std::cout  << "tmp[" << i << "][" << j << "] = " << (unsigned int)tmp[i][j] << std::endl;
      }
    }*/
    
   /* for (unsigned  int i = 0; i < 3; i++) {
          for (unsigned  int j = 0; j < 3; j++) {
                  std::cout  << "input[" << i << "][" << j << "] = " << (unsigned int)input[i][j] << std::endl;
                        }
                            }*/ 
    /*for (unsigned  int i = 0; i < 7; i++) {
      for (unsigned  int j = 0; j < 7; j++) {
        std::cout  << "output[" << i << "][" << j << "] = " << (unsigned int)output[i][j] << std::endl;
      }
    }*/
  
}


//class Image {
// public:
  boost::multi_array<unsigned char, 2> a;
 // boost::multi_array<unsigned char, 2> b;
  //std::string filename;
//  public:
  Image::Image(std::string filename) {
    this->filename = filename;
    ReadGrayscaleJPEG(filename, this->a);
  }

//    for (unsigned int n = 0; n < a.num_elements (); n++)
  //     std::cout  << "a.data ()[" << n << "] = " << a.data ()[n] << std::endl;  }
  boost::multi_array<unsigned char, 2> Image::getFile(void) {return a;}
   
  void Image::Save(std::string file) {
    if (file == "")
      file == "Stanford.jpg";
    WriteGrayscaleJPEG(file, this->a);
  }
 
 unsigned int Image::BoxBlur(unsigned int kernSize) {
   boost::multi_array<float, 2> kernel(boost::extents[kernSize][kernSize]);
   for (unsigned int i=0;i<kernSize;i++) {
     for (unsigned int j=0;j<kernSize;j++) {
       kernel[i][j] = (float)(1.0/(kernSize*kernSize));
      
     }
   }  
   long unsigned int outputH = this->a.shape()[0];
   long unsigned int outputW = this->a.shape()[1];
   boost::multi_array<unsigned char, 2> output(boost::extents[outputH][outputW]);
   Convolution(this->a, output,kernel);
/*   for (unsigned  int i = 0; i < outputH; i++) {
    for (unsigned  int j = 0; j < outputW; j++) {
      std::cout  << "output2[" << i << "][" << j << "] = " << output[i][j] << std::endl;
   }
   }*/
   //return 0;
   this->a = output;
   return Sharpness(output);
 }
 
 unsigned int Image::Sharpness(boost::multi_array<unsigned char,2>& sharpInput) {
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
   
   /*for (unsigned  int i = 0; i < 3; i++) {
     for (unsigned  int j = 0; j < 3; j++) {
       std::cout  << "input[" << i << "][" << j << "] = " << (unsigned int)input[i][j] << std::endl;
     }
   }*/
     
   Convolution(sharpInput, sharpOutput,kernel);
   /*for (unsigned  int n = 0; n < output.num_elements (); n++){
         std::cout  << "a.data ()[" << n << "] = " << output.data ()[n] << std::endl;
   }*/
   
   unsigned int max = *std::max_element(sharpOutput.origin(), sharpOutput.origin() + sharpOutput.num_elements());
   return max;
 }  
 
 /* void printArray(void) {
    //return a;
    for (unsigned  int n = 0; n < a.num_elements (); n++)
      std::cout  << "a.data ()[" << n << "] = " << a.data ()[n]*3 << std::endl;
  }*/
//};

/*
int main () {
  std::string file = "stanford.jpg";
  Image i(file);
  boost::multi_array<unsigned char, 2> input= i.getFile();
  std::cout << "Original Image: " << i.Sharpness(input) << std::endl;
  
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
  
  //std::cout << i.Sharpness(input) << std::endl;
  
  boost::multi_array<float, 2> kernel(boost::extents[3][3]);
  //boost::multi_array<unsigned char, 2> input(boost::extents[3][3]);
  //boost::multi_array<unsigned char, 2> input(boost::extents[3][3]);
  //boost::multi_array<unsigned char, 2> output(boost::extents[input.shape()[0]][input.shape()[1]]);
  
  input[0][0] = (unsigned char)2;
  input[0][1] = (unsigned char)2;
  input[0][2] = (unsigned char)2;
  input[1][0] = (unsigned char)2;
  input[1][1] = (unsigned char)2;
  input[1][2] = (unsigned char)2;
  input[2][0] = (unsigned char)2;
  input[2][1] = (unsigned char)2;
  input[2][2] = (unsigned char)2;
  n =0;
    for (unsigned  int i = 0; i < 3; i++) {
      for (unsigned int j=0;j<3;j++) {
        input[i][j] = n++;
  }
  }
  //std::cout << *std::max_element(input.origin(), input.origin() + input.num_elements()) << std::endl;
  for (unsigned  int i = 0; i < 3; i++) {
      for (unsigned int j=0;j<3;j++) {
        kernel[i][j] = 1.0;
  }
  }

  //std::cout << kernel.shape()[0] << std::endl;
  for (unsigned  int i = 0; i < 3; i++) {
        for (unsigned  int j = 0; j < 3; j++) {
                std::cout  << "input[" << i << "][" << j << "] = " << input[i][j] << std::endl;
  }
  }
  //i.Convolution(input, output, kernel);
  
  //  std::cout  << "a.data ()[" << n << "] = " << a.data ()[n] << std::endl;
  //std::cout << u.shape()[0] << std::endl;
  return 0;
}
*/
