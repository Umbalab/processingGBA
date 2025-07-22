const int larguraTela = 159;
const int alturaTela = 104;

void setup() {
  // Configuração inicial
}

void draw() {
  background(0, 0, 0); // Fundo preto

  for(int i = 4; i < larguraTela; i += 4) {
    // Se 'i' é par, desenha linha branca, caso contrário, desenha linha cinza
    if((i % 8) == 0) {
      stroke(31, 31, 31); // Branco
      line(i, 20, i, alturaTela/2);
    } else {
      stroke(15, 15, 15); // Cinza
      line(i, 5, i, alturaTela/2); 
    }
  }
}

void buttons(){ //function to press events
   
}