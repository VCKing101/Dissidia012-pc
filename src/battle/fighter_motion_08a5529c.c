typedef struct Fighter5529C {
    char pad_000[0x180];
    short motion;
} Fighter5529C;

extern int func_088374B0(Fighter5529C *fighter);
extern void func_08834C10(Fighter5529C *fighter, int state, int arg);

void func_08A5529C(void *self, Fighter5529C *fighter)
{
    short motion = fighter->motion;
    if (motion != 0x320 && motion != 0xF1 && motion != 0x65) {
        if (func_088374B0(fighter)) {
            func_08834C10(fighter, 0x12, 0);
        } else {
            func_08834C10(fighter, 0, 0);
        }
    }
}
