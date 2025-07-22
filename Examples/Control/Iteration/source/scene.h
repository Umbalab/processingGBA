const int larguraTela = 159;
const int alturaTela = 104;
const int num = 14;

void setup() {

}

void draw() {
  background(10, 10, 10); // Fundo cinza escuro
  noStroke();
  // Barras brancas
  fill(31, 31, 31); // Branco
  int y = 16;
  for(int i = 0; i < num/3; i++) {
    rect(10, y, 139, 4);
    y+=8;
  }

  // Barras cinzas
  fill(5, 5, 5); // Cinza escuro
  y = 8;
  for(int i = 0; i < num-3; i++) {
    rect(105, y, 15, 4);
    y += 8;
  }
  y = 12;
  for(int i = 0; i < num-3; i++) {
    rect(115, y, 15, 4);
    y += 8;
  }
  
  // Linhas finas
  y = 9;
  fill(0, 0, 0); // Preto
  for(int i = 0; i < num-4; i++) {
    rect(30, y, 20, 1);
    y+= 8;
  }
}

void buttons(){ //function to press events
   
}