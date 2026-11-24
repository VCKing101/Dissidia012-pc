/* Resets the object, rolls a random 15-bit seed into +4 and marks it active. */
typedef struct Obj { unsigned char state; char pad[3]; unsigned seed; } Obj;
extern int D_09AF84D0;
extern void func_08B015DC(void);

int func_08B015F8(Obj *self)
{
    func_08B015DC();
    self->seed = (D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    self->state = 1;
    return 0;
}
