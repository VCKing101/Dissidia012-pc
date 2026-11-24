/* Calls func_08B537E0 on the owner's field +0x40; if it passes, runs func_08B4C20C on field +0x38 and sets the state word at +0xB4 to 4. */
typedef struct O { char pad[0x38]; int f38; int pad2; int f40; } O;
typedef struct S { char pad[0xB4]; int state; char pad2[0xC8 - 0xB8]; O *o; } S;
extern int func_08B537E0(int v);
extern void func_08B4C20C(int v);
void func_08B37CEC(S *self)
{
    if (!func_08B537E0(self->o->f40)) return;
    func_08B4C20C(self->o->f38);
    self->state = 4;
}
