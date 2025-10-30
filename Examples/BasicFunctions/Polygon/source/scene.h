

void setup() {
}

void draw() {
    background(15,15,31); //change background color, background(r 0-31, g 0-31,b 0-31)
    fill(31,15,10); //change form fill color, fill(r 0-31, g 0-31,b 0-31)
    stroke(10,31,15); //change stroke color, stroke(r 0-31, g 0-31,b 0-31)
    polygon(25,40,15,5); //Function to draw polygons, polygon(float x, float y, float radius, int npoints)
    noFill(); //remove form fill
    polygon(60,40,15,6);
    noStroke(); //remove form stroke
    fill(31,15,10);
    polygon(95,40,15,7);
}

void buttons(){ //function to press events
   
}