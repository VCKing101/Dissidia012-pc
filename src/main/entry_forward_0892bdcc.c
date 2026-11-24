/* Forward the call to the indexed 0x44-byte entry when the index is in range. */
extern void func_0892B37C(int entry, int a, int b, int c, int d, int e);
void func_0892BDCC(int self, int index, unsigned char a, int b, int c, int d, int e)
{
    if (index < 0 || index >= *(int *)(self + 0x198)) {
        return;
    }
    func_0892B37C(self + index * 0x44 + 0x44, a, b, c, d, e);
}
