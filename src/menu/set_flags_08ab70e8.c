/* Updates the two flag words by their bits: starts the effect when bit 1 is set and bit 2 clear; otherwise sets 0x80000000 or (with the id) 0x8000000. */
extern int D_089D727C;
extern char D_08AB43D4[];
extern void func_088B1194(int g, int id, void *t, int a, int b);
void func_08AB70E8(int self, int id) {
    unsigned w = *(unsigned *)(self + 0x30);
    if ((w & 1) != 0) {
        if ((w & 2) == 0) {
            *(unsigned *)(self + 0x30) = w | 2;
            *(int *)(self + 0x28) = id;
            switch (id) {
            case -0x7FBEF4FD:
                break;
            case -0x7FBEF4F7:
                break;
            case -0x7FBEF4F1:
                break;
            default:
                break;
            }
            func_088B1194(D_089D727C, id, D_08AB43D4, 0, 0xA0);
        }
    } else {
        w = *(unsigned *)(self + 0x2C);
        if ((w & 0x20000000) == 0) {
            *(unsigned *)(self + 0x2C) = w | 0x80000000;
        } else {
            *(int *)(self + 0x28) = id;
            *(unsigned *)(self + 0x2C) = w | 0x8000000;
        }
    }
}
