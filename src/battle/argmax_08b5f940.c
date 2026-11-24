/* Finds the 0x10-byte entry (count at +0x140) with the largest float at +0xC and returns its index. */
struct Ent { int a; int b; int c; float f; };
struct Tbl { struct Ent e[20]; int count; };
int func_08B5F940(struct Tbl *self)
{
    int best = 0;
    int i = 0;
    int n = self->count;
    if (i < n) {
        struct Ent *p = self->e;
        do {
            if (self->e[best].f < p->f) {
                best = i;
            }
            i++;
            p++;
        } while (i < n);
    }
    return best;
}
