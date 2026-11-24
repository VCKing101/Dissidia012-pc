/* Runs guard 08B2BD98 and reset 08B2BE00, then opens the handle (kind 9) from the owner at +0x18 into +4; -1 guard failed, -2 open failed. */
typedef struct OSelf { char pad[4]; void *handle; } OSelf;
extern int func_08B2BD98(OSelf *self);
extern void func_08B2BE00(OSelf *self);
extern void *func_088DAA40(int owner, int kind, int z);
int func_08B2BDA0(OSelf *self)
{
    if (func_08B2BD98(self) == 0) {
        return -1;
    }
    func_08B2BE00(self);
    self->handle = func_088DAA40(*(int *)((char *)self + 0x18), 9, 0);
    if (self->handle == 0) {
        return -2;
    }
    return 0;
}
