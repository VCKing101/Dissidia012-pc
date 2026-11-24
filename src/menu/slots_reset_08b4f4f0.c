/* Initialises the 4 + 2 slot entries (0x40 bytes each), runs the setup call and stores three values and state 3. */
typedef struct S4F4F0 { char pad[0x28]; int state; char pad2[0x190]; int f1BC; int f1C0; int f1C4; } S4F4F0;
extern void func_08B4ED40(char *slot);
extern void func_08B4F6DC(S4F4F0 *self);

void func_08B4F4F0(S4F4F0 *self, int a, int b, int c)
{
    char *p = (char *)self;
    int i = 0;
    p += 0x3C;
    for (; i < 4; i++, p += 0x40) {
        func_08B4ED40(p);
    }
    for (i = 0, p = (char *)self + 0x13C; i < 2; i++, p += 0x40) {
        func_08B4ED40(p);
    }
    func_08B4F6DC(self);
    self->f1BC = a;
    self->f1C0 = b;
    self->f1C4 = c;
    self->state = 3;
}
