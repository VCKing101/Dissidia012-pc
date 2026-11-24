/* Calls one of two functions on three sub-objects depending on their flag bytes, then fires two others in a flag-dependent order. */
typedef struct Tp { char pad0[8]; int o8; int oC; int o10; char pad1[0x40 - 0x14]; int f40; int f44; unsigned char b48, b49, b4A; } Tp;
extern void func_088D9CD8(int);
extern void func_088D9CFC(int);
extern void func_088D9F5C(int, int, int);
extern void func_0887B924(int);
extern void func_0887B944(int);
void func_08A5CAE0(Tp *self) {
    if (self->b48) {
        func_088D9CD8(self->o10);
    } else {
        func_088D9CFC(self->o10);
    }
    if (self->b49) {
        func_088D9CD8(self->oC);
    } else {
        func_088D9CFC(self->oC);
    }
    if (self->b4A) {
        func_088D9F5C(self->o8, 8, 1);
        func_0887B924(self->f40);
        func_0887B944(self->f44);
    } else {
        func_088D9F5C(self->o8, 7, 1);
        func_0887B944(self->f40);
        func_0887B924(self->f44);
    }
}
