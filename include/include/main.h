///
/// \file main.h
/// \brief Contains the class Image which contains all the methods we need
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#ifndef MAIN
#define MAIN

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <stdexcept>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <chrono>

#define PI 3.14159265

using namespace cv;
using namespace std;

///
/// \class Image
/// \brief Class representing an image
///
/// The class deals with image matrix and its height and width
///
class Image {
private:
    Mat im;           /*!< Matrix of uint between 0 and 255 (uchar) */
    uint height;      /*!< Number of rows */
    uint width;       /*!< Number of columns */
public:
// STARTER 1
    ///
    /// \brief Constructor
    ///
    /// Constructor of the class Image with a filename.
    ///
    /// \param const string& file : name of the file where the image is.
    ///
    Image(const string&);

    ///
    /// \brief Constructor
    ///
    /// Constructor of the class Image with a matrix.
    ///
    /// \param Mat im_mat : matrix containing the pixels values
    ///
    Image(Mat);

    ///
    /// \brief Constructor
    ///
    /// Constructor of the class Image with the size of the image.
    ///
    /// \param uint h : height of the image
    /// \param uint w : width of the image
    ///
    Image(uint h, uint w);

    ///
    /// \brief Destructor
    ///
    /// Destructor of the class Image. It deallocates the matrice created from an image.
    ///
    /// \param uint h : height of the image
    /// \param uint w : width of the image
    ///
    ~Image();

    ///
    /// \brief Copy constructor
    ///
    /// Copys constructor of the class Image.
    ///
    /// \param const Image& imag : image à cloner
    ///
    Image(const Image&);

    ///
    /// \brief Image display
    ///
    /// \param string name : name of the window
    /// Displays an image.
    ///
    void display();

    ///
    /// \brief Save
    ///
    /// Saves the current image in the file in parameter.
    ///
    /// \param const string& filename : name of the file in which to save the image.
    ///
    void save(const string&);

    ///
    /// \brief Cast
    ///
    /// Casts all pixels values in the prompt
    ///
    void cast();

    ///
    /// \brief Converter to float
    ///
    /// Converts all the pixels values of the image from uchar type between 0 and 255 to float type between 0 and 1.
    /// Caution : values (uchar) 255 corresponding to the white are changed into (float) 0 and values (uchar) 0 corresponding to the black are changed into (float) 1.
    ///
    void map_to_norm();

    ///
    /// \brief Converter to uchar
    ///
    /// Converts all the pixels values of the image from float type between 0 and 1 to uchar type between 0 and 255.
    /// Caution : values (float) 0 corresponding to the white are changed into (uchar) 255 and values (float) 1 corresponding to the black are changed into (uchar) 0.
    ///
    void map_to_base();

    ///
    /// \brief Rectangle creator
    ///
    /// Creates a new image with a rectangle between top left point and bottom right point, filled with a give grayscale color.
    ///
    /// \param int xa, ya : Coordinates of the top left point
    /// \param int xb, yb : Coordinates of the bottom right point
    /// \param uchar color : Grayscale color of the rectangle (between 0 and 255)
    ///
    /// \return Image containing a rectangle filled with the color
    ///
    Image rectangle(int, int, int, int, uchar);

    ///
    /// \brief Symmetry along the height axis.
    ///
    /// Creates a new image containing the symmetry of the current image along the height axis.
    ///
    /// \return Symmetric image along the height axis
    ///
    Image sym_y();

    ///
    /// \brief Symmetry along the diagonal axis.
    ///
    /// Creates a new image containing the symmetry of the current image along the diagonal axis.
    ///
    /// \return Symmetric image along the diagonal axis
    ///
    Image sym_diag();

    ///
    /// \brief Overloaded ooperator to get the matrix
    ///
    /// \return Matrix containing the pixel values
    ///
    Mat operator()();

    ///
    /// \brief Size of the image
    ///
    /// \return Tuple containing the height and the width of the image
    ///
    tuple<int, int> size();   // Return the size (height, width)

    ///
    /// \brief Intensity of the blackest pixel
    ///
    /// \return Int corresponding to the value of the blackest pixel. Caution:
    /// The whitest pixel is value 255 and the blackest one is value 0.
    ///
    int min();

// Main 1
    ///
    /// \brief Barycenter of the image
    ///
    /// Computes the barycenter of black pixels, according to a floor.
    /// It corresponds to the pressure spot.
    ///
    /// \return A tuple containing the coordinates of the barrycenter
    ///
    Point barycenter();

    ///
    /// \brief Applies a specific function to the image
    ///
    /// Applies a specific function c(r) which is equal to 1 at r = 0, and equal to 0 as r rises
    /// This function is applied on every pixel of the image, with r the euclidean distance between the pixel and a certain point.
    ///
    /// \param uint x, uint y : coordinates of the center
    ///
    void coef_iso(uint, uint);

    ///
    /// \brief Applies a specific function to the image
    ///
    /// Applies a specific function c(r) which is equal to 1 at r = 0, and equal to 0 as r rises
    /// This function is applied on every pixel of the image, with r the elliptical distance between the pixel and a certain point,
    /// directed by an angle.
    ///
    /// \param int x0, y0 : coordinates of the center
    /// \param float angle : angle of direction of the ellipse
    ///
    void coef_aniso(int, int, float);

// Starter 2
    ///
    /// \brief Rotates the image with an angle theta around the center of the image.
    /// The size of the image is kept.
    ///
    /// \param float angle : angle of rotation
    ///
    /// \return A new image rotated.
    ///
    Image rotation(float);

// Main 2
    ///
    /// \brief Warp the image around the center of the image.
    /// The size of the image is kept.
    ///
    /// \param const double& theta : basic angle of rotation, which change with the distance from the center
    /// \param const Point& center : Center of rotation
    /// \param const double& a, const double& b : parameters
    ///
    /// \return A new image warped
    ///
    Image warp(const double&, const Point&, const double&, const double&);

// Starter 3
    ///
    /// \brief Apply a convolution filter on the image
    ///
    /// \param Mat filter : Convolution filter to apply on the image. Its height and width must be odd.
    ///
    /// \return A new image convolved
    ///
    Image convolution2D(Mat);

    ///
    /// \brief Apply a convolution filter on the image using FFT
    ///
    /// \param Mat filter : Convolution filter to apply on the image. Its height and width must be odd.
    ///
    /// \return A new image convolved
    ///
    Image FFT(Mat);

    ///
    /// \brief Apply a convolution filter on the image using the separability of the filter
    /// The filter must be of rank 1 to be separable.
    ///
    /// \param Mat filter : Convolution filter to apply on the image. Its height and width must be odd.
    ///
    /// \return A new image convolved
    ///
    Image convolutionSeparable2D(Mat, int);

// Main 3
    /*!
    * \brief Convolve an Image with a spatially variant kernel
    * \param kernel Filter we would like to have far away from center
    * \param center Point where we want identity kernel
    * \return convoluted image
    */
    Image convKernelVariable(Mat, Point);

// Starter 4
    ///
    /// \brief Binarisation of the image
    /// The image is binarised using th Otsu method
    ///
    /// \return A new image binarised
    ///
    Image binarisation();

    ///
    /// \brief Method which computes the Otsu threshold
    ///
    /// \return an integer corresponding to the intensity threshold
    ///
    int threshold_otsu();

    ///
    /// \brief Apply the erosion on a binary image
    ///
    /// \param Mat h : erosion shape.
    ///
    /// \return A new image eroded
    ///
    Image erosion_binary(Mat);

    ///
    /// \brief Apply the dilation on a binary image
    ///
    /// \param Mat h : dilation shape.
    ///
    /// \return A new image dilated
    ///
    Image dilation_binary(Mat);

// Main 4
    ///
    /// \brief Apply the erosion on a grayscale image
    ///
    /// \param Mat h : erosion shape.
    ///
    /// \return A new image eroded
    ///
    Image erosion_grayscale(Mat);

    ///
    /// \brief Apply the dilation on a grayscale image
    ///
    /// \param Mat h : dilation shape.
    ///
    /// \return A new image dilated
    ///
    Image dilation_grayscale(Mat);

// Starter 5
    ///
    /// \brief Computes the absolute error between two images
    ///
    /// \param Image image : image to compare with the original image
    ///
    /// \return A new error image
    ///
    Image Absolute_error(Image);

    ///
    /// \brief Translate the image of px, py. The size of the image is kept
    ///
    /// \param float px, py : number of pixels to translate through the height and the width
    ///
    /// \return A new image translated
    ///
    Image translateImg(float, float);

    ///
    /// \brief Translate the image of px, py using OpenCV function. The size of the image is kept
    ///
    /// \param float px, py : number of pixels to translate through the height and the width
    ///
    /// \return A new image translated
    ///
    Image translateImg_OpenCV(float, float);

    ///
    /// \brief Computes the first loss function
    ///
    /// \param Image image : image to compare with
    ///
    /// \return A float corresponding to the loss function
    ///
    float loss1(Image);

    ///
    /// \brief Computes the mean intensity of the image
    ///
    /// \return A float corresponding to the mean
    ///
    float mean();

    ///
    /// \brief Computes the second loss function
    ///
    /// \param Image image : image to compare with
    ///
    /// \return A float corresponding to the loss function
    ///
    float loss2(Image);

    ///
    /// \brief Computes the num_loss-th loss function
    ///
    /// \param Image image : image to compare with
    /// \param int num_loss : numero of the loss function to use (1 or 2)
    ///
    /// \return A float corresponding to the loss function
    ///
    float loss(Image, int);

    ///
    /// \brief Returns the result of the first loss function and the parameter px that minimalizes the loss function
    /// py is fixed all along
    /// Greedy algorithm
    ///
    /// \param Image image : image to compare with
    /// \param int px_init, py_init : intialized translation to take into account
    ///
    /// \return A float corresponding to the result of the loss function, and the translation px applied
    ///
    tuple<float, int> opti_l_x_1(int, int, Image);

    ///
    /// \brief Returns the optimal parameters py and px when their are two translations using the first loss function
    /// pinitx and pinity are the points where we want to start
    /// greedy algorithm
    ///
    /// \param int pinitx, pinity : initial translation to find faster the good one
    /// \param Image image : image to compare with
    ///
    /// \return Two integers which correspond to the height translation and the width translation
    ///
    tuple<int, int> opti_tx_ty_1(int, int, Image);

    ///
    /// Computes the first loss function in the set [px-ecart, px+ecart]x[py-ecart,py+ecart] and searches
    /// values of translation which minimizes the distance
    ///
    /// \param Image image : image to compare with
    /// \param int px, py : initial translations
    /// \param float erreur : difference we autorize
    ///
    /// \return Two floats which correspond to the height translation and the width translation
    ///
    tuple<float,float> xy_precise_1(Image, int, int, float);


    ///
    /// \brief Returns the optimal parameters rot, py and px when their are a rotation and two translations using the first loss function
    /// theta_init is the initial rotation we apply
    ///
    /// \param float theta_init : initial rotation to find faster the good one
    /// \param Image image : image to compare with
    /// \param float step : step of the angle
    /// \param float limit : limit angle
    /// \param int num_loss : numero of the loss function to use (1 or 2)
    ///
    /// \return A float an two integers which corresponds to the rotation, the height translation and the width translation
    ///
    tuple<float, int, int> opti_rtxy_1(Image, float, float, float);

    ///
    /// \brief Returns the result of the second loss function and the parameter px that minimalizes the loss function
    /// py is fixed all along
    /// Greedy algorithm
    ///
    /// \param Image image : image to compare with
    /// \param int px_init, py_init : intialized translation to take into account
    ///
    /// \return A float corresponding to the result of the loss function, and the translation px applied
    ///
    tuple<float, int> opti_l_x_2(int, int, Image);

    ///
    /// \brief Returns the optimal parameters py and px when their are two translations using the second loss function
    /// pinitx and pinity are the points where we want to start
    /// greedy algorithm
    ///
    /// \param int pinitx, pinity : initial translation to find faster the good one
    /// \param Image image : image to compare with
    ///
    /// \return Two integers which corresponds to the height translation and the width translation
    ///
    tuple<int, int> opti_tx_ty_2(int, int, Image);

    ///
    /// Computes the second loss function in the set [px-ecart, px+ecart]x[py-ecart,py+ecart] and searches
    /// values of translation which minimizes the distance
    ///
    /// \param Image image : image to compare with
    /// \param int px, py : initial translations
    /// \param float erreur : difference we autorize
    ///
    /// \return Two floats which correspond to the height translation and the width translation
    ///
    tuple<float,float> xy_precise_2(Image, int, int, float);

    ///
    /// \brief Returns the optimal parameters rot, py and px when their are a rotation and two translations using the secondloss function
    /// theta_init is the initial rotation we apply
    ///
    /// \param float theta_init : initial rotation to find faster the good one
    /// \param Image image : image to compare with
    /// \param float step : step of the angle
    /// \param float limit : limit angle
    /// \param int num_loss : numero of the loss function to use (1 or 2)
    ///
    /// \return A float an two integers which corresponds to the rotation, the height translation and the width translation
    ///
    tuple<float, int, int> opti_rtxy_2(Image, float, float, float);

    ///
    /// \brief Returns the result of the numloss-th loss function and the parameter px that minimalizes the loss function
    /// py is fixed all along
    /// Greedy algorithm
    ///
    /// \param Image image : image to compare with
    /// \param int px_init, py_init : intialized translation to take into account
    /// \param int num_loss : numero of the loss function to use (1 or 2)
    ///
    /// \return A float corresponding to the result of the loss function, and the translation px applied
    ///
    tuple<float, int> opti_l_x(int, int, Image, int);

    ///
    /// \brief Returns the optimal parameters py and px when their are two translations using the num_loss-th loss function
    /// pinitx and pinity are the points where we want to start
    /// greedy algorithm
    ///
    /// \param int pinitx, pinity : initial translation to find faster the good one
    /// \param Image image : image to compare with
    /// \param int num_loss : numero of the loss function to use (1 or 2)
    ///
    /// \return Two integers which corresponds to the height translation and the width translation
    ///
    tuple<int, int> opti_tx_ty(int, int, Image, int);

    ///
    /// Computes the num_loss-th loss function in the set [px-ecart, px+ecart]x[py-ecart,py+ecart] and searches
    /// values of translation which minimizes the distance
    ///
    /// \param Image image : image to compare with
    /// \param int px, py : initial translations
    /// \param float erreur : difference we autorize
    /// \param int num_loss : numero of the loss function to use (1 or 2)
    ///
    /// \return Two floats which correspond to the height translation and the width translation
    ///
    tuple<float,float> xy_precise(Image, int, int, float, int);

    ///
    /// \brief Returns the optimal parameters rot, py and px when their are a rotation and two translations using the num_loss-th loss function
    /// theta_init is the initial rotation we apply
    ///
    /// \param float theta_init : initial rotation to find faster the good one
    /// \param Image image : image to compare with
    /// \param float step : step of the angle
    /// \param float limit : limit angle
    /// \param int num_loss : numero of the loss function to use (1 or 2)
    ///
    /// \return A float an two integers which corresponds to the rotation, the height translation and the width translation
    ///
    tuple<float, int, int> opti_rtxy(Image, float, float, float, int);

    ///
    /// \brief Computes the differences between the barycenter of the two images
    ///
    /// \param Image image : image to compare with
    ///
    /// \return Two integers which corresponds to the height difference and the width difference
    ///
    tuple<int, int> initial_translation(Image g);

// Main 5

    ///
    /// \brief Computes the first derivative among the height of the image
    ///
    /// \param int x, int y : Pixel to compute the result
    ///
    /// \return A float corresponding to the result
    ///
    float first_derivative_x(int, int);

    ///
    /// \brief Computes the first derivative among the width of the image
    ///
    /// \param int x, int y : Pixel to compute the result
    ///
    /// \return A float corresponding to the result
    ///
    float first_derivative_y(int, int);

    ///
    /// \brief Computes the derivative of the loss function with respect to the height translation
    ///
    /// \param Image g : Image to compare with the loss function
    ///
    /// \return A float corresponding to the result
    ///
    float deriv_transl_x(Image);

    ///
    /// \brief Computes the derivative of the loss function with respect to the width translation
    ///
    /// \param Image g : Image to compare with the loss function
    ///
    /// \return A float corresponding to the result
    ///
    float deriv_transl_y(Image);

    ///
    /// \brief Computes gradient method on the translation only
    ///
    /// \param Image g : Image to compare with
    /// \param float px, py : Intial translation
    /// \param float alpha : initial angle
    /// \param float epsilon : error
    ///
    /// \return Two floats corresponding to the translation
    ///
    tuple<float, float> methode_grad_translation(Image, float, float, float, float);

    ///
    /// \brief Computes the derivative of the loss function with respect to the rotation
    ///
    /// \param Image g : Image to compare with the loss function
    /// \param float theta : angle
    ///
    /// \return A float corresponding to the result
    ///
    float deriv_rotation_theta(Image, float);

    ///
    /// \brief Computes gradient method on the translation and the rotation
    ///
    /// \param Image g : Image to compare with
    /// \param float px, py : Intial translation
    /// \param float alpha_trans, alpha_rot : initial angles for the translation and the rotation
    /// \param float epsilon : error
    ///
    /// \return Two floats corresponding to the translation
    ///
    tuple<float, float,float> methode_grad_translation_rotation(Image, float, float, float, float,float, float);

};

#endif
