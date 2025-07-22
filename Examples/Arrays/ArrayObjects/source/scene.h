typedef struct {
    int xOffset;
    int yOffset;
    float x, y;
    int unit;
    int xDirection;
    int yDirection;
    float speed;
} Module;

Module mods[158/20 * 103/20];

void update(Module *mod) {
    mod->x += mod->speed * mod->xDirection;
    if (mod->x >= mod->unit || mod->x <= 0) {
        mod->xDirection *= -1;
        mod->x += 1 * mod->xDirection;
        mod->y += 1 * mod->yDirection;
    }
    if (mod->y >= mod->unit || mod->y <= 0) {
        mod->yDirection *= -1;
        mod->y += 1 * mod->yDirection;
    }
}

void display(Module mod) {
    fill(31,31,31);
    ellipse(mod.xOffset + mod.x+5, mod.yOffset + mod.y+5, 6, 6);
}

void setup() {
    int wideCount = 159 / 20;
    int highCount = 104 / 20;
    int index = 0;
    
    for (int y = 0; y < highCount; y++) {
        for (int x = 0; x < wideCount; x++) {
            mods[index].xOffset = x * 20 + 5;
            mods[index].yOffset = y * 20 + 5;
            mods[index].x = 0;
            mods[index].y = 0;
            mods[index].unit = 20;
            mods[index].xDirection = 1;
            mods[index].yDirection = 1;
            mods[index].speed = randomfloat(0.05, 0.8);
            index++;
        }
    }
}

void draw() {
    background(0, 0, 0);
    
    for (int i = 0; i < 159/20 * 104/20; i++) {
        update(&mods[i]);
        display(mods[i]);
    }
}

void buttons(){ //function to press events
   
}