/* Increments the signed byte at 0x2A1F; while it is below 4 calls func_0891C1FC(self, counter), and when that gave 0 (or the counter reached 4) calls func_0891C330(self). */
typedef struct Obj0891c634 { unsigned char pad[0x2A1F]; signed char n; } Obj0891c634;
extern int func_0891C1FC(Obj0891c634 *o, int n);
extern void func_0891C330(Obj0891c634 *o);
void func_0891C634(Obj0891c634 *self)
{
    int r = 0;
    self->n = self->n + 1;
    if (self->n < 4) {
        r = func_0891C1FC(self, self->n);
    }
    if (r == 0) {
        func_0891C330(self);
    }
}
