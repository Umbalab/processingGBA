//Developed by Mateus Pelanda (2024-2025)
//Adapted from 3d sage turorial: <https://youtu.be/6ecgELrwAnQ?si=OEDT-npNEj4a-tNZ>

//#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int posicaoX = 0;
int posicaoY = 0;
int posicaoZ = 0;
int posicaoW = 0;
//---rename unsigned short and long---
typedef uint8_t u8;
typedef unsigned short u16;
typedef unsigned long  u32; 
//---Video buffer---
u16    *VRAM; 
#define VRAM_F         (u16*)0x6000000 
#define VRAM_B         (u16*)0x600A000
#define DISPCNT        *(u32*)0x4000000
#define BACKB   	   0x10 
volatile u16* Scanline=(volatile u16*)0x4000006;
//---Scale mode 5 screen---
#define REG_BG2PA *(volatile unsigned short *)0x4000020
#define REG_BG2PD *(volatile unsigned short *)0x4000026

//---Global variables---
#define  GBA_SW 160 //actual gba screen width
#define  SW     160 //game screen width
#define  SH     106 //game screen height

//---Timer for FPS---
#define REG_TM2D       *(volatile u16*)0x4000108
//--Fast Iwram and Ewram
#define IN_IWRAM       __attribute__ ((section (".iwram")))
#define IN_EWRAM       __attribute__ ((section (".ewram")))

//---Buttons---
#define KEY_STATE      (*(volatile u16*)0x4000130) 
#define KEY_A          !(KEY_STATE &   1)
#define KEY_B          !(KEY_STATE &   2)
#define KEY_SL         !(KEY_STATE &   4)
#define KEY_ST         !(KEY_STATE &   8)
#define KEY_R          !(KEY_STATE &  16)
#define KEY_L          !(KEY_STATE &  32)
#define KEY_U          !(KEY_STATE &  64)
#define KEY_D          !(KEY_STATE & 128)
#define KEY_RS         !(KEY_STATE & 256)
#define KEY_LS         !(KEY_STATE & 512)

//Color var
#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10)) //15 bit, 0-31, 5bit=r, 5bit=g, 5bit=b 

#define REG_SOUNDCNT_X  *(volatile u16*)0x4000084 //Extended sound control
#define REG_SOUNDCNT_L  *(volatile u16*)0x4000080 //DMG sound control
#define REG_SOUNDCNT_H  *(volatile u16*)0x4000082 //Direct sound control
#define REG_SOUND3CNT_L *(volatile u16*)0x4000070
#define SOUND3BANK32    0x0000	                  //Use two banks of 32 steps each
#define SOUND3SETBANK1  0x0040
#define REG_WAVE_RAM0   *(volatile u32*)0x4000090 
#define REG_WAVE_RAM1   *(volatile u32*)0x4000094 
#define REG_WAVE_RAM2   *(volatile u32*)0x4000098 
#define REG_WAVE_RAM3   *(volatile u32*)0x400009C 
#define SOUND3SETBANK0  0x0000	                  //Bank to play 0 or 1 (non set bank is written to)
#define SOUND3PLAY	    0x0080	                  //Output sound
#define REG_SOUND3CNT_H *(volatile u16*)0x4000072 
#define SOUND3OUTPUT1   0x2000	                  //Output unmodified
#define REG_SOUND3CNT_X *(volatile u16*)0x4000074 
#define SOUND3INIT	    0x8000	                  //Makes the sound restart
#define SOUND3PLAYONCE  0x4000	                  //Play sound once

//Utiliza valor de Pi = 3.14159265
#define PI 3.14159265
//Utiliza valor de metade de Pi = 1.5707964
#define HALF_PI 1.5707964
//Utiliza valor de um quarto de Pi = 0.7853982
#define QUARTER_PI 0.7853982
//Utiliza valor de duas vezes Pi = 6.2831855
#define TWO_PI 6.2831855
//Utiliza valor de duas vezes Pi = 6.2831855
#define TAU 6.2831855
//Número de iterações para a aproximação
#define MAX_ITER 10
//Define maximo de vérices para formas
#define MAX_VERTICES 100

//Criar sprites para player.
//Determina o máximo de quadros para animação da personagem por direção.
#define MAX_FRAMES 3
//Determina a largura dos sprites da personagem
#define SPRITE_WIDTH 15
//Determina a altura dos sprites da personagem
#define SPRITE_HEIGHT 16

//incluir sprites de fontes.
#include "C:/ProcessingGBA/fonts.h"

//incluir sprites de fontes.
#include "C:/ProcessingGBA/images.h"

//largura total do quadro.
const int width = 160;
//altura total do quadro.
const int height = 104;
//ultimo frame.
int lastFr=0;
//Variável para frames por segundo.
int FPS=0;

//Variável para mudar as cores de contornos, linhas.
u16 strokeColor = RGB(0,0,0); // Variável global para armazenar a cor atual.
//Variável para mudar a cor de preenchimento de uma forma.
u16 fillColor = RGB(31,31,31); // Variável global para armazenar a cor atual.
//Variável para verificar se o stroke está habilitado.
int isStrokeEnabled = 1;
//Variável para verificar se o preenchimento está habilitado.
int isFillEnabled = 1;

//Variável global para armazenar a fonte atual.
const u16* currentFont;
//Variável para armazenar a largura da fonte.
int charWidth;
//Variável para armazenar a altura da fonte.
int charHeight;

//Estrutura para boleanos.
typedef enum {
    false,
    true
} boolean;

//Estrutura PImage: PImage.x PImage.y PImage.rot PImage.frame
typedef struct{                                          
 int x;
 int y;                                                  //position
 int rot;                                                //rotate 0-360
 int frame;                                              //frames to animate
}PImage;

//Estrutura para o jogador
typedef struct{                                             //player
 int x,y;                                                  //position
}Player; Player P;

//Estrutura das direções da personagem em cena.
typedef enum {
    UPCHAR,
    DOWNCHAR,
    LEFTCHAR,
    RIGHTCHAR,
    STOPPEDCHAR
} Direction;

//Estrutura da localização de vértices.
typedef struct {
    int x;
    int y;
} Vertex;

//Cria um vetor com da localização dos vértices para poligonos.
Vertex vertices[MAX_VERTICES];
int vertexCount = 0;

//contador de frames 0 até 2000.
int frameCount = 0;

void incrementFrameCount() {
    frameCount++;
    if (frameCount >= 2000) {
        frameCount = 0;
    }
}

// Defina constantes para os modos de cor
#define RGBMODE 0
#define HUEMODE 1

// Variáveis para armazenar o modo de cor atual
u8 colorModeCurrent = RGBMODE;

// Função para definir o modo de cor
void colorMode(u8 mode) {
  if (mode == RGBMODE || mode == HUEMODE) {
    colorModeCurrent = mode;
  }
}

// Função para converter HUE para RGB
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

// Função para definir a cor de contorno
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

// Função para desabilitar o stroke
void noStroke() {
    isStrokeEnabled = 0; // Desabilita o stroke
}

// Função para definir a cor de preenchimento
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

// Função para desabilitar o stroke
void noFill() {
    isFillEnabled = 0; //Desabilita o fill
}

// Variável para armazenar a largura do contorno
int strokeWeightCurrent = 1;

// Função para definir a largura do contorno
void strokeWeight(int weight) {
    if (weight > 0) {
        strokeWeightCurrent = weight;
    }
}

void stretchView(int vw, int vh){
    REG_BG2PA=171+vw; //para deixar pixal perfeito, manter 128
    REG_BG2PD=169+vh; //para deixar pixal perfeito, manter 128
}

IN_IWRAM void image(int w,int h, int xo,int yo, const u16* map, int to){
    int x,y,c;          //image w/h, position offset,  texture name, texture offset
    for(x=0;x<w;x++){
        for(y=0;y<h;y++){
            c=map[(y+to*h)*w+x];
            if(c>0){
                VRAM[(y+yo)*SW+x+xo]=c;
            }
        }
    }
}

// Definições de cores
u16 red = RGB(31, 0, 0);        // Vermelho
u16 green = RGB(0, 31, 0);      // Verde
u16 blue = RGB(0, 0, 31);       // Azul
u16 yellow = RGB(31, 31, 0);    // Amarelo
u16 cyan = RGB(0, 31, 31);      // Ciano
u16 magenta = RGB(31, 0, 31);   // Magenta
u16 white = RGB(31, 31, 31);    // Branco
u16 black = RGB(0, 0, 0);       // Preto

// Tonalidades de cinza
u16 grayLight = RGB(15, 15, 15);    // Cinza claro
u16 grayMedium = RGB(10, 10, 10);   // Cinza médio
u16 grayDark = RGB(5, 5, 5);        // Cinza escuro

// Variantes de cores
u16 orange = RGB(31, 15, 0);        // Laranja
u16 purple = RGB(15, 0, 31);        // Roxo
u16 brown = RGB(15, 10, 5);         // Marrom

// Tons pastel
u16 pastelPink = RGB(31, 20, 20);   // Rosa pastel
u16 pastelGreen = RGB(20, 31, 20);  // Verde pastel
u16 pastelBlue = RGB(20, 20, 31);   // Azul pastel

//funções matemáticas

// Implementação das funções seno e cosseno usando série de Taylor
float mySin(float x) {
    // Normaliza o ângulo para o intervalo [-π, π]
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;

    float result = 0.0;
    float term = x; // Primeiro termo da série
    int n = 1;

    // Aproximação com 5 termos
    while (n < 6) {
        result += term;
        term *= -x * x / ((2 * n) * (2 * n + 1)); // Calcula o próximo termo
        n++;
    }
    
    return result;
}

float myCos(float x) {
    // Normaliza o ângulo para o intervalo [-π, π]
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;

    float result = 0.0;
    float term = 1.0; // Primeiro termo da série
    int n = 1;

    // Aproximação com 5 termos
    while (n < 6) {
        result += term;
        term *= -x * x / ((2 * n - 1) * (2 * n)); // Calcula o próximo termo
        n++;
    }
    
    return result;
}

// Tabela de seno e cosseno
#define SIN_TABLE_SIZE 360
float sinTable[SIN_TABLE_SIZE];
float cosTable[SIN_TABLE_SIZE];

// Inicializar tabela de seno e cosseno
void initTrigTables() {
    for (int i = 0; i < SIN_TABLE_SIZE; i++) {
        sinTable[i] = mySin(i * (PI / 180));
        cosTable[i] = myCos(i * (PI / 180));
    }
}

////função random
// Variável global para o gerador de números aleatórios
static unsigned long seed = 1; // Semente inicial

// Função para definir a semente do gerador
void setSeed(unsigned long newSeed) {
    seed = newSeed;
}

// Função para gerar um número aleatório inteiro em um intervalo [valor0, valor1]
int randombtw(int valor0, int valor1) { 
    seed = (seed * 1103515245 + 12345) % (1 << 31); // Geração de novo valor
    return valor0 + (seed % (valor1 - valor0 + 1)); // Inclui valor1
}

// Função para gerar um número aleatório variavel em um intervalo [valor0, valor1]
float randomfloat(float min, float max) {
    return min + (randombtw(0, 1000) / 1000.0) * (max - min);
}

// Função para calcular fatorial
int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Função para calcular potência
float power(float base, int exp) {
    float result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// Função para restringir um valor a um intervalo
float constrain(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Função para normalizar um número de uma faixa para outra
float norm(float value, float start, float stop) {
    return constrain((value - start) / (stop - start), 0, 1);
}

// Função de mapeamento personalizada
float map(float value, float start1, float stop1, float start2, float stop2) {
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

// Função de valor absoluto personalizada
float myabs(float n) {
    return (n < 0) ? -n : n;
}

float mySqrt(float x) {
    float y = x;
    float z = (y + 1) / 2;
    
    while (z < y) {
        y = z;
        z = (y + x / y) / 2;
    }
    
    return z;
}

float dist(float x1, float y1, float x2, float y2) {
    return mySqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Funções para calcular mínimo e máximo
int min(int a, int b) {
  return (a < b) ? a : b;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

int myRound(float number) {
    return (int)(number);
}

// Estrutura para armazenar a matriz de transformação
typedef struct {
  int x, y;
} Matrix;

// Pilha para armazenar as matrizes de transformação
Matrix matrixStack[32];
int matrixStackSize = 0;

// Variáveis para armazenar a posição atual
int translationX = 0;
int translationY = 0;

// Função para salvar a matriz atual na pilha
void pushMatrix() {
  if (matrixStackSize < 32) {
    matrixStack[matrixStackSize].x = translationX;
    matrixStack[matrixStackSize].y = translationY;
    matrixStackSize++;
  }
}

// Função para restaurar a matriz anterior da pilha
void popMatrix() {
  if (matrixStackSize > 0) {
    matrixStackSize--;
    translationX = matrixStack[matrixStackSize].x;
    translationY = matrixStack[matrixStackSize].y;
  }
}

// Função para definir a posição atual
void translate(int x, int y) {
  translationX += x;
  translationY += y;
}

//Angulo utilizado para rotação de objetos 2d
int currentAngle = 0;

//Função para rotacionar objetos 2D
void rotate(int angle) {
  currentAngle = angle;
}

//Funções de áudio

//playsound
void PlayNote( u16 frequency, unsigned char length ){
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

//Funções de desenhos básicos

//Determina as sprites da personagem como PImage
PImage charSprites;

//Função para desenhar a personagem na tela.
IN_IWRAM void imageChar(int w, int h, int xo, int yo, const u16* map, int to) {
    int x, y, c;

    // Aqui, vamos calcular a linha e a coluna baseadas na relação fornecida
    int frameRow = (to / MAX_FRAMES) * 20; // Cada conjunto de 4 frames ocupa 20 linhas
    int frameCol = (to % MAX_FRAMES) * 20 + 60; // Cada frame ocupa 20 colunas, começando da coluna 60

    for (x = 0; x < w-7; x++) {
        for (y = 0; y < h-4; y++) {
            c = map[(y + frameRow-1) * (w + 40) + (x + frameCol)]; // Utilizando a relação
            if (c > 0) {
                VRAM[(y + yo) * SW + x + xo] = c;
            }
        }
    }
}

int animationFrameCounter = 0; // Contador de animação
int animationSpeed = 1; //Velocidade da animação

//Função para mudar a animação da personagem na tela.
void updateAnimation(PImage* sprite, Direction dir, int isMoving) {
    if (isMoving) {
        // Aumenta o contador a cada chamada
        animationFrameCounter++;
        // Se o contador ultrapassar a velocidade, atualize o frame
        if (animationFrameCounter >= animationSpeed) {
            animationFrameCounter = 1; // Reseta o contador
            // Atualiza os frames de acordo com a direção
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
        // Se não está se movendo, define o frame de parada
        switch (dir) {
            case DOWNCHAR:
                sprite->frame = 1; // Parar no frame 1
                break;
            case LEFTCHAR:
                sprite->frame = 4; // Parar no frame 4
                break;
            case RIGHTCHAR:
                sprite->frame = 7; // Parar no frame 7
                break;
            case UPCHAR:
                sprite->frame = 10; // Parar no frame 10
                break;
            default:
                sprite->frame = 0; // Frame padrão quando parado
                break;
        }
    }
}

//Função para pintar o pano de fundo de alguma cor
void background(int r, int g, int b) {int x,y;
 for(x=0;x<SW;x++)
 {                                                         //rgb values 0-31
  for(y=0;y<SH;y++){ VRAM[y*SW+x]=RGB(r,g,b);}       //clear all 120x80 pixels
 }
}

//Função para desenhar um pixel na tela.
void point(int x, int y) {
    // Aplicar a transformação
    x += translationX;
    y += translationY;
    if (x >= 0 && x < SW && y >= 0 && y < SH) {
        if (isStrokeEnabled) {
            int offsetX = (strokeWeightCurrent - 1) / 2;
            for (int i = 0; i < strokeWeightCurrent; i++) {
                for (int j = 0; j < strokeWeightCurrent; j++) {
                    int newX = x + i - offsetX;
                    int newY = y + j;
                    if (newX >= 0 && newX < SW && newY >= 0 && newY < SH) {
                        VRAM[newY * SW + newX] = strokeColor;
                    }
                }
            }
        }
    }
}

//Função para desenhar um pixel na tela.
void pointDraw(int x, int y) {
    if (x >= 0 && x < SW && y >= 0 && y < SH) {
        if (isStrokeEnabled) {
            int offsetX = (strokeWeightCurrent - 1) / 2;
            for (int i = 0; i < strokeWeightCurrent; i++) {
                for (int j = 0; j < strokeWeightCurrent; j++) {
                    int newX = x + i - offsetX;
                    int newY = y + j;
                    if (newX >= 0 && newX < SW && newY >= 0 && newY < SH) {
                        VRAM[newY * SW + newX] = strokeColor;
                    }
                }
            }
        }
    }
}

//Função para desenhar um pixel para preenchimento de uma forma.
void pointFill(int x, int y) {
    // Aplicar a transformação
    x += translationX;
    y += translationY;
    if (x >= 0 && x < SW && y >= 0 && y < SH) {
        if (isFillEnabled) {
            VRAM[y * SW + x] = fillColor;
        }
    }
}

int formCenterX = 0;
int formCenterY = 0;

// Função para desenhar linhas com rotação em torno de seu próprio centro
void line(int x0, int y0, int x1, int y1) {
    // Aplicar a transformação
    x0 += translationX;
    y0 += translationY;
    x1 += translationX;
    y1 += translationY;
    // Calcular o centro da linha
    int centerX = (x0 + x1) / 2;
    int centerY = (y0 + y1) / 2;

    // Se a rotação não for 0, aplicamos a rotação nos pontos
    if (currentAngle != 0) {
        // Converte o ângulo de graus para radianos
        int angleIndex = currentAngle % 360;  // Obtemos o índice da tabela
        if (angleIndex < 0) angleIndex += 360; // Garantir índice positivo

        // Usar a tabela de senos e cossenos para a rotação
        float cosAngle = cosTable[angleIndex];
        float sinAngle = sinTable[angleIndex];

        // Rotaciona o ponto (x0, y0) em torno do centro
        int newX0 = (int)(centerX + (x0 - centerX) * cosAngle - (y0 - centerY) * sinAngle);
        int newY0 = (int)(centerY + (x0 - centerX) * sinAngle + (y0 - centerY) * cosAngle);

        // Rotaciona o ponto (x1, y1) em torno do centro
        int newX1 = (int)(centerX + (x1 - centerX) * cosAngle - (y1 - centerY) * sinAngle);
        int newY1 = (int)(centerY + (x1 - centerX) * sinAngle + (y1 - centerY) * cosAngle);

        // Agora desenha a linha com os pontos rotacionados
        int dx = myabs(newX1 - newX0);
        int dy = myabs(newY1 - newY0);
        int sx = (newX0 < newX1) ? 1 : -1;
        int sy = (newY0 < newY1) ? 1 : -1;
        int err = dx - dy;

        while (1) {
            pointDraw(newX0, newY0);  // Desenha o pixel no ponto rotacionado

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
        // Se o ângulo for 0, desenha a linha normalmente
        int dx = myabs(x1 - x0);
        int dy = myabs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (1) {
            pointDraw(x0, y0);  // Desenha o pixel

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

//Função para desenhar linhas
void lineDraw(int x0, int y0, int x1, int y1) {

    // Aplicar a transformação
    x0 += translationX;
    y0 += translationY;
    x1 += translationX;
    y1 += translationY;

	int dx = (x1 - x0) >= 0 ? (x1 - x0) : (x0 - x1);
	int dy = (y1 - y0) >= 0 ? (y1 - y0) : (y0 - y1);
    int sx = (x0 < x1) ? 1 : -1; // Direção do incremento em x
    int sy = (y0 < y1) ? 1 : -1; // Direção do incremento em y
    int err = dx - dy;           // Erro inicial

    while (1) {
        pointDraw(x0, y0); // Desenha o pixel

        // Verifica se chegamos ao ponto final
        if (x0 == x1 && y0 == y1) break;

        // Calcula o erro e ajusta as coordenadas
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


// Função que inicia a definição de uma nova forma
void beginShape() {
    vertexCount = 0; // Reseta o contador de vértices
}

//Função para adicionar um vértice à forma
void vertex(int x, int y) {
    if (vertexCount < MAX_VERTICES) {
        if (currentAngle != 0) {
            int angleDeg = currentAngle % 360;
            if (angleDeg < 0) angleDeg += 360; // Garante que o índice está entre 0 e 359

            vertices[vertexCount].x = (int)(formCenterX + (x - formCenterX) * cosTable[angleDeg] - (y - formCenterY) * sinTable[angleDeg]);
            vertices[vertexCount].y = (int)(formCenterY + (x - formCenterX) * sinTable[angleDeg] + (y - formCenterY) * cosTable[angleDeg]);

        } else {
            vertices[vertexCount].x = x;
            vertices[vertexCount].y = y;
        }
        vertexCount++;
    }
}

//Função para desenhar o final de uma forma
void endShape() {
    // Encontra o retângulo que envolve o polígono
    int minX = SW, minY = SH, maxX = 0, maxY = 0;
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].y > maxY) maxY = vertices[i].y;

    }

    // Para cada pixel dentro do retângulo
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            int windingNumber = 0;
            // Para cada segmento do polígono
            for (int i = 0; i < vertexCount; i++) {
                int j = (i + 1) % vertexCount; // Conecta ao primeiro vértice

                // Verifica se o segmento cruza a linha horizontal do pixel
                if ((vertices[i].y <= y && vertices[j].y > y) || (vertices[j].y <= y && vertices[i].y > y)) {
                    // Calcula a interseção
                    int intersectionX = (vertices[i].x * (vertices[j].y - y) + vertices[j].x * (y - vertices[i].y)) / (vertices[j].y - vertices[i].y);

                    // Verifica se a interseção está à esquerda do pixel
                    if (intersectionX < x) {
                        windingNumber++;
                    }
                }
            }
            // Se o número de voltas for ímpar, o pixel está dentro do polígono
            if (windingNumber % 2 != 0) {
                pointFill(x, y); // Desenha o pixel com a cor de preenchimento
            }
        }
    }
    // Desenha o contorno
    for (int i = 0; i < vertexCount - 1; i++) {
        lineDraw(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y);
    }
    // Se close, fechado, for verdadeiro, desenha a linha de volta ao primeiro vértice
    if (vertexCount > 2) {
        lineDraw(vertices[vertexCount - 1].x, vertices[vertexCount - 1].y, vertices[0].x, vertices[0].y);
    }
}

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
            if (x + j >= 0 && x + j < SW && y + i >= 0 && y + i < SH) {
                if (isFillEnabled) {
                    pointFill(x+j, y+i);
                }
            }
        }
    }
  // Desenha a borda superior
  lineDraw(x, y, x + w - 1, y);
  // Desenha a borda inferior
  lineDraw(x, y + h - 1, x + w - 1, y + h - 1);
  // Desenha a borda esquerda
  lineDraw(x, y, x, y + h - 1);
  // Desenha a borda direita
  lineDraw(x + w - 1, y, x + w - 1, y + h - 1);
  }
}

//Função para desenhar quadrados
void square(int x, int y, int w) {
    formCenterX = x + (w / 2);
    formCenterY = y + (w / 2);
    rect(x,y,w,w);
}

void quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  formCenterX = (x1 + x2 + x3 + x4) / 4;
  formCenterY = (y1 + y2 + y3 + y4) / 4;

  beginShape();
  vertex(x1, y1);
  vertex(x2, y2);
  vertex(x3, y3);
  vertex(x4, y4);
  endShape(); // Desenha o quadrilátero
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  formCenterX = (x1 + x2 + x3) / 3;
  formCenterY = (y1 + y2 + y3) / 3;

  beginShape();
  vertex(x1, y1);
  vertex(x2, y2);
  vertex(x3, y3);
  endShape(); // Desenha o triângulo
}

void polygon(float x, float y, float radius, int npoints) {
  formCenterX = x;
  formCenterY = y;

  float angle = TWO_PI / npoints;
  beginShape();
  for (float a = 0; a < TWO_PI; a += angle) {
    float sx = x + myCos(a) * radius;
    float sy = y + mySin(a) * radius;
    vertex(sx - x, sy - y);
  }
  endShape();
}

//Função para desenhar elipses
void ellipse(int x0, int y0, int rx, int ry) {
    int x, y;
    int rxa = rx/2;
    int rya = ry/2;
    int xa = x0+rxa;
    int ya = y0+rya;
    int rx2 = rxa * rxa;
    int ry2 = rya * rya;

    // Desenhar a elipse preenchida
    for (y = -rya; y <= rya; y++) {
        for (x = -rxa; x <= rxa; x++) {
            if ((x * x * ry2 + y * y * rx2) <= (rx2 * ry2)) {
                pointFill(xa + x, ya + y);
            }
        }
    }
    
    if (rya == 0) return; // Se ry for 0, não desenha nada

    // Inicializa os pontos
    x = 0;
    y = rya;

    // Primeiro conjunto de pontos
    int d1 = ry2 - (rx2 * rya) + (0.25 * rx2);
    int dx = 2 * ry2 * x;
    int dy = 2 * rx2 * y;

    // Desenhar a elipse
    while (dx < dy) {
        // Desenhar os quatro quadrantes
        pointDraw(xa + x, ya + y);
        pointDraw(xa - x, ya + y);
        pointDraw(xa + x, ya - y);
        pointDraw(xa - x, ya - y);

        // Atualiza a decisão
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

    // Segundo conjunto de pontos
    int d2 = (ry2 * (x + 0.5) * (x + 0.5)) + (rx2 * (y - 1) * (y - 1)) - (rx2 * ry2);
    while (y >= 0) {
        // Desenhar os quatro quadrantes
        pointDraw(xa + x, ya + y);
        pointDraw(xa - x, ya + y);
        pointDraw(xa + x, ya - y);
        pointDraw(xa - x, ya - y);

        // Atualiza a decisão
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
}

//Funcao para desenhar circulos
void circle(int x0, int y0, int rx) {
    ellipse(x0, y0, rx, rx);
}

void arc(int x, int y, int width, int height, float start, float stop) {
    int startAngle = (int)(start * (180 / PI));
    int stopAngle = (int)(stop * (180 / PI));

    // Garante que o ângulo de parada seja maior que o de início
    if (stopAngle < startAngle) {
        stopAngle += 360;
    }

    int halfWidth = width / 2;
    int halfHeight = height / 2;

    // Preencher a área do arco
    for (int i = 0; i < height; i++) {
        int yLine = y + i - halfHeight; // linha atual de y
        int xMin = x, xMax = x;

        for (int angle = startAngle; angle <= stopAngle; angle++) {
            int px = x + halfWidth * cosTable[angle];
            int py = y + halfHeight * sinTable[angle];

            // Verifica se a linha cruza a linha atual (yLine)
            if (py == yLine) {
                if (px < xMin) xMin = px;
                if (px > xMax) xMax = px;
            }
        }

        // Preenche a linha entre xMin e xMax
        if (xMax > xMin) {
            for (int fillX = xMin; fillX <= xMax; fillX++) {
                pointFill(fillX, yLine); // Desenha o ponto com a cor de preenchimento
            }
        }
    }

    // Desenhar a borda do arco
    for (int angle = startAngle; angle <= stopAngle; angle++) {
        int px = x + halfWidth * cosTable[angle];
        int py = y + halfHeight * sinTable[angle];
        point(px, py); // Desenha o ponto da borda
    }
}

// Função para desenhar uma curva de Bézier com preenchimento
void bezier(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  // Encontra o retângulo que envolve o polígono
  int minX = min(x1, min(x2, min(x3, x4)));
  int maxX = max(x1, max(x2, max(x3, x4)));
  int minY = min(y1, min(y2, min(y3, y4)));
  int maxY = max(y1, max(y2, max(y3, y4)));

  // Define o número de vértices
  int vertexCount = 100;

  // Cria um array para armazenar os vértices
  int vertices[vertexCount][2];

  // Calcula os vértices da curva de Bézier
  for (int i = 0; i < vertexCount; i++) {
    float t = (float)i / (float)(vertexCount - 1);
    float u = 1 - t;
    vertices[i][0] = (int)(u * u * u * x1 + 3 * u * u * t * x2 + 3 * u * t * t * x3 + t * t * t * x4);
    vertices[i][1] = (int)(u * u * u * y1 + 3 * u * u * t * y2 + 3 * u * t * t * y3 + t * t * t * y4);
  }

  // Algoritmo Scanline Fill
  for (int y = minY; y <= maxY; y++) {
    int xMin = maxX;
    int xMax = minX;

    // Encontra os pontos de interseção com a linha horizontal
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

    // Preenche a linha entre xMin e xMax
    if (xMax > xMin) {
      for (int fillX = xMin; fillX <= xMax; fillX++) {
        pointFill(fillX, y); // Desenha o ponto com a cor de preenchimento
      }
    }
  }

  // Desenha a borda da curva
  for (int i = 0; i < vertexCount - 1; i++) {
    lineDraw(vertices[i][0], vertices[i][1], vertices[i + 1][0], vertices[i + 1][1]);
  }
}

//Funcoes para desenho 3D

// Função para projetar os vértices em 2D
void project(float x, float y, float z, int *outX, int *outY) {
    // Para simplicidade, vamos usar uma projeção perspectiva simples
    float scale = 200.0f / (z + 400.0f); // Distância da câmera
    *outX = (int)(x * scale) + (SW / 2); // Centralizar
    *outY = (int)(y * scale) + (SH / 2); // Centralizar
}

// Estrutura para armazenar a transformação atual
typedef struct {
    float x, y, z;
    float angleX, angleY, angleZ;
} Transformation;

Transformation currentTransformation = {0, 0, 0, 0, 0, 0};

void rotateX(int angleX) {
    currentTransformation.angleX += angleX;
}

void rotateY(int angleY) {
    currentTransformation.angleY += angleY;
}

void rotateZ(int angleZ) {
    currentTransformation.angleZ += angleZ;
}

void box(int x, int y, int z, int w, int h, int d) {
    // Converter coordenadas para o sistema de coordenadas do GBA
    x = x - 160;
    y = y - 80;
    float vertices[8][3] = {
        {0, 0, 0}, {w, 0, 0}, {w, h, 0}, {0, h, 0},
        {0, 0, d}, {w, 0, d}, {w, h, d}, {0, h, d}
    };

    // Aplicar transformação atual
    float radX = currentTransformation.angleX * (PI / 180);
    float radY = currentTransformation.angleY * (PI / 180);
    float radZ = currentTransformation.angleZ * (PI / 180);

    for (int i = 0; i < 8; i++) {
        float tempX = vertices[i][0];
        float tempY = vertices[i][1];
        float tempZ = vertices[i][2];

        // Rotação em torno do eixo X
        float y1 = tempY * cosTable[(int)(radX * (180 / PI)) % SIN_TABLE_SIZE] - tempZ * sinTable[(int)(radX * (180 / PI)) % SIN_TABLE_SIZE];
        float z1 = tempY * sinTable[(int)(radX * (180 / PI)) % SIN_TABLE_SIZE] + tempZ * cosTable[(int)(radX * (180 / PI)) % SIN_TABLE_SIZE];
        tempY = y1;
        tempZ = z1;

        // Rotação em torno do eixo Y
        float x1 = tempX * cosTable[(int)(radY * (180 / PI)) % SIN_TABLE_SIZE] + tempZ * sinTable[(int)(radY * (180 / PI)) % SIN_TABLE_SIZE];
        z1 = -tempX * sinTable[(int)(radY * (180 / PI)) % SIN_TABLE_SIZE] + tempZ * cosTable[(int)(radY * (180 / PI)) % SIN_TABLE_SIZE];
        tempX = x1;
        tempZ = z1;

        // Rotação em torno do eixo Z
        vertices[i][0] = tempX * cosTable[(int)(radZ * (180 / PI)) % SIN_TABLE_SIZE] - tempY * sinTable[(int)(radZ * (180 / PI)) % SIN_TABLE_SIZE];
        vertices[i][1] = tempX * sinTable[(int)(radZ * (180 / PI)) % SIN_TABLE_SIZE] + tempY * cosTable[(int)(radZ * (180 / PI)) % SIN_TABLE_SIZE];
        vertices[i][2] = tempZ; // A coordenada Z permanece inalterada
    }

    // Transladar os vértices para a posição desejada
    for (int i = 0; i < 8; i++) {
        vertices[i][0] += x + currentTransformation.x;
        vertices[i][1] += y + currentTransformation.y;
        vertices[i][2] += z + currentTransformation.z;
    }

    int projected[8][2];
    for (int i = 0; i < 8; i++) {
        project(vertices[i][0], vertices[i][1], vertices[i][2], &projected[i][0], &projected[i][1]);
    }

    // Desenhar as faces e arestas
    // Face frontal
    beginShape();
    vertex(projected[0][0], projected[0][1]);
    vertex(projected[1][0], projected[1][1]);
    vertex(projected[2][0], projected[2][1]);
    vertex(projected[3][0], projected[3][1]);
    endShape();

    // Face traseira
    beginShape();
    vertex(projected[4][0], projected[4][1]);
    vertex(projected[5][0], projected[5][1]);
    vertex(projected[6][0], projected[6][1]);
    vertex(projected[7][0], projected[7][1]);
    endShape();

     // Face esquerda
    beginShape();
    vertex(projected[0][0], projected[0][1]);
    vertex(projected[3][0], projected[3][1]);
    vertex(projected[7][0], projected[7][1]);
    vertex(projected[4][0], projected[4][1]);
    endShape();

    // Face direita
    beginShape();
    vertex(projected[1][0], projected[1][1]);
    vertex(projected[2][0], projected[2][1]);
    vertex(projected[6][0], projected[6][1]);
    vertex(projected[5][0], projected[5][1]);
    endShape();

    // Face superior
    beginShape();
    vertex(projected[3][0], projected[3][1]);
    vertex(projected[2][0], projected[2][1]);
    vertex(projected[6][0], projected[6][1]);
    vertex(projected[7][0], projected[7][1]);
    endShape();

    // Face inferior
    beginShape();
    vertex(projected[0][0], projected[0][1]);
    vertex(projected[1][0], projected[1][1]);
    vertex(projected[5][0], projected[5][1]);
    vertex(projected[4][0], projected[4][1]);
    endShape();
}

//Definição de funções de texto
//Fonts

// Função para converter int para string e concatenar
char* intToString(const char* texto, int valor) {
  static char buffer[50];
  sprintf(buffer, "%s%d", texto, valor);
  return buffer;
}

// Função para converter float para string e concatenar
char* floatToString(const char* texto, float valor) {
  static char buffer[50];
  sprintf(buffer, "%s%.2f", texto, valor);
  return buffer;
}

// Função para converter unsigned char para string e concatenar
char* unsignedCharToString(const char* texto, unsigned char valor) {
  static char buffer[50];
  sprintf(buffer, "%s%d", texto, valor);
  return buffer;
}

// Função para comparar strings
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Função para trocar de fonte
void textFont(const char* nomeDaFonte) {
    if (strcmp(nomeDaFonte, "variable_8x8_font") == 0) {
        currentFont = (const u16*) variable_8x8_font;
        charWidth = 8;
        charHeight = 8;
    } else if (strcmp(nomeDaFonte, "fixed_8x16_font") == 0) {
        currentFont = (const u16*) fixed_8x16_font;
        charWidth = 8;
        charHeight = 16;
    } else if (strcmp(nomeDaFonte, "fixed_8x8_font") == 0) {
        currentFont = (const u16*) fixed_8x8_font;
        charWidth = 8;
        charHeight = 8;
    } else if (strcmp(nomeDaFonte, "variable_8x16_font") == 0) {
        currentFont = (const u16*) variable_8x16_font;
        charWidth = 8;
        charHeight = 16;
    } else if (strcmp(nomeDaFonte, "variable_16x16_font") == 0) {
        currentFont = (const u16*) variable_16x16_font;
        charWidth = 16;
        charHeight = 16;
    } else if (strcmp(nomeDaFonte, "fixed_32x64_font") == 0) {
        currentFont = (const u16*) fixed_32x64_font;
        charWidth = 32;
        charHeight = 64;
    }
}

// Função charDraw
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

// Defina os tipos de alinhamento
typedef enum {
    LEFT,
    CENTER,
    RIGHT,
    NORMAL
} Alignment;

// Variável global para armazenar o alinhamento atual
Alignment textAlignMode = NORMAL;

// Função para definir o alinhamento do texto
void textAlign(Alignment mode) {
    textAlignMode = mode;
}

// Função text atualizada para considerar o alinhamento
void text(const char* str, int x, int y) {
    int linhaAtual = y;
    int colunaAtual = x;
    int charsPorLinha = 0;
    int larguraTexto = 0;
    int limiteCharsPorLinha = 20;
    // Calcula a largura do texto
    for (int i = 0; str[i]; i++) {
        int charIndex = str[i];
        if (charIndex == 32) {
            larguraTexto += charWidth;
        } else if (charIndex >= 33 && charIndex <= 122) {
            larguraTexto += charWidth;
        } else if (charIndex == 161 || charIndex == 160 || charIndex == 162 || charIndex == 163) {
            larguraTexto += charWidth;
        }
    }

    // Ajusta a posição inicial com base no alinhamento
    switch (textAlignMode) {
        case RIGHT:
            colunaAtual -= larguraTexto;
            break;
        case CENTER:
            colunaAtual -= larguraTexto / 2;
            break;
        case NORMAL:
            colunaAtual -= 0;
            break;
        case LEFT:
            break;
    }

    while (*str) {
        int charIndex = *str; 
        if (charIndex == 32) { 
            colunaAtual += charWidth; 
            charsPorLinha++;
        } else if (charIndex >= 33 && charIndex <= 122) { 
            int to = charIndex - 33; 
            charDraw(colunaAtual, linhaAtual, to);
            colunaAtual += charWidth; 
            charsPorLinha++;

            // Verifica se ultrapassou o limite de caracteres por linha
            if (charsPorLinha >= limiteCharsPorLinha) {
                linhaAtual += charHeight;
                colunaAtual = x;
                charsPorLinha = 0;
            }
        } else if (charIndex == 161) { // á
            charDraw(colunaAtual, linhaAtual, 200); 
            colunaAtual += charWidth; 
            charsPorLinha++;

            // Verifica se ultrapassou o limite de caracteres por linha
            if (charsPorLinha >= limiteCharsPorLinha) {
                linhaAtual += charHeight;
                colunaAtual = x;
                charsPorLinha = 0;
            }
        } else if (charIndex == 160) { // à
            charDraw(colunaAtual, linhaAtual, 201); 
            colunaAtual += charWidth; 
            charsPorLinha++;

            // Verifica se ultrapassou o limite de caracteres por linha
            if (charsPorLinha >= limiteCharsPorLinha) {
                linhaAtual += charHeight;
                colunaAtual = x;
                charsPorLinha = 0;
            }
        } else if (charIndex == 162) { // â
            charDraw(colunaAtual, linhaAtual, 202); 
            colunaAtual += charWidth; 
            charsPorLinha++;

            // Verifica se ultrapassou o limite de caracteres por linha
            if (charsPorLinha >= limiteCharsPorLinha) {
                linhaAtual += charHeight;
                colunaAtual = x;
                charsPorLinha = 0;
            }
        } else if (charIndex == 163) { // ã
            charDraw(colunaAtual, linhaAtual, 203); 
            colunaAtual += charWidth; 
            charsPorLinha++;

            // Verifica se ultrapassou o limite de caracteres por linha
            if (charsPorLinha >= limiteCharsPorLinha) {
                linhaAtual += charHeight;
                colunaAtual = x;
                charsPorLinha = 0;
            }
        }
        str++;
    }
}

int mouseX = 0;
int mouseY = 0;

void mousePosition(){ //function to press events

    if(KEY_R){ //right button press
        mouseX +=1;
        if (mouseX > width) mouseX = width;
    }
    if(KEY_L){ //left button press
        mouseX -=1;
        if (mouseX < 0) mouseX = 0;
    }
    if(KEY_U){ //up button press
        mouseY -=1;
        if (mouseY < 0) mouseY = 0;
    }
    if(KEY_D){ //down button press
        mouseY +=1;
        if (mouseY > height) mouseY = height;
    }
}


//------------
//Aqui para baixo são alternativas de implementacao de utras funces. Para usar de referencia
//------------

//Outra implementação da função arc. Esta está mais proximo do o que seria no processing. Porém, ainda não implementei a lógica corretamente. Em uma versão futura, retomar essa.

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

    // Preencher área do arco
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

    // Desenhar arco
    for (int angle = startAngle; angle <= stopAngle; angle++) {
        int px = x + halfW * cosTable[angle];
        int py = y + halfH * sinTable[angle];
        point(px, py);
    }

    // Modo PIE
    if (mode == PIE) {
        int startX = x + halfW * cosTable[startAngle];
        int startY = y + halfH * sinTable[startAngle];
        line(x, y, startX, startY);
        int stopX = x + halfW * cosTable[stopAngle];
        int stopY = y + halfH * sinTable[stopAngle];
        line(x, y, stopX, stopY);
    }

    // Modo CHORD
    if (mode == CHORD) {
        int startX = x + halfW * cosTable[startAngle];
        int startY = y + halfH * sinTable[startAngle];
        int stopX = x + halfW * cosTable[stopAngle];
        int stopY = y + halfH * sinTable[stopAngle];
        line(startX, startY, stopX, stopY);
    }

    // Modo OPEN
    if (mode == OPEN) {
        int startX = x + halfW * cosTable[startAngle];
        int startY = y + halfH * sinTable[startAngle];
        int stopX = x + halfW * cosTable[stopAngle];
        int stopY = y + halfH * sinTable[stopAngle];
        line(startX, startY, stopX, stopY);
    }
}

void ellipse_a(int x0, int y0, int rx, int ry) {
    int x, y;
    int rxa = rx / 2;
    int rya = ry / 2;
    int xa = x0 + rxa;
    int ya = y0 + rya;
    int rx2 = rxa * rxa;
    int ry2 = rya * rya;
    float cosAngle = myCos(currentAngle * (PI / 180));
    float sinAngle = mySin(currentAngle * (PI / 180));
    // Desenhar a elipse preenchida
    for (y = -rya; y <= rya; y++) {
        for (x = -rxa; x <= rxa; x++) {
            if ((x * x * ry2 + y * y * rx2) <= (rx2 * ry2)) {
                int newX = (int)(xa + (x * cosAngle) - (y * sinAngle));
                int newY = (int)(ya + (x * sinAngle) + (y * cosAngle));
                pointFill(newX, newY);
            }
        }
    }
    // Desenhar a elipse não preenchida
    if (rya == 0) return;
    // Inicializa os pontos
    x = 0;
    y = rya;
    // Primeiro conjunto de pontos
    int d1 = ry2 - (rx2 * rya) + (0.25 * rx2);
    int dx = 2 * ry2 * x;
    int dy = 2 * rx2 * y;
    // Desenhar a elipse
    while (dx < dy) {
        int newX1 = (int)(xa + (x * cosAngle) - (y * sinAngle));
        int newY1 = (int)(ya + (x * sinAngle) + (y * cosAngle));
        int newX2 = (int)(xa - (x * cosAngle) - (y * sinAngle));
        int newY2 = (int)(ya - (x * sinAngle) + (y * cosAngle));
        int newX3 = (int)(xa + (x * cosAngle) + (y * sinAngle));
        int newY3 = (int)(ya + (x * sinAngle) - (y * cosAngle));
        int newX4 = (int)(xa - (x * cosAngle) + (y * sinAngle));
        int newY4 = (int)(ya - (x * sinAngle) - (y * cosAngle));
        
        pointDraw(newX1, newY1);
        pointDraw(newX2, newY2);
        pointDraw(newX3, newY3);
        pointDraw(newX4, newY4);
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
    // Segundo conjunto de pontos
    int d2 = (ry2 * (x + 0.5) * (x + 0.5)) + (rx2 * (y - 1) * (y - 1)) - (rx2 * ry2);

    while (y >= 0) {
        int newX1 = (int)(xa + (x * myCos(currentAngle)) - (y * mySin(currentAngle)));
        int newY1 = (int)(ya + (x * mySin(currentAngle)) + (y * myCos(currentAngle)));
        int newX2 = (int)(xa - (x * myCos(currentAngle)) - (y * mySin(currentAngle)));
        int newY2 = (int)(ya - (x * mySin(currentAngle)) + (y * myCos(currentAngle)));
        int newX3 = (int)(xa + (x * myCos(currentAngle)) + (y * mySin(currentAngle)));
        int newY3 = (int)(ya + (x * mySin(currentAngle)) - (y * myCos(currentAngle)));
        int newX4 = (int)(xa - (x * myCos(currentAngle)) + (y * mySin(currentAngle)));
        int newY4 = (int)(ya - (x * mySin(currentAngle)) - (y * myCos(currentAngle)));
        
        pointDraw(newX1, newY1);
        pointDraw(newX2, newY2);
        pointDraw(newX3, newY3);
        pointDraw(newX4, newY4);

        if (d2 > 0) {
            y--;
            dy -= 2 * rx2;
            d2 += rx2 - dy;
        } else {
            y--;
            x++;
            dx += 2 * ry2;
            dy -= 2 * rx2;
            d2 += dx - dy + rx2;
        }
    }
}

// Função para desenhar uma curva bezier
void bezier_a(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  float step = 0.01; // Passo para o cálculo da curva. Quanto menor (ex. 0.001 ou 0.0001) mais preciso, porém fica mais lento...
  for (float t = 0; t <= 1; t += step) {
    float u = 1 - t; // Complemento de t
    // Cálculo das coordenadas x e y da curva
    int x = (int)(u * u * u * x1 + 3 * u * u * t * x2 + 3 * u * t * t * x3 + t * t * t * x4);
    int y = (int)(u * u * u * y1 + 3 * u * u * t * y2 + 3 * u * t * t * y3 + t * t * t * y4);
    pointDraw(x, y); // Desenha o ponto na curva
  }
}