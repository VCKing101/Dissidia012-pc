/* Like func_0899243C but needs bit 1 set; starts a countdown: step 0, 1/n, bit 2 on, bits 1 and 3 off. */
typedef struct S { char p[4]; int mark; char q[0x18]; float a; float b; char r[4]; unsigned int flags; } S;
int func_089924C8(S *self, int n) {
    unsigned int fl = self->flags;
    if ((fl & 1) == 0) return -1;
    if (self->mark < 0) return -1;
    if (n < 1) n = 1;
    if ((fl & 2) != 0) {
        self->flags = ((fl & ~2) & ~0xC) | 4;
        self->a = 0.0f;
        self->b = 1.0f / (float)n;
        return 0;
    }
    return -1;
}
