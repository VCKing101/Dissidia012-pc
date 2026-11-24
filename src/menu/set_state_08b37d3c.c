/* If func_08B4C248 accepts the object at +0xC8's field +0x38, sets the state word at +0xB4 to 5. */
typedef struct O { char pad[0x38]; int f; } O;
typedef struct S { char pad[0xB4]; int state; char pad2[0xC8 - 0xB8]; O *o; } S;
extern int func_08B4C248(int f);
void func_08B37D3C(S *self)
{
    if (!func_08B4C248(self->o->f)) return;
    self->state = 5;
}
