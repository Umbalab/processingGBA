const int larguraTela = 159;
const int alturaTela = 104;

void setup() {
}

void draw() {
  background(0, 0, 0); // Fundo preto
  stroke(19, 19, 19); // Cor cinza claro
  strokeWeight(2); 

  int a = 12;
  int b = 30;
  int c = 45;

  line(a, b, a+c, b);
  line(a, b+5, a+c, b+5);
  line(a, b+10, a+c, b+10);
  line(a, b+15, a+c, b+15);

  a = a + c;
  b = alturaTela - b;

  line(a, b, a+c, b);
  line(a, b+5, a+c, b+5);
  line(a, b+10, a+c, b+10);
  line(a, b+15, a+c, b+15);

  a = a + c;
  b = alturaTela - b;

  line(a, b, a+c, b);
  line(a, b+5, a+c, b+5);
  line(a, b+10, a+c, b+10);
  line(a, b+15, a+c, b+15);
}
void buttons(){ //function to press events
   
}