/* Resets the record: clears state fields, the 3 value words, the 4 + 2 slot entries and the tail words. */
typedef struct S4F270 { char pad[0x28]; int state; int f2C; int v[3]; char pad2[0x180]; int f1BC; int f1C0; int f1C4; } S4F270;
extern void func_0892AC2C(void);
extern void func_08B4ECF0(char *slot);

void func_08B4F270(S4F270 *self)
{
    int i;
    char *p;
    func_0892AC2C();
    self->state = 0;
    self->f2C = 0;
    for (i = 0; i < 3; i++) {
        self->v[i] = 0;
    }
    for (i = 0, p = (char *)self + 0x3C; i < 4; i++, p += 0x40) {
        func_08B4ECF0(p);
    }
    for (i = 0, p = (char *)self + 0x13C; i < 2; i++, p += 0x40) {
        func_08B4ECF0(p);
    }
    self->f1BC = 0;
    self->f1C0 = 0;
    self->f1C4 = 0;
}
