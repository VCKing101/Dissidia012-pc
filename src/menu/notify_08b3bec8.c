/* Resolves the optional name through func_08922F7C (or -1) and forwards it with the owner's fields to func_08B52880; returns 1. */
typedef struct O { int first; char pad[0x2C - 4]; int f2c; } O;
typedef struct S { char pad[0xC8]; O *o; } S;
extern int func_08922F7C(int x, int y);
extern void func_08B52880(int a, int b, int c, int d);
int func_08B3BEC8(S *self, int x)
{
    int id = -1;
    if (x) {
        id = func_08922F7C(x, id);
    }
    func_08B52880(self->o->f2c, self->o->first, 6, id);
    return 1;
}
