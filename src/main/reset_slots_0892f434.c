/* Run the base reset, clear the field tables and reset the entry, then clear the trailing fields. */
extern void func_0892AC2C(void);
extern void func_0892EBA4(int e);
void func_0892F434(int self)
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
    } while (i < 2);
    i = 0;
    p = self;
    do {
        *(int *)(p + 0x34) = 0;
        i++;
        p += 4;
    } while (i < 1);
    i = 0;
    p = self;
    do {
        *(int *)(p + 0x38) = 0;
        i++;
        p += 4;
    } while (i < 1);
    i = 0;
    q = self + 0x3C;
    do {
        func_0892EBA4(q);
        i++;
        q += 0x10C;
    } while (i < 1);
    *(int *)(self + 0x148) = 0;
    *(int *)(self + 0x14C) = 0;
    *(int *)(self + 0x150) = 0;
}
