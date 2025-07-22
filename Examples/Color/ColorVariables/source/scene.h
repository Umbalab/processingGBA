const int larguraTela = 159;
const int alturaTela = 104;

void setup() {
  noStroke();
  background(16, 0, 0); // Fundo preto
}

void draw() {
  // Desenha o primeiro conjunto de retângulos
  pushMatrix();
  translate(10, 20); // Posição esquerda
  fill(9, 2, 0); // Cor de fora (equivalente a #993300)
  rect(0, 0, 60, 60);
  fill(12, 9, 0); // Cor do meio (equivalente a #CC9900)
  rect(12, 18, 36, 36);
  fill(15, 6, 0); // Cor de dentro (equivalente a #CC6600)
  rect(18, 27, 24, 24);
  popMatrix();

  // Desenha o segundo conjunto de retângulos
  pushMatrix();
  translate(90, 20); // Posição direita
  fill(15, 6, 0); // Cor de dentro (equivalente a #CC6600)
  rect(0, 0, 60, 60);
  fill(9, 2, 0); // Cor de fora (equivalente a #993300)
  rect(12, 18, 36, 36);
  fill(12, 9, 0); // Cor do meio (equivalente a #CC9900)
  rect(18, 27, 24, 24);
  popMatrix();
}

void buttons(){ //function to press events
   
}