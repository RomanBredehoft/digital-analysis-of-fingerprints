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
    image.display();

    // Height and width of the image
    int height, width;
    tie(height, width) = image.size();

    // Creation of rectangles black and white on the image
    Image image2 = image.rectangle(height/4, width/4, 3*height/4, width/2, 0);
    image2 = image2.rectangle(8*height/10, 1*width/5, 9*height/10, 4*width/5, 255);

    // Display of the image
    image2.display();

    // Save of the image
    image2.save("starter1_rectangles");

    return 0;
}
