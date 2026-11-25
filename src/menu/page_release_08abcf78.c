/* Menu page close: releases the 0x35 handle slots, fades out, switches to the next state and stores its result. */
typedef struct W { char p[0x8F4]; int h[0x35]; } W;
typedef struct O { char p[0x14C]; W *w; } O;
extern void func_0887BA5C(int);
extern void func_08885220(int);
extern int func_088D4C5C(void);
extern void func_08873D34(float, int, void (*)(void), int, int, int);
extern void func_08873DEC(int, int);
extern int func_08ABC868(O *);
extern int D_089D69E8;
extern void func_08ABF760(void);
void func_08ABCF78(O *self) {
    int i;
    int off;
    for (i = 0, off = 0; i < 0x35; i++, off += 4) {
        if (*(int *)((char *)self->w + off + 0x8F4)) {
            func_0887BA5C(*(int *)((char *)self->w + off + 0x8F4));
            *(int *)((char *)self->w + off + 0x8F4) = 0;
        }
    }
    func_08885220(D_089D69E8);
    func_08873D34((float)func_088D4C5C(), 3, func_08ABF760, 0, 1, 2);
    func_08873DEC(3, 2);
    *(int *)((char *)self->w + 0xA5C) = func_08ABC868(self);
}
