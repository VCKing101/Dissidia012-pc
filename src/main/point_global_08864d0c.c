#include "main/agent2_types.h"

typedef struct Agent2Clock {
    unsigned char unknown_0000[0x18];
    float field_0018;
} Agent2Clock;

extern int D_089D69A8;
extern Agent2Clock *D_089D69E0;
extern Agent2Point *D_089D5EA8;
extern float D_089D5EAC;
extern char D_089C9330[];

extern void *func_08882274(int);
extern void func_088823C4(void *, int, int, void (*)(void), char *);
extern void func_08864CF0(Agent2Point *);
extern float func_08864CD0(Agent2Point *);
extern void func_08864CD8(Agent2Point *, float, float);
void func_08864DD8(void);

void func_08864D0C(Agent2Point *self) {
    func_088823C4(func_08882274(D_089D69A8), 1, 0x1ADAF, func_08864DD8,
                  D_089C9330);
    func_08864CF0(self);
}

float func_08864D64(void) {
    return func_08864CD0(D_089D5EA8);
}

int func_08864D84(void) {
    return D_089D5EA8->field_0008 & 1;
}

void func_08864D98(float first, float second) {
    func_08864CD8(D_089D5EA8, first, second);
}

void func_08864DB8(void) {
    func_08864CF0(D_089D5EA8);
}

void func_08864DD8(void) {
    Agent2Point *point = D_089D5EA8;
    if (!func_08864D84()) {
        D_089D5EAC = D_089D69E0->field_0018;
        return;
    }
    if (point->field_0004 > 0.0f) {
        point->field_0004 -= point->field_0000;
        if (point->field_0004 <= 0.0f) func_08864CF0(point);
    }
    D_089D5EAC = func_08864D64();
}
