int a = 0;      // Create a variable "a" of the datatype "int"
float b = 0.0;  // Create a variable "b" of the datatype "float"

void setup() {
  stroke(31,31,31);
}

void draw() {
  background(0,0,0);
  
  a = a + 1;
  b = b + 0.2; 
  line(a, 1, a, (height/2)-1);
  line(b, 1+height/2, b, height-1);
  
  if(a > width) {
    a = 0;
  }
  if(b > width) {
    b = 0;
  }
}

void buttons(){ //function to press events
   
}