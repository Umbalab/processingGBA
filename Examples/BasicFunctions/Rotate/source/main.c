//---#defines---

#include "C:/ProcessingGBA/functions.h"
#include "scene.h"

int fps = 15;

void setFps(int newFps) {
    fps = newFps;
    *(u16*)0x400010E = (65536 / fps) - 1;
}

void initPalyer()
{
 P.x=70; P.y=35; //init player
}

void initOnce(){ //Here come the functions that initialize only once
 textFont("fixed_8x8_font");
 setup();
 initPalyer();                                                            
 initTrigTables();
}

void constantUpdate(){ //Here come the functions that needed constant updating
   updateKeys();
   mousePosition();
   buttons();                                                        
   draw();
   setFps(positionW);
   incrementFrameCount();
}

int main(){

 //Init mode 5------------------------------------------------------------------
 *(u16*)0x4000000 = 0x405; //mode 5 background 2 
 *(u16*)0x400010A = 0x82; //enable timer for fps
 //*(u16*)0x400010E = 0x84; //cnt timer overflow
 *(u16*)0x400010E = (65536 / fps) - 1;

 //scale small mode 5 screen to full screen-------------------------------------
 screenScale();
 initOnce();

 while(1){ 
  if((REG_TM2D)>>(12)!=(lastFr)){ //draw 15 frames a second
   constantUpdate();
   lastFr = (REG_TM2D) >> 12;
   //swap buffers---------------------------------------------------------------
   while(*Scanline<160){} //wait all scanlines 
   if  ( DISPCNT&BACKB){
    DISPCNT &= ~BACKB;
    VRAM=(u16*)VRAM_B;} //back buffer - add or decrease values ​​here to glitch the backbuffer. ex: VRAM=(u16*)VRAM_B-(positionW);
   else{
    DISPCNT |=  BACKB;
    VRAM=(u16*)VRAM_F;} //front buffer - add or decrease values ​​here to glitch the backbuffer. ex: VRAM=(u16*)VRAM_F-(positionW);
  }
 }
}

