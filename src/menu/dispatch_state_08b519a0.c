/* Calls the update function for the state at +0x14 (0..4). */
extern void func_08B51A30(int *self);
extern void func_08B51A38(int *self);
extern void func_08B51BD8(int *self);
extern void func_08B51CC0(int *self);
extern void func_08B51CFC(int *self);
void func_08B519A0(int *self)
{
    switch (self[0x14 / 4]) {
    case 0:
        func_08B51A30(self);
        break;
    case 1:
        func_08B51A38(self);
        break;
    case 2:
        func_08B51BD8(self);
        break;
    case 3:
        func_08B51CC0(self);
        break;
    case 4:
        func_08B51CFC(self);
        break;
    }
}
