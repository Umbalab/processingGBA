//incluir sprites da imagem personalizada.
#include "textures/myImage.c"

PImage CustomImage;
void setup() {
}

void draw() {
    image(160, 106, 0, 0, myImage, CustomImage.frame); //draw a costum image, image(imageWidth, imageHeight, x Position on Screen, y Position on Screen, ImageGeneratedFileName, PImageName.frame)
}

void buttons(){ //function to press events
}