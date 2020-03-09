#include "../include/main.h"

int main(int argc, char** argv) {
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

  Mat h = (Mat_<float>(3, 3) << 0, 1, 0,
                                1, 1, 1,
                                0, 1, 0);

  // Mat h = (Mat_<float>(5, 5) << 0, 1, 1, 1, 0,
  //                               1, 1, 1, 1, 1,
  //                               1, 1, 0, 1, 1,
  //                               1, 1, 1, 1, 1,
  //                               0, 1, 1, 1, 0);


  // BINARY

  // Eroded image
  cout << "\nErosion..." << endl;
  Image image_eroded_binary = image.erosion_binary(h);

  cout << "\nDisplay eroded image" << endl;
  image_eroded_binary.display();
  // image_eroded_binary.save("erosion_binary");


  // Dilated image
  cout << "\nDilation..." << endl;
  Image image_dilated_binary = image.dilation_binary(h);

  cout << "\nDisplay dilated image" << endl;
  image_dilated_binary.display();
  // image_dilated_binary.save("dilated_binary");


  // Closing image
  cout << "\nClosing..." << endl;
  Image image_closing_binary = image_dilated_binary.erosion_binary(h);

  cout << "\nDisplay dilated image" << endl;
  image_closing_binary.display();
  // image_closing_binary.save("closing_binary");


  // Opening image
  cout << "\nOpening..." << endl;
  Image image_opening_binary = image_eroded_binary.dilation_binary(h);

  cout << "\nDisplay dilated image" << endl;
  image_opening_binary.display();
  // image_opening_binary.save("opening_binary");



  // GRAYSCALE
  cout << "\nGrayscale erosion..." << endl;
  Image image_eroded_grayscale = image.erosion_grayscale(h);

  cout << "\nDisplay grayscale eroded image" << endl;
  image_eroded_grayscale.display();


  cout << "\nGraysale dilation..." << endl;
  Image image_dilated_grayscale = image.dilation_grayscale(h);

  cout << "\nDisplay grayscale dilated image" << endl;
  image_dilated_grayscale.display();

  return 0;
}
