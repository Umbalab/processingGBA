const int larguraTela = 159;
const int alturaTela = 104;

void setup() {
  // Configuração inicial
}

void draw() {
  background(0, 0, 0); // Fundo preto

  for(int i = 2; i < larguraTela; i += 2) {
    // Se 'i' divide por 8 sem resto
    if((i % 20) == 0) {
      stroke(31, 31, 31); // Branco
      line(i, 20, i, alturaTela/2);
    } 
    // Se 'i' divide por 10 sem resto
    else if((i % 10) == 0){
      stroke(15, 15, 15); // Cinza
      line(i, 5, i, alturaTela/2);
    } 
    // Se nenhuma das condições acima for atendida
    else{ 
      stroke(8, 8, 8); // Cinza escuro
      line(i, alturaTela/2, i, alturaTela-5);
    }
  }
}

void buttons(){ //function to press events
   
}