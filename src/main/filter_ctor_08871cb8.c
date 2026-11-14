typedef struct Mask64 { int field_00; } Mask64;
extern int D_089C96B0;
extern unsigned long long D_089C9A68;
extern void func_08871B40(void *self, int a, unsigned long long mask);
typedef struct Filter { char pad_00[0x34]; void *vtable; } Filter;
Filter *func_08871CB8(Filter *self, int a)
{
    self->vtable = &D_089C96B0;
    func_08871B40(self, a, D_089C9A68);
    return self;
}
