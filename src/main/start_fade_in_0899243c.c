/* Like func_089923B8 but refuses when bit 1 is set, and sets bits 1 and 3 (clearing bits 0, 2 of the old value except bit 0). */
typedef struct S { char p[4]; int mark; char q[0x18]; float a; float b; char r[4]; unsigned int flags; } S;
int func_0899243C(S *self, int n) {
    unsigned int fl = self->flags;
    if ((fl & 1) == 0) return -1;
    if (self->mark < 0) return -1;
    if (n < 1) n = 1;
    if ((fl & 2) != 0) return -1;
    self->a = 1.0f;
    self->b = 1.0f / (float)n;
    self->flags = (fl & ~0xC) | 0xA;
    return 0;
}
