#include "../include/main.h"

int main(int argc, char** argv )
{
    if( argc != 2)
    {
     cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Image image_clean = Image(argv[1]);

    Image image_tx = Image("../data/tx_finger.png");
    Image image_txy = Image("../data/txy_finger.png");
    Image image_rtxy = Image("../data/rtxy_finger.png");
    // Display of the image

    cout << "\nDisplay clean image" << endl;
    image_clean.display();

    cout << "\nDisplay rotated and translated image" << endl;
    image_rtxy.display();


    // // New image error
    // Image image_error = image.Absolute_error(image2);
    // // Display of the rotated image
    // image_error.display();

    // // New image translated
    // Image image_trans = image.translateImg(10, 10);
    // // Display
    // image_trans.display();

    // // Loss between two images
    // cout << image.loss2(image2) << endl;


    //////////// Starter optimisation
    float loss, rot;
    int px, py;

    // cout << "\nOpti x..." << endl;
    // tie(loss, px) = image.opti_l_x(0, 0, image_tx, 2);
    // cout << "Loss on x: " << loss << endl;
    // cout << "px: " << px << endl;
    //
    //
    // // tx ty
    // cout << "\nOpti tx ty..." << endl;
    // tie(px, py) = image_txy.opti_tx_ty(0, 0, image_clean, 1);
    // cout << "px: " << px << endl;
    // cout << "py: " << py << endl;
    //
    // Image error_txy = image_txy.Absolute_error(image_clean.translateImg_OpenCV(px, py));
    // error_txy.display();

    // rtxy
    cout << "\nOpti r tx ty..." << endl;
    chrono::time_point<std::chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    tie(rot, px, py) = image_rtxy.opti_rtxy(image_clean, -0.2, 0.05, PI, 2);
    end = chrono::system_clock::now();

    int elapsed_milliseconds = chrono::duration_cast<chrono::milliseconds>
                             (end-start).count();
    int elapsed_seconds = elapsed_milliseconds / 1000;
    cout << "Elapsed time: " << elapsed_seconds << "s " << elapsed_milliseconds - elapsed_seconds * 1000 << "ms" << endl;
    cout << "Rot: " << rot << endl;
    cout << "px: " << px << endl;
    cout << "py: " << py << endl;

    cout << "\nRotation..." << endl;
    Image image_rot = image_clean.rotation(rot);

    cout << "\nTranslation..." << endl;
    Image image_trans_rot = image_rot.translateImg_OpenCV(px, py);

    cout << "\nDisplay of the rotated the translated image" << endl;
    image_trans_rot.display();

    cout << "\nAbsolute error..." << endl;
    Image error_trans_rot = image_trans_rot.Absolute_error(image_rtxy);

    cout << "\nDisplay of the error" << endl;
    error_trans_rot.display();


    float px_prec, py_prec;

    tie(px_prec, py_prec) = image_rtxy.xy_precise(image_trans_rot, 0, 0, 0, 2);
    cout << "px precise: " << px_prec << endl;
    cout << "py precise: " << py_prec << endl;



    cout << "\nEnd" << endl;
    // float realpx, realpy;
    // cout << "\nPrecise translation..." << endl;
    // tie(realpx,realpy) = image_txy.xy_precise(image_clean, px, py, 0.001, 1);
    // cout << "Real px: " << realpx << endl;
    // cout << "Real py: " << realpy << endl;

    // image_trans = image_clean.translateImg_OpenCV(realpx, realpy);

    // image_trans.display();

    // Save of the image
    // im_rot.save("rotation_wrap");

    return 0;
}
