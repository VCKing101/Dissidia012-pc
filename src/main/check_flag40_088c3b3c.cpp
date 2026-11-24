// Looks up an entry from func_088B9004(base+0xE788, *key); sets out[1]=1 and out[0]/return = whether the entry has flag bit 6.
struct E { unsigned char b0, b1, b2, b3; static bool has40(E *e) { return (e->b3 & 0x40) != 0; } };
extern "C" E *func_088B9004(char *, int);
extern "C" int func_088C3B3C(int *out, char *base, int *key)
{
    E *e = func_088B9004(base + 0xE788, *key);
    int one = 1;
    out[1] = one;
    if (e != 0 && E::has40(e) == one) {
        out[0] = one;
        return 1;
    }
    out[0] = 0;
    return 0;
}
