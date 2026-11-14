extern int func_08AB6BEC(int self, unsigned char kind, int value);

unsigned char func_08AB5F38(int self)
{
    if (func_08AB6BEC(self, 0x10, 0) == 1) {
        return 1;
    }
    return 0;
}
