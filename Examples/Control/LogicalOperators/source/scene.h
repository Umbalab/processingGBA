const int larguraTela = 159;
const int alturaTela = 104;

boolean test = false;

void setup() {

}

void draw() {
    background(10, 10, 10); 
  for (int i = 2; i <= alturaTela; i += 2) {
    stroke(0, 0, 0); 
    if ((i > 15) && (i < 34)) {
      line(larguraTela/4, i, larguraTela/2, i);
      test = false;
    }
  
    stroke(15, 15, 15); 
    if ((i <= 15) || (i >= 34)) {
      line(larguraTela/2, i, larguraTela, i);
      test = true;
    }
  
    if (test) {
      stroke(0, 0, 0); 
      point(larguraTela/3, i);
    }
    
    if (!test) {
      stroke(31, 31, 31); 
      point(larguraTela/4, i);
    }
  }
}

void buttons(){ //function to press events
   
}