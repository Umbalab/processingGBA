void setup() {
  // Configuração inicial
  stroke(31, 31, 31); // Cor branca para o contorno
  noFill(); // Sem preenchimento
}

void draw() {
  background(0, 0, 0); // Fundo preto
  for (int i = 0; i < 120; i += 15) {
    // Ajusta as coordenadas para caber na tela de 159x104 pixels
    bezier(mouseX - (i / 3.0), 15 + i / 3, 
           159 - (i / 6.0), 5, 
           159, 80 - (i / 12.0), 
           60 - (i / 48.0), 80 - (i / 24.0));
  }
}

void buttons(){ //function to press events
   
}