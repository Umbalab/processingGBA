
    const int d = 15;
    const int p1 = d;
    const int p2 = p1+d;
    const int p3 = p2+d;
    const int p4 = p3+d;

void setup() {
    translate(40, 15);
}

void draw() {
    background(0,0,0);
    // Draw gray box
    noFill();
    stroke(15,15,15);
    line(p3, p3, p2, p3);
    line(p2, p3, p2, p2);
    line(p2, p2, p3, p2);
    line(p3, p2, p3, p3);

    // Draw white points
    stroke(31,31,31);
    point(p1, p1);
    point(p1, p3); 
    point(p2, p4);
    point(p3, p1); 
    point(p4, p2);
    point(p4, p4);
}


void buttons(){ //function to press events
   
}