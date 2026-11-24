// When all 20 consecutive bits from the base index are set in the global bitset, sets the following bit.
extern "C" {
extern unsigned short D_08A8B67E[];
extern unsigned int *D_089D726C;
}
struct Bits {
    static bool Has(unsigned id) { return (D_089D726C[id >> 5] & (1 << (id & 0x1F))) != 0; }
};
extern "C" void func_08A8A108(void) {
    int i = 0;
    unsigned base = D_08A8B67E[0];
    unsigned *w;
    unsigned id;
    do {
        if (!Bits::Has(base + i)) {
            return;
        }
        i++;
    } while (i < 0x14);
    id = D_08A8B67E[1];
    w = &D_089D726C[id >> 5];
    *w |= 1 << (id & 0x1F);
}
