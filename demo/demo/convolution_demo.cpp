#include "../include/main.h"


int main(int argc, char** argv )
{
    if( argc != 2)
    {
     cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    // Creation of the image
    Image image = Image(argv[1]);

    // Display the image
    cout << "\nDisplay clean image" << endl;
    image.display();

    // Filter Laplacian of size 3x3
    float a = 1./273;
    float b = 4./273;
    float c = 7./273;
    float d = 16./273;
    float e = 26./273;
    float f = 41./273;

    // Blur filter
    Mat h_blur = (Mat_<float>(5, 5) << a, b, c, b, a,
                                  b, d, e, d, b,
                                  c, e, f, e, c,
                                  b, d, e, d, b,
                                  a, b, c, b, a);

    // Laplacian filter
    Mat h_lapl = (Mat_<float>(5, 5) << 0, 0, -1, 0, 0,
                                  0,-1, -2, -1, 0,
                                  -1, -2, 16, -2, -1,
                                  0, -1, -2, -1, 0,
                                  0, 0, -1, 0, 0);

    Mat h_d = (Mat_<float>(3, 3) << 0, 1, 0,
                                    1, 1, 1,
                                    0, 1, 0);

    // Sobel operator
    Mat h_sobel = (Mat_<float>(5, 5) << 2, 1, 0, -1, -2,
                                  2, 1, 0, -1, -2,
                                  4, 2, 0, -2, -4,
                                  2, 1, 0, -1, -2,
                                  2, 1, 0, -1, -2);



    // Convolution of the image with the filter matrix h
    cout << "\nConvolution with the blur filter..." << endl;
    Image image2 = image.convolution2D(h_blur);

    // Display of the image
    cout << "\nDisplay blurred image" << endl;
    image2.display();


    cout << "\nFFT with blur filter..." << endl;
    Image image_fft = image.FFT(h_blur);

    cout << "\nDisplay FFT blurred image" << endl;
    image_fft.display();

    cout << "\nSeparable convolution with Sobel operator" << endl;
    Image image_sep = image.convolutionSeparable2D(h_sobel, 0);

    cout << "\nDisplay image with Sobel filter" << endl;
    image_sep.display();

    return 0;
}
