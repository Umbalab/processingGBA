

void setup() {
}

void draw() {
    background(15,15,31); //change background color, background(r 0-31, g 0-31,b 0-31)
    fill(31,15,10); //change form fill color, fill(r 0-31, g 0-31,b 0-31)
    stroke(10,31,15); //change stroke color, stroke(r 0-31, g 0-31,b 0-31)
    triangle(25, 70, 40, 50, 55, 70); //draw a triangle, triangle(int x1, int y1, int x2, int y2, int x3, int y3)
    noFill(); //remove form fill
    triangle(65, 70, 80, 50, 95, 70);
    noStroke(); //remove form stroke
    fill(31,15,10);
    triangle(105, 70, 120, 50, 135, 70);
}

void buttons(){ //function to press events
   
}