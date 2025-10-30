
void setup() {
    textFont("fixed_8x8_font"); //Function to define the typography (font) we will use, textFont("name of the predefined font in the library")
    //Name of predefined fonts: fixed_8x8_font , fixed_8x16_font ,	variable_8x8_font ,	variable_8x16_font , variable_16x16_font , fixed_32x64_font
}

void draw() {
    background(0,0,0); //function to change background color, background(red values ​​from 0 to 31, green values ​​from 0 to 31, blue values ​​from 0 to 31)
    fill(31,31,31); //Function to change the fill color of objects, fill(red values ​​from 0 to 31, green values ​​from 0 to 31, blue values ​​from 0 to 31)
    textAlign(LEFT); //The function to change the text alignment. It can be: LEFT, CENTER, RIGHT, or NORMAL
    text("Hello World!!!", 40, 20); //function to draw text on the screen, text("text between double quotes", x-position, y-position)
}

void buttons(){ //function to press events
   
}