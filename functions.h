/*
To be clear, to code this functions and variables, I used the following:

The files provided by [3DSage](https://github.com/3DSage/GBA_Mode_5_Starter)
and the video tutorial developed by [3DSage](https://www.youtube.com/watch?v=6ecgELrwAnQ&t=323s), which was very explanatory.

[The Mode 5 example file from gbadev.org](https://www.gbadev.org/demos.php?showinfo=323)

[Tonc (coranac) explanations](https://www.coranac.com/tonc/text/bitmaps.htm)

[Patater explanations](https://www.patater.com/gbaguy/gba/ch5.htm).

Explanations and tools contained in [gbadev](https://gbadev.org/index.php).

It wasn't effective by far, but some times, AI chatbots assisted me. I used [chatgpt3](https://chatgpt.com/) and [Meta AI](https://www.meta.ai/).
At great cost, these chatbots helped me fix certain bugs in the code, as well as help me find alternatives for some of the functions, so they should be mentioned here.

All project typography was taken from the [Butano project](https://github.com/GValiente/butano).

The majority of the functions developed are based on processing.org [functions](https://processing.org/reference/).

Likewise, the examples were adapted from the example library that comes with the [Processing IDE download](https://processing.org/download).

Among other reference materials, I highlight the [gbadev website] <https://www.gbadev.org/about.php>

Hugs from Curitiba (PR), Brazil,

Mateus F. Lima Pelanda, [Umbalab](https://umbalab.art/)

July 21, 2025 - last update October 27, 2025
*/

//---Inclusion of libraries for logical operations---
#include <stdio.h>
#include <stdlib.h>

//---Rename unsigned short and long---
typedef uint8_t u8;
typedef unsigned short u16;
typedef unsigned long  u32;

//---Include font sprites---
#include "C:/ProcessingGBA/fonts.h"
//---Include image sprites---
#include "C:/ProcessingGBA/images.h"

//---Variables already initialized for assigning screen position in x, y, z, and w. They can be freely used for other purposes---
int positionX = 0;
int positionY = 0;
int positionZ = 0;
int positionW = 0;
int positionK = 0;

//--Screen config
//Video buffer
u16    *VRAM;
#define VRAM_F         (u16*)0x6000000
#define VRAM_B         (u16*)0x600A000
#define DISPCNT        *(u32*)0x4000000
#define BACKB   	   0x10
volatile u16* Scanline=(volatile u16*)0x4000006;
//Scale mode 5 screen
#define REG_BG2PA *(volatile unsigned short *)0x4000020
#define REG_BG2PD *(volatile unsigned short *)0x4000026
//Screen scale (uniform)
float screenZoomScale = 1.5; // 1.0 = no zoom, 1.25 = full screen in height, 1.5 = full screen in width, 2.0 = pixel perfect full screen
//Update screen scale
void screenScale() {
    if (screenZoomScale <= 0.0f) screenZoomScale = 1.0f; //Protection for valid scale
    int fixedValue = (int)(256.0f / screenZoomScale + 0.5f); //Convert: we want REG_BG2PA/PD = 256 / Screen Scale (fixed 8.8)
    // Define uniform scale (X and Y)
    REG_BG2PA = (u16)fixedValue;
    REG_BG2PD = (u16)fixedValue;
    // Pixel offsets to center (can be negative if larger than screen)
    int offsetX_px = 0;
    int offsetY_px = 0;
    // REG_BG2X / REG_BG2Y are 32-bit fixed 8.8 (write to >>8)
    // (offset << 8) to convert pixels -> fixed 8.8
    *(volatile int*)0x4000028 = (offsetX_px << 8); // REG_BG2X
    *(volatile int*)0x400002C = (offsetY_px << 8); // REG_BG2Y
}
//Strech view size
void stretchView(int vw, int vh){
    REG_BG2PA=171+vw; //para deixar pixel perfeito, manter 128
    REG_BG2PD=169+vh; //para deixar pixel perfeito, manter 128
}
//Screen variables
//#define  GBA_SW 160 //actual gba screen width
#define  SW     160 //game screen width //160
#define  SH     106 //game screen height //maximum size is 128
//Variables that capture screen size
const int width = SW;
const int displayWidth = width;
//Total height of the display
const int height = SH;
const int displayHeight = height;
//Timer for FPS
#define REG_TM2D       *(volatile u16*)0x4000108
int FPS=0; //Variable for store frames per second
int lastFr=0; //last frame
//Frame counter 0 to 2000.
int frameCount = 0;
void incrementFrameCount() {
    frameCount++;
    if (frameCount >= 2000) {
        frameCount = 0;
    }
}
//Fast Iwram and Ewram
#define IN_IWRAM       __attribute__ ((section (".iwram")))
#define IN_EWRAM       __attribute__ ((section (".ewram")))

//---Buttons---

#define TRUE  1
#define FALSE 0
// Button state
u16 keyCurrent = 0;
u16 keyPrevious = 0;
#define REG_KEYINPUT (*(volatile u16*)0x04000130)
#define KEY_MASK 0x03FF  // 10 bits (A, B, Select, Start, Right, Left, Up, Down, R, L)
void updateKeys() {
    keyPrevious = keyCurrent;
    keyCurrent = ~REG_KEYINPUT & KEY_MASK; // Invertido: 0 = pressionado
}

int keyPressed(unsigned short key) {
    return (keyCurrent & key) && !(keyPrevious & key);
}

int keyHeld(unsigned short key) {
    return (keyCurrent & key);
}

int keyReleased(unsigned short key) {
    return !(keyCurrent & key) && (keyPrevious & key);
}

#define KEY_A   (1 << 0)
#define KEY_B   (1 << 1)
#define KEY_SL  (1 << 2)
#define KEY_ST  (1 << 3)
#define KEY_R   (1 << 4)
#define KEY_L   (1 << 5)
#define KEY_U   (1 << 6)
#define KEY_D   (1 << 7)
#define KEY_RS  (1 << 8)
#define KEY_LS  (1 << 9)

//---Color---
#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10)) //15 bit, 0-31, 5bit=r, 5bit=g, 5bit=b
u16 strokeColor = RGB(0,0,0); //Global variable to store the current stroke color
u16 fillColor = RGB(31,31,31); //Global variable to store the current fill color
int isStrokeEnabled = 1; //Variable to check if the stroke is enabled
int isFillEnabled = 1; //Variable to check if filling is enabled
#define RGBMODE 0 //Set color mode to RGB
#define HUEMODE 1 //Set color mode to HUE
u8 colorModeCurrent = RGBMODE; // Variable that stores the current color mode
//Function to set the color mode
void colorMode(u8 mode) {
  if (mode == RGBMODE || mode == HUEMODE) {
    colorModeCurrent = mode;
  }
}
//Function to convert HUE to RGB
void hueToRgb(u16 hue, u16 *r, u16 *g, u16 *b) {
  if (hue < 10.67) {
    *r = hue * 3;
    *g = 31 - hue * 3;
    *b = 0;
  } else if (hue < 21.33) {
    *r = 31 - (hue - 10.67) * 3;
    *g = (hue - 10.67) * 3;
    *b = 0;
  } else {
    *r = 0;
    *g = 31 - (hue - 21.33) * 3;
    *b = (hue - 21.33) * 3;
  }
}
//Function to set stroke color
void stroke(u16 r, u16 g, u16 b) {
    if (colorModeCurrent == HUEMODE) {
    u16 rgb[3];
    hueToRgb(r, &rgb[0], &rgb[1], &rgb[2]);
    strokeColor = RGB(rgb[0], rgb[1], rgb[2]);
    } else {
    strokeColor = RGB(r, g, b);
    }
    isStrokeEnabled = 1;
}
//Function to disable stroke
void noStroke() {
    isStrokeEnabled = 0;
}
int strokeWeightCurrent = 1; // Variable to store the stroke width
// Function to set the stroke width
void strokeWeight(int weight) {
    if (weight > 0) {
        strokeWeightCurrent = weight;
    }
}
//Function to set the fill color
void fill(u16 r, u16 g, u16 b) {
  if (colorModeCurrent == HUEMODE) {
    u16 rgb[3];
    hueToRgb(r, &rgb[0], &rgb[1], &rgb[2]);
    fillColor = RGB(rgb[0], rgb[1], rgb[2]);
  } else {
    fillColor = RGB(r, g, b);
  }
  isFillEnabled = 1;
}
// Function to disable fill color
void noFill() {
    isFillEnabled = 0;
}

//---Sound---
#define REG_SOUNDCNT_X  *(volatile u16*)0x4000084 //Extended sound control
#define REG_SOUNDCNT_L  *(volatile u16*)0x4000080 //DMG sound control
#define REG_SOUNDCNT_H  *(volatile u16*)0x4000082 //Direct sound control
#define REG_SOUND3CNT_L *(volatile u16*)0x4000070
#define SOUND3BANK32    0x0000 //Use two banks of 32 steps each
#define SOUND3SETBANK1  0x0040
#define REG_WAVE_RAM0   *(volatile u32*)0x4000090 
#define REG_WAVE_RAM1   *(volatile u32*)0x4000094 
#define REG_WAVE_RAM2   *(volatile u32*)0x4000098 
#define REG_WAVE_RAM3   *(volatile u32*)0x400009C 
#define SOUND3SETBANK0  0x0000 //Bank to play 0 or 1 (non set bank is written to)
#define SOUND3PLAY	    0x0080 //Output sound
#define REG_SOUND3CNT_H *(volatile u16*)0x4000072 
#define SOUND3OUTPUT1   0x2000 //Output unmodified
#define REG_SOUND3CNT_X *(volatile u16*)0x4000074 
#define SOUND3INIT	    0x8000 //Makes the sound restart
#define SOUND3PLAYONCE  0x4000 //Play sound once
void PlayNote( u16 frequency, unsigned char length ){ //Play sound function
	unsigned char adjustedLength = 0xFF - length;
	REG_SOUNDCNT_X = 0x80;
	REG_SOUNDCNT_L=0xFF77;
	REG_SOUNDCNT_H = 2;
	REG_SOUND3CNT_L = SOUND3BANK32 | SOUND3SETBANK1;
	REG_WAVE_RAM0=0x10325476;
	REG_WAVE_RAM1=0x98badcfe;
	REG_WAVE_RAM2=0x10325476;
	REG_WAVE_RAM3=0x98badcfe;
	REG_SOUND3CNT_L = SOUND3BANK32 | SOUND3SETBANK0;
	REG_SOUND3CNT_L |= SOUND3PLAY;
	REG_SOUND3CNT_H = SOUND3OUTPUT1 | adjustedLength;
	REG_SOUND3CNT_X=SOUND3INIT|SOUND3PLAYONCE| frequency ;
}

//---Math vars and functions---
//Pi Definitions
#define PI 3.14159265 //Pi value = 3.14159265
#define HALF_PI 1.5707964 //Value of half Pi = 1.5707964
#define QUARTER_PI 0.7853982 //Value of Pi quarter = 0.7853982
#define TWO_PI 6.2831855 //Value of Pi twice = 6.2831855
#define TAU 6.2831855 //Value of Pi twice = 6.2831855

//Implementation of sine and cosine functions using Taylor series
float mySin(float x) {
    //Normalizes the angle to the interval [-π, π]
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    float result = 0.0;
    float term = x; //First term of the series
    int n = 1;
    //Approximation with 5 terms
    while (n < 6) {
        result += term;
        term *= -x * x / ((2 * n) * (2 * n + 1)); //Calculate the next term
        n++;
    }
    return result;
}
float myCos(float x) {
    //Normalizes the angle to the interval [-π, π]
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    float result = 0.0;
    float term = 1.0; //First term of the series
    int n = 1;
    //Approximation with 5 terms
    while (n < 6) {
        result += term;
        term *= -x * x / ((2 * n - 1) * (2 * n)); //Calculate the next term
        n++;
    }
    return result;
}
//Sine and cosine table
#define SIN_TABLE_SIZE 360
float sinTable[SIN_TABLE_SIZE];
float cosTable[SIN_TABLE_SIZE];
//Initialize sine and cosine table
void initTrigTables() {
    for (int i = 0; i < SIN_TABLE_SIZE; i++) {
        sinTable[i] = mySin(i * (PI / 180));
        cosTable[i] = myCos(i * (PI / 180));
    }
}
//Function to calculate factorial
int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}
//Function to calculate power
float power(float base, int exp) {
    float result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}
//Function to restrict a value to a range
float constrain(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
//Function to normalize a number from one range to another
float norm(float value, float start, float stop) {
    return constrain((value - start) / (stop - start), 0, 1);
}
//Mapping function
float map(float value, float start1, float stop1, float start2, float stop2) {
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}
//Absolute value function
float myabs(float n) {
    return (n < 0) ? -n : n;
}
//Square value function
float mySqrt(float x) {
    float y = x;
    float z = (y + 1) / 2;
    while (z < y) {
        y = z;
        z = (y + x / y) / 2;
    }
    return z;
}
//Dist function
float dist(float x1, float y1, float x2, float y2) {
    return mySqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
//Function to calculate minimum value
int min(int a, int b) {
  return (a < b) ? a : b;
}
//Function to calculate maximum value
int max(int a, int b) {
  return (a > b) ? a : b;
}
//Function to convert float to int
int myRound(float number) {
    return (int)(number);
}

////Random functions
//Global variable for the random number generator
static unsigned long seed = 1; //Initial seed
//Function to set the generator seed
void setSeed(unsigned long newSeed) {
    seed = newSeed;
}
//Function to generate a random integer number in a range [value0, value1]
int randombtw(int value0, int value1) { 
    seed = (seed * 1103515245 + 12345) % (1 << 31); //Generation of new value
    return value0 + (seed % (value1 - value0 + 1)); //Includes value1
}
// Function to generate a random float number in a range [value0, value1]
float randomfloat(float min, float max) {
    return min + (randombtw(0, 1000) / 1000.0) * (max - min);
}

////Transformations and rotations
//Structure for storing the transformation matrix
typedef struct {
  int x, y;
} Matrix;
//Stack to store transformation matrices
Matrix matrixStack[32];
int matrixStackSize = 0;
//Variables to store the current position
int translationX = 0;
int translationY = 0;
//Function to save the current matrix on the stack
void pushMatrix() {
  if (matrixStackSize < 32) {
    matrixStack[matrixStackSize].x = translationX;
    matrixStack[matrixStackSize].y = translationY;
    matrixStackSize++;
  }
}
//Function to restore the previous array from the stack
void popMatrix() {
  if (matrixStackSize > 0) {
    matrixStackSize--;
    translationX = matrixStack[matrixStackSize].x;
    translationY = matrixStack[matrixStackSize].y;
  }
}
//Function to set the current position of an object
void translate(int x, int y) {
  translationX += x;
  translationY += y;
}
int currentAngle = 0; //Angle used for rotating 2D objects
//Function to rotate 2D objects
void rotate(int angle) {
  currentAngle = angle;
}
//Defining the center of drawn objects
int formCenterX = 0; //variable to define the x center of the object
int formCenterY = 0; //variable to define the y center of the object

//---Functions for images---
//PImage structure
typedef struct{                                          
 int x; //x position
 int y; //y position
 int rot; //rotate 0-360
 int frame; //frames to animate
}PImage;

//Draw imported image on the screen
IN_IWRAM void image(int w,int h, int xo,int yo, const u16* map, int to){ //image w/h, position offset, texture name, texture offset
    int x,y,c;
    for(x=0;x<w;x++){
        for(y=0;y<h;y++){
            c=map[(y+to*h)*w+x];
            if(c>0){
                VRAM[(y+yo)*SW+x+xo] = c;
            }
        }
    }
}

//---Create variables for player sprites---
#define MAX_FRAMES 3 //Determines the maximum number of frames for character animation per direction.
#define SPRITE_WIDTH 15 //Determines the width of the character sprites
#define SPRITE_HEIGHT 16 //Determines the height of the character sprites

//---Player position: Player.x Player.y
typedef struct{
 int x,y;//position
}Player; Player P;
//---Player direction structure---
typedef enum {
    UPCHAR,
    DOWNCHAR,
    LEFTCHAR,
    RIGHTCHAR,
    STOPPEDCHAR
} Direction;

//Determines character sprites with Image
PImage charSprites;

Direction currentDirection = STOPPEDCHAR;
//Function to draw the character on the screen
IN_IWRAM void imageChar(int w, int h, int xo, int yo, const u16* map, int to) {
    int x, y, c;
    //Here the line and column of the sprite drawing are calculated
    int frameRow = (to / MAX_FRAMES) * 20; //Each set of 4 frames occupies 20 lines
    int frameCol = (to % MAX_FRAMES) * 20 + 60; //Each frame occupies 20 columns, starting from column 60
    for (x = 0; x < w-7; x++) {
        for (y = 0; y < h-4; y++) {
            c = map[(y + frameRow-1) * (w + 40) + (x + frameCol)];
            if (c > 0) {
                VRAM[(y + yo) * SW + x + xo] = c;
            }
        }
    }
}

int animationFrameCounter = 0; //Animation counter
int animationSpeed = 1; //Animation speed

//Function to change the character's animation on the screen
void updateAnimation(PImage* sprite, Direction dir, int isMoving) {
    if (isMoving) {
        //Increase the counter with each call
        animationFrameCounter++;
        //If the counter exceeds the speed, update the frame
        if (animationFrameCounter >= animationSpeed) {
            animationFrameCounter = 1; //Reset the counter
            //Updates frames according to direction
            switch (dir) {
                case DOWNCHAR:
                    sprite->frame = (sprite->frame + 1) % MAX_FRAMES + 0; // 0, 1, 2
                break;
                case LEFTCHAR:
                    sprite->frame = (sprite->frame + 1) % MAX_FRAMES + 3; // 3, 4, 5
                break;
                case RIGHTCHAR:
                    sprite->frame = (sprite->frame + 1) % MAX_FRAMES + 6; // 6, 7, 8
                break;
                case UPCHAR:
                    sprite->frame = (sprite->frame + 1) % MAX_FRAMES + 9; // 9, 10, 11
                break;
                default:
                break;
            }
        }
    } else {
        //If not moving, set the stop frame
        switch (dir) {
            case DOWNCHAR:
                sprite->frame = 1; //Stop on frame 1
                break;
            case LEFTCHAR:
                sprite->frame = 4; //Stop on frame 4
                break;
            case RIGHTCHAR:
                sprite->frame = 7; //Stop on frame 7
                break;
            case UPCHAR:
                sprite->frame = 10; //Stop on frame 10
                break;
            default:
                sprite->frame = 0; //Stop on frame 0 - default frame
                break;
        }
    }
}

//Function to paint the background with some color
void background(int r, int g, int b) { //rgb values 0-31
    int x,y;
    for(x=0;x<width;x++){                                                         
        for(y=0;y<SH;y++){ VRAM[y*SW+x]=RGB(r,g,b);} //clear all pixels
    }
}

//Function to draw a pixel on the screen
void point(int x, int y) {
    //Apply the transformation
    x += translationX;
    y += translationY;
    if (x >= 0 && x < width && y >= 0 && y < SH) {
        if (isStrokeEnabled) {
            int offsetX = (strokeWeightCurrent - 1) / 2;
            for (int i = 0; i < strokeWeightCurrent; i++) {
                for (int j = 0; j < strokeWeightCurrent; j++) {
                    int newX = x + i - offsetX;
                    int newY = y + j;
                    if (newX >= 0 && newX < width && newY >= 0 && newY < SH) {
                        VRAM[newY * SW + newX] = strokeColor;
                    }
                }
            }
        }
    }
}

//Function to draw a pixel on the screen to create some object stroke
void pointDraw(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < SH) {
        if (isStrokeEnabled) {
            int offsetX = (strokeWeightCurrent - 1) / 2;
            for (int i = 0; i < strokeWeightCurrent; i++) {
                for (int j = 0; j < strokeWeightCurrent; j++) {
                    int newX = x + i - offsetX;
                    int newY = y + j;
                    if (newX >= 0 && newX < width && newY >= 0 && newY < SH) {
                        VRAM[newY * SW + newX] = strokeColor;
                    }
                }
            }
        }
    }
}

//Function to draw a pixel to fill a shape
void pointFill(int x, int y) {
    //Apply the transformation
    x += translationX;
    y += translationY;
    if (x >= 0 && x < width && y >= 0 && y < SH) {
        if (isFillEnabled) {
            VRAM[y * SW + x] = fillColor;
        }
    }
}


// Function to draw lines
void line(int x0, int y0, int x1, int y1) {
    //Apply the transformation
    x0 += translationX;
    y0 += translationY;
    x1 += translationX;
    y1 += translationY;
    //Calculate the center of the line
    int centerX = (x0 + x1) / 2;
    int centerY = (y0 + y1) / 2;
    //If rotation is not 0, apply rotation
    if (currentAngle != 0) {
        //Convert the angle from degrees to radians
        int angleIndex = currentAngle % 360;
        if (angleIndex < 0) angleIndex += 360;
        //Use the sines and cosines table for rotation
        float cosAngle = cosTable[angleIndex];
        float sinAngle = sinTable[angleIndex];
        //Rotates the point (x0, y0) around the center
        int newX0 = (int)(centerX + (x0 - centerX) * cosAngle - (y0 - centerY) * sinAngle);
        int newY0 = (int)(centerY + (x0 - centerX) * sinAngle + (y0 - centerY) * cosAngle);
        //Rotates the point (x1, y1) around the center
        int newX1 = (int)(centerX + (x1 - centerX) * cosAngle - (y1 - centerY) * sinAngle);
        int newY1 = (int)(centerY + (x1 - centerX) * sinAngle + (y1 - centerY) * cosAngle);
        //Draw the line with the rotated points
        int dx = myabs(newX1 - newX0);
        int dy = myabs(newY1 - newY0);
        int sx = (newX0 < newX1) ? 1 : -1;
        int sy = (newY0 < newY1) ? 1 : -1;
        int err = dx - dy;
        while (1) {
            pointDraw(newX0, newY0); //Draw the pixel at the rotated point

            if (newX0 == newX1 && newY0 == newY1) break;

            int err2 = err * 2;
            if (err2 > -dy) {
                err -= dy;
                newX0 += sx;
            }
            if (err2 < dx) {
                err += dx;
                newY0 += sy;
            }
        }
    } else {
        //If the angle is 0, draw the line normally
        int dx = myabs(x1 - x0);
        int dy = myabs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (1) {
            pointDraw(x0, y0);
            if (x0 == x1 && y0 == y1) break;
            int err2 = err * 2;
            if (err2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (err2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }
}

//Function for object outlines
void lineDraw(int x0, int y0, int x1, int y1) {

    //Apply the transformation
    x0 += translationX;
    y0 += translationY;
    x1 += translationX;
    y1 += translationY;

	int dx = (x1 - x0) >= 0 ? (x1 - x0) : (x0 - x1);
	int dy = (y1 - y0) >= 0 ? (y1 - y0) : (y0 - y1);
    int sx = (x0 < x1) ? 1 : -1; //Direction of increment in x
    int sy = (y0 < y1) ? 1 : -1; //Direction of the y increment
    int err = dx - dy;           //Initial error

    while (1) {
        pointDraw(x0, y0); //Draw the pixel
        //Check if we reached the end point
        if (x0 == x1 && y0 == y1) break;
        //Calculate the error and adjust the coordinates
        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

//Number of iterations and vertices for geometric shapes
#define MAX_VERTICES 100 //Defines maximum number of vertices for shapes
//Vertex structure
typedef struct {
    int x;
    int y;
} Vertex;
//Creates a vector with the location of the vertices for polygon draw
Vertex vertices[MAX_VERTICES];
int vertexCount = 0;

//Function that starts the definition of a new form
void beginShape() {
    vertexCount = 0; //Reset the vertex counter
}

//Function to add a vertex to the shape
void vertex(int x, int y) {
    if (vertexCount < MAX_VERTICES) {
        if (currentAngle != 0) {
            int angleDeg = currentAngle % 360;
            if (angleDeg < 0) angleDeg += 360; //Ensures that the index is between 0 and 359
            vertices[vertexCount].x = (int)(formCenterX + (x - formCenterX) * cosTable[angleDeg] - (y - formCenterY) * sinTable[angleDeg]);
            vertices[vertexCount].y = (int)(formCenterY + (x - formCenterX) * sinTable[angleDeg] + (y - formCenterY) * cosTable[angleDeg]);
        } else {
            vertices[vertexCount].x = x;
            vertices[vertexCount].y = y;
        }
        vertexCount++;
    }
}

//Function to close the drawing of a shape
void endShape() {
    //Find the rectangle that encloses the polygon
    int minX = width, minY = SH, maxX = 0, maxY = 0;
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].y > maxY) maxY = vertices[i].y;

    }
    //For each pixel within the rectangle
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            int windingNumber = 0;
            //For each segment of the polygon
            for (int i = 0; i < vertexCount; i++) {
                int j = (i + 1) % vertexCount; //Connect to the first vertex
                //Check if the segment crosses the horizontal line of the pixel
                if ((vertices[i].y <= y && vertices[j].y > y) || (vertices[j].y <= y && vertices[i].y > y)) {
                    //Calculate the intersection
                    int intersectionX = (vertices[i].x * (vertices[j].y - y) + vertices[j].x * (y - vertices[i].y)) / (vertices[j].y - vertices[i].y);
                    //Check if the intersection is to the left of the pixel
                    if (intersectionX < x) {
                        windingNumber++;
                    }
                }
            }
            //If the number of turns is odd, the pixel is inside the polygon
            if (windingNumber % 2 != 0) {
                pointFill(x, y); //Draws the pixel with the fill color
            }
        }
    }
    //Draw the outline
    for (int i = 0; i < vertexCount - 1; i++) {
        lineDraw(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y);
    }
    //If closed is true, draw the line back to the first vertex
    if (vertexCount > 2) {
        lineDraw(vertices[vertexCount - 1].x, vertices[vertexCount - 1].y, vertices[0].x, vertices[0].y);
    }
}

//Function to draw rectangles
void rect(int x, int y, int w, int h) {
    formCenterX = x + (w / 2);
    formCenterY = y + (h / 2);
    if (currentAngle != 0) {
        beginShape();
        vertex(x, y);
        vertex(x + w, y);
        vertex(x + w, y + h);
        vertex(x, y + h);
        endShape();
    } else {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (x + j >= 0 && x + j < width && y + i >= 0 && y + i < SH) {
                    if (isFillEnabled) {
                        pointFill(x+j, y+i);
                    }
                }
            }
        }
        //Draw the top border
        lineDraw(x, y, x + w - 1, y);
        //Draw the bottom border
        lineDraw(x, y + h - 1, x + w - 1, y + h - 1);
        //Draw the left border
        lineDraw(x, y, x, y + h - 1);
        //Draw the right border
        lineDraw(x + w - 1, y, x + w - 1, y + h - 1);
    }
}

//Function to draw squares
void square(int x, int y, int w) {
    formCenterX = x + (w / 2);
    formCenterY = y + (w / 2);
    rect(x,y,w,w);
}

//Function to draw quadrilaterals with angles other than 90, 90, 90, 90 degrees
void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    formCenterX = (x1 + x2 + x3 + x4) / 4;
    formCenterY = (y1 + y2 + y3 + y4) / 4;
    beginShape();
    vertex(x1, y1);
    vertex(x2, y2);
    vertex(x3, y3);
    vertex(x4, y4);
    endShape();
}

//Function to draw triangles
void triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    formCenterX = (x1 + x2 + x3) / 3;
    formCenterY = (y1 + y2 + y3) / 3;
    beginShape();
    vertex(x1, y1);
    vertex(x2, y2);
    vertex(x3, y3);
    endShape();
}

//Function to draw polygons
void polygon(float x, float y, float radius, int npoints) {
    formCenterX = x+radius;
    formCenterY = y+radius;
    float angle = TWO_PI / npoints;
    beginShape();
    for (float a = 0; a < TWO_PI; a += angle) {
        float sx = x + myCos(a) * radius;
        float sy = y + mySin(a) * radius;
        vertex(radius+sx, radius+sy);
    }
    endShape();
}

//Function to draw ellipses
void ellipse(int x0, int y0, int rx, int ry) {
    if (rx <= 0 || ry <= 0) return;
    //Drawing without rotation
    if (currentAngle == 0) {
        int x, y;
        int rxa = rx / 2;
        int rya = ry / 2;
        int xa = x0 + rxa;
        int ya = y0 + rya;
        int rx2 = rxa * rxa;
        int ry2 = rya * rya;
        //Draw fill
        if (isFillEnabled) {
            for (y = -rya; y <= rya; y++) {
                for (x = -rxa; x <= rxa; x++) {
                    if ((x * x * ry2 + y * y * rx2) <= (rx2 * ry2)) {
                        pointFill(xa + x, ya + y);
                    }
                }
            }
        }
        if (!isStrokeEnabled || rya == 0) return;
        //Initialize the points
        x = 0;
        y = rya;
        //First set (half of the ellipse)
        long d1 = ry2 - (rx2 * rya) + (rx2 / 4);
        long dx = 2 * ry2 * x;
        long dy = 2 * rx2 * y;
        while (dx < dy) {
            //Four quadrants
            pointDraw(xa + x, ya + y);
            pointDraw(xa - x, ya + y);
            pointDraw(xa + x, ya - y);
            pointDraw(xa - x, ya - y);
            if (d1 < 0) {
                x++;
                dx += 2 * ry2;
                d1 += dx + ry2;
            } else {
                x++;
                y--;
                dx += 2 * ry2;
                dy -= 2 * rx2;
                d1 += dx - dy + ry2;
            }
        }
        //Second set
        long d2 = (long)ry2 * (x + 0.5) * (x + 0.5) + (long)rx2 * (y - 1) * (y - 1) - (long)rx2 * ry2;
        while (y >= 0) {
            pointDraw(xa + x, ya + y);
            pointDraw(xa - x, ya + y);
            pointDraw(xa + x, ya - y);
            pointDraw(xa - x, ya - y);
            if (d2 > 0) {
                y--;
                dy -= 2 * rx2;
                d2 += rx2 - dy;
            } else {
                y--;
                x++;
                dx += 2 * ry2;
                dy -= 2 * rx2;
                d2 += rx2 - dy + dx;
            }
        }
        return;
    }
    //Drawing with non-zero angle, rotated
    int rxa = rx / 2;
    int rya = ry / 2;
    int cx = x0 + rxa;
    int cy = y0 + rya;

    int angleIndex = currentAngle % 360;
    if (angleIndex < 0) angleIndex += 360;
    int ca = (int)(cosTable[angleIndex] * 1024);
    int sa = (int)(sinTable[angleIndex] * 1024);
    int ac = ca; if (ac < 0) ac = -ac;
    int as = sa; if (as < 0) as = -as;
    int maxXoffset = ((rxa * ac) + (rya * as)) / 1024 + 1;
    int maxYoffset = ((rxa * as) + (rya * ac)) / 1024 + 1;
    int minX = cx - maxXoffset;
    int maxX = cx + maxXoffset;
    int minY = cy - maxYoffset;
    int maxY = cy + maxYoffset;
    if (isFillEnabled) {
        long invRx2 = (long)rxa * rxa;
        long invRy2 = (long)rya * rya;
        //Filler recoil factor in relation to stroke (63 seemed okay)
        long limit = invRx2 * invRy2 * 63 / 100;

        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                int dx = x - cx;
                int dy = y - cy;
                long xr = (dx * ca + dy * sa) / 1024;
                long yr = (-dx * sa + dy * ca) / 1024;
                long eq = (xr * xr * invRy2 + yr * yr * invRx2);
                if (eq <= limit) {
                    pointFill(x, y);
                }
            }
        }
    }

    if (isStrokeEnabled) {
        for (int a = 0; a < 360; a++) {
            int ex = (rxa * (int)(cosTable[a] * 1024)) / 1024;
            int ey = (rya * (int)(sinTable[a] * 1024)) / 1024;
            int xr = (ex * ca - ey * sa) / 1024;
            int yr = (ex * sa + ey * ca) / 1024;
            for (int sx = -strokeWeightCurrent / 2; sx <= strokeWeightCurrent / 2; sx++) {
                for (int sy = -strokeWeightCurrent / 2; sy <= strokeWeightCurrent / 2; sy++) {
                    pointDraw(cx + xr + sx, cy + yr + sy);
                }
            }
        }
    }
}

//Function to draw circles
void circle(int x0, int y0, int rx) {
    ellipse(x0, y0, rx, rx);
}

//Function to draw arcs
void arc(int x, int y, int w, int h, float start, float stop) {
    int startAngle = (int)(start * (180 / PI));
    int stopAngle = (int)(stop * (180 / PI));
    //Ensures that the stopping angle is greater than the starting angle
    if (stopAngle < startAngle) {
        stopAngle += 360;
    }
    int halfWidth = w/2;
    int halfHeight = h/2;
    //Fill the arc area
    for (int i = 0; i < height; i++) {
        int yLine = y + i - halfHeight;//current line of y
        int xMin = x, xMax = x;
        for (int angle = startAngle; angle <= stopAngle; angle++) {
            int px = x + halfWidth * cosTable[angle];
            int py = y + halfHeight * sinTable[angle];
            //Check if the line crosses the current line (yLine)
            if (py == yLine) {
                if (px < xMin) xMin = px;
                if (px > xMax) xMax = px;
            }
        }
        //Fills the line between xMin and xMax
        if (xMax > xMin) {
            for (int fillX = xMin; fillX <= xMax; fillX++) {
                pointFill(fillX, yLine); //Draw the point with the fill color
            }
        }
    }
    //Draw the edge of the arc
    for (int angle = startAngle; angle <= stopAngle; angle++) {
        int px = x + halfWidth * cosTable[angle];
        int py = y + halfHeight * sinTable[angle];
        point(px, py); //Draw the border point
    }
}

//Function to draw a Bézier curve with fill
void bezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  //Find the rectangle that encloses the polygon
  int minX = min(x1, min(x2, min(x3, x4)));
  int maxX = max(x1, max(x2, max(x3, x4)));
  int minY = min(y1, min(y2, min(y3, y4)));
  int maxY = max(y1, max(y2, max(y3, y4)));
  //Defines the number of vertices
  int vertexCount = 100;
  //Creates an array to store the vertices
  int vertices[vertexCount][2];
  //Calculate the vertices of the Bézier curve
  for (int i = 0; i < vertexCount; i++) {
    float t = (float)i / (float)(vertexCount - 1);
    float u = 1 - t;
    vertices[i][0] = (int)(u * u * u * x1 + 3 * u * u * t * x2 + 3 * u * t * t * x3 + t * t * t * x4);
    vertices[i][1] = (int)(u * u * u * y1 + 3 * u * u * t * y2 + 3 * u * t * t * y3 + t * t * t * y4);
  }
  //Scanline Fill Algorithm
  for (int y = minY; y <= maxY; y++) {
    int xMin = maxX;
    int xMax = minX;
    //Find the intersection points with the horizontal line
    for (int i = 0; i < vertexCount; i++) {
      if (vertices[i][1] == y) {
        xMin = min(xMin, vertices[i][0]);
        xMax = max(xMax, vertices[i][0]);
      } else {
        int iNext = (i + 1) % vertexCount;
        if ((vertices[i][1] <= y && vertices[iNext][1] > y) || (vertices[iNext][1] <= y && vertices[i][1] > y)) {
          int intersectionX = (vertices[i][0] * (vertices[iNext][1] - y) + vertices[iNext][0] * (y - vertices[i][1])) / (vertices[iNext][1] - vertices[i][1]);
          xMin = min(xMin, intersectionX);
          xMax = max(xMax, intersectionX);
        }
      }
    }
    //Fills the line between xMin and xMax
    if (xMax > xMin) {
      for (int fillX = xMin; fillX <= xMax; fillX++) {
        pointFill(fillX, y); //Draw the point with the fill color
      }
    }
  }
  //Draw the edge of the curve
  for (int i = 0; i < vertexCount - 1; i++) {
    lineDraw(vertices[i][0], vertices[i][1], vertices[i + 1][0], vertices[i + 1][1]);
  }
}

//---Functions and variables for 3D drawing---
//Function to project vertices in 2D
void project(float x, float y, float z, int *outX, int *outY) {
    //Simple perspective projection
    float scale = 200.0f / (z + 400.0f); //Distance from camera
    *outX = (int)(x * scale) + (width / 2); //Center in the object width
    *outY = (int)(y * scale) + (SH / 2); //Center in the object height
}
//Structure to the 3D transformation
typedef struct {
    float x, y, z;
    float angleX, angleY, angleZ;
}Transformation;
Transformation currentTransformation = {0, 0, 0, 0, 0, 0}; //Store the current 3D transformation
//Rotate in X
void rotateX(int angleX) {
    currentTransformation.angleX += angleX;
}
//Rotate in Y
void rotateY(int angleY) {
    currentTransformation.angleY += angleY;
}
//Rotate in Z
void rotateZ(int angleZ) {
    currentTransformation.angleZ += angleZ;
}
//Function to draw boxes (cubes or parallelepipeds)
void box(int x, int y, int z, int w, int h, int d) {
    //Convert coordinates to the GBA coordinate system
    x = x - 160;
    y = y - 80;
    float vertices[8][3] = {
        {0, 0, 0}, {w, 0, 0}, {w, h, 0}, {0, h, 0},
        {0, 0, d}, {w, 0, d}, {w, h, d}, {0, h, d}
    };
    //Apply current transformation
    float radX = currentTransformation.angleX * (PI / 180);
    float radY = currentTransformation.angleY * (PI / 180);
    float radZ = currentTransformation.angleZ * (PI / 180);

    for (int i = 0; i < 8; i++) {
        float tempX = vertices[i][0];
        float tempY = vertices[i][1];
        float tempZ = vertices[i][2];
        //Rotation around the X axis
        float y1 = tempY * cosTable[(int)(radX * (180 / PI)) % SIN_TABLE_SIZE] - tempZ * sinTable[(int)(radX * (180 / PI)) % SIN_TABLE_SIZE];
        float z1 = tempY * sinTable[(int)(radX * (180 / PI)) % SIN_TABLE_SIZE] + tempZ * cosTable[(int)(radX * (180 / PI)) % SIN_TABLE_SIZE];
        tempY = y1;
        tempZ = z1;
        //Rotation around the Y axis
        float x1 = tempX * cosTable[(int)(radY * (180 / PI)) % SIN_TABLE_SIZE] + tempZ * sinTable[(int)(radY * (180 / PI)) % SIN_TABLE_SIZE];
        z1 = -tempX * sinTable[(int)(radY * (180 / PI)) % SIN_TABLE_SIZE] + tempZ * cosTable[(int)(radY * (180 / PI)) % SIN_TABLE_SIZE];
        tempX = x1;
        tempZ = z1;
        //Rotation around the Z axis
        vertices[i][0] = tempX * cosTable[(int)(radZ * (180 / PI)) % SIN_TABLE_SIZE] - tempY * sinTable[(int)(radZ * (180 / PI)) % SIN_TABLE_SIZE];
        vertices[i][1] = tempX * sinTable[(int)(radZ * (180 / PI)) % SIN_TABLE_SIZE] + tempY * cosTable[(int)(radZ * (180 / PI)) % SIN_TABLE_SIZE];
        vertices[i][2] = tempZ; //The Z coordinate remains unchanged
    }
    //Translate the vertices to the desired position
    for (int i = 0; i < 8; i++) {
        vertices[i][0] += x + currentTransformation.x;
        vertices[i][1] += y + currentTransformation.y;
        vertices[i][2] += z + currentTransformation.z;
    }
    int projected[8][2];
    for (int i = 0; i < 8; i++) {
        project(vertices[i][0], vertices[i][1], vertices[i][2], &projected[i][0], &projected[i][1]);
    }
    //Draw the faces and edges
    //Front face
    beginShape();
    vertex(projected[0][0], projected[0][1]);
    vertex(projected[1][0], projected[1][1]);
    vertex(projected[2][0], projected[2][1]);
    vertex(projected[3][0], projected[3][1]);
    endShape();
    //Rear face
    beginShape();
    vertex(projected[4][0], projected[4][1]);
    vertex(projected[5][0], projected[5][1]);
    vertex(projected[6][0], projected[6][1]);
    vertex(projected[7][0], projected[7][1]);
    endShape();
    //Left face
    beginShape();
    vertex(projected[0][0], projected[0][1]);
    vertex(projected[3][0], projected[3][1]);
    vertex(projected[7][0], projected[7][1]);
    vertex(projected[4][0], projected[4][1]);
    endShape();
    //Right face
    beginShape();
    vertex(projected[1][0], projected[1][1]);
    vertex(projected[2][0], projected[2][1]);
    vertex(projected[6][0], projected[6][1]);
    vertex(projected[5][0], projected[5][1]);
    endShape();
    //Upper face
    beginShape();
    vertex(projected[3][0], projected[3][1]);
    vertex(projected[2][0], projected[2][1]);
    vertex(projected[6][0], projected[6][1]);
    vertex(projected[7][0], projected[7][1]);
    endShape();
    //Lower face
    beginShape();
    vertex(projected[0][0], projected[0][1]);
    vertex(projected[1][0], projected[1][1]);
    vertex(projected[5][0], projected[5][1]);
    vertex(projected[4][0], projected[4][1]);
    endShape();
}

//---Defining text functions---
//Global variable to store the current font
const u16* currentFont;
//Variable to store the font width
int charWidth;
//Variable to store the font height
int charHeight;

//Function to convert int to string and concatenate
char* intToString(const char* textw, int value) {
  static char buffer[50];
  sprintf(buffer, "%s%d", textw, value);
  return buffer;
}

//Function to convert float to string and concatenate
char* floatToString(const char* textw, float value) {
  static char buffer[50];
  sprintf(buffer, "%s%.2f", textw, value);
  return buffer;
}

//Function to convert unsigned char to string and concatenate
char* unsignedCharToString(const char* textw, unsigned char value) {
  static char buffer[50];
  sprintf(buffer, "%s%d", textw, value);
  return buffer;
}

//Function to compare strings
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

//Function to change font
void textFont(const char* fontName) {
    if (strcmp(fontName, "variable_8x8_font") == 0) {
        currentFont = (const u16*) variable_8x8_font;
        charWidth = 8;
        charHeight = 8;
    } else if (strcmp(fontName, "fixed_8x16_font") == 0) {
        currentFont = (const u16*) fixed_8x16_font;
        charWidth = 8;
        charHeight = 16;
    } else if (strcmp(fontName, "fixed_8x8_font") == 0) {
        currentFont = (const u16*) fixed_8x8_font;
        charWidth = 8;
        charHeight = 8;
    } else if (strcmp(fontName, "variable_8x16_font") == 0) {
        currentFont = (const u16*) variable_8x16_font;
        charWidth = 8;
        charHeight = 16;
    } else if (strcmp(fontName, "variable_16x16_font") == 0) {
        currentFont = (const u16*) variable_16x16_font;
        charWidth = 16;
        charHeight = 16;
    } else if (strcmp(fontName, "fixed_32x64_font") == 0) {
        currentFont = (const u16*) fixed_32x64_font;
        charWidth = 32;
        charHeight = 64;
    }
}

////Function to draw characters
void charDraw(int xo, int yo, int to) {
    int x, y, c;          
    int w = charWidth;
    int h = charHeight;

    for (x = 0; x < w; x++) {
        for (y = 0; y < h; y++) {
            c = currentFont[(y + to * h) * w + x];
            if (c > 0) {
                pointFill(x + xo, y + yo);
            }
        }
    }
}

//Structure for defining alignment types
typedef enum {
    LEFT,
    CENTER,
    RIGHT,
    NORMAL
} Alignment;

//Global variable to store the current alignment
Alignment textAlignMode = NORMAL;

//Function to set text alignment
void textAlign(Alignment mode) {
    textAlignMode = mode;
}

//text function
void text(const char* str, int x, int y) {
    int currentLine = y;
    int currentColumn = x;
    int charsPerLine = 0;
    int textWidth = 0;
    int limitCharsPerLine = 20;
    //Calculates the width of the text
    for (int i = 0; str[i]; i++) {
        int charIndex = str[i];
        if (charIndex == 32) {
            textWidth += charWidth;
        } else if (charIndex >= 33 && charIndex <= 122) {
            textWidth += charWidth;
        } else if (charIndex == 161 || charIndex == 160 || charIndex == 162 || charIndex == 163) {
            textWidth += charWidth;
        }
    }
    //Adjusts the starting position based on the alignment
    switch (textAlignMode) {
        case RIGHT:
            currentColumn -= textWidth;
            break;
        case CENTER:
            currentColumn -= textWidth / 2;
            break;
        case NORMAL:
            currentColumn -= 0;
            break;
        case LEFT:
            break;
    }
    while (*str) {
        int charIndex = *str; 
        if (charIndex == 32) { 
            currentColumn += charWidth; 
            charsPerLine++;
        } else if (charIndex >= 33 && charIndex <= 122) { 
            int to = charIndex - 33; 
            charDraw(currentColumn, currentLine, to);
            currentColumn += charWidth; 
            charsPerLine++;
            //Check if the character limit per line has been exceeded
            if (charsPerLine >= limitCharsPerLine) {
                currentLine += charHeight;
                currentColumn = x;
                charsPerLine = 0;
            }
        } else if (charIndex == 161) { // á
            charDraw(currentColumn, currentLine, 200); 
            currentColumn += charWidth; 
            charsPerLine++;
            //Check if the character limit per line has been exceeded
            if (charsPerLine >= limitCharsPerLine) {
                currentLine += charHeight;
                currentColumn = x;
                charsPerLine = 0;
            }
        } else if (charIndex == 160) { // à
            charDraw(currentColumn, currentLine, 201); 
            currentColumn += charWidth; 
            charsPerLine++;
            //Check if the character limit per line has been exceeded
            if (charsPerLine >= limitCharsPerLine) {
                currentLine += charHeight;
                currentColumn = x;
                charsPerLine = 0;
            }
        } else if (charIndex == 162) { // â
            charDraw(currentColumn, currentLine, 202); 
            currentColumn += charWidth; 
            charsPerLine++;
            //Check if the character limit per line has been exceeded
            if (charsPerLine >= limitCharsPerLine) {
                currentLine += charHeight;
                currentColumn = x;
                charsPerLine = 0;
            }
        } else if (charIndex == 163) { // ã
            charDraw(currentColumn, currentLine, 203); 
            currentColumn += charWidth; 
            charsPerLine++;
            //Check if the character limit per line has been exceeded
            if (charsPerLine >= limitCharsPerLine) {
                currentLine += charHeight;
                currentColumn = x;
                charsPerLine = 0;
            }
        }
        str++;
    }
}

//---Mouse Emulation---
int mouseX = 0;
int mouseY = 0;
void mousePosition(){ //function to press events
    if(keyHeld(KEY_R)){ //right button press
        mouseX +=1;
        if (mouseX > width) mouseX = width;
    }
    if(keyHeld(KEY_L)){ //left button press
        mouseX -=1;
        if (mouseX < 0) mouseX = 0;
    }
    if(keyHeld(KEY_U)){ //up button press
        mouseY -=1;
        if (mouseY < 0) mouseY = 0;
    }
    if(keyHeld(KEY_D)){ //down button press
        mouseY +=1;
        if (mouseY > height) mouseY = height;
    }
}

//------------
//Below are alternative implementations of some functions. For reference purposes to new versions only.
//------------
//Another implementation of the arc function. This one is closer to what it would be in Processing. However, I haven't implemented the logic correctly yet. I'll revisit this in a future version.

#define PIE 0
#define CHORD 1
#define OPEN 2

void arc_a(int x, int y, int w, int h, float start, float stop, int mode) {
    int startAngle = (int)(start * (180 / PI));
    int stopAngle = (int)(stop * (180 / PI));
    if (stopAngle < startAngle) {
        stopAngle += 360;
    }
    int halfW = w / 2;
    int halfH = h / 2;
    //Fill arc area
    if (mode == PIE) {
        for (int i = 0; i < h; i++) {
            int yLine = y + i - halfH;
            int xMin = x, xMax = x;
            for (int angle = startAngle; angle <= stopAngle; angle++) {
                int px = x + halfW * cosTable[angle];
                int py = y + halfH * sinTable[angle];
                if (py == yLine) {
                    if (px < xMin) xMin = px;
                    if (px > xMax) xMax = px;
                }
            }
            if (xMax > xMin) {
                for (int fillX = xMin; fillX <= xMax; fillX++) {
                    pointFill(fillX, yLine);
                }
            }
        }
    }
    //Draw arc
    for (int angle = startAngle; angle <= stopAngle; angle++) {
        int px = x + halfW * cosTable[angle];
        int py = y + halfH * sinTable[angle];
        point(px, py);
    }
    //PIE Mode
    if (mode == PIE) {
        int startX = x + halfW * cosTable[startAngle];
        int startY = y + halfH * sinTable[startAngle];
        line(x, y, startX, startY);
        int stopX = x + halfW * cosTable[stopAngle];
        int stopY = y + halfH * sinTable[stopAngle];
        line(x, y, stopX, stopY);
    }
    //CHORD Mode
    if (mode == CHORD) {
        int startX = x + halfW * cosTable[startAngle];
        int startY = y + halfH * sinTable[startAngle];
        int stopX = x + halfW * cosTable[stopAngle];
        int stopY = y + halfH * sinTable[stopAngle];
        line(startX, startY, stopX, stopY);
    }
    //OPEN Mode
    if (mode == OPEN) {
        int startX = x + halfW * cosTable[startAngle];
        int startY = y + halfH * sinTable[startAngle];
        int stopX = x + halfW * cosTable[stopAngle];
        int stopY = y + halfH * sinTable[stopAngle];
        line(startX, startY, stopX, stopY);
    }
}

//Function to draw a bezier curve
void bezier_a(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  float step = 0.01; //Step on to calculate the curve. The smaller (e.g. 0.001 or 0.0001) the more accurate, but it is slower...
  for (float t = 0; t <= 1; t += step) {
    float u = 1 - t; //Complement of t
    //Calculation of the x and y coordinates of the curve
    int x = (int)(u * u * u * x1 + 3 * u * u * t * x2 + 3 * u * t * t * x3 + t * t * t * x4);
    int y = (int)(u * u * u * y1 + 3 * u * u * t * y2 + 3 * u * t * t * y3 + t * t * t * y4);
    pointDraw(x, y); //Draw the point on the curve
  }
}