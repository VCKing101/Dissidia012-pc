// If the short at +0x106A is non-zero, calls func_08833534 on the object and clears it.
extern void func_08833534(void *);
typedef struct { char pad[0x106A]; short state; } Obj08B35018;
void func_08B35018(Obj08B35018 *self)
{
    if (self->state == 0) {
        return;
    }
    func_08833534(self);
    self->state = 0;
}
