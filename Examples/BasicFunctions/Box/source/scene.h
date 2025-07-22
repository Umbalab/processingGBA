
float angleInc = 0.0f; //Variable for rotation
float rotationIncrement = 1.0f; //Variable for rotation increment per frame

void setup() {

}

void draw() {
    
    angleInc += rotationIncrement; //Adds the rotation increment per frame to the angle value

    background(15,15,31); //change background color, background(r 0-31, g 0-31,b 0-31)
    fill(31,15,10); //change form fill color, fill(r 0-31, g 0-31,b 0-31)
    stroke(10,31,15); //change stroke color, stroke(r 0-31, g 0-31,b 0-31)
    
    if (angleInc >= 360.0f) angleInc -= 360.0f; // Keep angles within the range of 0-360

    rotateX(angleInc); //rotates the box on the x-axis
    rotateY(angleInc); //rotates the box on the y-axis
    rotateZ(angleInc); //rotates the box on the z-axis
    box(150, 45, 10, 30, 40, 25); //draw a box, box(int x, int y, int z, int w, int h, int d)
    
}

void buttons(){
   
}