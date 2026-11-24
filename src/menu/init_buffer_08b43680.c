/* Stores the heap handle, allocates a 0x20004-byte block from it, clears its first word and runs the next init step. */
extern int func_0885C6CC(int handle, int size, int a, int b, int c, int d);
extern void func_08B436FC(int *self);
void func_08B43680(int *self, int handle)
{
    int one = 1;
    int *block;
    self[0] = handle;
    block = (int *)func_0885C6CC(handle, 0x20004, 0, 0, one, 0);
    self[1] = (int)block;
    block[0] = 0;
    func_08B436FC(self);
}
