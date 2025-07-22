float coswave[159];

void setup() {
    background(31, 31, 31);  // Fundo branco
    
    // Calcula valores de seno para a onda
    for (int i = 0; i < 159; i++) {
        float amount = map(i, 0, 159, 0, PI);
        coswave[i] = myabs(myCos(amount));
    }
}

void draw() {
    int y1 = 0;
    int y2 = 104/3;
    
    // Desenha primeira linha
    for (int i = 0; i < 159; i++) {
        stroke(coswave[i]*31, coswave[i]*31,coswave[i]*31);
        line(i, y1, i, y2);
    }
    
    y1 = y2;
    y2 = y1 + y1;
    
    // Desenha segunda linha
    for (int i = 0; i < 159; i++) {
        stroke(31*coswave[i]/2, 31*coswave[i]/2, 31*coswave[i]/2);
        line(i, y1, i, y2);
    }
    
    y1 = y2;
    y2 = 104;
    
    // Desenha terceira linha
    for (int i = 0; i < 159; i++) {
        stroke(31 - (coswave[i] * 31), 31 - (coswave[i] * 31), 31 - (coswave[i] * 31));
        line(i, y1, i, y2);
    }
}

void buttons(){ //function to press events
   
}