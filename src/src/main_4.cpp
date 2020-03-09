///
/// \file main_4.cpp
/// \brief Methods and functions of the main_4 about morphological filtering on grayscale images
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"


///
/// \fn float c(float r, float a = 0.2, float b = 85, float p = 1)
/// \brief Function equal to 1 when r = 0, which tends to 0 when r rises
///
/// \param r : argument
/// \param a, b, p : optional arguments which fit the function
/// \return A tuple containing the coordinates of the barycenter
///
float c_main4(float r, float a = 0.1, float b = 85, float p = 1){
  // return exp(-a * pow(r, p));  // a = 1/4294960000, p = 2

  // return 1 / (1 + a * pow(r, p) );

  // return 1 / (a * pow(ln(r + 1), p) + 1);

  // return (1 + exp(-b)) / (1 + exp(a * pow(r - b, p)));

  return (1 + exp(-b)) / (1 + exp(a * ( pow(r - b, p) ))) ;
}

// Grayscale erosion
Image Image::erosion_grayscale(Mat h){
  int size = h.size().height;
  bool Exit = false;
  float min;
  float pixel;
  Point barycentre = this->barycenter();
  int x_bary = barycentre.x;
  int y_bary = barycentre.y;

  Mat im_eroded = Mat::zeros(height, width, CV_32FC1);

  // For each pixel, its new intensity will be the minimum intensity of its neighbours
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      min = 1;
      for (int k = -size/2; k < size/2 + 1; k++) {
        for (int l = 0; l < size; l++) {
          if (i + k >= 0 && i + k < height &&
              j + l >= 0 && j + l < width) {
              if (h.at<float>(k + size/2, l + size/2) == 1){
                pixel = im.at<float>(i + k, j + l);
                if (pixel < min) {
                  min = pixel;
                }
              }

          }
          // We decrease the intensity of pixels far from the barycenter to clear the fingerprint
          if (min < 0.5) {
            float r = sqrt( pow( (i - x_bary)/sqrt(2), 2) + pow(1.2*(j - y_bary), 2) );
            im_eroded.at<float>(i, j) = min * c_main4(r);
          }
          else {
            im_eroded.at<float>(i, j) = min;
          }

        }
      }
    }
  }

  return Image(im_eroded);
}


// Grayscale dilation
Image Image::dilation_grayscale(Mat h){
  int size = h.size().height;
  bool Exit = false;
  float max;
  float pixel;

  Point barycentre = this->barycenter();
  int x_bary = barycentre.x;
  int y_bary = barycentre.y;

  Mat im_dilated = Mat::zeros(height, width, CV_32FC1);

  // For each pixel, its new intensity will be the maximum intensity of its neighbours
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      max = 0;
      for (int k = -size/2; k < size/2 + 1; k++) {
        for (int l = 0; l < size; l++) {
          if (i + k >= 0 && i + k < height &&
              j + l >= 0 && j + l < width) {
                if (h.at<float>(k + size/2, l + size/2) == 1) {
                  pixel = im.at<float>(i + k, j + l);
                  if (pixel > max) {
                    max = pixel;
                  }
                }
          }

          // We decrease the intensity of pixels far from the barycenter to clear the fingerprint
          if (max < 0.5) {
            float r = sqrt( pow( (i - x_bary)/sqrt(2), 2) + pow(1.2*(j - y_bary), 2) );
            im_dilated.at<float>(i, j) = max * c_main4(r);
          }
          else {
            im_dilated.at<float>(i, j) = max;
          }
        }
      }
    }
  }
  return Image(im_dilated);
}
