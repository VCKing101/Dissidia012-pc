// Advances the timer at +0x64 by the global frame step; once it reaches +0x68, finds the owner via func_08B21058, sets its two flag words (+0x388/+0x38C) from constants, fires func_08889944 and moves to state 2. Returns -1 if no owner.
extern int func_08B21058(void *);
extern void func_08889944(int, int);
extern unsigned long long D_08B212E0;
extern int D_089D6A10;
typedef struct Step57 {
    char pad0[0x18];
    float step;
} Step57;
extern Step57 *D_089D69E0;
typedef struct Own57 {
    char pad0[4];
    int state;
    char pad1[0x5C];
    float timer;
    float limit;
} Own57;
typedef struct Flags57 {
    char pad0[0x388];
    unsigned long long bits;
} Flags57;
int func_08B21220(Own57 *self)
{
    float t = self->timer + D_089D69E0->step;
    self->timer = t;
    if (!(t < self->limit)) {
        Flags57 *o = (Flags57 *)func_08B21058(self);
        if (o == 0) {
            return -1;
        }
        o->bits = o->bits | D_08B212E0;
        func_08889944(D_089D6A10, 8);
        self->state = 2;
    }
    return 0;
}
