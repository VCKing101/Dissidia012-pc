typedef struct Vec8 { float v[8]; } Vec8;
typedef struct Filter { char pad_00[0x20]; Vec8 field_20; } Filter;
extern Vec8 D_09AF84C0;
extern unsigned long long D_089C9A68;
extern void func_089A3868(Vec8 *dst, Vec8 *src);
extern void func_08871BD0(Filter *self, unsigned long long mask);
void func_08872B04(Filter *self)
{
    func_089A3868(&self->field_20, &D_09AF84C0);
    func_08871BD0(self, D_089C9A68);
}
