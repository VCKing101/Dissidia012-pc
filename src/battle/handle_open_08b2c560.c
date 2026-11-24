/* Runs guard 08B2C558 and reset 08B2C5C0, then opens the handle (kind 0x14) from the owner at +0x1C into +4; -1 guard failed, -2 open failed. */
typedef struct OSelf { char pad[4]; void *handle; } OSelf;
extern int func_08B2C558(OSelf *self);
extern void func_08B2C5C0(OSelf *self);
extern void *func_088DAA40(int owner, int kind, int z);
int func_08B2C560(OSelf *self)
{
    if (func_08B2C558(self) == 0) {
        return -1;
    }
    func_08B2C5C0(self);
    self->handle = func_088DAA40(*(int *)((char *)self + 0x1C), 0x14, 0);
    if (self->handle == 0) {
        return -2;
    }
    return 0;
}
