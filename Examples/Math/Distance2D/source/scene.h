float max_distance;

void setup() {
  //stretchView(-43,-41);
  noStroke();
  max_distance = dist(0, 0, width, height);
}

void draw() {
  background(0,0,0);

  for(int i = 0; i <= width; i += 5) {
    for(int j = 0; j <= height; j += 5) {
      float size = dist(mouseX, mouseY, i, j);
      size = size/max_distance * 16;
      ellipse(i, j, size,size);
    }
  }
}

void buttons(){ //function to press events
   
}
