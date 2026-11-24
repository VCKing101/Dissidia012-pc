// Per-frame update of a HUD element: when func_08B0E724(+0x90) reports 1 or byte +0x9B is 1, tears down its effects and clears the flag; then ticks +8 and re-aims the effect at +0xA0.
extern int func_08B0E724(int);
extern void func_08B0E04C(void *);
extern void func_08B0E200(void *);
extern void func_08886824(int);
extern void func_08ABB570(int);
extern void func_08B122E4(void *);
extern void func_08B14768(void *);
extern void func_08B12248(float, void *);
extern int D_089D69F8;
extern int D_08ABBDF8;
extern float D_089D5EAC;
typedef struct Hud57 {
    char pad0[0x90];
    int f90;
    int f94;
    char pad1[3];
    unsigned char f9B;
} Hud57;
void func_08B0DEA0(Hud57 *self)
{
    int r = func_08B0E724(self->f90);
    char *fx = (char *)self + 0xA0;
    if (r == 1 || self->f9B == 1) {
        func_08B0E04C(self);
        func_08B0E200(self);
        self->f94 = 0;
        func_08886824(D_089D69F8);
        func_08ABB570(D_08ABBDF8);
        func_08B122E4(fx);
        self->f9B = 0;
    }
    func_08B14768((char *)self + 8);
    func_08B12248(D_089D5EAC, fx);
}
