//width = 158
//height = 105

void setup() {

}

void draw() {

    background(0,0,0);
    noStroke();

    fill(28,28,28);
    triangle(4, 0, 4, 104, 20, 104);

    fill(14,14,14);
    rect(20, 30, 15, 15);

    fill(28,28,28);
    quad(47, 6, 54, 6, 54, 104, 36, 104);

    fill(31,31,31);
    ellipse(54, 36, 18, 18);

    fill(28,28,28);
    triangle(71, 0, 87, 104, 71, 104); 
    fill(31,31,31);
    arc(120, 75, 70, 70, PI, TWO_PI);
}

void buttons(){ //function to press events
   
}
