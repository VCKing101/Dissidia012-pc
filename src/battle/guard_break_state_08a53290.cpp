/* Fighter state callback, guard break start: plays the break motion (0xF4
 * for kind 3, 0xF2 with any of the guard skills 0x38-0x3A, else 0xF0),
 * counts the break while held and enters state 0x13. */
extern "C" {

extern void func_08899300(void);
extern int func_088374B0(void *fighter);
extern void *func_08835778(void *fighter);
extern void *func_088BF4CC(void *skills, int id);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void func_08834C10(void *fighter, int state, unsigned short arg);

}

struct BreakFighter {
    unsigned char pad_0000[0x130];
    unsigned char motion[0xF45];
    unsigned char breaks;

    static bool has_skill(BreakFighter *fighter, int id) { return func_088BF4CC(func_08835778(fighter), id) != 0; }
};

extern float D_08A548EC;

extern "C" {

void func_08A53290(BreakFighter *self, unsigned int kind)
{
    int motion;
    func_08899300();
    BreakFighter *fighter = self;
    if (kind != 3) {
        motion = 0xF0;
        if (func_088374B0(self) == 0) {
            if (BreakFighter::has_skill(self, 0x38) || BreakFighter::has_skill(self, 0x39) || BreakFighter::has_skill(self, 0x3A)) {
                motion = 0xF2;
            }
        }
    } else {
        motion = 0xF4;
    }
    func_0882FA10(0.0f, D_08A548EC, self->motion, motion, 0);
    if (func_088374B0(self) != 0) {
        fighter->breaks++;
    }
    func_08834C10(self, 0x13, kind);
}

}
