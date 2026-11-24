/* Returns 1 only when all four handle checks on the object at +0xC8 pass. */
extern int func_0892AE30(int);
typedef struct { int w[0x34 / 4]; } Obj;
typedef struct { char d[0xC8]; Obj *obj; } Self;
int func_08B3BD9C(Self *self)
{
    int ok = 1;
    if (!func_0892AE30(self->obj->w[0x14 / 4])) ok = 0;
    if (!func_0892AE30(self->obj->w[0x1C / 4])) ok = 0;
    if (!func_0892AE30(self->obj->w[0x30 / 4])) ok = 0;
    if (!func_0892AE30(self->obj->w[0x18 / 4])) ok = 0;
    return ok;
}
