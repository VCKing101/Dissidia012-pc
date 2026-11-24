/* Runs the step routine for the current state (1..3), then the shared update. */
extern void func_08B4C384(void *self);
extern void func_08B4C330(void *self);
extern void func_08B4C3D4(void *self);
extern void func_08B4C47C(void *self);

void func_08B4C2A0(int *self)
{
    switch (self[0x28 / 4]) {
    case 1:
        func_08B4C384(self);
        func_08B4C330(self);
        break;
    case 2:
        func_08B4C3D4(self);
        func_08B4C330(self);
        break;
    case 3:
        func_08B4C47C(self);
        func_08B4C330(self);
        break;
    }
}
