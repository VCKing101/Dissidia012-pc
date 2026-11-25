/* constructor: clear memory blocks and scalar fields. */
extern void func_08935828(void *p, int v, int n);
typedef struct Obj {
    char b0[0x10]; char b10[0x10]; char b20[0x10];
    float f30;
    char b34[8]; char b3C[8]; char b44[8];
    float f4C;
    int a50, a54;
    char c58, c59, c5A, c5B, c5C;
    float f60, f64;
    int a68;
} Obj;
Obj *func_0898D51C(Obj *self)
{
    func_08935828(self->b0, 0, 0x10);
    func_08935828(self->b10, 0, 0x10);
    func_08935828(self->b20, 0, 0x10);
    self->f30 = 0.0f;
    func_08935828(self->b34, 0, 8);
    func_08935828(self->b3C, 0, 8);
    func_08935828(self->b44, 0, 8);
    self->f4C = 0.0f;
    self->a50 = 0;
    self->a54 = 0;
    self->c58 = 0;
    self->c59 = 0;
    self->c5A = 0;
    self->c5B = 0;
    self->c5C = 0;
    self->f60 = 0.0f;
    self->f64 = 0.0f;
    self->a68 = 0;
    return self;
}
