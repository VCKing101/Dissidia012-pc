/* After the result screen closes, moves the mode to the next step: retry
   (state 6, opening the retry menu) when allowed, otherwise state 7. */
extern "C" {

struct ClockInfo;

struct ModeCtl {
    char pad_00[0x48];
    int state;
    int kind;
    int step;
    char pad_54[0x68 - 0x54];
    unsigned char locked;
    char pad_69[0x7C8 - 0x69];
    ClockInfo *clock;

    static unsigned char IsPair(int kind)
    {
        int pair = 0;
        if (kind == 1 || kind == 2 || kind == 3) {
            pair = 1;
        }
        return pair;
    }
};

struct SaveInfo {
    signed char mode;
    char pad_01[0x2A44 - 1];
    unsigned int flags;
};

extern int D_08B0CF20;
extern int D_08B12DE0;
extern SaveInfo *D_089D8508;
extern unsigned char D_089D5F20;

void func_08B158C8(void);
int func_08B05DEC(int p);
int func_088C99E4(ClockInfo *clock);
void func_0888A8EC(void);
void func_08B129E0(int p);

void func_08888AC0(ModeCtl *self)
{
    func_08B158C8();
    if (func_08B05DEC(D_08B0CF20) == 1) {
        int closed = 0;
        int busy = func_088C99E4(self->clock);
        int kind = self->kind;
        if (busy == 0) {
            closed = 1;
        }
        if (kind == 9) {
            if (closed) {
                func_0888A8EC();
            }
        } else {
            unsigned char pair = ModeCtl::IsPair(kind);
            int ok = 1;
            if (pair == 1 && (self->locked == 1 || self->step == 4)) {
                ok = 0;
            } else if (self->step == 3) {
                ok = 0;
            }
            if (kind == 10) {
                ok = 0;
            }
            if (kind == 7) {
                ok = 0;
            }
            if (kind == 12) {
                ok = 0;
            }
            SaveInfo *save = D_089D8508;
            if (save->mode == 1 && !(unsigned char)((save->flags & 4) ? 1 : 0)) {
                ok = 0;
            }
            if (D_089D5F20 != 0) {
                ok = 0;
            }
            if (ok == 1) {
                func_08B129E0(D_08B12DE0);
                self->state = 6;
            } else {
                self->state = 7;
            }
        }
    }
}

}
