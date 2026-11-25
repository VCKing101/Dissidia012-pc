/* Clears four 16-byte blocks and a float vector at +0x40, returns self. */
typedef struct S { char p[0x40]; float a, b, c, d; } S;
extern void func_08935828(void *p, int v, int n);
S *func_0895AB68(S *self) {
    func_08935828(self, 0, 0x10);
    func_08935828((char *)self + 0x10, 0, 0x10);
    func_08935828((char *)self + 0x20, 0, 0x10);
    func_08935828((char *)self + 0x30, 0, 0x10);
    self->a = 0.0f; self->b = 0.0f; self->c = 0.0f;
    self->d = 0.0f;
    return self;
}
