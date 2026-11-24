/* By the stage at +0x28: stage 1 runs func_08B53250, stage 2 runs func_08B53250 then func_08B532A4. */
extern void func_08B53250(int *self);
extern void func_08B532A4(int *self);
void func_08B531F0(int *self)
{
    switch (self[0x28 / 4]) {
    case 1:
        func_08B53250(self);
        break;
    case 2:
        func_08B53250(self);
        func_08B532A4(self);
        break;
    }
}
