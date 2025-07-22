const int larguraTela = 159;
const int alturaTela = 104;
const int barWidth = 5; 
int barColors[32][3];

void setup() {
  colorMode(HUEMODE);
  background(0, 0, 0);
  noStroke();
  
  // Inicialize as cores dos retângulos
  for (int i = 0; i < 32; i++) {
    fill(i, 0, 0);
    barColors[i][0] = i;
    barColors[i][1] = 0;
    barColors[i][2] = 0;
    rect(i * barWidth, 0, barWidth, alturaTela);
  }
}

void draw() {
  // Desenhe todos os retângulos
  for (int i = 0; i < 32; i++) {
    // Verifique se o retângulo atual precisa ser redesenhado
    if (mouseX >= i * barWidth && mouseX < (i + 1) * barWidth) {
      fill(map(mouseY, 0, alturaTela, 0, 32), 0, 0);
      barColors[i][0] = map(mouseY, 0, alturaTela, 0, 32);
      barColors[i][1] = 0;
      barColors[i][2] = 0;
    } 
    // Desenhe o retângulo
    fill(barColors[i][0], barColors[i][1], barColors[i][2]);
    rect(i * barWidth, 0, barWidth, alturaTela);
  }
}

void buttons(){ //function to press events
   
}