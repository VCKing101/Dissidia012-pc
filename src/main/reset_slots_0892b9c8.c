/* Run the base reset, clear the fields, the four and two word tables and reset the five 0x44-byte entries. */
extern void func_0892AC2C(void);
extern void func_0892B0EC(int e);
void func_0892B9C8(int self)
{
    int i;
    int p;
    int q;
    func_0892AC2C();
    *(int *)(self + 0x28) = 0;
    i = 0;
    p = self;
    do {
        *(int *)(p + 0x2C) = 0;
        i++;
        p += 4;
    } while (i < 4);
    i = 0;
    p = self;
    do {
        *(int *)(p + 0x3C) = 0;
        i++;
        p += 4;
    } while (i < 2);
    i = 0;
    q = self + 0x44;
    do {
        func_0892B0EC(q);
        i++;
        q += 0x44;
    } while (i < 5);
    *(int *)(self + 0x198) = 0;
    *(int *)(self + 0x19C) = 0;
    *(int *)(self + 0x1A0) = 0;
}
