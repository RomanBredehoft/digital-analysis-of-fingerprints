///
/// \file starter_4.cpp
/// \brief Methods and functions of the starter_4 about morphological filtering on binary images
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"

// Binarisation of the image using the Otsu threshold
Image Image::binarisation(){
  // Otsu threshold
  int threshold = this->threshold_otsu();

  // We computes the calculus on integers between 0 and 255
  this->map_to_base();

  // Result matrix
  Mat im_bin = Mat::zeros(height, width, CV_32FC1);
  int pixel = 0;

  // We pass through all the pixels
  // If their value is less than the threshold, the intensity is put to 0
  // Else, their intensity is put to 1
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      pixel = (int)im.at<uchar>(i, j);
      if (pixel >= threshold ) {
        im_bin.at<float>(i, j) = 0;
      }
      else {
        im_bin.at<float>(i, j) = 1;
      }
    }
  }
  this->map_to_norm();
  Image image_bin = Image(im_bin);
  // image_bin.map_to_norm();
  return image_bin;
}


// Otsu method to find the right threshold for binarisation
int Image::threshold_otsu(){
  this->map_to_base();

  int N = height * width;

  float var_max, sum, sumB, q1, q2, mu1, mu2, sigma_b_carre = 0;
  int threshold = 0;
  int max_intensity = 255;

  Mat histogram = Mat::zeros(256, 1, CV_32FC1);

  // We count the number of each possible intensity in the image
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int value = (int)im.at<uchar>(i, j);
      histogram.at<float>(value, 0) += 1;
    }
  }

  for (int i = 0; i <= max_intensity; i++) {
    sum += i * histogram.at<float>(i, 0);
  }

  for (int t = 0; t <= max_intensity; t++) {
    q1 += histogram.at<float>(t, 0);
    if (q1 == 0) {
      continue;
    }
    q2 = N - q1;

    sumB += t * histogram.at<float>(t, 0);
    mu1 = sumB/q1;
    mu2 = (sum - sumB)/q2;

    sigma_b_carre = q1 * q2 * pow(mu1 - mu2, 2);

    if (sigma_b_carre > var_max) {
      threshold = t;
      var_max = sigma_b_carre;
    }
  }
  // histogram.release();
  this->map_to_norm();
  return threshold;
}


// grayscale erosion according to the size of the filter (odd)
Image Image::erosion_binary(Mat h){
  int size = h.size().height;
  bool Exit = false;

  // Binary image
  Image image_binary = this->binarisation();
  Mat im_binary = image_binary();

  Mat im_eroded = Mat::zeros(height, width, CV_32FC1);

  // We pass through all pixels. If the neighborhood pixels, according to the shape of h,
  // are equal to 0, the pixel intensity becomes 0, else it is the same
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

      Exit = false;

      if (im_binary.at<float>(i, j) == 1) {
        // We pass through the neighbours of the current pixel
          for (int k = -size/2; k < size/2 + 1; k++) {
            for (int l = -size/2; l < size/2 + 1; l++) {
              if (i + k >= 0 && i + k < height &&
                      j + l >= 0 && j + l < width) {
                  if (h.at<float>(k + size/2, l + size/2) == 1 &&
                      im_binary.at<float>(i + k, j + l) == 0) {
                    // Exit = true if the value of the pixel needs to be change
                    Exit = true;
                    break;
                  }
              }
            }
            if (Exit) {
              break;
            }
          }

          // If Exit = False, we keep the pixel intensity, otherwise the pixel value stays initilaized at 0
          if (not Exit) {
            im_eroded.at<float>(i, j) = im_binary.at<float>(i, j);
          }

      }
    }
  }

  return Image(im_eroded);
}


// Dilation according to the size of the filter (odd)
Image Image::dilation_binary(Mat h){
  int size = h.size().height;
  bool Exit = false;

  Image image_binary = this->binarisation();
  Mat im_binary = image_binary();

  Mat im_dilated = Mat::zeros(height, width, CV_32FC1);

  // We pass through all pixels. If the pixel intensity is equal to 1, then
  // all the pixels in the neighborhood, according to the shape, is equal to 1
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

      if (im_binary.at<float>(i, j) == 1) {
        for (int k = -size/2; k < size/2 + 1; k++) {
          for (int l = -size/2; l < size/2 + 1; l++) {
            if (i + k >= 0 && i + k < height &&
                    j + l >= 0 && j + l < width) {
                if (h.at<float>(k + size/2, l + size/2) == 1) {
                  im_dilated.at<float>(i + k, j + l) = 1;
                }
            }
          }
        }
      }
    }
  }

  return Image(im_dilated);
}
