

void setup() {
}

void draw() {
    background(15,15,31); //change background color, background(r 0-31, g 0-31,b 0-31)
    fill(31,15,10); //change form fill color, fill(r 0-31, g 0-31,b 0-31)
    stroke(10,31,15); //change stroke color, stroke(r 0-31, g 0-31,b 0-31)
    square(25,40,30); //draw a square, square(x,y,size)
    noFill(); //remove form fill
    square(60,40,30);
    noStroke(); //remove form stroke
    fill(31,15,10);
    square(95,40,30);
}

void buttons(){ //function to press events
   
}