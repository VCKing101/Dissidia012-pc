/* Run the base reset, clear the four and six word tables and set the five halfword quads to -1. */
extern void func_08931DA0(void);
void func_089323EC(int self)
{
    int i;
    int p;
    func_08931DA0();
    i = 0;
    p = self;
    do {
        *(int *)(p + 0x10) = 0;
        i++;
        p += 4;
    } while (i < 4);
    i = 0;
    p = self;
    do {
        *(int *)(p + 0x20) = 0;
        i++;
        p += 4;
    } while (i < 6);
    i = 0;
    do {
        *(unsigned short *)(self + 0x38) = 0xFFFF;
        *(unsigned short *)(self + 0x3A) = 0xFFFF;
        *(unsigned short *)(self + 0x3C) = 0xFFFF;
        *(unsigned short *)(self + 0x3E) = 0xFFFF;
        i++;
        self += 8;
    } while (i < 5);
}
