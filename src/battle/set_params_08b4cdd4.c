/* Sets four parameters of entry `index` (0x14 bytes each, from +0xD0), flagging changes in +0xE0 bits 0-3 and bit 4 when the third changes sign. */
struct Param { int a; int b; int c; int d; unsigned flags; };
struct Obj { char pad[0xD0]; struct Param p[1]; };
void func_08B4CDD4(struct Obj *self, int index, int a, int b, int c, int d)
{
    struct Param *p = &self->p[index];
    int old = p->c;
    if (p->a != a) {
        p->a = a;
        p->flags = p->flags | 1;
    }
    if (p->b != b) {
        p->b = b;
        p->flags = p->flags | 2;
    }
    if (p->c != c) {
        p->c = c;
        p->flags = p->flags | 4;
        old = c;
    }
    if (p->d != d) {
        p->d = d;
        p->flags = p->flags | 8;
    }
    if ((old < 0 && c >= 0) || (old >= 0 && c < 0)) {
        p->flags = p->flags | 0x10;
    }
}
