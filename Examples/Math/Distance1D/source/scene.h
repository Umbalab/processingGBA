float xpos1;
float xpos2;
float xpos3;
float xpos4;
int thin = 2;
int thick = 9;

void setup() 
{
  noStroke();
  xpos1 = width/2;
  xpos2 = width/2;
  xpos3 = width/2;
  xpos4 = width/2;
}

void draw() 
{
  background(0,0,0);
  
  float mx = mouseX * 0.4 - width/5.0;
  
  fill(10,10,10);
  rect(xpos2, 0, thick, height/2);
  fill(20,20,20);
  rect(xpos1, 0, thin, height/2);
  fill(10,10,10);
  rect(xpos4, height/2, thick, height/2);
  fill(20,20,20);
  rect(xpos3, height/2, thin, height/2);
    
  xpos1 += mx/4;
  xpos2 += mx/16;
  xpos3 -= mx/4;
  xpos4 -= mx/16;
  
  if(xpos1 < -thin)  { xpos1 =  width; }
  if(xpos1 >  width) { xpos1 = -thin; }
  if(xpos2 < -thick) { xpos2 =  width; }
  if(xpos2 >  width) { xpos2 = -thick; }
  if(xpos3 < -thin)  { xpos3 =  width; }
  if(xpos3 >  width) { xpos3 = -thin; }
  if(xpos4 < -thick) { xpos4 =  width; }
  if(xpos4 >  width) { xpos4 = -thick; }
}

void buttons(){}