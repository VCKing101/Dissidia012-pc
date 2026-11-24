/* Picks the next state of an active effect holder from its pending handles and starts the matching worker. */
typedef struct Holder {
    char pad0[0x24]; void *h24; void *h28; void *h2c; void *h30; int a34; int a38; char v3c[0x5C - 0x3C];
    int mode; int state; char pad1[0x68 - 0x64]; int a68; int a6c; unsigned char s70; unsigned char s71; unsigned char s72; unsigned char s73;
    char pad2[0x88 - 0x74]; char v88[4];
} Holder;
extern int func_08AF772C(void *);
extern void func_08AF66C0(void *);
extern void func_08AF70C0(void *);
extern void func_08AF5DBC(void *, int, int, int, void *, int, int, void *);
extern void func_08AF62EC(void *);
void func_08AF3154(Holder *self, int arg) {
    int one = 1;
    int two;
    if (self->s72 == one) {
        two = 2;
        if (self->h28 != 0 && func_08AF772C(self->h28) == one) {
            self->state = 4;
        } else if (self->h2c != 0) {
            func_08AF66C0(self->h2c);
            self->state = two;
        } else if (self->h30 != 0) {
            func_08AF70C0(self->h30);
            self->state = 3;
        } else if (self->h24 != 0) {
            func_08AF5DBC(self->h24, arg, self->a34, self->a38, self->v3c, self->a68, self->a6c, self->v88);
            func_08AF62EC(self->h24);
            self->state = one;
        } else {
            self->s73 = 1;
            self->state = 0;
        }
        self->mode = two;
    }
}
