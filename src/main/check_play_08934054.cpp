// Plays effect 0x65 unless flag 0x800 at +0x384 or bit 4 of the byte at +0x193 is set; then calls func_08834900 with 0.
struct S { char p[0x384]; unsigned int flags; };
struct T { char p[0x190]; char x[3]; unsigned char b; };
extern "C" void func_0882FAC4(void *sub, int id, int z, float a, float b);
extern "C" void func_08834900(void *self, float f);
extern "C" float D_089C7C30;
struct H {
    static bool has(unsigned int v, unsigned int m) { return (v & m) != 0; }
    static bool hasb(unsigned char *p, int m) { return (p[3] & m) != 0; }
};
extern "C" void func_08934054(int unused, S *s) {
    char *self = (char *)s;
    float zero = 0.0f;
    if (!H::has(s->flags, 0x800)) {
        unsigned char *b = (unsigned char *)(self + 0x190);
        if (!H::hasb(b, 0x10)) {
            func_0882FAC4(self + 0x130, 0x65, 0, D_089C7C30, zero);
        }
    }
    func_08834900(self, zero);
}
