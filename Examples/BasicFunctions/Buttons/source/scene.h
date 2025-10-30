
void setup() {
}

void draw() {
    background(15,15,31); //change background color, background(r 0-31, g 0-31,b 0-31)
    fill(positionZ,positionW,positionK); //change form fill color, fill(r 0-31, g 0-31,b 0-31)
    stroke(10,31,15); //change stroke color, stroke(r 0-31, g 0-31,b 0-31)
    square(positionX,positionY,30); //draw a square, square(x,y,size)
}

void buttons(){ //function to press events

    //List of the button states that we can be use:
        //keyReleased(keyName) //map the released button state
        //keyPressed(keyName)
        //keyCurrent(keyName)
        //keyPrevious(keyName)
    //List of key names:
        //KEY_R right
        //KEY_L left
        //KEY_U up
        //KEY_D down
        //KEY_A A
        //KEY_B B
        //KEY_LS Left Sholder
        //KEY_RS Right Sholder
        //KEY_ST Start
        //KEY_SL Select

    //Below, in a chain of If statements, the pressed buttons are shown. Try changing the status type to test. :)

    if(keyHeld(KEY_R)){ //right button press
        positionX +=1;
        if (positionX > width-30){
            positionX = width-30;
        }
    }
    if(keyHeld(KEY_L)){ //left button press
        positionX -=1;
        if (positionX < 0){
            positionX = 0;
        }
    }
    if(keyHeld(KEY_U)){ //up button press
        positionY -=1;
        if (positionY < 0){
            positionY = 0;
        }
    }
    if(keyHeld(KEY_D)){ //down button press
    positionY +=1;
        if (positionY > height-16){
            positionY = height-16;
        }
    }
    if(keyHeld(KEY_A)){ //A button press
        positionZ -=1;
        if(positionZ< 0){
            positionZ= 0;}
    } 
    if(keyHeld(KEY_B)){ //B button press
        positionZ +=1;
        if(positionZ>31){
            positionZ=31;
        }
    } 
    if(keyHeld(KEY_LS)){ //L button press
        positionW -=1;
        if(positionW< 0){
            positionW= 0;
        }
    }
    if(keyHeld(KEY_RS)){ //R button press
        positionW +=1;
        if(positionW>31){
            positionW=31;
        }
    } 
    if(keyHeld(KEY_ST)){ //Start button press
        positionK +=1;
        if(positionK>31){
            positionK=31;
        }
    } 
    if(keyHeld(KEY_SL)){ //Select button press
        positionK -=1;
        if(positionK< 0){
            positionK= 0;
        }
    }
}