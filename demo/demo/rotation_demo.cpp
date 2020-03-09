#include "../include/main.h"

int main(int argc, char** argv )
{
    if( argc != 2)
    {
     cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Image image = Image(argv[1]);

    // Display of the image
    cout << "\nDisplay clean image" << endl;
    image.display();
    //
    // New image rotated
    cout << "\nRotation..." << endl;
    float angle = PI;
    Image image_rot = image.rotation(angle);

    // Display of the rotated image
    cout << "\nDisplay rotated image" << endl;
    image_rot.display();

    // // Save of the image
    // image_rot.save("rotation_wrap");

    cout << "\nWarp..." << endl;
    Image image_warp = image.warp(-0.4, image.barycenter(), 60, 2);

    cout << "\nDisplay warped image" << endl;
    image_warp.display();

    return 0;
}
