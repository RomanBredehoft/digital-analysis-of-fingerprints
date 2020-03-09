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
    cout << "\nDisplay clear image" << endl;
    image.display();

    // Symmetry of the image along the y axis
    cout << "\nWidth symmetry..." << endl;
    Image image_y = image.sym_y();

    cout << "\nDisplay symmetric image" << endl;
    image_y.display();

    // image_y.save("starter1_sym_y");

    // Symmetry of the image along the diagonal
    cout << "\nDiagonal symmetry..." << endl;
    Image image_diag = image.sym_diag();

    cout << "\nDisplay symmetric image" << endl;
    image_diag.display();

    // image_diag.save("starter1_sym_diag");

    return 0;
}
