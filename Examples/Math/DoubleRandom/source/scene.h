int totalPts = 300;
float steps = 301;
  
void setup() {
  stroke(31,31,31);
  //frameRate(1);
} 

void draw() {
  background(0,0,0);
  float rand = 0;
  for  (int i = 1; i < steps; i++) {
    point( (width/steps) * i, (height/2) + randomfloat(-rand/4, rand/4) );
    rand += randomfloat(-5, 5);
  }
}

void buttons(){}