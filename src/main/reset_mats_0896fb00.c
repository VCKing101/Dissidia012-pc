/* Clears three 16-byte blocks and sets the scale at +0x230 to 1.0. */
typedef struct S { char p[0x230]; float scale; } S;
extern void func_08935828(void *p, int v, int n);
void func_0896FB00(S *self) {
    func_08935828((char *)self + 0x20, 0, 0x10);
    func_08935828((char *)self + 0x50, 0, 0x10);
    func_08935828((char *)self + 0x60, 0, 0x10);
    self->scale = 1.0f;
}
