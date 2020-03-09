///
/// \file main_5.cpp
/// \brief Methods and functions of the main_5 about optimisation
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"


float Image::first_derivative_x(int x, int y){
    /* finite difference for derivative along x_axis*/
    float res = 0;
    if (x < height - 1 && x > 0){
        res = ( im.at<float>(x + 1, y) - im.at<float>(x - 1, y) )/2;
    }
    else if (x == height - 1){
        res = (im.at<float>(x,y) - im.at<float>(x-1,y));
    }
    else {
        res = (im.at<float>(x + 1, y) - im.at<float>(x, y));
    }

    return res;
}



float Image::first_derivative_y(int x, int y){
    /* finite difference for derivative along y_axis*/
    float res =0;
    if (y < width - 1 && y > 0){
        res =(im.at<float>(x, y + 1) - im.at<float>(x, y - 1))/2;
    }
    else if (y == width - 1){
        res = (im.at<float>(x, y) - im.at<float>(x, y - 1));
    }
    else {
        res = (im.at<float>(x, y + 1) - im.at<float>(x, y));
    }
    return res;
}



float Image::deriv_transl_x(Image g){
    /*Partial derivative of the loss function compared the the translation px*/
    float res = 0;
    Mat im_g = g();
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            res -= (im.at<float>(i,j) - im_g.at<float>(i,j))*g.first_derivative_x(i, j);
        }
    }
    return 2*res;
}



float Image::deriv_transl_y(Image g){
    /*Partial derivative of the loss function compared the the translation py*/
    float res= 0;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            res -= (im.at<float>(i,j) - im.at<float>(i,j))*g.first_derivative_y(i, j);
        }
    }
    return 2*res;
}



tuple<float, float> Image::methode_grad_translation(Image g, float px, float py, float alpha, float epsilon){
    /*grandient method for only translation*/
    Image ginit = g.translateImg_OpenCV(px, py);
    float delta_x = this->deriv_transl_x(ginit);
    float delta_y = this->deriv_transl_y(ginit);

    while ( abs(delta_x) > epsilon || abs(delta_y) > epsilon ){
        px = px + alpha*delta_x;
        py = py + alpha*delta_y;
        Image ginit = g.translateImg_OpenCV(px, py);
        delta_x = this->deriv_transl_x(ginit);
        delta_y = this->deriv_transl_y(ginit);
    }

    return make_tuple(px, py);
}



float Image::deriv_rotation_theta(Image g, float theta){
    /*Partial derivative of the loss function compared the the rotation theta*/
    float res = 0;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            res = res +  (im.at<float>(i,j) - im.at<float>(i,j))*(g.first_derivative_x(i, j)*(cos(theta)*j - sin(theta)*i) + g.first_derivative_y(i, j)*(-cos(theta)*i - sin(theta)*j));
        }
    }
    return -2*res;
}




tuple<float, float,float> Image::methode_grad_translation_rotation(Image g, float px, float py, float theta, float alphatrans,float alpha_rot, float epsilon){
    /*gradient method for translation and rotation*/
    Image ginit = g.rotation(theta);
    ginit = ginit.translateImg_OpenCV(px, py);
    float delta_x = this->deriv_transl_x(ginit);
    float delta_y = this->deriv_transl_y(ginit);
    float delta_theta = this->deriv_rotation_theta(ginit, theta);

    while ( abs(delta_x) > epsilon || abs(delta_y) > epsilon || abs(delta_theta) > epsilon ){
        px = px + alphatrans*delta_x;
        py = py + alphatrans*delta_y;
        theta = theta - delta_theta*alpha_rot;
        ginit = g.rotation(theta);
        ginit = ginit.translateImg_OpenCV(px,py);

        delta_x = this->deriv_transl_x(ginit);
        delta_y = this->deriv_transl_y(ginit);
        delta_theta = this->deriv_rotation_theta(ginit,theta);

    }
    return make_tuple(theta,px,py);
}
