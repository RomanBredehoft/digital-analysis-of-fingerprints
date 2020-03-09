///
/// \file main_2.cpp
/// \brief Methods and functions of the starter_2 about warp
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"

///
/// \brief Customs the angle of rotation accordinf to the distance from the center
/// of rotation and paramters
///
/// \param double theta : Basic angle of rotation
/// \param Point RotationCenter : Center of the rotation
/// \param Point Current : Pixel on which to apply the warp
/// \param double a, double b : parameters
/// \return A new image rotated.
///
double CustomTheta(double theta, Point RotationCenter, Point Current, double a, double b){
    double dist = sqrt((Current.x-RotationCenter.x)*(Current.x-RotationCenter.x)
            + (Current.y-RotationCenter.y)*(Current.y-RotationCenter.y));
    double nTheta = theta*exp(-pow(dist/a, b));
        return nTheta;
}


// Method which applies a rotation to the image around the center of the image
Image Image::warp(const double& theta, const Point& center, const double& a, const double& b){
    // Center of the new cell
    float x_new = 0.5;
    float y_new = 0.5;
    int width = im.size().width;
    int heigth = im.size().height;
    double Ntheta;
    // Initialization of the matrix of the image rotated
    Mat im_rotated = Mat::zeros(height, width, CV_32FC1);

    // We pass through all pixels in the new image
    for (int i = 0; i < height ; i++){
        for (int j = 0; j < width ; j++){
            // We take a specifi theta for the warp
            Ntheta = CustomTheta(theta, center, Point(x_new, y_new), a, b);
            // Center of the corresponding old cell
            float y_old = (y_new - width / 2) * cos(Ntheta) -
                           (x_new - height / 2) * sin(Ntheta) +
                            width / 2;

            float x_old = (y_new - width/ 2) * sin(Ntheta) +
                            (x_new - height / 2) * cos(Ntheta) +
                             height / 2;

            // Find overlapping old cells
            int trc_y_old = trunc(y_old);
            int trc_x_old = trunc(x_old);

            // Difference between coordinates of the point coordinates (float)
            // and the actual old point pixel (int)
            float dy = y_old - trc_y_old;
            float dx = x_old - trc_x_old;

            // We check if the old pixel is in the old image
            if (y_old >= 0 && y_old <= width && x_old >= 0 && x_old <= height){

                // According to the case, the Low Left (LL), Low Right (LR), Up Left (UL) and Up Right (UR)
                // pixels to take into account are not the same, and the calculus are not the same
                if (dy > 0.5){
                    // Configuration of this case
                    //  ---------------------------
                    // |<----------->|             |
                    // |      1      |             |
                    // |             |             |
                    // |    -------------          |
                    // |   |         |   |         |
                    // | --|-------------|-------- |
                    // |   |    dx|  |   |         |
                    // |<--|----->°  |   |         |
                    // |   | dy      |   |         |
                    // |    -------------          |
                    // |             |             |
                    //  ---------------------------
                    if (dx < 0.5){
                        float coef_LL = im.at<float>(trc_x_old, trc_y_old ); //LOW-LEFT
                        float coef_LR = im.at<float>(trc_x_old, trc_y_old + 1); //LOW-RIGHT
                        float coef_UL = im.at<float>(trc_x_old - 1, trc_y_old); //UP-LEFT
                        float coef_UR = im.at<float>(trc_x_old - 1, trc_y_old + 1); //UP-RIGHT

                        im_rotated.at<float>(i, j) = coef_LL * (1 - dy + 0.5) * (dx + 0.5)
                            + coef_UR * (dy - 0.5) * (0.5 - dx)
                            + coef_UL * (1 - dy + 0.5) * (0.5 - dx)
                            + coef_LR * (dy - 0.5) * (dx + 0.5);
                    }

                    //  ---------------------------
                    // |             |             |
                    // |             |             |
                    // |             |             |
                    // |             |             |
                    // |             |             |
                    // | -- ------------- -------- |
                    // |   |         |   |         |
                    // |   |         |   |         |
                    // |   |      °  |   |         |
                    // |   |         |   |         |
                    // |   |         |   |         |
                    //  --- ------------- ---------
                    else if (2*dx == 1) {
                      float coef_L = im.at<float>(trc_x_old, trc_y_old); // Left
                      float coef_R = im.at<float>(trc_x_old, trc_y_old + 1); // Right

                      im_rotated.at<float>(i, j) = coef_R * (dy - 0.5)
                                                 + coef_L * (1 - dy + 0.5);
                    }

                    //  ---------------------------
                    // |             |             |
                    // |    -------------          |
                    // |   |         |   |         |
                    // |   |         |   |         |
                    // |   |      °      |         |
                    // | --|-------------|-------- |
                    // |   |         |   |         |
                    // |    -------------          |
                    // |             |             |
                    // |             |             |
                    // |             |             |
                    //  ---------------------------
                    else {
                        float coef_LL = im.at<float>(trc_x_old + 1, trc_y_old); //LOW-LEFT
                        float coef_LR = im.at<float>(trc_x_old + 1, trc_y_old + 1); //LOW-RIGHT
                        float coef_UL = im.at<float>(trc_x_old, trc_y_old); //UP-LEFT
                        float coef_UR = im.at<float>(trc_x_old, trc_y_old + 1); //UP-RIGHT

                        im_rotated.at<float>(i, j) = coef_LL * (1 - dy + 0.5) * (dx - 0.5)
                            + coef_UR * (dy - 0.5) * (1 - dx + 0.5)
                            + coef_UL * (1 - dy + 0.5) * (1 - dx + 0.5)
                            + coef_LR * (dy - 0.5) * (dx - 0.5);
                    }

                }
                else if (dy * 2 == 1) {

                  //  ---------------------------
                  // |             |             |
                  // |             |             |
                  // |             |             |
                  // |-------------|             |
                  // |             |             |
                  // |---------------------------|
                  // |             |             |
                  // |      °      |             |
                  // |             |             |
                  // |-------------|             |
                  // |             |             |
                  //  ---------------------------
                  if (dx < 0.5){
                      float coef_U = im.at<float>(trc_x_old - 1, trc_y_old);
                      float coef_Lo = im.at<float>(trc_x_old, trc_y_old);

                      im_rotated.at<float>(i, j) = coef_Lo * (dx + 0.5)
                                                 + coef_U * (0.5 - dx);
                  }

                  //  ---------------------------
                  // |             |             |
                  // |             |             |
                  // |      °      |             |
                  // |             |             |
                  // |             |             |
                  // |---------------------------|
                  // |             |             |
                  // |             |             |
                  // |             |             |
                  // |             |             |
                  // |             |             |
                  //  ---------------------------
                  else if (dx * 2 == 1) {
                    float coef_Curr = im.at<float>(trc_x_old, trc_y_old);

                    im_rotated.at<float>(i, j) = coef_Curr;
                  }

                  //  ---------------------------
                  // |             |             |
                  // |-------------|             |
                  // |             |             |
                  // |             |             |
                  // |      °      |             |
                  // |---------------------------|
                  // |             |             |
                  // |-------------|             |
                  // |             |             |
                  // |             |             |
                  // |             |             |
                  //  ---------------------------
                  else {
                      float coef_U = im.at<float>(trc_x_old, trc_y_old);
                      float coef_Lo = im.at<float>(trc_x_old + 1, trc_y_old);

                      im_rotated.at<float>(i, j) = coef_U * (1 - dx + 0.5)
                                                 + coef_Lo * (dx - 0.5);
                  }
                }
                else {

                  //  ---------------------------
                  // |             |             |
                  // |             |             |
                  // |             |             |
                  // |          -------------    |
                  // |         |   |         |   |
                  // | --------|-------------|-- |
                  // |         |   |         |   |
                  // |         |   |   °     |   |
                  // |         |   |         |   |
                  // |          -------------    |
                  // |             |             |
                  //  ---------------------------
                    if (dx <= 0.5){
                        float coef_LL = im.at<float>( trc_x_old, trc_y_old - 1); //LOW-LEFT
                        float coef_LR = im.at<float>( trc_x_old, trc_y_old); //LOW-RIGHT
                        float coef_UL = im.at<float>( trc_x_old - 1, trc_y_old - 1); //UP-LEFT
                        float coef_UR = im.at<float>( trc_x_old - 1, trc_y_old); //UP-RIGHT

                        im_rotated.at<float>(i, j) = coef_LL * (0.5 - dy) * (dx + 0.5)
                            + coef_LR * (dy + 0.5) * (dx + 0.5)
                            + coef_UL * dy * (1-dx)
                            + coef_UR * (dy + 0.5) * (0.5 - dx);
                    }

                    //  ---------------------------
                    // |             |             |
                    // |             |             |
                    // |             |             |
                    // |             |             |
                    // |             |             |
                    // | -------- ------------- -- |
                    // |         |   |         |   |
                    // |         |   |         |   |
                    // |         |   |   °     |   |
                    // |         |   |         |   |
                    // |         |   |         |   |
                    //  --------- ------------- ---
                    else if (dx*2 == 1) {
                      float coef_L = im.at<float>(trc_x_old, trc_y_old - 1);
                      float coef_R = im.at<float>(trc_x_old, trc_y_old);

                      im_rotated.at<float>(i, j) = coef_R * (dy + 0.5)
                                                 + coef_L * (0.5 - dy);
                    }

                    //  ---------------------------
                    // |             |             |
                    // |          -------------    |
                    // |         |   |         |   |
                    // |         |   |         |   |
                    // |         |   |   °     |   |
                    // | --------|-------------|-- |
                    // |         |   |         |   |
                    // |          -------------    |
                    // |             |             |
                    // |             |             |
                    // |             |             |
                    //  ---------------------------
                    else {
                        float coef_LL = im.at<float>( trc_x_old + 1, trc_y_old - 1); //LOW-LEFT
                        float coef_LR = im.at<float>( trc_x_old + 1, trc_y_old); //LOW-RIGHT
                        float coef_UL = im.at<float>( trc_x_old, trc_y_old - 1); //UP-LEFT
                        float coef_UR = im.at<float>( trc_x_old, trc_y_old); //UP-RIGHT

                        im_rotated.at<float>(i, j) = coef_UL * (0.5 - dy) * (1 - dx + 0.5)
                            + coef_UR * (dy + 0.5) * (1 - dx + 0.5)
                            + coef_LL * (0.5 - dy) * (dx - 0.5)
                            + coef_LR * (dy + 0.5) * (dx - 0.5);
                    }
                }
            }
            y_new += 1;
        }
        y_new = 0.5;
        x_new += 1;
    }

    Image image_rotated = Image(im_rotated);
    return image_rotated;
}
