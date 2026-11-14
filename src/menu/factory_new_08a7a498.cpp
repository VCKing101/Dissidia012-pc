/* Factories: new (heap) T for nine menu classes (out-of-line or inline constructors).
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

void func_08A7CB48(Object *self);
void func_08A7AAE4(Object *self);
void func_08A7E3C0(Object *self);
extern char D_08A79960[];
extern char D_08A778C8[];
extern char D_08A79A98[];
extern char D_08A79C00[];
extern char D_08A7EB18[];
extern char D_08A7AA20[];
void func_08A74260(Object *member);

Object *func_08A7A498(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x230, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        func_08A7CB48(self);
        made = self;
    }
    return made;
}

Object *func_08A7A4EC(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0xBC, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        func_08A7AAE4(self);
        made = self;
    }
    return made;
}

Object *func_08A7A540(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x6C, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        func_08A7E3C0(self);
        made = self;
    }
    return made;
}

Object *func_08A7A594(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x8, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A79960;
        made = self;
    }
    return made;
}

Object *func_08A7A5E4(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x4, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A778C8;
        made = self;
    }
    return made;
}

Object *func_08A7A634(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x8, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A79A98;
        made = self;
    }
    return made;
}

Object *func_08A7A684(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x8, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A79C00;
        made = self;
    }
    return made;
}

Object *func_08A7A6D4(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x8, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A7EB18;
        made = self;
    }
    return made;
}

Object *func_08A7A724(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x18, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A7AA20;
        func_08A74260(self + 1);
        made = self;
    }
    return made;
}

}
