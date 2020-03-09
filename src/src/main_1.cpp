///
/// \file main_1.cpp
/// \brief Methods and functions of the main course 1
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 16 2020
///
#include "../include/main.h"

///
/// \fn float c(float r, float a = 0.2, float b = 85, float p = 1)
/// \brief Function equal to 1 when r = 0, which tends to 0 when r rises
///
/// \param r : argument
/// \param a, b, p : optional arguments which fit the function
/// \return the result
///
float c(float r, float a = 0.2, float b = 85, float p = 1){
  // return exp(-a * pow(r, p));  // a = 1/4294960000, p = 2

  // return 1 / (1 + a * pow(r, p) );

  // return 1 / (a * pow(ln(r + 1), p) + 1);

  // return (1 + exp(-b)) / (1 + exp(a * pow(r - b, p)));

  return (1 + exp(-b)) / (1 + exp(a * ( pow(r - b, p) ))) ;
}


// Returns a tuple containing the coordinates of the barrycenter
Point Image::barycenter(){
  int sum_i =0;
  int sum_j = 0;
  int count = 0;

  // We take into account only the pixels intensity darker than a ceil
  for (uint i = 0; i < height; i++) {
    for (uint j = 0; j < width; j++) {
      if (im.at<float>(i, j) > 0.6) {
        sum_i += i;
        sum_j += j;
        count += 1;
      }
    }
  }
  return Point(sum_i/count, sum_j/count);
}


// Function which applies the coefficient function c isotropically
void Image::coef_iso(uint x, uint y){
  float r;
  // We apply the function to every pixel
  for (uint i = 0; i < height; i++) {
    for (uint j = 0; j < width; j++) {
      r = sqrt( pow((int)x-(int)i, 2) + pow((int)y-(int)j, 2));
      im.at<float>(i, j) = im.at<float>(i, j) * c(r);
    }
  }
}


// ANISTROPIC COEFFICIENT FUNCTION

///
/// \fn float c_anisotropic(int x_m, int y_m, float angle, int x_p, int y_p)
/// \brief Function equal to 1 when the pixel (x_m, y_m) is close to the barycenter (x_p, y_p),
/// which tends to 0 when this distance rises, according to an elliptic shape
/// with a certain angle
///
/// \param x_m, y_m : pixel cooridnates
/// \param angle : angle of rotation of the ellipse
/// \param x_p, y_p : coordinates of the barycenter
///
/// \return the value of the new pixel
///
float c_anisotropic(int x_m, int y_m, float angle, int x_p, int y_p){
  angle = PI/2 - angle;
  float x_b = (x_m - x_p) * cos(angle) - (y_m - y_p) * sin(angle) + x_p;
  float y_b = (x_m - x_p) * sin(angle) + (y_m - y_p) * cos(angle) + y_p;

  float r = sqrt( pow( (x_b - x_p)/sqrt(2), 2) + pow(1.2*(y_b - y_p), 2) );
  return c(r);
}


// Function which applies the coefficient function c according to a direction
// determined by an angle
void Image::coef_aniso(int x0, int y0, float angle){
  // For all the pixels, we apply the coefficient function
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      // This condition let filter the whitest values, to clear the fingerprint
      if (im.at<float>(i,j) < 0.5) {
        im.at<float>(i,j) = 0;
      }
      else {
        im.at<float>(i,j) = im.at<float>(i,j) * c_anisotropic(i, j, angle, x0, y0);
      }
    }
  }
}
