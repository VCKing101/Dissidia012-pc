/* Initialises a panel from a data record: stores the arguments and sets up each sub-part. */
typedef struct Dat { short s0, s2, s4, s6; signed char c8, c9, cA, cB, cC, cD, cE; } Dat;
typedef struct Panel {
    char pad0[0x28]; int f28;
    char pad1[0x10EC - 0x2C]; int f10EC;
    char pad2[0x111C - 0x10F0]; Dat **ref; int f1120;
} Panel;
extern int func_088729A0(int);
extern void func_088D46E4(void *, int, int, int, int, int);
extern void func_08A55ACC(void *, int);
extern void func_08A55F7C(void *, int, int, int);
extern void func_08A56014(void *, void *, void *, void *, int);
extern void func_08A5661C(void *, int, int, int, int);
extern void func_08A56AF0(void *, int);
extern void func_08A56D70(void *, int, int);
extern void func_08A575D8(Panel *, int);
extern void func_08A57F28(Panel *);
void func_08A581F0(Panel *self, Dat **ref, int p3, int p4, int p5) {
    char *grid;
    Dat *d;
    int n;
    self->f1120 = p4;
    self->f28 = p5;
    self->ref = ref;
    self->f10EC = func_088729A0(0);
    d = *self->ref;
    func_08A5661C((char *)self + 0x100, 0, d->c8, d->c9, d->cA);
    d = *self->ref;
    func_08A5661C((char *)self + 0x33C, 1, d->cC, d->cD, d->cE);
    n = self->f28;
    d = *self->ref;
    grid = (char *)self + 0xAB0;
    if (n <= 0) {
        if (n >= 0) {
            func_08A55F7C(grid, 0, d->s0, p3);
            d = *self->ref;
        }
    } else if (n < 2) {
        func_08A55F7C(grid, 1, d->s0, p3);
        d = *self->ref;
    }
    func_08A55ACC((char *)self + 0x578, d->s6);
    func_08A56AF0((char *)self + 0xDFC, (*self->ref)->s2);
    func_08A56D70((char *)self + 0xF54, (*self->ref)->s4, p3);
    func_08A56014(grid, (char *)self + 0x34, (char *)self + 0xD4, (char *)self + 0xD0, 0);
    func_08A575D8(self, p3);
    func_088D46E4((char *)self + 0x10F0, 0, 0, 6, 0, -1);
    func_08A57F28(self);
}
