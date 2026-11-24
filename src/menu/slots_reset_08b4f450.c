/* Initialises the 4 + 2 slot entries (0x40 bytes each), runs the setup call and stores two values and state 2. */
typedef struct S4F450 { char pad[0x28]; int state; char pad2[0x194]; int f1C0; int f1C4; } S4F450;
extern void func_08B4ED40(char *slot);
extern void func_08B4F6DC(S4F450 *self);

void func_08B4F450(S4F450 *self, int a, int b)
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
    self->f1C0 = a;
    self->f1C4 = b;
    self->state = 2;
}
