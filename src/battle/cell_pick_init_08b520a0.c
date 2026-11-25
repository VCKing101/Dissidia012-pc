// Picks one of three cells with an inline LCG (0 when no manager), then initialises each cell: picked one via func_08B51FB8, others via func_08B51F44.
struct R08B520A0 { int pad; int f4; };
struct O08B520A0 { int seed; char cells[0x24 - 4 + 0]; int pick; int f28; int f2c; float f30; };
extern struct R08B520A0 *func_08B52348(void);
extern void func_08B519C4(void *c);
extern void func_08B51F44(void *c);
extern void func_08B51FB8(void *c);
void func_08B520A0(struct O08B520A0 *self)
{
    struct R08B520A0 *r = func_08B52348();
    int pick;
    self->f2c = 0;
    if (r == 0 || r->f4 == 0) {
        pick = 0;
    } else {
        self->seed = self->seed * 0x343FD + 0x269EC3;
        pick = (self->seed >> 16 & 0x7FFF) % 3;
    }
    self->pick = pick;
    self->f28 = 0;
    self->f30 = 0.0f;
    {
        int i;
        char *c;
        for (i = 0, c = (char *)self + 0xC; i < 3; i++, c += 8) {
            if (i == self->pick) {
                func_08B51FB8(c);
            } else {
                func_08B51F44(c);
            }
            func_08B519C4(c);
        }
    }
}
