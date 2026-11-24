/* If func_08B4C218 accepts the owner's field +0x38, calls func_08B53734 with the owner's fields and sets the state word at +0xB4 to 3. */
typedef struct O { int f0; char pad[0x38 - 4]; int f38; int pad2; int f40; } O;
typedef struct S { char pad[0x38]; int f38; char pad2[0xB4 - 0x3C]; int state; char pad3[0xC8 - 0xB8]; O *o; } S;
extern int func_08B4C218(int v);
extern void func_08B53734(int a, int b, int c, int d);
void func_08B37C90(S *self)
{
    if (!func_08B4C218(self->o->f38)) return;
    func_08B53734(self->o->f40, self->o->f0, 7, self->f38);
    self->state = 3;
}
