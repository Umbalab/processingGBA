void setup() {

}

void draw() {
  background(15, 15, 15);

  pushMatrix();
  translate(width*0.2, height*0.5);
  rotate(frameCount);
  polygon(0, 0, 15, 3); // Triangle
  popMatrix();

  pushMatrix();
  translate(width*0.5, height*0.5);
  rotate(frameCount);
  polygon(0, 0, 15, 20-mouseX); // Icosahedron
  popMatrix();

  pushMatrix();
  translate(width*0.8, height*0.5);
  rotate(frameCount);
  polygon(0, 0, 15, 7-mouseX); // Heptagon
  popMatrix();
}

void buttons(){ //function to press events
   
}