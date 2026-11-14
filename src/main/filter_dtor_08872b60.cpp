/* Deleting destructor of the filter (vtable pointer at 0x34). */
struct Filter;

extern "C" {
void func_08992F04(Filter *self);
extern int D_089C96B0;
}

struct Filter {
    char pad_00[0x34];
    void *vtable;

    static void destroy(Filter *self, unsigned int flags, void *vt)
    {
        if (self != 0) {
            self->vtable = vt;
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {

void func_08872B60(Filter *self, unsigned int flags)
{
    Filter::destroy(self, flags, &D_089C96B0);
}

}
