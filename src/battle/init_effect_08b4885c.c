/* Initialises a sub-object at +4 and sets the colour fields (0, 0, 0, 1) at +0x70, then runs func_08B488E8. */
extern void func_08864FF0(void *p, int a, int b, int c);
extern void func_08B488E8(void *self);
typedef struct { char pad[0x70]; float c[4]; } Obj;
Obj *func_08B4885C(Obj *self) {
    func_08864FF0((char *)self + 4, 2, 0x11173, 0x57);
    self->c[0] = 0.0f;
    self->c[1] = 0.0f;
    self->c[2] = 0.0f;
    self->c[3] = 1.0f;
    func_08B488E8(self);
    return self;
}
