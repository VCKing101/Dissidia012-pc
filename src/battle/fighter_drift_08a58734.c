typedef struct Vec358734 {
    float x;
    float y;
    float z;
} Vec358734;

typedef struct Fighter58734 {
    char pad_000[0x180];
    short motion;
    char pad_182[0x226];
    Vec358734 *position;
} Fighter58734;

extern int func_08838074(Fighter58734 *fighter);
extern float func_08838300(Fighter58734 *fighter);
extern void func_08834C10(Fighter58734 *fighter, int state, int arg);

void func_08A58734(void *self, Fighter58734 *fighter)
{
    int stop = 0;
    if (func_08838074(fighter)) {
        stop = 1;
    } else if (fighter->motion != 0x1CC) {
        stop = 1;
    }
    if (stop) {
        func_08834C10(fighter, 0, 0);
    } else {
        Vec358734 *position = fighter->position;
        position->x += func_08838300(fighter);
    }
}
