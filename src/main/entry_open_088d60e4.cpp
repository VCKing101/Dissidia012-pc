/* Returns true when the table entry for the given id exists, the global lock byte is clear and the entry's bit is unset. */
struct Ent { int id; int pad[3]; int bit; };
extern Ent D_089D7BE0[];
extern unsigned *D_089D726C;
extern unsigned char D_089D5F1F;
struct Bits {
    static bool Test(int n) { unsigned *b = D_089D726C; unsigned w = b[(unsigned)n >> 5]; return (w & (1 << (n & 0x1F))) != 0; }
};
extern "C" bool func_088D60E4(int id)
{
    Ent *p = D_089D7BE0;
    bool r = false;
    int cur = p->id;
    int neg = -1;
    unsigned char lock = D_089D5F1F;
    for (;;) {
        if (cur == neg) {
            break;
        }
        if (cur == id && lock == 0) {
            if (!Bits::Test(p->bit)) {
                r = true;
            }
            break;
        }
        p++;
        cur = p->id;
    }
    return r;
}
