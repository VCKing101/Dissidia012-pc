/* When the transition finished, shuts down the screen's subsystems and moves to state 9. */
extern int func_088FEA4C(void);
extern void func_08B3C248(void *self);
extern void func_08B3C338(void *self);
extern void func_08B3C8B0(void *self);
extern void func_0891F740(void *g);
extern void func_08B436D8(int a);
extern void func_08B422B8(int a);
extern void func_08B4280C(int a);
extern void func_08841EF8(void);
extern void func_088A95E4(void *g);
extern void func_08B44594(int a);
extern void func_08844AB4(void *g);
extern void *D_089D8520;
extern int D_08B438C0;
extern int D_08B426B0;
extern int D_08B42A98;
extern void *D_089D6DC8;
extern int D_08B44F08;
extern void *D_089D4960;
extern char *D_089D6AC8;

typedef struct B368E4 { int state; char pad[0xC8]; unsigned char fCC; } B368E4;
typedef struct S368E4 { char pad[0xC4]; B368E4 *b; } S368E4;

void func_08B368E4(S368E4 *self)
{
    if (func_088FEA4C() != 0) {
        return;
    }
    func_08B3C248(self);
    func_08B3C338(self);
    func_08B3C8B0(self);
    func_0891F740(D_089D8520);
    func_08B436D8(D_08B438C0);
    func_08B422B8(D_08B426B0);
    func_08B4280C(D_08B42A98);
    func_08841EF8();
    func_088A95E4(D_089D6DC8);
    func_08B44594(D_08B44F08);
    func_08844AB4(D_089D4960);
    if (self->b->fCC != 0) {
        int zero = 0;
        D_089D6AC8[0x6B2] = zero;
    }
    self->b->state = 9;
}
