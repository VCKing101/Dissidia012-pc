/* If the object at +0xD0 has a handle at +0xC, releases it with func_088A6C70 and clears it. */
typedef struct O { char pad[0xC]; int h; } O;
typedef struct S { char pad[0xD0]; O *o; } S;
extern void func_088A6C70(int h);
void func_08B3C448(S *self)
{
    if (self->o->h != 0) {
        func_088A6C70(self->o->h);
        self->o->h = 0;
    }
}
