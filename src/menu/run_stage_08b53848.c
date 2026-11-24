/* By the stage at +0x28: stage 1 runs func_08B538C8 then func_08B5391C, stages 2 and 3 run func_08B538C8. */
extern void func_08B538C8(int *self);
extern void func_08B5391C(int *self);
void func_08B53848(int *self)
{
    switch (self[0x28 / 4]) {
    case 1:
        func_08B538C8(self);
        func_08B5391C(self);
        break;
    case 2:
        func_08B538C8(self);
        break;
    case 3:
        func_08B538C8(self);
        break;
    }
}
