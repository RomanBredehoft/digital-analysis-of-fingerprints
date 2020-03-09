///
/// \file starter_2.cpp
/// \brief Methods and functions of the starter_2 about rotation
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"

///
/// \brief Pixel rotation
///
/// Computes the new pixel coordinates after a rotation of angle theta.
///
/// \param Point p : Coordinates of the point to apply the rotation
/// \param float theta : Angle of rotation.
///
/// \return New point after rotation of angle theta
///
Point rotation_pixel(Point p, float theta){
    Point p_rotated(p.x * cos( theta ) - p.y * sin( theta ), p.x * sin( theta ) + p.y * cos( theta ));
    return p_rotated;
}


// Method which applies a rotation to the image around the center of the image
Image Image::rotation(float theta){
    // Center of the new cell
    float x_new = 0.5;
    float y_new = 0.5;

    // New dimension of the window, according to the algorithm
    int NewHeight, NewWidth;

    // New Matrix large enough
    NewHeight = (int)( width * abs(sin(theta)) + height * abs(cos(theta)) );
    NewWidth = (int)( width * abs(cos(theta))  + height * abs(sin(theta)) );

    // Initialization of the matrix of the image rotated
    Mat im_rotated = Mat::zeros(NewHeight, NewWidth, CV_32FC1);

    // We pass through all pixels in the new image
    for (int i = 0; i < NewHeight ; i++){
        for (int j = 0; j < NewWidth ; j++){

            // Center of the corresponding old cell
            float y_old = (y_new - NewWidth / 2) * cos(theta) -
                           (x_new - NewHeight / 2) * sin(theta) +
                            width / 2;

            float x_old = (y_new - NewWidth / 2) * sin(theta) +
                            (x_new - NewHeight / 2) * cos(theta) +
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

    // Resized image with the same center as the first image
    Mat im_rotated_resized = Mat::zeros(height, width, CV_32FC1);

    int ecart_h = (NewHeight - (int)height)/2;
    int ecart_w = (NewWidth - (int)width)/2;

    int max_h = max((int)height, NewHeight);
    int max_w = max((int)width, NewWidth);

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {

        if ( i + ecart_h >= 0 && j + ecart_w >= 0
          && i < max_h - ecart_h  && j < max_w - ecart_w) {
            im_rotated_resized.at<float>(i, j) = im_rotated.at<float>(i + ecart_h, j + ecart_w);
        }

      }
    }

    Image image_rotated_resized = Image(im_rotated_resized);
    return image_rotated_resized;
}
