/* Starts a fade of n steps (at least 1) on an active object: sets step 1.0 and 1/n, flag bit 3 on, bits 0 and 2 off. */
typedef struct S { char p[4]; int mark; char q[0x18]; float a; float b; char r[4]; unsigned int flags; } S;
int func_089923B8(S *self, int n) {
    unsigned int fl = self->flags;
    if ((fl & 1) == 0) return -1;
    if (self->mark < 0) return -1;
    if (n < 1) n = 1;
    self->a = 1.0f;
    self->b = 1.0f / (float)n;
    self->flags = ((fl & ~1) & ~0xC) | 8;
    return 0;
}
