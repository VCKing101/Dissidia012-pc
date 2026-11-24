/* Returns 0 when the child object's word at 0x708 is zero, else the result of func_08B2E550. */
struct Inner { char pad[0x708]; int v; };
struct Outer { char pad[0xA18]; struct Inner *in; };
extern int func_08B2E550(void);
int func_08B2A73C(struct Outer *self)
{
    if (self->in->v == 0) { return 0; } else { return func_08B2E550(); }
}
