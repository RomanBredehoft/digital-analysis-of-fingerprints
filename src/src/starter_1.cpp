///
/// \file starter_1.cpp
/// \brief Basic functions of the class Image.
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"

// BASIC FUNCTIONS //

// Constructor with a filename
Image::Image(const string& file){
  im = imread(file, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

  if(!im.data){                             // Check for invalid input
      cerr << "Could not open or find the image" << endl ;
  }

  height = im.size().height;      // Gives the size
  width = im.size().width;
  this->map_to_norm();
};


// Copy constructor
Image::Image(const Image& imag) {
  height = imag.height;
  width = imag.width;

  // Creation of the copied matrix
  im = Mat::zeros(height, width, imag.im.type());

  // Fill the matrix according to the type of the first matrix
  // Only two types are possible, float and uchar
  if (imag.im.type() == CV_32FC1) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        im.at<float>(i, j) = imag.im.at<float>(i, j);
      }
    }
  }

  else {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        im.at<uchar>(i, j) = imag.im.at<uchar>(i, j);
      }
    }
  }

  this->map_to_norm();
};


// Constructor with height and width of an image
Image::Image(uint h, uint w){
  height = h;
  width = w;
  im = Mat(height, width, CV_8UC1);
  this->map_to_norm();
};


// Constructor with a matrix image
Image::Image(Mat im_mat){
  height = im_mat.size().height;
  width = im_mat.size().width;

  im = Mat::zeros(height, width, im_mat.type());

  if (im_mat.type() == CV_32FC1) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        im.at<float>(i, j) = im_mat.at<float>(i, j);
      }
    }
  }

  else {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        im.at<uchar>(i, j) = im_mat.at<uchar>(i, j);
      }
    }
  }

  this->map_to_norm();
}


Image::~Image(){
  im.release();
}


// Function displaying the image in a new window
void Image::display(){
  this->map_to_base();
  namedWindow("Dsplay window", WINDOW_AUTOSIZE ); // Create a window for display.
  imshow("Display window", im);                   // Show our image inside it.
  waitKey(0);                                          // Wait for a keystroke in the window
  this->map_to_norm();
};


// Function saving the image into a file named filename.png
void Image::save(const string& filename) {
  this->map_to_base();
  string name = "../results/" + filename + ".png";
  imwrite(name, im);          // Write into a file
  this->map_to_norm();
};


// Function converting the type of the image from uchar to float
void Image::cast(){
  for (uint i = 0; i < height; i++){
    for (uint j = 0; j < width; j++){
      float pixelGrayValue = im.at<float>(i,j);
      cout << pixelGrayValue << " ";
    }
    cout << endl;
  }
};


// Function mapping the values of the image from [0, 255] to [0, 1]
// The whiter value 255 becomes the value 0 and the darker value 0 becomes the value 1
void Image::map_to_norm(){
  // Check if the values are uchar
  if (im.type() == CV_8UC1) {
    im.convertTo(im, CV_32FC1);   // CV_32FC1 is for 1-channel (C1, i.e. grey image) float valued (32F) pixels

    // Conversion for every pixel
    for (uint i = 0; i < height; i++){
      for (uint j = 0; j < width; j++){
        im.at<float>(i, j) = 1. - im.at<float>(i,j) / 255.;
      }
    }

  }
};


// Function mapping the values of the image from [0, 1] to [0, 255]
// The whiter value 0 becomes the value 255 and the darker value 1 becomes the value 0
void Image::map_to_base(){
  // Check if the values are floats
  if (im.type() == CV_32FC1) {

    // Conversion for every pixel
    for (uint i = 0; i < height; i++){
      for (uint j = 0; j < width; j++){
        im.at<float>(i, j) = (1 - im.at<float>(i,j)) * 255;
      }
    }
    // Conversion of the matrix from type float to uchar
    im.convertTo(im, CV_8UC1);
  }
}


// Function creating a rectangle between the top-left point A = (xa, ya) and
// the bottom-right point B = (xb, yb)
Image Image::rectangle(int xa, int ya, int xb, int yb, uchar color){
  // Copy of the actual image
  Image new_image = Image(im);

  // Transformation of the color to a float betwean 0 and 255
  // As (uchar)255 coresponds to (float)0, we have this function
  float color_float = 1 - (float)color / 255.;

  // We fill the rectangle with the right color
  for (int i = xa; i < xb + 1; i++){
    for (int j = ya; j < yb + 1; j++){
      new_image.im.at<float>(i,j) = color;
    }
  }

  return new_image;
}


// Function returning the symmetric of the image along the y axis
Image Image::sym_y(){
  // Copy of the image
  Mat im_sym = Mat::zeros(height, width, CV_32FC1);
  // Mat im_sym = im.clone();

  // For all pixels we apply the symmetry transform s(x, y) = f(x, width - y)
  for (uint i = 0; i < height; i++){
    for (uint j = 0; j < width; j++){
      im_sym.at<float>(i, width - j) = im.at<float>(i,j);
    }
  }

  // Creates the image sym of class Image from the matrix
  Image sym(im_sym);

  return sym;
}


// Function returning the symmetric of the image along the first diagonal
Image Image::sym_diag() {
  // Create an initialization of the transpose of the image
  Mat im_diag(width, height, CV_32FC1, 0.0);

  // We apply the transposition at every pixel s(x, y) = f(y, x)
  for (uint i = 0; i < width; i++){
    for (uint j = 0; j < height; j++){
      im_diag.at<float>(i,j) = im.at<float>(j,i);
    }
  }

  // Creates the image of class Image from the matrix
  Image sym_diag(im_diag);

  return sym_diag;
};


// Function returning the size of the image
tuple<int, int> Image::size(){
  return make_tuple(height, width);
}


// Operator overloading to get the matrix image
Mat Image::operator()(){
  return im;
};


// Function returning the value of the minimum of intensity, which correspondd
// to the value 255 which is white
int Image::min(){
  this->map_to_base();

  int min = 255;
  int value;
  for (uint i = 0; i < height; i++){
    for (uint j = 0; j < width; j++){
      value = (int)im.at<uchar>(i,j);
      if (min > value){
        min = value;
      }
      if (min == 0){
        return 0;
      }
    }
  }
  this->map_to_norm();
  return min;
};
