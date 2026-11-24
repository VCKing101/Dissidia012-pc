// If func_08B4A39C(self, 1, 0) succeeds, runs func_08B4A14C(self, 0, 0), sets the word at +0x6E0 to 2 and returns 1; else returns 0.
extern int func_08B4A39C(void *, int, int);
extern void func_08B4A14C(void *, int, int);
typedef struct { char pad[0x6E0]; int state; } Obj08B49E80;
int func_08B49E80(Obj08B49E80 *self)
{
    int one = 1;
    if (func_08B4A39C(self, 1, 0) == 0) {
        return 0;
    }
    func_08B4A14C(self, 0, 0);
    self->state = 2;
    return one;
}
