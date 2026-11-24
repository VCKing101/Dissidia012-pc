/* If the flag byte at +0xCC of the object at +0xC4 is set, clears it and runs func_08B3C768, func_08864DB8 and func_08B3C6CC. */
typedef struct O { char pad[0xCC]; unsigned char flag; } O;
typedef struct S { char pad[0xC4]; O *o; } S;
extern void func_08B3C768(S *self);
extern void func_08864DB8(void);
extern void func_08B3C6CC(S *self);
void func_08B3C678(S *self)
{
    if (!self->o->flag) return;
    self->o->flag = 0;
    func_08B3C768(self);
    func_08864DB8();
    func_08B3C6CC(self);
}
