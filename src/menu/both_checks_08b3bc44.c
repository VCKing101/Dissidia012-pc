/* Returns 1 only when both sub-checks on the object at +0xC8 succeed. */
extern int func_089312D8(int);
extern int func_08B507E8(int);
typedef struct { char d[0x14]; int a; char e[0x18]; int b; } Obj;
typedef struct { char d[0xC8]; Obj *obj; } Self;
int func_08B3BC44(Self *self)
{
    int ok = 1;
    if (!func_089312D8(self->obj->a)) ok = 0;
    if (!func_08B507E8(self->obj->b)) ok = 0;
    return ok;
}
