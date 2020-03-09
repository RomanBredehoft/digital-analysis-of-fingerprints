///
/// \file main_3.cpp
/// \brief Methods and functions of the starter_3 about convolution
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"

/*!
* \fn Mat KernelGenerator(Mat kernel, Point center, Point current, float dmax)
* \brief Spatially variant kernel
* \param kernel Filter we would like to have far away from center
* \param center Point where we want identity kernel
* \param current current pixel
* \param dmax maximum distance from center to a boundary
* \return spatially variant kernel at point current
*/
Mat KernelGenerator(Mat kernel, Point center, Point current, float dmax){
    float d = norm(current-center)/dmax;
    int height = kernel.size().height;
    int width = kernel.size().width;
    // Creation of identity kernel
    Mat id_kernel = Mat::zeros(kernel.size(), CV_32FC1);
    id_kernel.at<float>((int)height/2, (int)width/2) = 1;
    // return convex combination of those kernels
    return (1-d)*id_kernel + d * kernel;
}


Image Image::convKernelVariable(Mat h, Point center){
 // Size of the filter
  int height_h = h.size().height;
  int width_h = h.size().width;

  // Position of the principal coefficient of the filter
  int decalage_height = height_h / 2;
  int decalage_width = width_h / 2;

  // New matrix which will contain the convoluted result
  // It is the same size as the first image
  Mat im_convoluted = Mat::zeros(height, width, CV_32FC1);

  // Initialization of variables for the loop
  float sum;

  // We pass through every pixels
  for (uint i = 0; i < height; i++) {
    for (uint j = 0; j < width; j++) {
      // We calculate now the convolution for the pixel (i, j)
      sum = 0;
      // Filter for the pixel (i, j)
      Mat kernel = KernelGenerator(h, center, Point(i+0.5, j+0.5), 10);
      // We pass through every coefficient of the second image to
      // calculate the convolution
      for (uint k = 0; k < height_h; k++) {
        for (uint l = 0; l < width_h; l++) {
          if ((int)i + (int)k - decalage_height >= 0 && (int)i +  (int)k - decalage_height < height &&
              (int)j + (int)l - decalage_width >= 0 && (int)j + (int)l - decalage_width < width) {
            sum += kernel.at<float>(k, l) * im.at<float>(i + k - decalage_height, j + l - decalage_width);
          }
        }

      }

      im_convoluted.at<float>(i, j) = sum;
    }
  }

  // Creation of the result image
  Image image_convoluted = Image(im_convoluted);

  return image_convoluted;
}
