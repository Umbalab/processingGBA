
void setup() {

}

void draw() {
    background(15,15,31); //change background color, background(r 0-31, g 0-31,b 0-31)
    noStroke(); //remove form stroke
    fill(31,15,10); //change form fill color, fill(r 0-31, g 0-31,b 0-31)
    beginShape();     //Start drawing a polygon (maximum 100 vertices!)
    vertex(32, 60);  //first vertex - vertex notation refers as follows vertex(Xposition,Yposition)
    vertex(56, 72);  //second vertex
    vertex(46, 82);  //third vertex - I suggest at least three vertices to make sense of using this function
    vertex(52, 94);  //fourth vertex
    vertex(36, 82);  //fifth vertex
    vertex(24, 88);  //sixth vertex
    vertex(28, 76);  //seventh vertex
    vertex(24, 66);  //eighth vertex - other vertices could be added, but we'll stop here
    endShape();       //Finalizes the drawing endshape()

    stroke(10,31,15); //change stroke color, stroke(r 0-31, g 0-31,b 0-31)
    noFill(); //remove form fill
    beginShape();
    vertex(88, 60);
    vertex(112, 72);
    vertex(102, 82);
    vertex(108, 94);
    vertex(92, 82);
    vertex(80, 88);
    vertex(84, 76);
    vertex(80, 66);
    endShape();
    
    fill(31,15,10);
    beginShape();
    vertex(58, 10);
    vertex(82, 22);
    vertex(72, 32);
    vertex(78, 44);
    vertex(62, 32);
    vertex(50, 38);
    vertex(54, 26);
    vertex(50, 16);
    endShape();
    
}

void buttons(){ //function to press events

}