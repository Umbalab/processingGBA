boolean b = false;

int d = 5;
int meio = width/2;
int ajuste = 7;

void setup() {
  stroke(31, 31, 31); 
}

void draw() {
  background(0, 0, 0);
  for (int i = d; i <= width; i += d) {
    
    if (i < meio) {
      b = true;
    } else {
      b = false;
    }
    
    if (b == true) {
      // Linha vertical
      line(i, d+ajuste, i, height-d-ajuste);
    }
    
    if (b == false) {
      // Linha horizontal
      line(meio, i - meio + d+ajuste, width-d, i - meio + d+ajuste);
    }
  }
}

void buttons(){ //function to press events
   
}