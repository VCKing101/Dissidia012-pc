extern "C" {
extern int D_08B5BDA0;
extern int D_08B59D28;
extern int D_08B57E50;
extern int D_08B5D528;
void func_08864C48(void *p, int flags);
void func_08992F04(void *p);
}
struct BaseA { int field_00; int field_04; void *vtable; static void destroy(BaseA *p) { if (p != 0) { p->vtable = &D_08B5BDA0; } } };
struct BaseB { int field_00; int field_04; void *vtable; static void destroy(BaseB *p) { if (p != 0) { p->vtable = &D_08B59D28; } } };
struct BaseC { int field_00; int field_04; void *vtable; static void destroy(BaseC *p) { if (p != 0) { p->vtable = &D_08B57E50; } } };
struct BaseD { int field_00; int field_04; void *vtable; char field_0C[4];
  static void destroy(BaseD *p) { if (p != 0) { p->vtable = &D_08B5D528; func_08864C48(p->field_0C, 2); } } };
struct Multi {
    char pad_00[0x1B0];
    BaseD d;
    char pad_1C0[0xF0];
    BaseC c;
    char pad_2BC[0x4F4];
    BaseB b;
    char pad_7BC[0x8D4];
    BaseA a;
    static void destroy(Multi *self, unsigned int flags)
    {
        if (self != 0) {
            BaseA::destroy(&self->a);
            BaseB::destroy(&self->b);
            BaseC::destroy(&self->c);
            BaseD::destroy(&self->d);
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};
extern "C" void func_0887A0EC(Multi *self, unsigned int flags)
{
    Multi::destroy(self, flags);
}
