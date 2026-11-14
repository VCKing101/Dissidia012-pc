/* Factories: new (heap) T for a 0x88-byte class with a member at +4.
   Each allocates through the heap allocator and runs T's inline constructor.
   Retail keeps the new-expression's null result in the frame (0($sp)) across
   the allocation call instead of an s-register. One-element arrays are never
   promoted by SNC -O2, so `result[1]` reproduces the slot; `one` and `none`
   are the named temporaries the inlined operator new leaves (both `ori`). */
extern "C" void *func_0885C6CC(void *heap, unsigned size, int align, int offset, int kind, int flags);

struct Object {
    void *vtable;
};

extern "C" {

extern char D_08A7E370[];
void func_08A76F08(Object *member);

Object *func_08A7A180(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x88, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A7E370;
        func_08A76F08(self + 1);
        made = self;
    }
    return made;
}

}
