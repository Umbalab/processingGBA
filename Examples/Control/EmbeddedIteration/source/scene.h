const int larguraTela = 159;
const int alturaTela = 104;
const int tamanhoGrade = 14;

void setup() {
  // Configuração inicial
}

void draw() {
  background(0, 0, 0); // Fundo preto

  for(int x = tamanhoGrade; x <= larguraTela - tamanhoGrade; x += tamanhoGrade) {
    for(int y = tamanhoGrade; y <= alturaTela - tamanhoGrade; y += tamanhoGrade) {
      stroke(15, 15, 15); // Cinza
      line(x, y, larguraTela/2, alturaTela/2);
      noStroke();
      fill(31, 31, 31); // Branco
      rect(x-1, y-1, 3, 3);
    }
  }
}

void buttons(){ //function to press events
   
}