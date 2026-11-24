// Count the table entries whose state byte is 1.
extern "C" int func_088C90C0(int self, int i);
extern "C" int func_088C913C(int self)
{
    int n = 0;
    for (int i = 0; i < *(int *)(self + 0xBDA8); i++) {
        int e = func_088C90C0(self, i);
        if (e == 0) {
            continue;
        }
        if (*(unsigned char *)(e + 0x20) != 1) {
            continue;
        }
        n++;
    }
    return n;
}
