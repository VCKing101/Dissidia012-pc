typedef struct Filter { char pad_00[0x44]; unsigned int flags; } Filter;
extern unsigned long long D_089C9A68;
extern void func_08871B40(void *self, int a, unsigned long long mask);
void func_08872510(Filter *self)
{
    self->flags |= 1;
    func_08871B40(self, self->flags, D_089C9A68);
}
