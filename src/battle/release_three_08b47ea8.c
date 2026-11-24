/* Calls func_088CF970 on the global with fields 0x30, 0x2C and 0x34 of the object. */
struct Obj { char pad[0x2C]; void *a; void *b; void *c; };
extern void *D_089D7830;
extern void func_088CF970(void *, void *);
void func_08B47EA8(struct Obj *self)
{
    void *v;
    v = self->b;
    func_088CF970(D_089D7830, v);
    v = self->a;
    func_088CF970(D_089D7830, v);
    v = self->c;
    func_088CF970(D_089D7830, v);
}
