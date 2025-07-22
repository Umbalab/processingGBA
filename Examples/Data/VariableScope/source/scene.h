const int larguraTela = 159;
const int alturaTela = 104;
int a = 6;  // Variável global "a"

void drawOutraLinha() {
  // Variável local "a"
  int a = 108;
  // Desenhar linha usando variável local "a"
  line(a, 0, a, alturaTela);
}

void drawMaisUmaLinha() {
  // Desenhar linha usando variável global "a"
  line(a+2, 0, a+2, alturaTela);
}

void setup() {
}

void draw() {
  background(0, 0, 0); // Fundo preto
  stroke(31, 31, 31); // Cor branca
  // Desenhar linha usando variável global "a"
  line(a, 0, a, alturaTela);
  
  // Variável local "a" para o loop
  for (int a = 20; a < 62; a += 2) {
    line(a, 0, a, alturaTela);
  }
  
  // Nova variável local "a" para draw()
  int a = 100;
  // Desenhar linha usando nova variável local "a"
  line(a, 0, a, alturaTela);  
  
  // Chamar função personalizada drawOutraLinha()
  drawOutraLinha();
  
  // Chamar função personalizada drawMaisUmaLinha()
  drawMaisUmaLinha();
}
void buttons(){ //function to press events
   
}