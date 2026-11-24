/* Increments the signed byte at 0x2A1E; while it is below 4 calls func_0891C124(self, counter), and when that gave 0 (or the counter reached 4) sets bit 2 of field 0x2A44 and calls func_08889808(D_089D6A10). */
typedef struct Obj0891c4f0 { unsigned char pad0[0x2A1E]; signed char n; unsigned char pad1[0x2A44 - 0x2A1F]; unsigned flags; } Obj0891c4f0;
extern int D_089D6A10;
extern int func_0891C124(Obj0891c4f0 *o, int n);
extern void func_08889808(int v);
void func_0891C4F0(Obj0891c4f0 *self)
{
    int r = 0;
    self->n = self->n + 1;
    if (self->n < 4) {
        r = func_0891C124(self, self->n);
    }
    if (r == 0) {
        self->flags = self->flags | 2;
        func_08889808(D_089D6A10);
    }
}
