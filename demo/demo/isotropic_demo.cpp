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

    // Coordinates of the pressure spot
    Point barycentre = image.barycenter();
    int x_p = barycentre.x;
    int y_p = barycentre.y;

    // Application of the coefficient function
    cout << "\nIsotropic function on clean image" << endl;
    image.coef_iso(x_p, y_p);

    // Display of the transformed image
    cout << "\nDisplay transformed image" << endl;
    image.display();

    // // Save of the image
    // image.save("isotropic");

    return 0;
}
