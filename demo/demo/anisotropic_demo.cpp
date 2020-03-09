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

    // Angle of direction
    float alpha = PI/2 - 0.15;

    // Application of the coefficient function
    cout << "\nAnisotropic function on clean image" << endl;
    image.coef_aniso(x_p, y_p, alpha);

    // Display of the transformed image
    cout << "\nDisplay transformed image" << endl;
    image.display();

    // // Save of the image
    // string alpha_str = to_string(alpha);
    // image.save("anisotropic_" + alpha_str);

    return 0;
}
