// Reads func_08B493B0(self+0x58): 2 sets the word at +0x518 to 3, 3 sets the word at +0x28 to 2.
extern int func_08B493B0(void *);
typedef struct { char pad0[0x28]; int w28; char pad1[0x58 - 0x2C]; char sub[0x518 - 0x58]; int w518; } Obj08B481D4;
void func_08B481D4(Obj08B481D4 *self)
{
    switch (func_08B493B0(self->sub)) {
    case 0:
    case 1:
        break;
    case 3:
        self->w28 = 2;
        break;
    case 2:
        self->w518 = 3;
        break;
    }
}
