
int incrementalRotation = 0; //Here we are creating a variable to store the rotation value.
void setup() {
}

void draw() {
    background(15,15,31); //change background color, background(r 0-31, g 0-31,b 0-31)
    rotate(incrementalRotation); //Here we are using the rotation function (angle in Pi radians)
    fill(31,15,10); //change form fill color, fill(r 0-31, g 0-31,b 0-31)
    stroke(10,31,15); //change stroke color, stroke(r 0-31, g 0-31,b 0-31)
    rect(width/2,height/2,30,15); //draw a rect, rect(x,y,w,h)
    incrementalRotation++; //Here we are incrementing the rotation vaule by 1 on each frame update
}

void buttons(){ //function to press events
   
}