//---#defines---

#include "C:/ProcessingGBA/functions.h"
#include "scene.h"

int fps = 15;

void setFps(int novoFps) {
    fps = novoFps;
    *(u16*)0x400010E = (65536 / fps) - 1;
}

void init()
{
 P.x=70; P.y=35;                                                     //init player
}

void initOnce(){                                                    //Aqui vêm as funções que inicializam somente uma vez
 setup();
 init();                                                            
 initTrigTables();
 textFont("embedded_font");
}

void constantUpdate(){                                              //Aqui vem as funcoes que precisaram de constante atualizacao
   mousePosition();
   buttons();                                                        
   draw();
   setFps(posicaoW);
   incrementFrameCount();
}

int main(){

 //Init mode 5------------------------------------------------------------------
 *(u16*)0x4000000 = 0x405;                                           //mode 5 background 2 
 *(u16*)0x400010A = 0x82;                                            //enable timer for fps
 //*(u16*)0x400010E = 0x84;                                            //cnt timer overflow
 *(u16*)0x400010E = (65536 / fps) - 1;

 //scale small mode 5 screen to full screen-------------------------------------
 REG_BG2PA=171;                                                    //256=normal 128=scale 
 REG_BG2PD=169;                                                    //256=normal 128=scale 

 initOnce();

 while(1){ 
  if((REG_TM2D)>>(12)!=(lastFr)){                                           //draw 15 frames a second
   constantUpdate();
   lastFr = (REG_TM2D) >> 12;
   //swap buffers---------------------------------------------------------------
   while(*Scanline<160){}	                                         //wait all scanlines 
   if  ( DISPCNT&BACKB){
    DISPCNT &= ~BACKB;
    VRAM=(u16*)VRAM_B;}      //back  buffer  - adicione ou diminua valores aqui para dar glitch no backbuffer. ex: VRAM=(u16*)VRAM_B-(posicaoW);
   else{
    DISPCNT |=  BACKB;
    VRAM=(u16*)VRAM_F;}      //front buffer  - adicione ou diminua valores aqui para dar glitch no backbuffer. ex: VRAM=(u16*)VRAM_F-(posicaoW);
  }
 }
}

