static float distances[80][52];
float maxDistance;
int spacer;

void setup() {
    background(0, 0, 0);  // Fundo preto
    
    // Calcula distâncias máximas
    maxDistance = mySqrt((80 - 40) * (80 - 40) + (52 - 26) * (52 - 26));
    
    // Inicializa matriz de distâncias
    for (int y = 0; y < 52; y++) {
        for (int x = 0; x < 80; x++) {
            float distance = mySqrt((x - 40) * (x - 40) + (y - 26) * (y - 26));
            distances[x][y] = distance / maxDistance * 31;
        }
    }
    
    spacer = 2;
}


void draw() {
    // Limpa tela
    background(0, 0, 0);
    
    // Desenha quadrados com cor baseada na distância
    for (int y = 0; y < 52; y += spacer) {
        for (int x = 0; x < 80; x += spacer) {
            int color = (int)distances[x][y];
            fill(color, color, color);
            noStroke();
            square((x * 2) + spacer/2, (y * 2) + spacer/2, 1.5);
        }
    }
}


void buttons(){ //function to press events
   
}