///
/// \file starter_5.cpp
/// \brief Methods and functions of the starter_5 about optimisation
/// \author Bredehoft - Ducros - Iollo - Pourre
/// \version 0.1
/// \date Jan 22 2020
///

#include "../include/main.h"

// Compute the absolute error between the current image and a second one
Image Image::Absolute_error(Image image){
  // Size of the image
  int height_image, width_image;
  tie(height_image, width_image) = image.size();

  // if the size of the two images are not the same, we raise en error
  if (height != height_image || width != width_image) {
    cout << "The two images are not the same size.\nCurrent image : ("
         << height << ", " << width << ")\nOther image : ("
         << height_image << ", " << width_image << ")" << endl;
    return *this;
  }

  // Matrix of the image
  Mat im_image = image();

  // Creation of a new matrix the same size as the second image, full with floats
  Mat im_result = Mat::zeros(height, width, CV_32FC1);

  // For each pixel of the new image, we compute the error between
  // The two pixel intensities of the images
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        im_result.at<float>(i, j) = abs(im.at<float>(i, j) - im_image.at<float>(i, j));
    }
  }

  // Resulting image
  Image result = Image(im_result);
  return result;
}


// Returns a new image translated of px and py
Image Image::translateImg_OpenCV(float px, float py){
    // Translation matrix [1, 0, px
    //                     0, 1, py]
    Mat trans_mat = (Mat_<float>(2,3) << 1, 0, py, 0, 1, px);

    // Copy of the matrix im
    // Mat im_result = im.clone();
    Mat im_result = Mat::zeros(height, width, CV_32FC1);

    warpAffine(im, im_result, trans_mat, im.size(), INTER_LINEAR, BORDER_CONSTANT, 0);
    Image image_result = Image(im_result);

    return image_result;
}

// Returns a new image translated of px and py
Image Image::translateImg(float px, float py){
    // Center of the new cell
    float x_new = 0.5;
    float y_new = 0.5;

    // Initialization of the matrix of the image translated
    Mat im_translated = Mat::zeros(height, width, CV_32FC1);

    // We pass through all pixels in the new image
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){

            //Center of the corresponding old cell
            float y_old = (y_new - py);

            float x_old = (x_new - px);


            //Find overlapping old cells
            int trc_y_old = trunc(y_old);
            int trc_x_old = trunc(x_old);

            // Difference between coordinates of the point coordinates (float)
            // and the actual old point pixel (int)
            float dy = y_old - trc_y_old;
            float dx = x_old - trc_x_old;

            // We check if the old pixel is in the old image
            if (y_old >= 0 && y_old <= width && x_old >= 0 && x_old <= height){

                if (dy > 0.5){
                    if (dx < 0.5){
                        float coef_LL = im.at<float>(trc_x_old, trc_y_old ); //LOW-LEFT
                        float coef_LR = im.at<float>(trc_x_old, trc_y_old + 1); //LOW-RIGHT
                        float coef_UL = im.at<float>(trc_x_old - 1, trc_y_old); //UP-LEFT
                        float coef_UR = im.at<float>(trc_x_old - 1, trc_y_old + 1); //UP-RIGHT

                        im_translated.at<float>(i, j) = coef_LL * (1 - dy + 0.5) * (dx + 0.5)
                        + coef_UR * (dy - 0.5) * (0.5 - dx)
                        + coef_UL * (1 - dy + 0.5) * (0.5 - dx)
                        + coef_LR * (dy - 0.5) * (dx + 0.5);
                    }
                    else if (2*dx == 1) {
                        float coef_L = im.at<float>(trc_x_old, trc_y_old);
                        float coef_R = im.at<float>(trc_x_old, trc_y_old + 1);

                        im_translated.at<float>(i, j) = coef_R * (dy - 0.5)
                        + coef_L * (1 - dy + 0.5);
                    }
                    else {
                        float coef_LL = im.at<float>(trc_x_old + 1, trc_y_old); //LOW-LEFT
                        float coef_LR = im.at<float>(trc_x_old + 1, trc_y_old + 1); //LOW-RIGHT
                        float coef_UL = im.at<float>(trc_x_old, trc_y_old); //UP-LEFT
                        float coef_UR = im.at<float>(trc_x_old, trc_y_old + 1); //UP-RIGHT

                        im_translated.at<float>(i, j) = coef_LL * (1 - dy + 0.5) * (dx - 0.5)
                        + coef_UR * (dy - 0.5) * (1 - dx + 0.5)
                        + coef_UL * (1 - dy + 0.5) * (1 - dx + 0.5)
                        + coef_LR * (dy - 0.5) * (dx - 0.5);
                    }

                }
                else if (dy * 2 == 1) {
                    if (dx < 0.5){
                        float coef_U = im.at<float>(trc_x_old - 1, trc_y_old);
                        float coef_Lo = im.at<float>(trc_x_old, trc_y_old);

                        im_translated.at<float>(i, j) = coef_Lo * (dx + 0.5)
                        + coef_U * (0.5 - dx);
                    }
                    else if (dx * 2 == 1) {
                        float coef_Curr = im.at<float>(trc_x_old, trc_y_old);

                        im_translated.at<float>(i, j) = coef_Curr;
                    }
                    else {
                        float coef_U = im.at<float>(trc_x_old, trc_y_old);
                        float coef_Lo = im.at<float>(trc_x_old + 1, trc_y_old);

                        im_translated.at<float>(i, j) = coef_U * (1 - dx + 0.5)
                        + coef_Lo * (dx - 0.5);
                    }
                }
                else {
                    if (dx <= 0.5){
                        float coef_LL = im.at<float>( trc_x_old, trc_y_old - 1); //LOW-LEFT
                        float coef_LR = im.at<float>( trc_x_old, trc_y_old); //LOW-RIGHT
                        float coef_UL = im.at<float>( trc_x_old - 1, trc_y_old - 1); //UP-LEFT
                        float coef_UR = im.at<float>( trc_x_old - 1, trc_y_old); //UP-RIGHT

                        im_translated.at<float>(i, j) = coef_LL * (0.5 - dy) * (dx + 0.5)
                        + coef_LR * (dy + 0.5) * (dx + 0.5)
                        + coef_UL * (0.5 - dy) * (0.5 - dx)
                        + coef_UR * (dy + 0.5) * (0.5 - dx);
                    }
                    else if (dx*2 == 1) {
                        float coef_L = im.at<float>(trc_x_old, trc_y_old - 1);
                        float coef_R = im.at<float>(trc_x_old, trc_y_old);

                        im_translated.at<float>(i, j) = coef_R * (dy + 0.5)
                        + coef_L * (0.5 - dy);
                    }
                    else {
                        float coef_LL = im.at<float>( trc_x_old + 1, trc_y_old - 1); //LOW-LEFT
                        float coef_LR = im.at<float>( trc_x_old + 1, trc_y_old); //LOW-RIGHT
                        float coef_UL = im.at<float>( trc_x_old, trc_y_old - 1); //UP-LEFT
                        float coef_UR = im.at<float>( trc_x_old, trc_y_old); //UP-RIGHT

                        im_translated.at<float>(i, j) = coef_UL * (0.5 - dy) * (1 - dx + 0.5)
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

    Image image_translation(im_translated);

    return image_translation;
}




///////////////////// LOSS 1

// Computes the first loss functions between the current image and the image image
float Image::loss1(Image image){
  Mat im_image = image();

  // Result of the compute
  float sum = 0;

  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      sum += pow(im.at<float>(i,j)- im_image.at<float>(i,j), 2);
    }
  }

  return sum;
}

// Computes the mean of the image
float Image::mean(){
  float mean = 0;

  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      mean = mean + im.at<float>(i,j);
    }
  }

  return mean/(height*width);
}


// Returns the result of the loss and the parameter px that maximizes the loss function
// py is fixed all along !
// Greedy algorithm
tuple<float, int> Image::opti_l_x_1(int p_init_x, int p_init_y, Image g){

  // Current position px
  int p_x = p_init_x;

  // Initialization of the translated images
  Image g_init = g.translateImg_OpenCV(p_init_x, p_init_y);
  // Image translated one pixel down
  Image g_plus = g.translateImg_OpenCV(p_init_x + 1, p_init_y);
  // Image translated one pixel up
  Image g_moins = g.translateImg_OpenCV(p_init_x - 1, p_init_y);

  // Intialization of the losses
  float loss_init = this->loss(g_init, 1);
  float loss_plus = this->loss(g_plus, 1);
  float loss_moins = this->loss(g_moins, 1);

  // While the offset px is in the image and the loss
  while ( loss_init > 0 ){
    // cout << "loss_init =  " << loss_init << endl;
    // cout << "p_x =  " << p_x << endl;

    // If the loss is better when px rises, we rise it
    if ( loss_plus < loss_init && loss_plus < loss_moins ) {
      p_x += 1;
      // We slide the images
      g_moins = Image(g_init);
      g_init = Image(g_plus);
      g_plus = g_plus.translateImg_OpenCV(1, p_init_y);

      loss_moins = loss_init;
      loss_init = loss_plus;
      loss_plus = this->loss(g_plus, 1);
    }

    else if ( loss_moins < loss_init && loss_moins < loss_plus ){
      p_x = p_x - 1;

      g_plus = Image(g_init);
      g_init = Image(g_moins);
      g_moins = g_moins.translateImg_OpenCV(-1, p_init_y);

      loss_plus = loss_init;
      loss_init = loss_moins;
      loss_moins = this->loss(g_moins, 1);
    }

    else {
      for (int pas = 1; pas < 10; pas++){
        g_plus = g_plus.translateImg_OpenCV(1, p_init_y);
        g_moins = g_moins.translateImg_OpenCV(-1, p_init_y);

        loss_plus = this->loss(g_plus, 1);
        loss_moins = this->loss(g_moins, 1);

        if (loss_plus < loss_init){
          loss_init = loss_plus;
          p_x += pas;
          break;
        }

        else if ( loss_moins < loss_init ){
          loss_init = loss_moins;
          p_x = p_x - pas;
          break;
        }
        else {
          if (pas == 9){
            return make_tuple(loss_init, p_x);
          }
        }
      }
    }
  }

return make_tuple(loss_init, p_x);
}


tuple<int,int> Image::opti_tx_ty_1(int pinitx, int pinity, Image image){
    int px = pinitx;
    int py = pinity;
    int px_inter;
    float linit;
    tie(linit, px_inter) = this->opti_l_x(px, py, image, 1);
    float lplus;
    tie(lplus, px_inter) = this->opti_l_x(px, py + 1, image, 1);
    float lmoins;
    tie(lmoins, px_inter) = this->opti_l_x(px, py - 1, image, 1);

    while (linit > 0){
        //cout<< "linit =  "<< linit<< "lplus =  "<< lplus<< "lmoins =  "<< lmoins<< endl;
        // cout << linit << endl;
        if (lplus < linit && lplus < lmoins){
            py = py + 1;
            lmoins = linit;
            linit = lplus;
            tie(lplus, px_inter) = this->opti_l_x(px, py + 1, image, 1);
            px = px_inter;

        }
        else if (lmoins < linit && lmoins < lplus){
            py = py - 1;
            lplus = linit;
            linit = lmoins;
            tie(lmoins, px_inter) = this->opti_l_x(px, py - 1, image, 1);
            px = px_inter;
        }

        else {
            /* here we just try not to fall in a local maximum*/
            for(int pas = 1; pas < 10; pas++){
                int px_plus;
                int px_moins;
                tie(lplus, px_plus) = this->opti_l_x(px, py + pas, image, 1);
                tie(lmoins,px_moins) = this->opti_l_x(px, py - pas, image, 1);

                if (lplus < linit){
                    linit = lplus;
                    py = py + pas;
                    px = px_plus;
                    break;

                }
                else if (lmoins < linit){
                    linit = lmoins;
                    py = py - pas;
                    px = px_moins;
                    break;
                }
                else{
                  if(pas == 9){
                    return make_tuple(px,py);
                  }
                }
            }

        }

    }
    return make_tuple(px,py);
}

tuple<float,float> Image::xy_precise_1(Image image, int px, int py, float erreur){
  /*erreur represents the difference we autorize, since our algorithm converges toward the right px and py but usually never reach

  we calculate the loss function in the set [px-ecart, px+ecart]x[py-ecart,py+ecart]
  then ecart = ecart /2 and we do it again*/
  int count = 0;
  float ecart = 0.9;
  float realpx = (float)px;
  float realpy = (float)py;
  Image ginit = image.translateImg_OpenCV(px, py);
  float linit = this->loss(ginit, 1);

  while ( linit > erreur && count < 50){
      float pabs = realpx;
      float pord = realpy;

      for (float p = -ecart; p <= ecart; p = p + ecart){
        for (float p2 = -ecart; p2 <= ecart; p2 = p2 + ecart){
          Image ginit = image.translateImg_OpenCV(realpx + p, realpy + p2);
          float ltempo = this->loss(ginit, 1);
          count = count + 1;

          if (ltempo <= linit){
              linit = ltempo;
              pabs = realpx + p;
              pord = realpy + p2;
          }
        }
      }

    realpx = pabs;
    realpy = pord;
    ecart = ecart/2;

  }
  return make_tuple(realpx, realpy);
}


tuple<float, int, int> Image::opti_rtxy_1(Image g, float theta_init, float step, float limit){
    /*this is the algorithm very slow
    works the same way as before, we just add un parameter theta : the rotation angle
    greedy algorithm : for theta, theta + pas, theta - pas, we calculate the optimal px and py and compare the lossfunction*/
    float theta = theta_init;
    Image ginit = g.rotation(theta);
    Image gplus = g.rotation(theta + step);
    Image gmoins = g.rotation(theta - step);
    int pyinit, pxinit;
    tie(pxinit, pyinit) = this->opti_tx_ty(10, -10, ginit, 1);
    int pyplus, pxplus;
    tie(pxplus, pyplus) = this->opti_tx_ty(pxinit, pyinit, gplus, 1);

    int pymoins, pxmoins;
    tie(pxmoins,pymoins) = this->opti_tx_ty(pxinit, pyinit, gmoins, 1);;
    ginit = ginit.translateImg_OpenCV(pxinit, pyinit);
    gplus = gplus.translateImg_OpenCV(pxplus, pyplus);
    gmoins = gmoins.translateImg_OpenCV(pxmoins, pymoins);
    float linit = this->loss(ginit, 1);
    float lplus = this->loss(gplus, 1);
    float lmoins = this->loss(gmoins, 1);

    while ((abs(theta) < limit) && linit > 0){
        // cout << "Theta = "<< theta << ", px = " << pxinit << ", py = " << pyinit << endl;
        //cout<< "linit =  "<< linit<< " lplus " << lplus << " lmoins"<<lmoins<<endl;

        //cout<< "  p =  "<<p<<endl;
        if (lplus < linit && lplus < lmoins){
            gmoins = ginit;
            theta = theta + step;
            gplus = g.rotation(theta);
            pxmoins = pxinit;
            pymoins = pyinit;
            pxinit = pxplus;
            pyinit = pyplus;
            tie(pxplus, pyplus) = this->opti_tx_ty(pxinit, pyinit, gplus, 1);
            gplus = gplus.translateImg_OpenCV(pxplus, pyplus);
            lmoins = linit;
            linit = lplus;
            lplus = this->loss(gplus, 1);

        }
        else if (lmoins < linit && lmoins < lplus){
            gplus = ginit;
            theta = theta - step;
            gmoins = g.rotation(theta);
            pxplus = pxinit;
            pyplus = pyinit;
            pxinit = pxmoins;
            pyinit = pymoins;
            tie(pxmoins,pymoins) = this->opti_tx_ty(pxinit, pyinit, gmoins, 1);
            gmoins = gmoins.translateImg_OpenCV(pxmoins, pymoins);
            lplus = linit;
            linit = lmoins;
            lmoins = this->loss(gmoins, 1);
        }

        else {
            for (float epsilon = 0.1*step; epsilon < 6*0.1*step; epsilon = epsilon + 0.1*step){
                // cout << "epsilon  "<< epsilon << ", theta = "<< theta << endl;
                /*we create the images*/
                gplus = g.rotation(theta + epsilon);
                gmoins = g.rotation(theta - epsilon);
                tie(pxmoins, pymoins) = this->opti_tx_ty(pxinit, pyinit, gmoins, 1);
                tie(pxplus, pyplus) = this->opti_tx_ty(pxinit, pyinit, gplus, 1);
                gplus = gplus.translateImg_OpenCV(pxplus, pyplus);
                gmoins = gmoins.translateImg_OpenCV(pxmoins, pymoins);
                lplus = this->loss(gplus, 1);
                lmoins = this->loss(gmoins, 1);
                // cout << "linit =  " << linit << ", lplus " << lplus << ", lmoins"<< lmoins << endl;
                if (lplus < linit){
                    // linit = lplus;
                    theta = theta + epsilon - step;
                    epsilon = 6*step;
                }
                else if (lmoins < linit){
                    // linit = lmoins;
                    theta = theta - epsilon + step;
                    epsilon = 0.6*step;
                }
                else if (epsilon == 0.5*step){
                    return make_tuple(theta, pxinit, pyinit);
                }
            }
        }
    }
    return make_tuple(theta, pxinit, pyinit);

}

//////////////// LOSS 2

// Computes the second loss functions between the current image and the image image
float Image::loss2(Image image){
  // Initialization of the parameters
  // Sum used in the numerator
  float sum_numerator = 0;
  // First sum used in the denominator, for the current image
  float sum_f_denominator = 0;
  // First sum used in the denominator, for the image image
  float sum_g_denominator = 0;

  // Means of the current image, and the image image
  float f_bar = this->mean();
  float g_bar = image.mean();

  Mat im_image = image();

  // Compute of the sums of the loss between each pixel
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      sum_numerator = sum_numerator + (im.at<float>(i,j) - f_bar) * (im_image.at<float>(i,j) - g_bar);
      sum_f_denominator = sum_f_denominator + pow(im.at<float>(i,j) - f_bar, 2);
      sum_g_denominator = sum_g_denominator + pow(im_image.at<float>(i,j) - g_bar, 2);
    }
  }

  return sum_numerator/pow(sum_f_denominator*sum_g_denominator, 0.5);
}


// Returns the result of the loss and the parameter px that maximizes the loss function
// py is fixed all along !
// Greedy algorithm
tuple<float, int> Image::opti_l_x_2(int p_init_x, int p_init_y, Image g){

  // Current position px
  int p_x = p_init_x;

  // Initialization of the translated images
  Image g_init = g.translateImg_OpenCV(p_init_x, p_init_y);
  // Image translated one pixel down
  Image g_plus = g.translateImg_OpenCV(p_init_x + 1, p_init_y);
  // Image translated one pixel up
  Image g_moins = g.translateImg_OpenCV(p_init_x - 1, p_init_y);

  // Intialization of the losses
  float loss_init = this->loss(g_init, 2);
  float loss_plus = this->loss(g_plus, 2);
  float loss_moins = this->loss(g_moins, 2);

  // While the offset px is in the image and the loss
  while ( loss_init < 1 ){
    // cout << "loss_init =  " << loss_init << endl;
    // cout << "p_x =  " << p_x << endl;

    // If the loss is better when px rises, we rise it
    if ( loss_plus > loss_init && loss_plus > loss_moins ) {
      p_x += 1;
      // We slide the images
      g_moins = Image(g_init);
      g_init = Image(g_plus);
      g_plus = g.translateImg_OpenCV(p_x + 1, p_init_y);

      loss_moins = loss_init;
      loss_init = loss_plus;
      loss_plus = this->loss(g_plus, 2);
    }

    else if ( loss_moins > loss_init && loss_moins > loss_plus ){
      p_x = p_x - 1;

      g_plus = Image(g_init);
      g_init = Image(g_moins);
      g_moins = g.translateImg_OpenCV(p_x - 1, p_init_y);

      loss_plus = loss_init;
      loss_init = loss_moins;
      loss_moins = this->loss(g_moins, 2);
    }

    else {
      for (int pas = 1; pas < 10; pas++){
        g_plus = g.translateImg_OpenCV(p_x + 1, p_init_y);
        g_moins = g.translateImg_OpenCV(p_x - 1, p_init_y);

        loss_plus = this->loss(g_plus, 2);
        loss_moins = this->loss(g_moins, 2);

        if (loss_plus > loss_init){
          loss_init = loss_plus;
          p_x += pas;
          break;
        }

        else if ( loss_moins > loss_init ){
          loss_init = loss_moins;
          p_x = p_x - pas;
          break;
        }
        else {
          if (pas == 9){
            return make_tuple(loss_init, p_x);
          }
        }
      }
    }
  }

return make_tuple(loss_init, p_x);
}


tuple<int,int> Image::opti_tx_ty_2(int pinitx, int pinity, Image image){
    int px = pinitx;
    int py = pinity;
    int px_inter;
    float linit;
    tie(linit,px_inter) = this->opti_l_x(px, py, image, 2);
    float lplus;
    tie(lplus,px_inter) = this->opti_l_x(px, py + 1, image, 2);
    float lmoins;
    tie(lmoins,px_inter) = this->opti_l_x(px, py - 1, image, 2);

    while (linit < 1){
        //cout<< "linit =  "<< linit<< "lplus =  "<< lplus<< "lmoins =  "<< lmoins<< endl;
        // cout << linit << endl;
        if (lplus > linit && lplus > lmoins){
            py = py + 1;
            lmoins =linit;
            linit = lplus;
            tie(lplus, px_inter) = this->opti_l_x(px, py + 1, image, 2);
            px = px_inter;

        }
        else if (lmoins > linit && lmoins > lplus){
            py = py-1;
            lplus = linit;
            linit = lmoins;
            tie(lmoins, px_inter) = this->opti_l_x(px, py - 1, image, 2);
            px = px_inter;
        }

        else {
            /* here we just try not to fall in a local maximum*/
            for (int pas = 1; pas < 10; pas++){
                int px_plus;
                int px_moins;
                tie(lplus, px_plus) = this->opti_l_x(px, py + pas, image, 2);
                tie(lmoins,px_moins) = this->opti_l_x(px, py - pas, image, 2);
                if (lplus > linit){
                    linit = lplus;
                    py = py + pas;
                    px = px_plus;
                    break;

                }
                else if (lmoins > linit){
                    linit = lmoins;
                    py = py - pas;
                    px = px_moins;
                    break;
                }
                else{
                  if(pas == 9){
                    return make_tuple(px,py);
                  }
                }
            }

        }

    }
    return make_tuple(px,py);
}



tuple<float,float> Image::xy_precise_2(Image image, int px, int py, float erreur){
  /*erreur represents the difference we autorize, since our algorithm converges toward the right px and py but usually never reach

  we calculate the loss function in the set [px-ecart, px+ecart]x[py-ecart,py+ecart]
  then ecart = ecart /2 and we do it again*/
  int count = 0;
  float ecart = 0.9;
  float realpx = (float)px;
  float realpy = (float)py;
  Image ginit = image.translateImg_OpenCV(px, py);
  float linit = this->loss(ginit, 2);

  while ( 1 - linit > erreur && count < 50){
      float pabs = realpx;
      float pord = realpy;

      for (float p = -ecart; p <= ecart; p = p + ecart){
        for (float p2 = -ecart; p2 <= ecart; p2 = p2 + ecart){
          Image ginit = image.translateImg_OpenCV(realpx + p, realpy + p2);
          float ltempo = this->loss(ginit, 2);
          count = count + 1;

          if (ltempo >= linit){
              linit = ltempo;
              pabs = realpx + p;
              pord = realpy + p2;
          }
        }
      }

    realpx = pabs;
    realpy = pord;
    ecart = ecart/2;

  }
  return make_tuple(realpx, realpy);
}


tuple<float, int, int> Image::opti_rtxy_2(Image g, float theta_init, float step, float limit){
    /*this is the algorithm very slow
    works the same way as before, we just add un parameter theta : the rotation angle
    greedy algorithm : for theta, theta + pas, theta - pas, we calculate the optimal px and py and compare the lossfunction*/
    float theta = theta_init;
    Image ginit = g.rotation(theta);
    Image gplus = g.rotation(theta + step);
    Image gmoins = g.rotation(theta - step);

    int trans_x_init, trans_y_init;
    tie(trans_x_init, trans_y_init) = this->initial_translation(g);
    cout << trans_x_init << " " << trans_y_init << endl;
    int pyinit, pxinit;
    tie(pxinit, pyinit) = this->opti_tx_ty(trans_x_init, trans_y_init, ginit, 2);
    int pyplus, pxplus;
    tie(pxplus, pyplus) = this->opti_tx_ty(pxinit, pyinit, gplus, 2);

    int pymoins, pxmoins;
    tie(pxmoins,pymoins) = this->opti_tx_ty(pxinit, pyinit, gmoins, 2);;
    ginit = ginit.translateImg_OpenCV(pxinit, pyinit);
    gplus = gplus.translateImg_OpenCV(pxplus, pyplus);
    gmoins = gmoins.translateImg_OpenCV(pxmoins, pymoins);
    float linit = this->loss(ginit, 2);
    float lplus = this->loss(gplus, 2);
    float lmoins = this->loss(gmoins, 2);

    while ((abs(theta) < limit) && linit < 1){
        // cout << "Theta = "<< theta << ", px = " << pxinit << ", py = " << pyinit << endl;
        //cout<< "linit =  "<< linit<< " lplus " << lplus << " lmoins"<<lmoins<<endl;

        //cout<< "  p =  "<<p<<endl;
        if (lplus > linit && lplus > lmoins){
            gmoins = ginit;
            theta = theta + step;
            gplus = g.rotation(theta);
            pxmoins = pxinit;
            pymoins = pyinit;
            pxinit = pxplus;
            pyinit = pyplus;
            tie(pxplus, pyplus) = this->opti_tx_ty(pxinit, pyinit, gplus, 2);
            gplus = gplus.translateImg_OpenCV(pxplus, pyplus);
            lmoins = linit;
            linit = lplus;
            lplus = this->loss(gplus, 2);

        }
        else if (lmoins > linit && lmoins > lplus){
            gplus = ginit;
            theta = theta - step;
            gmoins = g.rotation(theta);
            pxplus = pxinit;
            pyplus = pyinit;
            pxinit = pxmoins;
            pyinit = pymoins;
            tie(pxmoins,pymoins) = this->opti_tx_ty(pxinit, pyinit, gmoins, 2);
            gmoins = gmoins.translateImg_OpenCV(pxmoins, pymoins);
            lplus = linit;
            linit = lmoins;
            lmoins = this->loss(gmoins, 2);
        }

        else {
            for (float epsilon = 0.1*step; epsilon < 6*0.1*step; epsilon = epsilon + 0.1*step){
                // cout << "epsilon  "<< epsilon << ", theta = "<< theta << endl;
                /*we create the images*/
                gplus = g.rotation(theta + epsilon);
                gmoins = g.rotation(theta - epsilon);
                tie(pxmoins, pymoins) = this->opti_tx_ty(pxinit, pyinit, gmoins, 2);
                tie(pxplus, pyplus) = this->opti_tx_ty(pxinit, pyinit, gplus, 2);
                gplus = gplus.translateImg_OpenCV(pxplus, pyplus);
                gmoins = gmoins.translateImg_OpenCV(pxmoins, pymoins);
                lplus = this->loss(gplus, 2);
                lmoins = this->loss(gmoins, 2);
                // cout << "linit =  " << linit << ", lplus " << lplus << ", lmoins"<< lmoins << endl;
                if (lplus > linit){
                    // linit = lplus;
                    theta = theta + epsilon - step;
                    epsilon = 6*step;
                }
                else if (lmoins > linit){
                    // linit = lmoins;
                    theta = theta - epsilon + step;
                    epsilon = 0.6*step;
                }
                else if (epsilon == 0.5*step){
                    return make_tuple(theta, pxinit, pyinit);
                }
            }
        }
    }
    return make_tuple(theta, pxinit, pyinit);

}



///////////////// Tous les loss

float Image::loss(Image image, int num_loss){
  if (num_loss == 1) {
    return this->loss1(image);
  }
  return this->loss2(image);
}



tuple<float, int> Image::opti_l_x(int p_init_x, int p_init_y, Image g, int num_loss){
  if (num_loss == 1) {
    return this->opti_l_x_1(p_init_x, p_init_y, g);
  }
  return this->opti_l_x_2(p_init_x, p_init_y, g);
}



tuple<int, int> Image::opti_tx_ty(int p_init_x, int p_init_y, Image g, int num_loss){
  if (num_loss == 1) {
    return this->opti_tx_ty_1(p_init_x, p_init_y, g);
  }
  return this->opti_tx_ty_2(p_init_x, p_init_y, g);
}


tuple<float,float> Image::xy_precise(Image image, int px, int py, float erreur, int num_loss){
  if (num_loss == 1) {
    return this->xy_precise_1(image, px, py, erreur);
  }
  return this->xy_precise_2(image, px, py, erreur);
}



tuple<float, int, int> Image::opti_rtxy(Image g, float theta_init, float step, float limit, int num_loss){
  if (num_loss == 1) {
    return this->opti_rtxy_1(g, theta_init, step, limit);
  }
  return this->opti_rtxy_2(g, theta_init, step, limit);
}



tuple<int, int> Image::initial_translation(Image g){
  Point bary_curr = this->barycenter();
  Point bary_g = g.barycenter();

  int trans_x_init, trans_y_init;
  trans_x_init = bary_curr.x - bary_g.x;
  trans_y_init = bary_curr.y - bary_g
  .y;

  return make_tuple(trans_x_init, trans_y_init);
}
