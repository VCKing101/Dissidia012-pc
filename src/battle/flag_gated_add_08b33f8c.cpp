// Adds a delta to a counter unless the object is locked, depending on two input flags.
extern "C" int func_08842828(int, int, int);
extern "C" void func_0884EA08(void *, int, int);
struct Sub { char d[8]; unsigned char f; char pad[7]; };
struct Obj {
    char d0[0x2F8]; int h; char d1[0x2B4]; Sub s; char d2[0x730]; char t[1];
};
struct Flags {
    static unsigned char Has(int v, int m) { int t = (v & m) != 0; unsigned char r = t; return r; }
};
extern "C" void func_08B33F8C(Obj *self, int add, unsigned char a, unsigned char b) {
    Sub *p = &self->s;
    int fl = p->f;
    unsigned char locked = Flags::Has(fl, 0x80);
    if (locked == 1) return;
    int c = (fl & 0x20) != 0;
    add = func_08842828(self->h, 1, c) + add;
    if ((c && b) || (!c && a) || (!a && !b)) {
        func_0884EA08(self->t, add, 1);
    }
}
