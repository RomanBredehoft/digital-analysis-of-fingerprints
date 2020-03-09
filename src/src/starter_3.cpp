///
/// \file starter_3.cpp
/// \brief Methods and functions of the starter_3 about convolution
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"

// 2D Convolution of an image
Image Image::convolution2D(Mat h){
  // Size of the filter
  int height_h = h.size().height;
  int width_h = h.size().width;

  if (height_h / 2 == 0 || width_h / 2 == 0) {
    cout << "The size of the filter must be odd (Height and width must be odd integers)" << endl;
    return *this;
  }

  // Position of the principal coefficient of the filter
  int decalage_height = height_h / 2;
  int decalage_width = width_h / 2;

  // New matrix which will contain the convoluted result
  // It is the same size as the first image
  Mat im_convoluted = im.clone();

  // Initialization of variables for the loop
  float sum = 0;

  // We pass through every pixels
  for (uint i = 0; i < height; i++) {
    for (uint j = 0; j < width; j++) {
      // We calculate now the convolution for the pixel (i, j)
      sum = 0;

      // We pass through every coefficient of the second image to
      // calculate the convolution
      for (uint k = 0; k < height_h; k++) {
        for (uint l = 0; l < width_h; l++) {
          if ((int)i + (int)k - decalage_height >= 0 && (int)i +  (int)k - decalage_height < height &&
              (int)j + (int)l - decalage_width >= 0 && (int)j + (int)l - decalage_width < width) {
            sum += h.at<float>(k, l) * im.at<float>(i + k - decalage_width, j + l - decalage_width);
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

// FFT convolution
Image Image::FFT(Mat h){
  // FFT of the original image
  Mat im_fft;
  dft(im, im_fft, 0);

  // FFT of the filter
  // We first have to change the filter which have to be the same size
  // as the image

  // Height and width of the filter
  int height_h = h.size().height;
  int width_h = h.size().width;

  // New matrix full of 0 with the same size as the image
  Mat H = Mat::zeros(height, width, CV_32FC1);

  // Top left square
  for (uint i = 0; i < height_h / 2 + 1; i++) {
    for (uint j = 0; j < width_h / 2 + 1; j++) {
      // cout <<  h.at<float>(i + height_h / 2, j + width_h / 2) << endl;
      H.at<float>(i, j) = h.at<float>(i + height_h / 2, j + width_h / 2);
    }
  }

  // Bottom right square
  for (uint i = 0; i < height_h/2; i++) {
    for (uint j = 0; j < width_h/2; j++) {
      H.at<float>( height - height_h / 2 + i, width - width_h / 2 + j) = h.at<float>(i, j);
    }
  }

  // Top right square
  for (uint i = 0; i < height_h / 2 + 1; i++) {
    for (uint j = 0; j < width_h / 2; j++) {
      H.at<float>(i, width - width_h / 2 + j) = h.at<float>(height_h / 2 + i, j);
    }
  }

  // Bottom left square
  for (uint i = 0; i < height_h / 2; i++) {
    for (uint j = 0; j < width_h / 2 + 1; j++) {
      H.at<float>(height - height_h / 2 + i, j) = h.at<float>(i, width_h / 2 + j);
    }
  }

  // FFT on the new sized matrix filter
  Mat filter_fft;
  dft(H, filter_fft, 0);

  // Spectrum Multiplication
  mulSpectrums(im_fft, filter_fft, im_fft, 0);

  // Result
  Mat im_result;
  dft(im_fft, im_result, DFT_INVERSE + DFT_SCALE, 0);

  // Result image
  Image image_result = Image(im_result);

  return image_result;
}


// Matrix separator if it's possible
pair<vector<float>, vector<float>> MatrixSeparator(Mat filter, int column){
    /*
     * We separate the matrix filter such as filter = UV
     * where U.shape (n, 1) and V.shape (1, n) but represented as vectors
     * of length n.
     */

  // Initialization of U and V
  vector<float> U;
  vector<float> V;

  for (int i = 0; i < filter.rows; i++){
    U.push_back(filter.at<float>(i, column));
  }

  for(int j = 0; j < filter.cols; j++){
    for (int i = 0; i < filter.rows; i++){
      if (filter.at<float>(i, column) != 0){
        V.push_back(filter.at<float>(i, j) / filter.at<float>(i, column));
        break;
      }
      if (i == filter.rows -1){
        V.push_back(0);
      }
    }
  }
  return make_pair(U, V);
}

// Convolution using separable filter
Image Image::convolutionSeparable2D(Mat h, int column){
    /*
    *RECETTE D'UNE BONNE CONVOLUTION:
    * 1e étape : convolution h2 * im(., n2)
    */

    // Size of the filter
    int height_h = h.size().height;
    int width_h = h.size().width;

    // Position of the principal coefficient of the filter
    int decalage_height = height_h / 2;
    int decalage_width = width_h / 2;

    // Seperate vectors such as filtres = (U, V) and h = U*V
    pair<vector<float>, vector<float>> filtres = MatrixSeparator(h, column);

    // New matrix which will contain the convoluted result
    // It is the same size as the first image
    vector<float> h1 = filtres.first;
    vector<float> h2 = filtres.second;

    int len_h1 = (int)h1.size();
    int len_h2 = (int)h2.size();

    Mat im_t = im.t();

    // g(n1, n2) = sum_i h_1(i) * X.t(n1, n2 - i)
    // g is of size (width, height) = (height_g, width_g)
    int height_g = width;
    int width_g = height;

    Mat im_g = Mat::zeros(height_g, width_g, CV_32FC1);

    // For each coordinate (n1, n2), we calculate g(n1, n2)
    for (int n1 = 0; n1 < height_g; n1++){
        // Pointer to the current row im[n1, :]
        float *current_row = im_t.ptr<float>(n1);
        for (int n2 = 0; n2 < width_g; n2++){
            // Calculus of g(n1, n2)
            for (int i = 0; i < len_h1; i++){
                if(n2 - i >= 0 && n2 - i  < height){
                    im_g.at<float>(n1, n2) += h1[len_h1 - 1 - i] * current_row[n2 - i + len_h1/2];
                }
            }
        }
    }

    // u(n1, n2) = g(n2, n1)
    Mat im_u = im_g.t();

    // u is of size (height_u, width_u) = (width_g, height_g) = (height, width)
    int height_u = width_g;
    int width_u = height_g;

    Mat im_c = Mat::zeros(height_u, width_u, CV_32FC1);

    // For each coordinate we calculate c(n1, n2) = sum_j h_2(j) * u(n1, n2 - j)
    for (int n1 = 0; n1 < height_u; n1++){
        // Pointer to the current row u[n1, :]
        float * current_row = im_u.ptr<float>(n1);
        for (int n2 = 0; n2 < width_u; n2++){
            // Calculus of c(n1, n2)
            for(int j = 0; j < len_h2; j++){
                if(n2 - j >= 0 && n2 - j < width_u){
                    im_c.at<float>(n1, n2) += h2[len_h2 - 1 -j] * current_row[n2 - j + len_h2/2];
                }
            }
        }
    }

  // Creation of the result image
  Image image_convoluted = Image(im_c);

  return image_convoluted;
}
