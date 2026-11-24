/* Sets this record bit in one of two bitmaps of the object from func_0891705C, chosen by flag 0x10000. */
struct Rec { short pad0; short index; int pad1[2]; unsigned flags; };
struct Obj { int pad; Rec *rec; };
struct Util {
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern char *func_0891705C(int);
void func_08B44E90(Obj *self)
{
    unsigned bit = 0x10000;
    int one = 1;
    if (Util::Has(self->rec->flags, bit)) {
        char *s = func_0891705C(-1);
        unsigned idx = self->rec->index;
        unsigned *p = (unsigned *)(s + (idx >> 5) * 4 + 0x110);
        *p = *p | one << (idx & 0x1F);
    } else {
        char *s = func_0891705C(-1);
        unsigned idx = self->rec->index;
        unsigned *p = (unsigned *)(s + (idx >> 5) * 4 + 0x90);
        *p = *p | one << (idx & 0x1F);
    }
}
}
