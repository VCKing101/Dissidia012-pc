/* Finishes a mode round: picks the result code, stops the round sound,
   resets the game mode when no menu is open, and moves to state 2. */
typedef struct ClockInfo {
    char pad_0000[0xBDE8];
    unsigned int flags;
} ClockInfo;

typedef struct ModeCtl {
    char pad_000[0x48];
    int state;
    int kind;
    char pad_050[0x7C5 - 0x50];
    signed char result;
    char pad_7C6[2];
    ClockInfo *clock;
    char pad_7CC[0x804 - 0x7CC];
    int sound;
} ModeCtl;

typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

extern int D_089D6C88;
extern int D_089D6C30;
extern int D_089D6AE8;
extern int D_089D69F8;
extern int D_08B11E28;
extern int D_08ABBDF8;

extern void func_088A18B0(int bank, int volume, int flag);
extern void func_0889F234(int bank, int sound);
extern void func_08895F60(int p);
extern int func_08B0DB90(int menu, int flag);
extern void func_08886824(int p);
extern void func_08873DB0(Vec4 *v, int mode);
extern void func_08ABB570(int p);
extern void func_08883F78(void);
extern void func_08883F64(void);

void func_08888868(ModeCtl *self)
{
    self->result = 0;
    if (self->kind == 9 && (unsigned char)((self->clock->flags & 2) ? 1 : 0)) {
        self->result = 5;
    }
    if (self->result == 0 && self->sound != -1) {
        func_088A18B0(D_089D6C88, 100, 0);
        func_0889F234(D_089D6C30, self->sound);
    }
    func_08895F60(D_089D6AE8);
    if (func_08B0DB90(D_08B11E28, 0) == 0) {
        Vec4 v;
        func_08886824(D_089D69F8);
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        v.w = 0.0f;
        func_08873DB0(&v, 2);
        func_08ABB570(D_08ABBDF8);
    }
    func_08883F78();
    func_08883F64();
    self->state = 2;
}
