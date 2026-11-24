/* When self->0x30 is set, calls func_08B51240(self, self->0xEC) and sets self->0x28 to 1. */
struct Self { char pad[0x28]; int state; char pad2[0x30 - 0x2C]; int link; char pad3[0xEC - 0x34]; int arg; };
extern void func_08B51240(struct Self *self, int arg);
void func_08B5087C(struct Self *self)
{
    if (self->link == 0) { return; }
    func_08B51240(self, self->arg);
    self->state = 1;
}
