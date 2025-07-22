

void setup() {
}

void draw() {
    background(15,15,31);
    fill(31,15,10); //change form fill color, fill(r 0-31, g 0-31,b 0-31)
    stroke(10,31,15); //change stroke color, stroke(r 0-31, g 0-31,b 0-31)
    square(mouseX,mouseY,5); //draw a square, square(x,y,size). Also, here is used the mouseX and mouseY positions, Which start at 0.0 and are modified by the joystick's directional arrows
}

void buttons(){ //function to press events
   
}