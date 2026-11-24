/* Runs the guard and reset, then opens two handles (kinds 0xA and 1) from the owner at +0x1B4 into +4 and +8 and marks it ready. */
typedef struct PSelf { char pad[4]; void *h1; void *h2; char pad_0c[0x198]; unsigned char ready; char pad_1a5[0xF]; int owner; } PSelf;
extern int func_08B2A1A8(PSelf *self);
extern void func_08B2A220(PSelf *self);
extern void *func_088DAA40(int owner, int kind, int z);
int func_08B2A1B0(PSelf *self)
{
    if (func_08B2A1A8(self) == 0) {
        return -1;
    }
    func_08B2A220(self);
    self->h1 = func_088DAA40(self->owner, 10, 0);
    self->h2 = func_088DAA40(self->owner, 1, 0);
    self->ready = 1;
    return 0;
}
