void setup() {
  background(0, 0, 0); 
  noStroke();
  textFont("fixed_8x8_font");
}

void draw() {
  background(0,0,0);//clear background
  fill(31,31,31);
  char c = 'A';
  float f = (float)c;
  int i = (int)(f * 1.4);
  unsigned char b = (unsigned char)(c / 2);

  text("Variables:", 2, 5);
  text(intToString("Variable c = ", c), 2, 25);
  text(floatToString("Variable f = ", f), 2, 45);
  text(intToString("Variable i = ", i), 2, 65);
  text(intToString("Variable b = ", b), 2, 85);
}

void buttons(){ //function to press events
   
}